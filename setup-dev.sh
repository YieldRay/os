sudo apt update && sudo apt install -y clang llvm lld qemu-system-riscv32 curl
curl -LO https://github.com/qemu/qemu/raw/v8.0.4/pc-bios/opensbi-riscv32-generic-fw_dynamic.bin

# create dummy disk image files just for test
echo "lorem ipsum dolor sit amet" >lorem.txt
mkdir disk
echo "Can you see me? Ah, there you are! You've unlocked the achievement \"Virtio Newbie!\"" >disk/hello.txt
echo "meow!" >disk/meow.txt
