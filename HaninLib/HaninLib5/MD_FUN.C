#include "hanin.h"

void	two_invert(unsigned char *s);
short	AddrCharMatch (short st);

/*******************************************************************
    Function --- SearchOneWord
    Synopsis --- search a word in LD->UD->SD->MD
    Argument --- st: index in cmpkigo
		 len: length of word
*******************************************************************/
short SearchOneWord(short st, short len)
{
	short	n;

    chlen = 0;	*chbuf = '\0';
    if (len == 6 && RepSearchWord(st))
		return 1;
		
	if (LDSearchWord(st, len, SEARCH_ONE))     /* search in LD */
		return 1;

	if (UDSearchWord(st, len, SEARCH_ONE))     /* search in UD */
		return 1;

	if (AddrState == 0)
	{
		for (n = 1; n <= MAXPROFDICOPENED; n++) 
		{
			if (MDSearchWord(n, st, len, SEARCH_ONE))
				return 1;
		}
	}
	if (MDSearchWord(0, st, len, SEARCH_ONE))    /* search in MD */
		return 1;

	if (UnitSearchWord(st, len, SEARCH_ONE))     /* search in unit table */
		return 1;

	return 0;
}

/*******************************************************************
    Function --- SearchOneChar
    Synopsis --- search a character in LD->SD->MD->FD
    Argument --- nth : index in cmpkigo
*******************************************************************/
short SearchOneChar(short st)
{
	short	n;

    chlen = 0;	*chbuf = '\0';
    if (LDSearchWord(st, 2, SEARCH_ONE)) /* search a char in LD */
		return 1;

	if (AddrState)
	{
		if (AddrCharMatch (st))
			return 1;
	}
	else
	{
		for (n = 1; n <= MAXPROFDICOPENED; n++) 
		{
			if (MDSearchChar(n, st, SEARCH_ONE))/* search a char in professional dics */
				return 1;
		}
	}
	if (MDSearchChar(0, st, SEARCH_ONE))	/* search a char in MD */
		return 1;

	chlen += GetBig5EChar (&cmpkigo[st], &chbuf[chlen], 1);
	if (chlen)
		return 1;

	return 0;
}

short MDSearchFChar(short nDic, short st)
{
    BYTE	item;
    IDX1	*idxTable1;
	FILE	*cfp;
    long	off;
    short	inxSize;
    short	i, j, count;
    short	low, high, mid;
    
    if (cmpkigo[st] == '~') 
	{
		Core[st>>1].fchar[0] = 0;
    }
    else 
	{
		low = 0;
		high = MD_MAX_IDX;
		item = *(cmpkigo+st);
		idxTable1 = mdIdxTable1[(AddrState)?1:0];
		inxSize = sizeof(IDX1);
		if ((mid=BinarySearch((unsigned char *)idxTable1, item, low, high,inxSize )) == -1)
			return(-1);

		count = ((idxTable1[mid+1].charoff1 << 8) + idxTable1[mid+1].charoff2) -
			((idxTable1[mid].charoff1 << 8) + idxTable1[mid].charoff2);

		off = (long)MD_FCHAR_POS + ((long)(idxTable1[mid].charoff1 << 8) + idxTable1[mid].charoff2) * (12+1);

		if (nDic == 0)
		{
			if (AddrState)
				cfp = fopen(add_dic, "rb");
			else
				cfp = fopen(hanin_dic, "rb");
		}
		else
		{
			if (prof_dic[nDic - 1][0])
				cfp = fopen(prof_dic[nDic - 1], "rb");
			else
				return -1;
		}

		fseek(cfp,off,0);
		fread(toutbuf, (13*count+1)&0xfffe, 1, cfp);
		fclose(cfp);

		for (i=0; i<count; i++) 
		{
			if (toutbuf[i] == *(cmpkigo+st+1)) 
			{
				for (j=0; j<10; j++)
					Core[st>>1].fchar[j] = toutbuf[count+12*i+j];
				break;
			}
		}
		return((i<count) ? 0 : -1);
    }
	return -1;
}

