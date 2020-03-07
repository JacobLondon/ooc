#include <stdio.h>

#include "class.h"
#include "string.h"

int main(void)
{
	void *a = new(String, "Hello, World!");

	printf("a = %s\n", str(a));
	void *c = copy(a);
	printf("c = %s\n", str(c));

	del(a);
	del(c);

	return 0;
}
