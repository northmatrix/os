#include "string.h"
#include "io.h"
#include "stdint.h"
#include "stddef.h"

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


volatile uint8_t* fb = (uint8_t*) 0x000B8000;
static size_t terminal_col;
static size_t terminal_row;


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

void fb_initialize() {
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
