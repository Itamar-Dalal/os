#include "tools.h"

// Write a byte out to the specified port.
void outb(unsigned short port, unsigned char value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

unsigned char inb(unsigned short port)
{
    unsigned char ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

unsigned short inw(unsigned short port)
{
    unsigned short ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Write len copies of val into dest.
void memset_tool(unsigned char *dest, unsigned char val, unsigned int len)
{
    unsigned char *temp = (unsigned char *)dest;
    for (; len != 0; len--) *temp++ = val;
}

// Convert int to ascii
char *itoa(int value, char *str, int base) {
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