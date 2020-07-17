#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <float.h>
#include <limits.h>

#include "class.h"
#include "string.h"
#include "iterator.h"
#include "util.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static var           String_New           (var _self, va_list *ap);
static var           String_Del           (var _self);
static var           String_Copy          (const var _self);

// comparison
static ssize_t       String_Cmp           (const var _self, const var _other);
static bool          String_Eq            (const var _self, const var _other);
static bool          String_Ne            (const var _self, const var _other);

// unary

// arithmetic
static var           String_Add           (const var _self, const var _other);

// assignment arithmetic
static shared        String_Iadd          (var _self, const var _other);

// representation
static size_t        String_Hash          (const var _self);
static char*         String_Str           (const var _self);
static char*         String_Repr          (const var _self);
static ssize_t       String_Int           (const var _self);
static size_t        String_Uint          (const var _self);
static double        String_Float         (const var _self);
static bool          String_Bool          (const var _self);

// containers
static size_t        String_Len           (const var _self);
static shared        String_Next          (var _self, va_list *ap);
static var           String_Iter          (const var _self);
static bool          String_Contains      (const var _self, const var _other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static void          NamespaceString_Clear      (var _self);
static ptrdiff_t     NamespaceString_Find       (const var _self, const char *substr);
static var           NamespaceString_Substring  (const var _self, size_t start, size_t length);
static void          NamespaceString_Reserve    (var _self, size_t size);
static char*         NamespaceString_Cstr       (const var _self);
static void          NamespaceString_Catf       (var _self, const char *fmt, ...);
static void          NamespaceString_Ccatf      (var _self, const char *fmt, ...);
static void          NamespaceString_Replace    (var _self, const char *old, const char *new);
static var           NamespaceString_Fread      (const char *path);
static void          NamespaceString_Fwrite     (var _self, const char *path);
static void          NamespaceString_Fappend    (var _self, const char *path);
static void          NamespaceString_Fclear     (const char *path);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "String",
	.size      = sizeof(struct String),

	// construction
	.New       = String_New,
	.Del       = String_Del,
	.Copy      = String_Copy,

	// comparison
	.Cmp       = String_Cmp,
	.Eq        = String_Eq,
	.Ne        = String_Ne,
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
	.Add       = String_Add,
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
	.Iadd      = String_Iadd,
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
	.Hash      = String_Hash,
	.Str       = String_Str,
	.Repr      = String_Repr,
	.Int       = String_Int,
	.Uint      = String_Uint,
	.Float     = String_Float,
	.Bool      = String_Bool,

	// containers
	.Len       = String_Len,
	.Getitem   = NULL,
	.Setitem   = NULL,
	.Delitem   = NULL,
	.Next      = String_Next,
	.Iter      = String_Iter,
	.Reversed  = NULL,
	.Contains  = String_Contains,
};

struct NamespaceString String = {
	.Class     = &class,
	.Clear     = NamespaceString_Clear,
	.Cstr      = NamespaceString_Cstr,
	.Find      = NamespaceString_Find,
	.Substring = NamespaceString_Substring,
	.Reserve   = NamespaceString_Reserve,
	.Catf      = NamespaceString_Catf,
	.Ccatf     = NamespaceString_Ccatf,
	.Replace   = NamespaceString_Replace,
	.Fread     = NamespaceString_Fread,
	.Fwrite    = NamespaceString_Fwrite,
	.Fappend   = NamespaceString_Fappend,
	.Fclear    = NamespaceString_Fclear,
};

struct StringIterator {
	size_t index;
};

/**********************************************************
 * Construction
 **********************************************************/

static var String_New(var _self, va_list *ap)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	const char *text = va_arg(*ap, const char *);

	self->text = strdup(text);
	assert(self->text);
	self->size = strlen(self->text);

	return self;
}

static var String_Del(var _self)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	memset(self->text, 0, self->size);
	free(self->text);
	self->size = 0;
	self->text = NULL;
	return self;
}

static var String_Copy(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	return New(String.Class, self->text);
}

/**********************************************************
 * Comparison
 **********************************************************/

static ssize_t String_Cmp(const var _self, const var _other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);
	return (ssize_t)strcmp(self->text, other->text);
}

static bool String_Eq(const var _self, const var _other)
{
	return String_Cmp(_self, _other) == 0;
}

