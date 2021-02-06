#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H
#include <stdint.h>

void register_followup_task(size_t, void (*hdlr)(size_t));
void bootstrap_interrupts();
void register_irq_handler(uint8_t irq_line, void (*handler)());
#endif