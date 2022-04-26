# Makefile
LIBRARY := out/string.o out/textio.o out/fileio.o out/program.o out/lib_interrupt.o out/message.o

all: diskimage bootloader lib kernel app program

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
	bcc -ansi -c -o out/message.o src/c/message.c

kernel:
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/lib_interrupt.o out/std_lib.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

program: lib
	bcc -ansi -c -o out/cd.o src/c/cd.c
	ld86 -o out/cd -d out/cd.o $(LIBRARY)
	bcc -ansi -c -o out/ls.o src/c/ls.c
	ld86 -o out/ls -d out/ls.o $(LIBRARY)
	bcc -ansi -c -o out/mkdir.o src/c/mkdir.c
	ld86 -o out/mkdir -d out/mkdir.o $(LIBRARY)
	bcc -ansi -c -o out/cat.o src/c/cat.c
	ld86 -o out/cat -d out/cat.o $(LIBRARY)
	bcc -ansi -c -o out/cp.o src/c/cp.c
	ld86 -o out/cp -d out/cp.o $(LIBRARY)

tcgen:
	gcc tc_gen/tc_gen.c tc_gen/tc_lib -o out/tc_gen

app:
	bcc -ansi -c -o out/shell.o src/c/shell.c
	ld86 -o out/shell -d out/shell.o $(LIBRARY)
shell:
	make lib app

run:
	bochs -f src/config/if2230.config

build-run: all run
