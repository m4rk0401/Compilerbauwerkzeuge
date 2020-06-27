#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

int nameCounter = 1;
bool renamed = false;
int nameCounterSkolem = 1;
char * selectedVarFunc;
int indexAllVar = 0;
char * allVar[255];
int excounter = 0;
bool firstknf = false;

/* forward declaration */
formula * nnf(formula * formulaElement);
void eleminationOfArrows(formula * formulaElement);
void deMorganRules(formula * formulaElement);
void eleminationOfDoubleNegation(formula * formulaElement);
void renameRule(formula * formulaElement);
void renameSubFormula(formula * formulaElement, char * selectedVarFunc);
void renameTerm(term * termElement, char * selectedVarFunc);
void renameTermList(termList * termListElement, char * selectedVarFunc);
void skolemizationRule(formula * formulaElement);
void renameSubFormulaSkolem(formula * formulaElement, char * selectedVarFunc);
void renameTermSkolem(term * termElement, char * selectedVarFunc);
void renameTermListSkolem(termList * termListElement, char * selectedVarFunc);
void renameTermListSkolemAll(termList * termListElement, char * selectedVarFunc);
void removeQuantifiers(formula * formulaElement);
void distributiveRule(formula * formulaElement);
void rebuildLeftOr(formula * formulaElement);
void rebuildRightOr(formula * formulaElement);
void rebuildLeftAnd(formula * formulaElement);
void rebuildRightAnd(formula * formulaElement);
knfFormula * convertToKnfFormula(formula * formulaElement);
/*void flatten(formula * formulaElement);
void createknfFormulaLeftAnd(formula * formulaElement);
void createknfFormulaLeftOr(formula * formulaElement);
void createknfFormulaRightAnd(formula * formulaElement);
void createknfFormulaRightOr(formula * formulaElement);*/



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

/* rule to rename variables */
void renameRule(formula * formulaElement)
{
    /* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        renameRule(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        renameRule(formulaElement -> subR);
    }

    /* check for all */
    if(formulaElement -> typS == type_all || formulaElement -> typS == type_ex)
    {
        selectedVarFunc = malloc(strlen(formulaElement -> varfunc) + 1);
        strncpy(selectedVarFunc, formulaElement -> varfunc, strlen(formulaElement -> varfunc) + 1);

        formulaElement -> varfunc = malloc(strlen("  ") + 1);
        snprintf(formulaElement -> varfunc, 3, "v%d", nameCounter);

        if(formulaElement -> list != NULL)
        {
            renameTermList(formulaElement -> list, selectedVarFunc);            
        }

        if(formulaElement -> subL != NULL)
        {
            renameSubFormula(formulaElement -> subL, selectedVarFunc);
        }

        if(formulaElement -> subR != NULL)
        {
            renameSubFormula(formulaElement -> subR, selectedVarFunc);
        }
        nameCounter++;
    }
}

/* function to rename subformula */
void renameSubFormula(formula * formulaElement, char * selectedVarFunc)
{
    if(formulaElement -> varfunc != NULL)
    {
        if(strcmp(formulaElement -> varfunc, selectedVarFunc) == 0)
        {
            formulaElement -> varfunc = malloc(strlen("  ") + 1);
            snprintf(formulaElement -> varfunc, 3, "v%d", nameCounter);
        }
    }
    if(formulaElement -> list != NULL)
    {
        renameTermList(formulaElement -> list, selectedVarFunc);
    }
    if(formulaElement -> subL != NULL)
    {
        renameSubFormula(formulaElement -> subL, selectedVarFunc);
    }
    if(formulaElement -> subR != NULL)
    {
        renameSubFormula(formulaElement -> subR, selectedVarFunc);
    }
}

/* rename term */
void renameTerm(term * termElement, char * selectedVarFunc)
{
    termElement -> varfunc = malloc(strlen("  ") + 1);
    snprintf(termElement -> varfunc, 3, "v%d", nameCounter);

    if(termElement -> list != NULL)
    {
        renameTermList(termElement -> list, selectedVarFunc);
    }
}

/* rename complete term list */
void renameTermList(termList * termListElement, char * selectedVarFunc)
{
    if(strcmp(termListElement -> first -> varfunc, selectedVarFunc) == 0)
    {
        renameTerm(termListElement -> first, selectedVarFunc);
    }
    if(termListElement -> next != NULL)
    {
        renameTermList(termListElement -> next, selectedVarFunc);
    }
}

