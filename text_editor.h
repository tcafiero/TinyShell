#ifndef _TEXT_EDITOR_H_
#define _TEXT_EDITOR_H_

#define TEXTEDITOR_MAXLEN 64

typedef struct {
    char buffer[TEXTEDITOR_MAXLEN];
    int pos;
    int len;
} text_editor_t;

void text_editor_init(text_editor_t *p);
int text_editor_insert(text_editor_t *p, char c);
int text_editor_backspace(text_editor_t *p);
int text_editor_cursor_get_position(text_editor_t *p);
int text_editor_cursor_head(text_editor_t *p);
int text_editor_cursor_tail(text_editor_t *p);
int text_editor_cursor_left(text_editor_t *p);
int text_editor_cursor_right(text_editor_t *p);
int text_editor_set_text(text_editor_t *p, char *buf);
int text_editor_get_text(text_editor_t *p, char *buf, int siz);
void text_editor_clear(text_editor_t *p);

#endif

