#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 

void kmain() {
  // Initialization vga, serial, gdt, idt
  vga_initialize(); 
  serial_initialize(); 
  gdt_init(); 
  idt_init();
  //writing
  printf("Welcome To MyOS.\n");
  int y = 5;
  int x = 0;
  int z = y / x;
  printf("5 / 0 = %d",z);
}

