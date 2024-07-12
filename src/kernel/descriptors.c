#include "descriptors.h"
#include "isr.h"
#include "tools.h"
#include "screen.h"

#define true 1
#define false 0

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x11
#define ICW4_8086    0x01

#define KEYBOARD_DATA_PORT 0x60

typedef unsigned char uint8_t;
typedef int bool;

extern void gdt_write(unsigned int);
extern void idt_write(unsigned int);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();
extern void isr48();
extern void isr49();
extern void isr50();
extern void isr51();
extern void isr52();
extern void isr53();
extern void isr54();
extern void isr55();
extern void isr56();
extern void isr57();
extern void isr58();
extern void isr59();
extern void isr60();
extern void isr61();
extern void isr62();
extern void isr63();
extern void isr64();
extern void isr65();
extern void isr66();
extern void isr67();
extern void isr68();
extern void isr69();
extern void isr70();
extern void isr71();
extern void isr72();
extern void isr73();
extern void isr74();
extern void isr75();
extern void isr76();
extern void isr77();
extern void isr78();
extern void isr79();
extern void isr80();
extern void isr81();
extern void isr82();
extern void isr83();
extern void isr84();
extern void isr85();
extern void isr86();
extern void isr87();
extern void isr88();
extern void isr89();
extern void isr90();
extern void isr91();
extern void isr92();
extern void isr93();
extern void isr94();
extern void isr95();
extern void isr96();
extern void isr97();
extern void isr98();
extern void isr99();
extern void isr100();
extern void isr101();
extern void isr102();
extern void isr103();
extern void isr104();
extern void isr105();
extern void isr106();
extern void isr107();
extern void isr108();
extern void isr109();
extern void isr110();
extern void isr111();
extern void isr112();
extern void isr113();
extern void isr114();
extern void isr115();
extern void isr116();
extern void isr117();
extern void isr118();
extern void isr119();
extern void isr120();
extern void isr121();
extern void isr122();
extern void isr123();
extern void isr124();
extern void isr125();
extern void isr126();
extern void isr127();
extern void isr128();
extern void isr129();
extern void isr130();
extern void isr131();
extern void isr132();
extern void isr133();
extern void isr134();
extern void isr135();
extern void isr136();
extern void isr137();
extern void isr138();
extern void isr139();
extern void isr140();
extern void isr141();
extern void isr142();
extern void isr143();
extern void isr144();
extern void isr145();
extern void isr146();
extern void isr147();
extern void isr148();
extern void isr149();
extern void isr150();
extern void isr151();
extern void isr152();
extern void isr153();
extern void isr154();
extern void isr155();
extern void isr156();
extern void isr157();
extern void isr158();
extern void isr159();
extern void isr160();
extern void isr161();
extern void isr162();
extern void isr163();
extern void isr164();
extern void isr165();
extern void isr166();
extern void isr167();
extern void isr168();
extern void isr169();
extern void isr170();
extern void isr171();
extern void isr172();
extern void isr173();
extern void isr174();
extern void isr175();
extern void isr176();
extern void isr177();
extern void isr178();
extern void isr179();
extern void isr180();
extern void isr181();
extern void isr182();
extern void isr183();
extern void isr184();
extern void isr185();
extern void isr186();
extern void isr187();
extern void isr188();
extern void isr189();
extern void isr190();
extern void isr191();
extern void isr192();
extern void isr193();
extern void isr194();
extern void isr195();
extern void isr196();
extern void isr197();
extern void isr198();
extern void isr199();
extern void isr200();
extern void isr201();
extern void isr202();
extern void isr203();
extern void isr204();
extern void isr205();
extern void isr206();
extern void isr207();
extern void isr208();
extern void isr209();
extern void isr210();
extern void isr211();
extern void isr212();
extern void isr213();
extern void isr214();
extern void isr215();
extern void isr216();
extern void isr217();
extern void isr218();
extern void isr219();
extern void isr220();
extern void isr221();
extern void isr222();
extern void isr223();
extern void isr224();
extern void isr225();
extern void isr226();
extern void isr227();
extern void isr228();
extern void isr229();
extern void isr230();
extern void isr231();
extern void isr232();
extern void isr233();
extern void isr234();
extern void isr235();
extern void isr236();
extern void isr237();
extern void isr238();
extern void isr239();
extern void isr240();
extern void isr241();
extern void isr242();
extern void isr243();
extern void isr244();
extern void isr245();
extern void isr246();
extern void isr247();
extern void isr248();
extern void isr249();
extern void isr250();
extern void isr251();
extern void isr252();
extern void isr253();
extern void isr254();
extern void isr255();


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

