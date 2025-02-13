#pragma once
#include "common.h"

// 获取链接器脚本符号
extern char __bss[], __bss_end[], __stack_top[];
extern char __free_ram[], __free_ram_end[];
extern char __kernel_base[];

extern char _binary_shell_bin_start[], _binary_shell_bin_size[];

/** 表示在 kernel_entry 中保存的程序状态 */
struct trap_frame
{
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed)); // packed 属性告诉编译器不要插入任何填充字节

#define READ_CSR(reg)                                         \
    ({                                                        \
        unsigned long __tmp;                                  \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp)); \
        __tmp;                                                \
    })

#define WRITE_CSR(reg, value)                                   \
    do                                                          \
    {                                                           \
        uint32_t __tmp = (value);                               \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp)); \
    } while (0)

#define PANIC(fmt, ...)                                                       \
    do                                                                        \
    {                                                                         \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__); \
        for (;;)                                                              \
            ;                                                                 \
                                                                              \
    } while (0)

// 分页实现
#define PAGE_SIZE 4096
paddr_t alloc_pages(uint32_t n);

#define SATP_SV32 (1u << 31)
#define SSTATUS_SUM (1 << 18)
#define PAGE_V (1 << 0) // "Valid" 位（表项已启用）
#define PAGE_R (1 << 1) // 可读
#define PAGE_W (1 << 2) // 可写
#define PAGE_X (1 << 3) // 可执行
#define PAGE_U (1 << 4) // 用户（用户模式可访问）

void map_page(uint32_t *table1, uint32_t vaddr, paddr_t paddr, uint32_t flags);

// 应用程序镜像的基础虚拟地址。与 `user.ld` 中定义的起始地址匹配
#define USER_BASE 0x1000000
#define SSTATUS_SPIE (1 << 5)