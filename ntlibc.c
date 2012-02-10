#include "ntlibc.h"

size_t ntlibc_strlen(const char *s)
{
    const char *p = s;
    int cnt = 0;
    while (*p) {
        cnt++;
        p++;
    }
    return cnt;
}

char *ntlibc_strcpy(char *des, const char *src)
{
    char *d = des;
    const char *s = src;
    while (*s) {
        *d = *s;
        d++;
        s++;
    }
    *d = '\0';
    return des;
}

char *ntlibc_strcat(char *des, const char *src)
{
    char *d = des;
    const char *s = src;
    while (*d) {
        d++;
    }
    while (*s) {
        *d = *s;
        d++;
        s++;
    }
    *d = '\0';
    return des;
}

int ntlibc_strcmp(const char *s1, const char *s2)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    while (*p1 || *p2) {
        if (*p1 != *p2) {
            return (*p1 < *p2) ? -1 : 1;
        }
        p1++;
        p2++;
    }
    if (*p1 == *p2) {
        return 0;
    } else {
        return (*p1 < *p2) ? -1 : 1;
    }
}

int ntlibc_strncmp(const char *s1, const char *s2, size_t n)
{
    char *p1 = (char *)s1;
    char *p2 = (char *)s2;
    size_t len = 0;
    while (*p1 || *p2) {
        if (n <= len) {
            break;
        }
        if (*p1 != *p2) {
            return (*p1 < *p2) ? -1 : 1;
        }
        p1++;
        p2++;
        len++;
    }
    return 0;
}

#if FUNCTION_TEST
#include <stdio.h>
int main(void);
int main(void) {
    char *str_a = "That";
    char *str_b = "The";
    printf("strcmp(%s,%s) = %d, %d\n",
            str_a, str_b,
            strcmp(str_a, str_b),
            ntlibc_strcmp(str_a, str_b));
    printf("strncmp(%s,%s,2) = %d, %d\n",
            str_a, str_b,
            strncmp(str_a, str_b, 2),
            ntlibc_strncmp(str_a, str_b, 2));
    printf("strncmp(%s,%s,3) = %d, %d\n",
            str_a, str_b,
            strncmp(str_a, str_b, 3),
            ntlibc_strncmp(str_a, str_b, 3));
    return 0;
}
#endif

