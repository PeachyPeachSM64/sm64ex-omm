#define OMM_ALL_HEADERS
#include "data/omm/omm_includes.h"
#undef OMM_ALL_HEADERS

void fb_init(OmmFileBuffer *fb, u8 *buffer, s32 length) {
    fb->data = buffer;
    fb->head = buffer;
    fb->tail = buffer + length;
}

void fb_free(OmmFileBuffer *fb) {
    if (fb->data) {
        mem_del(fb->data);
        fb->data = NULL;
        fb->head = NULL;
        fb->tail = NULL;
    }
}

bool fb_eof(OmmFileBuffer *fb) {
    return fb->head >= fb->tail;
}

s32 fb_tell(OmmFileBuffer *fb) {
    return (s32) (fb->head - fb->data);
}

void fb_seek(OmmFileBuffer *fb, s32 pos) {
    fb->head = fb->data + pos;
}

const void *fb_read_bytes(OmmFileBuffer *fb, s32 size) {
    static const u64 zero = 0;
    if (fb->head + size <= fb->tail) {
        const void *data = fb->head;
        fb->head += size;
        return data;
    }
    fb->head = fb->tail;
    return (const void *) &zero;
}

const void *fb_read_buffer(OmmFileBuffer *fb, s32 size, s32 *length) {
    *length = fb_read(fb, s32);
    if (fb->head + (size * *length) <= fb->tail) {
        if (*length > 0) {
            void *buffer = mem_dup(fb->head, size * *length);
            fb->head += size * *length;
            return buffer;
        }
        return NULL;
    }
    fb->head = fb->tail;
    return NULL;
}

const char *fb_read_str(OmmFileBuffer *fb, str_t dst) {
    s32 length = (s32) fb_read(fb, u8);
    if (fb->head + length <= fb->tail) {
        mem_zero(dst, sizeof(str_t));
        mem_cpy(dst, fb->head, length);
        fb->head += length;
        return dst;
    }
    fb->head = fb->tail;
    return NULL;
}
