#include <assert.h>
#include <memory.h>

#include "list.h"
#include "iterator.h"
#include "util.h"
#include "class.h"

/**********************************************************
 * Class Function Prototypes
 **********************************************************/

// construction
static var           List_New           (var _self, va_list *ap);
static var           List_Del           (var _self);
static var           List_Copy          (const var _self);

// comparison
static bool          List_Eq            (const var _self, const var _other);
static bool          List_Ne            (const var _self, const var _other);

// unary

// arithmetic
static var           List_Add           (const var _self, const var _other);

// assignment arithmetic
static var           List_Iadd          (const var _self, const var _other);

// representation
static char*         List_Str           (const var _self);
static char*         List_Repr          (const var _self);
static bool          List_Bool          (const var _self);

// containers
static size_t        List_Len           (const var _self);
static shared        List_Getitem       (const var _self, const var _key);
static void          List_Setitem       (var _self, const var _key, const var _value);
static void          List_Delitem       (var _self, const var _key);
static shared        List_Next          (var _self, va_list *ap);
static var           List_Iter          (const var _self);
static bool          List_Contains      (const var _self, const var _other);

/**********************************************************
 * Namespace Function Prototypes
 **********************************************************/

static void     NamespaceList_Clear         (var _self);
static void     NamespaceList_Push_back     (var _self, var _value);
static void     NamespaceList_Emplace_back  (var _self, const void *_class, ...);
static void     NamespaceList_Take_back     (var _self, var _value);
static void     NamespaceList_Push_front    (var _self, var _value);
static void     NamespaceList_Emplace_front (var _self, const void *_class, ...);
static void     NamespaceList_Take_front    (var _self, var _value);
static var      NamespaceList_Pop_back      (var _self);
static var      NamespaceList_Pop_front     (var _self);
static var      NamespaceList_Initializer   (var _self, ...);
static void     NamespaceList_Insert        (var _self, var _value, size_t idx);
static void     NamespaceList_Insert_emplace(var _self, size_t idx, const void *_class, ...);
static void     NamespaceList_Insert_take   (var _self, var _value, size_t idx);

/**********************************************************
 * Definitions
 **********************************************************/

static const struct Class class = {
	.class     = &class,
	.super     = NULL,
	.name      = "List",
	.size      = sizeof(struct List),

	// construction
	.New       = List_New,
	.Del       = List_Del,
	.Copy      = List_Copy,

	// comparison
	.Cmp       = NULL,
	.Eq        = List_Eq,
	.Ne        = List_Ne,
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
	.Add       = List_Add,
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
	.Iadd      = List_Iadd,
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
	.Str       = List_Str,
	.Repr      = List_Repr,
	.Int       = NULL,
	.Uint      = NULL,
	.Float     = NULL,
	.Bool      = List_Bool,

	// containers
	.Len       = List_Len,
	.Getitem   = List_Getitem,
	.Setitem   = List_Setitem,
	.Delitem   = List_Delitem,
	.Next      = List_Next,
	.Iter      = List_Iter,
	.Reversed  = NULL,
	.Contains  = List_Contains,
};

struct NamespaceList List = {
	.Class         = &class,
	.Clear         = NamespaceList_Clear,
	.Push_back     = NamespaceList_Push_back,
	.Emplace_back  = NamespaceList_Emplace_back,
	.Take_back     = NamespaceList_Take_back,
	.Push_front    = NamespaceList_Push_front,
	.Emplace_front = NamespaceList_Emplace_front,
	.Take_front    = NamespaceList_Take_front,
	.Pop_back      = NamespaceList_Pop_back,
	.Pop_front     = NamespaceList_Pop_front,
	.Initializer   = NamespaceList_Initializer,
	.Insert        = NamespaceList_Insert,
	.Insert_emplace= NamespaceList_Insert_emplace,
	.Insert_take   = NamespaceList_Insert_take,
};

struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
	var *value;
};

struct ListIterator {
	struct ListNode *cur;
};

/**********************************************************
 * Construction
 **********************************************************/

static var List_New(var _self, va_list *ap)
{
	struct List *self = _self;
	assert(self->class = List.Class);

	self->head = self->tail = NULL;
	self->size = 0;

	return self;
}

