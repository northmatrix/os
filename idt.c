#include "idt.h"
#include "serial.h"
#include "vga.h"

#define IDT_TRAP_GATE_TYPE		1 
#define PL0 0x0
#define PL3 0x3
#define SEGSEL_KERNEL_CS 0x08

struct idt_ptr { 
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

struct cpu_state {
  uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t edx;
	uint32_t ecx;
	uint32_t ebx;
	uint32_t eax;
	uint32_t esp;
} __attribute__((packed));

struct stack_state {
    unsigned int error_code;
    unsigned int eip;
    unsigned int cs;
    unsigned int eflags;
} __attribute__((packed));

struct idt_gate {
	uint16_t handler_low;
	uint16_t segsel;
	uint8_t zero;
	uint8_t config;
	uint16_t handler_high;
} __attribute__((packed));
typedef struct idt_gate idt_gate_t;


//external asm instructions
void interrupt_handler_0(void);
void interrupt_handler_1(void);
void interrupt_handler_2(void);
void interrupt_handler_3(void);


idt_gate_t idt[IDT_NUM_ENTRIES];

/* external assembly function to set the gdt */
void load_idt(uint32_t* idt_ptr_t);

void interrupt_handler(struct cpu_state cpu, struct stack_state stack, unsigned int interrupt) {
  serial_writeint(interrupt,16);
  serial_writeint(cpu.eax,16);
  serial_writeint(stack.cs,16);
  fb_writestring("\nDO NOT DIVIDE BY ZERO");
  return;
}

static void create_idt_gate(uint8_t n, uint32_t handler, uint8_t type, uint8_t pl) {
    idt[n].handler_low = handler & 0x0000FFFF; // this is equiv to only the  lower 16 as the hex is a and mask of just 1111 1111 1111 1111
    idt[n].handler_high = (handler >> 16) & 0x0000FFFF; //this is the same thing but handler is shifted 16 to the right and then masked

    idt[n].segsel = SEGSEL_KERNEL_CS;
    idt[n].zero = 0;

    idt[n].config =
        (0x01 << 7)          |
        ((pl & 0x03)  << 5)  |
        (0x01 << 3)          |
        (0x01 << 2)          |
        (0x01 << 1)          |
        type;
}

void idt_init() {
  idt_ptr_t idt_ptr;
  idt_ptr.limit = IDT_NUM_ENTRIES * sizeof(idt_gate_t) -1;
  idt_ptr.base = (uint32_t) &idt;
  create_idt_gate(0,(uint32_t) &interrupt_handler_0,IDT_TRAP_GATE_TYPE,PL0);
  create_idt_gate(1,(uint32_t) &interrupt_handler_1,IDT_TRAP_GATE_TYPE,PL0); 
  create_idt_gate(2,(uint32_t) &interrupt_handler_2,IDT_TRAP_GATE_TYPE,PL0); 
  create_idt_gate(3,(uint32_t) &interrupt_handler_3,IDT_TRAP_GATE_TYPE,PL0); 
  load_idt((uint32_t*) &idt_ptr);
  serial_writestring("i thinkt the idt is ready");
}