/*******************************************************************
    Function --- MDSearchChar
    Synopsis --- search char(s) in MD or SD
    Argument --- st:
		 mode: SEARCH_ONE(0) for only one,
		       SEARCH_ALL(1) for all
    Return   --- 0: not found
		 >0: the number of bytes of found chars
*******************************************************************/
short MDSearchChar(short nDic, short st, short mode)
{
    FILE	*wfp;
    short	key_fg, flt_fg;
    short	clen, spec_code, i, j;

    oldchlen = chlen;
    if (cmpkigo[st] == '~' && cmpkigo[st+1] == 0x20) 
	{   /* full Chinese blank */
		toutbuf[0] = 0xa1 ^ HANIN_CRYPT;
		toutbuf[1] = 0x40 ^ HANIN_CRYPT;
		clen = 2;
    }
    else if (mode == SEARCH_ONE && cmpkigo[st] != '~') 
	{
		clen = 0;
		if (MDSearchFChar(nDic, st) != -1) 
		{
			toutbuf[0] = Core[st>>1].fchar[0] ^ HANIN_CRYPT;
			toutbuf[1] = Core[st>>1].fchar[1] ^ HANIN_CRYPT;
			clen = 2;
		}
    }
    else 
	{
		clen = 0;
		if (nDic == 0)
		{
			if (AddrState)
				wfp = fopen(add_dic, "rb");
			else
				wfp = fopen(hanin_dic, "rb");
		}
		else
		{
			if (prof_dic[nDic - 1][0])
				wfp = fopen(prof_dic[nDic - 1], "rb");
			else
				return 0;
		}

		if (MDGetCharAddress(nDic, st, wfp, SEARCH_CHAR) != -1 &&
			(clen=Core[st>>1].clen[nDic] & 0x3fff) != 0) 
		{
			fseek(wfp, (long)(outoff), 0);

			spec_code = (cmpkigo[st] == '~') ? 1 : 0;
			if (mode == SEARCH_ONE && spec_code == 0) 
				clen = 2;
			ReadBuffer(toutbuf, wfp, clen);

/* omit 0xC6a1 ~ 0xc8fe */
			for (j=clen, i=clen=0; i<j; i+=2) 
			{
				if ((toutbuf[i] < 0xc6 && toutbuf[i] >= 0xa4) || 
					(toutbuf[i] < 0xf9 && toutbuf[i] >= 0xc9) ||
					(toutbuf[i] == 0xf9 && toutbuf[i+1] <= 0xd5) ||
					(toutbuf[i] == 0xc6 && toutbuf[i+1] <= 0x7e) ||
					(toutbuf[i] < 0xa3) || (toutbuf[i] == 0xa3 && toutbuf[i] < 0xe0)) 
				{
					toutbuf[clen++] = toutbuf[i] ^ HANIN_CRYPT;
					toutbuf[clen++] = toutbuf[i+1] ^HANIN_CRYPT;
				}
			}

			if (spec_code) 
			{
				flt_fg = 0;
				if (PininMode == 0 && (mode == SEARCH_ALL || inkey_bak)) 
				{  /* zhuin mode */
					for (i=0; i<41 && spec_key[i].spec_key != cmpkigo[st+1]; i++);
					if (i < 41)
						if ((spec_key[i].key_fg>>4) & (1<<InputMode-1)) flt_fg = 1;
				}
				key_fg = 0;
				for (i=j=0; j<clen; j+=2) 
				{
					if (toutbuf[j] == (0xa4 ^ HANIN_CRYPT) && toutbuf[j+1] == (0x47 ^ HANIN_CRYPT))
						key_fg ^= 1;	  /* must use xor */
					else if (key_fg == 0 || flt_fg == 1) 
					{
						toutbuf[i] = toutbuf[j];
						toutbuf[i+1] = toutbuf[j+1];
						i += 2;
					}
				}
				clen = i;
			}
		}
		fclose(wfp);
    }

    if (mode == SEARCH_ALL && youwant[st] < 0xa4 && cmpkigo[st] != '~') 
	{
		for (i=0; i<clen && (toutbuf[i] ^ HANIN_CRYPT) >= 0xa4; i+=2);
		for (j=i; j<clen && (toutbuf[j] ^ HANIN_CRYPT) < 0xa4; j+=2);
		if (j > i) AppendToBuffer(toutbuf+i, (short) (j-i), st, 2, mode);
		if (i > 0) AppendToBuffer(toutbuf, i, st, 2, mode);
		if (clen > j) AppendToBuffer(toutbuf+j, (short) (clen-j), st, 2, mode);
    }
    else
		AppendToBuffer(toutbuf, clen, st, 2, mode);
    return(chlen-oldchlen);
}

