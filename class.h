#ifndef OOC_CLASS_H
#define OOC_CLASS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>

struct Class {
	size_t size;
	const struct Class *class;
	const struct Class *super;

	// construction
	void *(* New)(void *_self, va_list *ap);
	void *(* Del)(void *_self);
	void *(* Copy)(const void *_self);

	// comparison
	ssize_t (* Cmp)(const void *_self, const void *_other);
	bool (* Eq)(const void *_self, const void *_other);
	bool (* Ne)(const void *_self, const void *_other);
	bool (* Lt)(const void *_self, const void *_other);
	bool (* Gt)(const void *_self, const void *_other);
	bool (* Le)(const void *_self, const void *_other);
	bool (* Ge)(const void *_self, const void *_other);

	// unary
	void *(* Pos)(const void *_self);
	void *(* Neg)(const void *_self);
	void *(* Abs)(const void *_self);
	void *(* Invert)(const void *_self);
	void *(* Round)(const void *_self, size_t n);
	void *(* Floor)(const void *_self);
	void *(* Ceil)(const void *_self);
	void *(* Trunc)(const void *_self);

	// arithmetic
	void *(* Add)(const void *_self, const void *_other);
	void *(* Sub)(const void *_self, const void *_other);
	void *(* Mul)(const void *_self, const void *_other);
	void *(* Floordiv)(const void *_self, const void *_other);
	void *(* Div)(const void *_self, const void *_other);
	void *(* Mod)(const void *_self, const void *_other);
	void *(* Pow)(const void *_self, const void *_other);
	void *(* Lshift)(const void *_self, const void *_other);
	void *(* Rshift)(const void *_self, const void *_other);
	void *(* And)(const void *_self, const void *_other);
	void *(* Or)(const void *_self, const void *_other);
	void *(* Xor)(const void *_self, const void *_other);

	// assignment arithmetic
	void *(* Iadd)(void *_self, const void *_other);
	void *(* Isub)(void *_self, const void *_other);
	void *(* Imul)(void *_self, const void *_other);
	void *(* Ifloordiv)(void *_self, const void *_other);
	void *(* Idiv)(void *_self, const void *_other);
	void *(* Imod)(void *_self, const void *_other);
	void *(* Ipow)(void *_self, const void *_other);
	void *(* Ilshift)(void *_self, const void *_other);
	void *(* Irshift)(void *_self, const void *_other);
	void *(* Iand)(void *_self, const void *_other);
	void *(* Ior)(void *_self, const void *_other);
	void *(* Ixor)(void *_self, const void *_other);

	// representation
	size_t (* Hash)(const void *_self);
	char *(* Str)(const void *_self);
	char *(* Repr)(const void *_self);
	ssize_t (* Int)(const void *_self);
	size_t (* Uint)(const void *_self);
	double (* Float)(const void *_self);
	bool (* Bool)(const void *_self);

	// containers
	size_t (* Len)(const void *_self);
	void *(* Getitem)(const void *_self, const void *_key);
	void (* Setitem)(void *_self, const void *_key, const void *_value);
	void (* Delitem)(void *_self, const void *_key);
	void *(* Iter)(const void *_self);
	void *(* Reversed)(const void *_self);
	bool (* Contains)(const void *_self, const void *_other);
};

// class-neutral introspection
size_t Sizeof(const void *_self);
const void *Super(const void *_self);
const void *Type(const void *_self);
bool Isinstance(const void *_self, const void *_class);
bool Issubclass(const void *_self, const void *_class);

// construction
void *New(const void *_class, ...);
void Del(void *_self);
void *Copy(const void *_self);
#define Pass ((void *)0)

// comparison
ssize_t Cmp(const void *_self, const void *_other);
bool Eq(const void *_self, const void *_other);
bool Ne(const void *_self, const void *_other);
bool Lt(const void *_self, const void *_other);
bool Gt(const void *_self, const void *_other);
bool Le(const void *_self, const void *_other);
bool Ge(const void *_self, const void *_other);

