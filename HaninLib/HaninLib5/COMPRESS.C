#include "hanin.h"

void	RecoverZhuin(short loc);

/********************************************************************/
/*	Function   : CompressZhuin				    */
/*	Synopsis   : compress the inner code			    */
/*	Argument   : zCode: inner code buffer			    */
/*		     cmpCode: store the compress code		    */
/*	Return	   : None					    */
/********************************************************************/
void	CompressZhuin(PBYTE zCode, PBYTE cmpCode)
{
    short i;

    i = 0;
    cmpCode[0] = cmpCode[1] = 33;
    if (zCode[i] >= 0 && zCode[i] <= 0x14)
	cmpCode[0] += zCode[i++] * 4;
    else
	cmpCode[0] += 84;
    if (zCode[i] > 0x14 && zCode[i] <= 0x17)
	cmpCode[0] += zCode[i++] - 0x14;
    if (zCode[i] > 0x17 && zCode[i] <= 0x24)
	cmpCode[1] += (zCode[i++] - 0x17) * TONE_COUNT;
    cmpCode[1] += zCode[i] - 0x25;
}

void	RecoverZhuin(short loc)
{
	short  i, k;

	for (i=0; i<10; i++)
	    nowkey_buf[i] = Core[loc].nowkey[i];

	Handelete(0);

	ownoff = 0;
	PininPos = -1;

	for (i=k=0; k==0; ) 
	{
	    inkey = nowkey_buf[i++];
	    k = match();
	}
	if (ownoff > 0) ownoff--;
	if (PininMode && nowkey_buf[0] != SPEC_CHAR && PininPos >=0) PininPos--;
}
