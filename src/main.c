#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <terminal.h>
#include <util.h>
#include <keyboard.h>

void echo (size_t argc, char **argv) 
{
	for (size_t i = 0; i < argc; i++) {
		kprintf("%s ", argv[i]);
	}
}

#define COMANDS(CMD) \
	CMD(echo) 

void execute_comand(size_t argc, char **argv) 
{
	const *arg1 = *argv;
	argv++;
	argc--;

#define CMD(cmd) \
	if (!strcmp(#cmd, arg1)) \
		return cmd(argc, argv);
	COMANDS(CMD)
#undef CMD

	kprintf("Unknown command %s", arg1);
}

void kernel_main() 
{
	init_keyboard();
	terminal_run_repl(execute_comand);
}