#ifndef OOC_VECTOR_H
#define OOC_VECTOR_H

#include <stddef.h>

struct Vector {
	const void *class;
	var *buf;
	size_t size;
	size_t cap;
};

struct NamespaceVector {
	const void *Class;
	void     (* Clear)        (var _self);
	void     (* Reserve)      (var _self, size_t cap);
	void     (* Shrink_to_fit)(var _self);
	void     (* Push_back)    (var _self, var _value);
	void     (* Pop_back)     (var _self);
	size_t   (* Find)         (const var _self, const var _value);
	void     (* Emplace_back) (var _self, const void *_class, ...);
};

extern struct NamespaceVector Vector;

#endif /* OOC_VECTOR_H */
