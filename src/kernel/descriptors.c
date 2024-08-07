#include "pch.h"
#include "descriptors.h"
#include "isr.h"
#include "keyboard.h"

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

#define ICW1_INIT    0x11
#define ICW4_8086    0x01

extern void gdt_write(uint32_t);
extern void idt_write(uint32_t);

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

gdt_entry_t gdt_entries[5];
idt_entry_t idt_entries[256];

gdt_entry_t gdt_set_gate(uint32_t base, uint32_t limit, uint8_t access, uint8_t granularity)
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
	gdt_ptr.base = (uint32_t)&gdt_entries;
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
	gdt_write((uint32_t)&gdt_ptr);
}

/*void watch() {
	static uint32_t time = 0;
	static uint32_t seconds = 0;
	if (time >= 1000) { // 1 second
		time %= 1000;
		seconds++;
		screen_print("Time: ", 0);
		screen_print_int(seconds, 10, 0);
		screen_print("s\n", 0);
	}
	time += 55; // PIT sends an interrupt every 55 milliseconds
}*/



void irq_setup() {
	// PIC information: https://ars.els-cdn.com/content/image/3-s2.0-B9780123914903000047-f04-05-9780123914903.jpg
	// ICW1
	outb(PIC1_COMMAND, ICW1_INIT);
	outb(PIC2_COMMAND, ICW1_INIT);
	// ICW2
	outb(PIC1_DATA, 0x20);
	outb(PIC2_DATA, 0x28);
	// ICW3
	outb(PIC1_DATA, 0x04); // 00000100 - IRQ2 (for slave PIC)
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

void idt_set_gate(int32_t index, uint32_t base, uint16_t selector, uint8_t flags) {
	idt_entries[index].base_low = base & 0xFFFF;
	idt_entries[index].base_high = (base >> 16) & 0xFFFF;
	idt_entries[index].selector = selector;
	idt_entries[index].always0 = 0;
	idt_entries[index].flags = flags;
}

void idt_setup() {
	idt_ptr_t idt_ptr;
	idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
	idt_ptr.base = (uint32_t)&idt_entries;

	idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8e); // 0x08 - selector to code segment ring 0
	idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8e);
	idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8e);
	idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8e);
	idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8e);
	idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8e);
	idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8e);
	idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8e);
	idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8e);
	idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8e);
	idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8e);
	idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8e);
	idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8e);
	idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8e);
	idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8e);
	idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8e);
	idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8e);
	idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8e);
	idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8e);
	idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8e);
	idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8e);
	idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8e);
	idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8e);
	idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8e);
	idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8e);
	idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8e);
	idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8e);
	idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8e);
	idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8e);
	idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8e);
	idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8e);
	idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8e);
	idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8e);
	idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8e);
	idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8e);
	idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8e);
	idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8e);
	idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8e);
	idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8e);
	idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8e);
	idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8e);
	idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8e);
	idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8e);
	idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8e);
	idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8e);
	idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8e);
	idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8e);
	idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8e);
	idt_set_gate(48, (uint32_t)isr48, 0x08, 0x8e);
	idt_set_gate(49, (uint32_t)isr49, 0x08, 0x8e);
	idt_set_gate(50, (uint32_t)isr50, 0x08, 0x8e);
	idt_set_gate(51, (uint32_t)isr51, 0x08, 0x8e);
	idt_set_gate(52, (uint32_t)isr52, 0x08, 0x8e);
	idt_set_gate(53, (uint32_t)isr53, 0x08, 0x8e);
	idt_set_gate(54, (uint32_t)isr54, 0x08, 0x8e);
	idt_set_gate(55, (uint32_t)isr55, 0x08, 0x8e);
	idt_set_gate(56, (uint32_t)isr56, 0x08, 0x8e);
	idt_set_gate(57, (uint32_t)isr57, 0x08, 0x8e);
	idt_set_gate(58, (uint32_t)isr58, 0x08, 0x8e);
	idt_set_gate(59, (uint32_t)isr59, 0x08, 0x8e);
	idt_set_gate(60, (uint32_t)isr60, 0x08, 0x8e);
	idt_set_gate(61, (uint32_t)isr61, 0x08, 0x8e);
	idt_set_gate(62, (uint32_t)isr62, 0x08, 0x8e);
	idt_set_gate(63, (uint32_t)isr63, 0x08, 0x8e);
	idt_set_gate(64, (uint32_t)isr64, 0x08, 0x8e);
	idt_set_gate(65, (uint32_t)isr65, 0x08, 0x8e);
	idt_set_gate(66, (uint32_t)isr66, 0x08, 0x8e);
	idt_set_gate(67, (uint32_t)isr67, 0x08, 0x8e);
	idt_set_gate(68, (uint32_t)isr68, 0x08, 0x8e);
	idt_set_gate(69, (uint32_t)isr69, 0x08, 0x8e);
	idt_set_gate(70, (uint32_t)isr70, 0x08, 0x8e);
	idt_set_gate(71, (uint32_t)isr71, 0x08, 0x8e);
	idt_set_gate(72, (uint32_t)isr72, 0x08, 0x8e);
	idt_set_gate(73, (uint32_t)isr73, 0x08, 0x8e);
	idt_set_gate(74, (uint32_t)isr74, 0x08, 0x8e);
	idt_set_gate(75, (uint32_t)isr75, 0x08, 0x8e);
	idt_set_gate(76, (uint32_t)isr76, 0x08, 0x8e);
	idt_set_gate(77, (uint32_t)isr77, 0x08, 0x8e);
	idt_set_gate(78, (uint32_t)isr78, 0x08, 0x8e);
	idt_set_gate(79, (uint32_t)isr79, 0x08, 0x8e);
	idt_set_gate(80, (uint32_t)isr80, 0x08, 0x8e);
	idt_set_gate(81, (uint32_t)isr81, 0x08, 0x8e);
	idt_set_gate(82, (uint32_t)isr82, 0x08, 0x8e);
	idt_set_gate(83, (uint32_t)isr83, 0x08, 0x8e);
	idt_set_gate(84, (uint32_t)isr84, 0x08, 0x8e);
	idt_set_gate(85, (uint32_t)isr85, 0x08, 0x8e);
	idt_set_gate(86, (uint32_t)isr86, 0x08, 0x8e);
	idt_set_gate(87, (uint32_t)isr87, 0x08, 0x8e);
	idt_set_gate(88, (uint32_t)isr88, 0x08, 0x8e);
	idt_set_gate(89, (uint32_t)isr89, 0x08, 0x8e);
	idt_set_gate(90, (uint32_t)isr90, 0x08, 0x8e);
	idt_set_gate(91, (uint32_t)isr91, 0x08, 0x8e);
	idt_set_gate(92, (uint32_t)isr92, 0x08, 0x8e);
	idt_set_gate(93, (uint32_t)isr93, 0x08, 0x8e);
	idt_set_gate(94, (uint32_t)isr94, 0x08, 0x8e);
	idt_set_gate(95, (uint32_t)isr95, 0x08, 0x8e);
	idt_set_gate(96, (uint32_t)isr96, 0x08, 0x8e);
	idt_set_gate(97, (uint32_t)isr97, 0x08, 0x8e);
	idt_set_gate(98, (uint32_t)isr98, 0x08, 0x8e);
	idt_set_gate(99, (uint32_t)isr99, 0x08, 0x8e);
	idt_set_gate(100, (uint32_t)isr100, 0x08, 0x8e);
	idt_set_gate(101, (uint32_t)isr101, 0x08, 0x8e);
	idt_set_gate(102, (uint32_t)isr102, 0x08, 0x8e);
	idt_set_gate(103, (uint32_t)isr103, 0x08, 0x8e);
	idt_set_gate(104, (uint32_t)isr104, 0x08, 0x8e);
	idt_set_gate(105, (uint32_t)isr105, 0x08, 0x8e);
	idt_set_gate(106, (uint32_t)isr106, 0x08, 0x8e);
	idt_set_gate(107, (uint32_t)isr107, 0x08, 0x8e);
	idt_set_gate(108, (uint32_t)isr108, 0x08, 0x8e);
	idt_set_gate(109, (uint32_t)isr109, 0x08, 0x8e);
	idt_set_gate(110, (uint32_t)isr110, 0x08, 0x8e);
	idt_set_gate(111, (uint32_t)isr111, 0x08, 0x8e);
	idt_set_gate(112, (uint32_t)isr112, 0x08, 0x8e);
	idt_set_gate(113, (uint32_t)isr113, 0x08, 0x8e);
	idt_set_gate(114, (uint32_t)isr114, 0x08, 0x8e);
	idt_set_gate(115, (uint32_t)isr115, 0x08, 0x8e);
	idt_set_gate(116, (uint32_t)isr116, 0x08, 0x8e);
	idt_set_gate(117, (uint32_t)isr117, 0x08, 0x8e);
	idt_set_gate(118, (uint32_t)isr118, 0x08, 0x8e);
	idt_set_gate(119, (uint32_t)isr119, 0x08, 0x8e);
	idt_set_gate(120, (uint32_t)isr120, 0x08, 0x8e);
	idt_set_gate(121, (uint32_t)isr121, 0x08, 0x8e);
	idt_set_gate(122, (uint32_t)isr122, 0x08, 0x8e);
	idt_set_gate(123, (uint32_t)isr123, 0x08, 0x8e);
	idt_set_gate(124, (uint32_t)isr124, 0x08, 0x8e);
	idt_set_gate(125, (uint32_t)isr125, 0x08, 0x8e);
	idt_set_gate(126, (uint32_t)isr126, 0x08, 0x8e);
	idt_set_gate(127, (uint32_t)isr127, 0x08, 0x8e);
	idt_set_gate(128, (uint32_t)isr128, 0x08, 0x8e);
	idt_set_gate(129, (uint32_t)isr129, 0x08, 0x8e);
	idt_set_gate(130, (uint32_t)isr130, 0x08, 0x8e);
	idt_set_gate(131, (uint32_t)isr131, 0x08, 0x8e);
	idt_set_gate(132, (uint32_t)isr132, 0x08, 0x8e);
	idt_set_gate(133, (uint32_t)isr133, 0x08, 0x8e);
	idt_set_gate(134, (uint32_t)isr134, 0x08, 0x8e);
	idt_set_gate(135, (uint32_t)isr135, 0x08, 0x8e);
	idt_set_gate(136, (uint32_t)isr136, 0x08, 0x8e);
	idt_set_gate(137, (uint32_t)isr137, 0x08, 0x8e);
	idt_set_gate(138, (uint32_t)isr138, 0x08, 0x8e);
	idt_set_gate(139, (uint32_t)isr139, 0x08, 0x8e);
	idt_set_gate(140, (uint32_t)isr140, 0x08, 0x8e);
	idt_set_gate(141, (uint32_t)isr141, 0x08, 0x8e);
	idt_set_gate(142, (uint32_t)isr142, 0x08, 0x8e);
	idt_set_gate(143, (uint32_t)isr143, 0x08, 0x8e);
	idt_set_gate(144, (uint32_t)isr144, 0x08, 0x8e);
	idt_set_gate(145, (uint32_t)isr145, 0x08, 0x8e);
	idt_set_gate(146, (uint32_t)isr146, 0x08, 0x8e);
	idt_set_gate(147, (uint32_t)isr147, 0x08, 0x8e);
	idt_set_gate(148, (uint32_t)isr148, 0x08, 0x8e);
	idt_set_gate(149, (uint32_t)isr149, 0x08, 0x8e);
	idt_set_gate(150, (uint32_t)isr150, 0x08, 0x8e);
	idt_set_gate(151, (uint32_t)isr151, 0x08, 0x8e);
	idt_set_gate(152, (uint32_t)isr152, 0x08, 0x8e);
	idt_set_gate(153, (uint32_t)isr153, 0x08, 0x8e);
	idt_set_gate(154, (uint32_t)isr154, 0x08, 0x8e);
	idt_set_gate(155, (uint32_t)isr155, 0x08, 0x8e);
	idt_set_gate(156, (uint32_t)isr156, 0x08, 0x8e);
	idt_set_gate(157, (uint32_t)isr157, 0x08, 0x8e);
	idt_set_gate(158, (uint32_t)isr158, 0x08, 0x8e);
	idt_set_gate(159, (uint32_t)isr159, 0x08, 0x8e);
	idt_set_gate(160, (uint32_t)isr160, 0x08, 0x8e);
	idt_set_gate(161, (uint32_t)isr161, 0x08, 0x8e);
	idt_set_gate(162, (uint32_t)isr162, 0x08, 0x8e);
	idt_set_gate(163, (uint32_t)isr163, 0x08, 0x8e);
	idt_set_gate(164, (uint32_t)isr164, 0x08, 0x8e);
	idt_set_gate(165, (uint32_t)isr165, 0x08, 0x8e);
	idt_set_gate(166, (uint32_t)isr166, 0x08, 0x8e);
	idt_set_gate(167, (uint32_t)isr167, 0x08, 0x8e);
	idt_set_gate(168, (uint32_t)isr168, 0x08, 0x8e);
	idt_set_gate(169, (uint32_t)isr169, 0x08, 0x8e);
	idt_set_gate(170, (uint32_t)isr170, 0x08, 0x8e);
	idt_set_gate(171, (uint32_t)isr171, 0x08, 0x8e);
	idt_set_gate(172, (uint32_t)isr172, 0x08, 0x8e);
	idt_set_gate(173, (uint32_t)isr173, 0x08, 0x8e);
	idt_set_gate(174, (uint32_t)isr174, 0x08, 0x8e);
	idt_set_gate(175, (uint32_t)isr175, 0x08, 0x8e);
	idt_set_gate(176, (uint32_t)isr176, 0x08, 0x8e);
	idt_set_gate(177, (uint32_t)isr177, 0x08, 0x8e);
	idt_set_gate(178, (uint32_t)isr178, 0x08, 0x8e);
	idt_set_gate(179, (uint32_t)isr179, 0x08, 0x8e);
	idt_set_gate(180, (uint32_t)isr180, 0x08, 0x8e);
	idt_set_gate(181, (uint32_t)isr181, 0x08, 0x8e);
	idt_set_gate(182, (uint32_t)isr182, 0x08, 0x8e);
	idt_set_gate(183, (uint32_t)isr183, 0x08, 0x8e);
	idt_set_gate(184, (uint32_t)isr184, 0x08, 0x8e);
	idt_set_gate(185, (uint32_t)isr185, 0x08, 0x8e);
	idt_set_gate(186, (uint32_t)isr186, 0x08, 0x8e);
	idt_set_gate(187, (uint32_t)isr187, 0x08, 0x8e);
	idt_set_gate(188, (uint32_t)isr188, 0x08, 0x8e);
	idt_set_gate(189, (uint32_t)isr189, 0x08, 0x8e);
	idt_set_gate(190, (uint32_t)isr190, 0x08, 0x8e);
	idt_set_gate(191, (uint32_t)isr191, 0x08, 0x8e);
	idt_set_gate(192, (uint32_t)isr192, 0x08, 0x8e);
	idt_set_gate(193, (uint32_t)isr193, 0x08, 0x8e);
	idt_set_gate(194, (uint32_t)isr194, 0x08, 0x8e);
	idt_set_gate(195, (uint32_t)isr195, 0x08, 0x8e);
	idt_set_gate(196, (uint32_t)isr196, 0x08, 0x8e);
	idt_set_gate(197, (uint32_t)isr197, 0x08, 0x8e);
	idt_set_gate(198, (uint32_t)isr198, 0x08, 0x8e);
	idt_set_gate(199, (uint32_t)isr199, 0x08, 0x8e);
	idt_set_gate(200, (uint32_t)isr200, 0x08, 0x8e);
	idt_set_gate(201, (uint32_t)isr201, 0x08, 0x8e);
	idt_set_gate(202, (uint32_t)isr202, 0x08, 0x8e);
	idt_set_gate(203, (uint32_t)isr203, 0x08, 0x8e);
	idt_set_gate(204, (uint32_t)isr204, 0x08, 0x8e);
	idt_set_gate(205, (uint32_t)isr205, 0x08, 0x8e);
	idt_set_gate(206, (uint32_t)isr206, 0x08, 0x8e);
	idt_set_gate(207, (uint32_t)isr207, 0x08, 0x8e);
	idt_set_gate(208, (uint32_t)isr208, 0x08, 0x8e);
	idt_set_gate(209, (uint32_t)isr209, 0x08, 0x8e);
	idt_set_gate(210, (uint32_t)isr210, 0x08, 0x8e);
	idt_set_gate(211, (uint32_t)isr211, 0x08, 0x8e);
	idt_set_gate(212, (uint32_t)isr212, 0x08, 0x8e);
	idt_set_gate(213, (uint32_t)isr213, 0x08, 0x8e);
	idt_set_gate(214, (uint32_t)isr214, 0x08, 0x8e);
	idt_set_gate(215, (uint32_t)isr215, 0x08, 0x8e);
	idt_set_gate(216, (uint32_t)isr216, 0x08, 0x8e);
	idt_set_gate(217, (uint32_t)isr217, 0x08, 0x8e);
	idt_set_gate(218, (uint32_t)isr218, 0x08, 0x8e);
	idt_set_gate(219, (uint32_t)isr219, 0x08, 0x8e);
	idt_set_gate(220, (uint32_t)isr220, 0x08, 0x8e);
	idt_set_gate(221, (uint32_t)isr221, 0x08, 0x8e);
	idt_set_gate(222, (uint32_t)isr222, 0x08, 0x8e);
	idt_set_gate(223, (uint32_t)isr223, 0x08, 0x8e);
	idt_set_gate(224, (uint32_t)isr224, 0x08, 0x8e);
	idt_set_gate(225, (uint32_t)isr225, 0x08, 0x8e);
	idt_set_gate(226, (uint32_t)isr226, 0x08, 0x8e);
	idt_set_gate(227, (uint32_t)isr227, 0x08, 0x8e);
	idt_set_gate(228, (uint32_t)isr228, 0x08, 0x8e);
	idt_set_gate(229, (uint32_t)isr229, 0x08, 0x8e);
	idt_set_gate(230, (uint32_t)isr230, 0x08, 0x8e);
	idt_set_gate(231, (uint32_t)isr231, 0x08, 0x8e);
	idt_set_gate(232, (uint32_t)isr232, 0x08, 0x8e);
	idt_set_gate(233, (uint32_t)isr233, 0x08, 0x8e);
	idt_set_gate(234, (uint32_t)isr234, 0x08, 0x8e);
	idt_set_gate(235, (uint32_t)isr235, 0x08, 0x8e);
	idt_set_gate(236, (uint32_t)isr236, 0x08, 0x8e);
	idt_set_gate(237, (uint32_t)isr237, 0x08, 0x8e);
	idt_set_gate(238, (uint32_t)isr238, 0x08, 0x8e);
	idt_set_gate(239, (uint32_t)isr239, 0x08, 0x8e);
	idt_set_gate(240, (uint32_t)isr240, 0x08, 0x8e);
	idt_set_gate(241, (uint32_t)isr241, 0x08, 0x8e);
	idt_set_gate(242, (uint32_t)isr242, 0x08, 0x8e);
	idt_set_gate(243, (uint32_t)isr243, 0x08, 0x8e);
	idt_set_gate(244, (uint32_t)isr244, 0x08, 0x8e);
	idt_set_gate(245, (uint32_t)isr245, 0x08, 0x8e);
	idt_set_gate(246, (uint32_t)isr246, 0x08, 0x8e);
	idt_set_gate(247, (uint32_t)isr247, 0x08, 0x8e);
	idt_set_gate(248, (uint32_t)isr248, 0x08, 0x8e);
	idt_set_gate(249, (uint32_t)isr249, 0x08, 0x8e);
	idt_set_gate(250, (uint32_t)isr250, 0x08, 0x8e);
	idt_set_gate(251, (uint32_t)isr251, 0x08, 0x8e);
	idt_set_gate(252, (uint32_t)isr252, 0x08, 0x8e);
	idt_set_gate(253, (uint32_t)isr253, 0x08, 0x8e);
	idt_set_gate(254, (uint32_t)isr254, 0x08, 0x8e);
	idt_set_gate(255, (uint32_t)isr255, 0x08, 0x8e);
	idt_write((uint32_t)&idt_ptr);

	irq_setup();
	__asm__("sti");
}