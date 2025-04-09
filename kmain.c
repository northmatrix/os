#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 
#include "pic.h" 

void kmain() {
  // Initialization vga, serial, gdt, idt
  vga_initialize(); 
  serial_initialize(); 
  gdt_init(); 
  pic_init();
  idt_init();
  //writing
  __asm__ (
    "sti"
  );
  printf("Welcome To MyOS.\n");
}

