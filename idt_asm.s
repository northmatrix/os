global load_idt

section .text
; load idt - Loads the interrupt descriptor table (IDT).
; stack : [esp + 4] the address of the first entry in the IDT 
;         [esp]     the return address 
load_idt: 
    mov   eax, [esp + 4]
    lidt [eax]
    ret