/*******************************************************************
    Function --- MDSearchWord
    Synopsis --- search word(s) in MD or SD

    Argument --- st:
		 len: word length
		 mode: SEARCH_ONE(0) for only one,
		       SEARCH_ALL(1) for all
    Return   --- 0: not found
		 >0: the number of bytes of found words
*******************************************************************/
short MDSearchWord(short nDic, short st, short len, short mode)
{
    short	i;
    short	j_fg;
    short	word_len, lenmark;
    short	word_off, temp_off;
	BYTE	toutbuf[512];  // ???? size is critical
    BYTE	cmpCode[17];
    BYTE	cmp[17], big[17];
    BYTE	newword, tone, freqF, freqB, no;

	FILE	*wfp;
    short	st2;

    oldchlen = chlen;
    strncpy((char *)cmpCode, (char *)cmpkigo+st+2, len-2);
    cmpCode[0] = cmpkigo[st+1];
    cmpCode[len-2] = '\0';

    st2 = st >> 1;
    if (Core[st2].mlenmark[nDic] != -1) 
	{
		if ((Core[st2].mlenmark[nDic] & (char)(1<<((len-2)>>1))) == 0) 
			return(0);
    }
    else
		Core[st2].mlenmark[nDic] = 0;

	if (nDic == 0)
	{
		if (AddrState)
			wfp = fopen(add_dic, "rb");
		else
			wfp = fopen(hanin_dic, "rb");
	}
	else
	{
		if (prof_dic[nDic - 1][0])
			wfp = fopen(prof_dic[nDic - 1], "rb");
		else
			return 0;
	}

    if (Core[st2].widx[nDic] == -1 &&
       (MDGetCharAddress(nDic, st, wfp, SEARCH_WORD) == -1 ||
		MDGetWordAddress(nDic, st, wfp) == -1))
		Core[st2].widx[nDic] = 0;

    if (Core[st2].widx[nDic] != 0) 
	{
		fseek(wfp, (long)Core[st2].widx[nDic], 0);
		ReadBuffer(toutbuf, wfp, (short) (Core[st2].wrange[nDic]>510?510:Core[st2].wrange[nDic]));
	
		/* prepare for filter unmatched char */

		for (word_off=0; word_off < Core[st2].wrange[nDic]; ) 
		{
			newword = 0;
			if (toutbuf[word_off+1] & 0x80) 
			{
				newword = 1;
				lenmark = 0;
				tone = (toutbuf[word_off] & 0x03) + TONE_COUNT - 5;
		
				freqB = (toutbuf[word_off] >> 1) & FRQ_FG;
				freqF = (toutbuf[word_off] << 2) & FRQ_FG;
	
				cmp[2] = (cmpCode[0] - '!') / TONE_COUNT * TONE_COUNT + '!' + tone;
				cmp[3] = toutbuf[word_off+1] &= 0x7f;
				word_off += 2;
			}

			temp_off = word_off;
			for (word_len=4; toutbuf[word_off]<0x80; word_len+=2, word_off+=2);
			word_off += (newword) ? (word_len >> 1) + 1 : word_len-4 >> 1;
			for (i=4; i<word_len; ) cmp[i++] = toutbuf[temp_off++];
			for (i=(newword)?0:4; i<word_len; i+=2) 
			{
				if ((no=(toutbuf[temp_off++] & 0x3f)-1) < 5) 
				{
					if (Core[st+i>>1].fchar[0] == 0) 
						MDSearchFChar(nDic, (short) (st+i));
					big[i] = Core[st+i>>1].fchar[no<<1] ^ HANIN_CRYPT;
					big[i+1] = Core[st+i>>1].fchar[(no<<1)+1] ^ HANIN_CRYPT;
				}
				else 
				{
					MDGetCharAddress(nDic, (short) (st+i), wfp, SEARCH_CHAR);
					fseek(wfp, (long)(outoff+(no<<1)), 0);
					ReadBuffer(big+i, wfp, 2);
					big[i] ^= HANIN_CRYPT;
					big[i+1] ^= HANIN_CRYPT;
				}
			}

			lenmark |= 1<<((word_len-2) >> 1);
			if (cmpCode[0] == cmp[2] && cmpCode[1] == cmp[3]) 
	   			Core[st2].mlenmark[nDic] |= lenmark;	     
			if (word_len < len) 
				continue;

			j_fg = CompareCmpcode(cmpCode, cmp+2, (short) (len-2));
			if (j_fg > 0)
				continue;
			else if (j_fg < 0)
				break;

			if (word_len == len) 
			{    // && freqB != 0)
				if (mode == SEARCH_ALL ||
					(big5cmp(big, st, len) == 0 && (len > 4 || freqB > oldfreq))) 
				{
					if (chlen == oldchlen) 
						freq_fg = freqF;
					AppendToBuffer(big, len, st, len, mode);
				}
			}
		}
    }
    fclose(wfp);
    return(chlen-oldchlen);
}

