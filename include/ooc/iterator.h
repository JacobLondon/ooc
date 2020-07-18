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

/**
 * @param T
 *     The type of ValueName
 * @param ValueName
 *     The name a value in the iterable
 * @param It
 *     The iterable object (String, Dict, Vector, ...)
 * @param Body
 *     A single line of code or a body of code enclosed with {}
 */
#define $For(T, ValueName, It, Body) \
	do { \
		T ValueName; \
		var ValueName##__$In__##It = Iter(It); \
		while (ValueName = Next(ValueName##__$In__##It)) \
			Body \
		Del(ValueName##__$In__##It); \
	} while (0)

#endif /* OOC_ITERATOR_H */
