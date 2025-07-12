
#include "stdarg.h"
#include "vga.h"
#include "stdlib.h"
#include "serial.h"
#include "string.h" 

void ftoa(float n, char *res, int precision) {
    int ipart = (int)n;
    float fpart = n - ipart;

    itoa(ipart, res, 10); // Convert the integer part to string
    int i = strlen(res);
    res[i++] = '.'; // Add the decimal point

    // Process the fractional part
    for (int j = 0; j < precision; j++) {
        fpart *= 10;
        int digit = (int)fpart;
        res[i++] = '0' + digit;
        fpart -= digit;
    }
    res[i] = '\0';
}

void printf(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            if (*fmt == 'f') {
                float num = va_arg(args, double);  // floats are promoted to double
                char buffer[64];
                ftoa(num, buffer, 6);  // Convert float to string (6 decimals)
                vga_writestring(buffer); // Print to VGA or use standard puts
            }
            else if (*fmt == 'd') {
                int num = va_arg(args, int);
                char buffer[32];
                itoa(num,buffer,10);
                vga_writestring(buffer);  // Assuming you have a print_dec function
            }
            else if (*fmt == 'x') {
                unsigned int num = va_arg(args, unsigned int);
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
            if (*fmt == 'f') {
                float num = va_arg(args, double);  // floats are promoted to double
                char buffer[64];
                ftoa(num, buffer, 6);  // Convert float to string (6 decimals)
                serial_writestring(buffer); // Print to VGA or use standard puts
            }
            else if (*fmt == 'd') {
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

