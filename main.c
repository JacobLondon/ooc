#include <stdio.h>

#include "class.h"
#include "string.h"

int main(void)
{
	void *a = New(String.Class, "Hello, World!");

	printf("a = %s\n", String.Cstr(a));
	void *c = Copy(a);
	printf("c = %s\n", String.Cstr(c));
	printf("'World' at index %ld\n", String.Find(a, "World"));
	printf("a ?= c: %d\n", Eq(a, c));
	printf("len(a) == %zd\n", Len(a));
	void *d = Add(a, c);
	printf("d = %s\n", String.Cstr(d));
	Iadd(a, c);
	printf("a now is %s\n", String.Cstr(a));
	void *e = String.Substring(a, 6, 4);
	printf("a[6 len 4] == %s\n", String.Cstr(e));

	Del(a);
	Del(c);
	Del(d);
	Del(e);

	return 0;
}
