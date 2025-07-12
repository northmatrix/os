#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

#include "stdint.h" 

void outb(unsigned short port, unsigned char data);
uint8_t inb(unsigned short port);

#endif /* INCLUDE_IO_H */
