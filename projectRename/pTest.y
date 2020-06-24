%{
    #include <stdio.h>
    #include <string.h>
    #include "dataTypes.h"
    #include "copy.h"
    #include "print.h"
    #include "normalize.h"

    // Declare stuff from Flex that Bison needs to know about:
    extern FILE *yyin;
    extern int yyparse();
    extern int yyerror(char* err);
    extern int yylex(void);

    /* create formula */
    formula * formula_input;
%}

%union {
  char *val;
  struct term * term;
  struct termList * termList;
  struct formula * formula;
}

%start start

%token OPENPAR CLOSEPAR
%token KOMMA
%token NEWLINE

%left EQU
%left IMP
%left OR
%left AND
%precedence NOT

%left ALL EX
%precedence PRAEDICATE
%precedence FUNCTION
%precedence VARIABLE TOP BOTTOM

%%

start: formula {
      formula_input = $<formula>1;
}

/* declaring term: x or f or f(termList) */
term: VARIABLE {
        $<term>$ = createTerm($<val>1, NULL);
        printf("reducing variable to term ");
        printTerm($<term>$);
        printf("\n");
      }
    | FUNCTION {
        $<term>$ = createTerm($<val>1, NULL);
        printf("reducing to term ");
        printTerm($<term>$);
        printf("\n");
      }
    | FUNCTION OPENPAR termList CLOSEPAR {
        $<term>$ = createTerm($<val>1, $<termList>3);
        printf("reducing to term ");
        printTerm($<term>$);
        printf("\n");
      };
	  
    
/* declaring termList: (x) or (f) or (f(x,...),...) */
termList: term {
        $<termList>$ = createTermList($<term>1, NULL);
        printf("reducing last term to term sequence ");
        printTermList($<termList>$);
        printf("\n");
      }
    | term KOMMA termList {
        $<termList>$ = createTermList($<term>1, $<termList>3);
        printf("reducing to term sequence ");
        printTermList($<termList>$);
        printf("\n");
      };

/* declaring formula: */
formula: PRAEDICATE {
        $<formula>$ = createFormula(type_atom, $<val>1, NULL, NULL, NULL);
        printf("reducing to atom ");
        printFormula($<formula>$);
        printf("\n");
        }
    | PRAEDICATE OPENPAR termList CLOSEPAR {
        $<formula>$ = createFormula(type_atom, $<val>1, $<termList>3, NULL, NULL);
        printf("reducing to atom ");
        printFormula($<formula>$);
        printf("\n");
        }
    | TOP {
        $<formula>$ = createFormula(type_top, NULL, NULL, NULL, NULL);
        printf("reducing to boolean constant TOP\n");
        }
    | BOTTOM {
        $<formula>$ = createFormula(type_bottom, NULL, NULL, NULL, NULL);
        printf("reducing to boolean constant BOTTOM\n");
        } 
    | ALL VARIABLE formula {
        $<formula>$ = createFormula(type_all, $<val>2, NULL, $<formula>3, NULL);
        printf("reducing to all formula ");
        printFormula($<formula>$);
        printf("\n");
      }
    | EX VARIABLE formula {
        $<formula>$ = createFormula(type_ex, $<val>2, NULL, $<formula>3, NULL);
        printf("reducing to existential formula ");
        printFormula($<formula>$);
        printf("\n");
      }
    | formula AND formula {
        $<formula>$ = createFormula(type_and, NULL, NULL, $<formula>1, $<formula>3);
        printf("reducing to conjunction ");
        printFormula($<formula>$);
        printf("\n");
      }
    | formula OR formula {
        $<formula>$ = createFormula(type_or, NULL, NULL, $<formula>1, $<formula>3);
        printf("reducing to disjunction ");
        printFormula($<formula>$);
        printf("\n");
      }
    | formula IMP formula {
        $<formula>$ = createFormula(type_implication, NULL, NULL, $<formula>1, $<formula>3);
        printf("reducing to implication ");
        printFormula($<formula>$);
        printf("\n");
      }
    | formula EQU formula {
        $<formula>$ = createFormula(type_equivalence, NULL, NULL, $<formula>1, $<formula>3);
        printf("reducing to equivalence ");
        printFormula($<formula>$);
        printf("\n");
      }
    | NOT formula {
        $<formula>$ = createFormula(type_not, NULL, NULL, $<formula>2, NULL);
        printf("reducing to negation ");
        printFormula($<formula>$);
        printf("\n");
      }
    | OPENPAR formula CLOSEPAR {
        $<formula>$ = $<formula>2;
        printf("reducing to brackets ");
        printFormula($<formula>$);
        printf("\n");
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
    
  yyparse();

  formula * formula_withoutArrows = copyFormula(formula_input);
  formula * formula_nnf =           copyFormula(formula_input);
  formula * formula_empty;

  printf("storing results.\n\n");

  printf("Original formula:\n");
  originalPrint(formula_input, 0);
  printf("\n");

  printf("Without arrows:\n");
  eleminationOfArrows(formula_withoutArrows);
  originalPrint(formula_withoutArrows, 0);
  printf("\n");

  printf("NNF:\n");
  nnf(formula_input);
  originalPrint(formula_input, 0);
  printf("\n");

  printf("Renamed variables:\n");
  renameRule(formula_input);
  printf("\n");
  originalPrint(formula_input, 0);
}

