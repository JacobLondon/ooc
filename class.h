#ifndef OOP_CLASS_H
#define OOP_CLASS_H

#include <stdarg.h>
#include <stdlib.h>

struct Class {
	size_t size;
	void *(* ctor)(void *self, va_list *ap);
	void *(* dtor)(void *self);
	void *(* clone)(const void *self);
};

void *new(const void *_class, ...);
void delete(void *self);
size_t size_of(const void *self);

#endif /* OOP_CLASS_H */