#include <assert.h>
#include <string.h>
#include "class.h"
#include "util.h"
#include "boolean.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static var           Boolean_New           (var _self, va_list *ap);
static var           Boolean_Del           (var _self);
static var           Boolean_Copy          (const var _self);

// comparison
static bool          Boolean_Eq            (const var _self, const var _other);
static bool          Boolean_Ne            (const var _self, const var _other);

// representation
static size_t        Boolean_Hash          (const var _self);
static char*         Boolean_Str           (const var _self);
static char*         Boolean_Repr          (const var _self);
static ssize_t       Boolean_Int           (const var _self);
static size_t        Boolean_Uint          (const var _self);
static double        Boolean_Float         (const var _self);
static bool          Boolean_Bool          (const var _self);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "Boolean",
	.size      = sizeof(struct Boolean),

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
	.Next      = NULL,
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

static var Boolean_New(var _self, va_list *ap)
{
	struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	self->value = (bool)va_arg(*ap, int);
	return self;
}

static var Boolean_Del(var _self)
{
	struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	self->value = 0;
	return self;
}

static var Boolean_Copy(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	return New(Boolean.Class, self->value);
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool Boolean_Eq(const var _self, const var _other)
{
	const struct Boolean *self = _self;
	const struct Boolean *other = _other;
	assert(self->class == Boolean.Class);

	return self->value == Bool(_other);
}

static bool Boolean_Ne(const var _self, const var _other)
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
static size_t Boolean_Hash(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	return fnv1a(&self->value, sizeof(self->value));
}

static char *Boolean_Str(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	char *text = NULL;
	if (self->value == false) {
		text = strdup("false");
	}
	else {
		text = strdup("true");
	}
	assert(text);
	return text;
}

static char *Boolean_Repr(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	char *text = NULL;
	strcatf(&text, "\"<%s object at 0x%x>\"", Nameof(_self), (size_t)self);
	return text;
}

static ssize_t Boolean_Int(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return 0;
	}
	return 1;
}

static size_t Boolean_Uint(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return 0;
	}
	return 1;
}

static double Boolean_Float(const var _self)
{
	const struct Boolean *self = _self;
	assert(self->class == Boolean.Class);
	if (self->value == false) {
		return 0.0;
	}
	return 1.0;
}

static bool Boolean_Bool(const var _self)
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
