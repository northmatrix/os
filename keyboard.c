#include "io.h"
#include "interrupt.h"
#include "pic.h"
#include "stdio.h"
#include "stdint.h" 

#define UNUSED_ARG(x) ((void)x)


static char scancode_to_ascii[128] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\','z','x',
    'c','v','b','n','m',',','.','/', 0, '*', 0,' ', 0, // up to 57
};

static char scancode_to_ascii[128];

static uint8_t is_shift = 0;

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


#define SHIFT_LEFT_SCANCODE 0x2A
#define SHIFT_RIGHT_SCANCODE 0x36
#define SHIFT_RELEASE_LEFT_SCANCODE 0xAA
#define SHIFT_RELEASE_RIGHT_SCANCODE 0xB6

void keyboard_interrupt_handler(cpu_state_t cpu_state,interrupt_detail_t interrupt_detail,stack_state_t stack_state) {
    UNUSED_ARG(cpu_state);
    UNUSED_ARG(stack_state);
    unsigned char scancode = read_scan_code();
    if (scancode == SHIFT_LEFT_SCANCODE || scancode == SHIFT_RIGHT_SCANCODE) {
        is_shift = 1;
    }
    else if (scancode == SHIFT_RELEASE_LEFT_SCANCODE || scancode == SHIFT_RELEASE_RIGHT_SCANCODE) {
        is_shift = 0;
    }

    if (scancode == 0x0E) {
        printf("%c",'\b');
        PIC_sendEOI(interrupt_detail.interrupt);
        return; 
    }
    else if (scancode < 128 && scancode_to_ascii[scancode] != 0) {
        char key = scancode_to_ascii[scancode];
        if (is_shift) {
            if ( key >= 'a' && key <= 'z')  {
                key -= 32;
            }
        }
        printf("%c",key);
    }
    PIC_sendEOI(interrupt_detail.interrupt);
    return;
}

void keyboard_init() {
    register_interrupt_handler(33,keyboard_interrupt_handler);
}
