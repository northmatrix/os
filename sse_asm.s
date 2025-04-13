global enableSSE



section .text

enableSSE:
    ; Clear EM (Emulation) bit in CR0 to enable SSE
    mov eax, cr0
    and eax, 0xFFFFFFFB      ; Clear EM (bit 2)
    or  eax, 0x2             ; Set ET (bit 1)
    mov cr0, eax

    ; Enable SSE/SSE2 in CR4 (OSFXSR bit - bit 9 and XMM - bit 10)
    mov eax, cr4
    or  eax, 0x600           ; Set OSFXSR (bit 9) and XMM (bit 10)
    mov cr4, eax

    ; Ensure FPU state is initialized (optional but recommended)
    fninit                   ; Initialize the FPU state
    ret

tempenableSSE:
      mov eax, cr0
      and eax, 0xFFFFFFFB
      or  eax, 0x2
      mov cr0, eax

      mov eax, cr4
      or  eax, 0x600
      mov cr4, eax
      ret
