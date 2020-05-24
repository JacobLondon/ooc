#include <assert.h>
#include <math.h>
#include <stdarg.h>
#include "class.h"
#include "util.h"
#include "integer.h"
#include "boolean.h"
#include "real.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void*         Real_New           (void *_self, va_list *ap);
static void*         Real_Del           (void *_self);
static void*         Real_Copy          (const void *_self);

// comparison
static ssize_t       Real_Cmp           (const void *_self, const void *_other);
static bool          Real_Eq            (const void *_self, const void *_other);
static bool          Real_Ne            (const void *_self, const void *_other);
static bool          Real_Lt            (const void *_self, const void *_other);
static bool          Real_Gt            (const void *_self, const void *_other);
static bool          Real_Le            (const void *_self, const void *_other);
static bool          Real_Ge            (const void *_self, const void *_other);

// unary
static void*         Real_Pos           (const void *_self);
static void*         Real_Neg           (const void *_self);
static void*         Real_Abs           (const void *_self);
static void*         Real_Round         (const void *_self, size_t n);
static void*         Real_Floor         (const void *_self);
static void*         Real_Ceil          (const void *_self);
static void*         Real_Trunc         (const void *_self);

// arithmetic
static void*         Real_Add           (const void *_self, const void *_other);
static void*         Real_Sub           (const void *_self, const void *_other);
static void*         Real_Mul           (const void *_self, const void *_other);
static void*         Real_Floordiv      (const void *_self, const void *_other);
static void*         Real_Div           (const void *_self, const void *_other);
static void*         Real_Mod           (const void *_self, const void *_other);
static void*         Real_Pow           (const void *_self, const void *_other);

// assignment arithmetic
static void*         Real_Iadd          (void *_self, const void *_other);
static void*         Real_Isub          (void *_self, const void *_other);
static void*         Real_Imul          (void *_self, const void *_other);
static void*         Real_Ifloordiv     (void *_self, const void *_other);
static void*         Real_Idiv          (void *_self, const void *_other);
static void*         Real_Imod          (void *_self, const void *_other);
static void*         Real_Ipow          (void *_self, const void *_other);

// representation
static size_t        Real_Hash          (const void *_self);
static char*         Real_Str           (const void *_self);
static char*         Real_Repr          (const void *_self);
static ssize_t       Real_Int           (const void *_self);
static size_t        Real_Uint          (const void *_self);
static double        Real_Float         (const void *_self);
static bool          Real_Bool          (const void *_self);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size      = sizeof(struct Real),
	.class     = &class,
	.super     = NULL,
	.name      = "Real",

	// construction
	.New       = Real_New,
	.Del       = Real_Del,
	.Copy      = Real_Copy,

	// comparison
	.Cmp       = Real_Cmp,
	.Eq        = Real_Eq,
	.Ne        = Real_Ne,
	.Lt        = Real_Lt,
	.Gt        = Real_Gt,
	.Le        = Real_Le,
	.Ge        = Real_Ge,

	// unary
	.Pos       = Real_Pos,
	.Neg       = Real_Neg,
	.Abs       = Real_Abs,
	.Invert    = NULL,
	.Round     = Real_Round,
	.Floor     = Real_Floor,
	.Ceil      = Real_Ceil,
	.Trunc     = Real_Trunc,

	// arithmetic
	.Add       = Real_Add,
	.Sub       = Real_Sub,
	.Mul       = Real_Mul,
	.Floordiv  = Real_Floordiv,
	.Div       = Real_Div,
	.Mod       = Real_Mod,
	.Pow       = Real_Pow,
	.Lshift    = NULL,
	.Rshift    = NULL,
	.And       = NULL,
	.Or        = NULL,
	.Xor       = NULL,

	// assignment arithmetic
	.Iadd      = Real_Iadd,
	.Isub      = Real_Isub,
	.Imul      = Real_Imul,
	.Ifloordiv = Real_Ifloordiv,
	.Idiv      = Real_Idiv,
	.Imod      = Real_Imod,
	.Ipow      = Real_Ipow,
	.Ilshift   = NULL,
	.Irshift   = NULL,
	.Iand      = NULL,
	.Ior       = NULL,
	.Ixor      = NULL,

	// representation
	.Hash      = Real_Hash,
	.Str       = Real_Str,
	.Repr      = Real_Repr,
	.Int       = Real_Int,
	.Uint      = Real_Uint,
	.Float     = Real_Float,
	.Bool      = Real_Bool,

	// containers
	.Len       = NULL,
	.Getitem   = NULL,
	.Setitem   = NULL,
	.Delitem   = NULL,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = NULL,
};