static var List_Del(var _self)
{
	struct List *self = _self;
	struct ListNode *cur = self->head;
	assert(self->class = List.Class);
	
	/* quick exit */
	if (cur == NULL) {
		return self;
	}

	for (;;) {
		if (cur->next != NULL) {
			cur = cur->next;
			Del(cur->prev->value);
			cur->prev->value = NULL;
			cur->prev->prev  = NULL;
			cur->prev->next  = NULL;
			free(cur->prev);
		}
		/* last item / at tail */
		else {
			Del(cur->value);
			cur->value = NULL;
			cur->prev = NULL;
			free(cur);
			break;
		}
	}

	return self;
}

static var List_Copy(const var _self)
{
	struct List *self = _self;
	assert(self->class = List.Class);

	var _new = New(List.Class);
	struct List *new = _new;

	struct ListNode *cur = self->head;

	/* quick exit */
	if (cur == NULL) {
		return new;
	}

	/* traverse backwards, copy / push front in new */
	for (; cur != NULL; cur = cur->next) {
		List.Push_back(new, cur->value);
	}

	return new;
}

/**********************************************************
 * Comparison
 **********************************************************/

static bool List_Eq(const var _self, const var _other)
{
	const struct List *self = _self;
	const struct List *other = _other;
	struct ListNode *scur, *ocur;
	assert(self->class == List.Class);
	assert(other->class == List.Class);

	if (self->size != other->size) {
		return false;
	}

	for (scur = self->head, ocur = other->head;
	     scur != NULL && ocur != NULL;
		 scur = scur->next, ocur = ocur->next)
	{
		if (Ne(scur->value, ocur->value)) {
			return false;
		}
	}
	return true;
}

static bool List_Ne(const var _self, const var _other)
{
	return !List_Eq(_self, _other);
}

/**********************************************************
 * Unary
 **********************************************************/

/**********************************************************
 * Arithmetic
 **********************************************************/

static var List_Add(const var _self, const var _other)
{
	const struct List *self = _self;
	const struct List *other = _other;
	struct ListNode *cur;
	assert(self->class == List.Class);
	assert(other->class == List.Class);

	void *_new = Copy(_self);
	struct List *new = _new;

	for (cur = other->head; cur != NULL; cur = cur->next) {
		NamespaceList_Push_back(new, cur->value);
	}

	return new;
}

/**********************************************************
 * Assignment Arithmetic
 **********************************************************/

static var List_Iadd(const var _self, const var _other)
{
	struct List *self = _self;
	const struct List *other = _other;
	struct ListNode *cur;
	assert(self->class == List.Class);
	assert(other->class == List.Class);

	for (cur = other->head; cur != NULL; cur = cur->next) {
		NamespaceList_Push_back(self, cur->value);
	}

	return self;
}

/**********************************************************
 * Representation
 **********************************************************/

static char *List_Str(const var _self)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	struct ListNode *cur;
	char *value;
	char *text;

	if (self->size == 0) {
		text = strdup("[]");
		assert(text);
		return text;
	}

	text = strdup("[");
	assert(text);
	for (cur = self->head; cur != NULL; cur = cur->next) {
		value = Str(cur->value);
		strcatf(&text, "%s ", value);
		free(value);
	}
	text[strlen(text) - 2] = ']';
	text[strlen(text) - 1] = '\0';
	return text;
}

static char *List_Repr(const var _self)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	char *text = NULL;
	strcatf(&text, "'<%s object at 0x%x>'", Nameof(_self), (size_t)self);
	assert(text);
	return text;
}

static bool List_Bool(const var _self)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	return self->size > 0;
}

/**********************************************************
 * Containers
 **********************************************************/


static size_t List_Len(const var _self)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	return self->size;
}

static shared List_Getitem(const var _self, const var _key)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	size_t idx = Uint(_key);
	size_t i;
	struct ListNode *cur;

	// wrap-around
	if (idx > self->size) {
		idx = idx % self->size;
	}

	for (i = 0, cur = self->head; cur != NULL; i++, cur = cur->next) {
		if (i == idx) {
			break;
		}
	}

	return cur->value;
}

static void List_Setitem(var _self, const var _key, const var _value)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	size_t idx = Uint(_key);
	size_t i;
	struct ListNode *cur;

	// wrap-around
	if (idx > self->size) {
		idx = idx % self->size;
	}

	for (i = 0, cur = self->head; cur != NULL; i++, cur = cur->next) {
		if (i == idx) {
			Del(cur->value);
			cur->value = Copy(_value);
			break;
		}
	}
}

