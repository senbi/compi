
// There may be some flexer directives here

%%
[ \t\n]+                                    // skip white space chars. 
[0-9]+                                      return NUMBER;  
[[:alpha:]_][[:alpha:][:digit:]_]*          return IDENTIFIER;
.                                           return matched()[0];

// IDENTIFIERs have been defined as any character from the alphabet or
// an underscore followed by 0 or more occurrences of any alphanumeric
// character or an underscore. :alpha: and :digit: are predefined.

// NUMBER and  IDENTIFIER are values  of an enum  type and have  to be
// defined. We shall show elsewhere how.

// matched () returns a string containing the detected lexeme - the 
// equivalent of yytext in lex.
