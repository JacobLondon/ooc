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
	while (line = Next(vec)) {
	Println("{}", line);
	}
	
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
		NULL, NULL
	);

	pair *item;
	while (item = Next(dict)) {
		Println("{}:", item->key);
		var lines = Vector.Strsplit(item->value, "\n\r");
		shared line;
		while (line = Next(lines)) {
			Println("{}", line);
		}
		Del(lines);
	}
	
	Del(dict);

	return 0;
}
```
