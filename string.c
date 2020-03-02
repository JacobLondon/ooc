#include <assert.h>
#include <stdarg.h>
#include <string.h>

#include "class.h"
#include "string.h"

static void *String_ctor(void *_self, va_list *ap);
static void *String_dtor(void *_self);
static void *String_clone(const void *_self);
static char *String_str(const void *_self);

static void *String_ctor(void *_self, va_list *ap)
{
	struct String *self = _self;
	const char *text = va_arg(*ap, const char *);

	self->text = calloc(strlen(text) + 1, sizeof(char));
	assert(self->text);

	strcpy(self->text, text);
	return self;
}

static void *String_dtor(void *_self)
{
	struct String *self = _self;
	free(self->text);
	self->text = NULL;
	return self;
}

static void *String_clone(const void *_self)
{
	const struct String *self = _self;
	return new(String, self->text);
}

static char *String_str(const void *_self)
{
	const struct String *self = _self;
	return self->text;
}

static const struct Class _String = {
	.size = sizeof(struct String),
	.class = &_String,
	.super = NULL,

	.ctor = String_ctor,
	.dtor = String_dtor,
	.clone = String_clone,
	.str = String_str,
	.hash = NULL,
};

const void *String = &_String;
