#include <stdlib.h>

/* forward declarations */
void printTerm(term * termElement);
void printTermList(termList * termListElement);
void printFormula(formula * formulaElement);
const char * createSpaces(int spaces);
void originalPrint(formula * formulaElement, int spaces);

/* print term */
void printTerm(term * termElement)
{
    printf("%s", termElement -> varfunc);

    if(termElement -> list != NULL)
    {
        printf("(");
        printTermList(termElement -> list);
        printf(")");
    }
}

/* print termList */
void printTermList(termList * termListElement)
{
    printTerm(termListElement -> first);

    if(termListElement -> next != NULL)
    {
        printf(",");
        printTermList(termListElement -> next);
    }
}

/* print formula depending on type */
void printFormula(formula * formulaElement)
{
    switch(formulaElement -> typS)
    {
    case type_atom:
      printf("%s", formulaElement -> varfunc);
      if (formulaElement -> list != NULL)
      {
        printf("(");
        printTermList(formulaElement -> list);
        printf(")");
      }
      break;
    case type_and:
    case type_or:
    case type_implication:
    case type_equivalence:
      printf("(");
      printFormula(formulaElement -> subL);

      switch(formulaElement -> typS)
      {
        case type_and:
            printf(" AND ");
            break;
        case type_or:
            printf(" OR ");
            break;
        case type_implication:
            printf(" -> ");
            break;
        case type_equivalence:
            printf(" <-> ");
            break;
      }

      printFormula(formulaElement -> subR);
      printf(")");
      break;
    case type_not:
      printf("(");
      printf("NOT ");
      printFormula(formulaElement -> subL);
      printf(")");
      break;
    case type_all:
    case type_ex:
      printf("(");
      
      switch(formulaElement -> typS)
      {
        case type_all:
            printf("ALL");
            break;
        case type_ex:
            printf("EX");
            break;
      }

      printf(" %s ", formulaElement -> varfunc);
      printFormula(formulaElement -> subL);
      printf(")");
      break;
    case type_top:
    case type_bottom:
      switch(formulaElement -> typS)
      {
        case type_top:
            printf("TOP");
            break;
        case type_bottom:
            printf("BOTTOM");
            break;
      }

      break;
  }
}