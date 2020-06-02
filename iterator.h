#ifndef OOC_ITERATOR_H
#define OOC_ITERATOR_H

struct Iterator {
	const void *class;
	void *object;
};

struct NamespaceIterator {
	const void *Class;
};

extern struct NamespaceIterator Iterator;

#endif /* OOC_ITERATOR_H */