/* skolemization rule */
void skolemizationRule(formula * formulaElement)
{
    /* check for ex without all*/
    if(formulaElement -> typS == type_ex && indexAllVar == 0)
    {
        selectedVarFunc = malloc(strlen(formulaElement -> varfunc) + 1);
        strncpy(selectedVarFunc, formulaElement -> varfunc, strlen(formulaElement -> varfunc) + 1);

        formula * mainSubFormula = copyFormula(formulaElement -> subL);
        formulaElement -> typS = mainSubFormula -> typS;
        formulaElement -> varfunc = mainSubFormula -> varfunc;
        formulaElement -> list = mainSubFormula -> list;
        formulaElement -> subL = mainSubFormula -> subL;
        formulaElement -> subR = mainSubFormula -> subR;

        if(formulaElement -> list != NULL)
        {
            renameTermListSkolem(formulaElement -> list, selectedVarFunc);
        }
        if(formulaElement -> subL != NULL)
        {
            renameSubFormulaSkolem(formulaElement -> subL, selectedVarFunc);
        }
        if(formulaElement -> subR != NULL)
        {
            renameSubFormulaSkolem(formulaElement -> subR, selectedVarFunc);
        }
        nameCounterSkolem++;
    }
    /* check for all */
    else if(formulaElement -> typS == type_all)
    {
        /* add into array */
        allVar[indexAllVar] = malloc(strlen("  "));
        strcpy(allVar[indexAllVar], formulaElement -> varfunc);
        indexAllVar++;

        if(formulaElement -> subL != NULL)
        {
            skolemizationRule(formulaElement -> subL);
        }
         if(formulaElement -> subR != NULL)
        {
            skolemizationRule(formulaElement -> subR);
        }

        /* remove from array */
        allVar[indexAllVar] = NULL;
        indexAllVar--;
    }
    /* check ex with all */
    else if(formulaElement -> typS == type_ex && indexAllVar != 0)
    {
        excounter++;

        selectedVarFunc = malloc(strlen(formulaElement -> varfunc) + 1);
        strncpy(selectedVarFunc, formulaElement -> varfunc, strlen(formulaElement -> varfunc) + 1);

        formula * mainSubFormula = copyFormula(formulaElement -> subL);
        formulaElement -> typS = mainSubFormula -> typS;
        formulaElement -> varfunc = mainSubFormula -> varfunc;
        formulaElement -> list = mainSubFormula -> list;
        formulaElement -> subL = mainSubFormula -> subL;
        formulaElement -> subR = mainSubFormula -> subR;

        if(formulaElement -> list != NULL)
        {
            renameTermListSkolemAll(formulaElement -> list, selectedVarFunc);
        }
        if(formulaElement -> subL != NULL)
        {
            renameSubFormulaSkolem(formulaElement -> subL, selectedVarFunc);
        }
        if(formulaElement -> subR != NULL)
        {
            renameSubFormulaSkolem(formulaElement -> subR, selectedVarFunc);
        }
    }

    /* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        skolemizationRule(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        skolemizationRule(formulaElement -> subR);
    }
}

/* function to rename subformula for skolem */
void renameSubFormulaSkolem(formula * formulaElement, char * selectedVarFunc)
{
    if(formulaElement -> varfunc != NULL)
    {
        if(strcmp(formulaElement -> varfunc, selectedVarFunc) == 0)
        {
            formulaElement -> varfunc = malloc(strlen("  ") + 1);
            snprintf(formulaElement -> varfunc, 3, "f%d", nameCounterSkolem);
        }
    }
    if(formulaElement -> list != NULL && indexAllVar != 0)
    {
        renameTermListSkolemAll(formulaElement ->list, selectedVarFunc);
    }
    else if(formulaElement -> list != NULL && indexAllVar == 0)
    {
        renameTermListSkolem(formulaElement ->list, selectedVarFunc);
    }
    if(formulaElement -> subL != NULL)
    {
        renameSubFormulaSkolem(formulaElement -> subL, selectedVarFunc);
    }
    if(formulaElement -> subR != NULL)
    {
        renameSubFormulaSkolem(formulaElement -> subR, selectedVarFunc);
    }
}

/* rename term for skolem*/
void renameTermSkolem(term * termElement, char * selectedVarFunc)
{
    termElement -> varfunc = malloc(strlen("  ") + 1);
    snprintf(termElement -> varfunc, 3, "f%d", nameCounterSkolem);

    if(termElement -> list != NULL)
    {
        renameTermListSkolem(termElement -> list, selectedVarFunc);
    }
}

/* rename complete term list for skolem */
void renameTermListSkolem(termList * termListElement, char * selectedVarFunc)
{
    if(strcmp(termListElement -> first -> varfunc, selectedVarFunc) == 0)
    {
        renameTermSkolem(termListElement -> first, selectedVarFunc);
    }
    if(termListElement -> next != NULL)
    {
        renameTermListSkolem(termListElement -> next, selectedVarFunc);
    }
}

/* rename complete term list for skolem */
void renameTermListSkolemAll(termList * termListElement, char * selectedVarFunc)
{
    if(strcmp(termListElement -> first -> varfunc, selectedVarFunc) == 0)
    {
        term * tmpTerm;
        termList * tmpList;
        termList * tmpList2;
        for(int idx = 0; idx < indexAllVar; idx++)
        {
            term * tmpTerm = createTerm(allVar[idx], NULL);

            if(idx == 0)
            {
                tmpList = createTermList(tmpTerm, NULL);
            }
            else
            {
                tmpList = createTermList(tmpTerm, tmpList2);
            }
            tmpList2 = tmpList;
        }
        char * tmpString= malloc(strlen("  ") + 1);
        snprintf(tmpString, 3, "f%d", excounter);

        termListElement -> first = createTerm(tmpString, tmpList);
    }
    if(termListElement -> next != NULL)
    {
        renameTermListSkolemAll(termListElement -> next, selectedVarFunc);
    }
}

/* function to remove all quantifiers */
void removeQuantifiers(formula * formulaElement)
{
	/* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        removeQuantifiers(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        removeQuantifiers(formulaElement -> subR);
    }

    /* check for all and ex */
    if(formulaElement -> typS == type_all || formulaElement -> typS == type_ex)
    {
        formula * mainSubFormula = copyFormula(formulaElement -> subL);
        formulaElement -> typS = mainSubFormula -> typS;
        formulaElement -> varfunc = mainSubFormula -> varfunc;
        formulaElement -> list = mainSubFormula -> list;
        formulaElement -> subL = mainSubFormula -> subL;
        formulaElement -> subR = mainSubFormula -> subR;
	}
}

/* function for distributiv rule */
void  distributiveRule(formula * formulaElement)
{
    /* check for or in typS */
    if(formulaElement -> typS == type_or)
    {
        /* check left formula */
        if(formulaElement -> subL -> typS == type_and && formulaElement -> subR != NULL)
        {
            rebuildLeftOr(formulaElement);
        }
        /* check right formula */
        else if(formulaElement -> subR -> typS == type_and && formulaElement -> subL != NULL)
        {
            rebuildRightOr(formulaElement);
        }
    }
    /* check fir and in typS */
    else if(formulaElement -> typS == type_and)
    {
        /* check left formula */
        if(formulaElement -> subL -> typS == type_or)
        {
            rebuildLeftAnd(formulaElement);
        }
        /* check right formula */
        else if(formulaElement -> subR -> typS == type_or)
        {
            rebuildRightOr(formulaElement);
        }
    }

    if(formulaElement -> subL != NULL)
    {
        distributiveRule(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        distributiveRule(formulaElement -> subR);
    }
}

/* function to rebuild with | as typ and left contains & */
void rebuildLeftOr(formula * formulaElement)
{
    /* duplicate subformulas */
    formula * subL_copy = copyFormula(formulaElement -> subL);
    formula * subR_copy = copyFormula(formulaElement -> subR);

    /* create new formula */
    formulaElement -> typS = type_and;
    formulaElement -> subL = createFormula(type_or, NULL, NULL, subL_copy -> subL, subR_copy);
    formulaElement -> subR = createFormula(type_or, NULL, NULL, subL_copy -> subR, subR_copy);
}

/* function to rebuild with | as typ and right contains & */
void rebuildRightOr(formula * formulaElement)
{
    /* duplicate subformulas */
    formula * subL_copy = copyFormula(formulaElement -> subL);
    formula * subR_copy = copyFormula(formulaElement -> subR);

    /* create new formula */
    formulaElement -> typS = type_and;
    formulaElement -> subL = createFormula(type_or, NULL, NULL, subL_copy, subR_copy -> subL);
    formulaElement -> subR = createFormula(type_or, NULL, NULL, subL_copy, subR_copy -> subR);
}

/* function to rebuild with & as typ and left contains | */
void rebuildLeftAnd(formula * formulaElement)
{
    /* duplicate subformulas */
    formula * subL_copy = copyFormula(formulaElement -> subL);
    formula * subR_copy = copyFormula(formulaElement -> subR);

    /* create new formula */
    formulaElement -> typS = type_or;
    formulaElement -> subL = createFormula(type_and, NULL, NULL, subL_copy -> subL, subR_copy);
    formulaElement -> subR = createFormula(type_and, NULL, NULL, subL_copy -> subR, subR_copy);
}

/* function to rebuild with & as typ and right contains | */
void rebuildRightAnd(formula * formulaElement)
{
    /* duplicate subformulas */
    formula * subL_copy = copyFormula(formulaElement -> subL);
    formula * subR_copy = copyFormula(formulaElement -> subR);

    /* create new formula */
    formulaElement -> typS = type_or;
    formulaElement -> subL = createFormula(type_and, NULL, NULL, subL_copy, subR_copy -> subL);
    formulaElement -> subR = createFormula(type_and, NULL, NULL, subL_copy, subR_copy -> subR);
}

/* function to convert formula to knfFormula */
knfFormula * convertToKnfFormula(formula * formulaElement)
{
    /* variables */
    knfFormula * tmpLeft = (knfFormula*) malloc(sizeof(knfFormula));
    knfFormula * tmpRight = (knfFormula*) malloc(sizeof(knfFormula));

    tmpLeft = NULL;
    tmpRight = NULL;

    /* check for subformula left and right */
    if(formulaElement -> subL != NULL)
    {
        tmpLeft = convertToKnfFormula(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        tmpRight = convertToKnfFormula(formulaElement -> subR);
    }

    /* create subformula */
    knfFormula * tmpFormula = createKnfFormula(formulaElement -> typS, formulaElement -> varfunc, formulaElement -> list, tmpLeft, tmpRight, NULL);

    return tmpFormula;
}

/*void flatten(formula * formulaElement)
{
	if(firstknf == false){
	
		firstknf = true;
		knfformula * temp = createknfFormula(NULL, NULL, NULL, NULL);
		startknfformula = createknfFormula(formulaElement -> typS, formulaElement -> varfunc, formulaElement -> list, temp);
	
	}
	//* check for and in this and one stage beneath left
    if(formulaElement -> typS == type_and && formulaElement -> subL -> typS == type_and)
    {
		
    }
    //* check for or in this and one stage beneath left
    else if(formulaElement -> typS == type_or && formulaElement -> subL -> typS == type_or)
    {
        
    }
    //* check for or in this and one stage beneath right
    else if(formulaElement -> typS == type_and && formulaElement -> subR -> typS == type_and)
    {
        
    }
	//* check for or in this and one stage beneath right
    else if(formulaElement -> typS == type_or && formulaElement -> subR -> typS == type_or)
    {
        
    }
	else
	{
	
		knfformula = createknfFormula(formulaElement -> typS);
		
	}

    //* check for subformula left and right 
    if(formulaElement -> subL != NULL)
    {
        flatten(formulaElement -> subL);
    }
    if(formulaElement -> subR != NULL)
    {
        flatten(formulaElement -> subR);
    }
}*/

/*void createknfFormulaLeftAnd(formula * formulaElement)
{
	knfformula temp1 = createknfFormula(type_and, formulaElement -> subR -> varfunc, formulaElement -> subR -> list, NULL);
	knfformula temp2 = createknfFormula(type_and, formulaElement -> subL -> subR -> varfunc, formulaElement -> subL -> subR -> list, temp1);
	knfformula temp3 = createknfFormula(type_and, formulaElement -> subL -> subL -> varfunc, formulaElement -> subL -> subL -> list, temp2);
	
	
	
}


void createknfFormulaLeftOr(formula * formulaElement)
{
	
	
	
	
	
}

void createknfFormulaRightAnd(formula * formulaElement)
{
	
	
	
	
	
}

void createknfFormulaRightOr(formula * formulaElement)
{
	
	
	
	
	
}*/