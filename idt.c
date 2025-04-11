#include "idt.h"

#define IDT_TRAP_GATE_TYPE		1 
#define PL0 0x0
#define PL3 0x3
#define SEGSEL_KERNEL_CS 0x08


#define CREATE_IDT_GATE(idx) \
    create_idt_gate(idx, (uint32_t) &interrupt_handler_##idx,\
                    IDT_TRAP_GATE_TYPE, PL0);

#define DECLARE_INTERRUPT_HANDLER(i) void interrupt_handler_##i(void)
              
struct idt_ptr { 
  uint16_t limit;
  uint32_t base;
} __attribute__((packed));
typedef struct idt_ptr idt_ptr_t;


struct idt_gate {
	uint16_t handler_low;
	uint16_t segsel;
	uint8_t zero;
	uint8_t config;
	uint16_t handler_high;
} __attribute__((packed));
typedef struct idt_gate idt_gate_t;


//external asm instructions
DECLARE_INTERRUPT_HANDLER(0);
DECLARE_INTERRUPT_HANDLER(1);
DECLARE_INTERRUPT_HANDLER(2);
DECLARE_INTERRUPT_HANDLER(3);
DECLARE_INTERRUPT_HANDLER(4);
DECLARE_INTERRUPT_HANDLER(5);
DECLARE_INTERRUPT_HANDLER(6);
DECLARE_INTERRUPT_HANDLER(7);
DECLARE_INTERRUPT_HANDLER(8);
DECLARE_INTERRUPT_HANDLER(9);
DECLARE_INTERRUPT_HANDLER(10);
DECLARE_INTERRUPT_HANDLER(11);
DECLARE_INTERRUPT_HANDLER(12);
DECLARE_INTERRUPT_HANDLER(13);
DECLARE_INTERRUPT_HANDLER(14);
DECLARE_INTERRUPT_HANDLER(15);
DECLARE_INTERRUPT_HANDLER(16);
DECLARE_INTERRUPT_HANDLER(17);
DECLARE_INTERRUPT_HANDLER(18);
DECLARE_INTERRUPT_HANDLER(19);

/* IRQs */
DECLARE_INTERRUPT_HANDLER(32);
DECLARE_INTERRUPT_HANDLER(33);
DECLARE_INTERRUPT_HANDLER(34);
DECLARE_INTERRUPT_HANDLER(35);
DECLARE_INTERRUPT_HANDLER(36);
DECLARE_INTERRUPT_HANDLER(37);
DECLARE_INTERRUPT_HANDLER(38);
DECLARE_INTERRUPT_HANDLER(39);
DECLARE_INTERRUPT_HANDLER(40);
DECLARE_INTERRUPT_HANDLER(41);
DECLARE_INTERRUPT_HANDLER(42);
DECLARE_INTERRUPT_HANDLER(43);
DECLARE_INTERRUPT_HANDLER(44);
DECLARE_INTERRUPT_HANDLER(45);
DECLARE_INTERRUPT_HANDLER(46);
DECLARE_INTERRUPT_HANDLER(47);


static idt_gate_t idt[IDT_NUM_ENTRIES];

/* external assembly function to set the gdt */
void load_idt(uint32_t idt_ptr_t);


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
  CREATE_IDT_GATE(0);
  CREATE_IDT_GATE(1);
  CREATE_IDT_GATE(2);
  CREATE_IDT_GATE(3);
  CREATE_IDT_GATE(4);
  CREATE_IDT_GATE(5);
  CREATE_IDT_GATE(6);
  CREATE_IDT_GATE(7);
  CREATE_IDT_GATE(8);
  CREATE_IDT_GATE(9);
  CREATE_IDT_GATE(10);
  CREATE_IDT_GATE(11);
  CREATE_IDT_GATE(12);
  CREATE_IDT_GATE(13);
  CREATE_IDT_GATE(14);
  CREATE_IDT_GATE(15);
  CREATE_IDT_GATE(16);
  CREATE_IDT_GATE(17);
  CREATE_IDT_GATE(18);
  CREATE_IDT_GATE(19);
  /* IRQs */
  CREATE_IDT_GATE(32);
  CREATE_IDT_GATE(33);
  CREATE_IDT_GATE(34);
  CREATE_IDT_GATE(35);
  CREATE_IDT_GATE(36);
  CREATE_IDT_GATE(37);
  CREATE_IDT_GATE(38);
  CREATE_IDT_GATE(39);
  CREATE_IDT_GATE(40);
  CREATE_IDT_GATE(41);
  CREATE_IDT_GATE(42);
  CREATE_IDT_GATE(43);
  CREATE_IDT_GATE(44);
  CREATE_IDT_GATE(45);
  CREATE_IDT_GATE(46);
  CREATE_IDT_GATE(47);
  load_idt((uint32_t) &idt_ptr);
}


