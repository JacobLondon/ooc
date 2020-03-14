#ifndef OOC_DICT_H
#define OOC_DICT_H

struct Dict {
	const void *class;
	void **values;
	void **keys;
	size_t size;
	size_t cap;
};

struct Namespace_dict {
	const void *Class;
	size_t (* Hash)(const void *_self, const void *_key);
	void *(* Grow)(void *_self, size_t mod);
};

extern struct Namespace_dict Dict;

#endif /* OOC_DICT_H */