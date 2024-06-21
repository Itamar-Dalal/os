#include "descriptors.h"

extern void gdt_write(unsigned int);

struct gdt_entry_struct
{
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char  base_middle;
    unsigned char  access;
    unsigned char  granularity;
    unsigned char  base_high;
} __attribute__((packed)); 
// __attribute__((packed)) - Instructs the compiler not to perform any padding
// between the members of the type and not to change their alignment 
typedef struct gdt_entry_struct gdt_entry_t;

gdt_entry_t gdt_set_gate(unsigned int base, unsigned int limit, unsigned char access, unsigned char granularity)
{
	gdt_entry_t gdt_entry;
	gdt_entry.base_low = (base & 0xFFFF);
	gdt_entry.base_middle = (base >> 16) & 0xFF;
	gdt_entry.base_high = (base >> 24) & 0xFF;
	gdt_entry.limit_low = (limit & 0xFFFF);
	gdt_entry.granularity = (limit >> 16) & 0x0F;
	gdt_entry.granularity |= granularity & 0xF0;
	gdt_entry.access = access;
	return gdt_entry;
}

struct gdt_ptr_struct
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;

void gdt_setup()
{
	gdt_entry_t gdt_entries[5];
	gdt_ptr_t gdt_ptr;
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 5) - 1; // -1 because it starts from 0
	gdt_ptr.base = (unsigned int)&gdt_entries;
	/*
	Present bit (P): 1
	DPL (Descriptor Privilege Level): 00 (for ring 0, kernel privilege) and 11 (for ring 3, user privilege)
	Descriptor type (S): 1
	Type field: 1010 and 0010 (1 at the start means code segment and 0 means data segment)
	I encoded the result to hex
	*/
	gdt_entries[0] = gdt_set_gate(0, 0, 0, 0);
	gdt_entries[1] = gdt_set_gate(0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment with kernel privilege
	gdt_entries[2] = gdt_set_gate(0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment with kernel privilege
	gdt_entries[3] = gdt_set_gate(0, 0xFFFFFFFF, 0xFA, 0xCF); // Code segment with user privilege
	gdt_entries[4] = gdt_set_gate(0, 0xFFFFFFFF, 0xF2, 0xCF); // Data segment with user privilege
	gdt_write((unsigned int)&gdt_ptr);
}