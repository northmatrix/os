#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 
#include "pic.h" 
#include "keyboard.h" 
#include "multiboot.h"

void kmain(multiboot_info_t multiboot_info) {
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
  printf("Welcome to RunexOS\n\n");
  printf("VGA Init\n");
  printf("COM1 Init\n");
  printf("GDT Init\n");
  printf("IDT Init\n");
  printf("KEYBOARD Init\n\n");
  printf("Multioot Module Address: 0x%x\n",multiboot_info.mods_addr);
  printf("Multioot Mem Lower: %dKB\n",multiboot_info.mem_lower);
  printf("Multioot Mem Upper: %dKB\n",multiboot_info.mem_upper);
  printf("\n");
}