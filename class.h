#ifndef OOC_CLASS_H
#define OOC_CLASS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

struct Class {
	size_t size;
	const struct Class *class;
	const struct Class *super;
	/* class methods */
	void *(* new)(void *_self, va_list *ap);
	void *(* del)(void *_self);
	void *(* copy)(const void *_self);
	size_t (* hash)(const void *_self);
	size_t (* len)(const void *_self);
	char *(* str)(const void *_self);
};

/**
 * Class Methods
 */

/* construction */
void *new(const void *_class, ...);
void del(void *_self);
void *copy(const void *_self);
/* comparison */
//bool cmp(const void *_self);
//bool eq(const void *_self);
//bool ne(const void *_self);
//bool lt(const void *_self);
//bool gt(const void *_self);
//bool le(const void *_self);
//bool ge(const void *_self);
/* unary */
//void pos(const void *_self);
//void neg(const void *_self);
//void abs(const void *_self);
//void invert(const void *_self);
//void round(const void *_self, size_t n);
//void floor(const void *_self);
//void ceil(const void *_self);
//void trunc(const void *_self);
/* arithmetic */
//void add(const void *_self, const void *_other);
//void sub(const void *_self, const void *_other);
//void mul(const void *_self, const void *_other);
//void floordiv(const void *_self, const void *_other);
//void div(const void *_self, const void *_other);
//void mod(const void *_self, const void *_other);
//void pow(const void *_self, const void *_other);
//void lshift(const void *_self, const void *_other);
//void rshift(const void *_self, const void *_other);
//void and(const void *_self, const void *_other);
//void or(const void *_self, const void *_other);
//void xor(const void *_self, const void *_other);
/* augmented assignment */
//void iadd(const void *_self, const void *_other);
//void isub(const void *_self, const void *_other);
//void imul(const void *_self, const void *_other);
//void ifloordiv(const void *_self, const void *_other);
//void idiv(const void *_self, const void *_other);
//void imod(const void *_self, const void *_other);
//void ipow(const void *_self, const void *_other);
//void ilshift(const void *_self, const void *_other);
//void irshift(const void *_self, const void *_other);
//void iand(const void *_self, const void *_other);
//void ior(const void *_self, const void *_other);
//void ixor(const void *_self, const void *_other);

/* representation */
size_t hash(const void *_self);
char *str(const void *_self);
//char *repr(const void *_self);
/* containers */
size_t len(const void *_self);
//void *getitem(const void *_self, const void *_key);
//void setitem(const void *_self, const void *_key, const void *_value);
//void delitem(const void *_self, const void *_key);
//void *iter(const void *_self);
//void *reversed(const void *_self);
//bool contains(const void *_self);

//ssize_t as_int(const void *_self);
//double as_flt(const void *_self);
//bool as_bool(const void *_self);

/**
 * Methods
 */
size_t size_of(const void *_self);
const void *super(const void *_self);
const void *type(const void *_self);
bool isinstance(const void *_self, const void *_class);
bool issubclass(const void *_self, const void *_class);

#endif /* OOC_CLASS_H */