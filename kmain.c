#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 
#include "pic.h" 
#include "keyboard.h" 

void kmain() {
  // Initialization vga, serial, gdt, idt
  vga_initialize(); 
  serial_initialize(); 
  gdt_init();
  idt_init();
  pic_init();
  keyboard_init();
  //writing
  __asm__ (
    "sti"
  );
  printf("Welcome To MyOS.\n");
  printf("So far i have enabled VGA\nSERIAL COM1\nThe global descriptor table\nThe interrupt descriptor table\nAnd the Programmable interrupt controller\n");
}

