#include <assert.h>
#include <string.h>

#include "class.h"
#include "string.h"
#include "util.h"

/**********************************************************
 * Class-Neutral Introspection
 **********************************************************/

size_t Sizeof(const var _self)
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

const void *Type(const var _self)
{
	const struct Class *self = _self;
	assert(_self);
	assert(self->class);
	return self->class;
}

const char *Nameof(const var _self)
{
	const struct Class *self = _self;
	assert(_self);
	assert(self->class);
	assert(self->class->name);
	return self->class->name;
}

bool Isinstance(const void *_class, const var _other)
{
	return _other && (Type(_other) == _class);
}

bool Issubclass(const var _self, const void *_class)
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

var New(const void *_class, ...)
{
	const struct Class *class = _class;
	var self = calloc(1, class->size);

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

void Del(var _self)
{
	const struct Class **self = _self;
	if (_self && *self && (*self)->Del) {
		_self = (* self)->Del(_self);
		free(_self);
	}
}

var Copy(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Copy);
	return (*self)->Copy(_self);
}

var Vnew(const void *_class, va_list *ap)
{
	const struct Class *class = _class;
	var self = calloc(1, class->size);

	assert(self);
	/* Technically not a Class *, it is pointing
	   to the first element of the custom struct
	   with the first member being a Class * And
	   setting that */
	*(const struct Class **)self = class;

	if (class->New) {
		self = class->New(self, ap);
	}

	return self;
}

/**********************************************************
 * Comparison
 **********************************************************/

ssize_t Cmp(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert(_other);
	assert((*self)->Cmp);
	return (*self)->Cmp(_self, _other);
}

bool Eq(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert(_other);
	assert((*self)->Eq);
	return (*self)->Eq(_self, _other);
}
// TODO: change assertions to follow above format
bool Ne(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ne);
	return (*self)->Ne(_self, _other);
}

bool Lt(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Lt);
	return (*self)->Lt(_self, _other);
}

bool Gt(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Gt);
	return (*self)->Gt(_self, _other);
}

bool Le(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Le);
	return (*self)->Le(_self, _other);
}

bool Ge(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ge);
	return (*self)->Ge(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

var Pos(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Pos);
	return (*self)->Pos(_self);
}

var Neg(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Neg);
	return (*self)->Neg(_self);
}

var Abs(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Abs);
	return (*self)->Abs(_self);
}

var Invert(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Invert);
	return (*self)->Invert(_self);
}

var Round(const var _self, size_t n)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Round);
	return (*self)->Round(_self, n);
}

var Floor(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Floor);
	return (*self)->Floor(_self);
}

var Ceil(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Ceil);
	return (*self)->Ceil(_self);
}

var Trunc(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Trunc);
	return (*self)->Trunc(_self);
}

/**********************************************************
 * Arithmetic
 **********************************************************/

var Add(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Add);
	return (*self)->Add(_self, _other);
}

var Sub(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Sub);
	return (*self)->Sub(_self, _other);
}

var Mul(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Mul);
	return (*self)->Mul(_self, _other);
}

var Floordiv(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Floordiv);
	return (*self)->Floordiv(_self, _other);
}

var Div(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Div);
	return (*self)->Div(_self, _other);
}

var Mod(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Mod);
	return (*self)->Mod(_self, _other);
}

var Pow(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Pow);
	return (*self)->Pow(_self, _other);
}

var Lshift(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Lshift);
	return (*self)->Lshift(_self, _other);
}

var Rshift(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Rshift);
	return (*self)->Rshift(_self, _other);
}

var And(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->And);
	return (*self)->And(_self, _other);
}

var Or(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Or);
	return (*self)->Or(_self, _other);
}

var Xor(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Xor);
	return (*self)->Xor(_self, _other);
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

shared Iadd(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Iadd);
	return (*self)->Iadd(_self, _other);
}

shared Isub(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Isub);
	return (*self)->Isub(_self, _other);
}

shared Imul(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Imul);
	return (*self)->Imul(_self, _other);
}

shared Ifloordiv(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ifloordiv);
	return (*self)->Ifloordiv(_self, _other);
}

