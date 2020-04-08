#include <assert.h>

#include "class.h"
#include "string.h"

/**********************************************************
 * Class-Neutral Introspection
 **********************************************************/

size_t Sizeof(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	return (*self)->size;
}

const void *Super(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && (*self));
	assert((*self)->super);
	return (*self)->super;
}

const void *Type(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->class);
	return (*self)->class;
}

bool Isinstance(const void *_self, const void *_class)
{
	return _self && Type(_self);
}

bool Issubclass(const void *_self, const void *_class)
{
	assert(_class);
	
	if (_self) {
		const struct Class *class = Type(_self);
		if (class != NULL) {
			while (class != _class) {
				if (class != NULL) {
					class = Super(class);
				}
				else {
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

/**********************************************************
 * Construction
 **********************************************************/

void *New(const void *_class, ...)
{
	const struct Class *class = _class;
	void *self = calloc(1, class->size);

	assert(self);
	/* Technically not a Class *, it is pointing
	   to the first element of the custom struct
	   with the first member being a Class * And
	   setting that */
	*(const struct Class **)self = class;

	if (class->New) {
		va_list ap;
		va_start(ap, _class);
		self = class->New(self, &ap);
		va_end(ap);
	}

	return self;
}

void Del(void *_self)
{
	const struct Class **self = _self;
	if (_self && *self && (*self)->Del) {
		_self = (* self)->Del(_self);
		free(_self);
	}
}

void *Copy(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Copy);
	return (*self)->Copy(_self);
}

/**********************************************************
 * Comparison
 **********************************************************/

ssize_t Cmp(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert(_other);
	assert((*self)->Cmp);
	return (*self)->Cmp(_self, _other);
}

bool Eq(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert(_other);
	assert((*self)->Eq);
	return (*self)->Eq(_self, _other);
}
// TODO: change assertions to follow above format
bool Ne(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ne);
	return (*self)->Ne(_self, _other);
}

bool Lt(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Lt);
	return (*self)->Lt(_self, _other);
}

bool Gt(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Gt);
	return (*self)->Gt(_self, _other);
}

bool Le(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Le);
	return (*self)->Le(_self, _other);
}

bool Ge(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ge);
	return (*self)->Ge(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

void *Pos(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Pos);
	return (*self)->Pos(_self);
}

void *Neg(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Neg);
	return (*self)->Neg(_self);
}

void *Abs(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Abs);
	return (*self)->Abs(_self);
}

void *Invert(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Invert);
	return (*self)->Invert(_self);
}

void *Round(const void *_self, size_t n)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Round);
	return (*self)->Round(_self, n);
}

void *Floor(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Floor);
	return (*self)->Floor(_self);
}

void *Ceil(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Ceil);
	return (*self)->Ceil(_self);
}

void *Trunc(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Trunc);
	return (*self)->Trunc(_self);
}

/**********************************************************
 * Arithmetic
 **********************************************************/

void *Add(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Add);
	return (*self)->Add(_self, _other);
}

void *Sub(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Sub);
	return (*self)->Sub(_self, _other);
}

void *Mul(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Mul);
	return (*self)->Mul(_self, _other);
}

void *Floordiv(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Floordiv);
	return (*self)->Floordiv(_self, _other);
}

void *Div(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Div);
	return (*self)->Div(_self, _other);
}

void *Mod(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Mod);
	return (*self)->Mod(_self, _other);
}

void *Pow(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Pow);
	return (*self)->Pow(_self, _other);
}

void *Lshift(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Lshift);
	return (*self)->Lshift(_self, _other);
}

void *Rshift(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Rshift);
	return (*self)->Rshift(_self, _other);
}

void *And(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->And);
	return (*self)->And(_self, _other);
}

void *Or(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Or);
	return (*self)->Or(_self, _other);
}

void *Xor(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Xor);
	return (*self)->Xor(_self, _other);
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

void *Iadd(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Iadd);
	return (*self)->Iadd(_self, _other);
}

void *Isub(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Isub);
	return (*self)->Isub(_self, _other);
}

void *Imul(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Imul);
	return (*self)->Imul(_self, _other);
}

void *Ifloordiv(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ifloordiv);
	return (*self)->Ifloordiv(_self, _other);
}

void *Idiv(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Idiv);
	return (*self)->Idiv(_self, _other);
}

void *Imod(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Imod);
	return (*self)->Imod(_self, _other);
}

void *Ipow(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ipow);
	return (*self)->Ipow(_self, _other);
}

void *Ilshift(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ilshift);
	return (*self)->Ilshift(_self, _other);
}

void *Irshift(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Irshift);
	return (*self)->Irshift(_self, _other);
}

void *Iand(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Iand);
	return (*self)->Iand(_self, _other);
}

void *Ior(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ior);
	return (*self)->Ior(_self, _other);
}

void *Ixor(void *_self, const void *_other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ixor);
	return (*self)->Ixor(_self, _other);
}

/**********************************************************
 * Representation
 **********************************************************/

size_t Hash(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Hash);
	return (*self)->Hash(_self);
}

char *Str(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Str);
	return (*self)->Str(_self);
}

char *Repr(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Repr);
	return (*self)->Repr(_self);
}

ssize_t Int(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Int);
	return (*self)->Int(_self);
}

double Float(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Float);
	return (*self)->Float(_self);
}

bool Bool(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Bool);
	return (*self)->Bool(_self);
}


/**********************************************************
 * Containers
 **********************************************************/

size_t Len(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Len);
	return (*self)->Len(_self);
}

void *Getitem(const void *_self, const void *_key)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _key);
	assert((*self)->Getitem);
	return (*self)->Getitem(_self, _key);
}

void Setitem(void *_self, const void *_key, const void *_value)
{
	struct Class **self = _self;
	const struct Class *const *value = _value;
	assert(_self && *self && _key && _value && *value);
	assert((*value)->Copy);
	assert((*self)->Setitem);
	(*self)->Setitem(_self, _key, _value);
}

void Delitem(void *_self, const void *_key)
{
	struct Class **self = _self;
	assert(_self && *self && _key);
	assert((*self)->Delitem);
	(*self)->Delitem(_self, _key);
}

void *Iter(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Iter);
	return (*self)->Iter(_self);
}

void *Reversed(const void *_self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Reversed);
	return (*self)->Reversed(_self);
}

bool Contains(const void *_self, const void *_other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Contains);
	return (*self)->Contains(_self, _other);
}
