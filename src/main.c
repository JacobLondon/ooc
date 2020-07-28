#include <assert.h>
#include <stdio.h>

#include <ooc/ooc.h>

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

#if 0
	{
		var mydict = New(Dict.Class);

		var a = $String("Hello");
		var b = $String("There");
		Setitem(mydict, a, b);
		
		printf("a in mydict? %d\n", Contains(mydict, a));
		char *s = Str(mydict);
		Println("Dict: {}", mydict);

		Println("Next on dict: {}", mydict);
		pair *item;
		var it = Iter(mydict);
		while (item = Next(it)) {
			Println("{}: {}", item->key, item->value);
		}

		Dict.Initializer(mydict, $String("1234"), $String("abcd"), NULL);
		Println("Dict initializer: {}", mydict);

		Del(it);
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
	}

	{
		var v = $Vector();
		Vector.Initializer(v, $String("Hello"), $String("World"), NULL);

		$For(shared, word, v, {
			Println("{}", word);
		});

		Del(v);
	}

	{
		var d = $Dict();
		Dict.Initializer(d,
			$String("Yes"), $Int(10),
			$String("No"), $Int(50),
			NULL);

		$For(pair*, item, d, {
			Println("{}: {}", item->key, item->value);
		});

		Del(d);
	}

	{
		var s = $String("12345678");
		Println("{}", s);

		$For(char*, p, s, {
			printf("%c\n", *p);
		});

		Del(s);
	}

	{
		var mylist = $List();
		List.Initializer(mylist, $String("Hello"), $String("World"));
		var i = Iter(mylist);
		shared word;
		while (word = Next(i)) {
			Println("{}", word);
		}
		Del(mylist);
		Del(i);
	}
	
#endif

	var json = Json.Load("test/test.json");
	Println("{}", json);
	var key = $String("d");
	var key2 = $String("b");
	Println("Hello, World? {}", Getitem(Getitem(json, key), key2));
	Del(key);
	Del(key2);
	Del(json);

	return 0;
}