shared Idiv(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Idiv);
	return (*self)->Idiv(_self, _other);
}

shared Imod(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Imod);
	return (*self)->Imod(_self, _other);
}

shared Ipow(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ipow);
	return (*self)->Ipow(_self, _other);
}

shared Ilshift(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ilshift);
	return (*self)->Ilshift(_self, _other);
}

shared Irshift(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Irshift);
	return (*self)->Irshift(_self, _other);
}

shared Iand(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Iand);
	return (*self)->Iand(_self, _other);
}

shared Ior(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ior);
	return (*self)->Ior(_self, _other);
}

shared Ixor(var _self, const var _other)
{
	struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Ixor);
	return (*self)->Ixor(_self, _other);
}

/**********************************************************
 * Representation
 **********************************************************/

size_t Hash(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Hash);
	return (*self)->Hash(_self);
}

char *Str(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Str);
	return (*self)->Str(_self);
}

char *Repr(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Repr);
	return (*self)->Repr(_self);
}

ssize_t Int(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Int);
	return (*self)->Int(_self);
}

size_t Uint(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Uint);
	return (*self)->Uint(_self);
}

double Float(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Float);
	return (*self)->Float(_self);
}

bool Bool(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Bool);
	return (*self)->Bool(_self);
}


/**********************************************************
 * Containers
 **********************************************************/

size_t Len(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Len);
	return (*self)->Len(_self);
}

shared Getitem(const var _self, const var _key)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _key);
	assert((*self)->Getitem);
	return (*self)->Getitem(_self, _key);
}

void Setitem(var _self, const var _key, const var _value)
{
	struct Class **self = _self;
	const struct Class *const *value = _value;
	assert(_self && *self && _key && _value && *value);
	assert((*value)->Copy);
	assert((*self)->Setitem);
	(*self)->Setitem(_self, _key, _value);
}

void Delitem(var _self, const var _key)
{
	struct Class **self = _self;
	assert(_self && *self && _key);
	assert((*self)->Delitem);
	(*self)->Delitem(_self, _key);
}

shared Next(var _self, ...)
{
	struct Class **self = _self;
	va_list ap;
	shared tmp;
	assert(_self && *self);
	assert((*self)->Next);
	va_start(ap, _self);
	tmp = (*self)->Next(_self, &ap);
	va_end(ap);
	return tmp;
}

var Iter(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Iter);
	return (*self)->Iter(_self);
}

var Reversed(const var _self)
{
	const struct Class *const *self = _self;
	assert(_self && *self);
	assert((*self)->Reversed);
	return (*self)->Reversed(_self);
}

bool Contains(const var _self, const var _other)
{
	const struct Class *const *self = _self;
	assert(_self && *self && _other);
	assert((*self)->Contains);
	return (*self)->Contains(_self, _other);
}

char *Format(const char *_fmt, ...)
{
	char *ret;
	va_list ap;
	va_start(ap, _fmt);
	ret = Format_va(_fmt, &ap);
	va_end(ap);
	return ret;
}

char *Format_va(const char *_fmt, va_list *ap)
{
	assert(_fmt);
	size_t i;
	var object;
	char *value;
	char *ret = NULL;

	for (i = 0; _fmt[i] != '\0'; i++) {
		if (_fmt[i] == '{' && _fmt[i + 1] == '}') {
			object = va_arg(*ap, var);
			value = Str(object);
			strcatf(&ret, "%s", value);
			free(value);
			i++;
		}
		else {
			strcatf(&ret, "%c", _fmt[i]);
		}
	}
	return ret;
}

void Println(const char *_fmt, ...)
{
	assert(_fmt);
	size_t i;
	va_list ap;
	var object;
	char *value;

	va_start(ap, _fmt);
	
	for (i = 0; _fmt[i] != '\0'; i++) {
		if (_fmt[i] == '{' && _fmt[i + 1] == '}') {
			object = va_arg(ap, var);
			value = Str(object);
			fputs(value, stdout);
			free(value);
			i++;
		}
		else {
			putc(_fmt[i], stdout);
		}
		fflush(stdout);
	}
	va_end(ap);
	printf("\n");
}
