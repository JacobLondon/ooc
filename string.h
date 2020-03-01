#ifndef OOC_STRING_H
#define OOC_STRING_H

struct String {
	const void *class;
	char *text;
};

extern const void *String;

#endif /* OOC_STRING_H */