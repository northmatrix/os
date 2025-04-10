#include "interrupt.h"
#include "stddef.h"
#include "stdio.h" 

static interrupt_handler_t interrupt_handlers[IDT_NUM_ENTRIES];

uint32_t register_interrupt_handler(uint32_t interrupt,
                                    interrupt_handler_t handler)
{
    if (interrupt > 255) {
        return 1;
    }
    if (interrupt_handlers[interrupt] != NULL) {
        return 1;
    }
    interrupt_handlers[interrupt] = handler;
    return 0;
}

void interrupt_handler(cpu_state_t state, interrupt_detail_t info, stack_state_t exec)
{
    sprintf("Interrupt: %d\n",info.interrupt);
    sprintf("EIP: %x\n",exec.eip);
    sprintf("EAX: %x\n",state.eax);
    if (interrupt_handlers[info.interrupt] != NULL) {
        interrupt_handlers[info.interrupt](state, info, exec);
    } else {
        sprintf("interrupt_handler unhandled interrupt: %d, eip: %d, cs: %d, eflags: %d\n",info.interrupt, exec.eip, exec.cs, exec.eflags);
    }
}
