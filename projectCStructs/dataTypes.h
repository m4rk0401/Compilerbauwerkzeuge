#include <stdlib.h>

typedef struct term term;
typedef struct termList termList;
typedef struct formula formula;

enum typ
{
    type_atom,
    type_and,
    type_or,
    type_not,
    type_implication,
    type_equivalence,
    type_ex,
    type_all,
    type_top,
    type_bottom
};

struct term
{
    char * varfunc;
    termList * list;
};

struct termList
{
    term * first;
    struct termList * next;
};

struct formula
{
    enum typ typS;
    char * varfunc;
    termList * list;
    formula * subL;
    formula * subR;
};

term * createTerm(char * varfunc, termList * tL);
termList * createTermList(term * termElement, termList * termListElement);
formula * createFormula(enum typ typs, char * varName, termList * listElement, formula * subLElement, formula * subRElement);


/* print functions */
void printTerm(term * termElement);
void printTermList(termList * termListElement);
void printFormula(formula * formulaElement);

/*
termList * addElement(termList * list, term * term)
{
    term -> next = list->first;
    list -> first = term;
    return list;
}*/

/*
termList * deleteElement(termList * list, termList * tL)
{
    termList * tmp = list;

    if(tL -> first -> varfunc == tmp -> first -> varfunc)
    {
        tmp = tmp -> next;
        return list;
    }

    while(tmp -> next != NULL)
    {
        if(tL -> first -> varfunc == tmp -> first -> varfunc)
        {
            tmp = tmp -> next;
            return list;
        }
        tmp = tmp -> next;
    }
    return list;
}
*/

/* create term */
term * createTerm(char * varfunc, termList * tL)
{
    term * term_new = (term*) malloc(sizeof(term));

    if(term_new == NULL)
    {
        return NULL;
    }

    term_new -> varfunc = varfunc;
    term_new -> list = tL;

    return term_new;
}

/* create termlist */
termList * createTermList(term * termElement, termList * termListElement)
{
    termList * termList_new = (termList*) malloc(sizeof(termList));

    if(termList_new == NULL)
    {
        return NULL;
    }

    termList_new -> first = termElement;
    termList_new -> next = termListElement;

    return termList_new;
}

/* create formula */
formula * createFormula(enum typ pTypS, char * pvarfunc, termList * listElement, formula * subLElement, formula * subRElement)
{
    formula * formula_new = (formula*) malloc(sizeof(formula));

    if(formula_new == NULL)
    {
        return NULL;
    }

    formula_new -> typS = pTypS;
    formula_new -> varfunc = pvarfunc;
    formula_new -> list = listElement;
    formula_new -> subL = subLElement;
    formula_new -> subR = subRElement;

    return formula_new;
}



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

/* print formula */
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