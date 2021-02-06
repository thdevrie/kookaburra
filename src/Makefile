CC=i686-elf-gcc
AS=i686-elf-as
ODIR=out
INCLUDES=headers
MODULES=asm
CFLAGS=-std=gnu99 -ggdb -mgeneral-regs-only -ffreestanding -O2 -Wall -Wextra -I$(INCLUDES)
LFLAGS=-ffreestanding -O2 -nostdlib 

_OBJ=boot.o \
 main.o \
 terminal.o \
 util.o \
 io.o \
 vga_text.o \
 interrupts.o \
 keyboard.o \
 cpuio.o \
 irq_handlers.o 

.PHONY: all
all: out out/os.iso 

.PHONY: boot
boot: all 
	qemu-system-i386 -s -cdrom out/os.iso

out/os.iso: grub.cfg $(ODIR)/os.bin 
	mkdir -p $(ODIR)/isodir/boot/grub
	cp grub.cfg $(ODIR)/isodir/boot/grub
	cp $(ODIR)/os.bin $(ODIR)/isodir/boot
	grub-mkrescue -o $@ $(ODIR)/isodir

OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/os.bin: linker.ld $(OBJ)
	$(CC) -T $< -o $@ $(LFLAGS) $(OBJ) -lgcc

$(ODIR)/%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(ODIR)/%.o: %.S
	$(AS) -ggdb $< -o $@

.PHONY: clean
clean: 
	rm -rf out
	rm os.iso

.PHONY: $(ODIR)
out: 
	mkdir -p $(ODIR)