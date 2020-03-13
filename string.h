#ifndef OOC_STRING_H
#define OOC_STRING_H

#include <stddef.h>

struct String {
	const void *class;
	char *text;
};

struct Namespace_string {
	const void *Class;
	char *( *Cstr)(const void *_self);
	ptrdiff_t (*Find)(const void *_self, const char *substr);
	void *(* Substring)(const void *_self, size_t start, size_t length);
};

extern struct Namespace_string String;

#endif /* OOC_STRING_H */