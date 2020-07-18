#include <assert.h>
#include <stdint.h>
#include <string.h>

#include <ooc/class.h>
#include <ooc/iterator.h>
#include <ooc/string.h>
#include <ooc/util.h>
#include <ooc/vector.h>

#define VECTOR_DEFAULT_CAP 8
#define VECTOR_DEFAULT_SCALING 2

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static var           Vector_New           (var _self, va_list *ap);
static var           Vector_Del           (var _self);
static var           Vector_Copy          (const var _self);

// comparison
static bool          Vector_Eq            (const var _self, const var _other);
static bool          Vector_Ne            (const var _self, const var _other);

// unary
// arithmetic
// assignment arithmetic

// representation
static char*         Vector_Str           (const var _self);
static char*         Vector_Repr          (const var _self);
static bool          Vector_Bool          (const var _self);

// containers
static size_t        Vector_Len           (const var _self);
static shared        Vector_Getitem       (const var _self, const var _key);
static void          Vector_Setitem       (var _self, const var _key, const var _value);
static void          Vector_Delitem       (var _self, const var _key);
static shared        Vector_Next          (var _self, va_list *ap);
static var           Vector_Iter          (const var _self);
static bool          Vector_Contains      (const var _self, const var _other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static void          NamespaceVector_Clear        (var _self);
static void          NamespaceVector_Reserve      (var _self, size_t cap);
static void          NamespaceVector_Shrink_to_fit(var _self);
static void          NamespaceVector_Push_back    (var _self, var _value);
static void          NamespaceVector_Pop_back     (var _self);
static size_t        NamespaceVector_Find         (const var _self, const var _value);
static void          NamespaceVector_Emplace_back (var _self, const void *_class, ...);
static void          NamespaceVector_Take_back    (var _self, var _value);
static void          NamespaceVector_Initializer  (var _self, ...);
static var           NamespaceVector_Strsplit     (var _string, const char *fmt);
static shared        NamespaceVector_First        (var _self);
static shared        NamespaceVector_Last         (var _self);
static shared        NamespaceVector_GetbyInt     (var _self, size_t idx);
static shared        NamespaceVector_SetbyInt     (var _self, size_t idx, var _value);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "Vector",
	.size      = sizeof(struct Vector),

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
	.Repr      = Vector_Repr,
	.Int       = NULL,
	.Uint      = NULL,
	.Float     = NULL,
	.Bool      = Vector_Bool,

	// containers
	.Len       = Vector_Len,
	.Getitem   = Vector_Getitem,
	.Setitem   = Vector_Setitem,
	.Delitem   = Vector_Delitem,
	.Next      = Vector_Next,
	.Iter      = Vector_Iter,
	.Reversed  = NULL,
	.Contains  = Vector_Contains,
};

struct NamespaceVector Vector = {
	.Class         = &class,
	.Clear         = NamespaceVector_Clear,
	.Shrink_to_fit = NamespaceVector_Shrink_to_fit,
	.Push_back     = NamespaceVector_Push_back,
	.Pop_back      = NamespaceVector_Pop_back,
	.Find          = NamespaceVector_Find,
	.Emplace_back  = NamespaceVector_Emplace_back,
	.Take_back     = NamespaceVector_Take_back,
	.Initializer   = NamespaceVector_Initializer,
	.Strsplit      = NamespaceVector_Strsplit,
	.First         = NamespaceVector_First,
	.Last          = NamespaceVector_Last,
	.GetbyInt      = NamespaceVector_GetbyInt,
	.SetbyInt      = NamespaceVector_SetbyInt,
};

struct VectorIterator {
	size_t index;
};

/**********************************************************
 * Construction
 **********************************************************/

static var Vector_New(var _self, va_list *ap)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	self->buf = calloc(VECTOR_DEFAULT_CAP, sizeof(var));
	assert(self->buf);

	self->size = 0;
	self->cap = VECTOR_DEFAULT_CAP;

	return self;
}

static var Vector_Del(var _self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	for (size_t i = 0; i < self->cap; i++) {
		if (self->buf[i] != NULL) {
			Del(self->buf[i]);
			self->buf[i] = NULL;
		}
	}

	free(self->buf);
	self->buf = NULL;
	self->size = 0;
	self->cap = 0;

	return self;
}

static var Vector_Copy(const var _self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	var _new = New(Vector.Class);
	struct Vector *new = _new;

	for (size_t i = 0; i < self->size; i++) {
		Vector.Push_back(new, self->buf[i]);
	}

	return new;
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool Vector_Eq(const var _self, const var _other)
{
	const struct Vector *self = _self;
	const struct Vector *other = _other;
	size_t i;
	assert(self->class == Vector.Class);
	assert(other->class == Vector.Class);

	if (self->size != other->size || self->cap != other->cap) {
		return false;
	}

	for (i = 0; i < self->size && i < other->size; i++) {
		if (Ne(self->buf[i], other->buf[i])) {
			return false;
		}
	}
	return true;
}

static bool Vector_Ne(const var _self, const var _other)
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

static char *Vector_Str(const var _self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t i;
	char *value;
	char *text;

	if (self->size == 0) {
		text = strdup("[]");
		assert(text);
		return text;
	}

	text = strdup("[");
	assert(text);
	for (i = 0; i < self->size; i++) {
		value = Str(self->buf[i]);
		strcatf(&text, "%s, ", value);
		free(value);
	}
	text[strlen(text) - 2] = ']';
	text[strlen(text) - 1] = '\0';
	return text;
}

static char *Vector_Repr(const var _self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	char *text = NULL;
	strcatf(&text, "\"<%s object at 0x%x>\"", Nameof(_self), (size_t)self);
	assert(text);
	return text;
}

static bool Vector_Bool(const var _self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->size > 0;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t Vector_Len(const var _self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	return self->size;
}

static var Vector_Getitem(const var _self, const var _key)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t idx = Uint(_key);
	if (idx > self->size) {
		idx = idx % self->size;
	}
	return self->buf[idx];
}

static void Vector_Setitem(var _self, const var _key, const var _value)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t idx = Uint(_key);
	if (idx > self->size) {
		idx = idx % self->size;
	}
	if (self->buf[idx] != NULL) {
		Del(self->buf[idx]);
	}
	self->buf[idx] = Copy(_value);
}

