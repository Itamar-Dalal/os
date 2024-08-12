#ifndef TOOLS_H
#define TOOLS_H

void outb(uint16_t port, uint8_t value);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
void memset_tool(void *dest, uint8_t val, uint32_t len);
char *itoa(int32_t value, char *str, uint32_t base);

#endif