#ifndef _TOKENS_H_
#define _TOKENS_H_

// White space tokens
#define WS ' '
#define TAB '\t'
#define NEWLINE '\n'

// Symbols some tokens require forward looking
// ie += or << or {{ 
#define EQ '='
#define GT '>'
#define LT '<'
#define LP '('
#define RP ')'
#define LB '{'
#define RB '}'
#define SEMICOLON ';'
#define COMMA ','
#define AND '&'
#define PIPE '|'
#define ASTERISK '*'
#define PLUS '+'
#define MINUS '-'
#define FSLASH '/'
#define BSLASH '\\'
#define UNDERSCORE '_'
#define BANG '!'
#define STRING_QUOTE_SINLE '\''
#define STRING_QUOTE_DOUBLE '\"'

// Use a range to determine 
// val >= NUMBER_ONE || val <= NUMBER_NINE
#define NUMBER_ONE '1'
#define NUMBER_ZERO '0'
#define NUMBER_NINE '9'
#define A_LOWER 'a'
#define A_UPPER 'A'
#define Z_LOWER 'z'
#define Z_UPPER 'Z'


// Token Types
typedef enum {
	NONE,
	STRING,
	NUMBER,
	SYMBOL,
	WHITESPACE,
	COMMENT,
	IDENTIFIER
} TokenType;

typedef struct {
	char * value;
	TokenType type;
} Token;

// Functions to define the token types
// based off of a string
Token * createToken(char * value, TokenType type);

#endif 