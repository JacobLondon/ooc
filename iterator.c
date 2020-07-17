#include <assert.h>
#include <memory.h>
#include "iterator.h"
#include "util.h"
#include "class.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static var           Iterator_New           (var _self, va_list *ap);
static var           Iterator_Del           (var _self);
static var           Iterator_Copy          (const var _self);

// comparison

// unary

// arithmetic

// assignment arithmetic

// representation
static char*         Iterator_Str           (const var _self);
static char*         Iterator_Repr          (const var _self);

// containers
static shared        Iterator_Next          (var _self, va_list *ap);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "Iterator",
	.size      = sizeof(struct Iterator),

	// construction
	.New       = Iterator_New,
	.Del       = Iterator_Del,
	.Copy      = Iterator_Copy,

	// comparison
	.Cmp       = NULL,
	.Eq        = NULL,
	.Ne        = NULL,
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
	.Str       = Iterator_Str,
	.Repr      = Iterator_Repr,
	.Int       = NULL,
	.Uint      = NULL,
	.Float     = NULL,
	.Bool      = NULL,

	// containers
	.Len       = NULL,
	.Getitem   = NULL,
	.Setitem   = NULL,
	.Delitem   = NULL,
	.Next      = Iterator_Next,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = NULL,
};

struct NamespaceIterator Iterator = {
	.Class = &class,
};

/**********************************************************
 * Construction
 **********************************************************/

/**
 * object pointer, state pointer, state size,
 */
static var Iterator_New(var _self, va_list *ap)
{
	struct Iterator *self = _self;
	assert(self->class == Iterator.Class);
	
	self->object = va_arg(*ap, void *);
	void *state = va_arg(*ap, void *);
	self->size = va_arg(*ap, size_t);

	self->state = calloc(1, self->size);
	assert(self->state);
	memcpy(self->state, state, self->size);

	return self;
}

static var Iterator_Del(var _self)
{
	struct Iterator *self = _self;
	assert(self->class == Iterator.Class);

	free(self->state);
	return self;
}

static var Iterator_Copy(const var _self)
{
	struct Iterator *self = _self;
	assert(self->class == Iterator.Class);

	var _new = New(Iterator.Class, self->state, self->size);
	struct Iterator *new = _new;
	return new;
}

/**********************************************************
 * Comparison
 **********************************************************/

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

static char *Iterator_Str(const var _self)
{
	return Iterator_Repr(_self);
}

static char *Iterator_Repr(const var _self)
{
	struct Iterator *self = _self;
	assert(self->class == Iterator.Class);
	char *text = NULL;
	strcatf(&text, "\"<%s object at 0x%x>\"", Nameof(_self), (size_t)self);
	return text;
}

/**********************************************************
 * Containers
 **********************************************************/

static shared Iterator_Next(var _self, va_list *ap)
{
	// assume no va arg, Iterator does not implement Iter
	struct Iterator *self = _self;
	assert(self->class == Iterator.Class);
	return Next(self->object, self->state);
}

/**********************************************************
 * Namespace Functions
 **********************************************************/


