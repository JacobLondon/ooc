#include <assert.h>
#include <stdarg.h>

#include "class.h"
#include "vector.h"

#define VECTOR_DEFAULT_CAP 8
#define VECTOR_DEFAULT_SCALING 2

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void *Vector_new(void *_self, va_list *ap);
static void *Vector_del(void *_self);
static void *Vector_copy(const void *_self);

// comparison
static bool Vector_eq(const void *_self, const void *_other);
static bool Vector_ne(const void *_self, const void *_other);

// unary
// arithmetic
// assignment arithmetic

// representation
static char *Vector_str(const void *_self);
static bool Vector_bool(const void *_self);

// containers
static size_t Vector_len(const void *_self);
static void *Vector_getitem(const void *_self, const void *_key);
static void Vector_setitem(void *_self, const void *_key, const void *_value);
static void Vector_delitem(void *_self, const void *_key);
static bool Vector_contains(const void *_self, const void *_other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static void NamespaceVector_push_back(void *_self, const void *_value);


/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size = sizeof(struct Vector),
	.class = &class,
	.super = NULL,

	// construction
	.New = Vector_new,
	.Del = Vector_del,
	.Copy = Vector_copy,

	// comparison
	.Cmp = NULL,
	.Eq  = Vector_eq,
	.Ne  = Vector_ne,
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
	.Str = Vector_str,
	.Repr = NULL,
	.Int = NULL,
	.Uint = NULL,
	.Float = NULL,
	.Bool = Vector_bool,

	// containers
	.Len = Vector_len,
	.Getitem = Vector_getitem,
	.Setitem = Vector_setitem,
	.Delitem = Vector_delitem,
	.Iter = NULL,
	.Reversed = NULL,
	.Contains = Vector_contains,
};

struct NamespaceVector Vector = {
	.class = &class,
	.push_back = NamespaceVector_push_back,
	.pop_back = NULL,
	.insert = NULL,
	.find = NULL,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *Vector_new(void *_self, va_list *ap)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	self->buf = calloc(VECTOR_DEFAULT_CAP, sizeof(void *));
	assert(self->buf);

	self->size = 0;
	self->cap = VECTOR_DEFAULT_CAP;

	return self;
}

static void *Vector_del(void *_self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	for (size_t i = 0; i < self->cap; i++) {
		if (self->buf[i]) {
			Del(self->buf[i]);
		}
	}

	free(self->buf);
	self->buf = NULL;

	return self;
}

static void *Vector_copy(const void *_self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	void *_new = New(Vector.Class);
	struct Vector *new = _new;

	for (size_t i = 0; i < self->size; i++) {
		Vector.Append(new, self->buf[i]);
	}

	return new;
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool Vector_eq(const void *_self, const void *_other)
{
	const struct Vector *self = _self;
	const struct Vector *other = _other;
	assert(self->class == Vector.Class);
	assert(other->class == Vector.Class);

	if (self->size != other->size || self->cap != other->cap) {
		return false;
	}

	for (size_t i = 0; i < self->size; i++) {
		if (Ne(self->buf[i], other->buf[i])) {
			return false;
		}
	}
	return true;
}

static bool Vector_ne(const void *_self, const void *_other)
{
	return !Vector_eq(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

/**********************************************************
 * Arithmetic
 **********************************************************/

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

/**********************************************************
 * Representation
 **********************************************************/

static char *Vector_str(const void *_self)
{
	return "TODO";
}

static bool Vector_bool(const void *_self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->size > 0;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t Vector_len(const void *_self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->size;
}

static void *Vector_getitem(const void *_self, const void *_key)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->buf[Uint(_key)];
}

static void Vector_setitem(void *_self, const void *_key, const void *_value)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	self->buf[Uint(_key)] = Copy(_value);
}

static void Vector_delitem(void *_self, const void *_key)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t idx = Uint(_key);
	Del(self->buf[idx]);

	// shift buffer
	for (size_t i = idx; self->buf[i] < self->size; i++) {
		self->buf[i] = self->buf[i + 1];
	}
	self->size--;
}

static bool Vector_contains(void *_self, const void *_other)
{
	struct Vector *self = _self;
	struct Vector *self = _other;
	assert(self->class == Vector.Class);
	assert(other->class == Vector.Class);

	// unsorted linear search
	for (size_t i = 0; i < self->size; i++) {
		if (Eq(self->buf[i], _other)) {
			return true;
		}
	}
	return false;
}

/**********************************************************
 * Namespace Functions
 **********************************************************/


