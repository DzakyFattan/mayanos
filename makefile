# Makefile
all: diskimage bootloader lib kernel app

# Recipes
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880
	
bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc

lib:
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/program.o src/c/program.c

kernel:
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/lib_interrupt.o out/std_lib.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

app:
	bcc -ansi -c -o out/shell.o src/c/shell.c
	ld86 -o out/shell_utils/shell -d out/shell.o out/lib_interrupt.o out/string.o out/textio.o out/std_lib.o

run:
	bochs -f src/config/if2230.config

build-run: all run
