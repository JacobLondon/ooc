#ifndef OOC_CLASS_H
#define OOC_CLASS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

struct Class {
	size_t size;
	const struct Class *class;
	const struct Class *super;
	/* class methods */
	void *(* ctor)(void *_self, va_list *ap);
	void *(* dtor)(void *_self);
	void *(* clone)(const void *_self);
	char *(* str)(const void *_self);
	size_t (* hash)(const void *_self);
};

/* class methods */
void *new(const void *_class, ...);
void delete(void *_self);
void *clone(const void *_self);
char *str(const void *_self);
size_t hash(const void *_self);
/* introspection methods */
const void *super(const void *_self);
size_t size_of(const void *_self);
const void *class_of(const void *_self);
bool is_a(const void *_self, const void *_class);
bool is_of(const void *_self, const void *_class);

#endif /* OOC_CLASS_H */