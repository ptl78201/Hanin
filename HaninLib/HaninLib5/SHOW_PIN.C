#include "hanin.h"

char juin_symbol[] = {	  /* low byte of BIG5 code */
	0x74,0x75,0x76,0x77,0x78,0x79,0x7a,0x7b,
	0x7c,0x7d,0x7e,0xa1,0xa2,0xa3,0xa4,0xa5,
	0xa6,0xa7,0xa8,0xa9,0xaa,    /* level 1 */
	0xb8,0xb9,0xba, 	     /* level 2 */
	0xab,0xac,0xad,0xae,0xaf,0xb0,0xb1,0xb2,
	0xb3,0xb4,0xb5,0xb6,0xb7,    /* level 3 */
	0xbc,0xbd,0xbe,0xbf,0xbb     /* level 4 */
};

/********************************************************************/
/*	Function   : show_pinin 				    */
/*	Synopsis   : display the chuin symbols			    */
/*	Argument   : mm : 0 - append after current pos		    */
/*			  1 - replace current pos		    */
/*			  2 - replace pos before current pos	    */
/*		     code : the code of zhuin symbol		    */
/*	Return	   : 5 -- error 				    */
/*		     0 -- OK					    */
/********************************************************************/
void	show_pinin(short mode, char code)
{
    BYTE	sym[10];
    short	i, j, len, pos;

    if (mode & (SP_INSERT|SP_REPLACE)) 
	{
		i = (mode & SP_POS) - 1;
		distmp[i] = code;
    }

    if (code < 37) 
	{
		if (DisplayMode == DISPLAY_ALPHA && nowkey_buf[0] != SPEC_CHAR) 
		{
			if (mode & SP_INSERT) 
			{
				pos = f_want + PininPos + 1;
				put_ch((short) (W_START+pos-1), nowkey_buf[PininPos], HILITE);
			}
			else 
			{
				pos = j_off + PininPos + 1;
				clr_line((short) (W_START+pos), (short) (W_START+pos+4));	/* clear first */
				pos = f_want + PininPos + 1;
			}
		}
		else 
		{
			if (mode & (SP_INSERT|SP_REPLACE)) 
			{
				len = (mode & SP_REPLACE) ? 1 : ownoff-i;
				if (nowkey_buf[0] != SPEC_CHAR) 
				{
					for (j=0; j<len; j++) 
					{
					sym[j<<1] = 0xa3;
					sym[(j<<1)+1] = juin_symbol[distmp[i+j]];
					}
				}
				else 
				{
					sym[0] = 0xb2;
					sym[1] = 0xc5;
				}
				words_dis((short) (W_START+f_want+(i<<1)), sym, (short) (len<<1), HILITE);
				pos = f_want + (ownoff<<1);
			}
			else 
			{
				pos = j_off + (ownoff<<1);
				clr_line((short) (W_START+pos), (short) (W_START+pos+4));	/* clear first */
				pos = f_want + (ownoff<<1);
			}
		}

		words_dis((short) (W_START+pos), youwant+f_want, (short) (j_off-f_want), NOR);
    }
}

