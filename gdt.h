#ifndef GDT_H
#define GDT_H

#include "stdint.h"

#define PL0 0x0
#define PL3 0x3

void gdt_init();

#endif /* GDT_H */
