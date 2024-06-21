#include "buffer.h"
#include "string.h"

Buffer *buffer_init(size_t initial_capacity) {
    Buffer *buf = malloc(sizeof(Buffer));

    if (buf == NULL) {
        return NULL;
    }

    buf->data = malloc(initial_capacity);
    if (buf->data == NULL) {
        free(buf);
        return NULL;
    }

    buf->size = 0;
    buf->capacity = initial_capacity;

    return buf;
}

void buffer_append(Buffer *buf, const uint8_t *data, size_t len) {
    if (buf->size + len < buf->capacity) {
        size_t new_capacity = buf->capacity * 2;
        while (buf->size + len > new_capacity) {
            new_capacity *= 2;
        }

        uint8_t *new_data = realloc(buf->data, new_capacity);

        buf->data = new_data;
        memcpy(buf->data + buf->size, data, len);
        buf->size += len;
    }
}

void buffer_free(Buffer *buf) {
    if (buf != NULL) {
        free(buf->data);
        free(buf);
    }
}
