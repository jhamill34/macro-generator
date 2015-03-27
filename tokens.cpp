#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include "tokens.h"

using namespace std;

void add_token(queue<Token *> * token_queue, string value, TokenType type)
{
  Token * tmp = new Token;
  tmp->value = value;
  tmp->type = type;
  token_queue->push(tmp);
}

queue<Token *> * 
scanner(char * filename){
  string line;
  unsigned int i;
  unsigned int lineNum = 0;
  bool inMacro = false;
  queue<Token *> * token_queue = new queue<Token *>;

  ifstream input(filename, ifstream::in);
  while(getline(input, line))
  {
    for(i = 0; i < (line.size() + 1); i++)
    {
      if(inMacro)
      {
        if(i == line.size()){ 
          inMacro = false;
        }else if(line.compare(i, strlen(FOR_LOOP), FOR_LOOP) == 0){
          add_token(token_queue, "FOR", FOR_LOOP_STATEMENT);
          i += (strlen(FOR_LOOP) - 1);
        }else if(line.compare(i, strlen(IF), IF) == 0){
          add_token(token_queue, "IF", IF_CONDITION);
          i += (strlen(IF) -1);
        }else if(line.compare(i, strlen(ELSE), ELSE) == 0){
          add_token(token_queue, "ELSE", ELSE_CONDITION);
          i += (strlen(ELSE) - 1);
        }else if(line[i] == LP){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, OPEN_PAREN);
        }else if(line[i] == RP){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, CLOSE_PAREN);
        }else if(line[i] == LB){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, OPEN_BLOCK);
        }else if(line[i] == RB){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, CLOSE_BLOCK);
        }else if(line[i] == SEMICOLON){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, END_STATEMENT);
        }else if(line[i] == STRING_QUOTE_DOUBLE){
          int start, end;
          i++;
          start = i;
          while(line[i] != STRING_QUOTE_DOUBLE){
             i++;
          }
          end = i;
          add_token(token_queue, line.substr(start, end - start), STRING);
        }else if(line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO){
          // Assumes numbers are not concatenated with letters or anything
          int start, end;
          start = i;
          while(line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO){
            i++;
          }
          end = i;
          i--;
          add_token(token_queue, line.substr(start, end - start), NUMBER);
        }else if(IS_SYMBOL(line[i])){
          int start, end;
          start = i;
          if(IS_SYMBOL(line[i+1])){
            i++;
          }
          end = i + 1;
         
          add_token(token_queue, line.substr(start, end - start), SYMBOL); 
        }else if(line[i] != WS && line[i] != TAB){
          int start, end;
          start = i; 
          while(IS_VALID_IDENTIFIER(line[i])){
            i++;
          }
          end = i;
          i--;
          add_token(token_queue, line.substr(start, end - start), IDENTIFIER);
        }
      }else{
        if(line.compare(i, strlen(OPEN_MACRO), OPEN_MACRO) == 0){
          i += (strlen(OPEN_MACRO) - 1);
          inMacro = true;
        }else if(line.size() > 0){
          int start, end;
          start = i;
          while(i < line.size()){
            if(i <= (line.size() - strlen(INLINE_OPEN)) && line.compare(i, strlen(INLINE_OPEN), INLINE_OPEN) == 0){
              end = i;
              if(end != start){  
                add_token(token_queue, line.substr(start, end - start), PRINT);
              }
            
              add_token(token_queue, line.substr(i, strlen(INLINE_OPEN)), INLINE_START);
              i += (strlen(INLINE_OPEN));

              while(line.compare(i, strlen(INLINE_CLOSE), INLINE_CLOSE)){
                if(line[i] != WS && line[i] != TAB){
                  int start, end;
                  start = i;
                  while(IS_VALID_IDENTIFIER(line[i])){
                    i++;
                  }
                  end = i;
                  add_token(token_queue, line.substr(start, end - start), IDENTIFIER);
                }
                i++;
              }

              add_token(token_queue, line.substr(i, strlen(INLINE_CLOSE)), INLINE_END);
              i += (strlen(INLINE_CLOSE)-1);
              start = i + 1;
            }
            i++;
          }
          end = i;
          if(end != start){  
            add_token(token_queue, line.substr(start, end - start), PRINT);
          }
        }else{
          add_token(token_queue, "\n", PRINT);
        }
      }
    }
    lineNum++;
  }

  return token_queue;
}


