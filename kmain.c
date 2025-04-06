#include "vga.h"
#include "serial.h"

void kmain() {
  // Initialization of the Framebuffer and the Serial Port
  fb_initialize(); serial_initialize();
  // Writing a string null terminated to the buffers 
  fb_writestring("Testing to see if this framebuffer with memory mapped IO works\nCertified Rizz Lord");
  serial_writestring("Testing t see if serial port works usign PORT IO out and in\n");
}
