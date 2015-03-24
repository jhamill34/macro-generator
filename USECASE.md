Generate File:

User calls program with a file
file parses through each line

if line contains the MACRO token
	parse line and generate TOKENS
else
	search for inline substitutions
	access stored variables
	replace the substitution with the value
	implicitly call the PRINT function

open up grammar file
parse and create data structure


===SCANNER===
The scanner's job is to read the source file one character at a 
time. For each character, it keeps track of the line and 
character position where the character was found. Each time the 
scanner is called, it reads the next character from the file and 
returns it.

===LEXER===
The lexer's job is to group the characters of the source file 
into chunks called tokens. (If the source text was written in a 
natural language (English, Spanish, French, etc.) the tokens 
would correspond to the words and punctuation marks in the text.)
Each time the lexer is called, it calls the scanner (perhaps 
several times) to get as many characters as it needs in order 
to assemble the characters into a token. It determines the type 
of token that it has found (a string, a number, an identifier, 
a comment, etc.) and returns the token.
This will be a set of #define statements that get converted to constant strings



PARSER
parse a gammar text file and store a "map" that maps the "statement" to a list of things it expects possibly "function" pointers

GENERATOR

