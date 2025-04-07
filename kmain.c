#include "vga.h"
#include "serial.h"
#include "gdt.h"
#include "idt.h"

void kmain() {
  // Initialization of the Framebuffer and the Serial Port
  fb_initialize(); serial_initialize(); gdt_init(); idt_init();
  // Writing a string null terminated to the buffers 
  fb_writestring("This is an operating system\n");
  fb_writestring("Developed by me\n");
  fb_writestring("Is it a good operating system fuck no are you stupid its a shit one\n");
  fb_writestring("Are you ready for some maths\n");
  int arr[10];  // Array of size 10
  int index = 20;  // Invalid index, exceeding array bounds
  arr[index] = 42;
  serial_writeint(arr[5],10);
}

