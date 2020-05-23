#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

#include "class.h"
#include "string.h"
#include "util.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void*         String_New           (void *_self, va_list *ap);
static void*         String_Del           (void *_self);
static void*         String_Copy          (const void *_self);

// comparison
static ssize_t       String_Cmp           (const void *_self, const void *_other);
static bool          String_Eq            (const void *_self, const void *_other);
static bool          String_Ne            (const void *_self, const void *_other);

// unary

// arithmetic
static void*         String_Add           (const void *_self, const void *_other);

// assignment arithmetic
static void*         String_Iadd          (void *_self, const void *_other);

// representation
static size_t        String_Hash          (const void *_self);
static char*         String_Str           (const void *_self);
static ssize_t       String_Int           (const void *_self);
static size_t        String_Uint          (const void *_self);
static double        String_Float         (const void *_self);
static bool          String_Bool          (const void *_self);

// containers
static size_t        String_Len           (const void *_self);
static bool          String_Contains      (const void *_self, const void *_other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static ptrdiff_t     NamespaceString_Find       (const void *_self, const char *substr);
static void*         NamespaceString_Substring  (const void *_self, size_t start, size_t length);
static char*         NamespaceString_Cstr       (const void *_self);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size  = sizeof(struct String),
	.class = &class,
	.super = NULL,
	.name  = "String",

	// construction
	.New = String_New,
	.Del = String_Del,
	.Copy = String_Copy,

	// comparison
	.Cmp = String_Cmp,
	.Eq  = String_Eq,
	.Ne  = String_Ne,
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
	.Add = String_Add,
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
	.Iadd = String_Iadd,
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
	.Hash = String_Hash,
	.Str = String_Str,
	.Repr = NULL,
	.Int = String_Int,
	.Uint = String_Uint,
	.Float = String_Float,
	.Bool = String_Bool,

	// containers
	.Len = String_Len,
	.Getitem = NULL,
	.Setitem = NULL,
	.Delitem = NULL,
	.Iter = NULL,
	.Reversed = NULL,
	.Contains = String_Contains,
};

struct NamespaceString String = {
	.Class = &class,
	.Cstr = NamespaceString_Cstr,
	.Find = NamespaceString_Find,
	.Substring = NamespaceString_Substring,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *String_New(void *_self, va_list *ap)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	const char *text = va_arg(*ap, const char *);

	self->text = calloc(strlen(text) + 1, sizeof(char));
	assert(self->text);

	strcpy(self->text, text);
	return self;
}

static void *String_Del(void *_self)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	free(self->text);
	self->text = NULL;
	return self;
}

static void *String_Copy(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	return New(String.Class, self->text);
}

/**********************************************************
 * Comparison
 **********************************************************/

static ssize_t String_Cmp(const void *_self, const void *_other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);
	return (ssize_t)strcmp(self->text, other->text);
}

static bool String_Eq(const void *_self, const void *_other)
{
	return String_Cmp(_self, _other) == 0;
}

static bool String_Ne(const void *_self, const void *_other)
{
	return !String_Eq(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

/**********************************************************
 * Arithmetic
 **********************************************************/

static void *String_Add(const void *_self, const void *_other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);

	size_t count = strlen(self->text) + strlen(other->text) + 1;
	char *text = calloc(count, sizeof(char));
	assert(text);
	strcat(text, self->text);
	strcat(text, other->text);
	void *new = New(String.Class, text);
	free(text);
	return new;
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static void *String_Iadd(void *_self, const void *_other)
{
	struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);

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

static size_t String_Hash(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	char *p = self->text;

	return fnv1a(p, strlen(p));
}

static char *String_Str(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	size_t len;
	char *text = NULL;
	if (self->text == NULL) {
		text = strdup("\"\"");
		assert(text);
	}
	else {
		len = strlen(self->text);
		text = calloc(len + 2, sizeof(char));
		assert(text);
		text[0] = '"';
		sprintf(&text[1], "%s", self->text);
		text[len + 1] = '"';
		text[len + 2] = '\0';
	}

	return text;
}

static ssize_t String_Int(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	ssize_t retval;
	if (sscanf(self->text, "%zd", &retval) != 1) {
		return SSIZE_MAX;
	}
	return retval;
}

static size_t String_Uint(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	ssize_t retval;
	if (sscanf(self->text, "%zu", &retval) != 1) {
		return SSIZE_MAX;
	}
	return retval;
}

static double String_Float(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	double retval;
	if (sscanf(self->text, "%lf", &retval) != 1) {
		return DBL_MAX;
	}
	return retval;
}

static bool String_Bool(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	if (strlen(self->text) > 0) {
		return true;
	}
	return false;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t String_Len(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	return strlen(self->text);
}

static bool String_Contains(const void *_self, const void *_other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);

	if (strstr(self->text, other->text) != NULL) {
		return true;
	}
	return false;
}

/**********************************************************
 * Namespace Functions
 **********************************************************/

static char *NamespaceString_Cstr(const void *_self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	return self->text;
}

static ptrdiff_t NamespaceString_Find(const void *_self, const char *substr)
{
	const struct String *self = _self;
	assert(self && substr);
	assert(self->class == String.Class);
	return (ptrdiff_t)(strstr(self->text, substr) - self->text);
}

static void *NamespaceString_Substring(const void *_self, size_t start, size_t length)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	char *text = calloc(length, sizeof(char));
	assert(text);
	strncat(text, &self->text[start], length + 1);
	void *new = New(String.Class, text);
	free(text);
	return new;
}
