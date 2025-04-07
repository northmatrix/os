#ifndef INCLUDE_VGA_H
#define INCLUDE_VGA_H

void fb_initialize();
void fb_writestring(char* buf);
void fb_writeint(int num,int base);

#endif


