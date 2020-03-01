#include <stdio.h>

#include "class.h"
#include "string.h"

int main(void)
{
	void *a = new(String, "Hello, World!");

	printf("a = %s\n", to_string(a));
	void *c = clone(a);
	printf("c = %s\n", to_string(c));

	delete(a);
	delete(c);

	return 0;
}
