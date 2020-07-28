# Object-Oriented C
Reading Object-Oriented Programming with ANSI-C


# To Do
* Completely fix dict / vector GetbyInt/GetbyStr
* Repr print function

# Example 1
```C
#include <ooc.h>

int main(void)
{
	var str = String.Fread("data.txt");
	var vec = Vector.Strsplit(s, "\n");
	shared line;
	var it = Iter(vec);
	while (line = Next(it)) {
		Println("{}", line);
	}
	
	Del(it);
	Del(str);
	Del(vec);

	return 0;
}
```

# Example 2
```C
#include <ooc.h>

int main(void)
{
	var dict = New(Dict.Class);
	Dict.Initializer(dict,
		New(String.Class, "Hamlet"), String.Fread("hamlet.txt"),
		New(String.Class, "Macbeth"), String.Fread("macbeth.txt"),
		New(String.Class, "Othello"), String.Fread("othello.txt"),
		NULL
	);

	var it = Iter(dict);
	pair *item;
	while (item = Next(it)) {
		Println("{}:", item->key);
		var lines = Vector.Strsplit(item->value, "\n\r");
		var line_it = Iter(lines);
		shared line;
		while (line = Next(line_it)) {
			Println("{}", line);
		}
		Del(line_it);
		Del(lines);
	}
	
	Del(it);
	Del(dict);

	return 0;
}
```

# Example 3
Shorthand notation, indicated by the leading $
```C
#include <ooc.h>

int main(void)
{
	var dict = $Dict();
	Dict.Initializer(d,
		$String("Key1"), $Int(1),
		$String("Key2"), $Real(3.14159),
		NULL);
	
	$For(pair *, item, dict, {
		Println("{}: {}", item->key, item->value);
	});

	Del(dict);

	return 0;
}
```

# Example 4
Json reading and writing
```C
#include <ooc.h>

int main(void)
{
	var json = Json.Load("test/test.json");
	Println("{}", json);
	var str = Json.Loads("test/test.json");
	Println("Also string loading: {}", str);
	Dict.Take(json, $String("New entry"), $Real(134.3));
	Json.Dump("test/out.json");

	Del(str);
	Del(json);
	return 0;
}
```
