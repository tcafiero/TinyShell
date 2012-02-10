#include "ntshell.h"
#include "ntlibc.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 0
#define VERSION_RELEASE 3

typedef struct {
    text_editor_t *editor;
    text_history_t *history;
    int suggest_index;
    char suggest_source[TEXTEDITOR_MAXLEN];
    int (*func_read)(char *buf, int cnt);
    int (*func_write)(const char *buf, int cnt);
    int (*func_cb)(const char *text);
} ntshell_user_data_t;

#define SUGGEST_INDEX(vtp) \
    ((ntshell_user_data_t *)(vtp)->user_data)->suggest_index
#define SUGGEST_SOURCE(vtp) \
    ((ntshell_user_data_t *)(vtp)->user_data)->suggest_source

#define GET_EDITOR(vtp) \
    ((ntshell_user_data_t *)(vtp)->user_data)->editor

#define GET_HISTORY(vtp) \
    ((ntshell_user_data_t *)(vtp)->user_data)->history

#define SERIAL_READ(vtp,buf,cnt) \
    ((ntshell_user_data_t *)(vtp)->user_data)->func_read(buf, cnt)

#define SERIAL_WRITE(vtp,buf,cnt) \
    ((ntshell_user_data_t *)(vtp)->user_data)->func_write(buf, cnt)

#define CALLBACK(vtp, text) \
    ((ntshell_user_data_t *)(vtp)->user_data)->func_cb(text)

static void actfunc_history_prev(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_history_read_point_prev(GET_HISTORY(parser))) {
        char txt[TEXTHISTORY_MAXLEN];
        int n = text_history_read(GET_HISTORY(parser), &txt[0], sizeof(txt));
        if (0 < n) {
            SERIAL_WRITE(parser, "\x1b[2K", 4);
            SERIAL_WRITE(parser, "\x1b[80D", 5);
            SERIAL_WRITE(parser, ">", 1);
            SERIAL_WRITE(parser, txt, n);
            text_editor_set_text(GET_EDITOR(parser), txt);
        }
    }
}

static void actfunc_history_next(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_history_read_point_next(GET_HISTORY(parser))) {
        char txt[TEXTHISTORY_MAXLEN];
        int n = text_history_read(GET_HISTORY(parser), &txt[0], sizeof(txt));
        if (0 < n) {
            SERIAL_WRITE(parser, "\x1b[2K", 4);
            SERIAL_WRITE(parser, "\x1b[80D", 5);
            SERIAL_WRITE(parser, ">", 1);
            SERIAL_WRITE(parser, txt, n);
            text_editor_set_text(GET_EDITOR(parser), txt);
        }
    }
}

static void actfunc_cursor_left(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_cursor_left(GET_EDITOR(parser))) {
        SERIAL_WRITE(parser, "\x1b[1D", 4);
    }
}

static void actfunc_cursor_right(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_cursor_right(GET_EDITOR(parser))) {
        SERIAL_WRITE(parser, "\x1b[1C", 4);
    }
}

static void actfunc_enter(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    char txt[TEXTEDITOR_MAXLEN];
    text_editor_get_text(GET_EDITOR(parser), &txt[0], sizeof(txt));
    text_editor_clear(GET_EDITOR(parser));
    text_history_write(GET_HISTORY(parser), txt);
    SERIAL_WRITE(parser, "\r\n", 2);
    CALLBACK(parser, txt);
    SERIAL_WRITE(parser, ">", 1);
}

static void actfunc_cancel(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    SERIAL_WRITE(parser, "^C\r\n", 4);
    text_editor_clear(GET_EDITOR(parser));
    SERIAL_WRITE(parser, ">", 1);
}

