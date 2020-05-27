#ifndef OOC_CLASS_H
#define OOC_CLASS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <sys/types.h>

typedef void *var;
typedef void *shared;
typedef struct pair {
	var key;
	var value;
} pair;

struct Class {
	const struct Class *class;
	const struct Class *super;
	const char*  name;
	size_t size;

	// construction
	var      (* New)        (var _self, va_list *ap);
	var      (* Del)        (var _self);
	var      (* Copy)       (const var _self);

	// comparison
	ssize_t  (* Cmp)        (const var _self, const var _other);
	bool     (* Eq)         (const var _self, const var _other);
	bool     (* Ne)         (const var _self, const var _other);
	bool     (* Lt)         (const var _self, const var _other);
	bool     (* Gt)         (const var _self, const var _other);
	bool     (* Le)         (const var _self, const var _other);
	bool     (* Ge)         (const var _self, const var _other);

	// unary
	var      (* Pos)       (const var _self);
	var      (* Neg)       (const var _self);
	var      (* Abs)       (const var _self);
	var      (* Invert)    (const var _self);
	var      (* Round)     (const var _self, size_t n);
	var      (* Floor)     (const var _self);
	var      (* Ceil)      (const var _self);
	var      (* Trunc)     (const var _self);

	// arithmetic
	var      (* Add)       (const var _self, const var _other);
	var      (* Sub)       (const var _self, const var _other);
	var      (* Mul)       (const var _self, const var _other);
	var      (* Floordiv)  (const var _self, const var _other);
	var      (* Div)       (const var _self, const var _other);
	var      (* Mod)       (const var _self, const var _other);
	var      (* Pow)       (const var _self, const var _other);
	var      (* Lshift)    (const var _self, const var _other);
	var      (* Rshift)    (const var _self, const var _other);
	var      (* And)       (const var _self, const var _other);
	var      (* Or)        (const var _self, const var _other);
	var      (* Xor)       (const var _self, const var _other);

	// assignment arithmetic
	shared   (* Iadd)      (var _self, const var _other);
	shared   (* Isub)      (var _self, const var _other);
	shared   (* Imul)      (var _self, const var _other);
	shared   (* Ifloordiv) (var _self, const var _other);
	shared   (* Idiv)      (var _self, const var _other);
	shared   (* Imod)      (var _self, const var _other);
	shared   (* Ipow)      (var _self, const var _other);
	shared   (* Ilshift)   (var _self, const var _other);
	shared   (* Irshift)   (var _self, const var _other);
	shared   (* Iand)      (var _self, const var _other);
	shared   (* Ior)       (var _self, const var _other);
	shared   (* Ixor)      (var _self, const var _other);

	// representation
	size_t   (* Hash)      (const var _self);
	char*    (* Str)       (const var _self);
	char*    (* Repr)      (const var _self);
	ssize_t  (* Int)       (const var _self);
	size_t   (* Uint)      (const var _self);
	double   (* Float)     (const var _self);
	bool     (* Bool)      (const var _self);

	// containers
	size_t   (* Len)       (const var _self);
	shared   (* Getitem)   (const var _self, const var _key);
	void     (* Setitem)   (var _self, const var _key, const var _value);
	void     (* Delitem)   (var _self, const var _key);
	shared   (* Next)      (var _self);
	var      (* Iter)      (const var _self);
	var      (* Reversed)  (const var _self);
	bool     (* Contains)  (const var _self, const var _other);
};

// class-neutral introspection
size_t        Sizeof        (const var _self);
const void*   Super         (const void *_self);
const void*   Type          (const var _self);
const char*   Nameof        (const var _self);
bool          Isinstance    (const void *_class, const var _other);
bool          Issubclass    (const var _self, const void *_class);

// construction
var           New           (const void *_class, ...);
void          Del           (var _self);
var           Copy          (const var _self);
var           Vnew          (const void *_class, va_list *ap);

// comparison
ssize_t       Cmp           (const var _self, const var _other);
bool          Eq            (const var _self, const var _other);
bool          Ne            (const var _self, const var _other);
bool          Lt            (const var _self, const var _other);
bool          Gt            (const var _self, const var _other);
bool          Le            (const var _self, const var _other);
bool          Ge            (const var _self, const var _other);

