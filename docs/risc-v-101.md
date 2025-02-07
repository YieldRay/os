[QEMU virt 虚拟机](https://www.qemu.org/docs/master/system/riscv/virt.html)

# CPU 模式

-   M-mode: (machine) OpenSBI（即 BIOS）运行的模式
-   S-mode: (supervisor) 内核运行的模式，又称“内核模式”
-   U-mode: (user) 应用运行的模式，又称“用户模式”

# CSR 特权指令

[The RISC-V Instruction Set Manual: Volume II: Privileged Architecture](https://michaelliao.github.io/riscv-isa/privileged.html)

**CSR (Control and Status Register, 控制和状态寄存器)** 是一系列特殊寄存器，用于：

-   **控制 (Control):** 配置 CPU 的操作模式，例如中断使能、内存管理单元 (MMU) 设置、安全模式等。
-   **状态 (Status):** 保存 CPU 的当前状态信息，例如中断标志、异常状态、错误代码等。

例如：

-   `mstatus` (Machine Status): 保存 M-mode 的状态信息，如中断使能、特权级别等。
-   `mie` (Machine Interrupt Enable): 控制 M-mode 的中断使能。
-   `mtvec` (Machine Trap Vector): 指定 M-mode 异常处理程序的入口地址。
-   `mepc` (Machine Exception Program Counter): 保存发生异常时的程序计数器 (PC)。
-   `mscratch` (Machine Scratch Register): 一个供 M-mode 使用的临时寄存器。
-   `sepc` (Supervisor Exception Program Counter): 保存发生异常时的程序计数器 (PC)。
-   `sscratch` (Supervisor Scratch Register): 一个供 S-mode 使用的临时寄存器。

# 内联汇编

```
asm asm-qualifiers ( AssemblerTemplate
                 : OutputOperands
                 [ : InputOperands
                 [ : Clobbers ] ])
```

<https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html>  
<https://en.cppreference.com/w/c/language/asm>  
<https://www.ibiblio.org/gferg/ldp/GCC-Inline-Assembly-HOWTO.html>  
<https://gcc.gnu.org/onlinedocs/gnat_ugn/Inline-Assembler.html>

```c
// csrr 指令读取 sepc 的值，到 value 变量
uint32_t value;
__asm__ __volatile__("csrr %0, sepc" : "=r"(value));
```

```c
// csrw 指令写入 字面量123 到 sscratch
__asm__ __volatile__("csrw sscratch, %0" : : "r"(123));
```
