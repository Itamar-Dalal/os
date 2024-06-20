CC:=i586-elf-gcc
CFLAGS:=-c -Wall
LD:=i586-elf-ld
LDFLAGS:=-T linker.ld -o kernel.bin
ASM:=nasm
ASMFLAGS:=-f elf

all: kernel.o boot.o
	$(LD) $(LDFLAGS) kernel.o boot.o screen.o

kernel.o: screen.o
	$(CC) $(CFLAGS) kernel/kernel.c

boot.o: kernel.o
	$(ASM) $(ASMFLAGS) -o boot.o boot/boot.asm

screen.o:
	$(CC) $(CFLAGS) kernel/screen.c

clean:
	$(shell rm -rf *.o kernel.bin)