// unary
var           Pos           (const var _self);
var           Neg           (const var _self);
var           Abs           (const var _self);
var           Invert        (const var _self);
var           Round         (const var _self, size_t n);
var           Floor         (const var _self);
var           Ceil          (const var _self);
var           Trunc         (const var _self);

// arithmetic
var           Add           (const var _self, const var _other);
var           Sub           (const var _self, const var _other);
var           Mul           (const var _self, const var _other);
var           Floordiv      (const var _self, const var _other);
var           Div           (const var _self, const var _other);
var           Mod           (const var _self, const var _other);
var           Pow           (const var _self, const var _other);
var           Lshift        (const var _self, const var _other);
var           Rshift        (const var _self, const var _other);
var           And           (const var _self, const var _other);
var           Or            (const var _self, const var _other);
var           Xor           (const var _self, const var _other);

// assignment arithmetic
shared        Iadd          (var _self, const var _other);
shared        Isub          (var _self, const var _other);
shared        Imul          (var _self, const var _other);
shared        Ifloordiv     (var _self, const var _other);
shared        Idiv          (var _self, const var _other);
shared        Imod          (var _self, const var _other);
shared        Ipow          (var _self, const var _other);
shared        Ilshift       (var _self, const var _other);
shared        Irshift       (var _self, const var _other);
shared        Iand          (var _self, const var _other);
shared        Ior           (var _self, const var _other);
shared        Ixor          (var _self, const var _other);

// representation
size_t        Hash          (const var _self);
char*         Str           (const var _self);
char*         Repr          (const var _self);
ssize_t       Int           (const var _self);
size_t        Uint          (const var _self);
double        Float         (const var _self);
bool          Bool          (const var _self);

// containers
size_t        Len           (const var _self);
shared        Getitem       (const var _self, const var _key);
void          Setitem       (var _self, const var _key, const var _value);
void          Delitem       (var _self, const var _key);
shared        Next          (var _self);
var           Iter          (const var _self);
var           Reversed      (const var _self);
bool          Contains      (const var _self, const var _other);

/* Empty Definition
static const struct Class class = {
	.size      = sizeof(struct Example),
	.class     = &class,
	.super     = NULL,

	// construction
	.New       = Example_new,
	.Del       = Example_del,
	.Copy      = NULL,

	// comparison
	.Cmp       = NULL,
	.Eq        = NULL,
	.Ne        = NULL,
	.Lt        = NULL,
	.Gt        = NULL,
	.Le        = NULL,
	.Ge        = NULL,

	// unary
	.Pos       = NULL,
	.Neg       = NULL,
	.Abs       = NULL,
	.Invert    = NULL,
	.Round     = NULL,
	.Floor     = NULL,
	.Ceil      = NULL,
	.Trunc     = NULL,

	// arithmetic
	.Add       = NULL,
	.Sub       = NULL,
	.Mul       = NULL,
	.Floordiv  = NULL,
	.Div       = NULL,
	.Mod       = NULL,
	.Pow       = NULL,
	.Lshift    = NULL,
	.Rshift    = NULL,
	.And       = NULL,
	.Or        = NULL,
	.Xor       = NULL,

	// assignment arithmetic
	.Iadd      = NULL,
	.Isub      = NULL,
	.Imul      = NULL,
	.Ifloordiv = NULL,
	.Idiv      = NULL,
	.Imod      = NULL,
	.Ipow      = NULL,
	.Ilshift   = NULL,
	.Irshift   = NULL,
	.Iand      = NULL,
	.Ior       = NULL,
	.Ixor      = NULL,

	// representation
	.Hash      = NULL,
	.Str       = NULL,
	.Repr      = NULL,
	.Int       = NULL,
	.Uint      = NULL,
	.Float     = NULL,
	.Bool      = NULL,

	// containers
	.Len       = NULL,
	.Getitem   = NULL,
	.Setitem   = NULL,
	.Delitem   = NULL,
	.Iter      = NULL,
	.Reversed  = NULL,
	.Contains  = NULL,
};
*/

#define ClassError(Class0, Class1) do { \
	printf("%s: %s not compatible with %s.\n", __func__, ((struct Class *)Class0)->name, ((struct Class *)Class1)->name); \
	assert(0); \
} while (0)

char *Format(const char *_fmt, ...);
char *Format_va(const char *_fmt, va_list *ap);
void Println(const char *_fmt, ...);

#endif /* OOC_CLASS_H */
