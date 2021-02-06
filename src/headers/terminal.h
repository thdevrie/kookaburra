#ifndef _TERMINAL_H
#define _TERMINAL_H
#include <stdint.h>
typedef void (*comand_line_hander_t)(size_t argc, char** argv);
void terminal_run_repl(comand_line_hander_t);
void terminal_cls();
void kprintf(const char *, ...);
#endif