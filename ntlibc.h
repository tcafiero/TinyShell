#ifndef NTLIBC_H
#define NTLIBC_H

#ifndef size_t
typedef unsigned int size_t;
#endif

size_t ntlibc_strlen(const char *s);
char *ntlibc_strcpy(char *des, const char *src);
char *ntlibc_strcat(char *des, const char *src);
int ntlibc_strcmp(const char *s1, const char *s2);
int ntlibc_strncmp(const char *s1, const char *s2, size_t n);

#endif

