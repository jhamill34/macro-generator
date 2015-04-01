<% include(examples/include.mg) 
       	// We are printing the condition below 10 times
	for(i = 0; i < 10; i += 1 ){ // Actually anthin after double slash is a comment
		// If this is true print "less than..."
		if(i < 5){ %>
Less than 5 -- {{ i }}.
	<% 	// Else print greater than...
		}else{ 
	%>
Greater than 5 -- {{ i }}.
	<% 	} // END IF/ELSE
	} //END FOR LOOP %> <-- can't comment out tags either :/
	%>

Contact:{{ j += 1 }} 

Inline value:
j = {{ j }} 
some other stuff 

Contact:{{ j += 1 }} 

Anything that is not in the Macro style blocks gets printed. 
There are no excaped versions of those blocks though so .... don't use them unless 
you mean to 

<% excape = "<% %>"; %>

If you really need to use them use a string and inline blocks (ie. "{{ excape }}" )
Contact:{{ j += 1 }} 
<% 
// We can do complex expressions too!
if(rst == "tes" || 57 == (1 + 2) * ( 3 + 4 ^ 2)){ 
// The next line prints a newline before TEST... BEWARE spaces after tags!!!!
%> 
TEST 
THIS WILL NOT BE PRINTED :)	<% }else{  %>  NEITHER WILL THIS
This wont be printed either
<% 
}
b = -10 * -2 / 10;
if(b){ %>
B is defined as 
{{ b += 1 }} 
{{ b }} 
<% }else{ %>
but really B isn't defined
<% } 

// we can compare strings too!
if(rst == "test"){
// watch out for indented output text
// |
// v
%>
	rst is indeed {{ rst }}  
<%
}
%>

Contact:{{ j += 1 }}

<% // a = 1;  set a in another file

// If else construct 
if(a == 1){ %>
Bill {{ a }} 
<% }else if(a == 2){ %>
BOB {{ a }} 
<% }else{ %>
ROB {{ a }} 
<% }  %> 
