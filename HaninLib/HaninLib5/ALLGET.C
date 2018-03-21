#include "hanin.h"


/********************************************************************/
/*	Function   : SearchAllChars				    */
/*	Synopsis   : search all homophone chars 		    */
/*	Argument   : None					    */
/*	Return	   : the number of bytes in searching buffer	    */
/********************************************************************/
short	SearchAllChars()
{
	short	n;
    short	j, k;

    LDSearchWord((short) f_want, 2, SEARCH_ALL);    /* search LD */

	for (n = 1; n <= MAXPROFDICOPENED; n++)
		MDSearchChar(n, (short) f_want, SEARCH_ALL);	    /* search professional dics */
	
	MDSearchChar(0, (short) f_want, SEARCH_ALL);	    /* search MD */

    if (specChoice) 
	{			    /* full special char */
		for (j=k=0; j<chlen; j+=2) 
		{
			if (chbuf[j] < 0xa4) 
			{	    /* is special char? */
				chbuf[k] = chbuf[j];
				chbuf[k+1] = chbuf[j+1];
				k += 2;
			}
		}
		if (k > 0) { chlen=k; chbuf[chlen]='\0'; }
    }
	else
		chlen += GetBig5EChar (&cmpkigo[f_want], &chbuf[chlen], 0);

    return(chlen);
}


/********************************************************************/
/*	Function   : SearchAllWords				    */
/*	Synopsis   : search all homophone words 		    */
/*	Argument   : None					    */
/*	Return	   : the number of bytes in searching buffer	    */
/********************************************************************/
short	SearchAllWords()
{
	short	n;
	
    if (cmpkigo[f_want] == '~' || cmpkigo[f_want+2] == '~')
		return(-1);	/* special code in the word */

	LDSearchWord((short) f_want, 4, SEARCH_ALL);	/* search LD */

    UDSearchWord((short) f_want, 4, SEARCH_ALL);	/* search UD */
	for (n = 1; n <= MAXPROFDICOPENED; n++)
		MDSearchWord(n, (short) f_want, 4, SEARCH_ALL);	/* search professional dics */

	MDSearchWord(0, (short) f_want, 4, SEARCH_ALL);	/* search MD */

    UnitSearchWord((short) f_want, 4, SEARCH_ALL);	/* search Unit table */
    return(chlen);
}

