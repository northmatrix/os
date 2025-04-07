#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"

void kmain() {
  // Initialization of the Framebuffer and the Serial Port
  fb_initialize(); serial_initialize(); gdt_init(); idt_init();
  // Writing a string null terminated to the buffers 
  fb_writestring("This is my operating system if this text appears then it is working.\n");

  int y = 5;
  int x = 0;
  int z = y /x ;
  fb_writeint(z,10);
}

