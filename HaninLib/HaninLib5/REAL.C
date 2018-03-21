#include "hanin.h"

void	init_vars();
void	perform();
void	put_back();
void	chg_key();
short	UnusedKey();
short	others();
void	modify();
void	do_tab();
short	keypad();

Char	scancode;
USHORT  KB_STATUS=0;

/********************************************************************/
/*	Function   : realtime					    */
/*	Synopsis   : The entry of C programs.			    */
/*		     Parsing the function key.			    */
/*	Argument   : None					    */
/*	Return	   : None					    */
/********************************************************************/
short	realtime()
{
	inkey_bak = 0;

	if (inkey == 0 && scancode == 0) 
		return 1;

	if (inkey && scancode) 
		return 0;

	if (InputState == IS_PASS)
	{
		put_back();
		return(1);
	}

    if (InputState != IS_FULLABC) 
    {
    	if ((KB_STATUS & CAPS_LOCK) && (!(KB_STATUS & SHIFT)))
    		if (inkey >= 'A' && inkey <= 'Z') 
				inkey += ('a' -'A');

    	if (KB_STATUS & SHIFT) 
			chg_key();

		if (PininMode == 0) 
    	{
    		short		i;
    		static char	keytbl1[] = ";',.";
    		static char skeytbl1[]= ":\"<>";
    		static char	keytbl2[] = "`1234567890-=[]/";
    		static char	skeytbl2[]= "~!@#$%^&*()_+{}?";

			if ((KB_STATUS & CAPS_LOCK) && (KB_STATUS & SHIFT))
			{
    			for (i = 0; i < 16; i++)
    			{
	    			if (skeytbl2[i] == inkey) 
    				{
	    				inkey = keytbl2[i];
    					break;
    				}
    			}
			}
    	}
    } 
    else 
    { // IS_FULLABC 
		chg_key();
    }

	if (inkey >= 0x20 && inkey < 0x7f && ownoff == 0) 
	{
	    if (InputState == IS_FULLABC || ((KB_STATUS & SHIFT) && inkey != SPEC_CHAR) || UnusedKey())
		{
			inkey_bak = inkey;
			inkey = SPEC_CHAR;
			if (others() == 1)
			    inkey = inkey_bak;
			else
				return(1);
	    }
	}

	if(nowkey_buf[0] != SPEC_CHAR)
	{
		if (inkey >= 'A' && inkey <= 'Z')
			inkey += ('a' - 'A');
	}
    //specChoice = ((KB_STATUS & SHIFT) && scancode == DOWN) ? 1 : 0;

	if (scancode == UP && ownoff == 0) 
	{
	    if (f_want == j_off) 
			ENDSEL = 1;
	    
	    shift_cursor(-2, 1);
	    modify();
	}
	else if (scancode == HOME || scancode == END) 
	{
	    shift_cursor((short) ((scancode == HOME) ? -(L_STNCE+1) : (L_STNCE+1)), 1);
	    ENDSEL = 0;
	}
	else if (scancode == LEFT || scancode == RIGHT) 
	{
	    shift_cursor((short) ((scancode == LEFT) ? -2 : 2), 1);
	    ENDSEL = 0;
	}
	else if (scancode == DEL) 
	{
	    if (ownoff == 0)
			Handelete(0);
	    else
			return 0;
	}
	else if (scancode == BCKSP)
	    backsp();
	else if (scancode == ESC) 
	{
	    if (ownoff == 0 && undoLen)
			recover();
	    else
			return 0;
	}
	else if (scancode == RET) 
		enter();
	else
	    others();

	if (ownoff == 0) 
		SpecialRuleMatch();

	return 1;
}

/********************************************************************/
/*	Function   : others					    */
/*	Synopsis   : Process of normal input key.		    */
/*	Argument   : None					    */
/*	Return	   : None					    */
/********************************************************************/
short	others()
{
	if (ownoff == 0 && j_off >= L_STNCE) 
	{
		if (f_want == 0) 
			return 0;
		else
			scroll();
	}
	if (match() < 0)
		return 0;
	
	return(1);
}

