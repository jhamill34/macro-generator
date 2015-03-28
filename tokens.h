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
                    || x == MINUS || x == FSLASH || x == BSLASH || x == BANG

// TODO: replace line[i] with x... we lucked out on this bug
#define IS_VALID_IDENTIFIER(x) (line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO) \
                            || (line[i] >= A_LOWER && line[i] <= Z_LOWER) \
                            || (line[i] >= A_UPPER && line[i] <= Z_UPPER) \
                            || line[i] == UNDERSCORE

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
#define OPEN_MACRO "//MACRO:"
#define CLOSE_MACRO ":MACRO//"
#define INLINE_OPEN "{{"
#define INLINE_CLOSE "}}"
#define FOR_LOOP "for"
#define IF "if"
#define ELSE "else"

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

typedef struct Token{
  std::string value;
  TokenType type;
  int col_num; // not yet
  int row_num;
} Token;

void add_token(std::queue<Token *> *, std::string, TokenType);
std::queue<Token *> * scanner(char *);

#endif
