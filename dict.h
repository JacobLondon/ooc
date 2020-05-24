#ifndef OOC_DICT_H
#define OOC_DICT_H

struct Dict {
	const void *class;
	void **values;
	void **keys;
	size_t size;
	size_t cap;
};

struct NamespaceDict {
	const void *Class;
	void     (* Clear)        (void *_self);
	void*    (* Reserve)      (void *_self, size_t mod);
	void     (* Shrink_to_fit)(void *_self);
	size_t   (* Hash)         (const void *_self, const void *_key);
};

extern struct NamespaceDict Dict;

#endif /* OOC_DICT_H */
