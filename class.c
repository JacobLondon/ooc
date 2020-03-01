#include <stdio.h>
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

void delete(void * self)
{
	const struct Class **cp = self;
	if (self && *cp && (*cp)->dtor) {
		self = (* cp)->dtor(self);
		free(self);
	}
}

size_t size_of(const void *self)
{
	const struct Class *const *cp = self;
	assert(self && *cp);
	return (*cp)->size;
}

int main(void)
{
	void *a = new(String, "a");
	void *b = new(String, "b");

	printf("%s\n", ((struct String *)a)->text);

	delete(a);
	delete(b);

	return 0;
}
