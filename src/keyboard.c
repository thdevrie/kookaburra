#include <stddef.h>
#include <keyboard.h>
#include <stdbool.h>
#include <interrupts.h>
#include <cpuio.h>

#define KEY_UP 0x80

char * get_key_name(uint8_t scan_code) 
{
    bool is_key_up = (scan_code & KEY_UP) != 0;
    if (is_key_up)
        scan_code &= 0x7F;
    

    switch (scan_code)
    {
#define NAME_COMMAND(NAME, VALUE) \
    case KEY_##NAME: \
        if (is_key_up) \
            return "KEY_" #NAME "_UP"; \
        return "KEY_" #NAME;       
#define NAME_ALPHA(NAME, VALUE, _, _2) \
    NAME_COMMAND(NAME, VALUE) 

        KEYS(NAME_COMMAND, NAME_ALPHA, NAME_ALPHA)
#undef NAME_COMMAND
#undef NAME_ALPHA
    default:
        return NULL;
    }

}

// State for ascii translation layer
bool caps_lock_key_down = false;
bool caps_lock_enabled = false;
bool shift_key_down = false;

void (*keyboard_charhandler)(char);
void (*keyboard_scanhandler)(uint8_t);


char read_char(uint8_t scan_code) 
{
    bool is_key_up = (scan_code & 0x80) != 0;
    switch (scan_code)
    {
    case KEY_CAPSLOCK:
        if (!caps_lock_key_down) {
            caps_lock_key_down = true;
            caps_lock_enabled = !caps_lock_enabled;
        }
        return '\0';
    case (KEY_CAPSLOCK | KEY_UP):
        caps_lock_key_down = false;
        return '\0';
    case KEY_ENTER:
        return '\n';
    case KEY_BACKSPACE:
        return '\b';
    case KEY_SPACE:
        return ' ';
    case KEY_LSHIFT:
    case KEY_RSHIFT:
        shift_key_down = true;
        return '\0';
    case (KEY_LSHIFT | KEY_UP):
    case (KEY_RSHIFT | KEY_UP):
        shift_key_down = false;
        return '\0';
#define READ_COMMAND(NAME, VALUE) 
#define READ_VALUE(NAME, VALUE, LOWER, UPPER) \
    case KEY_##NAME: \
        if (shift_key_down) \
            return UPPER; \
        return LOWER; 
#define READ_ALPHA(NAME, VALUE, LOWER, UPPER) \
    case KEY_##NAME: \
        if (shift_key_down || caps_lock_enabled) \
            return UPPER;\
        return LOWER;
        KEYS(READ_COMMAND, READ_VALUE, READ_ALPHA);
#undef READ_COMMAND
#undef READ_VALUE
#undef READ_ALPHA
    default:
        return '\0';
        break;
    }
}

void on_key_press() {
    uint8_t key = in8(0x60);
    if (keyboard_charhandler) {
        char c = read_char(key);
        if (c)
            keyboard_charhandler(c);
    }
    if (keyboard_scanhandler) 
        keyboard_scanhandler(key);
}

void init_keyboard() 
{
    keyboard_charhandler = NULL;
    keyboard_scanhandler = NULL;
    register_irq_handler(1, on_key_press);
}