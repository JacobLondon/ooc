#include <assert.h>
#include <stdarg.h>

#include "class.h"
#include "dict.h"

#define DICT_DEFAULT_CAP 8
#define DICT_DEFAULT_SCALING 2
#define DICT_COLLISION_BIAS 10
#define DICT_FULL_RATIO (2.0 / 3.0)

/* class methods */
static void *Dict_new(void *_self, va_list *ap);
static void *Dict_del(void *_self);

/* methods */

static const struct Class _Dict = {
	.size = sizeof(struct Dict),
	.class = &_Dict,
	.super = NULL,

	.New = Dict_new,
	.Del = Dict_del,
	.Copy = NULL,
	.Hash = NULL,
	.Len = NULL,
	.Str = NULL,
};

const void *Dict = &_Dict;


static void *Dict_new(void *_self, va_list *ap)
{
	struct Dict *self = _self;

	self->values = calloc(DICT_DEFAULT_CAP, sizeof(void *));
	assert(self->values);
	self->keys = calloc(DICT_DEFAULT_CAP, sizeof(void *));
	assert(self->keys);

	self->Len = 0;
	self->cap = DICT_DEFAULT_CAP;

	return self;
}

static void *Dict_del(void *_self)
{
	size_t i;
	struct Dict *self = _self;

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
