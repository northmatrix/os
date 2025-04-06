#ifndef INCLUDE_GDT_H
#define INCLUDE_GDT_H

struct gdt_ptr {
    unsigned short size;   // Size of the GDT in bytes
    unsigned int address;  // Address of the GDT table
} __attribute__((packed));

void load_gdt(struct gdt_ptr* gdtable);

#endif /* INCLUDE_GDT_H */
