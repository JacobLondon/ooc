#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "util.h"
#include "class.h"
#include "vector.h"

#define VECTOR_DEFAULT_CAP 8
#define VECTOR_DEFAULT_SCALING 2

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void*         Vector_New           (void *_self, va_list *ap);
static void*         Vector_Del           (void *_self);
static void*         Vector_Copy          (const void *_self);

// comparison
static bool          Vector_Eq            (const void *_self, const void *_other);
static bool          Vector_Ne            (const void *_self, const void *_other);

// unary
// arithmetic
// assignment arithmetic

// representation
static char*         Vector_Str           (const void *_self);
static char*         Vector_Repr          (const void *_self);
static bool          Vector_Bool          (const void *_self);

// containers
static size_t        Vector_Len           (const void *_self);
static void*         Vector_Getitem       (const void *_self, const void *_key);
static void          Vector_Setitem       (void *_self, const void *_key, const void *_value);
static void          Vector_Delitem       (void *_self, const void *_key);
static bool          Vector_Contains      (const void *_self, const void *_other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static void          NamespaceVector_Push_back  (void *_self, const void *_value);


/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size      = sizeof(struct Vector),
	.class     = &class,
	.super     = NULL,
	.name      = "Vector",

	// construction
	.New       = Vector_New,
	.Del       = Vector_Del,
	.Copy      = Vector_Copy,

	// comparison
	.Cmp       = NULL,
	.Eq        = Vector_Eq,
	.Ne        = Vector_Ne,
	.Lt        = NULL,
	.Gt        = NULL,
	.Le        = NULL,
	.Ge        = NULL,

	// unary
	.Pos       = NULL,
	.Neg       = NULL,
	.Abs       = NULL,
	.Invert    = NULL,
	.Round     = NULL,
	.Floor     = NULL,
	.Ceil      = NULL,
	.Trunc     = NULL,

	// arithmetic
	.Add       = NULL,
	.Sub       = NULL,
	.Mul       = NULL,
	.Floordiv  = NULL,
	.Div       = NULL,
	.Mod       = NULL,
	.Pow       = NULL,
	.Lshift    = NULL,
	.Rshift    = NULL,
	.And       = NULL,
	.Or        = NULL,
	.Xor       = NULL,

	// assignment arithmetic
	.Iadd      = NULL,
	.Isub      = NULL,
	.Imul      = NULL,
	.Ifloordiv = NULL,
	.Idiv      = NULL,
	.Imod      = NULL,
	.Ipow      = NULL,
	.Ilshift   = NULL,
	.Irshift   = NULL,
	.Iand      = NULL,
	.Ior       = NULL,
	.Ixor      = NULL,

	// representation
	.Hash      = NULL,
	.Str       = Vector_Str,
	.Repr      = NULL,
	.Int       = NULL,
	.Uint      = NULL,
	.Float     = NULL,
	.Bool      = Vector_Bool,

	// containers
	.Len       = Vector_Len,
	.Getitem   = Vector_Getitem,
	.Setitem   = Vector_Setitem,
	.Delitem   = Vector_Delitem,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = Vector_Contains,
};

struct NamespaceVector Vector = {
	.Class         = &class,
	.Clear         = NULL,
	.Shrink_to_fit = NULL,
	.Push_back     = NamespaceVector_Push_back,
	.Pop_back      = NULL,
	.Find          = NULL,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *Vector_New(void *_self, va_list *ap)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	self->buf = calloc(VECTOR_DEFAULT_CAP, sizeof(void *));
	assert(self->buf);

	self->size = 0;
	self->cap = VECTOR_DEFAULT_CAP;

	return self;
}

static void *Vector_Del(void *_self)
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

static void *Vector_Copy(const void *_self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	void *_new = New(Vector.Class);
	struct Vector *new = _new;

	for (size_t i = 0; i < self->size; i++) {
		Vector.Push_back(new, self->buf[i]);
	}

	return new;
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool Vector_Eq(const void *_self, const void *_other)
{
	const struct Vector *self = _self;
	const struct Vector *other = _other;
	assert(self->class == Vector.Class);
	assert(other->class == Vector.Class);

	if (self->size != other->size || self->cap != other->cap) {
		return false;
	}

	for (size_t i = 0; i < self->size && i < other->size; i++) {
		if (Ne(self->buf[i], other->buf[i])) {
			return false;
		}
	}
	return true;
}

static bool Vector_Ne(const void *_self, const void *_other)
{
	return !Vector_Eq(_self, _other);
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

static char *Vector_Str(const void *_self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	char *value;
	char *text = strdup("[");
	size_t i;
	for (i = 0; i < self->size; i++) {
		value = Str(self->buf[i]);
		strcatf(&text, "%s, ", value);
		free(value);
	}
	text[strlen(text) - 2] = ']';
	return text;
}

static char *Vector_Repr(const void *_self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	char *text = NULL;
	strcatf(&text, "'<%s object at 0x%x>'", ((struct Class *)(self))->name, (size_t)self);
	return text;
}

static bool Vector_Bool(const void *_self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->size > 0;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t Vector_Len(const void *_self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->size;
}

static void *Vector_Getitem(const void *_self, const void *_key)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->buf[Uint(_key)];
}

static void Vector_Setitem(void *_self, const void *_key, const void *_value)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t idx = Uint(_key);
	if (self->buf[idx] != NULL) {
		Del(self->buf[idx]);
	}
	self->buf[idx] = Copy(_value);
}

static void Vector_Delitem(void *_self, const void *_key)
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

static bool Vector_Contains(void *_self, const void *_other)
{
	struct Vector *self = _self;
	struct Vector *other = _other;
	assert(self->class == Vector.Class);

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


