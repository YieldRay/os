<https://asm-docs.microagi.org/risc-v/riscv-asm.html>

## 版权和许可信息

《RISC-V 汇编程序员手册》

© 2017 Palmer Dabbelt <palmer@dabbelt.com>
© 2017 Michael Clark <michaeljclark@mac.com>
© 2017 Alex Bradbury <asb@lowrisc.org>

本作品采用知识共享署名 4.0 国际许可协议 (CC-BY 4.0) 授权。完整许可文本见 https://creativecommons.org/licenses/by/4.0/。

## 范围

本文档旨在为面向标准 RISC-V 汇编语言的汇编程序员提供指导，该汇编语言受到 GNU as 和 LLVM 的汇编器等常见开源汇编器的支持。其他汇编器可能不支持相同的指令或伪指令；它们的方言不在本文档的范围之内。

## 命令行参数

我认为加强 binutils 文档可能比在此处重复它更好。

## 寄存器

寄存器是任何处理器最重要的组成部分。RISC-V 定义了各种类型，具体取决于包含哪些扩展：通用寄存器（带有程序计数器）、控制寄存器、浮点寄存器（F 扩展）和向量寄存器（V 扩展）。

## 通用寄存器

RV32I 基本整数 ISA 包括 32 个寄存器，命名为 `x0` 到 `x31`。与 ARM-32 等其他处理器相比，程序计数器 `PC` 与这些寄存器是分开的。第一个寄存器 `x0` 具有特殊功能：读取它总是返回 0，并且对其的写入将被忽略。正如我们稍后将看到的，这允许各种技巧和简化。

实际上，程序员不使用此表示法来表示寄存器。尽管就处理器而言，`x1` 到 `x31` 都是同样通用的寄存器，但按照惯例，某些寄存器用于特殊任务。在汇编器中，它们被赋予标准化名称，作为 RISC-V **应用程序二进制接口** (ABI) 的一部分。这通常是你在代码清单中看到的。如果你真的想看到数字寄存器名称，可以使用 `objdump` 的 `-M` 参数来提供它们。

| 寄存器 | ABI        | 按惯例的用途               | 是否保留？ |
| ------ | ---------- | -------------------------- | ---------- |
| x0     | zero       | 硬连线到 0，忽略写入       | _不适用_   |
| x1     | ra         | 跳转的返回地址             | 否         |
| x2     | sp         | 堆栈指针                   | 是         |
| x3     | gp         | 全局指针                   | _不适用_   |
| x4     | tp         | 线程指针                   | _不适用_   |
| x5     | t0         | 临时寄存器 0               | 否         |
| x6     | t1         | 临时寄存器 1               | 否         |
| x7     | t2         | 临时寄存器 2               | 否         |
| x8     | s0 _或_ fp | 保存的寄存器 0 _或_ 帧指针 | 是         |
| x9     | s1         | 保存的寄存器 1             | 是         |
| x10    | a0         | 返回值 _或_ 函数参数 0     | 否         |
| x11    | a1         | 返回值 _或_ 函数参数 1     | 否         |
| x12    | a2         | 函数参数 2                 | 否         |
| x13    | a3         | 函数参数 3                 | 否         |
| x14    | a4         | 函数参数 4                 | 否         |
| x15    | a5         | 函数参数 5                 | 否         |
| x16    | a6         | 函数参数 6                 | 否         |
| x17    | a7         | 函数参数 7                 | 否         |
| x18    | s2         | 保存的寄存器 2             | 是         |
| x19    | s3         | 保存的寄存器 3             | 是         |
| x20    | s4         | 保存的寄存器 4             | 是         |
| x21    | s5         | 保存的寄存器 5             | 是         |
| x22    | s6         | 保存的寄存器 6             | 是         |
| x23    | s7         | 保存的寄存器 7             | 是         |
| x24    | s8         | 保存的寄存器 8             | 是         |
| x25    | s9         | 保存的寄存器 9             | 是         |
| x26    | s10        | 保存的寄存器 10            | 是         |
| x27    | s11        | 保存的寄存器 11            | 是         |
| x28    | t3         | 临时寄存器 3               | 否         |
| x29    | t4         | 临时寄存器 4               | 否         |
| x30    | t5         | 临时寄存器 5               | 否         |
| x31    | t6         | 临时寄存器 6               | 否         |
| pc     | _(无)_     | 程序计数器                 | _不适用_   |

