
#include "stdarg.h"
#include "vga.h"
#include "stdlib.h"
#include "serial.h"

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int num = va_arg(args, int);
                char buffer[32];
                itoa(num,buffer,10);
                vga_writestring(buffer);  // Assuming you have a print_dec function
            }
            else if (*fmt == 'x') {
                int num = va_arg(args, int);
                char buffer[32];
                itoa(num,buffer,16);
                vga_writestring(buffer);  // Assuming you have a print_dec function
            }
            else if (*fmt == 's') {
                const char* str = va_arg(args, const char*);
                vga_writestring(str);
            }
            else if (*fmt == 'c') {
                char c = (char)va_arg(args, int);
                vga_putchar(c);
            }
            else {
                vga_putchar('%');
                vga_putchar(*fmt);
            }
        } else {
            vga_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

void sprintf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'd') {
                int num = va_arg(args, int);
                char buffer[32];
                itoa(num,buffer,10);
                serial_writestring(buffer);  // Assuming you have a print_dec function
            }
            else if (*fmt == 'x') {
                int num = va_arg(args, int);
                char buffer[32];
                itoa(num,buffer,16);
                serial_writestring(buffer);  // Assuming you have a print_dec function
            }
            else if (*fmt == 's') {
                const char* str = va_arg(args, const char*);
                serial_writestring(str);
            }
            else if (*fmt == 'c') {
                char c = (char)va_arg(args, int);
                serial_putchar(c);
            }
            else {
                serial_putchar('%');
                serial_putchar(*fmt);
            }
        } else {
            serial_putchar(*fmt);
        }
        fmt++;
    }

    va_end(args);
}

