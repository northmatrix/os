#include "vga.h"
#include "serial.h"
#include "gdt.h"

void kmain() {
  // Initialization of the Framebuffer and the Serial Port
  fb_initialize(); serial_initialize(); gdt_init();
  // Writing a string null terminated to the buffers 
  fb_writestring("spagetOS");
  serial_writestring("Testing t see if serial port works usign PORT IO out and in\n");
}
