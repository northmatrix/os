#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 
#include "pic.h" 
#include "keyboard.h" 
#include "multiboot.h"
#include "interrupt.h"

typedef void (*call_module_t)(void);

void kmain(unsigned int ebx) {
  vga_initialize(); 
  serial_initialize(); 
  gdt_init();
  idt_init();
  init_interrupt();
  pic_init();
  keyboard_init();

  printf("Welcome to RunexOS\n\n");
  printf("VGA INIT\n");
  printf("COM INIT\n");
  printf("GDT INIT\n");
  printf("IDT INIT\n");
  printf("KBD INIT\n\n");
  sprintf("Testing serial works\n");

  multiboot_info_t *mbinfo = (multiboot_info_t *) ebx;
  unsigned int address_of_module = mbinfo->mods_addr;
  unsigned int module_count = mbinfo->mods_count;

  for (unsigned int i = 0; i < module_count; i++) {
    multiboot_module_t *mod = (multiboot_module_t *)(address_of_module + i * sizeof(multiboot_module_t));
    unsigned int module_start = mod->mod_start;
    unsigned int module_end = mod->mod_end;

    sprintf("Loading module from 0x%x to 0x%x\n", module_start, module_end);

    if (module_end > module_start) {
      call_module_t start_program = (call_module_t) module_start;
      start_program();
      sprintf("Program has run\n");
    }
  }

  printf("Multioot Mem Lower: %dKB\n",mbinfo->mem_lower);
  printf("Multioot Mem Upper: %dKB\n\n",mbinfo->mem_upper);
  printf("Look at all these colors.\n\n");

  for (uint8_t i = 1; i <= 15; i++) {
    vga_setcolor(i);
    printf("%c%c", 0xDB, 0xDB);
  }
  vga_setcolor(VGA_COLOR_LIGHT_GREY);
}