struct gdt_ptr_struct
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

typedef struct gdt_ptr_struct gdt_ptr_t;


struct idt_entry_struct {
	unsigned short base_low;
	unsigned short selector;
	unsigned char  always0;
	unsigned char  flags;
	unsigned short base_high;
} __attribute__((packed));

typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct {
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

typedef struct idt_ptr_struct idt_ptr_t;


gdt_entry_t gdt_entries[5];
idt_entry_t idt_entries[256];

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

void gdt_setup()
{
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

/*void watch() {
	static unsigned int time = 0;
	static unsigned int seconds = 0;
	if (time >= 1000) { // 1 second
		time %= 1000;
		seconds++;
		screen_print("Time: ");
		screen_print_int(seconds, 10);
		screen_print("s\n");
	}
	time += 55; // PIT sends an interrupt every 55 milliseconds
}*/

void keyboard_interrupt_handler() {
	uint8_t scancode = inb(KEYBOARD_DATA_PORT);
	// A simple lookup table for US keyboard layout (partial)
	static char scancode_table[128] = {
		0, 27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
		'9', '0', '-', '=', '\b', /* Backspace */
		'\t', /* Tab */
		'q', 'w', 'e', 'r', /* 19 */
		't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
		0, /* 29   - Control */
		'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
		'\'', '`', 0, /* Left shift */
		'\\', 'z', 'x', 'c', 'v', 'b', 'n', /* 49 */
		'm', ',', '.', '/', 0, /* Right shift */
		'*',
		0, /* Alt */
		' ', /* Space bar */
		0, /* Caps lock */
		/* ... other keys can be added here */
	};

	// A simple lookup table for Shift+key for US keyboard layout (partial)
	static char shift_scancode_table[128] = {
		0, 27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
		'(', ')', '_', '+', '\b', /* Backspace */
		'\t', /* Tab */
		'Q', 'W', 'E', 'R', /* 19 */
		'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
		0, /* 29   - Control */
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
		'"', '~', 0, /* Left shift */
		'|', 'Z', 'X', 'C', 'V', 'B', 'N', /* 49 */
		'M', '<', '>', '?', 0, /* Right shift */
		'*',
		0, /* Alt */
		' ', /* Space bar */
		0, /* Caps lock */
		/* ... other keys can be added here */
	};

	bool shift_pressed = false;
	bool caps_lock_active = false;
	// Handle key releases
	if (scancode & 0x80) {
		scancode &= 0x7F; // Clear the top bit to get the make code
		if (scancode == 0x2A || scancode == 0x36) {
			shift_pressed = false; // Left Shift or Right Shift released
		}
		return;
	}

	// Handle key presses
	if (scancode == 0x2A || scancode == 0x36) {
		shift_pressed = true; // Left Shift or Right Shift pressed
		return;
	}
	else if (scancode == 0x3A) {
		caps_lock_active = !caps_lock_active; // Toggle Caps Lock
		return;
	}

	char ascii;
	if (shift_pressed) {
		ascii = shift_scancode_table[scancode];
	}
	else {
		ascii = scancode_table[scancode];
		if (caps_lock_active && ascii >= 'a' && ascii <= 'z') {
			ascii -= 32; // Convert to uppercase if Caps Lock is active
		}
	}
	screen_print(&ascii);
}

void irq_setup() {
	// ICW1
	outb(PIC1_COMMAND, ICW1_INIT);
	outb(PIC2_COMMAND, ICW1_INIT);
	// ICW2
	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);
	// ICW3
	outb(PIC1_DATA, 0x04); // 00000100 - IRQ2
	outb(PIC2_DATA, 0x02);
	// ICW4
	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);
	// Unmask (all IRQ are enabled)
	outb(PIC1_DATA, 0x00);
	outb(PIC2_DATA, 0x00);

	//register_isr_callback(IRQ0, &watch);
	register_isr_callback(IRQ1, &keyboard_interrupt_handler);
}

