#ifndef _NTOPT_H_
#define _NTOPT_H_

#define NTOPT_MAXCNT_ARGC  32
#define NTOPT_MAXLEN_ARGV  32

int ntopt_parse(const char *str, void (*func)(int argc, char **argv));

#endif