short	UnusedKey()
{
    short	i;
    short	spec_key_bit;

    if (InputMode == IBMKBD)
	    spec_key_bit = IBM_BIT;
    else if (InputMode == ETKBD)
	    spec_key_bit = ET_BIT;
    else if (InputMode == MITECKBD)
	    spec_key_bit = MITEC_BIT;
    else
	    spec_key_bit = STD_BIT;

    for (i=0; i<41; i++) 
	{
		if (spec_key[i].spec_key == inkey) 
		{
			if (PininMode != 0 || spec_key[i].key_fg & spec_key_bit)
				return(1);
		}
    }
    return(0);
}

/********************************************************************/
/*	Function   : init_vars					    */
/*	Synopsis   : Inialize the global variables.		    */
/*	Argument   : None					    */
/*	Return	   : None					    */
/********************************************************************/
void	init_vars()
{
	short i;

	ownoff = j_off = f_want = 0;
	PininPos = -1;
	ENDSEL = 0;
	undoLen = 0;
	ldDirty = 0;
	born_flag = 0;
	nowkey_buf[0] = '\0';
	for (i=0; i<10; i++) 
	{
	    rule_buf[i] = 0;
	    pos_buf[i] = 0;
	}
	for (i=0; i < (L_STNCE>>1); i++)
	    init_len_mark(i);
}

/************************************************************/
/*	Function   : perform									*/
/*	Synopsis   : Send Chinese char. back to system. 	    */
/*	Argument   : None										*/
/*	Return	   : None										*/
/************************************************************/
void	perform()
{
	short	i, j;

	pWA->cchResult = j_off;
	memcpy(pWA->chResult, youwant, pWA->cchResult);
	for (i=j=0; i<j_off; i+=2) 
	{
		fillback(j, i);
		j += 4;
	}
}

void	initial()
{
	init_vars();
	clr_line(W_START, W_END+39+4);
}

/********************************************************************/
/*	Function   : enter					    */
/*	Synopsis   : The process of "Return" key.                   */
/*	Argument   : None					    */
/*	Return	   : None					    */
/********************************************************************/
void	enter()
{
	if (j_off != 0 /* && ownoff == 0*/) 
	{
		perform();
		LDWrite();
	}
	initial();
}

void	put_back()
{
	perform();
	LDWrite();
	pWA->chResult[pWA->cchResult] = inkey;
	pWA->cchResult++;
	pWA->chResult[pWA->cchResult] = 0;
	initial();
}

void	modify()
{
	born_flag = 1;
	RecoverZhuin((short) (f_want >> 1));
	born_flag = 0;
}

void	recover()
{
    short			i, pos;
    unsigned char	temp;

    if (undoLen > 0 && undoStart >= 0 && j_off >= undoStart+undoLen) 
	{
		for (i=0; i<undoLen; i++) 
		{
			temp = youwant[undoStart+i];
			youwant[undoStart+i] = youwant_bak[undoStart+i];
			youwant_bak[undoStart+i] = temp;
		}
		for (i=0; i<undoLen; i+=2) 
		{
			temp = Core[undoStart+i>>1].flag;
			Core[undoStart+i>>1].flag = Core[undoStart+i>>1].flag_bak;
			Core[undoStart+i>>1].flag_bak = temp;
		}

		pos = undoStart + undoLen - 2;
		if ((Core[pos>>1].flag_bak & 0x0f) > 1) 
		{  /* cancel word matching */
			Core[pos>>1].flag = 1;
			SearchOneChar(pos);
			youwant[pos] = chbuf[0];
			youwant[pos+1] = chbuf[1];
		}
		words_dis((short) (W_START+undoStart), youwant+undoStart, undoLen, NOR);
    }
}

short	keypad()
{
	if ((scancode== 0xe0 && (inkey == 0x2f || inkey==0x0d || inkey==0x0a)) ||
	    (scancode== 0x37 && inkey == 0x2a) ||
	    (scancode >= 0x47 && scancode <= 0x53 && inkey >= '+' && inkey <= '9'))
		return(1);
	else
		return(0);
}

void	chg_key()
{
	if (inkey >= 'A' && inkey <= 'Z')
		inkey += ('a' - 'A');
	else if (inkey >= 'a' && inkey <= 'z')
		inkey -= ('a' - 'A');
}
