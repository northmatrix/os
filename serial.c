#include "serial.h"
#include "io.h"
#include "string.h"
#include "stdlib.h"

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */
#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)
#define SERIAL_LINE_ENABLE_DLAB         0x80


void serial_configure_baud_rate(unsigned short com, unsigned short divisor)
{
    outb(SERIAL_LINE_COMMAND_PORT(com),
             SERIAL_LINE_ENABLE_DLAB);
    outb(SERIAL_DATA_PORT(com),
             (divisor >> 8) & 0x00FF);
    outb(SERIAL_DATA_PORT(com),
             divisor & 0x00FF);
}

void serial_configure_line(unsigned short com)
{
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}

int serial_is_transmit_fifo_empty(unsigned int com)
{
    /* 0x20 = 0010 0000 */
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}


void serial_initialize() {
    serial_configure_baud_rate(SERIAL_COM1_BASE,1);
    serial_configure_line(SERIAL_COM1_BASE);
}

void serial_putchar(const char c) {
  while (!serial_is_transmit_fifo_empty(SERIAL_COM1_BASE));
  outb(SERIAL_DATA_PORT(SERIAL_COM1_BASE),c);
}

void serial_write(const char* data, size_t len ) {
  for ( size_t i = 0 ; i < len ; i ++ ) {
    serial_putchar(data[i]);
  }
}

void serial_writestring(const char* data) {
  serial_write(data,strlen(data));
}


void serial_writeint(int num,int base) {
  char buffer[32];
  itoa(num,buffer,base);
  serial_writestring(buffer);
}
