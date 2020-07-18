#ifndef OOC_TYPES_H
#define OOC_TYPES_H

#include <stddef.h>
#include <sys/types.h>

typedef void *var;

typedef void *shared;

typedef struct pair {
	var key;
	var value;
} pair;

#endif /* OOC_TYPES_H */
