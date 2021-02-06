#include <io.h>
#include <stdarg.h>
#include <util.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MAX_INT_STR_SIZE (10)
#define MAX_UINT_STR_SIZE (11)
#define HEX (0xF)
#define HEX_SIZE (8)
static const char hex_values[0x10] = {'0', '1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void vprintf(struct output_stream *ostream, const char* format, va_list parameters) {
    while (*format) 
    {
        if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
            size_t amount = 1;
            while (format[amount] && format[amount] != '%') 
                amount++;
            write(ostream, format, amount);
            format += amount;
            continue;
        }

        format++;
        switch (*format) {
        case 'c': {
                char c = (char) va_arg(parameters, int);
                write(ostream, &c, sizeof(char));
            }
            break;
        case 's': {
                const char* string = va_arg(parameters, const char*);
                size_t len = strlen(string);
                write(ostream, string, len);
            }
            break;
        case 'i': {
                char str[MAX_INT_STR_SIZE];
                size_t offset = MAX_INT_STR_SIZE;
                uint32_t val = va_arg(parameters, uint32_t);
                do {
                    char c = '0'+(char)(val%10);
                    val /= 10;
                    str[--offset] = c;
                } while (val);
                write(ostream, str+offset, MAX_INT_STR_SIZE - offset);
            }
            break;
        case 'u': {
                char str[MAX_UINT_STR_SIZE];
                size_t offset = MAX_UINT_STR_SIZE;
                int32_t val = va_arg(parameters, int32_t);

                bool flip = val < 0;
                if (flip) 
                    val = -val;

                do {
                    char c = '0'+(char)(val%10);
                    val /= 10;
                    str[--offset] = c;
                } while (val);

                if (flip) 
                    str[--offset] = '-';
                
                write(ostream, str+offset, MAX_UINT_STR_SIZE - offset);
            }
            break;
        case 'x': {
                uint32_t val = va_arg(parameters, uint32_t);
                char str[HEX_SIZE+2];
                size_t offset = HEX_SIZE+2;
                while(val) {
                    uint32_t value = val & 0xF;
                    str[--offset] = hex_values[value];
                    val >>= 4;
                }
                str[--offset]= 'x';
                str[--offset]= '0';
                write(ostream, str+offset, (HEX_SIZE+2)-offset);
            }
        break;
        case 'p': {
                uint32_t val = va_arg(parameters, uint32_t);
                char str[HEX_SIZE+2];
                size_t offset = HEX_SIZE;
                while(--offset>=2) {
                    uint32_t value = val & 0xF;
                    str[offset] = hex_values[value];
                    val >>= 4;
                }
                str[0]= '0';
                str[1]= 'x';
                write(ostream, str, HEX_SIZE+2);
            }
        }
        format++;
    }
}

void printf(struct output_stream *ostream, const char* format, ...)
{
    va_list argp;
    va_start(argp, format);
    vprintf(ostream, format, argp);
    va_end(argp);
}