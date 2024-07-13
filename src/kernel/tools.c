#include "pch.h"

// Write a byte out to the specified port
void outb(uint16_t port, uint8_t value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Read a byte from the specified port
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Read a word from the specified port
uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Write len copies of val into dest.
void memset_tool(uint8_t *dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for (; len != 0; len--) *temp++ = val;
}

// Convert int to ascii
char *itoa(int32_t value, char *str, uint32_t base) {
	char *rc;
	char *ptr;
	char *low;
	// Check if the base is valid
	if (base < 2 || base > 36)
	{
		*str = '\0';
		return str;
	}
	rc = ptr = str;
	// Set '-' for negative decimals
	if (value < 0 && base == 10)
		*ptr++ = '-';
	low = ptr;
	do
	{
		// Modulo is negative for negative value. This trick makes abs() unnecessary.
		*ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + value % base];
		value /= base;
	} while (value);
	// Terminating the string
	*ptr-- = '\0';
	// Invert the numbers
	while (low < ptr)
	{
		char tmp = *low;
		*low++ = *ptr;
		*ptr-- = tmp;
	}
	return rc;
}