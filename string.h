#ifndef OOC_STRING_H
#define OOC_STRING_H

#include <stddef.h>

struct String {
	const void *class;
	char *text;
};

struct NamespaceString {
	const void *Class;
	void      (* Clear)     (var _self);
	char*     (* Cstr)      (const var _self);
	ptrdiff_t (* Find)      (const var _self, const char *substr);
	void*     (* Substring) (const var _self, size_t start, size_t length);

	void      (* Catf)      (var _self, const char *fmt, ...);
	void      (* Replace)   (var _self, const char *old, const char *new);
	var       (* Fread)     (const char *path);
	void      (* Fwrite)    (const char *path);
	void      (* Fappend)   (const char *path);
	void      (* Fclear)    (const char *path);
};

extern struct NamespaceString String;

#endif /* OOC_STRING_H */
