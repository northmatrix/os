#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h"
#include "stdio.h"

void kmain() {
  // Initialization vga, serial, gdt, idt
  vga_initialize(); 
  vga_setcolor(VGA_COLOR_LIGHT_BLUE);
  printf("VGA Initialized\n");
  serial_initialize(); 
  printf("SRL Initialized\n");
  gdt_init(); 
  printf("GDT Initialized\n");
  idt_init();
  printf("IDT Initialized\n\n");
  vga_setcolor(VGA_COLOR_LIGHT_GREY);
 
  printf("This is myosnameOS version 0.1.4\n\n");
}

