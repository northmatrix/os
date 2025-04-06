#include "io.h"
#include "gdt.h"

/* VGA Color Enum */
#define FB_BLACK       0
#define FB_BLUE        1
#define FB_GREEN       2
#define FB_CYAN        3
#define FB_RED         4
#define FB_MAGENTA     5
#define FB_BROWN       6
#define FB_LIGHT_GREY  7
#define FB_DARK_GREY   8
#define FB_LIGHT_BLUE  9
#define FB_LIGHT_GREEN 10
#define FB_LIGHT_CYAN  11
#define FB_LIGHT_RED   12
#define FB_LIGHT_MAGENTA 13
#define FB_LIGHT_BROWN 14
#define FB_WHITE       15

/* Frame Buffer */
#define VGA_HEIGHT 25 
#define VGA_WIDTH 80 

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15

#define SERIAL_COM1_BASE                0x3F8      /* COM1 base port */
#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)
#define SERIAL_LINE_ENABLE_DLAB         0x80


volatile unsigned char *fb = (unsigned char *) 0x000B8000;
static unsigned int terminal_col;
static unsigned int terminal_row;



unsigned int strlen(char* string) {
  unsigned int i = 0;
  while (string[i] != '\0') {
    i++;
  }
  return i;
}

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

void serial_write(unsigned short com, char* data, unsigned int len ) {
  for ( unsigned int i = 0 ; i < len ; i ++) {
    while (!serial_is_transmit_fifo_empty(com));
    outb(SERIAL_DATA_PORT(com), data[i]);
  }
}

void serial_writestring(char* buf) {
  serial_write(SERIAL_COM1_BASE,buf,strlen(buf));
}

void fb_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
         fb[i] = c;
         fb[i + 1] = ((bg & 0x0F) << 4) | (fg & 0x0F);
}

void fb_shift_up() {
  //lets first shift everything up 1 discarding the first row and leaving the last row empty  
  for (unsigned int i = 1; i < VGA_HEIGHT; i ++ ) {
    for (unsigned int j = 0; j < VGA_WIDTH; j ++) {
      unsigned int src_idx = (VGA_WIDTH * i + j ) * 2;
      unsigned int dst_idx = (VGA_WIDTH * (i-1) + j ) * 2;
      fb[dst_idx] = fb[src_idx];         // Copy character
      fb[dst_idx + 1] = fb[src_idx + 1]; // Copy attribute (color)
    }
  }
  //clear the last row
  for (unsigned int i = 0; i < VGA_WIDTH; i++) {
        unsigned int idx = (VGA_WIDTH * (VGA_HEIGHT - 1) + i) * 2;
        fb[idx] = ' ';                     // Clear character
        fb[idx + 1] = (FB_BLACK << 4) | FB_GREEN; // Default color (black on green)
   }
   terminal_row --;
   terminal_col = 0;
   fb_move_cursor(terminal_row * VGA_WIDTH + terminal_col);
}

void fb_intitialize() {
  terminal_row = 0;
  terminal_col = 0;
  for(unsigned int i = 0; i < VGA_HEIGHT; i ++ ) {
    for(unsigned int j = 0; j < VGA_WIDTH; j ++ ) {
      fb_write_cell((i * VGA_WIDTH + j) * 2,' ',FB_GREEN,FB_BLACK);
    }
  } 
} 
  
void fb_write(char *buf, unsigned int len) {
  for (unsigned int i = 0; i < len; i ++) {
    if ( buf[i] == '\n') {
      terminal_row ++;
      terminal_col = 0;
      if (terminal_row == VGA_HEIGHT ) {
        fb_shift_up();
      }
    } else {
        fb_write_cell(((terminal_row * VGA_WIDTH) + terminal_col) * 2,buf[i],FB_GREEN,FB_BLACK);
        if ( ++terminal_col == VGA_WIDTH ) {
             terminal_col = 0;
             if ( ++terminal_row == VGA_HEIGHT ) {
                 fb_shift_up();
             }
        }
    }
  }
  fb_move_cursor(terminal_row * VGA_WIDTH + terminal_col);
}

void fb_writestring(char* buf) {
  fb_write(buf,strlen(buf));
}


// GDT entry (describes each segment in the GDT)
struct gdt_entry {
    unsigned short limit_low;  // Limit (bits 0-15)
    unsigned short base_low;   // Base (bits 0-15)
    unsigned char base_middle; // Base (bits 16-23)
    unsigned char access;      // Access flags
    unsigned char granularity; // Granularity flags
    unsigned char base_high;   // Base (bits 24-31)
} __attribute__((packed));


// GDT table (array of entries)
struct gdt_entry gdt[3]; // 3 GDT entries (null, code, and data)
struct gdt_ptr gp;        // Pointer to the GDT (size and address)

void init_gdt() {
    // Null Descriptor (Index 0)
    gdt[0].limit_low = 0x0000;
    gdt[0].base_low = 0x0000;
    gdt[0].base_middle = 0x00;
    gdt[0].access = 0x00;
    gdt[0].granularity = 0x00;
    gdt[0].base_high = 0x00;

    // Code Segment Descriptor (Index 1)
    gdt[1].limit_low = 0xFFFF;
    gdt[1].base_low = 0x0000;
    gdt[1].base_middle = 0x00;
    gdt[1].access = 0x9A;   // Executable, readable, access from ring 0
    gdt[1].granularity = 0xCF; // 4KB pages, 32-bit protected mode
    gdt[1].base_high = 0x00;

    // Data Segment Descriptor (Index 2)
    gdt[2].limit_low = 0xFFFF;
    gdt[2].base_low = 0x0000;
    gdt[2].base_middle = 0x00;
    gdt[2].access = 0x92;   // Writable, access from ring 0
    gdt[2].granularity = 0xCF; // 4KB pages, 32-bit protected mode
    gdt[2].base_high = 0x00;

    // Set up the GDT pointer
    gp.size = sizeof(struct gdt_entry) * 3 - 1;
    gp.address = (unsigned int)&gdt;

    // Load the GDT
    load_gdt(&gp);
}

void kmain() {
  fb_intitialize();
  serial_configure_baud_rate(SERIAL_COM1_BASE,1);
  serial_configure_line(SERIAL_COM1_BASE);
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello\n");
  fb_writestring("Hello24\n");
  fb_writestring("rizz\n");
  serial_writestring("Howdy\n");
}
