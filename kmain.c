#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"
#include "stdio.h" 
#include "pic.h" 
#include "keyboard.h" 
#include "multiboot.h"
#include "interrupt.h"
#include "constants.h"

struct kernel_meminfo {
    uint32_t kernel_physical_start;
    uint32_t kernel_physical_end;
    uint32_t kernel_virtual_start;
    uint32_t kernel_virtual_end;
} __attribute__((packed));

typedef struct kernel_meminfo kernel_meminfo_t;

static multiboot_info_t *remap_multiboot_info(uint32_t mbaddr)
{
    multiboot_info_t *mbinfo = (multiboot_info_t *) PHYSICAL_TO_VIRTUAL(mbaddr);

    mbinfo->mmap_addr = PHYSICAL_TO_VIRTUAL(mbinfo->mmap_addr);
    mbinfo->mods_addr = PHYSICAL_TO_VIRTUAL(mbinfo->mods_addr);

    return mbinfo;
}

typedef void (*call_module_t)(void);


void kmain(uint32_t mbaddr, uint32_t magic_number, kernel_meminfo_t mem, uint32_t kernel_pdt_vaddr, uint32_t kernel_pt_vaddr) {
  vga_initialize(); 
  serial_initialize(); 
  gdt_init();
  idt_init();
  pic_init();
  keyboard_init();
  enable_interrupts();

  printf("\nKernel Pt Vaddr: %x\n",kernel_pt_vaddr);
  printf("Kernel Pdt Vaddr: %x\n",kernel_pdt_vaddr);
  printf("Magic Number: %x\n", magic_number);
  printf("Kernel Physical Start: %x\n", mem.kernel_physical_start);
  printf("Kernel Physical End: %x\n", mem.kernel_physical_end);
  printf("Kernel Virtual Start: %x\n", mem.kernel_virtual_start);
  printf("Kernel Virtual End: %x\n\n", mem.kernel_virtual_end);

  printf("Welcome to RunexOS\n\n");
  printf("VGA INIT\n");
  printf("COM INIT\n");
  printf("GDT INIT\n");
  printf("IDT INIT\n");
  printf("KBD INIT\n");
  printf("PGN INIT\n\n");
  sprintf("Testing serial works\n");

  // multiboot_info_t *mbinfo = remap_multiboot_info(ebx);
  // unsigned int address_of_module = mbinfo->mods_addr;
  // unsigned int module_count = mbinfo->mods_count;
  //
  // for (unsigned int i = 0; i < module_count; i++) {
  //    multiboot_module_t *mod = (multiboot_module_t *)(address_of_module + i * sizeof(multiboot_module_t));
  //    unsigned int module_start = mod->mod_start;
  //    unsigned int module_end = mod->mod_end;
  //
  //    sprintf("Loading module from 0x%x to 0x%x\n", module_start, module_end);
  //
  //    if (module_end > module_start) {
  //      call_module_t start_program = (call_module_t) module_start;
  //      start_program();
  //      sprintf("Program has run\n");
  //    }
  //  }

  //printf("Multioot Mem Lower: %dKB\n",mbinfo->mem_lower);
  //printf("Multioot Mem Upper: %dKB\n\n",mbinfo->mem_upper);
  printf("Look at all these colors.\n\n");

  for (uint8_t i = 1; i <= 15; i++) {
    vga_setcolor(i);
    printf("%c", 0xDB);
  }
  printf("\n\n");
  vga_setcolor(VGA_COLOR_LIGHT_GREY);
}

