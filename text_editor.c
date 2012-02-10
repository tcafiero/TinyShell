#include "text_editor.h"
void text_editor_init(text_editor_t *p)
{
    p->pos = 0;
    p->len = 0;
    p->buffer[p->len] = '\0';
}

int text_editor_insert(text_editor_t *p, char c)
{
    if (p->len < sizeof(p->buffer) - 1) {
        int n = p->len - p->pos + 1;
        int i;
        char *src = p->buffer + p->len + 0;
        char *des = p->buffer + p->len + 1;
        for (i = 0; i < n; i++) {
            *des = *src;
            des--;
            src--;
        }

        p->buffer[p->pos] = c;
        p->pos++;
        p->len++;
        p->buffer[p->len] = '\0';
        return 1;
    }
    return 0;
}

int text_editor_backspace(text_editor_t *p)
{
    if (0 < p->pos) {
        int n = p->len - p->pos;
        int i;
        char *src = p->buffer + p->pos - 0;
        char *des = p->buffer + p->pos - 1;
        p->pos--;
        p->len--;
        for (i = 0; i < n; i++) {
            *des = *src;
            des++;
            src++;
        }
        *(p->buffer + p->len) = '\0';
        return 1;
    }
    return 0;
}

int text_editor_cursor_get_position(text_editor_t *p)
{
    return p->pos;
}

int text_editor_cursor_head(text_editor_t *p)
{
    if (0 < p->pos) {
        p->pos = 0;
        return 1;
    }
    return 0;
}

int text_editor_cursor_tail(text_editor_t *p)
{
    if (p->pos < p->len) {
        p->pos = p->len;
        return 1;
    }
    return 0;
}

int text_editor_cursor_left(text_editor_t *p)
{
    if (0 < p->pos) {
        p->pos--;
        return 1;
    }
    return 0;
}

int text_editor_cursor_right(text_editor_t *p)
{
    if (p->pos < p->len) {
        p->pos++;
        return 1;
    }
    return 0;
}

int text_editor_set_text(text_editor_t *p, char *buf)
{
    char *src = buf;
    char *des = p->buffer;
    int n = 0;
    while (*src) {
        *des = *src;
        des++;
        src++;
        n++;
        if (sizeof(p->buffer) <= n - 1) {
            break;
        }
    }
    *des = '\0';
    p->len = n;
    p->pos = p->len;
    return n;
}

int text_editor_get_text(text_editor_t *p, char *buf, int siz)
{
    char *src = p->buffer;
    char *des = buf;
    int n = 0;
    while (*src) {
        *des++ = *src++;
        n++;
        if (siz <= n) {
            break;
        }
    }
    *des = '\0';
    return n;
}

void text_editor_clear(text_editor_t *p)
{
    p->pos = 0;
    p->len = 0;
    p->buffer[p->len] = '\0';
}

