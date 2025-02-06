#!/bin/bash
set -xue

QEMU=qemu-system-riscv32

# clang 路径和编译器标志
CC=/opt/homebrew/opt/llvm/bin/clang # Ubuntu 用户：使用 CC=clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"
# -fno-stack-protector 禁用栈保护功能
# -ffreestanding       不使用主机环境标准库
# -nostdlib            不链接标准库

# 构建内核
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c common.c
# -Wl,-Tkernel.ld      指定链接器脚本
# -Wl,-Map=kernel.map  输出映射文件（链接器分配结果）

# 启动 QEMU
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot -kernel kernel.elf
# -machine virt      启动一个 virt 机器。可以用 -machine '?' 选项查看其他支持的机器
# -bios default      使用默认固件（在本例中是 OpenSBI）
# -nographic         启动 QEMU 时不使用 GUI 窗口
# -serial mon:stdio  将 QEMU 标准输入/输出连接到虚拟机的串行端口。指定 mon: 允许通过按下 Ctrl+A 然后 C 切换到 QEMU 监视器
# --no-reboot        如果虚拟机崩溃，停止模拟器而不重启（对调试有用）
