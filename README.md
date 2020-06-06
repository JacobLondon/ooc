# Object-Oriented C
Reading Object-Oriented Programming with ANSI-C


# To Do
* Initializer for Vector and Dict should just be in new, the current functions should be renamed
* Repr print function
* Next for Strings (actual iterator stuff)
* 

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
		shared line;
		while (line = Next(lines)) {
			Println("{}", line);
		}
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
