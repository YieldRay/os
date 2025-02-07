struct sbiret
{
    long error;
    long value;
};

/**
 * 参考 linux 实现：https://github.com/torvalds/linux/blob/master/arch/riscv/kernel/sbi_ecall.c
 * @see https://courses.stephenmarz.com/my-courses/cosc562/risc-v/opensbi-calls/
 * TODO：是否可以改成宏或inline？
 */
struct sbiret sbi_call(long arg0, long arg1, long arg2, long arg3, long arg4,
                       long arg5, long fid, long eid)
{
    register long a0 __asm__("a0") = arg0; // 将 arg0 的值赋给寄存器 a0
    register long a1 __asm__("a1") = arg1; // 将 arg1 的值赋给寄存器 a1
    register long a2 __asm__("a2") = arg2; // 将 arg2 的值赋给寄存器 a2
    register long a3 __asm__("a3") = arg3; // 将 arg3 的值赋给寄存器 a3
    register long a4 __asm__("a4") = arg4; // 将 arg4 的值赋给寄存器 a4
    register long a5 __asm__("a5") = arg5; // 将 arg5 的值赋给寄存器 a5
    register long a6 __asm__("a6") = fid;  // 将 fid 的值赋给寄存器 a6
    register long a7 __asm__("a7") = eid;  // 将 eid 的值赋给寄存器 a7

    __asm__ __volatile__(    // volatile 防止编译器优化
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

long getchar(void)
{
    struct sbiret ret = sbi_call(0, 0, 0, 0, 0, 0, 0, 2);
    return ret.error;
}