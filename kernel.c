#include "kernel.h"
#include "common.h"

// 获取链接器脚本符号
extern char __bss[], __bss_end[], __stack_top[];

// TODO: 改成宏以省掉函数栈帧开销
// 或改为 inline，例如 https://git.musl-libc.org/cgit/musl/tree/arch/riscv64/syscall_arch.h
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid)
{
    register long a0 __asm__("a0") = arg0; // 将arg0的值赋给寄存器a0
    register long a1 __asm__("a1") = arg1; // 将arg1的值赋给寄存器a1
    register long a2 __asm__("a2") = arg2; // 将arg2的值赋给寄存器a2
    register long a3 __asm__("a3") = arg3; // 将arg3的值赋给寄存器a3
    register long a4 __asm__("a4") = arg4; // 将arg4的值赋给寄存器a4
    register long a5 __asm__("a5") = arg5; // 将arg5的值赋给寄存器a5
    register long a6 __asm__("a6") = fid;  // 将fid的值赋给寄存器a6
    register long a7 __asm__("a7") = eid;  // 将eid的值赋给寄存器a7

    __asm__ __volatile__(    // volatile防止编译器优化
        "ecall"              // 执行 ecall 指令，发起 SBI 调用
        : "=r"(a0), "=r"(a1) // 输出部分，将 a0 和 a1 寄存器的值作为输出
        : "r"(a0), "r"(a1),  // 输入部分，将 a0 到 a7 寄存器的值作为输入
          "r"(a2), "r"(a3),
          "r"(a4), "r"(a5),
          "r"(a6), "r"(a7)
        : "memory" // clobber list，告知编译器 ecall 指令可能会修改内存
    );
    return (struct sbiret){.error = a0, .value = a1};
}

void putchar(char ch)
{
    sbi_call(ch, 0, 0, 0, 0, 0, 0, 1 /* Console Putchar */);
}

void kernel_main(void)
{
    // .bss 段初始化
    memset(__bss, 0, (size_t)__bss_end - (size_t)__bss);

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