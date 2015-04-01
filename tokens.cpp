#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <string>
#include "tokens.h"

using namespace std;

/** Function to add a new token to the queue based off a value and type
  * @param string value 
  * @param queue<Token *> * pointer to a queue that holds tokens
  * @param enum TokenType type
  * @return void
  */
void add_token(queue<Token *> * token_queue, string value, TokenType type, int col, int row)
{
  Token * tmp = new Token;
  tmp->value = value;
  tmp->type = type;
  tmp->col_num = col;
  tmp->row_num = row;
  if(type == SYMBOL){
    if(tmp->value[0] == PLUS ){
      tmp->priority = ADD;
    }else if(tmp->value[0] == MINUS){
      tmp->priority = SUB;
    }else if(tmp->value[0] == ASTERISK){
      tmp->priority = MULT;
    }else if(tmp->value[0] == FSLASH){
      tmp->priority = DIV;
    }else if(tmp->value[0] == CARET){
      tmp->priority = EXP;
    }else if(tmp->value[0] == LT || tmp->value[0] == GT || tmp->value[0] == EQ || tmp->value[0] == BANG){
      tmp->priority = INEQUALITY;
    }else if(tmp->value.size() == 2 && ((tmp->value[0] == AND && tmp->value[1] == AND) || (tmp->value[0] == PIPE && tmp->value[1] == PIPE))){
      tmp->priority = LOGIC;
    }else{
      tmp->priority = DEFAULT_PRIORTY;
    }
  }else{
    tmp->priority = DEFAULT_PRIORTY;
  }


  token_queue->push(tmp);
}

