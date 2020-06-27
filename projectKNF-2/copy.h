#include <stdlib.h>

/* forward declaration */
term * copyTerm(term * termElement);
termList * copyTermList(termList * termListElement);
formula * copyFormula(formula * formulaElement);
knfFormula * copyKnfFormula(knfFormula * formulaElement);

/* create copy of term */
term * copyTerm(term * termElement)
{
    if(termElement == NULL)
    {
        return NULL;
    }
    
    term * term_new = (term*) malloc(sizeof(term));

    if(term_new == NULL)
    {
        return NULL;
    }

    term_new -> varfunc = termElement -> varfunc;
    term_new -> list = copyTermList(termElement -> list);

    return term_new;
}

/* create copy of termList */
termList * copyTermList(termList * termListElement)
{
    if(termListElement == NULL)
    {
        return NULL;
    }

    termList * termList_new = (termList*) malloc(sizeof(termList));

    if(termList_new == NULL)
    {
        return NULL;
    }

    termList_new -> first = copyTerm(termListElement -> first);
    termList_new -> next = copyTermList(termListElement -> next);

    return termList_new;
}

/* create copy of formula */
formula * copyFormula(formula * formulaElement)
{
    if(formulaElement == NULL)
    {
        return NULL;
    }
    
    formula * formula_new = (formula*) malloc(sizeof(formula));

    if(formula_new == NULL)
    {
        return NULL;
    }

    formula_new -> typS = formulaElement -> typS;
    formula_new -> varfunc = formulaElement -> varfunc;
    formula_new -> list = copyTermList(formulaElement -> list);
    formula_new -> subL = copyFormula(formulaElement -> subL);
    formula_new -> subR = copyFormula(formulaElement -> subR);

    return formula_new;
}

/* create copy of knf formula */
knfFormula * copyKnfFormula(knfFormula * formulaElement)
{
    if(formulaElement == NULL)
    {
        return NULL;
    }
    
    knfFormula * formula_new = (knfFormula*) malloc(sizeof(knfFormula));

    if(formula_new == NULL)
    {
        return NULL;
    }

    formula_new -> typS = formulaElement -> typS;
    formula_new -> varfunc = formulaElement -> varfunc;
    formula_new -> list = copyTermList(formulaElement -> list);
    formula_new -> subL = copyKnfFormula(formulaElement -> subL);
    formula_new -> subR = copyKnfFormula(formulaElement -> subR);
    formula_new -> next = copyKnfFormula(formulaElement -> next);

    return formula_new;
}
