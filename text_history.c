#include "text_history.h"
#include "ntlibc.h"

void text_history_init(text_history_t *p)
{
    p->rp = 0;
    p->wp = 0;
    int i;
    for (i = 0; i < sizeof(p->history); i++) {
        p->history[i] = 0;
    }
}

int text_history_write(text_history_t *p, char *buf)
{
    if (buf[0] == '\0') {
        return 0;
    }
    char *sp = p->history + (TEXTHISTORY_MAXLEN * p->wp);
    while (*buf) {
        *sp = *buf;
        sp++;
        buf++;
    }
    *sp = '\0';
    p->wp = (p->wp + 1) % TEXTHISTORY_DEPTH;
    p->rp = p->wp;
    return 1;
}

int text_history_read(text_history_t *p, char *buf, const int siz)
{
    char *sp = p->history + (TEXTHISTORY_MAXLEN * p->rp);
    int n = 0;
    while (*sp) {
        *buf = *sp;
        buf++;
        sp++;
        n++;
        if (siz - 1 <= n) {
            break;
        }
    }
    *buf = '\0';
    return n;
}

int text_history_read_point_next(text_history_t *p)
{
    int n = (p->rp + 1) % TEXTHISTORY_DEPTH;
    if (n != p->wp) {
        p->rp = n;
        return 1;
    }
    return 0;
}

int text_history_read_point_prev(text_history_t *p)
{
    int n = (p->rp == 0) ? (TEXTHISTORY_DEPTH - 1) : (p->rp - 1);
    if (n != p->wp) {
        char *sp = p->history + (TEXTHISTORY_MAXLEN * n);
        if (*sp != '\0') {
            p->rp = n;
            return 1;
        }
    }
    return 0;
}

int text_history_find(text_history_t *p,
        const int index, const char *text,
        char *buf, const int siz)
{
    const int text_len = ntlibc_strlen((const char *)text);
    int found = 0;
    int i;
    for (i = 0; i < TEXTHISTORY_DEPTH; i++) {
        int target = (p->rp + i) % TEXTHISTORY_DEPTH;
        char *txtp = p->history + (TEXTHISTORY_MAXLEN * target);
        const int target_len = ntlibc_strlen((const char *)txtp);
        int comp_len = (target_len < text_len) ? target_len : text_len;
        if ((ntlibc_strncmp(
                    (const char *)txtp,
                    (const char *)text, comp_len) == 0) && (comp_len > 0)) {
            if (found == index) {
                ntlibc_strcpy((char *)buf, (char *)txtp);
                return 0;
            }
            found++;
        }
    }
    return -1;
}

