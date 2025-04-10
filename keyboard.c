#include "io.h"
#include "interrupt.h"
#include "pic.h"
#include "stdio.h"

#define UNUSED_ARG(x) ((void)x)

static char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
   '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
   'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x',
   'c','v','b','n','m',',','.','/', 0, '*', 0,' ', 0, // up to 57
  };

#define KBD_DATA_PORT   0x60

    /** read_scan_code:
     *  Reads a scan code from the keyboard
     *
     *  @return The scan code (NOT an ASCII character!)
     */
unsigned char read_scan_code(void)
{
    return inb(KBD_DATA_PORT);
}


void keyboard_interrupt_handler(cpu_state_t cpu_state,interrupt_detail_t interrupt_detail,stack_state_t stack_state) {
    UNUSED_ARG(cpu_state);
    UNUSED_ARG(stack_state);
    unsigned char c = read_scan_code();
    printf("%c",scancode_to_ascii[c]);
    PIC_sendEOI(interrupt_detail.interrupt);
}

void keyboard_init() {
    register_interrupt_handler(33,keyboard_interrupt_handler);
}
