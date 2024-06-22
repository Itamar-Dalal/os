CC:=i586-elf-gcc
CFLAGS:=-c -Wall
LD:=i586-elf-ld
LDFLAGS:=-T linker.ld -o kernel.bin
ASM:=nasm
ASMFLAGS:=-f elf

all: kernel.o boot.o
	$(LD) $(LDFLAGS) kernel.o boot.o screen.o descriptors.o gdt.o

kernel.o: screen.o descriptors.o
	$(CC) $(CFLAGS) kernel/kernel.c

boot.o: kernel.o
	$(ASM) $(ASMFLAGS) -o boot.o boot/boot.asm

gdt.o:
	$(ASM) $(ASMFLAGS) -o gdt.o kernel/gdt.asm

descriptors.o: gdt.o
	$(CC) $(CFLAGS) kernel/descriptors.c

screen.o:
	$(CC) $(CFLAGS) kernel/screen.c

clean:
	$(shell rm -rf *.o kernel.bin)
