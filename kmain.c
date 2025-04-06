#include "vga.h"
#include "serial.h"
#include "gdt.h"

void kmain() {
  // Initialization of the Framebuffer and the Serial Port
  fb_initialize(); serial_initialize(); gdt_init();
  // Writing a string null terminated to the buffers 
  fb_writestring("spagOS");
  serial_writestring("testing the output\n");
}