/**
* Function to parse through each character in a file and analyze if the 
* characters and following characters form a particular token
* @param char * filename (ie char array)
* @param queue<Token *> * queue of all the tokens found FIFO
*/
queue<Token *> * 
scanner(char * filename, queue<Token *> * token_queue, string ns){
  if(ns.compare(0, 1, "")){
    ns = ns + ".";
  }

  string line;
  unsigned int i;
  unsigned int lineNum = 0;
  bool inMacro = false;
  bool mainProgram = false;
  if(token_queue == NULL){      
        token_queue = new queue<Token *>;
        mainProgram = true;
  }

  ifstream input(filename, ifstream::in);
  while(getline(input, line))
  {
    for(i = 0; i < (line.size() + 1); i++)
    {
      if(inMacro)
      {
        //if(i == line.size()){ 
        if(line.compare(i, strlen(CLOSE_MACRO), CLOSE_MACRO) == 0){  
          i += (strlen(CLOSE_MACRO) - 1);
          inMacro = false;
          break;
        }else if(line.compare(i, strlen(INCLUDE), INCLUDE) == 0){
          int start, end;
          i += (strlen(INCLUDE));
          
          if(line[i] == LP){
            i++;

            while(line[i] == WS || line[i] == TAB || line[i] == NEWLINE){
              i++;
            }

            start = i;
            while(line[i] != COMMA){
              i++;
            }

            end = i;
            char * fname = (char *)line.substr(start, end - start).c_str();
            i++;
            
            while(line[i] == WS || line[i] == TAB || line[i] == NEWLINE){
              i++;
            }

            start = i;
            while(line[i] != RP){
              i++;
            }
            end = i;
            string new_ns = line.substr(start, end - start);

            scanner(fname, token_queue, new_ns);
          }else{
            cout << "Expected open parenthesis after include statement" << endl;
            exit(1);
          }

        }else if(line.compare(i, strlen(FOR_LOOP), FOR_LOOP) == 0 
              && !(IS_VALID_IDENTIFIER(line[i + strlen(FOR_LOOP)]))){
          add_token(token_queue, "FOR", FOR_LOOP_STATEMENT, i, lineNum);
          i += (strlen(FOR_LOOP) - 1);
        }else if(line.compare(i, strlen(IF), IF) == 0
          && !(IS_VALID_IDENTIFIER(line[i + strlen(IF)]))){
          add_token(token_queue, "IF", IF_CONDITION, i, lineNum);
          i += (strlen(IF) -1);
        }else if(line.compare(i, strlen(ELSE), ELSE) == 0
          && !(IS_VALID_IDENTIFIER(line[i + strlen(ELSE)]))){
          add_token(token_queue, "ELSE", ELSE_CONDITION, i, lineNum);
          i += (strlen(ELSE) - 1);
        }else if(line[i] == LP){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, OPEN_PAREN, i, lineNum);
        }else if(line[i] == RP){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, CLOSE_PAREN, i, lineNum);
        }else if(line[i] == LB){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, OPEN_BLOCK, i, lineNum);
        }else if(line[i] == RB){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, CLOSE_BLOCK, i, lineNum);
        }else if(line[i] == SEMICOLON){
          char ch[2] = {line[i], '\0'};
          add_token(token_queue, ch, END_STATEMENT, i, lineNum);
        }else if(line[i] == STRING_QUOTE_DOUBLE){
          int start, end;
          i++;
          start = i;
          while(line[i] != STRING_QUOTE_DOUBLE){
             i++;
          }
          end = i;
          add_token(token_queue, line.substr(start, end - start), STRING, i, lineNum);
        }else if(line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO){
          // Assumes numbers are not concatenated with letters or anything
          int start, end;
          start = i;
          while(line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO){
            i++;
          }
          end = i;
          i--;
          add_token(token_queue, line.substr(start, end - start), NUMBER, i, lineNum);
        }else if(IS_SYMBOL(line[i])){
          int start, end;
          start = i;
          if(IS_SYMBOL(line[i+1])){
            if(line[i] == FSLASH && line[i + 1] == FSLASH){
              break; // ignore the rest of the line its a comment
            }
            i++;
          }
          end = i + 1;
         
          add_token(token_queue, line.substr(start, end - start), SYMBOL, i, lineNum); 
        }else if(line[i] != WS && line[i] != TAB && line[i] != NEWLINE && line[i] != '\0'){
          int start, end;
          start = i; 
          while(IS_VALID_IDENTIFIER(line[i])){
            i++;
          }
          end = i;
          i--;
          add_token(token_queue, ns + line.substr(start, end - start), IDENTIFIER, i, lineNum);
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
                add_token(token_queue, line.substr(start, end - start), PRINT, i, lineNum);
              }
            
              add_token(token_queue, line.substr(i, strlen(INLINE_OPEN)), INLINE_START, i, lineNum);
              i += (strlen(INLINE_OPEN));

              while(line.compare(i, strlen(INLINE_CLOSE), INLINE_CLOSE)){
                if(line[i] != WS && line[i] != TAB){
                  if(IS_SYMBOL(line[i])){
                    int start, end;
                    start = i;
                    if(IS_SYMBOL(line[i+1])){
                      i++;
                    }
                    end = i + 1;
                    
                    add_token(token_queue, line.substr(start, end - start), SYMBOL, i, lineNum); 
                  }else if(line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO){
                    // Assumes numbers are not concatenated with letters or anything
                    int start, end;
                    start = i;
                    while(line[i] <= NUMBER_NINE && line[i] >= NUMBER_ZERO){
                      i++;
                    }
                    end = i;
                    i--;
                    add_token(token_queue, line.substr(start, end - start), NUMBER, i, lineNum);
                  }else if(IS_VALID_IDENTIFIER(line[i])){
                    int start, end;
                    start = i;
                    while(IS_VALID_IDENTIFIER(line[i])){
                      i++;
                    }
                    end = i;
                    add_token(token_queue, line.substr(start, end - start), IDENTIFIER, i, lineNum);
                  }
                }
                i++;
              }

              add_token(token_queue, line.substr(i, strlen(INLINE_CLOSE)), INLINE_END, i, lineNum);
              i += (strlen(INLINE_CLOSE)-1);
              start = i + 1;
            }else if(i <= (line.size() - strlen(OPEN_MACRO)) && line.compare(i, strlen(OPEN_MACRO), OPEN_MACRO) == 0){
              inMacro = true;
              i += (strlen(OPEN_MACRO));
              start = i;

              break;
            }
            i++;
          }
          end = i;
          if(end != start){  
            add_token(token_queue, line.substr(start, end - start) + "\n", PRINT, i, lineNum);
          }
        }else{
          add_token(token_queue, "\n", PRINT, i, lineNum);
        }
      }
    }
    lineNum++;
  }
  if(mainProgram){
    add_token(token_queue, "Program End", EOP, -1, -1);
  }
  return token_queue;
}


