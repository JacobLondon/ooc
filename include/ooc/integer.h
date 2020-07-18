#ifndef OOC_INTEGER_H
#define OOC_INTEGER_H

#include "types.h"

struct Integer {
	const void *class;
	ssize_t value;
};

struct NamespaceInteger {
	const void *Class;
	ssize_t (* Add)(var _self, ssize_t other);
	ssize_t (* Sub)(var _self, ssize_t other);
	ssize_t (* Mul)(var _self, ssize_t other);
	ssize_t (* Div)(var _self, ssize_t other);
};

extern struct NamespaceInteger Integer;

#define $Int(value) New(Integer.Class, value)

#endif /* OOC_INTEGER_H */
