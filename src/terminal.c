#include <stddef.h>
#include <terminal.h>
#include <io.h>
#include <interrupts.h>
#include <cpuio.h>
#include <stdarg.h>
#include <keyboard.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEM ((uint16_t*) 0xB8000)
#define VGA_LIGHT_GREY (7)
#define VGA_COLOR_BLACK (0)
#define VGA_DISPLAY_SETTING_MASK \
    ((uint16_t)((VGA_LIGHT_GREY | VGA_COLOR_BLACK << 4) << 8))
#define CMD_BUFFER_MAX_SIZE 0x1000
#define CMD_ARG_SIZE 0x100


static size_t terminal_row = 0; 
static size_t terminal_column = 0;
static size_t cmd_buffer_size = 0;
static char cmd_buffer[CMD_BUFFER_MAX_SIZE];
static char *cmd_arguments[CMD_ARG_SIZE];
comand_line_hander_t comand_line_hander;


static uint16_t *display_index(size_t row, size_t col) 
{
    return &VGA_MEM[(row * VGA_WIDTH) + col];
}

static void setc(size_t row, size_t col, char c) 
{
    *display_index(row, col) = c | VGA_DISPLAY_SETTING_MASK;
}

static void terminal_push_up() 
{
    for (size_t i = 0; i < VGA_HEIGHT - 1; i++) 
        for (size_t j = 0; j < VGA_WIDTH; j++) 
            *display_index(i, j) = *display_index(i+1, j);
    for (size_t i = 0; i < VGA_WIDTH; i++) 
        *display_index(VGA_HEIGHT-1, i) = ' ' | VGA_DISPLAY_SETTING_MASK;
}



static void terminal_newline(void) 
{
    if (terminal_row == VGA_HEIGHT-1)
        terminal_push_up();
    else
        terminal_row++;
    terminal_column = 0;
}

static void terminal_putc(char c) 
{
    if (c == '\b') {
        if (terminal_column==0) {
            if (terminal_row > 0) {
                terminal_column = VGA_WIDTH - 1;
                terminal_row--;
            }
        } else {
            terminal_column--;
        }
        
        setc(terminal_row, terminal_column, ' ');
        return;
    }

    if (c == '\n') {
        terminal_newline();
        return;
    }

    setc(terminal_row, terminal_column, c);
    
    if (terminal_column == VGA_WIDTH) {
        terminal_newline();
        terminal_column = 0;
    } else {
        terminal_column++;
    }
}

static void terminal_update_cursor() {
    uint16_t position = terminal_row*VGA_WIDTH + terminal_column;
    out8(0x3D4, 0x0F);
    out8(0x3D5, (uint8_t)(position & 0xFF));
	out8(0x3D4, 0x0E);
    out8(0x3D5, (uint8_t)((position>>8) & 0xFF));
}

static void terminal_write(void * ignore __attribute__((unused)), const char *data, size_t size) 
{
    for (size_t i =0; i < size; i++) 
        terminal_putc(data[i]);
    terminal_update_cursor();

}

static struct output_stream vga_ostream = {
    .private_data = NULL,
    .write = &terminal_write
};

static void keypress(char charcode);

void execute_task(size_t ignore __attribute__((unused))) 
{
    size_t argc = 0;
    for (size_t i = 0; i < cmd_buffer_size; i++) {
        if (cmd_buffer[i] == ' ') {
            cmd_buffer[i] = '\0';
        } else {
            if (i == 0 || cmd_buffer[i-1] == '\0') {
                cmd_arguments[argc++] = cmd_buffer+i;
            }
        }
    }
    comand_line_hander(argc, cmd_arguments);
    memset(cmd_buffer, '\0', cmd_buffer_size);
    cmd_buffer_size = 0;
    terminal_putc('\n');
    terminal_putc('>');
    terminal_update_cursor();
    keyboard_charhandler = keypress;
}

static void keypress(char charcode) 
{

    if (charcode == '\n') {
        keyboard_charhandler = NULL;
        register_followup_task(0, execute_task);
        terminal_putc('\n');
        return;
    }

    if (charcode == '\b') {
        if (cmd_buffer_size > 0) { 
            cmd_buffer_size--;
            terminal_putc('\b');
            terminal_update_cursor();
        }
        return;
    } 

    if (cmd_buffer_size >= CMD_BUFFER_MAX_SIZE)
        return;

    terminal_putc(charcode);
    terminal_update_cursor();

    cmd_buffer[cmd_buffer_size++] = charcode;
}

void terminal_cls(void) {
    terminal_row = 0;
    terminal_column = 0;
    for (size_t i = 0; i < VGA_HEIGHT; i++) 
        for (size_t j = 0; j < VGA_WIDTH; j++) 
            *display_index(i, j) = ' ' | VGA_DISPLAY_SETTING_MASK;
    terminal_push_up();
}

void kprintf(const char* format, ...) {
    va_list argp;
    va_start(argp, format);
    vprintf(&vga_ostream, format, argp);
    va_end(argp);
}

void terminal_run_repl(comand_line_hander_t hdlr) 
{
    terminal_putc('>');
    terminal_update_cursor();
    keyboard_charhandler = keypress;
    comand_line_hander = hdlr;
}

