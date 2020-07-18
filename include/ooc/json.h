#ifndef OOC_JSON_H
#define OOC_JSON_H

#include "types.h"

/**
 * A bunch of functions which operate on a 'dict' object,
 * assuming that all of its items are or are subsets of what I will
 * call 'serializable' to json.
 */

struct NamespaceJson {
	var  (* Load)(const char *path);
	var  (* Loads)(const var _string);
	void (* Dump)(const var _self, const char *path);
	var  (* Dumps)(const var _self);
};

extern struct NamespaceJson Json;

#endif /* OOC_JSON_H */