static void actfunc_insert(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {

    SUGGEST_INDEX(parser) = -1;

    if (text_editor_insert(GET_EDITOR(parser), ch)) {
        char txt[TEXTEDITOR_MAXLEN];
        int len = text_editor_get_text(GET_EDITOR(parser), &txt[0], sizeof(txt));
        int pos = text_editor_cursor_get_position(GET_EDITOR(parser));
        SERIAL_WRITE(parser, (char *)&ch, sizeof(ch));
        int n = len - pos;
        if (n > 0) {
            int i;
            SERIAL_WRITE(parser, txt + pos, len - pos);
            for (i = 0; i < n; i++) {
                SERIAL_WRITE(parser, "\x1b[1D", 4);
            }
        }
    }
}

static void actfunc_backspace(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    if (text_editor_backspace(GET_EDITOR(parser))) {
        char txt[TEXTEDITOR_MAXLEN];
        SERIAL_WRITE(parser, "\x1b[1D", 4);
        int len = text_editor_get_text(GET_EDITOR(parser), &txt[0], sizeof(txt));
        int pos = text_editor_cursor_get_position(GET_EDITOR(parser));
        int n = len - pos;
        if (n > 0) {
            int i;
            SERIAL_WRITE(parser, txt + pos, len - pos);
            SERIAL_WRITE(parser, " ", 1);
            for (i = 0; i < n + 1; i++) {
                SERIAL_WRITE(parser, "\x1b[1D", 4);
            }
        } else {
            SERIAL_WRITE(parser, " ", 1);
            SERIAL_WRITE(parser, "\x1b[1D", 4);
        }
    }
}

static void actfunc_suggest(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    char buf[TEXTEDITOR_MAXLEN];
    if (SUGGEST_INDEX(parser) < 0) {
        if (text_editor_get_text(
                    GET_EDITOR(parser),
                    SUGGEST_SOURCE(parser),
                    sizeof(SUGGEST_SOURCE(parser))) > 0) {
            SUGGEST_INDEX(parser) = 0;
            if (text_history_find(
                        GET_HISTORY(parser),
                        SUGGEST_INDEX(parser),
                        SUGGEST_SOURCE(parser),
                        buf,
                        sizeof(buf)) == 0) {
                int n = ntlibc_strlen((const char *)buf);
                SERIAL_WRITE(parser, "\x1b[2K", 4);
                SERIAL_WRITE(parser, "\x1b[80D", 5);
                SERIAL_WRITE(parser, ">", 1);
                SERIAL_WRITE(parser, buf, n);
                text_editor_set_text(GET_EDITOR(parser), buf);
            } else {
                SUGGEST_INDEX(parser) = -1;
            }
        }
    } else {
        SUGGEST_INDEX(parser) = SUGGEST_INDEX(parser) + 1;
        if (text_history_find(
                    GET_HISTORY(parser),
                    SUGGEST_INDEX(parser),
                    SUGGEST_SOURCE(parser),
                    buf,
                    sizeof(buf)) == 0) {
            int n = ntlibc_strlen((const char *)buf);
            SERIAL_WRITE(parser, "\x1b[2K", 4);
            SERIAL_WRITE(parser, "\x1b[80D", 5);
            SERIAL_WRITE(parser, ">", 1);
            SERIAL_WRITE(parser, buf, n);
            text_editor_set_text(GET_EDITOR(parser), buf);
        } else {
            int n = ntlibc_strlen(SUGGEST_SOURCE(parser));
            SERIAL_WRITE(parser, "\x1b[2K", 4);
            SERIAL_WRITE(parser, "\x1b[80D", 5);
            SERIAL_WRITE(parser, ">", 1);
            SERIAL_WRITE(parser, SUGGEST_SOURCE(parser), n);
            text_editor_set_text(GET_EDITOR(parser), SUGGEST_SOURCE(parser));
            SUGGEST_INDEX(parser) = -1;
        }
    }
}

static void actfunc_cursor_head(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    SERIAL_WRITE(parser, "\x1b[80D", 5);
    SERIAL_WRITE(parser, ">", 1);
    text_editor_cursor_head(GET_EDITOR(parser));
}

static void actfunc_cursor_tail(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    char buf[TEXTEDITOR_MAXLEN];
    int len;
    text_editor_get_text(GET_EDITOR(parser), buf, sizeof(buf));
    len = ntlibc_strlen((const char *)buf);
    SERIAL_WRITE(parser, "\x1b[80D", 5);
    SERIAL_WRITE(parser, ">", 1);
    SERIAL_WRITE(parser, buf, len);
    text_editor_cursor_tail(GET_EDITOR(parser));
}

typedef struct {
    vtparse_action_t action;
    unsigned char ch;
    void (*func)(
            vtparse_t *parser,
            vtparse_action_t action,
            unsigned char ch);
} ntshell_action_table_t;

static const ntshell_action_table_t action_table[] = {
    {VTPARSE_ACTION_EXECUTE, 0x01, actfunc_cursor_head},
    {VTPARSE_ACTION_EXECUTE, 0x02, actfunc_cursor_left},
    {VTPARSE_ACTION_EXECUTE, 0x03, actfunc_cancel},
    {VTPARSE_ACTION_EXECUTE, 0x05, actfunc_cursor_tail},
    {VTPARSE_ACTION_EXECUTE, 0x06, actfunc_cursor_right},
    {VTPARSE_ACTION_EXECUTE, 0x08, actfunc_backspace},
    {VTPARSE_ACTION_EXECUTE, 0x09, actfunc_suggest},
    {VTPARSE_ACTION_EXECUTE, 0x0d, actfunc_enter},
    {VTPARSE_ACTION_EXECUTE, 0x0e, actfunc_history_next},
    {VTPARSE_ACTION_EXECUTE, 0x10, actfunc_history_prev},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x41, actfunc_history_prev},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x42, actfunc_history_next},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x43, actfunc_cursor_right},
    {VTPARSE_ACTION_CSI_DISPATCH, 0x44, actfunc_cursor_left},
    {VTPARSE_ACTION_PRINT, 0x7f, actfunc_backspace},
};

