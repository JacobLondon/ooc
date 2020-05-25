# Object-Oriented C
Reading Object-Oriented Programming with ANSI-C

# Easy Tooling
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
