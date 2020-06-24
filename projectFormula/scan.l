%option noyywrap


%{
   #include "pTest.tab.h"
%}
 
VARIABLE            [n-z][0-9]*
FUNCTION           [a-m][0-9]*
PRAEDICATE      [A-Z][0-9]*

%%

{VARIABLE} {
   printf("scanning variable %s\n", yytext);
   yylval.val = strdup(yytext);
   return VARIABLE;
}

{FUNCTION} {
   printf("scanning function %s\n", yytext);
   yylval.val = strdup(yytext);
   return FUNCTION;
}

{PRAEDICATE} {
   printf("scanning praedicate %s\n", yytext);
   yylval.val = strdup(yytext);
   return PRAEDICATE;
}

"&" {
   printf("scanning AND\n");
   return AND;
}

"|"  {
   printf("scanning OR\n");
   return OR;
}

"~" {
   printf("scanning NOT\n");
   return NOT;
}

"->" {
   printf("scanning IMPLICATION\n");
   return IMP;
}

"<->"  {
   printf("scanning EQUIVALENCE\n");
   return EQU;
}

"all" {
   printf("scanning ALL\n");
   return ALL;
}

"ex"  {
   printf("scanning EX\n");
   return EX;
}

"top" {
   printf("scanning TOP\n");
   return TOP;
}

"bottom" {
   printf("scanning BOTTOM\n");
   return BOTTOM;
}

"("  {
   printf("scanning opening brackets\n");
   return OPENPAR;
}

")" {
   printf("scanning closing brackets\n");
   return CLOSEPAR;
}

"," {
   printf("scanning comma\n");
   return KOMMA;
}

" " {
     /* DO NOTHING */
}

\n|. { 
        /* do NOTHING */
      }
%%
