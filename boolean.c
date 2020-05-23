#include <string.h>
#include "class.h"
#include "util.h"
#include "boolean.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void*         Boolean_New           (void *_self, va_list *ap);
static void*         Boolean_Del           (void *_self);
static void*         Boolean_Copy          (const void *_self);

// comparison
static bool          Boolean_Eq            (const void *_self, const void *_other);
static bool          Boolean_Ne            (const void *_self, const void *_other);

// representation
static size_t        Boolean_Hash          (const void *_self);
static char*         Boolean_Str           (const void *_self);
static char*         Boolean_Repr          (const void *_self);
static ssize_t       Boolean_Int           (const void *_self);
static size_t        Boolean_Uint          (const void *_self);
static double        Boolean_Float         (const void *_self);
static bool          Boolean_Bool          (const void *_self);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size      = sizeof(struct Integer),
	.class     = &class,
	.super     = NULL,
	.name      = "Boolean",

	// construction
	.New       = Boolean_New,
	.Del       = Boolean_Del,
	.Copy      = Boolean_Copy,

	// comparison
	.Cmp       = NULL,
	.Eq        = Boolean_Eq,
	.Ne        = Boolean_Ne,
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
	.Hash      = Boolean_Hash,
	.Str       = Boolean_Str,
	.Repr      = Boolean_Repr,
	.Int       = Boolean_Int,
	.Uint      = Boolean_Uint,
	.Float     = Boolean_Float,
	.Bool      = Boolean_Bool,

	// containers
	.Len       = NULL,
	.Getitem   = NULL,
	.Setitem   = NULL,
	.Delitem   = NULL,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = NULL,
};

struct NamespaceBoolean Boolean = {
	.Class = &class,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *Boolean_New(void *_self, va_list *ap)
{
	struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	self->value = va_arg(*ap, bool);
	return self;
}

static void *Boolean_Del(void *_self)
{
	struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	self->value = 0;
	return self;
}

static void *Boolean_Copy(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	return New(Boolean.Class, self->value);
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool Boolean_Eq(const void *_self, const void *_other)
{
	const struct Boolean *self = _self;
	const struct Boolean *other = _other;
	assert(self->class == Boolean.Class);

	return self->value == Bool(_other);
}

static bool Boolean_Ne(const void *_self, const void *_other)
{
	const struct Boolean *self = _self;
	const struct Boolean *other = _other;
	assert(self->class == Boolean.Class);

	return self->value != Bool(_other);
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

// representation
static size_t Boolean_Hash(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	return fnv1a(&self->value, sizeof(bool));
}

static char *Boolean_Str(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return strdup("False");
	}
	return strdup("True");
}

static char *Boolean_Repr(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	char *text = NULL;
	strcatf(&text, "'<%s object at 0x%x>'", ((struct Class *)(self))->name, (size_t)self);
	return text;
}

static ssize_t Boolean_Int(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return 0;
	}
	return 1;
}

static size_t Boolean_Uint(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return 0;
	}
	return 1;
}

static double Boolean_Float(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return 0.0;
	}
	return 1.0;
}

static bool Boolean_Bool(const void *_self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return false;
	}
	return true;
}

/**********************************************************
 * Containers
 **********************************************************/

/**********************************************************
 * Namespace Functions
 **********************************************************/
