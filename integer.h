#ifndef OOC_INTEGER_H
#define OOC_INTEGER_H

#include "types.h"

struct Integer {
	const void *class;
	ssize_t value;
};

struct NamespaceInteger {
	const void *Class;
};

extern struct NamespaceInteger Integer;

#endif /* OOC_INTEGER_H */
