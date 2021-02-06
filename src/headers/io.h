#ifndef _IO_H
#define _IO_H
#include <stddef.h>
#include <stdarg.h>
struct output_stream {
    void *private_data;
    void (*write)(void*,const char *, size_t);
};

void printf(struct output_stream*, const char*, ...);
void vprintf(struct output_stream*, const char*, va_list);

inline static void write(struct output_stream *stream, 
                    const char* char_data, 
                    size_t length) {
    stream->write(stream->private_data, char_data, length);
}
#endif