static bool String_Ne(const var _self, const var _other)
{
	return !String_Eq(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

/**********************************************************
 * Arithmetic
 **********************************************************/

static var String_Add(const var _self, const var _other)
{
	const struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);

	size_t count = self->size + other->size + 1;
	char *text = calloc(count, sizeof(char));
	assert(text);
	strcat(text, self->text);
	strcat(text, other->text);
	var new = New(String.Class, text);
	free(text);
	return new;
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static var String_Iadd(var _self, const var _other)
{
	struct String *self = _self;
	const struct String *other = _other;
	assert(self->class == String.Class);
	assert(other->class == String.Class);

	size_t count = self->size + other->size;
	char *tmp = realloc(self->text, count + 1);
	assert(tmp);
	self->text = tmp;
	strncat(self->text, other->text, count);
	self->text[count] = '\0';
	self->size = count;
	return (var)self;
}

/**********************************************************
 * Representation
 **********************************************************/

static size_t String_Hash(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	char *p = self->text;

	return fnv1a(p, self->size);
}

static char *String_Str(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	
	char *text = NULL;
	if (self->text == NULL) {
		text = strdup("\"\"");
		assert(text);
	}
	else {
		text = calloc(self->size + 3, sizeof(char));
		assert(text);
		snprintf(text, self->size + 3, "\"%s\"", self->text);
	}

	return text;
}

static char *String_Repr(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	char *text = NULL;
	strcatf(&text, "\"<%s object at 0x%x>\"", Nameof(_self), (size_t)self);
	assert(text);
	return text;
}

static ssize_t String_Int(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	ssize_t retval;
	if (sscanf(self->text, "%zd", &retval) != 1) {
		return SSIZE_MAX;
	}
	return retval;
}

static size_t String_Uint(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	ssize_t retval;
	if (sscanf(self->text, "%zu", &retval) != 1) {
		return SSIZE_MAX;
	}
	return retval;
}

static double String_Float(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	double retval;
	if (sscanf(self->text, "%lf", &retval) != 1) {
		return DBL_MAX;
	}
	return retval;
}

static bool String_Bool(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	if (self->size > 0) {
		return true;
	}
	return false;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t String_Len(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	return self->size;
}

static shared String_Next(var _self, va_list *ap)
{
	struct String *self = _self;
	assert(self->class == String.Class);

	struct StringIterator *state = va_arg(*ap, void *);

	if (self->size == 0 || state->index >= self->size) {
		state->index = 0;
		return NULL;
	}

	for (; state->index < self->size;) {
		return &self->text[state->index++];
	}

	state->index = 0;
	return NULL;
}

static var String_Iter(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	struct StringIterator state = {
		.index = 0,
	};
	var iterator = New(Iterator.Class, _self, &state, (size_t)sizeof(state));
	return iterator;
}

static bool String_Contains(const var _self, const var _other)
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

static void NamespaceString_Clear(var _self)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	free(self->text);
	self->text = strdup("");
}

static char *NamespaceString_Cstr(const var _self)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	return self->text;
}

static ptrdiff_t NamespaceString_Find(const var _self, const char *substr)
{
	const struct String *self = _self;
	assert(self && substr);
	assert(self->class == String.Class);
	return (ptrdiff_t)(strstr(self->text, substr) - self->text);
}

static var NamespaceString_Substring(const var _self, size_t start, size_t length)
{
	const struct String *self = _self;
	assert(self->class == String.Class);
	char *text = calloc(length, sizeof(char));
	assert(text);
	strncat(text, &self->text[start], length + 1);
	var new = New(String.Class, text);
	free(text);
	return new;
}

static void NamespaceString_Reserve(var _self, size_t size)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	assert(self->text);
	void *tmp = realloc(self->text, size);
	assert(tmp);
}

static void NamespaceString_Catf(var _self, const char *fmt, ...)
{
	struct String *self = _self;
	assert(self->class == String.Class);

	va_list ap;
	va_start(ap, fmt);
	char *tmp = Format_va(fmt, &ap);
	va_end(ap);

	strcatf(&self->text, "%s", tmp);
	free(tmp);
}

static void NamespaceString_Ccatf(var _self, const char *fmt, ...)
{
	struct String *self = _self;
	assert(self->class == String.Class);

	va_list ap, copy;
	va_start(ap, fmt);
	va_copy(copy, ap);
	(void)strcatf_va(&self->text, fmt, &ap, &copy);
	va_end(copy);
	va_end(ap);
}

static void NamespaceString_Replace(var _self, const char *old, const char *new)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	streplace(&self->text, old, new);
}

static var NamespaceString_Fread(const char *path)
{
	var _self = New(String.Class, "");
	struct String *self = _self;
	long len;
	char *buf;
	FILE *f = fopen(path, "r");
	assert(f);

	fseek(f, 0, SEEK_END);
	len = ftell(f);
	fseek(f, 0, SEEK_SET);
	free(self->text);
	self->text = malloc((len + 1) * sizeof(char));
	assert(self->text);
	self->size = fread(self->text, 1, len, f);
	self->text[self->size] = '\0';
	fclose(f);
	return self;
}

static void NamespaceString_Fwrite(var _self, const char *path)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	FILE *f = fopen(path, "w");
	assert(f);
	fputs(self->text, f);
	fclose(f);
}

static void NamespaceString_Fappend(var _self, const char *path)
{
	struct String *self = _self;
	assert(self->class == String.Class);
	FILE *f = fopen(path, "a");
	assert(f);
	fputs(self->text, f);
	fclose(f);
}

static void NamespaceString_Fclear(const char *path)
{
	FILE *f = fopen(path, "w");
	assert(f);
	fputs("", f);
	fclose(f);
}
