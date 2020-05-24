#ifndef OOC_BOOLEAN_H
#define OOC_BOOLEAN_H

#include <stdbool.h>

struct Boolean {
	const void *class;
	bool value;
};

struct NamespaceBoolean {
	const void *Class;
};

extern struct NamespaceBoolean Boolean;

#endif /* OOC_BOOLEAN_H */