void parser_callback(
        vtparse_t *parser,
        vtparse_action_t action,
        unsigned char ch) {
    ntshell_action_table_t *p;
    int i;
    const int ACTTBLSIZ = sizeof(action_table) / sizeof(action_table[0]);

    p = (ntshell_action_table_t *)action_table;
    for (i = 0; i < ACTTBLSIZ; i++) {
        if ((p->action == action) && (p->ch == ch)) {
            p->func(parser, action, ch);
            return;
        }
        p++;
    }

    if (VTPARSE_ACTION_PRINT == action) {
        actfunc_insert(parser, action, ch);
    }
}

void ntshell_version(int *major, int *minor, int *release)
{
    *major = VERSION_MAJOR;
    *minor = VERSION_MINOR;
    *release = VERSION_RELEASE;
}

void ntshell_execute(
        ntshell_t *p,
        int (*func_read)(char *buf, int cnt),
        int (*func_write)(const char *buf, int cnt),
        int (*func_cb)(const char *text))
{
    ntshell_user_data_t user_data;

    user_data.editor = &(p->editor);
    user_data.history = &(p->history);
    user_data.func_read = func_read;
    user_data.func_write = func_write;
    user_data.func_cb = func_cb;

    p->parser.user_data = &user_data;

    vtparse_init(&(p->parser), parser_callback);
    text_editor_init(GET_EDITOR(&(p->parser)));
    text_history_init(GET_HISTORY(&(p->parser)));
    SUGGEST_INDEX(&(p->parser)) = -1;

    SERIAL_WRITE(&(p->parser), ">", 1);
    while(1)
    {
        unsigned char ch;
        SERIAL_READ(&(p->parser), (char *)&ch, sizeof(ch));
        vtparse(&(p->parser), &ch, sizeof(ch));
    }
}

