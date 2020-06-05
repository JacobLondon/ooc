#ifndef OOC_ITERATOR_H
#define OOC_ITERATOR_H

#include "types.h"

struct Iterator {
	const void *class;
	void *state;
	size_t size;
	shared *object;
};

struct NamespaceIterator {
	const void *Class;
};

extern struct NamespaceIterator Iterator;

#endif /* OOC_ITERATOR_H */
