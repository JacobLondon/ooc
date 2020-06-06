#ifndef OOC_STRING_H
#define OOC_STRING_H

#include "types.h"

struct String {
	const void *class;
	char *text;
	size_t size;
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
	void      (* Fwrite)    (var _self, const char *path);
	void      (* Fappend)   (var _self, const char *path);
	void      (* Fclear)    (const char *path);
};

extern struct NamespaceString String;

#define $String(value) New(String.Class, value)

#endif /* OOC_STRING_H */