/*******************************************************************
    Function --- MDGetCharAddress
    Synopsis --- get char address by 1st and 2nd compressed codes
    Argument --- st: input index
		 fnum: file reference number
		 mode: SEARCH_CHAR(0) for char,
		       SEARCH_WORD(1) for word
    Return   --- =0: found
		 <0: not found
*******************************************************************/
short MDGetCharAddress(short nDic, short st, FILE *wfp, short mode)
{
    int		i;
    IDX1	*idxTable1;
    IDX2	idxTable2[128];  /* 128 maximum number of entries is 88*6 = 528 */
    BYTE	item;
    short	low, high, mid;
    short	inxsize;

    if (mode == SEARCH_CHAR && Core[st>>1].cidx[nDic] != -1) {  /* file ptr exist */
	outoff = Core[st>>1].cidx[nDic];
    }
    else {
	low = 0;
	high = MD_MAX_IDX;
	item = *(cmpkigo+st);
	if (nDic == 0)
	{
		idxTable1 = mdIdxTable1[(AddrState)?1:0];
	}
	else
		idxTable1 = mdIdxTable1[nDic + 1];

	inxsize = sizeof(IDX1);
	if ((mid=BinarySearch((unsigned char *)idxTable1, item, low, high, inxsize)) == -1)
	    return(-1);

	low = 0;
	high = ((idxTable1[mid+1].idxoff1 << 8) + idxTable1[mid+1].idxoff2) -
	       ((idxTable1[mid].idxoff1 << 8) + idxTable1[mid].idxoff2);
	item = (mode == SEARCH_CHAR) ? *(cmpkigo+st+1) : *(cmpkigo+st+2);
	fseek(wfp, (long)(((idxTable1[mid].idxoff1 << 8) + idxTable1[mid].idxoff2) * sizeof(IDX2) + MD_IDX2_POS), 0);
	fread(idxTable2, high * sizeof(IDX2),1, wfp);
    for (i = 0; i < high; i++)
    {
        two_invert ((PBYTE) &(idxTable2[i].offset));
        two_invert ((PBYTE) &(idxTable2[i].clen));
    }
	inxsize = sizeof(IDX2);
	if ((mid=BinarySearch((unsigned char *)idxTable2, item, low, high, inxsize)) == -1)
	    return(-1);

	outoff = ((long)(idxTable2[mid].fseg-'0')<<16)+(long)idxTable2[mid].offset;
	if (mode == SEARCH_CHAR) {
	    Core[st>>1].cidx[nDic] = outoff;
	    Core[st>>1].clen[nDic] = idxTable2[mid].clen;
	}
	else
	    Core[st>>1].wlen[nDic] = idxTable2[mid].clen;
    }
    return(0);
}

/*********************************************************************
    Function --- MDGetWordAddress
    Synopsis --- get word address by 3rd compressed code
    Argument --- st:
		 fnum: file reference number
    Return   --- -1: not found
		 0: found
*********************************************************************/
short MDGetWordAddress(short nDic, short st, FILE *wfp)
{
    BYTE	idxBuf[60];
    short	wlen;
    short	byteCount, bitCount;
    short	offset1, offset2, pos;
    short	whichGroup, whichByte, whichBit;

    if ((Core[st>>1].wlen[nDic] & 0xc000) == 0)
	return(-1);  /* no word matching the 1st and 2nd compressed codes */

    whichGroup = (short)(*(cmpkigo+st+1) - '!') / TONE_COUNT;
    whichByte = whichGroup >> 3;
    whichBit = whichGroup % 8;

    wlen = Core[st>>1].wlen[nDic] & 0x3fff;
	fseek(wfp, (long)(outoff+wlen), 0);
	ReadBuffer(idxBuf, wfp, 40);     /* maximum entry is 2+13*2 = 28 */

    if ((idxBuf[whichByte] & (1 << whichBit)) == 0)
	return(-1);  /* no word matching the 1st, 2nd and 3rd compressed codes */

    for (offset1=-2, byteCount=0; byteCount<2; byteCount++) {
	if (idxBuf[byteCount]) {
	    for (bitCount=0; bitCount<8; bitCount++) {
		if (byteCount == whichByte && bitCount == whichBit) pos = offset1;
		if (idxBuf[byteCount] & (1 << bitCount)) offset1 += 2;
	    }
	}
    }

    offset2 = (pos < 0) ? 0 : ((idxBuf[2+pos] & 0xff)|(idxBuf[3+pos] << 8));
    Core[st>>1].widx[nDic] = outoff + wlen + 2 + offset1 + offset2;
    Core[st>>1].wrange[nDic] = ((idxBuf[4+pos] & 0xff)|(idxBuf[5+pos] << 8)) - offset2;
    return(0);
}