static void List_Delitem(var _self, const var _key)
{
	struct List *self = _self;
	assert(self->class == List.Class);
	size_t idx = Uint(_key);
	size_t i;
	struct ListNode *cur;

	// wrap-around
	if (idx > self->size) {
		idx = idx % self->size;
	}

	for (i = 0, cur = self->head; cur != NULL; i++, cur = cur->next) {
		if (i == idx) {
			Del(cur->value);
			break;
		}
	}
	self->size--;
}

static shared List_Next(var _self, va_list *ap)
{
	struct List *self = _self;
	assert(self->class == List.Class);

	struct ListIterator *state = va_arg(*ap, void *);
	shared tmp;

	if (state->cur == self->tail) {
		state->cur = self->head;
		return NULL;
	}

	if (self->size > 1) {
		for (; state->cur != NULL;) {
			tmp = state->cur->value;
			state->cur = state->cur->next;
			return tmp;
		}
	}
	else if (self->size == 1) {
		state->cur = self->head;
		return self->head;
	}
	
	state->cur = self->head;
	return NULL;
}

static var List_Iter(const var _self)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	struct ListIterator state = {
		.cur = NULL,
	};
	var iterator = New(Iterator.Class, _self, &state, (size_t)sizeof(state));
	return iterator;
}

static bool List_Contains(const var _self, const var _other)
{
	const struct List *self = _self;
	assert(self->class == List.Class);
	struct ListNode *cur;

	for (cur = self->head; cur != NULL; cur = cur->next) {
		if (Eq(cur->value, _other)) {
			return true;
		}
	}
	return false;
}

/**********************************************************
 * Namespace Functions
 **********************************************************/

static void NamespaceList_Clear(var _self)
{
	(void)List_Del(_self);
}

static void NamespaceList_Push_back(var _self, var _value)
{
	struct List *self = _self;
	assert(self->class == List.Class);

	NamespaceList_Take_back(self, Copy(_value));
}

static void NamespaceList_Emplace_back(var _self, const void *_class, ...)
{
	struct List *self = _self;
	va_list ap;
	var new;
	assert(self->class == List.Class);

	va_start(ap, _class);
	new = Vnew(_class, &ap);
	va_end(ap);
	NamespaceList_Take_back(self, new);
}

static void NamespaceList_Take_back(var _self, var _value)
{
	struct List *self = _self;
	assert(self->class == List.Class);
	struct ListNode *node = calloc(1, sizeof(struct ListNode));
	struct ListNode *tmp;
	assert(node);
	node->value = _value;

	if (self->size > 1) {
		// set tail next to new
		tmp = self->tail;
		tmp->next = node;
		// set new's prev to tail
		node->prev = tmp;
		// move tail to new end
		self->tail = node;
	}
	// head and tail are same node, split head from tail
	else if (self->size == 1) {
		// tail points to new, head next points to tail
		tmp = self->head;
		self->tail = tmp->next = node;
		// tail prev points to head
		tmp = self->tail;
		tmp->prev = self->head;
	}
	// no nodes in list, set head to tail to value
	else {
		self->head = self->tail = node;
	}
	self->size++;
}

static void NamespaceList_Push_front(var _self, var _value)
{
	struct List *self = _self;
	assert(self->class == List.Class);

	NamespaceList_Take_front(self, Copy(_value));
}

static void NamespaceList_Emplace_front(var _self, const void *_class, ...)
{
	struct List *self = _self;
	va_list ap;
	var new;
	assert(self->class == List.Class);

	va_start(ap, _class);
	new = Vnew(_class, &ap);
	va_end(ap);
	NamespaceList_Take_front(self, new);
}

static void NamespaceList_Take_front(var _self, var _value)
{
	struct List *self = _self;
	assert(self->class == List.Class);
	struct ListNode *node = calloc(1, sizeof(struct ListNode));
	struct ListNode *tmp;
	assert(node);
	node->value = _value;

	if (self->size > 1) {
		// set head prev to new
		tmp = self->head;
		tmp->prev = node;
		// set new's next to head
		node->next = tmp;
		// move head to new front
		self->head = node;
	}
	// head and tail are same node, split head from tail
	else if (self->size == 1) {
		// head points to new, tail prev points to head
		tmp = self->tail;
		self->head = tmp->prev = node;
		// head next points to tail
		tmp = self->head;
		tmp->next = self->tail;
	}
	// no nodes in list, set head to tail to value
	else {
		self->head = self->tail = node;
	}
	self->size++;
}

