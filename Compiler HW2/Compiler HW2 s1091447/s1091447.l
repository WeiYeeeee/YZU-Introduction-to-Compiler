%{
    #include <stdio.h>
    #include <ctype.h>
    #include "s1091447.tab.h"
    extern int yylval;
%}

%%
[0-9]+ {
    yylval = atoi(yytext);
    //printf("number: %d  ", yylval);
    return INT;
}
"+" {
    //printf("operator: +  ");
    return ADD;
}
"-" {
    //printf("operator: -  ");
    return SUB;
}
"*" {
    //printf("operator: *  ");
    return MUL;
}
"/" {
    //printf("operator: /  ");
    return DIV;
}
";" { return EOL; }
[()] return yytext[0];
[ \t\n] ;
%%

int yywrap(void)
{
    return 1;
}
