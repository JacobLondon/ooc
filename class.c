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

	if (class->v->ctor) {
		va_list ap;
		va_start(ap, _class);
		self = class->v->ctor(self, &ap);
		va_end(ap);
	}

	return self;
}

void delete(void *_self)
{
	const struct Class **class = _self;
	if (_self && *class && (*class)->v->dtor) {
		_self = (* class)->v->dtor(_self);
		free(_self);
	}
}

const void *super(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && (*class));
	assert((*class)->super);
	return (*class)->super;
}

size_t size_of(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	return (*class)->size;
}

const void *class_of(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->class);
	return (*class)->class;
}

bool is_a(const void *_self, const void *_class)
{
	return _self && class_of(_self);
}

bool is_of(const void *_self, const void *_class)
{
	assert(_class);
	
	if (_self) {
		const struct Class *class = class_of(_self);
		if (class != NULL) {
			while (class != _class) {
				if (class != NULL) {
					class = super(class);
				}
				else {
					return 0;
				}
			}
			return 1;
		}
	}
	return 0;
}

void *clone(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->v->clone);
	return (*class)->v->clone(_self);
}

char *str(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->v->str);
	return (*class)->v->str(_self);
}

size_t hash(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->v->hash);
	return (*class)->v->hash(_self);
}
