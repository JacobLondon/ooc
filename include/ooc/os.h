#ifndef OOC_OS_H
#define OOC_OS_H

#include "types.h"

struct NamespaceOs {
	int       (* System)     (char *command);
	int       (* Chdir)      (char *location);
	var       (* Pwd)        (void); /* return String */
	var       (* Ls)         (char *directory); /* return Vector[String], NULL for cur directory */
};

extern struct NamespaceOs Os;

#endif // OOC_OS_H