// unary
void *Pos(const void *_self);
void *Neg(const void *_self);
void *Abs(const void *_self);
void *Invert(const void *_self);
void *Round(const void *_self, size_t n);
void *Floor(const void *_self);
void *Ceil(const void *_self);
void *Trunc(const void *_self);

// arithmetic
void *Add(const void *_self, const void *_other);
void *Sub(const void *_self, const void *_other);
void *Mul(const void *_self, const void *_other);
void *Floordiv(const void *_self, const void *_other);
void *Div(const void *_self, const void *_other);
void *Mod(const void *_self, const void *_other);
void *Pow(const void *_self, const void *_other);
void *Lshift(const void *_self, const void *_other);
void *Rshift(const void *_self, const void *_other);
void *And(const void *_self, const void *_other);
void *Or(const void *_self, const void *_other);
void *Xor(const void *_self, const void *_other);

// assignment arithmetic
void *Iadd(void *_self, const void *_other);
void *Isub(void *_self, const void *_other);
void *Imul(void *_self, const void *_other);
void *Ifloordiv(void *_self, const void *_other);
void *Idiv(void *_self, const void *_other);
void *Imod(void *_self, const void *_other);
void *Ipow(void *_self, const void *_other);
void *Ilshift(void *_self, const void *_other);
void *Irshift(void *_self, const void *_other);
void *Iand(void *_self, const void *_other);
void *Ior(void *_self, const void *_other);
void *Ixor(void *_self, const void *_other);

// representation
size_t Hash(const void *_self);
char *Str(const void *_self);
char *Repr(const void *_self);
ssize_t Int(const void *_self);
size_t Uint(const void *_self);
double Float(const void *_self);
bool Bool(const void *_self);

// containers
size_t Len(const void *_self);
void *Getitem(const void *_self, const void *_key);
void Setitem(void *_self, const void *_key, const void *_value);
void Delitem(void *_self, const void *_key);
void *Iter(const void *_self);
void *Reversed(const void *_self);
bool Contains(const void *_self, const void *_other);

/* Empty Definition
static const struct Class class = {
	.size = sizeof(struct Example),
	.class = &class,
	.super = NULL,

	// construction
	.New = Example_new,
	.Del = Example_del,
	.Copy = NULL,

	// comparison
	.Cmp = NULL,
	.Eq  = NULL,
	.Ne  = NULL,
	.Lt  = NULL,
	.Gt  = NULL,
	.Le  = NULL,
	.Ge  = NULL,

	// unary
	.Pos = NULL,
	.Neg = NULL,
	.Abs = NULL,
	.Invert = NULL,
	.Round = NULL,
	.Floor = NULL,
	.Ceil = NULL,
	.Trunc = NULL,

	// arithmetic
	.Add = NULL,
	.Sub = NULL,
	.Mul = NULL,
	.Floordiv = NULL,
	.Div = NULL,
	.Mod = NULL,
	.Pow = NULL,
	.Lshift = NULL,
	.Rshift = NULL,
	.And = NULL,
	.Or = NULL,
	.Xor = NULL,

	// assignment arithmetic
	.Iadd = NULL,
	.Isub = NULL,
	.Imul = NULL,
	.Ifloordiv = NULL,
	.Idiv = NULL,
	.Imod = NULL,
	.Ipow = NULL,
	.Ilshift = NULL,
	.Irshift = NULL,
	.Iand = NULL,
	.Ior = NULL,
	.Ixor = NULL,

	// representation
	.Hash = NULL,
	.Str = NULL,
	.Repr = NULL,
	.Int = NULL,
	.Uint = NULL,
	.Float = NULL,
	.Bool = NULL,

	// containers
	.Len = NULL,
	.Getitem = NULL,
	.Setitem = NULL,
	.Delitem = NULL,
	.Iter = NULL,
	.Reversed = NULL,
	.Contains = NULL,
};
*/

#endif /* OOC_CLASS_H */