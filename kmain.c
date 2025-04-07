#include "vga.h"
#include "serial.h"
#include "gdt.h"

void kmain() {
  // Initialization of the Framebuffer and the Serial Port
  fb_initialize(); serial_initialize(); gdt_init();
  // Writing a string null terminated to the buffers 
  fb_writestring("This is an operating system\n");
  fb_writestring("Developed by Northmatrix\n");
  fb_writeint(518,10);
  fb_writestring("\nThat was a number\n");
  serial_writestring("testing the output\n");
}
