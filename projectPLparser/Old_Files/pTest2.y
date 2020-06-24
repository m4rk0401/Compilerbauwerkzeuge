%{
    #include <stdio.h>
    #include <string.h>
    // Declare stuff from Flex that Bison needs to know about:
    extern FILE *yyin;
    extern int yyparse();
    extern int yyerror(char* err);
    extern int yylex(void);
%}

%union {
  int val;
  char *sval;
}

%start stmtseq

%token OPENPAR CLOSEPAR
%token KOMMA
%token NEWLINE

%left BIIMP
%left IMP
%left OR
%left AND
%precedence NOT

%left ALL EX
%precedence PRAEDICATE
%precedence FUNCTION
%precedence VARIABLE TOP BOTTOM

%%

stmtseq: /* Empty */
    | NEWLINE stmtseq       {}
    | stmt  NEWLINE stmtseq {}
    | error NEWLINE stmtseq {};  /* After an error, start afresh */

/* Declaring Passable stmts */
stmt: formula  

/* declaring term: x or f or f(termArgs) */
term: VARIABLE {
        printf("reducing variable %s to term\n", $<sval>1);
      }
    | FUNCTION {
        printf("reducing to term %s\n", $<sval>1);
      }
    | FUNCTION OPENPAR termArgs CLOSEPAR {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, $<sval>1); 
        strcat(x, "("); 
        strcat(x, $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to term %s\n", $<sval>$);
      };
	  
    
/* declaring termArgs: (x) or (f) or (f(x,...),...) */
termArgs: term {
        $<sval>$ = $<sval>1;
        printf("reducing last term %s to term sequence\n", $<sval>$);
      }
    | term KOMMA termArgs {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, $<sval>1);
        strcat(x, ","); 
        strcat(x,  $<sval>3);
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing term %s to term sequence\n", $<sval>$);
      };
	  
/* declaring atom: P or P(predArgs) */
atom: PRAEDICATE {
	$<sval>$ = $<sval>1;
        printf("reducing to atom %s\n", $<sval>$);
      }
    | PRAEDICATE OPENPAR predArgs CLOSEPAR {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, $<sval>1); 
        strcat(x, "("); 
        strcat(x, $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to atom %s\n", $<sval>$);
      };


/* declaring predArgs: term */
predArgs: term {
        $<sval>$ = $<sval>1;
      }
    | term KOMMA predArgs {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, $<sval>1);
        strcat(x, ","); 
        strcat(x,  $<sval>3);
        $<sval>$ = strdup(x);
        free(x);
      };

/* declaring formula: */
formula: atom {printf("reducing atom %s to formula\n", $<sval>1);}    
    | TOP {
        $<sval>$ = " TOP";
        printf("reducing boolean TOP %s to formula\n", $<sval>$);
        }
    | BOTTOM {
        $<sval>$ = " BOTTOM";
        printf("reducing boolean BOT %s to formula\n", $<sval>$);
        } 
    | ALL VARIABLE formula {
        char * x = malloc((strlen($<sval>2) + strlen($<sval>3) + 20)* sizeof(char));
        strcat(x, "(ALL ");
        strcat(x, $<sval>2); 
        strcat(x, $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to all formula %s\n", $<sval>$);
      }
    | EX VARIABLE formula {
        char * x = malloc((strlen($<sval>2) + strlen($<sval>3) + 20)* sizeof(char));
        strcat(x, "(EX ");
        strcat(x, $<sval>2); 
        strcat(x, $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to existential formula %s\n", $<sval>$);
      }
    | formula AND formula {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, "(");
        strcat(x, $<sval>1);
        strcat(x, " AND "); 
        strcat(x,  $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to conjunction %s\n", $<sval>$);
      }
    | formula OR formula {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, "(");
        strcat(x, $<sval>1);
        strcat(x, " OR "); 
        strcat(x,  $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to disjunction %s\n", $<sval>$);
      }
    | formula IMP formula {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, "(");
        strcat(x, $<sval>1);
        strcat(x, " IMP "); 
        strcat(x,  $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to implication %s\n", $<sval>$);
      }
    | formula BIIMP formula {
        char * x = malloc((strlen($<sval>1) + 20 + strlen($<sval>3))* sizeof(char));
        strcat(x, "(");
        strcat(x, $<sval>1);
        strcat(x, " EQUIV "); 
        strcat(x,  $<sval>3);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing to equivalence %s\n", $<sval>$);
      }
    | NOT formula {
        char * x = malloc((strlen($<sval>2) + 20)* sizeof(char));
        strcat(x, "(NOT ");
        strcat(x, $<sval>2);
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x); 
        printf("reducing to neagtion %s\n", $<sval>$);
      }
    | OPENPAR formula CLOSEPAR {
        char * x = malloc((strlen($<sval>2) + 20)* sizeof(char));
        strcat(x, "(");
        strcat(x, $<sval>2); 
        strcat(x, ")");
        $<sval>$ = strdup(x);
        free(x);
        printf("reducing %s to formula\n", $<sval>$);
      }
%%

int yyerror(char* err)
{
   printf("Error: %s\n", err);
   return 0;
}


int main (int argc, char* argv[])
{  
   ++argv, --argc;  /* skip over program name */
   if ( argc > 0 )
      yyin = fopen( argv[0], "r" );
   else
      yyin = stdin;
    
  return yyparse();
}
