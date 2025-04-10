#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 
#include "pic.h" 
#include "keyboard.h" 
#include "multiboot.h"

typedef void (*call_module_t)(void);

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
  printf("VGA INIT\n");
  printf("COM INIT\n");
  printf("GDT INIT\n");
  printf("IDT INIT\n");
  printf("KBD INIT\n\n");
  printf("Multioot Module Address: 0x%x\n",multiboot_info.mods_addr);
  printf("Multioot Mem Lower: %dKB\n",multiboot_info.mem_lower);
  printf("Multioot Mem Upper: %dKB\n\n",multiboot_info.mem_upper);
  printf("Look at all these colors.\n\n");
  for (uint8_t i = 1; i <= 15; i ++) {
    vga_setcolor(i);
    printf("%c%c",0xDB,0xDB);
  }
  call_module_t start_program = (call_module_t) multiboot_info.mods_addr;
  start_program();
}