#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
    uint8_t *data;
    size_t size;
    size_t capacity;
} Buffer;

Buffer *buffer_init(size_t initial_capacity);
void buffer_append(Buffer *buf, const uint8_t *data, size_t len);
void buffer_free(Buffer *buf);

#endif
