#include "hanin.h"

/*******************************************************************
Function   : match
synopsis   : Perform Hanin algorithm.
Return	   : -1: error, 0: continue matching, 2: finish matching
	      1: juiin symbol born again and finished
********************************************************************/
short	match()
{
	short i, j, k, len, oldlen;
	short ooldlen,ooldfreq;
	short start;
	short fndx_idx;

	if ((ownoff == 0 && inkey == SPEC_CHAR) ||
	    (ownoff != 0 && nowkey_buf[0] == SPEC_CHAR))
	    i = SymbolMatch();
	else
	    i = (PininMode == 0) ? ZhuinMatch() : PininMatch();

	if (i == 5) {	       /* error */
	    return(-1);
	}
	else if (i != 4)       /* uncompleted zhuin */
	    return(0);
	else if (born_flag)
	    return(1);

	rshift_data();

	if (nowkey_buf[0] == SPEC_CHAR) {    /* SymbolMatch */
	    cmpkigo[f_want] = '~';
	    cmpkigo[f_want+1] = Core[f_want>>1].nowkey[1] = nowkey_buf[1];  /* trick for MDSearchChar */
//	    if (cmpkigo[f_want+1] >= 'A' && cmpkigo[f_want+1] <= 'Z')
//		cmpkigo[f_want+1] += 'a' - 'A';
	}
	else
	    CompressZhuin(distmp, cmpkigo+f_want);    /* compress zhuin */

	Core[f_want>>1].fchar[0] = 0;
	Core[f_want>>1].flag = 1;
	init_len_mark((short) (f_want>>1));
	if (f_want > 0 && f_want < j_off) 
		init_len_mark((short) ((f_want>>1)-1));

	j_off += 2;

	for (i=0; i<=8; i+=2) {   /* max word length is 5 */
	    if (f_want-i < 0 || (!AddrState && cmpkigo[f_want-i] == '~'))
		break;
	}
	if ((i-=2) < 0) i = 0;	  /* for cmpkigo[f_want] == '~' */

	for ( ; i>=2; i-=2) {
	    len = (i+2) >> 1;
	    start = f_want - i;
	    word_fg = WRD_FG;	  /* initialize */
	    oldfreq = -1;
	    liang[2]=0;
	    fndx_idx = -1;
	    if((len > (oldlen=Core[(f_want>>1)-1].flag & LEN_FG) ||
	       (len==3 && oldlen==3 ) ||
		(oldlen == 2 &&
		((undoLen && undoStart+undoLen == f_want && ((ooldlen=Core[(f_want>>1)-2].flag_bak & LEN_FG)) == 2) ||
		(fndx_idx=fndx((short) (f_want-4))) != -1 ||
		((oldfreq=Core[(f_want>>1)-1].flag & FRQ_FG) != FRQ_FG && 
		(fndx(f_want) == -1 || (liang[2]&0x07)==0))))) &&
 		(SearchOneWord((short) start, (short) (i+2)))) 
		{
			if ( len==3 && oldlen==3 )
			{
				if ( (Core[(f_want>>1)-1].flag & FRQ_FG ) >= freq_fg )
					continue;
			}	
/*
	      WW
	       WWW
*/
			if ( len == 3  && f_want >= 6 )     
			{
	    		if ((Core[(f_want>>1)-2].flag & LEN_FG) == 2 )
	    		{
					ooldfreq=Core[(f_want>>1)-2].flag & FRQ_FG;
					if (fndx((short) (f_want-6)) == -1 && ooldfreq > freq_fg )
						  continue;                                       
	    		}
			}
/*
      WW
       WW
        WW
*/
			
		if ( len == 2 && oldlen == 2 )     
		{
			oldfreq=Core[(f_want>>1)-1].flag & FRQ_FG; 
			if ( ooldlen == 2 )
			{
				ooldfreq=(Core[(f_want>>1)-2].flag_bak & FRQ_FG);
				if ( ooldfreq+freq_fg > oldfreq )
					goto XX11;
			}
			if ( fndx_idx != -1 && (liang[2] >= oldfreq ))
				goto XX11;
			if ( oldfreq > freq_fg)
				continue;
	//		if (fndx_idx != -1 && ((liang[2]&0x70) < oldfreq) && oldfreq >= freq_fg)
	//			continue;
		}
		
	XX11:	
		if (cmpkigo[i] == '=' && cmpkigo[i+1] == '4') word_fg = 0;  /* for pinin "le5" */
		if (undoLen && start > undoStart && start < undoStart+undoLen &&
		    (j=Core[(undoStart+undoLen-2)>>1].flag & LEN_FG) > 1 &&
		    start != undoStart+undoLen-(j<<1)) {   /* > */
		    undoLen = (f_want+2) - undoStart;
		}
		else {
		    undoStart = (char) start;
		    undoLen = i+2;
		}

		if (fndx_idx != -1) {	 /* for liang jie char */
		    youwant_bak[start-2] = liang[0];
		    youwant_bak[start-1] = liang[1];
		}
		for (k=0; k<i+2; k++)
		    youwant_bak[start+k] = chbuf[k];
		for (k=0; k<i+2; k+=2)
		    Core[(start+k)>>1].flag_bak = (k<i) ? (word_fg|1) : (word_fg|freq_fg|len);
		recover();
		break;
	    }
	}
	if (i < 2) {
	    if (SearchOneChar(f_want)) {
		youwant[f_want] = chbuf[0];
		youwant[f_want+1] = chbuf[1];
		if (f_want <= undoStart)
		    undoStart += 2;
		else if (f_want > undoStart && f_want < undoStart+undoLen)
		    undoLen = 0;
	    }
	    else {
		lshift_data((short) (f_want+2));
		j_off -= 2;
		ownoff--;
		if (PininMode && nowkey_buf[0] != SPEC_CHAR) PininPos--;
		if (f_want > 0) 
			init_len_mark ((short) (((int) f_want >> 1) - 1));
		return(2);
	    }
	}

	len = i + 2;
	k = (PininMode == 0 || nowkey_buf[0] == SPEC_CHAR) ? ownoff : PininPos+1;
	for (i=0; i<k; i++)
	    Core[f_want>>1].nowkey[i] = nowkey_buf[i];
	ownoff = 0;
	nowkey_buf[0] = '\0';
	PininPos = -1;
	f_want += 2;

	start = f_want - len;
	words_dis((short) (W_START+start), (PBYTE) (youwant+start), (short) (j_off-start), NOR);
	clr_line((short) (W_START+j_off), (short) (W_START+j_off+8));

	return(2);
}

short	SymbolMatch()
{
    if (ownoff == 0) {
	nowkey_buf[ownoff++] = SPEC_CHAR;
	if (inkey_bak == 0) 
		show_pinin((short) (SP_INSERT|ownoff), 0);  /* SPEC_CHAR pressed by user */
	/* ownoff++;	increment after show_pinin() */
	return(1);
    }
    else {
	nowkey_buf[ownoff++] = inkey;
	return(4);
    }
}

void	rshift_data()
{
    short i;

    for (i=j_off-1; i>=f_want; i--) {
	youwant[i+2] = youwant[i];
	youwant_bak[i+2] = youwant_bak[i];
	cmpkigo[i+2] = cmpkigo[i];
    }
    for (i=(j_off>>1)-1; i>=(f_want>>1); i--)
	Core[i+1] = Core[i];
}

void	lshift_data(short start)
{
    short i;

    for (i=start; i < j_off; i++) {
	youwant[i-2] = youwant[i];
	youwant_bak[i-2] = youwant_bak[i];
	cmpkigo[i-2] = cmpkigo[i];
    }
    for (i=(start>>1); i < (j_off>>1); i++)
	Core[i-1] = Core[i];
}

