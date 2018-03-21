#include "hanin.h"

/*********************************************************************
    Function --- LDWrite
    Synopsis --- write LD from memory to disk
    Argument --- none
    Return   --- 0(succeed), -1(fail)
*********************************************************************/
short	LDWrite()
{
    FILE	*l_fp;

    if (ldDirty) 
	{
		if ((l_fp=fopen(user_dic, "r+")) == NULL)
			return(-1);

		ldWord[LD_MAX_WORD].next = ldWordHead;
		fseek(l_fp, (long)USR_LD_POS, 0);
		fwrite((unsigned char *)ldWord, 1, USR_LD_LEN, l_fp);
		fclose(l_fp);

		ldDirty = 0;
    }
    return(0);
}

/*********************************************************************
    Function --- LDSearchWord
    Synopsis --- search word in LD
    Argument --- cmpCode: the buffer of compressed codes
		 outBuf: the buffer of serached word(s)
		 len: 2 for char, 4 for word
		 mode: SEARCH_ONE for only one, SEARCH_ALL for all
    Return   --- the number of bytes searched in outBuf
*********************************************************************/
short	LDSearchWord(short st, short len, short mode)
{
	int				i = 0;
    LDRec			*ldPtr;
    unsigned char	pHi, pLo;
    unsigned char	ldWordCur;
    unsigned char	cmpCode[5];

    if (len > 4) 
		return(0);

    oldchlen = chlen;
    strncpy((char *)cmpCode, (char *)cmpkigo+st, len);
    if (len == 2) 
	{   /* search LD char */
		cmpCode[2] = cmpCode[3] = '\0';
    }
    cmpCode[4] = '\0';

    ldWordCur = ldWord[ldWordHead].last;
    for ( ; ; ) 
	{
		ldPtr = &ldWord[ldWordCur];
		if (strncmp((char *)ldPtr->cmpCode, (char *)cmpCode, 4) == 0 &&
			(mode == SEARCH_ALL || big5cmp(ldPtr->bigCode, st, len) == 0)) 
		{
			pHi = ldPtr->bigCode[0] ^ HANIN_CRYPT;
			pLo = ldPtr->bigCode[1] ^ HANIN_CRYPT;
			if (mode == SEARCH_ALL || len == 4 ||
				(pHi >= 0xa4 && pHi <= 0xc5) ||  /* frequently-used char */
				(pHi == 0xc6 && pLo <= 0x7e)) 
			{  /* user-defined char */
				AppendToBuffer(ldPtr->bigCode, len, st, len, mode);
				if (mode == SEARCH_ONE) 
				{
					LDMoveWord(ldWordCur);
					break;
				}
			}
		}
		if (ldWordCur == ldWordHead) 
			break;   /* LD word not exist */
		ldWordCur = ldPtr->last;
		i++;
    }
    freq_fg = FRQ_FG;
    return(chlen-oldchlen);
}

/*********************************************************************
    Function --- LDInsertWord
    Synopsis --- insert learning word into LD, or move it to top
    Argument --- cmpCode: the buffer of compressed codes
		 bigCode: the buffer of Big5 codes
    Return   --- 0: insert successfully
		 -1: word exist
*********************************************************************/
short	LDInsertWord(PBYTE cmpCode, PBYTE bigCode)
{
    short			i;
    LDRec			*ldPtr;
    unsigned char	ldWordCur;

    ldWordCur = ldWord[ldWordHead].last;
    for ( ; ; ) 
	{
		ldPtr = &ldWord[ldWordCur];
		for (i=0; i<4; i++)
		{
			if (ldPtr->cmpCode[i] != *(cmpCode+i) ||
				(*(cmpCode+i) &&
				ldPtr->bigCode[i] != (*(bigCode+i) ^ HANIN_CRYPT))) 
				break;
		}
		if (i == 4) 
		{  /* LD word exist, move it to top */
			LDMoveWord(ldWordCur);
			return(-1);
		}

		if (ldWordCur == ldWordHead) 
		{	/* LD word not exist, insert it */
			for (i=0; i<4; i++) 
			{
				ldPtr->cmpCode[i] = *(cmpCode+i);
				ldPtr->bigCode[i] = *(bigCode+i) ^ HANIN_CRYPT;
			}
			ldDirty = 1;
			ldWordHead = ldPtr->next;
			return(0);
		}

		ldWordCur = ldPtr->last;
    }
}

/*********************************************************************
    Function --- LDMoveWord
    Synopsis --- move learning word to top of LD
    Argument --- ldWordCur: the index of learning word to be moved
    Return   --- none
*********************************************************************/
void	LDMoveWord(BYTE ldWordCur)
{
    ldDirty = 1;
    if (ldWordCur == ldWordHead)
		ldWordHead = ldWord[ldWordCur].next;
    else 
	{
		ldWord[ldWord[ldWordCur].next].last = ldWord[ldWordCur].last;
		ldWord[ldWord[ldWordCur].last].next = ldWord[ldWordCur].next;
		ldWord[ldWordCur].last = ldWord[ldWordHead].last;
		ldWord[ldWordCur].next = ldWordHead;
		ldWord[ldWord[ldWordHead].last].next = ldWordCur;
		ldWord[ldWordHead].last = ldWordCur;
    }
}

void	InsertLearnCode (short learnlen)
{
	BYTE	cmpCode[4];

	if (learnlen == 2) 
	{
		if (cmpkigo[f_want] != '~' &&
		    ((youwant[f_want] >= 0x81 && youwant[f_want] <= 0xa0) ||
		     (youwant[f_want] >= 0xa4 && youwant[f_want] <= 0xfe))) 
		{
			cmpCode[0] = cmpkigo[f_want];
			cmpCode[1] = cmpkigo[f_want+1];
			cmpCode[2] = cmpCode[3] = '\0';
			LDInsertWord(cmpCode, youwant+f_want);
		}
		Core[f_want>>1].flag |= CHG_FG;
	}
	else 
	{
		LDInsertWord(cmpkigo+f_want, youwant+f_want);
		Core[f_want>>1].flag |= CHG_FG;
		Core[(f_want>>1)+1].flag |= CHG_FG;
	}

	if (undoLen && f_want > undoStart-learnlen && f_want < undoStart+undoLen)
		undoLen = 0;
}

