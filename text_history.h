#ifndef TEXT_HISTORY_H
#define TEXT_HISTORY_H

#define TEXTHISTORY_MAXLEN 64   /**< テキストヒストリあたりの最大文字列長。 */
#define TEXTHISTORY_DEPTH 8     /**< テキストヒストリのヒストリ数。 */

typedef struct {
    char history[TEXTHISTORY_MAXLEN * TEXTHISTORY_DEPTH];
    int rp;
    int wp;
} text_history_t;

void text_history_init(text_history_t *p);
int text_history_write(text_history_t *p, char *buf);
int text_history_read(text_history_t *p, char *buf, const int siz);
int text_history_read_point_next(text_history_t *p);
int text_history_read_point_prev(text_history_t *p);
int text_history_find(text_history_t *p,
        const int index, const char *text,
        char *buf, const int siz);

#endif

