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

void serial_write(unsigned short com, char* data, unsigned int len ) {
  for ( unsigned int i = 0 ; i < len ; i ++) {
    while (!serial_is_transmit_fifo_empty(com));
    outb(SERIAL_DATA_PORT(com), data[i]);
  }
}

void serial_writestring(char* buf) {
  serial_write(SERIAL_COM1_BASE,buf,strlen(buf));
}


void serial_writeint(int num,int base) {
  char buffer[32];
  itoa(num,buffer,base);
  serial_writestring(buffer);
}
