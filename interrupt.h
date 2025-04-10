#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "stdint.h" 
#include "idt.h"

struct cpu_state {
    uint32_t edi;
      uint32_t esi;
      uint32_t ebp;
      uint32_t edx;
      uint32_t ecx;
      uint32_t ebx;
      uint32_t eax;
  } __attribute__((packed));
typedef struct cpu_state cpu_state_t;

  struct interrupt_detail {
    unsigned int interrupt;
    unsigned int error_code;
  } __attribute__((packed));
typedef struct interrupt_detail interrupt_detail_t;
  
  struct stack_state {
      unsigned int eip;
      unsigned int cs;
      unsigned int eflags;
  } __attribute__((packed));
typedef struct stack_state stack_state_t;

typedef void (*interrupt_handler_t)(cpu_state_t cpu_state,
    interrupt_detail_t interrupt_detail,
    stack_state_t stack_state);

uint32_t register_interrupt_handler(uint32_t interrupt,
    interrupt_handler_t handler);

void interrupt_handler(cpu_state_t state, interrupt_detail_t info, stack_state_t exec);
#endif