struct NamespaceReal Real = {
	.Class = &class,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *Real_New(void *_self, va_list *ap)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = va_arg(*ap, double);
	return self;
}

static void *Real_Del(void *_self)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = 0.0;
	return self;
}

static void *Real_Copy(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, self->value);
}

/**********************************************************
 * Comparison
 **********************************************************/

static ssize_t Real_Cmp(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);

	if (Isinstance(Real.Class, _other)) {
		if (self->value < Float(_other)) {
			return -1;
		}
		else if (Float(self) == Float(_other)) {
			return 0;
		}
		else {
			return 1;
		}
	}

	if (self->value < Int(_other)) {
		return -1;
	}
	else if (self->value == Int(_other)) {
		return 0;
	}
	else {
		return 1;
	}
}

static bool Real_Eq(const void *_self, const void *_other)
{
	return Cmp(_self, _other) == 0;
}

static bool Real_Ne(const void *_self, const void *_other)
{
	return Cmp(_self, _other) != 0;
}

static bool Real_Lt(const void *_self, const void *_other)
{
	return Cmp(_self, _other) < 0;
}

static bool Real_Gt(const void *_self, const void *_other)
{
	return Cmp(_self, _other) > 0;
}

static bool Real_Le(const void *_self, const void *_other)
{
	return Cmp(_self, _other) <= 0;
}

static bool Real_Ge(const void *_self, const void *_other)
{
	return Cmp(_self, _other) >= 0;
}

/**********************************************************
 * Unary
 **********************************************************/

static void *Real_Pos(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value < 0.0 ? +self->value : self->value));
}

static void *Real_Neg(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value > 0.0 ? -self->value : self->value));
}

static void *Real_Abs(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(fabs(self->value)));
}

static void *Real_Round(const void *_self, size_t n)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(round(self->value)));
}

static void *Real_Floor(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(floor(self->value)));
}

static void *Real_Ceil(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(ceil(self->value)));
}

static void *Real_Trunc(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(trunc(self->value)));
}

/**********************************************************
 * Arithmetic
 **********************************************************/

static void *Real_Add(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value + Float(_other)));
}

static void *Real_Sub(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value - Float(_other)));
}

static void *Real_Mul(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value * Float(_other)));
}

static void *Real_Floordiv(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)floor(self->value / Float(_other)));
}

static void *Real_Div(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value / Float(_other)));
}

static void *Real_Mod(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)fmod(self->value, Float(_other)));
}

static void *Real_Pow(const void *_self, const void *_other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)pow(self->value, Float(_other)));
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static void *Real_Iadd(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value + Float(_other);
	return self;
}

static void *Real_Isub(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value - Float(_other);
	return self;
}

static void *Real_Imul(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value * Float(_other);
	return self;
}

static void *Real_Ifloordiv(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = floor(self->value / Float(_other));
	return self;
}

static void *Real_Idiv(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value / Float(_other);
	return self;
}

static void *Real_Imod(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = fmod(self->value, Float(_other));
	return self;
}

static void *Real_Ipow(void *_self, const void *_other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = pow(self->value, Float(_other));
	return self;
}

/**********************************************************
 * Representation
 **********************************************************/

static size_t Real_Hash(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return fnv1a(&self->value, sizeof(self->value));
}

static char *Real_Str(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	char *text = NULL;
	strcatf(&text, "%lf", self->value);
	assert(text);
	return text;
}

static char *Real_Repr(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	char *text = NULL;
	strcatf(&text, "'<%s object at 0x%x>'", Classof(self)->name, (size_t)self);
}

static ssize_t Real_Int(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return (ssize_t)self->value;
}

static size_t Real_Uint(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return (size_t)self->value;
}

static double Real_Float(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return (double)self->value;
}

static bool Real_Bool(const void *_self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return self->value != 0.0;
}

/**********************************************************
 * Containers
 **********************************************************/

/**********************************************************
 * Namespace Functions
 **********************************************************/
