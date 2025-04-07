#ifndef INCLUDE_SERIAL_H
#define INCLUDE_SERIAL_H

void serial_initialize();
void serial_writestring(char* buf);
void serial_writeint(int num,int base);
#endif

