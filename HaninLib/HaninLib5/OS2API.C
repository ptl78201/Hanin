#include <stdio.h>
#include <assert.h>
#include "hanin.h"

short	LDCheck(void);
void	LDCreate(void);

char	szHanHead[12] = "º~­µ3.0";

void	Clr_line(short x1, short x2)
{
	short	i;

	for(i=x1;i<=x2;i++) 
		put_ch(i, ' ', NOR);
}

void	bell()
{
}

void	words_dis(short loc, PBYTE s, short len, short mode)
{
	short i;

	for (i=0; i<len; i++) 
		put_ch((short) (loc+i), s[i], mode);
}

void	put_ch(short x, Char ch, short mode)
{
	if (x >= W_START && x < W_END) 
	{
		x = x - W_START;
		if ( mode == NOR) 
			mode = BLUE;
		pWA->chDisplay[x] = ch == ' ' ? 0 : ch;
		pWA->attrDisplay[x] = convert_attr(mode);
		return;
	}
	if (x < W_START) 
	{
		pWA->chMode[x] = ch;
		pWA->attrMode[x] = convert_attr(NOR);
	} 
	else if (x >= W_END) 
	{
		x = x-W_END+6;
		if (x >= 63) 
			return;
		pWA->chMode[x] = ch;
		pWA->attrMode[x] = convert_attr(NOR);
	}	
}

char	convert_attr(short attr)
{
	switch(attr) 
	{
	case NOR :	return 1;
	case REV :	return 2;
	case GREEN :return 3;
	case RED :	return 4;
	case BLUE :	return 5;
	case HILITE:return 6;
	default :	return 7;
	}
}

short	CheckDictionary()
{
	short	n;

	if (! MDLoad(0)) 
		return 0;

	if (! MDLoad(1))	 /* address dictionary */
		return 0;

	for (n = 0; n < MAXPROFDICOPENED; n++)
	{
		if (prof_dic[n][0] && ProfDicLoad (n) == 0)
			prof_dic[n][0] = 0;
	}

	ckUDLoad();	  /* check user dictionary exist? */
	return 1;
}

short	MDLoad(short i)
{
	long	len;
	FILE	*fp;
	
	if (i)
		fp = fopen(add_dic,"rb");
	else
		fp = fopen(hanin_dic,"rb");
	if (fp == 0)
		return 0;
	len = fread(&mdIdxTable1[i][0],1,MD_IDX_SIZE,fp);
	
	fclose(fp);

    return(1);
}

short	ProfDicLoad(short n)
{
	long	len;
	FILE	*fp;
	
	fp = fopen(prof_dic[n],"rb");
	if (fp == 0)
		return 0;
	len = fread(&mdIdxTable1[n + 2][0],1,MD_IDX_SIZE,fp);
	
	fclose(fp);

    return(1);
}

short	ckUDLoad()
{
	BYTE	*u;
	FILE	*fnum;
	long	i, len;
	short	j;

	if ((fnum=fopen(user_dic, "rb+")) == NULL)
	{
		for (i=0; i<LD_MAX_WORD; i++) 
		{
		    ldWord[i].last = i - 1;
		    ldWord[i].next = i + 1;
		    ldWord[i].cmpCode[0] = '\0';
		}
		ldWord[0].last = LD_MAX_WORD - 1;
		ldWord[LD_MAX_WORD-1].next = 0;
		ldWord[LD_MAX_WORD].next = ldWordHead = 0;
		for (i=0; i<UD_MAX_IDX+1; i++) udHashIdx[i] = 0;
		if ((fnum=fopen(user_dic, "w+"/*_O_BINARY|_O_CREAT|_O_RDWR*/)) == 0) 
			return(0);

		for (i=0; i<AD_IDX_SIZE; i++) toutbuf[i] = 0;
		ck_write(fnum, (unsigned char *)szHanHead, USR_ID_LEN);
		ck_write(fnum, (unsigned char *)toutbuf, USR_LAB_LEN);
		ck_write(fnum, (unsigned char *)ldWord, USR_LD_LEN);
		ck_write(fnum, (char *)udHashIdx, UD_IDX_SIZE);
		for (i=(long)UD_MAX_SIZE+(long)USR_AD_LEN; i>0; i-=len) 
		{
		    len = (i > AD_IDX_SIZE) ? AD_IDX_SIZE : i;
		    ck_write(fnum, (char *)toutbuf, (size_t)len);
		}
		memset (idxbuff, 0, QD_IDXTABLE_SIZE);
	}
	else 
	{
		ck_lseek(fnum, USR_LD_POS, 0, 0);
		ck_read(fnum, (unsigned char *)ldWord, LD_WORD_SIZE*(LD_MAX_WORD+1));
		ck_lseek(fnum, USR_UD_POS, 0, 0);
		ck_read(fnum, (unsigned char *)udHashIdx, UD_IDX_SIZE);
		u = (unsigned char *)udHashIdx;
		for(j=0; j<UD_IDX_SIZE; j+=2)
			two_invert(u+j);

		ck_lseek (fnum, OFF_QDSTART, 0, 0);
		ck_read (fnum, (PBYTE)idxbuff, QD_IDXTABLE_SIZE);  // 256*6

		for (i = 0; i < QD_IDX_SIZE; i++)
		{
			for (j = 0; j < 6; j += 2)
			{
				if (idxbuff[i * 6 + j] == 0)
					break;

				if (idxbuff[i * 6 + j] < 0xA0)
				{
					memset (&idxbuff[i * 6], 0, 6);
					break;
				}
				if (idxbuff[i * 6 + j] == 0xa1 && idxbuff[i * 6 + j + 1] == 0x40)
				{
					memset (&idxbuff[i * 6], 0, 6);
					break;
				}
			}
		}
	}
	ck_close(fnum);
	ldWordHead = ldWord[LD_MAX_WORD].next;
	
	if (LDCheck() != 0)
		LDCreate();
	
	return(1);
}


short	LDCheck(void)
{
	short	i;
	BYTE	CheckArray[LD_MAX_WORD];
	
	if (ldWordHead>=LD_MAX_WORD)	return(1);
	
	for (i = 0; i < LD_MAX_WORD; i++)
		CheckArray[i] = 0;

	for (i=0; i<LD_MAX_WORD; i++) {
		if (ldWord[i].last >= LD_MAX_WORD)
			return (1);

		if (CheckArray[ldWord[i].last] == 1)
			return (1);

		CheckArray[ldWord[i].last] = 1;
	}

	for (i = 0; i < LD_MAX_WORD; i++)
		CheckArray[i] = 0;

	for (i=0; i<LD_MAX_WORD; i++) {
		if (ldWord[i].next >= LD_MAX_WORD)
			return (1);

		if (CheckArray[ldWord[i].next] == 1)
			return (1);

		CheckArray[ldWord[i].next] = 1;
	}

	return 0;
}


void	LDCreate(void)
{
	short	i;

	for (i=0; i<LD_MAX_WORD; i++) 
	{
	    ldWord[i].last = i - 1;
	    ldWord[i].next = i + 1;
	    ldWord[i].cmpCode[0] = '\0';
	}
	ldWord[0].last = LD_MAX_WORD - 1;
	ldWord[LD_MAX_WORD-1].next = 0;
	ldWord[LD_MAX_WORD].next = ldWordHead = 0;
}


