#include "hanin.h"
#include "juitable.h"

char key2index[] = {
/* space */  41, 27, 43, 29, 30, 31, 33, 43, 35, 26, 34, 42, 36, 40, 37, 39,
/* 0...9 */  26, 27, 28, 29, 30, 31, 32, 33, 34, 35,
/* :;<=> */  38, 38, 36, 42, 37, 39, 28,
/* A...Z */   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
	     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
/* [\]^_ */  44, 99, 99, 32, 40, 99,
/* a...z */   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12,
	     13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
/* {|}~  */  44, 99, 99, 99 };

/********************************************************************/
/*	Function   : ChuinMatch 				    */
/*	Synopsis   : convert the chuin key into chuin symbol	    */
/*	Argument   : None					    */
/*	Return	   : 1,2,3,4(the level of zhuin symbol), 5(error)   */
/********************************************************************/
short	ZhuinMatch()
{
    short i, j, op;
    short level, preLevel;
    char code;

	i=key2index[inkey-' '];
	code=jui_iner[i];
	level=Code2Level(code);
	
    if (inkey < ' ' || inkey > '~' ||
       i == 99 		||                   	/* unused key */
       code == 99 	||			   		/* unused code */
       (level == 4 && ownoff == 0))	   /* tone only */
	return(5);

    for (i=0, preLevel=4; i<ownoff; i++) {
	preLevel = Code2Level(distmp[i]);
	if (level <= preLevel) break;
    }
    op = SP_INSERT | SP_REPLACE | i+1;
    if (i < ownoff && level != preLevel) {
	op &= ~SP_REPLACE;
	for (j=ownoff; j>i; j--) {
	    nowkey_buf[j] = nowkey_buf[j-1];
	    distmp[j] = distmp[j-1];
	}
    }
    nowkey_buf[i] = inkey;
    if (i == ownoff || level < preLevel) ownoff++;
    show_pinin(op, code);
    return(level);
}
