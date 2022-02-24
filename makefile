# Makefile
all: diskimage bootloader stdlib kernel

# Recipes
diskimage:
	# TODO : Tambahkan untuk pembuatan image
	

bootloader:
	# TODO : Tambahkan untuk pembuatan bootloader

kernel:
	# TODO : Tambahkan untuk pembuatan kernel
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/std_lib.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

stdlib:
	# Opsional
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c

run:
	bochs -f src/config/if2230.config

build-run: all run
