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

	if (class->new) {
		va_list ap;
		va_start(ap, _class);
		self = class->new(self, &ap);
		va_end(ap);
	}

	return self;
}

void del(void *_self)
{
	const struct Class **class = _self;
	if (_self && *class && (*class)->del) {
		_self = (* class)->del(_self);
		free(_self);
	}
}

size_t size_of(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	return (*class)->size;
}

const void *super(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && (*class));
	assert((*class)->super);
	return (*class)->super;
}

const void *type(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->class);
	return (*class)->class;
}

bool isinstance(const void *_self, const void *_class)
{
	return _self && class_of(_self);
}

bool issubclass(const void *_self, const void *_class)
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
					return false;
				}
			}
			return true;
		}
	}
	return false;
}

void *copy(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->copy);
	return (*class)->copy(_self);
}

char *str(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->str);
	return (*class)->str(_self);
}

size_t hash(const void *_self)
{
	const struct Class *const *class = _self;
	assert(_self && *class);
	assert((*class)->hash);
	return (*class)->hash(_self);
}
