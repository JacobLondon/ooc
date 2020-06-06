#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "util.h"
#include "class.h"
#include "dict.h"
#include "iterator.h"

#define DICT_DEFAULT_CAP 8
#define DICT_DEFAULT_SCALING 2
#define DICT_COLLISION_BIAS 10
#define DICT_FULL_RATIO (2.0 / 3.0)

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static var           Dict_New           (var _self, va_list *ap);
static var           Dict_Del           (var _self);
static var           Dict_Copy          (const var _self);

// comparison
static bool          Dict_Eq            (const var _self, const var _other);
static bool          Dict_Ne            (const var _self, const var _other);

// unary

// arithmetic

// assignment arithmetic

// representation
static size_t        Dict_Hash          (const var _self);
static char*         Dict_Str           (const var _self);
static char*         Dict_Repr          (const var _self);
static bool          Dict_Bool          (const var _self);

// containers
static size_t        Dict_Len           (const var _self);
static shared        Dict_Getitem       (const var _self, const var _key);
static void          Dict_Setitem       (var _self, const var _key, const var _value);
static void          Dict_Delitem       (var _self, const var _key);
static shared        Dict_Next          (var _self, va_list *ap);
static var           Dict_Iter          (const var _self);
static bool          Dict_Contains      (const var _self, const var _other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static var           NamespaceDict_Reserve  (var _self, size_t mod);
static size_t        NamespaceDict_Hash     (const var _self, const var _key);
static void          NamespaceDict_Take     (var _self, var _key, var _value);
static void          NamespaceDict_Initializer(var _self, ...);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "Dict",
	.size      = sizeof(struct Dict),

	// construction
	.New       = Dict_New,
	.Del       = Dict_Del,
	.Copy      = NULL,

	// comparison
	.Cmp       = NULL,
	.Eq        = Dict_Eq,
	.Ne        = Dict_Ne,
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
	.Str       = Dict_Str,
	.Repr      = Dict_Repr,
	.Int       = NULL,
	.Uint      = NULL,
	.Float     = NULL,
	.Bool      = Dict_Bool,

	// containers
	.Len       = Dict_Len,
	.Getitem   = Dict_Getitem,
	.Setitem   = Dict_Setitem,
	.Delitem   = Dict_Delitem,
	.Next      = Dict_Next,
	.Iter      = Dict_Iter,
	.Reversed  = NULL,
	.Contains  = Dict_Contains,
};

struct NamespaceDict Dict = {
	.Class         = &class,
	.Clear         = NULL,
	.Reserve       = NamespaceDict_Reserve,
	.Shrink_to_fit = NULL,
	.Hash          = NamespaceDict_Hash,
	.Take          = NamespaceDict_Take,
	.Initializer   = NamespaceDict_Initializer,
};

struct DictIterator {
	size_t index;
	pair keyvalue;
};

/**********************************************************
 * Construction
 **********************************************************/

static var Dict_New(var _self, va_list *ap)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);

	self->values = calloc(DICT_DEFAULT_CAP, sizeof(var));
	assert(self->values);
	self->keys = calloc(DICT_DEFAULT_CAP, sizeof(var));
	assert(self->keys);

	self->size = 0;
	self->cap = DICT_DEFAULT_CAP;

	return self;
}

static var Dict_Del(var _self)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);

	for (size_t i = 0; i < self->cap; i++) {
		if (self->keys[i]) {
			Del(self->keys[i]);
			Del(self->values[i]);
			self->keys[i] = NULL;
			self->values[i] = NULL;
		}
	}

	free(self->values);
	self->values = NULL;
	free(self->keys);
	self->keys = NULL;

	return self;
}

