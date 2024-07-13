#ifndef TYPES_H
#define TYPES_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

#define true 1
#define false 0
typedef uint8_t bool;

typedef uint32_t size_t;
typedef int32_t ssize_t;

typedef uint32_t physaddr_t; // Physical address
typedef uint32_t virtaddr_t; // Virtual address

typedef struct {
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t  base_middle;
	uint8_t  access;
	uint8_t  granularity;
	uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;
// __attribute__((packed)) - Instructs the compiler not to perform any padding
// between the members of the type and not to change their alignment 

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

typedef struct {
	uint16_t base_low;
	uint16_t selector;
	uint8_t  always0;
	uint8_t  flags;
	uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed)) idt_ptr_t;

typedef struct {
	uint32_t ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

#endif