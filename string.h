#ifndef OOC_STRING_H
#define OOC_STRING_H

#include <stddef.h>

struct String {
	const void *class;
	char *text;
};

struct NamespaceString {
	const void *class;
	void (* clear)(void *_self);
	char *(* cstr)(const void *_self);
	ptrdiff_t (* find)(const void *_self, const char *substr);
	void *(* substring)(const void *_self, size_t start, size_t length);
};

extern struct NamespaceString String;

#endif /* OOC_STRING_H */