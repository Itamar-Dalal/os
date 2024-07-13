CC:=i586-elf-gcc
CFLAGS:=-c -Wall -I../include
LD:=i586-elf-ld
LDFLAGS:=-T linker.ld -o kernel.bin
ASM:=nasm
ASMFLAGS:=-f elf

all: kernel.o boot.o
	$(LD) $(LDFLAGS) kernel.o boot.o screen.o asmisr.o gdt.o idt.o descriptors.o isr.o tools.o keyboard.o

kernel.o: screen.o descriptors.o
	$(CC) $(CFLAGS) -o kernel.o kernel/kernel.c

boot.o: kernel.o
	$(ASM) $(ASMFLAGS) -o boot.o boot/boot.asm

gdt.o:
	$(ASM) $(ASMFLAGS) -o gdt.o kernel/gdt.asm

idt.o: isr.o
	$(ASM) $(ASMFLAGS) -o idt.o kernel/idt.asm

asmisr.o:
	$(ASM) $(ASMFLAGS) -o asmisr.o kernel/asmisr.asm

tools.o:
	$(CC) $(CFLAGS) -o tools.o kernel/tools.c

descriptors.o: asmisr.o gdt.o idt.o tools.o isr.o keyboard.o
	$(CC) $(CFLAGS) -o descriptors.o kernel/descriptors.c

screen.o: tools.o
	$(CC) $(CFLAGS) -o screen.o kernel/screen.c

isr.o: tools.o
	$(CC) $(CFLAGS) -o isr.o kernel/isr.c

keyboard.o:
	$(CC) $(CFLAGS) -o keyboard.o kernel/keyboard.c

clean:
	rm -rf *.o kernel.bin