static var Dict_Copy(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	var _new = New(Dict.Class);
	struct Dict *new = _new;
	var tmp;

	tmp = realloc(new->values, self->cap);
	assert(tmp);
	new->values = tmp;
	tmp = realloc(new->keys, self->cap);
	assert(tmp);
	new->keys = tmp;

	for (size_t i = 0; i < self->cap; i++) {
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

static bool Dict_Eq(const var _self, const var _other)
{
	const struct Dict *self = _self;
	const struct Dict *other = _other;
	assert(self->class == Dict.Class);
	assert(other->class == Dict.Class);

	if (self->size != other->size || self->cap != other->cap) {
		return false;
	}

	for (size_t i = 0; i < self->cap; i++) {
		if (Ne(self->keys[i], other->keys[i]) ||
			Ne(self->values[i], other->values[i]))
		{
			return false;
		}
	}
	return true;
}

static bool Dict_Ne(const var _self, const var _other)
{
	return !Dict_Eq(_self, _other);
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

static size_t Dict_Hash(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	return fnv1a(self, sizeof(self));
}

static char *Dict_Str(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t i;
	char *key, *value;
	char *text;

	if (self->size == 0) {
		text = strdup("{}");
		assert(text);
		return text;
	}

	text = strdup("{");
	assert(text);
	for (i = 0; i < self->cap; i++) {
		if (self->keys[i] != NULL) {
			// heap char stars!
			key = Str(self->keys[i]);
			value = Str(self->values[i]);
			strcatf(&text, "%s: %s, ", key, value);
			free(key);
			free(value);
		}
	}

	text[strlen(text) - 2] = '}';
	text[strlen(text) - 1] = '\0';
	return text;
}

static char *Dict_Repr(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	char *text = NULL;
	strcatf(&text, "'<%s object at 0x%x>'", Nameof(_self), (size_t)self);
	assert(text);
	return text;
}

static bool Dict_Bool(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	return self->size > 0;
}

/**********************************************************
 * Containers
 **********************************************************/

static size_t Dict_Len(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	return self->size;
}

static var Dict_Getitem(const var _self, const var _key)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(_self, _key);
	return self->values[index];
}

static void Dict_Setitem(var _self, const var _key, const var _value)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(self, _key);
	if (self->keys[index] != NULL) {
		Del(self->keys[index]);
		Del(self->values[index]);
	}
	if (self->size + 1 == (size_t)(self->cap * DICT_FULL_RATIO)) {
		NamespaceDict_Reserve(_self, DICT_DEFAULT_SCALING);
	}
	self->values[index] = Copy(_value);
	self->keys[index] = Copy(_key);
	self->size++;
}

static void Dict_Delitem(var _self, const var _key)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(self, _key);
	if (self->keys[index] != NULL) {
		Del(self->values[index]);
		Del(self->keys[index]);
		self->size--;
	}
}

static shared Dict_Next(var _self, va_list *ap)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);

	struct DictIterator *state = va_arg(*ap, void *);
	assert(state);

	if (self->size == 0 || state->index == self->size) {
		state->index = 0;
		state->keyvalue = (pair){ NULL, NULL };
		return NULL;
	}

	for (; state->index < self->cap; state->index++) {
		if (self->keys[state->index] != NULL) {
			state->keyvalue = (pair){
				self->keys[state->index],
				self->values[state->index],
			};
			state->index++;
			return &state->keyvalue;
		}
	}

	state->index = 0;
	state->keyvalue = (pair){ NULL, NULL };
	return NULL;
}

static var Dict_Iter(const var _self)
{
	const struct Dict *self = _self;
	assert(self->class == Dict.Class);
	struct DictIterator state = {
		.index = 0,
		.keyvalue = (pair){ NULL, NULL },
	};

	var iterator = New(Iterator.Class, _self, &state, (size_t)sizeof(state));
	return iterator;
}

static bool Dict_Contains(const var _self, const var _other)
{
	const struct Dict *self = _self;
	const struct Class *other = _other;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(_self, _other);
	return Eq((const var)self->keys[index], _other);
}

/**********************************************************
 * Namespace Functions
 **********************************************************/

static size_t NamespaceDict_Hash(const var _self, const var _key)
{
	const struct Dict *self = _self;
	const struct Class *const *key = _key;
	assert(self->class == Dict.Class);
	assert(_key && *key);
	assert((*key)->Hash && (*key)->Ne);
	
	size_t index = Hash(_key) % self->cap;
	/* valid key and keys not equal */
	while (self->keys[index] && Ne(self->keys[index], _key)) {
		index = index + DICT_COLLISION_BIAS;
		index = fnv1a(&index, sizeof(index)) % self->cap;
	}
	return index;
}

static var NamespaceDict_Reserve(var _self, size_t mod)
{
	struct Dict *self = _self;
	assert(_self);
	assert(mod >= 1);
	assert(self->class == Dict.Class);

	// swap data
	var *oldvals = self->values;
	var *oldkeys = self->keys;
	const size_t oldcap = self->cap;
	self->cap *= mod;

	self->values = calloc(self->cap, sizeof(var));
	assert(self->values);
	self->keys = calloc(self->cap, sizeof(char *));
	assert(self->keys);

	for (size_t index, i = 0; i < oldcap; i++) {
		if (oldkeys[i] != NULL) {
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

static void NamespaceDict_Take(var _self, var _key, var _value)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);
	size_t index = Dict.Hash(self, _key);
	if (self->keys[index] != NULL) {
		Del(self->keys[index]);
		Del(self->values[index]);
	}
	if (self->size + 1 == (size_t)(self->cap * DICT_FULL_RATIO)) {
		NamespaceDict_Reserve(_self, DICT_DEFAULT_SCALING);
	}
	self->values[index] = _value;
	self->keys[index] = _key;
	self->size++;
}

static void NamespaceDict_Initializer(var _self, ...)
{
	struct Dict *self = _self;
	assert(self->class == Dict.Class);
	va_list ap;
	var key = NULL;
	var value = NULL;

	va_start(ap, _self);

	for (;;) {
		key = va_arg(ap, var);
		if (key == NULL) {
			break;
		}
		value = va_arg(ap, var);
		if (value == NULL) {
			break;
		}
		NamespaceDict_Take(_self, key, value);
	}

	va_end(ap);
}
