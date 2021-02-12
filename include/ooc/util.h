#ifndef OOC_UTIL_H
#define OOC_UTIL_H

#include <stdarg.h>
#include <stddef.h>
#include <string.h>

int strcatf(char **buffer, const char *format, ...);
int strcatf_va(char **buffer, const char *format, va_list *ap, va_list *copy);
int streplace(char **s, const char *old, const char *new);
#ifndef strndup
char *strndup(const char *str, size_t n);
#endif
char **strsplit(char *s, const char *fmt);
void strsplit_free(char **buf);
size_t fnv1a(const void *buf, size_t size);

int arg_check(int argc, char **argv, const char *arg);
char *arg_get(int argc, char **argv, const char *arg);

void swap(void **a, void **b);

#endif /* OOC_UTIL_H */
