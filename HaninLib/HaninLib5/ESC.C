#include "hanin.h"

/********************************************************************/
/*	Function   : scroll					    */
/*	Synopsis   : scroll one character into operation system     */
/*	Return	   : None					    */
/********************************************************************/
void	scroll()
{
	fillback(0, 0);
	pWA->cchResult = 2;
	pWA->chResult[0] = youwant[0];
	pWA->chResult[1] = youwant[1];
	pWA->chResult[2] = 0;

	lshift_data(2);
	j_off -= 2;
	f_want -= 2;
	if (undoStart > 0)
	    undoStart -= 2;
	else
	    undoLen = 0;

	words_dis(W_START, youwant, j_off, NOR);
	clr_line((short) (W_START+j_off), (short) (W_START+j_off+8));
}

void	fillback(short j, short i)
{
	unsigned short temp;

	if (TCAFLAG == 0) 
	{   /* TCAFLAG: 0(TCA code), 1(BIG5 code) */
		temp = (youwant[i]<<8)+youwant[i+1];
		temp = codeconv(temp);
		youwant[i] = (temp&0xff00)>>8;
		youwant[i+1] = temp&0xff;
	}
}

