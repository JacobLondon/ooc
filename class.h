#ifndef OOC_CLASS_H
#define OOC_CLASS_H

#include <stdarg.h>
#include <stdlib.h>

struct Class {
	size_t size;
	void *(* ctor)(void *self, va_list *ap);
	void *(* dtor)(void *self);
	void *(* clone)(const void *self);
	char *(* to_string)(const void *self);
};

void *new(const void *_class, ...);
void delete(void *_self);
void *clone(const void *_self);
size_t size_of(const void *_self);
char *to_string(const void *_self);

#endif /* OOC_CLASS_H */