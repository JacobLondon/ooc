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
static var           Real_New           (var _self, va_list *ap);
static var           Real_Del           (var _self);
static var           Real_Copy          (const var _self);

// comparison
static ssize_t       Real_Cmp           (const var _self, const var _other);
static bool          Real_Eq            (const var _self, const var _other);
static bool          Real_Ne            (const var _self, const var _other);
static bool          Real_Lt            (const var _self, const var _other);
static bool          Real_Gt            (const var _self, const var _other);
static bool          Real_Le            (const var _self, const var _other);
static bool          Real_Ge            (const var _self, const var _other);

// unary
static var           Real_Pos           (const var _self);
static var           Real_Neg           (const var _self);
static var           Real_Abs           (const var _self);
static var           Real_Round         (const var _self, size_t n);
static var           Real_Floor         (const var _self);
static var           Real_Ceil          (const var _self);
static var           Real_Trunc         (const var _self);

// arithmetic
static var           Real_Add           (const var _self, const var _other);
static var           Real_Sub           (const var _self, const var _other);
static var           Real_Mul           (const var _self, const var _other);
static var           Real_Floordiv      (const var _self, const var _other);
static var           Real_Div           (const var _self, const var _other);
static var           Real_Mod           (const var _self, const var _other);
static var           Real_Pow           (const var _self, const var _other);

// assignment arithmetic
static shared        Real_Iadd          (var _self, const var _other);
static shared        Real_Isub          (var _self, const var _other);
static shared        Real_Imul          (var _self, const var _other);
static shared        Real_Ifloordiv     (var _self, const var _other);
static shared        Real_Idiv          (var _self, const var _other);
static shared        Real_Imod          (var _self, const var _other);
static shared        Real_Ipow          (var _self, const var _other);

// representation
static size_t        Real_Hash          (const var _self);
static char*         Real_Str           (const var _self);
static char*         Real_Repr          (const var _self);
static ssize_t       Real_Int           (const var _self);
static size_t        Real_Uint          (const var _self);
static double        Real_Float         (const var _self);
static bool          Real_Bool          (const var _self);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

double               NamespaceReal_Add  (var _self, double other);
double               NamespaceReal_Sub  (var _self, double other);
double               NamespaceReal_Mul  (var _self, double other);
double               NamespaceReal_Div  (var _self, double other);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "Real",
	.size      = sizeof(struct Real),

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
	.Next      = NULL,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = NULL,
};

struct NamespaceReal Real = {
	.Class = &class,
	.Add = NamespaceReal_Add,
	.Sub = NamespaceReal_Sub,
	.Mul = NamespaceReal_Mul,
	.Div = NamespaceReal_Div,
};

/**********************************************************
 * Construction
 **********************************************************/

static var Real_New(var _self, va_list *ap)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = va_arg(*ap, double);
	return self;
}

static var Real_Del(var _self)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = 0.0;
	return self;
}

static var Real_Copy(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, self->value);
}

/**********************************************************
 * Comparison
 **********************************************************/

static ssize_t Real_Cmp(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);

	if (Isinstance(Real.Class, _other)) {
		if (self->value < Float(_other)) {
			return -1;
		}
		else if (Float(_self) == Float(_other)) {
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

static bool Real_Eq(const var _self, const var _other)
{
	return Cmp(_self, _other) == 0;
}

static bool Real_Ne(const var _self, const var _other)
{
	return Cmp(_self, _other) != 0;
}

static bool Real_Lt(const var _self, const var _other)
{
	return Cmp(_self, _other) < 0;
}

static bool Real_Gt(const var _self, const var _other)
{
	return Cmp(_self, _other) > 0;
}

static bool Real_Le(const var _self, const var _other)
{
	return Cmp(_self, _other) <= 0;
}

static bool Real_Ge(const var _self, const var _other)
{
	return Cmp(_self, _other) >= 0;
}

/**********************************************************
 * Unary
 **********************************************************/

static var Real_Pos(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value < 0.0 ? +self->value : self->value));
}

static var Real_Neg(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value > 0.0 ? -self->value : self->value));
}

static var Real_Abs(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(fabs(self->value)));
}

static var Real_Round(const var _self, size_t n)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(round(self->value)));
}

static var Real_Floor(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(floor(self->value)));
}

static var Real_Ceil(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(ceil(self->value)));
}

static var Real_Trunc(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(trunc(self->value)));
}

/**********************************************************
 * Arithmetic
 **********************************************************/

static var Real_Add(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value + Float(_other)));
}

static var Real_Sub(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value - Float(_other)));
}

static var Real_Mul(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value * Float(_other)));
}

static var Real_Floordiv(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)floor(self->value / Float(_other)));
}

static var Real_Div(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)(self->value / Float(_other)));
}

static var Real_Mod(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)fmod(self->value, Float(_other)));
}

static var Real_Pow(const var _self, const var _other)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return New(Real.Class, (double)pow(self->value, Float(_other)));
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static var Real_Iadd(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value + Float(_other);
	return self;
}

static var Real_Isub(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value - Float(_other);
	return self;
}

static var Real_Imul(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value * Float(_other);
	return self;
}

static var Real_Ifloordiv(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = floor(self->value / Float(_other));
	return self;
}

static var Real_Idiv(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = self->value / Float(_other);
	return self;
}

static var Real_Imod(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = fmod(self->value, Float(_other));
	return self;
}

static var Real_Ipow(var _self, const var _other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value = pow(self->value, Float(_other));
	return self;
}

/**********************************************************
 * Representation
 **********************************************************/

static size_t Real_Hash(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return fnv1a(&self->value, sizeof(self->value));
}

static char *Real_Str(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	char *text = NULL;
	strcatf(&text, "%lf", self->value);
	assert(text);
	return text;
}

static char *Real_Repr(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	char *text = NULL;
	strcatf(&text, "\"<%s object at 0x%x>\"", Nameof(_self), (size_t)self);
}

static ssize_t Real_Int(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return (ssize_t)self->value;
}

static size_t Real_Uint(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return (size_t)self->value;
}

static double Real_Float(const var _self)
{
	const struct Real *self = _self;
	assert(self->class == Real.Class);
	return (double)self->value;
}

static bool Real_Bool(const var _self)
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

double NamespaceReal_Add(var _self, double other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value += other;
}

double NamespaceReal_Sub(var _self, double other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value -= other;
}

double NamespaceReal_Mul(var _self, double other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value *= other;
}

double NamespaceReal_Div(var _self, double other)
{
	struct Real *self = _self;
	assert(self->class == Real.Class);
	self->value /= other;
}