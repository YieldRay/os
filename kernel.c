#include "common.h"
#include "kernel.h"
#include "kernel_memory.c"
#include "kernel_sbi.c"
#include "kernel_process.c"

// 获取链接器脚本符号
extern char __bss[], __bss_end[], __stack_top[];

__attribute__((naked))      // 不要生成函数序言/结尾代码
__attribute__((aligned(4))) // 确保函数地址 4 字节对齐
void
kernel_entry(void)
{
    __asm__ __volatile__(

        "csrrw sp, sscratch, sp\n" // 从 sscratch 中获取运行进程的内核栈

        "addi sp, sp, -4 * 31\n" // 在栈上分配空间，用于保存 31 个寄存器 (每个寄存器 4 字节)

        // 保存通用寄存器到栈上
        "sw ra,  4 * 0(sp)\n"  // 保存 ra (返回地址) 到 sp + 0
        "sw gp,  4 * 1(sp)\n"  // 保存 gp (全局指针) 到 sp + 4
        "sw tp,  4 * 2(sp)\n"  // 保存 tp (线程指针) 到 sp + 8
        "sw t0,  4 * 3(sp)\n"  // 保存 t0 (临时寄存器 0) 到 sp + 12
        "sw t1,  4 * 4(sp)\n"  // 保存 t1 (临时寄存器 1) 到 sp + 16
        "sw t2,  4 * 5(sp)\n"  // 保存 t2 (临时寄存器 2) 到 sp + 20
        "sw t3,  4 * 6(sp)\n"  // 保存 t3 (临时寄存器 3) 到 sp + 24
        "sw t4,  4 * 7(sp)\n"  // 保存 t4 (临时寄存器 4) 到 sp + 28
        "sw t5,  4 * 8(sp)\n"  // 保存 t5 (临时寄存器 5) 到 sp + 32
        "sw t6,  4 * 9(sp)\n"  // 保存 t6 (临时寄存器 6) 到 sp + 36
        "sw a0,  4 * 10(sp)\n" // 保存 a0 (参数/返回值 0) 到 sp + 40
        "sw a1,  4 * 11(sp)\n" // 保存 a1 (参数/返回值 1) 到 sp + 44
        "sw a2,  4 * 12(sp)\n" // 保存 a2 (参数/返回值 2) 到 sp + 48
        "sw a3,  4 * 13(sp)\n" // 保存 a3 (参数/返回值 3) 到 sp + 52
        "sw a4,  4 * 14(sp)\n" // 保存 a4 (参数/返回值 4) 到 sp + 56
        "sw a5,  4 * 15(sp)\n" // 保存 a5 (参数/返回值 5) 到 sp + 60
        "sw a6,  4 * 16(sp)\n" // 保存 a6 (参数/返回值 6) 到 sp + 64
        "sw a7,  4 * 17(sp)\n" // 保存 a7 (参数/返回值 7) 到 sp + 68
        "sw s0,  4 * 18(sp)\n" // 保存 s0 (保存的寄存器 0) 到 sp + 72
        "sw s1,  4 * 19(sp)\n" // 保存 s1 (保存的寄存器 1) 到 sp + 76
        "sw s2,  4 * 20(sp)\n" // 保存 s2 (保存的寄存器 2) 到 sp + 80
        "sw s3,  4 * 21(sp)\n" // 保存 s3 (保存的寄存器 3) 到 sp + 84
        "sw s4,  4 * 22(sp)\n" // 保存 s4 (保存的寄存器 4) 到 sp + 88
        "sw s5,  4 * 23(sp)\n" // 保存 s5 (保存的寄存器 5) 到 sp + 92
        "sw s6,  4 * 24(sp)\n" // 保存 s6 (保存的寄存器 6) 到 sp + 96
        "sw s7,  4 * 25(sp)\n" // 保存 s7 (保存的寄存器 7) 到 sp + 100
        "sw s8,  4 * 26(sp)\n" // 保存 s8 (保存的寄存器 8) 到 sp + 104
        "sw s9,  4 * 27(sp)\n" // 保存 s9 (保存的寄存器 9) 到 sp + 108
        "sw s10, 4 * 28(sp)\n" // 保存 s10 (保存的寄存器 10) 到 sp + 112
        "sw s11, 4 * 29(sp)\n" // 保存 s11 (保存的寄存器 11) 到 sp + 116

        // 获取并保存异常发生时的 sp
        "csrr a0, sscratch\n"
        "sw a0,  4 * 30(sp)\n"

        // 重置内核栈
        "addi a0, sp, 4 * 31\n"
        "csrw sscratch, a0\n"

        "mv a0, sp\n"        // 将当前的 sp 值传递给 a0 作为 handle_trap 的参数
        "call handle_trap\n" // 调用 handle_trap 函数来处理中断

        // 从栈上恢复通用寄存器
        "lw ra,  4 * 0(sp)\n"  // 从 sp + 0 恢复 ra
        "lw gp,  4 * 1(sp)\n"  // 从 sp + 4 恢复 gp
        "lw tp,  4 * 2(sp)\n"  // 从 sp + 8 恢复 tp
        "lw t0,  4 * 3(sp)\n"  // 从 sp + 12 恢复 t0
        "lw t1,  4 * 4(sp)\n"  // 从 sp + 16 恢复 t1
        "lw t2,  4 * 5(sp)\n"  // 从 sp + 20 恢复 t2
        "lw t3,  4 * 6(sp)\n"  // 从 sp + 24 恢复 t3
        "lw t4,  4 * 7(sp)\n"  // 从 sp + 28 恢复 t4
        "lw t5,  4 * 8(sp)\n"  // 从 sp + 32 恢复 t5
        "lw t6,  4 * 9(sp)\n"  // 从 sp + 36 恢复 t6
        "lw a0,  4 * 10(sp)\n" // 从 sp + 40 恢复 a0
        "lw a1,  4 * 11(sp)\n" // 从 sp + 44 恢复 a1
        "lw a2,  4 * 12(sp)\n" // 从 sp + 48 恢复 a2
        "lw a3,  4 * 13(sp)\n" // 从 sp + 52 恢复 a3
        "lw a4,  4 * 14(sp)\n" // 从 sp + 56 恢复 a4
        "lw a5,  4 * 15(sp)\n" // 从 sp + 60 恢复 a5
        "lw a6,  4 * 16(sp)\n" // 从 sp + 64 恢复 a6
        "lw a7,  4 * 17(sp)\n" // 从 sp + 68 恢复 a7
        "lw s0,  4 * 18(sp)\n" // 从 sp + 72 恢复 s0
        "lw s1,  4 * 19(sp)\n" // 从 sp + 76 恢复 s1
        "lw s2,  4 * 20(sp)\n" // 从 sp + 80 恢复 s2
        "lw s3,  4 * 21(sp)\n" // 从 sp + 84 恢复 s3
        "lw s4,  4 * 22(sp)\n" // 从 sp + 88 恢复 s4
        "lw s5,  4 * 23(sp)\n" // 从 sp + 92 恢复 s5
        "lw s6,  4 * 24(sp)\n" // 从 sp + 96 恢复 s6
        "lw s7,  4 * 25(sp)\n" // 从 sp + 100 恢复 s7
        "lw s8,  4 * 26(sp)\n" // 从 sp + 104 恢复 s8
        "lw s9,  4 * 27(sp)\n" // 从 sp + 108 恢复 s9
        "lw s10, 4 * 28(sp)\n" // 从 sp + 112 恢复 s10
        "lw s11, 4 * 29(sp)\n" // 从 sp + 116 恢复 s11

        "lw sp,  4 * 30(sp)\n" // 从栈上恢复之前的 sp 值

        "sret\n" // 从中断返回
    );
}

void handle_trap(struct trap_frame *f)
{
    uint32_t scause = READ_CSR(scause);
    uint32_t stval = READ_CSR(stval);
    uint32_t user_pc = READ_CSR(sepc);

    PANIC("unexpected trap scause=%x, stval=%x, sepc=%x\n", scause, stval, user_pc);
}

void kernel_main(void)
{
    // .bss 段初始化
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

    WRITE_CSR(stvec, (uint32_t)kernel_entry); // 写入 Supervisor Trap Vector Base Address Register

    printf("Hello, %s!\n", "RISC-V");

    for (;;)
    {
        __asm__ __volatile__("wfi");
    }
}

__attribute__((section(".text.boot"))) // 放置在入口点
__attribute__((naked))                 // 确保内联汇编代码就是确切的函数体
void
boot(void)
{
    __asm__ __volatile__(
        "mv sp, %[stack_top]\n" // 设置栈指针
        "j kernel_main\n"       // 跳转到内核主函数
        :
        : [stack_top] "r"(__stack_top) // 将栈顶地址作为 %[stack_top] 传递
    );
}