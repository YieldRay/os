typedef int bool;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;
typedef uint32_t size_t;
/** 表示物理内存地址 */
typedef uint32_t paddr_t;
/** 表示虚拟内存地址 */
typedef uint32_t vaddr_t;

#define true 1
#define false 0
#define NULL ((void *)0)

// https://clang.llvm.org/docs/LanguageExtensions.html#builtin-functions

/** 将 value 向上舍入到 align 的最近倍数。align 必须是 2 的幂 */
#define align_up(value, align) __builtin_align_up(value, align)
/** 检查 value 是否是 align 的倍数。align 必须是 2 的幂 */
#define is_aligned(value, align) __builtin_is_aligned(value, align)
/** 返回结构体中成员的偏移量（距离结构体开始的字节数） */
#define offsetof(type, member) __builtin_offsetof(type, member)
#define va_list __builtin_va_list
#define va_start __builtin_va_start
#define va_end __builtin_va_end
#define va_arg __builtin_va_arg

// std

void *memset(void *buf, char c, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
char *strcpy(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
void printf(const char *fmt, ...);

// system call
#define SCAUSE_ECALL 8
#define SYS_PUTCHAR 1
#define SYS_GETCHAR 2
#define SYS_EXIT 3