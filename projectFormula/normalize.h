#include <stdlib.h>

/* forward declaration */
formula * nnf(formula * formulaElement);
void eleminationOfArrows(formula * formulaElement);
void deMorganRules(formula * formulaElement);
void eleminationOfDoubleNegation( formula * formulaElement);

/* apply operations */
formula * nnf(formula * formulaElement)
{
    eleminationOfArrows(formulaElement);
    deMorganRules(formulaElement);
    eleminationOfDoubleNegation(formulaElement);
}

/* eleminate all arrows in formula */
void eleminationOfArrows(formula * formulaElement)
{
    /* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        eleminationOfArrows(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        eleminationOfArrows(formulaElement -> subR);
    }

    /* check for implication */
    if(formulaElement -> typS == type_implication)
    {
        formulaElement -> typS = type_or;
        formulaElement -> subL = createFormula(type_not, NULL, NULL, formulaElement -> subL, NULL);
    }
    else if(formulaElement -> typS == type_equivalence)
    {
        /* duplicate subformulas */
        formula * subL_copy = copyFormula(formulaElement -> subL);
        formula * subR_copy = copyFormula(formulaElement -> subR);

        /* negotiate subformulas */
        formula * subL_not = createFormula(type_not, NULL, NULL, subL_copy, NULL);
        formula * subR_not = createFormula(type_not, NULL, NULL, subR_copy, NULL);

        /* combine new formulas */
        formulaElement -> typS = type_or;
        formulaElement -> subL = createFormula(type_and, NULL, NULL, formulaElement -> subL, formulaElement -> subR);
        formulaElement -> subR = createFormula(type_and, NULL, NULL, subL_not, subR_not);
    }
}

/* apply De-Morgan-Rules */
void deMorganRules(formula * formulaElement)
{
    /* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        deMorganRules(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        deMorganRules(formulaElement -> subR);
    }

    /* check for not */
    if(formulaElement -> typS == type_not)
    {
        formula * mainSubFormula = copyFormula(formulaElement -> subL);

        /* NOT (subL AND subR) -> NOT subL OR NOT subR */
        if(mainSubFormula -> typS == type_and)
        {
            formulaElement -> subL = createFormula(type_not, NULL, NULL, mainSubFormula -> subL, NULL);
            formulaElement -> subR = createFormula(type_not, NULL, NULL, mainSubFormula -> subR, NULL);
            formulaElement -> typS = type_or;
            deMorganRules(formulaElement -> subL);
            deMorganRules(formulaElement -> subR);
        }

        /* NOT (subL OR subR) -> NOT subL and NOT subR */
        else if(mainSubFormula -> typS == type_or)
        {
            formulaElement -> subL = createFormula(type_not, NULL, NULL, mainSubFormula -> subL, NULL);
            formulaElement -> subR = createFormula(type_not, NULL, NULL, mainSubFormula -> subR, NULL);
            formulaElement -> typS = type_and;
            deMorganRules(formulaElement -> subL);
            deMorganRules(formulaElement -> subR);
        }

        /* NOT(ALL var subL) -> EX var NOT subL */
        else if(mainSubFormula -> typS == type_all)
        {
            formulaElement -> typS = type_ex;
            formulaElement -> varfunc = mainSubFormula -> varfunc;
            formulaElement -> subL = createFormula(type_not, NULL, NULL, mainSubFormula -> subL, NULL);
            deMorganRules(formulaElement -> subL);
        }

        /* NOT(EX var subL) -> ALL var NOT subL */
        else if(mainSubFormula -> typS == type_ex)
        {
            formulaElement -> typS = type_all;
            formulaElement -> varfunc = mainSubFormula -> varfunc;
            formulaElement -> subL = createFormula(type_not, NULL, NULL, mainSubFormula -> subL, NULL);
            deMorganRules(formulaElement -> subL);
        }
    }
}

/* eleminate double negation */
void eleminationOfDoubleNegation(formula * formulaElement)
{
    /* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        eleminationOfDoubleNegation(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        eleminationOfDoubleNegation(formulaElement -> subR);
    }

    /* check for negation */
    if(formulaElement -> typS == type_not && formulaElement -> subL -> typS == type_not)
    {
        formula * mainSubFormula = copyFormula(formulaElement -> subL -> subL);
        formulaElement -> typS = mainSubFormula -> typS;
        formulaElement -> varfunc = mainSubFormula -> varfunc;
        formulaElement -> list = mainSubFormula -> list;
        formulaElement -> subL = mainSubFormula -> subL;
        formulaElement -> subR = mainSubFormula -> subR;
    } 

    /* check for NOT TOP */
    else if(formulaElement -> typS == type_not && formulaElement -> subL -> typS == type_top)
    {
        formulaElement -> typS = type_bottom;
        formulaElement -> subL = NULL;
    }

    /* check for NOT BOTTOM */
    else if(formulaElement -> typS == type_not && formulaElement -> subL -> typS == type_bottom)
    {
        formulaElement -> typS = type_top;
        formulaElement -> subL = NULL;
    }
}
