#include "pch.h"

// Write a byte out to the specified port
inline void outb(uint16_t port, uint8_t value){
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Write a word out to the specified port
inline void outw(uint16_t port, uint16_t value) {
	asm volatile ("outw %1, %0" : : "dN" (port), "a" (value));
}

// Read a byte from the specified port
inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Read a word from the specified port
inline uint16_t inw(uint16_t port)
{
    uint16_t ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// Write len copies of val into dest
inline void memset_tool(void *dest, uint8_t val, size_t len)
{
	uint8_t *dest_copy = (uint8_t *)dest;
	for (; len != 0; len--) *dest_copy++ = val;
}

// Copy the value pointed to by src into dest
inline void memcpy_tool(void *dest, const void *src, size_t len)
{
	uint8_t *dest_copy = (uint8_t *)dest;
	const uint8_t *src_copy = (const uint8_t *)src;
	for (; len != 0; len--) *dest_copy++ = *src_copy++;
}


// Compare the value pointed to by src to the value pointed to by dest. Return values:
// -1 - the first byte that does not match in both memory blocks has a lower value in src than in dest (if evaluated as unsigned char values)
// 0 -the contents of both memory blocks are equal
// 1 - the first byte that does not match in both memory blocks has a greater value in src than in dest (if evaluated as unsigned char values)
int32_t memcmp_tool(void *dest, void *src, size_t len){
	uint8_t *d = (uint8_t *)dest;
	uint8_t *s = (uint8_t *)src;
	for(size_t i = 0; i < len; i++){
		if(s[i] < d[i])
			return -1;
		else if(s[i] > d[i])
			return 1;
	}
	return 0;
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