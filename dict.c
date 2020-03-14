#include <assert.h>
#include <stdarg.h>

#include "class.h"
#include "dict.h"

#define DICT_DEFAULT_CAP 8
#define DICT_DEFAULT_SCALING 2
#define DICT_COLLISION_BIAS 10
#define DICT_FULL_RATIO (2.0 / 3.0)

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static void *Dict_new(void *_self, va_list *ap);
static void *Dict_del(void *_self);
static void *Dict_copy(const void *_self);

// comparison
static bool Dict_eq(const void *_self, const void *_other);
static bool Dict_ne(const void *_self, const void *_other);

// unary

// arithmetic

// assignment arithmetic

// representation
static char *Dict_str(const void *_self);
static bool Dict_bool(const void *_self);

// containers
static size_t Dict_len(const void *_self);
void *Dict_getitem(const void *_self, const void *_key);
void Dict_setitem(const void *_self, const void *_key, const void *_value);
void Dict_delitem(const void *_self, const void *_key);
static bool Dict_contains(const void *_self, const void *_other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

void *NamespaceDict_grow(void *_self, size_t mod);
size_t NamespaceDict_hash(const void *_self, const void *_key);


/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.size = sizeof(struct Dict),
	.class = &class,
	.super = NULL,

	// construction
	.New = Dict_new,
	.Del = Dict_del,
	.Copy = NULL,

	// comparison
	.Cmp = NULL,
	.Eq  = Dict_eq,
	.Ne  = Dict_ne,
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
	.Add = NULL,
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
	.Iadd = NULL,
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
	.Hash = NULL,
	.Str = Dict_str,
	.Repr = NULL,
	.Int = NULL,
	.Float = NULL,
	.Bool = Dict_bool,

	// containers
	.Len = Dict_len,
	.Getitem = Dict_getitem,
	.Setitem = Dict_setitem,
	.Delitem = Dict_delitem,
	.Iter = NULL,
	.Reversed = NULL,
	.Contains = Dict_contains,
};

struct Namespace_dict Dict = {
	.Class = &class,
	.Grow = NamespaceDict_grow,
	.Hash = NamespaceDict_hash,
};

/**********************************************************
 * Construction
 **********************************************************/

static void *Dict_new(void *_self, va_list *ap)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);

	self->values = calloc(DICT_DEFAULT_CAP, sizeof(void *));
	assert(self->values);
	self->keys = calloc(DICT_DEFAULT_CAP, sizeof(void *));
	assert(self->keys);

	self->size = 0;
	self->cap = DICT_DEFAULT_CAP;

	return self;
}

static void *Dict_del(void *_self)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);

	size_t i;
	for (i = 0; i < self->cap; i++) {
		if (self->keys[i] != NULL) {
			Del(self->keys[i]);
			Del(self->values[i]);
		}
	}

	free(self->values);
	self->values = NULL;
	free(self->keys);
	self->keys = NULL;

	return self;
}

static void *Dict_copy(const void *_self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	void *_new = New(Dict.Class);
	struct Dict *new = _new;

	size_t i;
	for (i = 0; i < self->cap; i++) {
		new->values[i] = Copy(self->values[i]);
		new->keys[i] = Copy(self->keys[i]);
	}
	new->size = self->size;
	new->cap = self->cap;
	return new;
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool Dict_eq(const void *_self, const void *_other)
{
	const struct Dict *self = _self;
	const struct Dict *other = _other;
	assert(self->class == Dict.Class);
	assert(other->class == Dict.Class);

	if (self->size != other->size || self->cap != other->cap) {
		return false;
	}

	size_t i;
	for (i = 0; i < self->cap; i++) {
		if (Ne(self->keys[i], other->keys[i]) ||
			Ne(self->values[i], other->values[i]))
		{
			return false;
		}
	}
	return true;
}

static bool Dict_ne(const void *_self, const void *_other)
{
	return !Dict_eq(_self, _other);
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

static char *Dict_str(const void *_self)
{
	return "TODO";
}

static bool Dict_bool(const void *_self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	return self->size > 0;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t Dict_len(const void *_self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	return self->size;
}

void *Dict_getitem(const void *_self, const void *_key)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(self, _key);
	return self->values[index];
}

void Dict_setitem(const void *_self, const void *_key, const void *_value)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(self, _key);
	self->values[index] = Copy(_value);
	self->keys[index] = Copy(_key);
}

void Dict_delitem(const void *_self, const void *_key)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(self, _key);
	Del(self->values[index]);
	Del(self->keys[index]);
}

static bool Dict_contains(const void *_self, const void *_other)
{
	const struct Dict *self = _self;
	const struct Class *const *other = _other;
	assert(self->class == Dict.Class);
	assert(*other);
	size_t index = Dict.Hash(self, _other);
	return Eq(self->keys[index], _other);
}

/**********************************************************
 * Namespace Functions
 **********************************************************/

size_t NamespaceDict_hash(const void *_self, const void *_key)
{
	const struct Dict *self = _self;
	const struct Class *const *key = _key;
	assert(self->class == Dict.Class);
	assert(_key && *key);
	assert((*key)->Hash && (*key)->Ne);
	
	size_t index = Hash(_key) % self->cap;
	/* valid key and keys not equal */
	while (self->keys[index] && Ne(self->keys[index], key)) {
		index = (index + DICT_COLLISION_BIAS) % self->cap;
	}
	return index;
}

void *NamespaceDict_grow(void *_self, size_t mod)
{
	struct Dict *self = _self;
	assert(_self);
	assert(mod >= 1);
	assert(self->class == Dict.Class);

	// swap data
	void **oldvals = self->values;
	void **oldkeys = self->keys;
	const size_t oldcap = self->cap;
	self->cap *= mod;

	self->values = calloc(self->cap, sizeof(void *));
	assert(self->values);
	self->keys = calloc(self->cap, sizeof(char *));
	assert(self->keys);

	size_t i, index;
	for (i = 0; i < oldcap; i++) {
		if (oldkeys[i]) {
			index = Dict.Hash(self, oldkeys[i]);

			// move value
			self->values[index] = oldvals[i];
			oldvals[i] = NULL;
			
			// move key
			self->keys[index] = oldkeys[i];
			oldkeys[i] = NULL;
		}
	}
	free(oldvals);
	free(oldkeys);

	return self;
}
