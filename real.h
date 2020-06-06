#ifndef OOC_REAL_H
#define OOC_REAL_H

struct Real {
	const void *class;
	double value;
};

struct NamespaceReal {
	const void *Class;
};

extern struct NamespaceReal Real;

#define $Real(value) New(Real.Class, value)

#endif /* OOC_REAL_H */
