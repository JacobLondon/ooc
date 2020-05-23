#include <math.h>
#include <stdarg.h>
#include "class.h"
#include "integer.h"
#include "boolean.h"
#include "util.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void*         Integer_New           (void *_self, va_list *ap);
static void*         Integer_Del           (void *_self);
static void*         Integer_Copy          (const void *_self);

// comparison
static ssize_t       Integer_Cmp           (const void *_self, const void *_other);
static bool          Integer_Eq            (const void *_self, const void *_other);
static bool          Integer_Ne            (const void *_self, const void *_other);
static bool          Integer_Lt            (const void *_self, const void *_other);
static bool          Integer_Gt            (const void *_self, const void *_other);
static bool          Integer_Le            (const void *_self, const void *_other);
static bool          Integer_Ge            (const void *_self, const void *_other);

// unary
static void*         Integer_Pos           (const void *_self);
static void*         Integer_Neg           (const void *_self);
static void*         Integer_Abs           (const void *_self);
static void*         Integer_Invert        (const void *_self);

// arithmetic
static void*         Integer_Add           (const void *_self, const void *_other);
static void*         Integer_Sub           (const void *_self, const void *_other);
static void*         Integer_Mul           (const void *_self, const void *_other);
static void*         Integer_Div           (const void *_self, const void *_other);
static void*         Integer_Mod           (const void *_self, const void *_other);
static void*         Integer_Pow           (const void *_self, const void *_other);
static void*         Integer_Lshift        (const void *_self, const void *_other);
static void*         Integer_Rshift        (const void *_self, const void *_other);
static void*         Integer_And           (const void *_self, const void *_other);
static void*         Integer_Or            (const void *_self, const void *_other);
static void*         Integer_Xor           (const void *_self, const void *_other);

// assignment arithmetic
static void*         Integer_Iadd          (void *_self, const void *_other);
static void*         Integer_Isub          (void *_self, const void *_other);
static void*         Integer_Imul          (void *_self, const void *_other);
static void*         Integer_Idiv          (void *_self, const void *_other);
static void*         Integer_Imod          (void *_self, const void *_other);
static void*         Integer_Ipow          (void *_self, const void *_other);
static void*         Integer_Ilshift       (void *_self, const void *_other);
static void*         Integer_Irshift       (void *_self, const void *_other);
static void*         Integer_Iand          (void *_self, const void *_other);
static void*         Integer_Ior           (void *_self, const void *_other);
static void*         Integer_Ixor          (void *_self, const void *_other);

// representation
static size_t        Integer_Hash          (const void *_self);
static char*         Integer_Str           (const void *_self);
static char*         Integer_Repr          (const void *_self);
static ssize_t       Integer_Int           (const void *_self);
static size_t        Integer_Uint          (const void *_self);
static double        Integer_Float         (const void *_self);
static bool          Integer_Bool          (const void *_self);

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
	.name      = "Integer",

	// construction
	.New       = Integer_New,
	.Del       = Integer_Del,
	.Copy      = Integer_Copy,

	// comparison
	.Cmp       = Integer_Cmp,
	.Eq        = Integer_Eq,
	.Ne        = Integer_Ne,
	.Lt        = Integer_Lt,
	.Gt        = Integer_Gt,
	.Le        = Integer_Le,
	.Ge        = Integer_Ge,

	// unary
	.Pos       = Integer_Pos,
	.Neg       = Integer_Neg,
	.Abs       = Integer_Abs,
	.Invert    = Integer_Invert,
	.Round     = NULL,
	.Floor     = NULL,
	.Ceil      = NULL,
	.Trunc     = NULL,

	// arithmetic
	.Add       = Integer_Add,
	.Sub       = Integer_Sub,
	.Mul       = Integer_Mul,
	.Floordiv  = NULL,
	.Div       = Integer_Div,
	.Mod       = Integer_Mod,
	.Pow       = Integer_Pow,
	.Lshift    = Integer_Lshift,
	.Rshift    = Integer_Rshift,
	.And       = Integer_And,
	.Or        = Integer_Or,
	.Xor       = Integer_Xor,

	// assignment arithmetic
	.Iadd      = Integer_Iadd,
	.Isub      = Integer_Isub,
	.Imul      = Integer_Imul,
	.Ifloordiv = NULL,
	.Idiv      = Integer_Idiv,
	.Imod      = Integer_Imod,
	.Ipow      = Integer_Ipow,
	.Ilshift   = Integer_Ilshift,
	.Irshift   = Integer_Irshift,
	.Iand      = Integer_Iand,
	.Ior       = Integer_Ior,
	.Ixor      = Integer_Ixor,

	// representation
	.Hash      = Integer_Hash,
	.Str       = Integer_Str,
	.Repr      = Integer_Repr,
	.Int       = Integer_Int,
	.Uint      = Integer_Uint,
	.Float     = Integer_Float,
	.Bool      = Integer_Bool,

	// containers
	.Len       = NULL,
	.Getitem   = NULL,
	.Setitem   = NULL,
	.Delitem   = NULL,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = NULL,
};

