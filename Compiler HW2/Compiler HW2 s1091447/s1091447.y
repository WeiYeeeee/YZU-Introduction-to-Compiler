%{
    #include <stdio.h>
    #include <ctype.h>
    #include <string.h>
    #include <stdlib.h>
    extern FILE* yyin;
    typedef struct yy_buffer_state * YY_BUFFER_STATE;
    extern int yyparse();
    extern int yylex_destroy(void);
    extern YY_BUFFER_STATE yy_scan_string(char * str);
    extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
    extern void yy_switch_to_buffer(YY_BUFFER_STATE buffer);
    int yylex(void);
    void yyerror(char *s);
%}
%token INT
%token ADD SUB MUL DIV
%token EOL
%token quit

%%
cal : cal exp EOL { printf("%d\n", $2); }
    | quit { printf("bye!\n"); exit(0); }
    |
    ;
exp : term
    | exp ADD term { $$ = $1 + $3; }
    | exp SUB term { $$ = $1 - $3; }
    ;
term : unary
     | term MUL unary { $$ = $1 * $3; }
     | term DIV unary { $$ = $1 / $3; }
     ;
unary: SUB unary { $$ = $2 * -1; }
     | factor
     ;
factor : INT { $$ = $1; }
       | '(' exp ')' { $$ = ($2); }
       ;
%%

int main(int argc, char **argv)
{
    FILE* fp;
    fp = fopen("testcase.in", "r");
    char get_ch,last_ch='0';
    int i=0;
    char *result = malloc (i+2);
    while((get_ch=fgetc(fp))!=EOF) {
        if(get_ch=='-'&&(last_ch=='('||i==0)){
          result[i]='0';
          i++;
        }
        result[i]=get_ch;
        last_ch=get_ch;
        result[i+1]='\0';
        i++;
    }

    YY_BUFFER_STATE bp =yy_scan_string(result);
    yy_switch_to_buffer(bp);
    yyparse();
    yy_delete_buffer(bp);
    yylex_destroy();
    fclose(fp);
    return 0;
}

void yyerror(char *s)
{
    fprintf(stderr, "%s\n", s);
}

