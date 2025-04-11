OBJECTS = loader.o kmain.o io.o vga.o string.o serial.o gdt.o gdt_asm.o stdlib.o  interrupt_asm.o idt.o idt_asm.o  stdio.o pic.o interrupt.o keyboard.o paging.o paging_asm.o
CC = i686-elf-gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
		 -nostartfiles -nodefaultlibs -Wall -Wextra -Werror -fomit-frame-pointer \
		 -Wno-unused-function -c -mno-sse -mno-sse2 -mno-mmx -mno-3dnow -mfpmath=387 
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf

all: kernel.elf

kernel.elf: $(OBJECTS)
	ld $(LDFLAGS) $(OBJECTS) -o kernel.elf

os.iso: kernel.elf
	cp kernel.elf iso/boot/kernel.elf
	genisoimage -R                              \
							-b boot/grub/stage2_eltorito    \
							-no-emul-boot                   \
							-boot-load-size 4               \
							-A os                           \
							-input-charset utf8             \
							-quiet                          \
							-boot-info-table                \
							-o os.iso                       \
							iso

run: os.iso
	bochs -f bochsrc.txt -q

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -rf *.o kernel.elf os.iso

