CC := i586-elf-gcc
CFLAGS := -c -Wall -I../include 
LD := i586-elf-ld
LDFLAGS := -T linker.ld -o kernel.bin
ASM := nasm
ASMFLAGS := -f elf

all: kernel.bin

kernel.bin: kernel.o boot.o screen.o asmisr.o gdt.o idt.o descriptors.o isr.o tools.o keyboard.o memory.o multiboot_info.o
	$(LD) $(LDFLAGS) kernel.o boot.o screen.o asmisr.o gdt.o idt.o descriptors.o isr.o tools.o keyboard.o memory.o multiboot_info.o

kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -o kernel.o kernel/kernel.c

boot.o: boot/boot.asm
	$(ASM) $(ASMFLAGS) -o boot.o boot/boot.asm

gdt.o: kernel/gdt.asm
	$(ASM) $(ASMFLAGS) -o gdt.o kernel/gdt.asm

idt.o: kernel/idt.asm
	$(ASM) $(ASMFLAGS) -o idt.o kernel/idt.asm

asmisr.o: kernel/asmisr.asm
	$(ASM) $(ASMFLAGS) -o asmisr.o kernel/asmisr.asm

tools.o: kernel/tools.c
	$(CC) $(CFLAGS) -o tools.o kernel/tools.c

descriptors.o: kernel/descriptors.c
	$(CC) $(CFLAGS) -o descriptors.o kernel/descriptors.c

screen.o: kernel/screen.c
	$(CC) $(CFLAGS) -o screen.o kernel/screen.c

isr.o: kernel/isr.c
	$(CC) $(CFLAGS) -o isr.o kernel/isr.c

keyboard.o: kernel/keyboard.c
	$(CC) $(CFLAGS) -o keyboard.o kernel/keyboard.c

memory.o: kernel/memory.c
	$(CC) $(CFLAGS) -o memory.o kernel/memory.c

multiboot_info.o: kernel/multiboot_info.c
	$(CC) $(CFLAGS) -o multiboot_info.o kernel/multiboot_info.c

clean:
	rm -rf *.o kernel.bin
