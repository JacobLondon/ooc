#ifndef OOC_DICT_H
#define OOC_DICT_H

struct Dict {
	const void *class;
	void **values;
	void **keys;
	size_t len;
	size_t cap;
};

extern const void *Dict;

#endif /* OOC_DICT_H */