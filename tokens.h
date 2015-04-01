#ifndef _TOKENS_H_
#define _TOKENS_H_

#include <string>
#include <queue>
#include <stack>

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
#define COLON ':'
#define SEMICOLON ';'
#define COMMA ','
#define AND '&'
#define PIPE '|'
#define ASTERISK '*'
#define CARET '^'
#define PLUS '+'
#define MINUS '-'
#define FSLASH '/'
#define BSLASH '\\'
#define UNDERSCORE '_'
#define BANG '!'
#define STRING_QUOTE_SINLE '\''
#define STRING_QUOTE_DOUBLE '\"'


#define IS_SYMBOL(x) x == EQ || x == GT || x == LT || x == COLON || x == COMMA || x == AND \
                    || x == PIPE || x == ASTERISK || x == PLUS  \
                    || x == MINUS || x == FSLASH || x == BSLASH || x == BANG || x == CARET

// TODO: replace line[i] with x... we lucked out on this bug
#define IS_VALID_IDENTIFIER(x) (x <= NUMBER_NINE && x >= NUMBER_ZERO) \
                            || (x >= A_LOWER && x <= Z_LOWER) \
                            || (x >= A_UPPER && x <= Z_UPPER) \
                            || x == UNDERSCORE

// Use a range to determine 
// val >= NUMBER_ONE || val <= NUMBER_NINE
#define NUMBER_ONE '1'
#define NUMBER_ZERO '0'
#define NUMBER_NINE '9'
#define A_LOWER 'a'
#define A_UPPER 'A'
#define Z_LOWER 'z'
#define Z_UPPER 'Z'

// special
#define OPEN_MACRO "<\%"
#define CLOSE_MACRO "\%>"
#define INLINE_OPEN "{{"
#define INLINE_CLOSE "}}"
#define FOR_LOOP "for"
#define IF "if"
#define ELSE "else"
#define INCLUDE "include"

// Token Types
typedef enum {
  NONE,
  PROGRAM,
  PRINT,
  STRING,
  NUMBER,
  SYMBOL,
  WHITESPACE,
  COMMENT,
  IDENTIFIER,
  OPEN_PAREN,
  CLOSE_PAREN,
  OPEN_BLOCK,
  CLOSE_BLOCK,
  FOR_LOOP_STATEMENT,
  IF_CONDITION,
  ELSE_CONDITION,
  MACRO_START,
  MACRO_END,
  INLINE_START,
  INLINE_END,
  END_STATEMENT,
  EOP
} TokenType;

#define IS_CONSTANT(x) x == STRING || x == NUMBER


// DEFINED Priorities to used to order
// Expressions in the correcto way PEMDAS
#define DEFAULT_PRIORTY -1 // Note this is unsigned
#define LOGIC 1
#define INEQUALITY 2
#define ADD 3
#define SUB 4
#define MULT 6
#define DIV 5
#define EXP 7
#define UNARY 8
#define PAREN_DELTA 8

typedef struct Token{
  std::string value;
  TokenType type;
  int col_num; // not yet
  int row_num;
  unsigned int priority;
} Token;

void add_token(std::queue<Token *> *, std::string, TokenType);
std::queue<Token *> * scanner(char *, std::queue<Token *> * );

#endif
