#include "common.h"
#include "kernel.h"
#define PROCS_MAX 8 // 最大进程数量

#define PROC_UNUSED 0   // 未使用的进程控制结构
#define PROC_RUNNABLE 1 // 可运行的进程

struct process
{
    int pid;              // 进程 ID
    int state;            // 进程状态: PROC_UNUSED 或 PROC_RUNNABLE
    vaddr_t sp;           // 栈指针
    uint32_t *page_table; // 页表
    uint8_t stack[8192];  // 内核栈
};

__attribute__((naked)) void switch_context(uint32_t *prev_sp,
                                           uint32_t *next_sp)
{
    __asm__ __volatile__(
        // 将被调用者保存寄存器保存到当前进程的栈上
        "addi sp, sp, -13 * 4\n" // 为13个4字节寄存器分配栈空间
        "sw ra,  0  * 4(sp)\n"   // 仅保存被调用者保存的寄存器
        "sw s0,  1  * 4(sp)\n"
        "sw s1,  2  * 4(sp)\n"
        "sw s2,  3  * 4(sp)\n"
        "sw s3,  4  * 4(sp)\n"
        "sw s4,  5  * 4(sp)\n"
        "sw s5,  6  * 4(sp)\n"
        "sw s6,  7  * 4(sp)\n"
        "sw s7,  8  * 4(sp)\n"
        "sw s8,  9  * 4(sp)\n"
        "sw s9,  10 * 4(sp)\n"
        "sw s10, 11 * 4(sp)\n"
        "sw s11, 12 * 4(sp)\n"

        // 切换栈指针
        "sw sp, (a0)\n" // *prev_sp = sp;
        "lw sp, (a1)\n" // 在这里切换栈指针(sp)

        // 从下一个进程的栈中恢复被调用者保存的寄存器
        "lw ra,  0  * 4(sp)\n" // 仅恢复被调用者保存的寄存器
        "lw s0,  1  * 4(sp)\n"
        "lw s1,  2  * 4(sp)\n"
        "lw s2,  3  * 4(sp)\n"
        "lw s3,  4  * 4(sp)\n"
        "lw s4,  5  * 4(sp)\n"
        "lw s5,  6  * 4(sp)\n"
        "lw s6,  7  * 4(sp)\n"
        "lw s7,  8  * 4(sp)\n"
        "lw s8,  9  * 4(sp)\n"
        "lw s9,  10 * 4(sp)\n"
        "lw s10, 11 * 4(sp)\n"
        "lw s11, 12 * 4(sp)\n"
        "addi sp, sp, 13 * 4\n" // 我们已从栈中弹出13个4字节寄存器
        "ret\n");
}

struct process procs[PROCS_MAX]; // 所有进程控制结构

struct process *create_process(uint32_t pc)
{
    // 查找未使用的进程控制结构
    struct process *proc = NULL;
    int i;
    for (i = 0; i < PROCS_MAX; i++)
    {
        if (procs[i].state == PROC_UNUSED)
        {
            proc = &procs[i];
            break;
        }
    }

    if (!proc)
        PANIC("no free process slots");

    // 设置被调用者保存的寄存器。这些寄存器值将在 switch_context
    // 中的第一次上下文切换时被恢复。
    uint32_t *sp = (uint32_t *)&proc->stack[sizeof(proc->stack)];
    *--sp = 0;            // s11
    *--sp = 0;            // s10
    *--sp = 0;            // s9
    *--sp = 0;            // s8
    *--sp = 0;            // s7
    *--sp = 0;            // s6
    *--sp = 0;            // s5
    *--sp = 0;            // s4
    *--sp = 0;            // s3
    *--sp = 0;            // s2
    *--sp = 0;            // s1
    *--sp = 0;            // s0
    *--sp = (uint32_t)pc; // ra

    // 映射内核页面。
    uint32_t *page_table = (uint32_t *)alloc_pages(1);
    for (paddr_t paddr = (paddr_t)__kernel_base;
         paddr < (paddr_t)__free_ram_end; paddr += PAGE_SIZE)
        map_page(page_table, paddr, paddr, PAGE_R | PAGE_W | PAGE_X);

    // 初始化字段
    proc->pid = i + 1;
    proc->state = PROC_RUNNABLE;
    proc->page_table = page_table;
    proc->sp = (uint32_t)sp;
    return proc;
}

//
// 调度器
//

/** 当前运行的进程 */
struct process *current_proc;
/** 空闲进程 */
struct process *idle_proc;

/** 主动让出 CPU 给另一个进程 */
void yield(void)
{
    // 搜索可运行的进程
    struct process *next = idle_proc;
    for (int i = 0; i < PROCS_MAX; i++)
    {
        struct process *proc = &procs[(current_proc->pid + i) % PROCS_MAX];
        if (proc->state == PROC_RUNNABLE && proc->pid > 0)
        {
            next = proc;
            break;
        }
    }

    // 如果除了当前进程外没有可运行的进程，返回并继续处理
    if (next == current_proc)
        return;

    __asm__ __volatile__(
        // 启用分页
        "sfence.vma\n"
        "csrw satp, %[satp]\n"
        "sfence.vma\n"
        // 进程切换期间在 sscratch 寄存器中设置当前执行进程的内核栈的初始值
        "csrw sscratch, %[sscratch]\n"
        :
        : [satp] "r"(SATP_SV32 | ((uint32_t)next->page_table / PAGE_SIZE)) // 读入 SATP_SV32配置+分页号
          ,
          [sscratch] "r"((uint32_t)&next->stack[sizeof(next->stack)]) // 读入内核栈
    );

    // 上下文切换
    struct process *prev = current_proc;
    current_proc = next;
    switch_context(&prev->sp, &next->sp);
    // 这里的关键点是每个进程都有自己独立的内核栈
    // 通过在上下文切换期间切换sscratch的内容，我们可以从进程被中断的点恢复执行
}