static void Vector_Delitem(var _self, const var _key)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t idx = Uint(_key);
	if (idx > self->size) {
		idx = idx % self->size;
	}
	Del(self->buf[idx]);

	// shift buffer
	for (size_t i = idx; (size_t)self->buf[i] < self->size; i++) {
		self->buf[i] = self->buf[i + 1];
	}
	self->size--;
}

static shared Vector_Next(var _self, va_list *ap)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	struct VectorIterator *state = va_arg(*ap, void *);

	if (self->size == 0 || state->index >= self->size) {
		state->index = 0;
		return NULL;
	}

	for (; state->index < self->size;) {
		return self->buf[state->index++];
	}

	state->index = 0;
	return NULL;
}

static var Vector_Iter(const var _self)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);
	struct VectorIterator state = {
		.index = 0,
	};
	var iterator = New(Iterator.Class, _self, &state, (size_t)sizeof(state));
	return iterator;
}

static bool Vector_Contains(const var _self, const var _other)
{
	const struct Vector *self = _self;
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

static void NamespaceVector_Clear(var _self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	size_t i;
	for (i = 0; i < self->size; i++) {
		if (self->buf[i]) {
			Del(self->buf[i]);
			self->buf[i] = NULL;
		}
	}
	self->size = 0;
}

static void NamespaceVector_Reserve(var _self, size_t cap)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	var tmp;
	size_t i;

	/* del items that will be truncated off if shrinking */
	if (cap < self->size) {
		for (i = cap; i < self->size; i++) {
			if (self->buf[i]) {
				Del(self->buf[i]);
				self->buf[i] = NULL;
			}
		}
		self->size = cap;
	}

	/* more space is being allocated */
	tmp = realloc(self->buf, cap * sizeof(var));
	assert(tmp);
	self->buf = tmp;
	self->cap = cap;

	for (i = self->size; i < self->cap; i++) {
		self->buf[i] = NULL;
	}
}

static void NamespaceVector_Shrink_to_fit(var _self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	NamespaceVector_Reserve(self, self->size);
}

static void NamespaceVector_Push_back(var _self, var _value)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	if (self->size + 1 >= self->cap) {
		NamespaceVector_Reserve(self, self->cap * VECTOR_DEFAULT_SCALING);
	}
	self->buf[self->size++] = _value;
}

static void NamespaceVector_Pop_back(var _self)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);

	if (self->buf[self->size - 1]) {
		Del(self->buf[self->size - 1]);
	}
	self->size--;
}

static size_t NamespaceVector_Find(const var _self, const var _value)
{
	const struct Vector *self = _self;
	assert(self->class == Vector.Class);

	size_t i;
	for (i = 0; i < self->size; i++) {
		if (Cmp(self->buf[i], _value) == 0) {
			return i;
		}
	}
	return SIZE_MAX;
}

static void NamespaceVector_Emplace_back(var _self, const void *_class, ...)
{
	struct Vector *self = _self;
	va_list ap;
	var new;
	assert(self->class == Vector.Class);

	if (self->size + 1 >= self->cap) {
		NamespaceVector_Reserve(self, self->cap * VECTOR_DEFAULT_SCALING);
	}
	va_start(ap, _class);
	new = Vnew(_class, &ap);
	va_end(ap);
	self->buf[self->size++] = new;
}

static void NamespaceVector_Take_back(var _self, var _value)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	if (self->size + 1 >= self->cap) {
		NamespaceVector_Reserve(self, self->cap * VECTOR_DEFAULT_SCALING);
	}

	self->buf[self->size++] = _value;
}

static void NamespaceVector_Initializer(var _self, ...)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	va_list ap;
	var p = NULL;

	va_start(ap, _self);

	for (;;) {
		p = va_arg(ap, var);
		if (p == NULL) {
			break;
		}
		NamespaceVector_Take_back(_self, p);
	}

	va_end(ap);
}

static var NamespaceVector_Strsplit(var _string, const char *fmt)
{
	var _self = New(Vector.Class);
	struct Vector *self = _self;
	struct String *string = _string;
	assert(string->class == String.Class);
	char **buf = strsplit(string->text, fmt);
	size_t i;

	for (i = 0; buf[i]; i++) {
		NamespaceVector_Emplace_back(self, String.Class, buf[i]);
	}
	strsplit_free(buf);
	return self;
}

static shared NamespaceVector_First(var _self)
{
	struct Vector *self = _self;
	if (self->size == 0) {
		return NULL;
	}
	return self->buf[0];
}

static shared NamespaceVector_Last(var _self)
{
	struct Vector *self = _self;
	if (self->size == 0) {
		return NULL;
	}
	return self->buf[self->size - 1];
}

static shared NamespaceVector_GetbyInt(var _self, size_t idx)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	if (idx > self->size) {
		idx = idx % self->size;
	}
	return self->buf[idx];
}

static shared NamespaceVector_SetbyInt(var _self, size_t idx, var _value)
{
	struct Vector *self = _self;
	assert(self->class == Vector.Class);
	if (idx > self->size) {
		idx = idx % self->size;
	}
	if (self->buf[idx] != NULL) {
		Del(self->buf[idx]);
	}
	self->buf[idx] = Copy(_value);
}