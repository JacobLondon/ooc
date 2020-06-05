#include <assert.h>
#include <stdio.h>

#include "ooc.h"

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

	/*{
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
		var v = Vector.Strsplit(s, " \n\r\t");
		char *rep = Repr(s);
		printf("%s\n", rep);
		free(rep);
		rep = Repr(v);
		printf("%s\n", rep);
		free(rep);

		Del(s);
		Del(v);
	}*/

	{
		var v = New(Vector.Class);
		Vector.Initializer(v, New(String.Class, "Hello"), New(String.Class, "World"), NULL);

		var i = Iter(v);
		shared word;
		while (word = Next(i)) {
			Println("{}", word);
		}
		Del(i);
		Del(v);
	}

	{
		var d = New(Dict.Class);
		Dict.Initializer(d,
			New(String.Class, "Yes"), New(Integer.Class, 10),
			New(String.Class, "No"), New(Integer.Class, 50),
			NULL, NULL);

		var r = Iter(d);
		pair *item;
		while (item = Next(r)) {
			Println("{}: {}", item->key, item->value);
		}
		Del(r);
		Del(d);
	}

	{
		var s = New(String.Class, "12345678");
		Println("{}", s);
		var i = Iter(s);
		char *p;
		while (p = Next(i)) {
			printf("%c\n", *p);
		}
	}

	{
		var mylist = New(List.Class);
		List.Initializer(mylist, New(String.Class, "Hello"), New(String.Class, "World"), NULL);
		var i = Iter(mylist);
		shared word;
		while (word = Next(i)) {
			Println("{}", word);
		}
		Del(mylist);
		Del(i);
	}
	

	return 0;
}