_RV32I 的寄存器。基于 RISC-V 文档和 Patterson and Waterman 的 "The RISC-V Reader" (2017)_

一般来说，**保存的寄存器** `s0` 到 `s11` 在函数调用之间被保留，而**参数寄存器** `a0` 到 `a7` 和**临时寄存器** `t0` 到 `t6` 则不保留。各种专用寄存器（如 `sp`）的惯例用法将在稍后更详细地讨论。

## 控制寄存器

（待定）

## 浮点寄存器 (RV32F)

（待定）

## 向量寄存器 (RV32V)

（待定）

## 寻址

像 %pcrel_lo() 这样的寻址格式。我们可以链接到 RISC-V PS ABI 文档来描述重定位的实际作用。

## 指令集

官方规范网页：- https\://riscv.org/specifications/

最新规范草案仓库：- https\://github.com/riscv/riscv-isa-manual

## 指令

## RISC-V ISA 规范

https\://riscv.org/specifications/

## 指令别名

来自 opcodes/riscv-opc.c 的 ALIAS 行

为了更好地诊断程序流程到达意外位置的情况，你可能希望在那里发出一个已知会陷入陷阱的指令。你可以使用 `UNIMP` 伪指令，它应该在几乎所有系统中陷入陷阱。该指令的 _事实上的_ 标准实现是：

-   `C.UNIMP`: `0000`。全零模式不是有效指令。因此，任何在无效指令上陷入陷阱的系统都将在这种 `UNIMP` 指令形式上陷入陷阱。尽管不是有效指令，但它仍然符合 16 位（压缩）指令格式，因此 `0000 0000` 被解释为两个 16 位 `UNIMP` 指令。

-   `UNIMP` : `C0001073`。这是 `CSRRW x0, cycle, x0` 的别名。由于 `cycle` 是一个只读 CSR，那么（无论此 CSR 是否存在）尝试写入它将生成非法指令异常。当面向没有 C 扩展的系统，或者使用 `.option norvc` 指令时，将发出这种 32 位形式的 `UNIMP`。

## 伪操作

包括特定于 RISC-V 的选项和以 GNU .- 开头的选项。

下表列出了汇编器指令：

