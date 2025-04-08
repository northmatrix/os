#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"

void kmain() {
  // Initialization vga, serial, gdt, idt
  vga_initialize(); 
  serial_initialize(); 
  gdt_init(); 
  idt_init();
 
  vga_writestring("This is myosnameOS version 0.1.4\n");
  serial_writestring("This is mynameOS version 0.1.4 i think its working\n");
  vga_writestring("I am going to try and write something to vga output\n and hope it works i dont know why it wouldnt work surley it should\n if it doesnt them i dont know what to do");
}

