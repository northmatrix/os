#ifndef PIC_H
#define PIC_H 

#include "stdint.h"

void pic_init();
void PIC_sendEOI(uint8_t irq);

#endif 
