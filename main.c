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
	/*{
		var a = New(String.Class, "Hello, World!");

		Println("a = {}", a);
		var c = Copy(a);
		Println("c = {}", c);
		printf("'World' at index %ld\n", String.Find(a, "World"));
		printf("a ?= c: %d\n", Eq(a, c));
		printf("len(a) == %zd\n", Len(a));
		var d = Add(a, c);
		Println("d = {}", d);
		Iadd(a, c);
		Println("a now is {}", a);
		var e = String.Substring(a, 6, 4);
		Println("a[6:4] == {}", e);

		Del(a);
		Del(c);
		Del(d);
		Del(e);
	}*/

	{
		var mydict = New(Dict.Class);

		var a = New(String.Class, "Hello");
		var b = New(String.Class, "There");
		Setitem(mydict, a, b);
		
		printf("a in mydict? %d\n", Contains(mydict, a));
		char *s = Str(mydict);
		Println("Dict: {}", mydict);

		Println("Next on dict: {}", mydict);
		pair *item;
		while (item = Next(mydict)) {
			Println("{}: {}", item->key, item->value);
		}

		Dict.Initializer(mydict, New(String.Class, "1234"), New(String.Class, "abcd"), NULL, NULL);
		Println("Dict initializer: {}", mydict);

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
			Println("v has things in it");
		}
		else {
			Println("v has nothing in it");
		}

		Vector.Emplace_back(v, Vector.Class);
		Println("{}", v);

		Println("Printing all the items:");
		shared item;
		while (item = Next(v)) {
			Println("{}", item);
		}

		Println("Initializer Vector:");
		Vector.Initializer(v, New(String.Class, "Hello"), New(String.Class, "World"), NULL);
		Println("{}", v);
		
		Del(v);
	}

	{
		var s = String.Fread("main.c");
		var v = Vector.Strsplit(s, "\n");

		shared line;
		while (line = Next(v)) {
			Println("{}", line);
		}

		Del(s);
		Del(v);
	}

	return 0;
}
