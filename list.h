#ifndef OOC_LIST_H
#define OOC_LIST_H

#include "types.h"

struct ListNode;

struct List {
    const void *class;
	struct ListNode *head;
	struct ListNode *tail;
	size_t size;
};

struct NamespaceList {
	const void *Class;
	void     (* Clear)(var _self);
	void     (* Push_back)(var _self, var _value);
	void     (* Emplace_back)(var _self, const void *_class, ...);
	void     (* Take_back)(var _self, var _value);
	void     (* Push_front)(var _self, var _value);
	void     (* Emplace_front)(var _self, const void *_class, ...);
	void     (* Take_front)(var _self, var _value);
	var      (* Pop_back)(var _self);
	var      (* Pop_front)(var _self);
	var      (* Initializer)(var _self, ...);
	void     (* Insert)(var _self, var _value, size_t idx);
	void     (* Insert_emplace)(var _self, size_t idx, const void *_class, ...);
	void     (* Insert_take)(var _self, var _value, size_t idx);
};

extern struct NamespaceList List;

#endif /* OOC_LIST_H */ 
