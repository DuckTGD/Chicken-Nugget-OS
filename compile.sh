#!/bin/bash

# Assemble bootloader
nasm -f bin bootldr.asm -o bootldr.bin

# Assemble kernel entry
nasm -f elf32 kernel_entry.asm -o kernel_entry.o

# Assemble zeroes (used for padding)
nasm -f bin zeroes.asm -o zeroes.bin

# Compile kernel
/usr/local/i386elfgcc/bin/i386-elf-gcc -ffreestanding -m32 -g -c kernel.cpp -o kernel.o

# Link kernel
/usr/local/i386elfgcc/bin/i386-elf-ld -m elf_i386 -Ttext 0x1000 --oformat binary -o kernel.bin kernel_entry.o kernel.o

# Get the size of kernel.bin
kernel_size=$(stat -c %s kernel.bin)

# Calculate the padding size to the next 512-byte boundary
padding_size=$((512 - (kernel_size % 512)))
if [ $padding_size -eq 512 ]; then
    padding_size=0
fi

# Create padding
dd if=/dev/zero bs=1 count=$padding_size of=padding.bin

# Concatenate bootloader, kernel, padding, and zeroes to create the final image
cat bootldr.bin kernel.bin padding.bin zeroes.bin > os-image.bin

# Run the image in QEMU
qemu-system-x86_64 -drive format=raw,file="os-image.bin",index=0,if=floppy -m 128M

# Create ISO
mkisofs -o boot.iso -b os-image.bin -no-emul-boot os-image.bin

# Done!
echo "Done! Hopefully it works this time :/"
