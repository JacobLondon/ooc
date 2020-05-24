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
	const void *Class;
	void     (* Clear)        (void *_self);
	void     (* Reserve)      (void *_self, size_t cap);
	void     (* Shrink_to_fit)(void *_self);
	void     (* Push_back)    (void *_self, void *_value);
	void     (* Pop_back)     (void *_self);
	size_t   (* Find)         (const void *_self, const void *_value);
	void*    (* Emplace_back) (void *_self, const void *_class, ...);
};

extern struct NamespaceVector Vector;

#endif /* OOC_VECTOR_H */
