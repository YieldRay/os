#!/bin/bash
set -xue

QEMU=qemu-system-riscv32
OBJCOPY=/opt/homebrew/opt/llvm/bin/llvm-objcopy

# clang 路径和编译器标志
CC=/opt/homebrew/opt/llvm/bin/clang # Ubuntu 用户：使用 CC=clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fno-stack-protector -ffreestanding -nostdlib"
# -fno-stack-protector 禁用栈保护功能
# -ffreestanding       不使用主机环境标准库
# -nostdlib            不链接标准库

# TODO: 使用Makefile替代这里的手动编译？

# 构建一个用户空间程序，并转换为原始二进制（而不是elf），因为我们的操作系统很简单
$CC $CFLAGS -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf shell.c user.c common.c
$OBJCOPY --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
$OBJCOPY -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o

# 构建内核
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c common.c shell.bin.o
# -Wl,-Tkernel.ld      指定链接器脚本
# -Wl,-Map=kernel.map  输出映射文件（链接器分配结果）

# 启动 QEMU
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -d unimp,guest_errors,int,cpu_reset -D qemu.log \
    -drive id=drive0,file=lorem.txt,format=raw,if=none \
    -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
    -kernel kernel.elf
# -machine virt      启动一个 virt 机器。可以用 -machine '?' 选项查看其他支持的机器
# -bios default      使用默认固件（在本例中是 OpenSBI）
# -nographic         启动 QEMU 时不使用 GUI 窗口
# -serial mon:stdio  将 QEMU 标准输入/输出连接到虚拟机的串行端口。指定 mon: 允许通过按下 Ctrl+A 然后 C 切换到 QEMU 监视器
# --no-reboot        如果虚拟机崩溃，停止模拟器而不重启（对调试有用）
# -drive id=drive0   定义一个名为 drive0 的磁盘，使用 lorem.txt 作为磁盘镜像。磁盘镜像格式为 raw (文件内容按原样作磁盘数据处理)
# -device virtio-blk-device,drive=drive0 添加一个带有 drive0 磁盘的 virtio-blk 设备。
#         bus=virtio-mmio-bus.0 将设备映射到 virtio-mmio 总线 (通过内存映射 I/O 的 virtio)。
