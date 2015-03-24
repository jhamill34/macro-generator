
// Constructor that returns an allocated amount of memory for a 
// list of tokens
TokenList * createList(){
	TokenList * token_list = (TokenList *)malloc(sizeof(TokenList));
	token_list->head = NULL;
	token_list->tail = NULL;

	return token_list;
}

ListNode * createListNode(Token * value, ListNode * next){
	ListNode * tmp = (ListNode *)malloc(sizeof(ListNode));
	tmp->value = value;
	tmp->next = next;

	return tmp;
}

// Returns the value just pushed on to the list
// head always is the new item
ListNode * append(TokenList * list, Token * tok){
	if(list->head == NULL){
		list->head = createListNode(tok, NULL);
		list->tail = list->head;
	}else{
		ListNode * tmp = createListNode(tok, NULL);
		list->tail->next = tmp;
		list->tail = tmp;
	}

	return list->head;
}

Token * end(TokenList * list){
	return list->tail->value;
}

Token * first(TokenList * list){
	return list->head->value;
}