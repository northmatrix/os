global load_gdt
global set_segments_and_jump

load_gdt:
    lgdt [esp+4]


set_segments_and_jump:
    ; Set up segment registers for data segments
    mov ax, 0x10 
    mov ds, ax        ; Set DS to the data segment (0x10 points to the second GDT entry)
    mov ss, ax        ; Set SS to the stack segment
    mov es, ax        ; Set ES to the extra segment

    ; Perform far jump to change CS (Code Segment) to 0x08
    jmp 0x08:flush_cs   ; Far jump to flush_cs

flush_cs:
    ; Now CS is changed to 0x08, and execution continues here
    ret
