#include "string.h"
#include "io.h"
#include "stdlib.h"
#include "stdint.h"
#include "vga.h"
#include "stdio.h"
#include "constants.h"

/* Frame Buffer */
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY KERNEL_START_VADDR + 0x000B8000

/* The I/O ports */
#define FB_COMMAND_PORT         0x3D4
#define FB_DATA_PORT            0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15


static size_t terminal_column;
static size_t terminal_row; 
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

static inline uint8_t vga_entry_color(enum vga_color fg,enum vga_color bg) {
  return fg | bg << 4;
}

static inline uint16_t vga_entry(unsigned char uc,uint8_t color) {
  return (uint16_t) uc | (uint16_t) color << 8;
}



void vga_move_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT,    pos & 0x00FF);
}

void vga_putentryat(unsigned char c,uint8_t color, size_t x,size_t y)
{
         const size_t index = y * VGA_WIDTH + x;
         terminal_buffer[index] = vga_entry(c, color);
}

void vga_shift_up() {
  //lets first shift everything up 1 discarding the first row and leaving the last row empty  
  for (unsigned int i = 1; i < VGA_HEIGHT; i ++ ) {
    for (unsigned int j = 0; j < VGA_WIDTH; j ++) {
      unsigned int src_idx = (VGA_WIDTH * i + j );
      unsigned int dst_idx = (VGA_WIDTH * (i-1) + j );
      terminal_buffer[dst_idx] = terminal_buffer[src_idx];         // Copy character
    }
  }
  //clear the last row
  for (unsigned int i = 0; i < VGA_WIDTH; i++) {
        unsigned int idx = (VGA_WIDTH * (VGA_HEIGHT - 1) + i);
        terminal_buffer[idx] = ' ';                     // Clear character
   }
   terminal_row = VGA_HEIGHT - 1;
   terminal_column = 0;
   vga_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
}

void vga_backspace() {
  //if (--terminal_column == 0) {
  //  terminal_column = VGA_WIDTH;
  //  if (terminal_row != 0) {
  //    terminal_row  --;
  //  }
  //}
  if (terminal_column > 0) {
    --terminal_column;
  } else if (terminal_row > 0) {
    --terminal_row;
    for(terminal_column = VGA_WIDTH -1; terminal_column > 0; terminal_column --) {
      if ((terminal_buffer[terminal_row * VGA_WIDTH+ terminal_column] & 0xFF) != ' ') {
        break;
      }
      terminal_column++;
    }
  }
  vga_putentryat((char) ' ',terminal_color,terminal_column,terminal_row);
  vga_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
}


void vga_putchar(char c) {
  unsigned char uc = c;
  if (uc == '\n') {
    terminal_row++;
    terminal_column=0;
    if ( terminal_row == VGA_HEIGHT) {
      vga_shift_up();
    }
  } else if (uc == '\b') {
    vga_backspace();
  } else {
    vga_putentryat(uc,terminal_color,terminal_column,terminal_row);
    if(++terminal_column == VGA_WIDTH) {
      terminal_column = 0;
      if(++terminal_row==VGA_HEIGHT) {
        vga_shift_up();
      }
    }
  }
  const size_t index = terminal_row * VGA_WIDTH + terminal_column;
  vga_move_cursor(index);
}


void vga_initialize() {
  terminal_row = 0;
  terminal_column = 0;
  terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY,VGA_COLOR_BLACK);
  terminal_buffer = VGA_MEMORY;
  for(unsigned int i = 0; i < VGA_HEIGHT; i ++ ) {
    for(unsigned int j = 0; j < VGA_WIDTH; j ++ ) {
      const size_t index = i * VGA_WIDTH + j;
      terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
  } 
} 
  
void vga_write(const char* data, size_t size) {
  for (size_t i = 0; i < size; i++)
    vga_putchar(data[i]);
}


void vga_writestring(const char* data) {
  vga_write(data,strlen(data));
}

void vga_setcolor(enum vga_color fg) {
  terminal_color = vga_entry_color(fg,VGA_COLOR_BLACK);
}
