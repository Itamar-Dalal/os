CC:=i586-elf-gcc
CFLAGS:=-c -Wall
LD:=i586-elf-ld
LDFLAGS:=-T linker.ld -o kernel.bin
ASM:=nasm
ASMFLAGS:=-f elf

all: kernel.o boot.o
	$(LD) $(LDFLAGS) kernel.o boot.o screen.o asmisr.o gdt.o idt.o descriptors.o isr.o

kernel.o: screen.o descriptors.o
	$(CC) $(CFLAGS) kernel/kernel.c

boot.o: kernel.o
	$(ASM) $(ASMFLAGS) -o boot.o boot/boot.asm

gdt.o:
	$(ASM) $(ASMFLAGS) -o gdt.o kernel/gdt.asm

idt.o: isr.o
	$(ASM) $(ASMFLAGS) -o idt.o kernel/idt.asm

asmisr.o:
	$(ASM) $(ASMFLAGS) -o asmisr.o kernel/asmisr.asm

descriptors.o: asmisr.o gdt.o idt.o 
	$(CC) $(CFLAGS) kernel/descriptors.c

screen.o:
	$(CC) $(CFLAGS) kernel/screen.c

isr.o:
	$(CC) $(CFLAGS) kernel/isr.c

clean:
	$(shell rm -rf *.o kernel.bin)