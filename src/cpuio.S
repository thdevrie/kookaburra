.section .text
.global out32
.global out16
.global out8
.global in32
.global in16
.global in8
.global template
.global irq0
.global instra
out32:
    push    %ebp
    movl    %esp, %ebp
    movl	8(%ebp), %edx
    movl    8+4(%ebp), %eax
    outl    %dx 
    pop     %ebp
    ret
.size out32, . - out32

out16:
    push    %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %edx
    movl    8+4(%ebp), %eax
    outw    %dx
    pop     %ebp
    ret
.size out16, . - out16

out8:
	push	%ebp
	movl	%esp, %ebp
	movl	8(%ebp), %edx
	movl	8+4(%ebp), %eax	
	outb	%dx	
	pop	    %ebp
	ret
.size out8, . - out8


in32:
    push    %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %edx
    inl     %dx
    pop     %ebp
    ret
.size in32, . - in32


in16:
    push    %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %edx
    xorl    %eax, %eax
    inw     %dx
    pop     %ebp
    ret
.size in16, . - in16

in8:
    push    %ebp
    movl    %esp, %ebp
    movl    8(%ebp), %edx
    xorl    %eax, %eax
    inb     %dx
    pop     %ebp
    ret
.size in8, . - in8
