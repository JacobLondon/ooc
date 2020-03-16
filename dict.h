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
	const void *class;
	void (* clear)(void *_self);
	void *(* reserve)(void *_self, size_t mod);
	void (* shrink_to_fit)(void *_self);
	size_t (* hash)(const void *_self, const void *_key);
};

extern struct NamespaceDict Dict;

#endif /* OOC_DICT_H */