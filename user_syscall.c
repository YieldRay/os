#include "common.h"

/**
 * linux实现：https://git.musl-libc.org/cgit/musl/tree/arch/riscv64/syscall_arch.h
 */
int syscall(int sysno, int arg0, int arg1, int arg2)
{
    register int a0 __asm__("a0") = arg0;
    register int a1 __asm__("a1") = arg1;
    register int a2 __asm__("a2") = arg2;
    register int a3 __asm__("a3") = sysno;

    __asm__ __volatile__("ecall"
                         : "=r"(a0)
                         : "r"(a0), "r"(a1), "r"(a2), "r"(a3)
                         : "memory" // 告知编译器可能会修改内存
    );

    return a0;
}

void putchar(char ch)
{
    syscall(SYS_PUTCHAR, ch, 0, 0);
}

int getchar(void)
{
    return syscall(SYS_GETCHAR, 0, 0, 0);
}

__attribute__((noreturn)) void exit(void)
{
    syscall(SYS_EXIT, 0, 0, 0);
    for (;;)
        ;
}

int readfile(const char *filename, char *buf, int len)
{
    return syscall(SYS_READFILE, (int)filename, (int)buf, len);
}

int writefile(const char *filename, const char *buf, int len)
{
    return syscall(SYS_WRITEFILE, (int)filename, (int)buf, len);
}