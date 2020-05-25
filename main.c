#include <assert.h>
#include <stdio.h>

#include "class.h"
#include "string.h"
#include "dict.h"
#include "integer.h"
#include "vector.h"
#include "real.h"
#include "boolean.h"

int main(void)
{
	{
		var a = New(String.Class, "Hello, World!");

		println("a = {}", a);
		var c = Copy(a);
		println("c = {}", c);
		printf("'World' at index %ld\n", String.Find(a, "World"));
		printf("a ?= c: %d\n", Eq(a, c));
		printf("len(a) == %zd\n", Len(a));
		var d = Add(a, c);
		println("d = {}", d);
		Iadd(a, c);
		println("a now is {}", a);
		var e = String.Substring(a, 6, 4);
		println("a[6:4] == {}", e);

		Del(a);
		Del(c);
		Del(d);
		Del(e);
	}

	{
		var mydict = New(Dict.Class);

		var a = New(String.Class, "Hello");
		var b = New(String.Class, "There");
		Setitem(mydict, a, b);
		
		printf("a in mydict? %d\n", Contains(mydict, a));
		char *s = Str(mydict);
		println("Dict: {}", mydict);
		Del(a);
		Del(b);
		Del(mydict);
		free(s);
	}

	{
		var v = New(Vector.Class);
		Vector.Emplace_back(v, Integer.Class, 100);
		Vector.Emplace_back(v, Integer.Class, 200);
		Vector.Emplace_back(v, Real.Class, 123.0);
		Vector.Emplace_back(v, Boolean.Class, true);
		Vector.Emplace_back(v, Dict.Class);
		Vector.Emplace_back(v, Vector.Class);

		if (Bool(v)) {
			println("v has things in it");
		}
		else {
			println("v has nothing in it");
		}

		Vector.Emplace_back(v, Vector.Class);
		println("{}", v);
		Del(v);
	}

	return 0;
}