/*********************************************************************
    Function --- BinarySearch
    Synopsis --- binary-search the item in table
    Argument --- table: the table to be searched
		 item: the item to be searched
		 low: minimum index
		 high: maximum index
		 size: the size of item
    Return   --- -1: not found
		 >=0: the index of the item in table
*********************************************************************/
short	BinarySearch(PBYTE table, BYTE item, short low, short high, short size)
{
    short mid;
    unsigned char midItem;

   high--;
    while (high >= low) {
	mid = (low+high) >> 1;
	midItem = *(table+size*mid);
	if (item < midItem)
	    high = mid - 1;
	else if (item > midItem)
	    low = mid + 1;
	else
	    return(mid);
    }
    return(-1);
}

/*******************************************************************
    Function --- CompareCmpcode
    Synopsis --- compare two cmpCode
    Argument --- len: the length to compare
    Return   ---
*******************************************************************/
short CompareCmpcode(PBYTE cmpCode1, PBYTE cmpCode2, short len)
{
    short i, diff1, diff2;

/* 6 tones */
/*
    for (i=0; i<len; i++) {
	if (diff1 = *(cmpCode1+i) - *(cmpCode2+i)) {
	    diff2 = (*(cmpCode1+i)-'!') / 6 - (*(cmpCode2+i)-'!') / 6;
	    tone = (*(cmpCode1+i)-'!') % 6;
	    if (i < 2) {
		if (diff2) return(diff1);
		if (tone) return(1);
	    }
	    else if (i & 1) {
		if (tone) return(1);
		if (diff2) return(diff1);
	    }
	    else
		return(1);
	}
    }
    return(0);
*/

/* 5 tones */
    for (i=0; i<len; i++) {
	diff1 = *(cmpCode1+i) - *(cmpCode2+i);
	diff2 = (*(cmpCode1+i)-'!')/TONE_COUNT - (*(cmpCode2+i)-'!')/TONE_COUNT;
	if (i <= 1) {
	    if (diff1) return((diff2 == 0) ? 1 : diff1);
	}
	else if (diff1)
	    return(1);
    }
    return(0);
}

/*********************************************************************
    Function --- ReadBuffer
    Synopsis ---
    Argument --- buf:
		 fnum: file number
		 num: number of bytes to be read
    Return   --- number of bytes actually read
*********************************************************************/
short ReadBuffer(PBYTE buf, FILE *wfp, short num)
{
    short	numRead;

    numRead = fread(buf, 1, num,wfp);
    if (numRead < num)
	*(buf+numRead+1) = 0xff;
    else
	*(buf+numRead) = '\0';
    return(numRead);
}

short	RepSearchWord(short st)
{
    short i;
    static unsigned char rep_cmp_code[12] = {
	'x','=',0xb6,0x56,      /* ¶V..¶V.. */
	'x','%',0xb7,0x55,      /* ·U..·U.. */
	'K','\"',0xa9,0xbf       /* ©¿..©¿.. */
    };

    if (AddrState || (Core[(st+2)>>1].flag & LEN_FG) > 1) return(0);
    for (i=0; i<12; i+=4) {
	if (cmpkigo[st] == rep_cmp_code[i] &&
	    cmpkigo[st+1] == rep_cmp_code[i+1] &&
	    cmpkigo[st+4] == rep_cmp_code[i] &&
	    cmpkigo[st+5] == rep_cmp_code[i+1] &&
	    ((Core[st>>1].flag & CHG_FG) == 0 ||
	     (youwant[st] == rep_cmp_code[i+2] && youwant[st+1] == rep_cmp_code[i+3]))) {
	    chbuf[0] = chbuf[4] = rep_cmp_code[i+2];
	    chbuf[1] = chbuf[5] = rep_cmp_code[i+3];
	    chbuf[2] = youwant[st+2];
	    chbuf[3] = youwant[st+3];
	    chlen = 6;
	    chbuf[chlen] = '\0';
	}
    }
    return(chlen);
}

void	two_invert(unsigned char *s)
{
#ifdef	_MAC_
    unsigned char   t;

    t = s[0];
    s[0] = s[1];
    s[1] = t;
#endif
}

