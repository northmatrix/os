#include "interrupt.h"
#include "idt.h"
#include "stdio.h" 

#define NULL ((void*)0)

static interrupt_handler_t interrupt_handlers[IDT_NUM_ENTRIES] = { (interrupt_handler_t) NULL };

uint32_t register_interrupt_handler(uint32_t interrupt,
                                    interrupt_handler_t handler)
{
    if (interrupt > IDT_NUM_ENTRIES ) {
        return 1;
    }
    if (interrupt_handlers[interrupt] != 0) {
        return 1;
    }
    interrupt_handlers[interrupt] = handler;
    return 0;
}

void interrupt_handler(cpu_state_t state, interrupt_detail_t info, stack_state_t exec)
{
    sprintf("Interrupt Number: %d\n",info.interrupt);
    sprintf("Error Code: %d\n",info.error_code);
    sprintf("EIP: %x\n",exec.eip);
    sprintf("EAX: %x\n",state.eax);
    sprintf("EBP: %x\n",state.ebp);
    sprintf("EBX: %x\n",state.ebx);
    sprintf("ESI: %x\n",state.esi);
    sprintf("Handler Ptr: 0x%x\n\n",interrupt_handlers[info.interrupt]);
    if (interrupt_handlers[info.interrupt] != 0) {
        interrupt_handlers[info.interrupt](state, info, exec);
    }
}