struct NamespaceInteger Integer = {
	.Class = &class,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *Integer_New(void *_self, va_list *ap)
{
	struct Integer *self = _self;
	assert(self->class == Integer.Class);
	self->value = va_arg(*ap, ssize_t);
	return self;
}

static void *Integer_Del(void *_self)
{
	struct Integer *self = _self;
	assert(self->class == Integer.Class);
	self->value = 0;
	return self;
}

static void *Integer_Copy(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return New(Integer.Class, self->value);
}

/**********************************************************
 * Comparison
 **********************************************************/

static ssize_t Integer_Cmp(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		if (self->value < Float(other)) {
			return -1;
		}
		else if (Float(self) == Float(other)) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		if (self->value < Int(other)) {
			return -1;
		}
		else if (self->value == Int(other)) {
			return 0;
		}
		else {
			return 1;
		}
	}

	ClassError(self, other);
	return 0;
}

static bool Integer_Eq(const void *_self, const void *_other)
{
	return Cmp(_self, _other) == 0;
}

static bool Integer_Ne(const void *_self, const void *_other)
{
	return Cmp(_self, _other) != 0;
}

static bool Integer_Lt(const void *_self, const void *_other)
{
	return Cmp(_self, _other) < 0;
}

static bool Integer_Gt(const void *_self, const void *_other)
{
	return Cmp(_self, _other) > 0;
}

static bool Integer_Le(const void *_self, const void *_other)
{
	return Cmp(_self, _other) <= 0;
}

static bool Integer_Ge(const void *_self, const void *_other)
{
	return Cmp(_self, _other) >= 0;
}

/**********************************************************
 * Unary
 **********************************************************/

static void *Integer_Pos(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return New(Integer.Class, (ssize_t)(+self->value));
}

static void *Integer_Neg(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return New(Integer.Class, (ssize_t)(-self->value));
}

static void *Integer_Abs(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return New(Integer.Class, (ssize_t)abs(self->value));
}

static void *Integer_Invert(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return New(Integer.Class, (ssize_t)(~self->value));
}

/**********************************************************
 * Arithmetic
 **********************************************************/

static void *Integer_Add(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		return New(Real.Class, (double)(self->value + Float(other)));
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value + Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Sub(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		return New(Real.Class, (double)(self->value - Float(other)));
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value - Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Mul(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		return New(Real.Class, (double)(self->value * Float(other)));
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value * Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Div(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		return New(Real.Class, (double)(self->value / Float(other)));
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value / Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Mod(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		return New(Real.Class, (double)fmod(self->value, Float(other)));
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value % Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Pow(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		return New(Real.Class, (double)pow(self->value, Float(other)));
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)pow(self->value, Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Lshift(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value << Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Rshift(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value >> Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_And(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value & Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Or(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value | Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Xor(const void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		return New(Integer.Class, (ssize_t)(self->value ^ Int(other)));
	}

	ClassError(self, other);
	return NULL;
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static void *Integer_Iadd(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		self->value = (double)(self->value + Float(other));
		return self;
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value + Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Isub(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		self->value = (double)(self->value - Float(other));
		return self;
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value - Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Imul(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		self->value = (double)(self->value * Float(other));
		return self;
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value * Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Idiv(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		self->value = (double)(self->value / Float(other));
		return self;
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value / Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Imod(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		self->value = (double)fmod(self->value, Float(other));
		return self;
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value % Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Ipow(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);

	if (other->class == Real.Class) {
		self->value = (double)pow(self->value, Float(other));
		return self;
	}
	else if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)pow(self->value, Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Ilshift(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value << Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Irshift(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value >> Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Iand(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value & Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Ior(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	         other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value | Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

static void *Integer_Ixor(void *_self, const void *_other)
{
	const struct Integer *self = _self;
	const struct Integer *other = _other;
	assert(self->class == Integer.Class);
	
	if (other->class == Integer.Class ||
	    other->class == Boolean.Class)
	{
		self->value = (ssize_t)(self->value ^ Int(other));
		return self;
	}

	ClassError(self, other);
	return NULL;
}

/**********************************************************
 * Representation
 **********************************************************/

static size_t Integer_Hash(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return fnv1a(&self->value, sizeof(ssize_t));
}

static char *Integer_Str(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	char *text = NULL;
	strcatf(&text, "%zd", self->value);
	return text;
}

static char *Integer_Repr(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	char *text = NULL;
	strcatf(&text, "'<%s object at 0x%x>'", ((struct Class *)(self))->name, (size_t)self);
	return text;
}

static ssize_t Integer_Int(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return (ssize_t)self->value;
}

static size_t Integer_Uint(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return (size_t)self->value;
}

static double Integer_Float(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return (double)self->value;
}

static bool Integer_Bool(const void *_self)
{
	const struct Integer *self = _self;
	assert(self->class == Integer.Class);
	return (bool)!!self->value;
}

/**********************************************************
 * Containers
 **********************************************************/

/**********************************************************
 * Namespace Functions
 **********************************************************/
