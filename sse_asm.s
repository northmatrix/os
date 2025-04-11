global enableSSE



section .text

enableSSE:
      mov eax, cr0
      and eax, 0xFFFFFFFB
      or  eax, 0x2
      mov cr0, eax

      mov eax, cr4
      or  eax, 0x600
      mov cr4, eax
      ret