static var NamespaceList_Pop_back(var _self)
{
	struct List *self = _self;
	var tmp;
	struct ListNode *node = self->tail;
	assert(self->class == List.Class);

	if (self->size > 2) {
		// get value out
		tmp = node->value;
		// adjust the tail to penultimate item
		node = node->prev;
		// clear the tail node
		node->next->value = NULL;
		node->next->prev  = NULL;
		// free the tail node
		free(node->next);
		// move tail to what was the penultimate item
		self->tail = node;
		self->size--;
	}
	// re-join head and tail
	else if (self->size == 2) {
		// get value out
		tmp = node->value;
		// clear the tail node
		node->value = NULL;
		node->prev = NULL;
		// free the tail node
		free(self->tail);
		// move tail to head
		self->tail = self->head;
		self->size--;
	}
	// remove last item
	else if (self->size == 1) {
		tmp = node->value;
		self->head = NULL;
		self->tail = NULL;
		free(node);
		self->size--;
	}
	// none
	else {
		tmp = NULL;
	}
	return tmp;
}

static var NamespaceList_Pop_front(var _self)
{
	struct List *self = _self;
	var tmp;
	struct ListNode *node = self->head;
	assert(self->class == List.Class);

	if (self->size > 2) {
		// get value out
		tmp = node->value;
		// adjust the head to second item
		node = node->next;
		// clear the head node
		node->prev->value = NULL;
		node->prev->next  = NULL;
		// free the tail node
		free(node->prev);
		// move head to what was the second item
		self->head = node;
		self->size--;
	}
	// re-join head and tail
	else if (self->size == 2) {
		// get value out
		tmp = node->value;
		// clear the head node
		node->value = NULL;
		node->next  = NULL;
		// free the head node
		free(self->head);
		// move head to tail 
		self->head = self->tail;
		self->size--;
	}
	// remove last item
	else if (self->size == 1) {
		tmp = node->value;
		self->head = NULL;
		self->tail = NULL;
		free(node);
		self->size--;
	}
	// none
	else {
		tmp = NULL;
	}
	return tmp;
}

static var NamespaceList_Initializer(var _self, ...)
{
	struct List *self = _self;
	assert(self->class == List.Class);
	va_list ap;
	var p = NULL;

	va_start(ap, _self);

	for (;;) {
		p = va_arg(ap, var);
		if (p == NULL) {
			break;
		}
		NamespaceList_Take_back(_self, p);
	}

	va_end(ap);
}

static void NamespaceList_Insert(var _self, var _value, size_t idx)
{
	NamespaceList_Insert_take(_self, Copy(_value), idx);
}

static void NamespaceList_Insert_emplace(var _self, size_t idx, const void *_class, ...)
{
	struct List *self = _self;
	va_list ap;
	var new;
	assert(self->class == List.Class);

	va_start(ap, _class);
	new = Vnew(_class, &ap);
	va_end(ap);
	NamespaceList_Insert_take(self, new, idx);
}

static void NamespaceList_Insert_take(var _self, var _value, size_t idx)
{
	struct List *self = _self;
	assert(self->class == List.Class);
	size_t i;
	struct ListNode *cur;
	struct ListNode *new;
	var tmp;

	// wrap-around
	if (idx > self->size) {
		idx = idx % self->size;
	}

	// no items
	if (self->size == 0) {
		NamespaceList_Take_back(self, _value);
		return;
	}
	// head and tail are the same item
	else if (self->size == 1) {
		NamespaceList_Take_front(self, _value);
		return;
	}

	// standard case
	for (i = 0, cur = self->head; cur != NULL; i++, cur = cur->next) {
		if (i == idx) {
			// replace the item at the index with the other
			tmp = cur->value;
			cur->value = _value;
			new = calloc(1, sizeof(struct ListNode));
			assert(new);

			// move the current item to the next
			new->next = cur->next;
			new->prev = cur;
			new->value = tmp;
			// set next's prev to the node
			cur->next->prev = new;

			// set the current item to the new value
			cur->next = new;
			// prev stays same

			break;
		}
	}
}
