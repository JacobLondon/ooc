#ifndef OOC_REAL_H
#define OOC_REAL_H

struct Real {
	const void *class;
	double value;
};

struct NamespaceReal {
	const void *Class;
	double (* Add)(var _self, double other);
	double (* Sub)(var _self, double other);
	double (* Mul)(var _self, double other);
	double (* Div)(var _self, double other);
};

extern struct NamespaceReal Real;

#define $Real(value) New(Real.Class, value)

#endif /* OOC_REAL_H */
