#include "pch.h"
#include "isr.h"

#define PIC1_COMMAND 0x20
#define PIC2_COMMAND 0xA0
#define PIC_EOI 0x20 // End of interrupt

void (*isr_callbacks[16])();

void isr_handler(registers_t regs) {
	//screen_print("Recieved interrupt: ");
	//screen_print_int(regs.int_no, 16);
	//screen_print("\n");
}

void irq_handler(registers_t regs)
{
	outb(PIC1_COMMAND, PIC_EOI); // Ack to the main PIC
	if (regs.int_no >= IRQ8) { // Check if the interrupt is from the slave PIC
		outb(PIC2_COMMAND, PIC_EOI); // Ack also to the slave PIC
	}
	if (isr_callbacks[regs.int_no - IRQ0] != 0) {
		(*isr_callbacks[regs.int_no - IRQ0])();
	}
	else {
		//screen_print("Recieved IRQ: ");
		//screen_print_int(regs.int_no, 16);
		//screen_print("\n");
	}
}

// Function for assigning a callback function to a specific IRQ
void register_isr_callback(uint32_t irq, void (*callback)()) {
	isr_callbacks[irq - IRQ0] = callback;
}