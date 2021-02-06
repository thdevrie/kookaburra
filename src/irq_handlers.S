.align   4
.macro irq val:req
.global irq\val
irq\val:
    pusha
    cld
    pushw $1
    call irq_entry
    addl $0x02, %esp
    popa
    iret
.endm
irq 0
irq 1
irq 2
irq 3
irq 4
irq 5
irq 6
irq 7
irq 8
irq 9
irq 10
irq 11
irq 12
irq 13
irq 14
irq 15
