#ifndef OOC_STRING_H
#define OOC_STRING_H

#include <stddef.h>

struct String {
	const void *class;
	char *text;
};

struct NamespaceString {
	const void *Class;
	void      (* Clear)(var _self);
	char*     (* Cstr)(const var _self);
	ptrdiff_t (* Find)(const var _self, const char *substr);
	void*     (* Substring)(const var _self, size_t start, size_t length);
};

extern struct NamespaceString String;

#endif /* OOC_STRING_H */
