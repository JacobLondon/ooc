#ifndef OOC_TYPES_H
#define OOC_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

typedef void *var;

typedef void *shared;

typedef struct tuple1 {
	var value;
} tuple1;

typedef struct tuple2 {
	var key;
	var value;
} tuple2;

#endif /* OOC_TYPES_H */
