
// Multiboot constants
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */
 
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
 

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:
 

.section .text
// Globaly descriptor table this defines the memory map for the kernel 
gdt:
	// Null
	.quad 0x0000000000000000
	// Code
	.short 0xFFFF
	.short 0x0000
	.byte  0x00
	.byte  0b10011010
	.byte  0b11001111
	.byte  0x00
	// Data
	.short 0xFFFF
	.short 0x0000
	.byte  0x00
	.byte  0b10010010
	.byte  0b11001111
	.byte  0x00
.set GDT_SIZE, . - gdt
gdt_descr:
	.short GDT_SIZE
	.int gdt
idt_descr: 
	.short 0x800
	// Defined setup and populated in interrupts.c
	.int IDT

_start:
	// initialize stack
	mov $stack_top, %esp
	cli
	// initialize memory lay out 
	lgdt gdt_descr
	movw $0x0010, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
	// jump to reset segment layout
	jmp $0x0008, $boot
boot:
	// Configure IRQ's and Interupt Descriptor Table
	call bootstrap_irq
	lidt idt_descr
	sti

	// Bootstrap the kernel
	call kernel_main
	
	// Drop i
1:	hlt
	call run_follow_up_task
	jmp 1b



