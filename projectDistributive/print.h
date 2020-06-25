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

/* function to calculate spaces */
const char * createSpaces(int spaces)
{
  char * spaces_new = (char*) malloc(spaces*sizeof(char));

  for(int i=0; i<spaces; i++)
  {
    strcat(spaces_new, "  ");
  }

  return spaces_new;
}

/* function to print the with identation depending on type */
void originalPrint(formula * formulaElement, int spaces)
{
  if(formulaElement == NULL)
  {
    return;
  }

  char * typeChar = "";
  switch(formulaElement -> typS)
  {
    case type_atom:
      printf("%s%s", createSpaces(spaces), formulaElement -> varfunc);
      if (formulaElement -> list != NULL)
      {
        printf("(");
        printTermList(formulaElement -> list);
        printf(")\n");
      }
      break;

    case type_and:
    case type_or:
    case type_implication:
    case type_equivalence:
      switch(formulaElement -> typS)
      {
        case type_and:
            typeChar = "AND";
            break;
        case type_or:
            typeChar = "OR";
            break;
        case type_implication:
            typeChar = "IMPLIES";
            break;
        case type_equivalence:
            typeChar = "EQUIV";
            break;
      }

      printf("%s%s\n", createSpaces(spaces), typeChar);
      originalPrint(formulaElement -> subL, spaces+1);
      originalPrint(formulaElement -> subR, spaces+1);
      break;
    
    case type_not:
      printf("%s%s\n", createSpaces(spaces), "NOT");
      originalPrint(formulaElement -> subL, spaces+1);
      break;

    case type_all:
    case type_ex:
      switch(formulaElement -> typS)
      {
        case type_all:
            typeChar = "ALL";
            break;
        case type_ex:
            typeChar = "EX";
            break;
      }

      printf("%s%s %s\n", createSpaces(spaces), typeChar, formulaElement -> varfunc);
      originalPrint(formulaElement -> subL, spaces+1);
      break;

    case type_top:
    case type_bottom:
      switch(formulaElement -> typS)
      {
        case type_top:
            typeChar = "TOP";
            break;
        case type_bottom:
            typeChar = "BOTTOM";
            break;
      }

      printf("%s%s\n", createSpaces(spaces), typeChar);
      break;
  }
}
