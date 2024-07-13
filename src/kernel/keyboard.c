#include "pch.h"
#include "keyboard.h"

void keyboard_interrupt_handler() {
	uint8_t scancode = inb(KEYBOARD_DATA_PORT);
	// Lookup table for US keyboard layout (partial)
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

	// Lookup table for Shift+key for US keyboard layout (partial)
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