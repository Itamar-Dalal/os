#ifndef TOOLS_H
#define TOOLS_H

void outb(unsigned short port, unsigned char value);
unsigned char inb(unsigned short port);
unsigned short inw(unsigned short port);
void memset_tool(unsigned char *dest, unsigned char val, unsigned int len);
char *itoa(int value, char *str, int base);

#endif