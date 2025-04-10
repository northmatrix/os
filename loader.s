global loader                   ; the entry symbol for ELF
extern kmain

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
ALIGN_MODULES   equ 0x00000001  ; tells grub to align modules
CHECKSUM     equ -(MAGIC_NUMBER + ALIGN_MODULES); calculate the checksum (magic number + checksum + flags should equal 0)

KERNEL_STACK_SIZE equ 4096      ; the stack size in bytes

section .bss                   ; start of the bss (uninitialized memory) section
align 4                         ; 4 byte (32 bit) alignment
kernel_stack:                   ; label points to the beggining of memory
    resb KERNEL_STACK_SIZE      ; reserve stack for the kernel
    

section .text                   ; start of the text (code) section
align 4                         ; the code must be 4 byte aligned
    dd MAGIC_NUMBER             ; write the magic number to the machine code,
    dd ALIGN_MODULES            ; align modules instruction 
    dd CHECKSUM                 ; and the checksum



loader:                                         ; the loader label (defined as entry point in linker script)
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ;point esp to the start of the stack ( end of memory area )
    mov ebp, esp
    push ebx
    call kmain 
    pop ebx

.loop:
    jmp .loop                   ; loop forever
