#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include <stdint.h>
#include <stdbool.h>

#define KEYS(COMAND, VALUE, ALPHA)             \
    VALUE(TILD,               0x29, '`', '~')  \
    VALUE(ONE,                0x02, '1', '!')  \
    VALUE(TWO,                0x03, '2', '@')  \
    VALUE(THREE,              0x04, '3', '#')  \
    VALUE(FOUR,               0x05, '4', '$')  \
    VALUE(FIVE,               0x06, '5', '%')  \
    VALUE(SIX,                0x07, '6', '^')  \
    VALUE(SEVEN,              0x08, '7', '*')  \
    VALUE(EIGHT,              0x09, '8', '*')  \
    VALUE(NINE,               0x0A, '9', '(')  \
    VALUE(ZERO,               0x0B, '0', ')')  \
    VALUE(MINUS,              0x0c, '-', '_')  \
    VALUE(EQUALS,             0x0D, '=', '+')  \
    COMAND(BACKSPACE,         0x0E)            \
    COMAND(TAB,               0x0F)            \
    ALPHA(Q,                  0x10, 'q', 'Q')  \
    ALPHA(W,                  0x11, 'w', 'W')  \
    ALPHA(E,                  0x12, 'e', 'E')  \
    ALPHA(R,                  0x13, 'r', 'R')  \
    ALPHA(T,                  0x14, 't', 'T')  \
    ALPHA(Y,                  0x15, 'y', 'Y')  \
    ALPHA(U,                  0x16, 'u', 'U')  \
    ALPHA(I,                  0x17, 'i', 'I')  \
    ALPHA(O,                  0x18, 'o', 'O')  \
    ALPHA(P,                  0x19, 'p', 'P')  \
    VALUE(RBRACE,             0x1A, '[', '{')  \
    VALUE(LBRACE,             0x1B, ']', '}')  \
    VALUE(BACKSLASH,          0x2B, '\\', '|') \
    COMAND(CAPSLOCK,          0x3A)            \
    ALPHA(A,                  0x1E, 'a', 'A')  \
    ALPHA(S,                  0x1F, 's', 'S')  \
    ALPHA(D,                  0x20, 'd', 'D')  \
    ALPHA(F,                  0x21, 'f', 'F')  \
    ALPHA(G,                  0x22, 'g', 'G')  \
    ALPHA(H,                  0x23, 'h', 'H')  \
    ALPHA(J,                  0x24, 'j', 'J')  \
    ALPHA(K,                  0x25, 'k', 'K')  \
    ALPHA(L,                  0x26, 'l', 'L')  \
    VALUE(SIMICOLON,          0x27, ';', ':')  \
    VALUE(QUOTE,              0x28, '\'', '"') \
    COMAND(ENTER,             0x1c)            \
    COMAND(LSHIFT,            0x2A)            \
    ALPHA(Z,                  0x2c, 'z', 'Z')  \
    ALPHA(X,                  0x2d, 'x', 'X')  \
    ALPHA(C,                  0x2e, 'c', 'C')  \
    ALPHA(V,                  0x2f, 'v', 'V')  \
    ALPHA(B,                  0x30, 'b', 'B')  \
    ALPHA(N,                  0x31, 'n', 'N')  \
    ALPHA(M,                  0x32, 'm', 'M')  \
    VALUE(COMA,               0x33, ',', '<')  \
    VALUE(PERIOD,             0x34, '.', '>')  \
    VALUE(SLASH,              0x35, '/', '?')  \
    COMAND(RSHIFT,            0x36)            \
    COMAND(ALT,               0x38)            \
    COMAND(CTRL,              0x1d)            \
    COMAND(SPACE,             0x39)            



enum key {

#define ENUM_COMMAND(NAME,VAL) \
    KEY_##NAME = VAL,    
#define ENUM_ALPHA(NAME,VAL,_,_2) \
    KEY_##NAME = VAL,

    KEYS(ENUM_COMMAND, ENUM_ALPHA, ENUM_ALPHA)
#undef ENUM_ALPHA
#undef ENUM_COMMAND

};

void init_keyboard();
char * get_key_name(uint8_t scan_code);
extern void (*keyboard_charhandler)(char);
extern void (*keyboard_scanhandler)(uint8_t);

#endif