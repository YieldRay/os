#include "user.h"
#include "user_syscall.c"

extern char __stack_top[];

__attribute__((noreturn)) void exit(void)
{
    for (;;)
        ;
}

__attribute__((section(".text.start")))
__attribute__((naked)) void
start(void)
{
    __asm__ __volatile__(
        "mv sp, %[stack_top] \n"
        "call main           \n"
        "call exit           \n" ::[stack_top] "r"(__stack_top));
}

void main(void)
{

    uint8_t *shell_bin = (uint8_t *)_binary_shell_bin_start;
    printf("shell_bin size = %d\n", (int)_binary_shell_bin_size);
    printf("shell_bin[0] = %x (%d bytes)\n", shell_bin[0]);
    for (;;)
        ;
}