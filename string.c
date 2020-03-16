#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

#include "class.h"
#include "string.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void *String_new(void *_self, va_list *ap);
static void *String_del(void *_self);
static void *String_copy(const void *_self);

// comparison
static ssize_t String_cmp(const void *_self, const void *_other);
static bool String_eq(const void *_self, const void *_other);
static bool String_ne(const void *_self, const void *_other);

// unary

// arithmetic
static void *String_add(const void *_self, const void *_other);

// assignment arithmetic
static void *String_iadd(void *_self, const void *_other);

// representation
static size_t String_hash(const void *_self);
static char *String_str(const void *_self);
static ssize_t String_int(const void *_self);
static size_t String_uint(const void *_self);
static double String_float(const void *_self);
static bool String_bool(const void *_self);

// containers
static size_t String_len(const void *_self);
static bool String_contains(const void *_self, const void *_other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static ptrdiff_t NamespaceString_find(const void *_self, const char *substr);
static void *NamespaceString_substring(const void *_self, size_t start, size_t length);
static char *NamespaceString_cstr(const void *_self);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size = sizeof(struct String),
	.class = &class,
	.super = NULL,

	// construction
	.New = String_new,
	.Del = String_del,
	.Copy = String_copy,

	// comparison
	.Cmp = String_cmp,
	.Eq  = String_eq,
	.Ne  = String_ne,
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
	.Add = String_add,
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
	.Iadd = String_iadd,
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
	.Hash = String_hash,
	.Str = String_str,
	.Repr = NULL,
	.Int = String_int,
	.Uint = String_uint,
	.Float = String_float,
	.Bool = String_bool,

	// containers
	.Len = String_len,
	.Getitem = NULL,
	.Setitem = NULL,
	.Delitem = NULL,
	.Iter = NULL,
	.Reversed = NULL,
	.Contains = String_contains,
};

struct NamespaceString String = {
	.class = &class,
	.cstr = NamespaceString_cstr,
	.find = NamespaceString_find,
	.substring = NamespaceString_substring,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *String_new(void *_self, va_list *ap)
{
	struct String *self = _self;
	assert(self->class == String.class);
	const char *text = va_arg(*ap, const char *);

	self->text = calloc(strlen(text) + 1, sizeof(char));
	assert(self->text);

	strcpy(self->text, text);
	return self;
}

static void *String_del(void *_self)
{
	struct String *self = _self;
	assert(self->class == String.class);
	free(self->text);
	self->text = NULL;
	return self;
}

static void *String_copy(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	return New(String.class, self->text);
}

/**********************************************************
 * Comparison
 **********************************************************/

static ssize_t String_cmp(const void *_self, const void *_other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.class);
	assert(other->class == String.class);
	return (ssize_t)strcmp(self->text, other->text);
}

static bool String_eq(const void *_self, const void *_other)
{
	return String_cmp(_self, _other) == 0;
}

static bool String_ne(const void *_self, const void *_other)
{
	return !String_eq(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

/**********************************************************
 * Arithmetic
 **********************************************************/

static void *String_add(const void *_self, const void *_other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.class);
	assert(other->class == String.class);

	size_t count = strlen(self->text) + strlen(other->text) + 1;
	char *text = calloc(count, sizeof(char));
	assert(text);
	strcat(text, self->text);
	strcat(text, other->text);
	void *new = New(String.class, text);
	free(text);
	return new;
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static void *String_iadd(void *_self, const void *_other)
{
	struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.class);
	assert(other->class == String.class);

	size_t count = strlen(self->text) + strlen(other->text);
	char *tmp = realloc(self->text, count + 1);
	assert(tmp);
	self->text = tmp;
	strncat(self->text, other->text, count);
	self->text[count] = '\0';
	return (void *)self;
}

/**********************************************************
 * Representation
 **********************************************************/

static size_t String_hash(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	char *p = self->text;

	#define FNV1A_PRIME 0x01000193
	#define FNV1A_SEED  0x811C9DC5

	size_t hash = FNV1A_SEED;
	while (*p) {
		hash = (*p++ ^ hash) * FNV1A_PRIME;
	}
	return hash;

	#undef FNV1A_PRIME
	#undef FNV1A_SEED
}

static char *String_str(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	char *text = strdup(self->text);
	assert(text);
	return text;
}

static ssize_t String_int(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	ssize_t retval;
	if (sscanf(self->text, "%zd", &retval) != 1) {
		return SSIZE_MAX;
	}
	return retval;
}

static size_t String_uint(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	ssize_t retval;
	if (sscanf(self->text, "%zu", &retval) != 1) {
		return SIZE_MAX;
	}
	return retval;
}

static double String_float(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	double retval;
	if (sscanf(self->text, "%lf", &retval) != 1) {
		return DBL_MAX;
	}
	return retval;
}

static bool String_bool(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	if (strlen(self->text) > 0) {
		return true;
	}
	return false;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t String_len(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	return strlen(self->text);
}

static bool String_contains(const void *_self, const void *_other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.class);
	assert(other->class == String.class);

	if (strstr(self->text, other->text) != NULL) {
		return true;
	}
	return false;
}

/**********************************************************
 * Namespace Functions
 **********************************************************/

static char *NamespaceString_cstr(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	return self->text;
}

static ptrdiff_t NamespaceString_find(const void *_self, const char *substr)
{
	const struct String *self = _self;
	assert(self && substr);
	assert(self->class == String.class);
	return (ptrdiff_t)(strstr(self->text, substr) - self->text);
}

static void *NamespaceString_substring(const void *_self, size_t start, size_t length)
{
	const struct String *self = _self;
	assert(self->class == String.class);
	char *text = calloc(length, sizeof(char));
	assert(text);
	strncat(text, &self->text[start], length + 1);
	void *new = New(String.class, text);
	free(text);
	return new;
}