| 指令         | 参数                                              | 描述                                                                                                |
| ------------ | ------------------------------------------------- | --------------------------------------------------------------------------------------------------- |
| .align       | 整数                                              | 对齐到 2 的幂（`.p2align` 的别名，首选使用 - 参见 [.align](https://asm-docs.microagi.org/#.align)） |
| .p2align     | p2,\[pad_val=0],max                               | 对齐到 2 的幂                                                                                       |
| .balign      | b,\[pad_val=0]                                    | 字节对齐                                                                                            |
| .file        | "文件名"                                          | 发出文件名 FILE LOCAL 符号表                                                                        |
| .globl       | 符号\_名称                                        | 将符号\_名称发出到符号表（范围 GLOBAL）                                                             |
| .local       | 符号\_名称                                        | 将符号\_名称发出到符号表（范围 LOCAL）                                                              |
| .comm        | 符号\_名称,大小,对齐                              | 将公共对象发出到 .bss 段                                                                            |
| .common      | 符号\_名称,大小,对齐                              | 将公共对象发出到 .bss 段                                                                            |
| .ident       | "字符串"                                          | 为源兼容性而接受                                                                                    |
| .section     | \[{.text,.data,.rodata,.bss}]                     | 发出段（如果不存在，则默认为 .text）并使其成为当前段                                                |
| .size        | 符号, 符号                                        | 为源兼容性而接受                                                                                    |
| .text        |                                                   | 发出 .text 段（如果不存在）并使其成为当前段                                                         |
| .data        |                                                   | 发出 .data 段（如果不存在）并使其成为当前段                                                         |
| .rodata      |                                                   | 发出 .rodata 段（如果不存在）并使其成为当前段                                                       |
| .bss         |                                                   | 发出 .bss 段（如果不存在）并使其成为当前段                                                          |
| .string      | "字符串"                                          | 发出字符串                                                                                          |
| .asciz       | "字符串"                                          | 发出字符串（.string 的别名）                                                                        |
| .equ         | 名称, 值                                          | 常量定义                                                                                            |
| .macro       | 名称 arg1 \[, argn]                               | 开始宏定义 \argname 用于替换                                                                        |
| .endm        |                                                   | 结束宏定义                                                                                          |
| .type        | 符号, @function                                   | 为源兼容性而接受                                                                                    |
| .option      | {arch,rvc,norvc,pic,nopic,relax,norelax,push,pop} | RISC-V 选项。有关更详细的描述，请参阅 [.option](https://asm-docs.microagi.org/#.option)。           |
| .byte        | 表达式 \[, 表达式]\*                              | 8 位逗号分隔的字                                                                                    |
| .2byte       | 表达式 \[, 表达式]\*                              | 16 位逗号分隔的字                                                                                   |
| .half        | 表达式 \[, 表达式]\*                              | 16 位逗号分隔的字                                                                                   |
| .short       | 表达式 \[, 表达式]\*                              | 16 位逗号分隔的字                                                                                   |
| .4byte       | 表达式 \[, 表达式]\*                              | 32 位逗号分隔的字                                                                                   |
| .word        | 表达式 \[, 表达式]\*                              | 32 位逗号分隔的字                                                                                   |
| .long        | 表达式 \[, 表达式]\*                              | 32 位逗号分隔的字                                                                                   |
| .8byte       | 表达式 \[, 表达式]\*                              | 64 位逗号分隔的字                                                                                   |
| .dword       | 表达式 \[, 表达式]\*                              | 64 位逗号分隔的字                                                                                   |
| .quad        | 表达式 \[, 表达式]\*                              | 64 位逗号分隔的字                                                                                   |
| .float       | 表达式 \[, 表达式]\*                              | 32 位浮点值，有关值格式，请参阅 [浮点文字](https://asm-docs.microagi.org/#fp-literal)。             |
| .double      | 表达式 \[, 表达式]\*                              | 64 位浮点值，有关值格式，请参阅 [浮点文字](https://asm-docs.microagi.org/#fp-literal)。             |
| .quad        | 表达式 \[, 表达式]\*                              | 128 位浮点值，有关值格式，请参阅 [浮点文字](https://asm-docs.microagi.org/#fp-literal)。            |
| .dtprelword  | 表达式 \[, 表达式]\*                              | 32 位线程局部字                                                                                     |
| .dtpreldword | 表达式 \[, 表达式]\*                              | 64 位线程局部字                                                                                     |
| .sleb128     | 表达式                                            | 有符号小端 base 128, DWARF                                                                          |
| .uleb128     | 表达式                                            | 无符号小端 base 128, DWARF                                                                          |
| .zero        | 整数                                              | 零字节                                                                                              |
| .variant_cc  | 符号\_名称                                        | 使用变体调用约定注释符号                                                                            |
| .attribute   | 名称, 值                                          | RISC-V 对象属性，更详细的描述请参阅 [.attribute](https://asm-docs.microagi.org/#.attribute)。       |

## []()`.align`

RISC-V 的 `.align` 指令是 `.p2align` 的别名，它对齐到 2 的幂，因此 `.align 2` 意味着对齐到 4 个字节。由于 `.align` 指令的定义[因架构而异](https://sourceware.org/binutils/docs/as/Align.html)，因此建议使用明确的 `.p2align` 或 `.balign` 指令代替。

## []()`.attribute`

`.attribute` 指令用于记录有关目标文件/二进制文件的信息，链接器或运行时加载程序需要检查这些信息以确保兼容性。

有关更多信息，例如属性名称、编号、值类型和描述，请参阅 [RISC-V psABI 中的属性部分](https://github.com/riscv-non-isa/riscv-elf-psabi-doc/blob/master/riscv-elf.adoc#attributes)。

`.attribute` 接受两个参数。`.attribute` 的第一个参数是属性的符号名称或属性编号，可以省略前缀 `Tag_RISCV_`，第二个参数可以是字符串或数字。

`.attribute` 的语法：

```
.attribute <名称_或_编号>, <属性_值>

名称_或_编号 := <属性-名称>
                | [1-9][0-9]*

属性_值 := <字符串>
                 | <数字>
```

### []()`.option`

#### `rvc`/`norvc`

在 `.option arch` 在主流开源工具链上得到广泛实现后，此选项将很快被弃用。

启用/禁用以下代码区域的 C 扩展。此选项等效于 `.option arch, +c`/`.option arch, -c`，但受到旧版本工具链的广泛支持。

替代样式：

```
.option push
.option arch, +c   # .option rvc 的替代方法
.option pop

.option push
.option arch, -c   # .option norvc 的替代方法
.option pop
```

注意：`.option rvc` 可能会在某些工具链中设置 ELF 标志 `EF_RISCV_RVC`。这可能会导致链接器压缩代码区域中不打算压缩的指令。

注意：`.option rvc`/`.option norvc` 和 `.option arch, +c`/`.option arch, -c` 之间存在差异。后者不会在 ELF 标志中设置 EF_RISCV_RVC。

#### `arch`

为以下代码区域启用和/或禁用特定的 ISA 扩展，但不会更改 ELF 标志中的 arch 属性和 `EF_RISCV_RVC`，这意味着它不会提高最低执行环境要求，因此用户应注意 `.option push`/`.option arch`/`.option pop` 周围代码区域的执行。

`.option arch` 的语法：

```
.option arch, <扩展_或_完整架构>

扩展_或_完整架构 := <扩展>
                        | <完整架构字符串>

扩展             := <扩展> ',' <扩展>
                        | <扩展>

完整架构字符串            := <完整架构字符串>

扩展              := <操作> <扩展-名称> <版本>

操作                     := '+'
                        | '-'

版本                := [0-9]+ 'p' [0-9]+
                        | [1-9][0-9]*
                        |

扩展-名称         := 命名规则在 RISC-V ISA 手册中定义
```

-   可以省略扩展版本，汇编器将使用该扩展的内置默认版本。
-   `OP` 可以是启用 (`+`) 或禁用 (`-`)。
-   `<完整架构字符串>` 的格式与 `-march` 选项相同。

示例：

```
.attribute arch, rv64imafdc
# 你只能使用来自 i、m、a、f、d 和 c 扩展的指令。
memcpy_general:
    add     a5,a1,a2
    beq     a1,a5,.L2
    add     a2,a0,a2
    mv      a5,a0
.L3:
    addi    a1,a1,1
    addi    a5,a5,1
    lbu     a4,-1(a1)
    sb      a4,-1(a5)
    bne     a5,a2,.L3
.L2:
    ret

.option push     # 将当前选项推送到堆栈。
.option arch, +v # 启用向量扩展，我们可以使用 imafdcv 扩展中的任何指令。
memcpy_vec:
    mv a3, a0
.Lloop:
    vsetvli t0, a2, e8, m8, ta, ma
    vle8.v v0, (a1)
    add a1, a1, t0
    sub a2, a2, t0
    vse8.v v0, (a3)
    add a3, a3, t0
    bnez a2, .Lloop
    ret
.option pop   # 从堆栈中弹出当前选项，将启用的 ISA 扩展状态恢复为 imafdc。

.option push     # 将当前选项推送到堆栈。
.option arch, -c # 禁用压缩扩展，我们无法使用扩展中的任何指令。
memcpy_norvc:
    add     a5,a1,a2
    beq     a1,a5,.L2
    add     a2,a0,a2
    mv      a5,a0
.L3:
    addi    a1,a1,1
    addi    a5,a5,1
    lbu     a4,-1(a1)
    sb      a4,-1(a5)
    bne     a5,a2,.L3
.L2:
    ret
.option pop   # 从堆栈中弹出当前选项，将启用的 ISA 扩展状态恢复为 imafdc。

.option push  # 将当前选项推送到堆栈。
.option arch, rv64imc # 将 arch 设置为 rv64imc。
    nop
.option pop   # 从堆栈中弹出当前选项，将启用的 ISA 扩展状态恢复为 imafdc。
```

注意：一个典型的用例是 `ifunc`，例如，C 库是用 `rv64gc` 构建的，但像 memcpy 这样的一些函数提供了两个版本，一个是用 `rv64gc` 构建的，另一个是用 `rv64gcv` 构建的，然后在运行时通过 ifunc 机制在它们之间进行选择。但是，我们不想将最低执行环境要求更改为 `rv64gcv`，因为只有在执行环境支持向量扩展时才会调用 `rv64gcv` 版本，因此最低执行环境要求仍然是 `rv64gc`。

注意：`.option arch, +` 还会启用所有必需的扩展，例如，`rv32i` + `.option arch, +v` 还会启用 `f`、`d`、`zve32x`、`zve32f`、`zve64x`、`zve64f`、`zve64d`、`zvl32b`、`zvl64b` 和 `zvl128b` 扩展。

注意：我们建议 `.option arch, +` 和 `.option arch, -` 与 `.option push`/`.option pop` 一起使用，而不是 `.option arch, +` / `.option arch, -` 对，因为 `.option arch, +` 将启用所有必需的扩展，但 `.option arch, -` 仅禁用特定的扩展，因此结果可能会出乎意料，例如：`rv32i` + `.option arch, +v` + `.option arch, -v` 将导致 `rv32ifd_zve32x_zve32f_zve64x_zve64f_zve64d_zvl32b_zvl64b_zvl128b` 而不是 `rv32i`。另一个例子是 `.option arch, rv64ifd` + `.option arch, -f`，这将导致 `rv64ifd`，因为当添加 `d` 的隐含扩展时，`f` 将被添加回来。

注意：`.option arch, +<ext>, -<ext>` 被接受，并将导致启用依赖于 `ext` 的扩展，例如 `rv32i` + `.option arch, +v, -v` 将导致 `rv32ifd_zve32x_zve32f_zve64x_zve64f_zve64d_zvl32b_zvl64b_zvl128b`。

#### `pic`/`nopic`

将代码模型设置为 PIC（位置无关代码）或非 PIC。这将影响 `la` 伪指令的扩展，请参阅 [标准 RISC-V 伪指令列表](https://asm-docs.microagi.org/#pseudoinstructions)。

#### `relax`/`norelax`

启用/禁用以下代码区域的链接器松弛。

注意：即使链接器不支持松弛，后跟 `.option relax` 的代码区域也会发出 `R_RISCV_RELAX`/`R_RISCV_ALIGN`。建议的使用方法是使用 `.option norelax` 与 `.option push`/`.option pop`，如果在代码区域中应禁用链接器松弛。

注意：禁用特定代码区域的链接器松弛的推荐方法是使用 `.option push`、`.option norelax` 和 `.option pop`，这可以防止意外启用链接器松弛（如果用户已经禁用了链接器松弛）。

#### `push`/`pop`

将当前选项推送到选项堆栈或从选项堆栈中弹出。

## 汇编器重定位函数

下表列出了汇编器重定位扩展：

| 汇编器表示法              | 描述                     | 指令/宏          |
| ------------------------- | ------------------------ | ---------------- |
| %hi(符号)                 | 绝对 (HI20)              | lui              |
| %lo(符号)                 | 绝对 (LO12)              | load, store, add |
| %pcrel_hi(符号)           | PC 相对 (HI20)           | auipc            |
| %pcrel_lo(标签)           | PC 相对 (LO12)           | load, store, add |
| %tprel_hi(符号)           | TLS LE "本地执行"        | lui              |
| %tprel_lo(符号)           | TLS LE "本地执行"        | load, store, add |
| %tprel_add(符号)          | TLS LE "本地执行"        | add              |
| %tls_ie_pcrel_hi(符号) \* | TLS IE "初始执行" (HI20) | auipc            |
| %tls_gd_pcrel_hi(符号) \* | TLS GD "全局动态" (HI20) | auipc            |
| %got_pcrel_hi(符号) \*    | GOT PC 相对 (HI20)       | auipc            |

\* 这些重用 %pcrel_lo(标签) 作为它们的低半部分

## 标签

文本标签用作分支、无条件跳转目标和符号偏移量。文本标签被添加到已编译模块的符号表中。

```assembly
loop:
        j loop
```

数字标签用于本地引用。对本地标签的引用以 'f' 作为前向引用，或以 'b' 作为后向引用。

```assembly
1:
        j 1b
```

## 绝对寻址

以下示例展示了如何加载绝对地址：

```assembly
    lui a0, %hi(msg + 1)
    addi    a0, a0, %lo(msg + 1)
```

这会生成以下汇编器输出和重定位，如 `objdump` 所见：

```
0000000000000000 <.text>:
   0:   00000537            lui a0,0x0
            0: R_RISCV_HI20 msg+0x1
   4:   00150513            addi    a0,a0,1 # 0x1
            4: R_RISCV_LO12_I   msg+0x1
```

## 相对寻址

以下示例展示了如何加载 PC 相对地址：

```assembly
1:
    auipc   a0, %pcrel_hi(msg + 1)
    addi    a0, a0, %pcrel_lo(1b)
```

这会生成以下汇编器输出和重定位，如 `objdump` 所见：

```
0000000000000000 <.text>:
   0:   00000517            auipc   a0,0x0
            0: R_RISCV_PCREL_HI20   msg+0x1
   4:   00050513            mv  a0,a0
            4: R_RISCV_PCREL_LO12_I .L1
```

## GOT 间接寻址

以下示例展示了如何从 GOT 加载地址：

```assembly
1:
    auipc   a0, %got_pcrel_hi(msg + 1)
    ld  a0, %pcrel_lo(1b)(a0)
```

这会生成以下汇编器输出和重定位，如 `objdump` 所见：

```
0000000000000000 <.text>:
   0:   00000517            auipc   a0,0x0
            0: R_RISCV_GOT_HI20 msg+0x1
   4:   00050513            mv  a0,a0
            4: R_RISCV_PCREL_LO12_I .L1
```

以下示例展示了用于加载立即值的 `li` 伪指令：

```assembly
    .equ    CONSTANT, 0xdeadbeef

    li  a0, CONSTANT
```

对于 RV32I，这会生成以下汇编器输出，如 `objdump` 所见：

```
00000000 <.text>:
   0:   deadc537            lui a0,0xdeadc
   4:   eef50513            addi    a0,a0,-273 # deadbeef <CONSTANT+0x0>
```

`lui` 的立即参数是 \[0x0, 0xfffff] 区间内的整数。它的压缩形式 `c.lui` 仅接受子区间 \[0x1, 0x1f] 和 \[0xfffe0, 0xfffff] 中的整数。

所有带有 12 位有符号立即数的 I 型和 S 型指令（例如 `addi` 但不是 `slli`）都接受其立即参数作为 \[-2048, 2047] 区间内的整数。在 RV32I 上，区间 \[-2048, -1] 中的整数也可以通过它们在区间 \[0xfffff800, 0xffffffff] 中的（无符号）关联数传递，在 RV32I 和 RV64I 上，区间 \[-2048, -1] 中的整数也可以通过它们在区间 \[0xfffffffffffff800, 0xffffffffffffffff] 中的（无符号）关联数传递。

## []()浮点文字

汇编器支持与 C 和 C++ 标准中定义的浮点文字格式相同的格式（即，带有十进制指数的十进制浮点文字以及带有二进制指数的十六进制浮点文字）。

以下是一些示例：

-   3.14159
-   0.271828e1
-   0x0.3p-4

注意：浮点立即值的详细格式可以在 [此页面](https://en.cppreference.com/w/cpp/language/floating_literal) 上找到。

`Zfa` 扩展引入了 `fli.{h|s|d|q}` 指令，用于加载一组特定的浮点立即数，支持的值可以在 RISC-V ISA 规范中找到，但也在下面列出。

`fli` 指令用于将浮点立即数加载到浮点寄存器中，接受的立即数在 [浮点文字](https://asm-docs.microagi.org/#fp-literal) 中定义，参考表可以在 [FLI 操作数参考表](https://asm-docs.microagi.org/#fli-operand-value) 中找到。

```assembly
    fli.s   fa0, 0x1p-15
    fli.s   fa1, 0.00390625
    fli.s   fa2, 6.25e-02
```

对于与 'fli' 指令的浮点立即数操作数不完全匹配的任何值，工具都应拒绝。

RISC-V 不提供加载任意浮点立即值的通用伪指令。相反，程序员可以使用 `.float`/`.double` 指令在源代码中声明浮点立即值，然后使用加载全局伪指令 (`fl{h|w|d|q}`) 将其加载到浮点寄存器中。

```assembly
    .data
.VAL:
    .float .0x1p+17
    .text
    flw fa0, .VAL, t0
```

[]()FLI 操作数参考表

| 值              | 示例合法输入值                                 |
| --------------- | ---------------------------------------------- |
| -1.0            | -0x1p+0, -1.0, -1.0e+0                         |
| 最小正规数      | min                                            |
| 1.0 x 2 ^ -16   | 0x1p-16, 0.0000152587890625, 1.52587890625e-05 |
| 1.0 x 2 ^ -15   | 0x1p-15, 0.000030517578125, 3.0517578125e-05   |
| 1.0 x 2 ^ -8    | 0x1p-8, 0.00390625, 3.90625e-03                |
| 1.0 x 2 ^ -7    | 0x1p-7, 0.0078125, 7.8125e-03                  |
| 0.0625 (2 ^ -4) | 0x1p-4, 0.0625, 6.25e-02                       |
| 0.125 (2 ^ -3)  | 0x1p-3, 0.125, 1.25e-01                        |
| 0.25            | 0x1p-2, 0.25, 2.5e-01                          |
| 0.3125          | 0x1.4p-2, 0.3125, 3.125e-01                    |
| 0.375           | 0x1.8p-2, 0.375, 3.75e-01                      |
| 0.4375          | 0x1.cp-2, 0.4375, 4.375e-01                    |
| 0.5             | 0x1p-1, 0.5, 5.0e-01                           |
| 0.625           | 0x1.4p-1, 0.625, 6.25e-01                      |
| 0.75            | 0x1.8p-1, 0.75, 7.5e-01                        |
| 0.875           | 0x1.cp-1, 0.875, 8.75e-01                      |
| 1.0             | 0x1p+0, 1.0, 1.0e+00                           |
| 1.25            | 0x1.4p+0, 1.25, 1.25e+00                       |
| 1.5             | 0x1.8p+0, 1.5, 1.5e+00                         |
| 1.75            | 0x1.cp+0, 1.75, 1.75e+00                       |
| 2.0             | 0x1p+1, 2.0, 2.0e+00                           |
| 2.5             | 0x1.4p+1, 2.5, 2.5e+00                         |
| 3               | 0x1.8p+1, 3.0, 3.0e+00                         |
| 4               | 0x1p+2, 4.0, 4.0e+00                           |
