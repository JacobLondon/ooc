#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "class.h"
#include "string.h"

/* class methods */
static void *String_new(void *_self, va_list *ap);
static void *String_del(void *_self);
static void *String_copy(const void *_self);
static size_t String_len(const void *_self);
static char *String_str(const void *_self);

/* methods */

static const struct Class _String = {
	.size = sizeof(struct String),
	.class = &_String,
	.super = NULL,

	.new = String_new,
	.del = String_del,
	.copy = String_copy,
	.hash = NULL,
	.len = String_len,
	.str = String_str,
};

const void *String = &_String;

static void *String_new(void *_self, va_list *ap)
{
	struct String *self = _self;
	const char *text = va_arg(*ap, const char *);

	self->text = calloc(strlen(text) + 1, sizeof(char));
	assert(self->text);

	strcpy(self->text, text);
	return self;
}

static void *String_del(void *_self)
{
	struct String *self = _self;
	free(self->text);
	self->text = NULL;
	return self;
}

static void *String_copy(const void *_self)
{
	const struct String *self = _self;
	return new(String, self->text);
}

static size_t String_len(const void *_self)
{
	const struct String *self = _self;
	return strlen(self->text);
}

static char *String_str(const void *_self)
{
	const struct String *self = _self;
	return self->text;
}
