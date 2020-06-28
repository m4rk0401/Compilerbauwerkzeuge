#include <stdlib.h>

typedef struct term term;
typedef struct termList termList;
typedef struct formula formula;
typedef struct knfFormula knfFormula;

enum typ
{
    type_null,
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

struct knfFormula
{
	enum typ typS;
    char * varfunc;
    termList * list;
    knfFormula * subL;
    knfFormula * subR;
    knfFormula * next;	
};

/* forward declaration */
term * createTerm(char * varfunc, termList * tL);
termList * createTermList(term * termElement, termList * termListElement);
formula * createFormula(enum typ typs, char * varName, termList * listElement, formula * subLElement, formula * subRElement);
knfFormula * createKnfFormula(enum typ pTypS, char * pvarfunc, termList * listElement, knfFormula * subL, knfFormula * subR, knfFormula * next);

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

/* create knf formula */
knfFormula * createKnfFormula(enum typ pTypS, char * pvarfunc, termList * listElement, knfFormula * pSubL, knfFormula * pSubR, knfFormula * pNext)
{
    knfFormula * knfFormula_new = (knfFormula*) malloc(sizeof(knfFormula));

    if(knfFormula_new == NULL)
    {
        return NULL;
    }

    knfFormula_new -> typS = pTypS;
    knfFormula_new -> varfunc = pvarfunc;
    knfFormula_new -> list = listElement;
    knfFormula_new -> subL = pSubL;
    knfFormula_new -> subR = pSubR;
    knfFormula_new -> next = pNext;

    return knfFormula_new;
}