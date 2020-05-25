#ifndef OOC_DICT_H
#define OOC_DICT_H

struct Dict {
	const void *class;
	var *values;
	var *keys;
	size_t size;
	size_t cap;
};

struct NamespaceDict {
	const void *Class;
	void     (* Clear)        (var _self);
	var      (* Reserve)      (var _self, size_t mod);
	void     (* Shrink_to_fit)(var _self);
	size_t   (* Hash)         (const var _self, const var _key);
};

extern struct NamespaceDict Dict;

#endif /* OOC_DICT_H */
