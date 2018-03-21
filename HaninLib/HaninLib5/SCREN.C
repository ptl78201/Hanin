#include "hanin.h"

/********************************************************************/
/*      Function   : delete                                         */
/*      Synopsis   : delete a char on cursor position               */
/*      Return     : None                                           */
/********************************************************************/
short	Handelete(short isBackSpace)
{
    if (j_off == 0 || f_want == j_off) 
	{
        BELL();
        return(-1);
    }

    if (f_want < undoStart)
        undoStart -= 2;
    else if (f_want >= undoStart && f_want < undoStart+undoLen) 
	{
        if (isBackSpace && undoLen &&
            f_want == undoStart+undoLen-2) 
		{  /* pressing backspace */
            if ((Core[f_want>>1].flag & 0x0f) > 1) 
				recover();
            for (undoLen-=2; undoLen>2; undoLen-=2)
                if ((Core[(undoStart+undoLen-2)>>1].flag & 0x0f) > 1) 
					break;
            if (undoLen <= 2) 
				undoLen = 0;
        }
        else
            undoLen = 0;
    }

    /* shift data after recover() */
    lshift_data((short) (f_want+2));

    if (f_want > 0 && f_want < j_off)
        init_len_mark((short) ((f_want>>1)-1));

    if ((j_off-=2) == 0)
        f_want = 0;

    words_dis((short) (W_START+f_want), youwant+f_want, (short) (j_off-f_want), NOR);
    clr_line((short) (W_START+j_off), (short) (W_START+j_off+2));
	return 0;
}

/********************************************************************/
/*      Function   : backsp                                         */
/*      Synopsis   : the back space function                        */
/*      Argument   : None                                           */
/*      Return     : None                                           */
/********************************************************************/
short	backsp()
{
    if (ownoff == 0 && f_want == 0)
	{
        BELL();
        return(-1);
    }

    if (ownoff == 0) 
	{
        shift_cursor(-2, 1);
        Handelete(1);
    }
    else 
	{   /* backsp a zhuin */
        if (PininMode && nowkey_buf[0] != SPEC_CHAR)
            PininDelete();
        else 
		{    /* zhuin mode */
            ownoff--;
            show_pinin((short) (SP_DELETE|ownoff), 0);
            if(ownoff == 0)
                nowkey_buf[0] = '\0';
        }
    }
    return(0);
}

/********************************************************************/
/*      Function   : shift_cursor                                   */
/*      Synopsis   : shift cursor                                   */
/*      Argument   : count : count of shift                         */
/*                      99 : rightmost position                     */
/*                     -99 : leftmost position                      */
/*      Return     : None                                           */
/********************************************************************/
void	shift_cursor(short count, short show)
{
    short len;

    if (ownoff != 0 ||
        (count < 0 && f_want == 0) ||
        (count > 0 && f_want == j_off))
        BELL();
    else 
	{
        if ((len=j_off-f_want) == 0);
        else 
		{
            if (len > 2) len = 4;
            words_dis((short) (W_START+f_want), youwant+f_want, len, NOR);
        }

        f_want += count;
        if (f_want < 0)
            f_want = 0;
        else if (f_want > j_off)
            f_want = j_off;
    }
}


