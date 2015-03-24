Token * createToken(char * value, TokenType type){
	Token * tmp = (Token *)malloc(sizeof(Token));
	tmp->value = (char *)malloc(sizeof(char) * strlen(value) + 1);
	strcpy(tmp->value, value);
	tmp->type = type;

	return tmp;
}

