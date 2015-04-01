Need to be able to enter into the MACRO
then need to be able to parse the line and execute the command


Example based on file using macro 

!! Change block characters like PHP??
!! if so we need a comment character to ignore
//MACRO: for(i = 0; i < 10; i++){
	this is some text that will be repeated... {{ i }}
//MACRO: }

would produce the inner line 10 times using the inline //MACRO: tag
and the inline tag {{ $i }}
!! Important need this support

Purpose of the language:
We only want this to be a macro language

for loops
while loops
if else elif statements
basic math operators + - * / % ++ -- += 
	!need support for uniary operators
	TODO: if we see shorthand assign operators we could 
		compile to what we need in the AST
Logical operators < > <= >= == != 
groupings ( ) { } 
*Array Literals [ ]
asignment operator = 
end of statements ;
String Literals "" ''

What we don't need:
built in data structures such as arrays or dictionarys (posibly rev 2)

4. Variable scopes?? right now its only global
4b. Namespaces. Keeping the global scope for now
2. Including files higher priority. Two pass, insert files were they are requested (first pass macro) then parse new created file DONE
1. fix nesting blocks bug DONE
3. order of operations for logical statements DONE
?. Comments done
?. Fixed macro blocks done
5+.Funtions and Data structures?? definitely a rev 2 action
