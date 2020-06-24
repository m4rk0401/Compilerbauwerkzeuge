  /* Find and add all integers and floats... */

%option noyywrap


%{
   int    intval   = 0;
   double floatval = 0.0;
%}
 
VAR            [n-z][0-9]*
FUNC           [a-m][0-9]*
PRAEDICATE      [A-Z][0-9]*

%%

{VAR} {
   printf("scanned variable %s\n", yytext);
   /* return VAR; */
}

{FUNC} {
   printf("scanned function %s\n", yytext);
   /* return FUNC; */
}

{PRAEDICATE} {
   printf("scanned praedicate %s\n", yytext);
   /* return PRAEDICATE */
}

"&" {
   printf("scanned AND\n");
   /* return AND; */
}

"|"  {
   printf("scanned OR\n");
   /* return OR; */
}

"~" {
   printf("scanned NOT\n");
   /* return NOT; */
}

"->" {
   printf("scanned IMPLICATION\n");
   /* return IMP; */
}

"<->"  {
   printf("scanned EQUIVALENCE\n");
   /* return BIIMP; */
}

"all" {
   printf("scanned ALL\n");
   /* return ALL; */
}

"ex"  {
   printf("scanned EX\n");
   /* return EX; */
}

"top" {
   printf("scanned TOP\n");
   /* return TOP; */
}

"bottom" {
   printf("scanned BOTTOM\n");
   /* return BOT; */
}

"("  {
   printf("scanned opening brackets\n");
   /* return OPENPAR; */
}

")" {
   printf("scanned closing brackets\n");
   /* return CLOSEPAR; */
}

"," {
   printf("scanned comma\n");
   /* return KOMMA; */
}

" " {
     /* DO NOTHING */
}

\n|. { 
        /* do NOTHING */
      }
%%

int main( int argc, char **argv )       
{
   ++argv, --argc;  /* skip over program name */
   if ( argc > 0 )
      yyin = fopen( argv[0], "r" );
   else
      yyin = stdin;
   
   yylex();
}
