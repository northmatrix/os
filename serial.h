#ifndef INCLUDE_SERIAL_H
#define INCLUDE_SERIAL_H

#include "stddef.h"

void serial_initialize();
void serial_putchar(const char c);
void serial_write(const char* data,size_t base);
void serial_writestring(const char* data);


#endif

