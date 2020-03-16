#ifndef OOC_VECTOR_H
#define OOC_VECTOR_H

#include <stddef.h>

struct Vector {
	const void *class;
	void **buf;
	size_t size;
	size_t cap;
};

struct NamespaceVector {
	const void *class;
	void (* clear)(void *_self);
	void (* reserve)(void *_self);
	void (*shrink_to_fit)(const void *_self);
	void (* push_back)(void *_self, const void *_value);
	void (* insert)(void *_self, size_t index, const void *_value);
	void (* pop_back)(void *_self);
	size_t (* find)(const void *_self, const void *_value);
	
};

extern struct NamespaceVector Vector;

#endif /* OOC_VECTOR_H */