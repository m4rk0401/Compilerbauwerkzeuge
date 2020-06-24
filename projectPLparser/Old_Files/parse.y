%{
   #include <stdio.h>

   extern int yyerror(char* err);
   extern int yylex(void);
%}

%union {
   
}


%start stmtseq

%token VARIABLE
%token OPENPAR
%token CLOSEPAR
%token TOP
%token BOTTOM
%token KOMMA
%token NEWLINE

%precedence BIIMP
%precedence IMP
%precedence OR
%precedence AND
%precedence NOT
%precedence ALL
%precedence EX
%precedence PREDICATE
%precedence FUNCTION


%%

stmtseq: /* Empty */
    | NEWLINE stmtseq       {}
    | NEWLINE stmtseq {}
    | error NEWLINE stmtseq {};  /* After an error, start afresh */

term: VARIABLE {$<val>$ = $<val>1; printf("term> %s", $<val>$);}
    | FUNCTION OPENPAR term CLOSEPAR {$<val>$ = $<val>1+$<val>2+$<val>3+$<val>4; printf("term> %s", $<val>$)}
    | term KOMMA term {$<val>$ = $<val>1+$<val>2+$<val>3; printf("term> %s", $<val>$)};

atom: PREDICATE {}
    | PREDICATE OPENPAR term CLOSEPAR {};

formula: atom {$<val>$ = $<val>1; printf("atom> %s", $<val>$);}
    | NOT formula {$<val>$ = $<val>1+$<val>2; printf("atom> %s", $<val>$);}
    | formula AND formula {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);}
    | formula OR formula {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);}
    | formula IMP formula {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);}
    | formula BIIMP formula {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);}
    | ALL VARIABLE formula {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);}
    | EX VARIABLE formula {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);}
    | OPENPAR formula CLOSEPAR {$<val>$ = $<val>1+$<val>2+$<val>3; printf("atom> %s", $<val>$);};


%%

int yyerror(char* err)
{
   printf("Error: %s\n", err);
   return 0;
}


int main (int argc, char* argv[])
{

  return yyparse();
}
