#ifndef _NTSHELL_H_
#define _NTSHELL_H_

#include "vtparse.h"
#include "text_editor.h"
#include "text_history.h"

typedef struct {
    vtparse_t parser;
    text_editor_t editor;
    text_history_t history;
} ntshell_t;

void ntshell_version(int *major, int *minor, int *release);
void ntshell_execute(
        ntshell_t *p,
        int (*func_read)(char *buf, int cnt),
        int (*func_write)(const char *buf, int cnt),
        int (*func_cb)(const char *text));

#endif

