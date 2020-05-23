#ifndef OOC_UTIL_H
#define OOC_UTIL_H

#include <stddef.h>

int strcatf(char **buffer, const char *format, ...);
size_t fnv1a(void *buf, size_t size);

#endif /* OOC_UTIL_H */
