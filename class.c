#include <assert.h>

#include "class.h"
#include "string.h"

void *new(const void *_class, ...)
{
	const struct Class *class = _class;
	void *self = calloc(1, class->size);

	assert(self);
	/* Technically not a Class *, it is pointing
	   to the first element of the custom struct
	   with the first member being a Class * and
	   setting that */
	*(const struct Class **)self = class;

	if (class->ctor) {
		va_list ap;
		va_start(ap, _class);
		self = class->ctor(self, &ap);
		va_end(ap);
	}

	return self;
}

void delete(void *_self)
{
	const struct Class **cp = _self;
	if (_self && *cp && (*cp)->dtor) {
		_self = (* cp)->dtor(_self);
		free(_self);
	}
}

void *clone(const void *_self)
{
	const struct Class *const *cp = _self;
	assert(_self && *cp);
	assert((*cp)->clone);
	return (*cp)->clone(_self);
}

size_t size_of(const void *_self)
{
	const struct Class *const *cp = _self;
	assert(_self && *cp);
	return (*cp)->size;
}

char *to_string(const void *_self)
{
	const struct Class *const *cp = _self;
	assert(_self && *cp);
	assert((*cp)->to_string);
	return (*cp)->to_string(_self);
}
