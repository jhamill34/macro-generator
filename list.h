#ifndef _LIST_H_
#define _LIST_H_

	typedef struct ListNode{
		Token * value;
		struct ListNode * next;
	} ListNode;

	typedef struct {
		ListNode * head;
		ListNode * tail;
	} TokenList;

	ListNode * push(TokenList * list, Token * tok);
	Token * end(TokenList * list);
	Token * first(TokenList * list);

	// Constructor Methods
	ListNode * createListNode(Token * value, ListNode * next);
	TokenList * createList();

#endif