void idt_set_gate(int index, unsigned int base, unsigned short selector, uint8_t flags) {
	idt_entries[index].base_low = base & 0xFFFF;
	idt_entries[index].base_high = (base >> 16) & 0xFFFF;
	idt_entries[index].selector = selector;
	idt_entries[index].always0 = 0;
	idt_entries[index].flags = flags;
}

void idt_setup() {
	idt_ptr_t idt_ptr;
	idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
	idt_ptr.base = (unsigned int)&idt_entries;

	idt_set_gate(0, (unsigned int)isr0, 0x08, 0x8e); // 0x08 - selector to code segment ring 0
	idt_set_gate(1, (unsigned int)isr1, 0x08, 0x8e);
	idt_set_gate(2, (unsigned int)isr2, 0x08, 0x8e);
	idt_set_gate(3, (unsigned int)isr3, 0x08, 0x8e);
	idt_set_gate(4, (unsigned int)isr4, 0x08, 0x8e);
	idt_set_gate(5, (unsigned int)isr5, 0x08, 0x8e);
	idt_set_gate(6, (unsigned int)isr6, 0x08, 0x8e);
	idt_set_gate(7, (unsigned int)isr7, 0x08, 0x8e);
	idt_set_gate(8, (unsigned int)isr8, 0x08, 0x8e);
	idt_set_gate(9, (unsigned int)isr9, 0x08, 0x8e);
	idt_set_gate(10, (unsigned int)isr10, 0x08, 0x8e);
	idt_set_gate(11, (unsigned int)isr11, 0x08, 0x8e);
	idt_set_gate(12, (unsigned int)isr12, 0x08, 0x8e);
	idt_set_gate(13, (unsigned int)isr13, 0x08, 0x8e);
	idt_set_gate(14, (unsigned int)isr14, 0x08, 0x8e);
	idt_set_gate(15, (unsigned int)isr15, 0x08, 0x8e);
	idt_set_gate(16, (unsigned int)isr16, 0x08, 0x8e);
	idt_set_gate(17, (unsigned int)isr17, 0x08, 0x8e);
	idt_set_gate(18, (unsigned int)isr18, 0x08, 0x8e);
	idt_set_gate(19, (unsigned int)isr19, 0x08, 0x8e);
	idt_set_gate(20, (unsigned int)isr20, 0x08, 0x8e);
	idt_set_gate(21, (unsigned int)isr21, 0x08, 0x8e);
	idt_set_gate(22, (unsigned int)isr22, 0x08, 0x8e);
	idt_set_gate(23, (unsigned int)isr23, 0x08, 0x8e);
	idt_set_gate(24, (unsigned int)isr24, 0x08, 0x8e);
	idt_set_gate(25, (unsigned int)isr25, 0x08, 0x8e);
	idt_set_gate(26, (unsigned int)isr26, 0x08, 0x8e);
	idt_set_gate(27, (unsigned int)isr27, 0x08, 0x8e);
	idt_set_gate(28, (unsigned int)isr28, 0x08, 0x8e);
	idt_set_gate(29, (unsigned int)isr29, 0x08, 0x8e);
	idt_set_gate(30, (unsigned int)isr30, 0x08, 0x8e);
	idt_set_gate(31, (unsigned int)isr31, 0x08, 0x8e);
	idt_set_gate(32, (unsigned int)irq0, 0x08, 0x8e);
	idt_set_gate(33, (unsigned int)irq1, 0x08, 0x8e);
	idt_set_gate(34, (unsigned int)irq2, 0x08, 0x8e);
	idt_set_gate(35, (unsigned int)irq3, 0x08, 0x8e);
	idt_set_gate(36, (unsigned int)irq4, 0x08, 0x8e);
	idt_set_gate(37, (unsigned int)irq5, 0x08, 0x8e);
	idt_set_gate(38, (unsigned int)irq6, 0x08, 0x8e);
	idt_set_gate(39, (unsigned int)irq7, 0x08, 0x8e);
	idt_set_gate(40, (unsigned int)irq8, 0x08, 0x8e);
	idt_set_gate(41, (unsigned int)irq9, 0x08, 0x8e);
	idt_set_gate(42, (unsigned int)irq10, 0x08, 0x8e);
	idt_set_gate(43, (unsigned int)irq11, 0x08, 0x8e);
	idt_set_gate(44, (unsigned int)irq12, 0x08, 0x8e);
	idt_set_gate(45, (unsigned int)irq13, 0x08, 0x8e);
	idt_set_gate(46, (unsigned int)irq14, 0x08, 0x8e);
	idt_set_gate(47, (unsigned int)irq15, 0x08, 0x8e);
	idt_set_gate(48, (unsigned int)isr48, 0x08, 0x8e);
	idt_set_gate(49, (unsigned int)isr49, 0x08, 0x8e);
	idt_set_gate(50, (unsigned int)isr50, 0x08, 0x8e);
	idt_set_gate(51, (unsigned int)isr51, 0x08, 0x8e);
	idt_set_gate(52, (unsigned int)isr52, 0x08, 0x8e);
	idt_set_gate(53, (unsigned int)isr53, 0x08, 0x8e);
	idt_set_gate(54, (unsigned int)isr54, 0x08, 0x8e);
	idt_set_gate(55, (unsigned int)isr55, 0x08, 0x8e);
	idt_set_gate(56, (unsigned int)isr56, 0x08, 0x8e);
	idt_set_gate(57, (unsigned int)isr57, 0x08, 0x8e);
	idt_set_gate(58, (unsigned int)isr58, 0x08, 0x8e);
	idt_set_gate(59, (unsigned int)isr59, 0x08, 0x8e);
	idt_set_gate(60, (unsigned int)isr60, 0x08, 0x8e);
	idt_set_gate(61, (unsigned int)isr61, 0x08, 0x8e);
	idt_set_gate(62, (unsigned int)isr62, 0x08, 0x8e);
	idt_set_gate(63, (unsigned int)isr63, 0x08, 0x8e);
	idt_set_gate(64, (unsigned int)isr64, 0x08, 0x8e);
	idt_set_gate(65, (unsigned int)isr65, 0x08, 0x8e);
	idt_set_gate(66, (unsigned int)isr66, 0x08, 0x8e);
	idt_set_gate(67, (unsigned int)isr67, 0x08, 0x8e);
	idt_set_gate(68, (unsigned int)isr68, 0x08, 0x8e);
	idt_set_gate(69, (unsigned int)isr69, 0x08, 0x8e);
	idt_set_gate(70, (unsigned int)isr70, 0x08, 0x8e);
	idt_set_gate(71, (unsigned int)isr71, 0x08, 0x8e);
	idt_set_gate(72, (unsigned int)isr72, 0x08, 0x8e);
	idt_set_gate(73, (unsigned int)isr73, 0x08, 0x8e);
	idt_set_gate(74, (unsigned int)isr74, 0x08, 0x8e);
	idt_set_gate(75, (unsigned int)isr75, 0x08, 0x8e);
	idt_set_gate(76, (unsigned int)isr76, 0x08, 0x8e);
	idt_set_gate(77, (unsigned int)isr77, 0x08, 0x8e);
	idt_set_gate(78, (unsigned int)isr78, 0x08, 0x8e);
	idt_set_gate(79, (unsigned int)isr79, 0x08, 0x8e);
	idt_set_gate(80, (unsigned int)isr80, 0x08, 0x8e);
	idt_set_gate(81, (unsigned int)isr81, 0x08, 0x8e);
	idt_set_gate(82, (unsigned int)isr82, 0x08, 0x8e);
	idt_set_gate(83, (unsigned int)isr83, 0x08, 0x8e);
	idt_set_gate(84, (unsigned int)isr84, 0x08, 0x8e);
	idt_set_gate(85, (unsigned int)isr85, 0x08, 0x8e);
	idt_set_gate(86, (unsigned int)isr86, 0x08, 0x8e);
	idt_set_gate(87, (unsigned int)isr87, 0x08, 0x8e);
	idt_set_gate(88, (unsigned int)isr88, 0x08, 0x8e);
	idt_set_gate(89, (unsigned int)isr89, 0x08, 0x8e);
	idt_set_gate(90, (unsigned int)isr90, 0x08, 0x8e);
	idt_set_gate(91, (unsigned int)isr91, 0x08, 0x8e);
	idt_set_gate(92, (unsigned int)isr92, 0x08, 0x8e);
	idt_set_gate(93, (unsigned int)isr93, 0x08, 0x8e);
	idt_set_gate(94, (unsigned int)isr94, 0x08, 0x8e);
	idt_set_gate(95, (unsigned int)isr95, 0x08, 0x8e);
	idt_set_gate(96, (unsigned int)isr96, 0x08, 0x8e);
	idt_set_gate(97, (unsigned int)isr97, 0x08, 0x8e);
	idt_set_gate(98, (unsigned int)isr98, 0x08, 0x8e);
	idt_set_gate(99, (unsigned int)isr99, 0x08, 0x8e);
	idt_set_gate(100, (unsigned int)isr100, 0x08, 0x8e);
	idt_set_gate(101, (unsigned int)isr101, 0x08, 0x8e);
	idt_set_gate(102, (unsigned int)isr102, 0x08, 0x8e);
	idt_set_gate(103, (unsigned int)isr103, 0x08, 0x8e);
	idt_set_gate(104, (unsigned int)isr104, 0x08, 0x8e);
	idt_set_gate(105, (unsigned int)isr105, 0x08, 0x8e);
	idt_set_gate(106, (unsigned int)isr106, 0x08, 0x8e);
	idt_set_gate(107, (unsigned int)isr107, 0x08, 0x8e);
	idt_set_gate(108, (unsigned int)isr108, 0x08, 0x8e);
	idt_set_gate(109, (unsigned int)isr109, 0x08, 0x8e);
	idt_set_gate(110, (unsigned int)isr110, 0x08, 0x8e);
	idt_set_gate(111, (unsigned int)isr111, 0x08, 0x8e);
	idt_set_gate(112, (unsigned int)isr112, 0x08, 0x8e);
	idt_set_gate(113, (unsigned int)isr113, 0x08, 0x8e);
	idt_set_gate(114, (unsigned int)isr114, 0x08, 0x8e);
	idt_set_gate(115, (unsigned int)isr115, 0x08, 0x8e);
	idt_set_gate(116, (unsigned int)isr116, 0x08, 0x8e);
	idt_set_gate(117, (unsigned int)isr117, 0x08, 0x8e);
	idt_set_gate(118, (unsigned int)isr118, 0x08, 0x8e);
	idt_set_gate(119, (unsigned int)isr119, 0x08, 0x8e);
	idt_set_gate(120, (unsigned int)isr120, 0x08, 0x8e);
	idt_set_gate(121, (unsigned int)isr121, 0x08, 0x8e);
	idt_set_gate(122, (unsigned int)isr122, 0x08, 0x8e);
	idt_set_gate(123, (unsigned int)isr123, 0x08, 0x8e);
	idt_set_gate(124, (unsigned int)isr124, 0x08, 0x8e);
	idt_set_gate(125, (unsigned int)isr125, 0x08, 0x8e);
	idt_set_gate(126, (unsigned int)isr126, 0x08, 0x8e);
	idt_set_gate(127, (unsigned int)isr127, 0x08, 0x8e);
	idt_set_gate(128, (unsigned int)isr128, 0x08, 0x8e);
	idt_set_gate(129, (unsigned int)isr129, 0x08, 0x8e);
	idt_set_gate(130, (unsigned int)isr130, 0x08, 0x8e);
	idt_set_gate(131, (unsigned int)isr131, 0x08, 0x8e);
	idt_set_gate(132, (unsigned int)isr132, 0x08, 0x8e);
	idt_set_gate(133, (unsigned int)isr133, 0x08, 0x8e);
	idt_set_gate(134, (unsigned int)isr134, 0x08, 0x8e);
	idt_set_gate(135, (unsigned int)isr135, 0x08, 0x8e);
	idt_set_gate(136, (unsigned int)isr136, 0x08, 0x8e);
	idt_set_gate(137, (unsigned int)isr137, 0x08, 0x8e);
	idt_set_gate(138, (unsigned int)isr138, 0x08, 0x8e);
	idt_set_gate(139, (unsigned int)isr139, 0x08, 0x8e);
	idt_set_gate(140, (unsigned int)isr140, 0x08, 0x8e);
	idt_set_gate(141, (unsigned int)isr141, 0x08, 0x8e);
	idt_set_gate(142, (unsigned int)isr142, 0x08, 0x8e);
	idt_set_gate(143, (unsigned int)isr143, 0x08, 0x8e);
	idt_set_gate(144, (unsigned int)isr144, 0x08, 0x8e);
	idt_set_gate(145, (unsigned int)isr145, 0x08, 0x8e);
	idt_set_gate(146, (unsigned int)isr146, 0x08, 0x8e);
	idt_set_gate(147, (unsigned int)isr147, 0x08, 0x8e);
	idt_set_gate(148, (unsigned int)isr148, 0x08, 0x8e);
	idt_set_gate(149, (unsigned int)isr149, 0x08, 0x8e);
	idt_set_gate(150, (unsigned int)isr150, 0x08, 0x8e);
	idt_set_gate(151, (unsigned int)isr151, 0x08, 0x8e);
	idt_set_gate(152, (unsigned int)isr152, 0x08, 0x8e);
	idt_set_gate(153, (unsigned int)isr153, 0x08, 0x8e);
	idt_set_gate(154, (unsigned int)isr154, 0x08, 0x8e);
	idt_set_gate(155, (unsigned int)isr155, 0x08, 0x8e);
	idt_set_gate(156, (unsigned int)isr156, 0x08, 0x8e);
	idt_set_gate(157, (unsigned int)isr157, 0x08, 0x8e);
	idt_set_gate(158, (unsigned int)isr158, 0x08, 0x8e);
	idt_set_gate(159, (unsigned int)isr159, 0x08, 0x8e);
	idt_set_gate(160, (unsigned int)isr160, 0x08, 0x8e);
	idt_set_gate(161, (unsigned int)isr161, 0x08, 0x8e);
	idt_set_gate(162, (unsigned int)isr162, 0x08, 0x8e);
	idt_set_gate(163, (unsigned int)isr163, 0x08, 0x8e);
	idt_set_gate(164, (unsigned int)isr164, 0x08, 0x8e);
	idt_set_gate(165, (unsigned int)isr165, 0x08, 0x8e);
	idt_set_gate(166, (unsigned int)isr166, 0x08, 0x8e);
	idt_set_gate(167, (unsigned int)isr167, 0x08, 0x8e);
	idt_set_gate(168, (unsigned int)isr168, 0x08, 0x8e);
	idt_set_gate(169, (unsigned int)isr169, 0x08, 0x8e);
	idt_set_gate(170, (unsigned int)isr170, 0x08, 0x8e);
	idt_set_gate(171, (unsigned int)isr171, 0x08, 0x8e);
	idt_set_gate(172, (unsigned int)isr172, 0x08, 0x8e);
	idt_set_gate(173, (unsigned int)isr173, 0x08, 0x8e);
	idt_set_gate(174, (unsigned int)isr174, 0x08, 0x8e);
	idt_set_gate(175, (unsigned int)isr175, 0x08, 0x8e);
	idt_set_gate(176, (unsigned int)isr176, 0x08, 0x8e);
	idt_set_gate(177, (unsigned int)isr177, 0x08, 0x8e);
	idt_set_gate(178, (unsigned int)isr178, 0x08, 0x8e);
	idt_set_gate(179, (unsigned int)isr179, 0x08, 0x8e);
	idt_set_gate(180, (unsigned int)isr180, 0x08, 0x8e);
	idt_set_gate(181, (unsigned int)isr181, 0x08, 0x8e);
	idt_set_gate(182, (unsigned int)isr182, 0x08, 0x8e);
	idt_set_gate(183, (unsigned int)isr183, 0x08, 0x8e);
	idt_set_gate(184, (unsigned int)isr184, 0x08, 0x8e);
	idt_set_gate(185, (unsigned int)isr185, 0x08, 0x8e);
	idt_set_gate(186, (unsigned int)isr186, 0x08, 0x8e);
	idt_set_gate(187, (unsigned int)isr187, 0x08, 0x8e);
	idt_set_gate(188, (unsigned int)isr188, 0x08, 0x8e);
	idt_set_gate(189, (unsigned int)isr189, 0x08, 0x8e);
	idt_set_gate(190, (unsigned int)isr190, 0x08, 0x8e);
	idt_set_gate(191, (unsigned int)isr191, 0x08, 0x8e);
	idt_set_gate(192, (unsigned int)isr192, 0x08, 0x8e);
	idt_set_gate(193, (unsigned int)isr193, 0x08, 0x8e);
	idt_set_gate(194, (unsigned int)isr194, 0x08, 0x8e);
	idt_set_gate(195, (unsigned int)isr195, 0x08, 0x8e);
	idt_set_gate(196, (unsigned int)isr196, 0x08, 0x8e);
	idt_set_gate(197, (unsigned int)isr197, 0x08, 0x8e);
	idt_set_gate(198, (unsigned int)isr198, 0x08, 0x8e);
	idt_set_gate(199, (unsigned int)isr199, 0x08, 0x8e);
	idt_set_gate(200, (unsigned int)isr200, 0x08, 0x8e);
	idt_set_gate(201, (unsigned int)isr201, 0x08, 0x8e);
	idt_set_gate(202, (unsigned int)isr202, 0x08, 0x8e);
	idt_set_gate(203, (unsigned int)isr203, 0x08, 0x8e);
	idt_set_gate(204, (unsigned int)isr204, 0x08, 0x8e);
	idt_set_gate(205, (unsigned int)isr205, 0x08, 0x8e);
	idt_set_gate(206, (unsigned int)isr206, 0x08, 0x8e);
	idt_set_gate(207, (unsigned int)isr207, 0x08, 0x8e);
	idt_set_gate(208, (unsigned int)isr208, 0x08, 0x8e);
	idt_set_gate(209, (unsigned int)isr209, 0x08, 0x8e);
	idt_set_gate(210, (unsigned int)isr210, 0x08, 0x8e);
	idt_set_gate(211, (unsigned int)isr211, 0x08, 0x8e);
	idt_set_gate(212, (unsigned int)isr212, 0x08, 0x8e);
	idt_set_gate(213, (unsigned int)isr213, 0x08, 0x8e);
	idt_set_gate(214, (unsigned int)isr214, 0x08, 0x8e);
	idt_set_gate(215, (unsigned int)isr215, 0x08, 0x8e);
	idt_set_gate(216, (unsigned int)isr216, 0x08, 0x8e);
	idt_set_gate(217, (unsigned int)isr217, 0x08, 0x8e);
	idt_set_gate(218, (unsigned int)isr218, 0x08, 0x8e);
	idt_set_gate(219, (unsigned int)isr219, 0x08, 0x8e);
	idt_set_gate(220, (unsigned int)isr220, 0x08, 0x8e);
	idt_set_gate(221, (unsigned int)isr221, 0x08, 0x8e);
	idt_set_gate(222, (unsigned int)isr222, 0x08, 0x8e);
	idt_set_gate(223, (unsigned int)isr223, 0x08, 0x8e);
	idt_set_gate(224, (unsigned int)isr224, 0x08, 0x8e);
	idt_set_gate(225, (unsigned int)isr225, 0x08, 0x8e);
	idt_set_gate(226, (unsigned int)isr226, 0x08, 0x8e);
	idt_set_gate(227, (unsigned int)isr227, 0x08, 0x8e);
	idt_set_gate(228, (unsigned int)isr228, 0x08, 0x8e);
	idt_set_gate(229, (unsigned int)isr229, 0x08, 0x8e);
	idt_set_gate(230, (unsigned int)isr230, 0x08, 0x8e);
	idt_set_gate(231, (unsigned int)isr231, 0x08, 0x8e);
	idt_set_gate(232, (unsigned int)isr232, 0x08, 0x8e);
	idt_set_gate(233, (unsigned int)isr233, 0x08, 0x8e);
	idt_set_gate(234, (unsigned int)isr234, 0x08, 0x8e);
	idt_set_gate(235, (unsigned int)isr235, 0x08, 0x8e);
	idt_set_gate(236, (unsigned int)isr236, 0x08, 0x8e);
	idt_set_gate(237, (unsigned int)isr237, 0x08, 0x8e);
	idt_set_gate(238, (unsigned int)isr238, 0x08, 0x8e);
	idt_set_gate(239, (unsigned int)isr239, 0x08, 0x8e);
	idt_set_gate(240, (unsigned int)isr240, 0x08, 0x8e);
	idt_set_gate(241, (unsigned int)isr241, 0x08, 0x8e);
	idt_set_gate(242, (unsigned int)isr242, 0x08, 0x8e);
	idt_set_gate(243, (unsigned int)isr243, 0x08, 0x8e);
	idt_set_gate(244, (unsigned int)isr244, 0x08, 0x8e);
	idt_set_gate(245, (unsigned int)isr245, 0x08, 0x8e);
	idt_set_gate(246, (unsigned int)isr246, 0x08, 0x8e);
	idt_set_gate(247, (unsigned int)isr247, 0x08, 0x8e);
	idt_set_gate(248, (unsigned int)isr248, 0x08, 0x8e);
	idt_set_gate(249, (unsigned int)isr249, 0x08, 0x8e);
	idt_set_gate(250, (unsigned int)isr250, 0x08, 0x8e);
	idt_set_gate(251, (unsigned int)isr251, 0x08, 0x8e);
	idt_set_gate(252, (unsigned int)isr252, 0x08, 0x8e);
	idt_set_gate(253, (unsigned int)isr253, 0x08, 0x8e);
	idt_set_gate(254, (unsigned int)isr254, 0x08, 0x8e);
	idt_set_gate(255, (unsigned int)isr255, 0x08, 0x8e);
	idt_write((unsigned int)&idt_ptr);

	irq_setup();
	__asm__("sti");
}