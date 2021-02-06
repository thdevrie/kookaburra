#include <cpuio.h>
#include <terminal.h>
#include <keyboard.h>

#define PIC1_COMMAND (0x20)
#define PIC1_DATA (PIC1_COMMAND+1)
#define PIC2_COMMAND (0xA0)
#define PIC2_DATA (PIC2_COMMAND+1)
#define PIC_EOI (0x20)
#define QUEUE_MAX_SIZE (0x100)

struct {
	uint16_t offset_lowerbits;
	uint16_t selector;
	uint8_t zero;
	uint8_t type_attr;
	uint16_t offset_higherbits;
} IDT[256];



static size_t follow_up_task_data;
static void (*follow_up_task)(size_t);
size_t follow_up_task_registered = 0;

uint16_t disabledIRQ = 0xFFFF;
void (*handlers[16])();

#define LOADIRQ(nirq) do { 														\
		extern int irq##nirq ();												\
		unsigned long ptr = (uint32_t) irq##nirq;						  		\
		uint8_t voff = 32+nirq;													\
		IDT[voff].offset_lowerbits = ptr & 0xFFFF;								\
		IDT[voff].selector = 0x8; 												\
		IDT[voff].zero = 0;														\
		IDT[voff].type_attr = 0x8e;  											\
		IDT[voff].offset_higherbits = (ptr&0xFFFF0000)>> 16 ;					\
	} while (0)

void register_irq_handler(uint8_t irq_line, void (*handler)()) 
{
	disabledIRQ &= ~(1 << irq_line);
	if (irq_line < 8)
		out8(PIC1_DATA, disabledIRQ & 0xFF);
	else 
		out8(PIC2_DATA, (disabledIRQ & 0xFF00)>>8);
	handlers[irq_line] = handler;
}

void bootstrap_irq() 
{
	LOADIRQ(0);
	LOADIRQ(1);
	LOADIRQ(2);
	LOADIRQ(3);
	LOADIRQ(4);
	LOADIRQ(5);
	LOADIRQ(6);
	LOADIRQ(7);
	LOADIRQ(8);
	LOADIRQ(9);
	LOADIRQ(10);
	LOADIRQ(11);
	LOADIRQ(12);
	LOADIRQ(13);
	LOADIRQ(14);
	LOADIRQ(15);
	out8(0x20, 0x11);
	out8(0xA0, 0x11);
	out8(0x21, 0x20);
	out8(0xA1, 40);
	out8(0x21, 0x04);
	out8(0xA1, 0x02);
	out8(0x21, 0x01);
	out8(0xA1, 0x01);
	out8(0x21, (disabledIRQ & 0xFF));
	out8(0xA1, (disabledIRQ & 0xFF00) >> 8);
	kprintf("Bootstrapped IRQ\n");
}


inline void irq_eoi(uint8_t irq) {

	if(irq >= 8)
		out8(PIC2_COMMAND,PIC_EOI);
	out8(PIC1_COMMAND,PIC_EOI);
}

void irq_entry(uint16_t v) 
{
	handlers[v]();
	irq_eoi(v);
}

void register_followup_task(size_t data, void (*hdlr)(size_t)) 
{
	follow_up_task = hdlr;
	follow_up_task_data = data;
	follow_up_task_registered = 1;
}

void run_follow_up_task() 
{
	if (!follow_up_task) 
		return;
	follow_up_task(follow_up_task_data);
	follow_up_task = NULL;
	follow_up_task_data = NULL;
}