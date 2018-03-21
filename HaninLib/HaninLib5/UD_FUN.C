#include "hanin.h"

#define		RC_RANGEERROR		-101

void	insert_list (PBYTE in_code, PBYTE in_char, int strSize);
int		strcmp2 (char *s,char *t);
void	Mk2_Word();

short	GetExtVacancy ();
short	HasHalfWord (short num);

// for user word maintenance
short		sort_num;
PSORTEDLIST	sort_list;

/*********************************************************************
    Function --- UDGetHashIndex
    Synopsis --- get hash index according to compressed codes
    Argument --- cmpCode: the buffer of compressed codes
    Return   --- hash index
*********************************************************************/
short	UDGetHashIndex(PBYTE cmpCode)
{
    return((*cmpCode + *(cmpCode+1) + *(cmpCode+2) + *(cmpCode+3)) % UD_MAX_IDX);
}

/*********************************************************************
    Function --- UDSearchWord
    Synopsis --- search word(s) in UD
    Argument --- st:
		 len: length of word
		 mode: SEARCH_ONE(0) for only one, SEARCH_ALL(1) for all
    Return   --- 0: not found
		 >0: number of bytes of found words
*********************************************************************/
short	UDSearchWord(short st, short len, short mode)
{
    FILE	*u_fp;
    BYTE	cmpCode[11], temp;
    short	i, idx, byteCount;
    short	beginPos, endPos, range;

    oldchlen = chlen;
    strncpy((char *)cmpCode, (char *)cmpkigo+st, len);

    if (Core[st>>1].ulenmark != 0xff) 
	{
		if ((Core[st>>1].ulenmark & (Char)(1 << ((len-2)>>1))) == 0) // no UD word
			return(0);
    }
    else
		Core[st>>1].ulenmark = 0;

    temp = cmpCode[1];
    cmpCode[1] = cmpCode[2];
    cmpCode[2] = temp;
    idx = UDGetHashIndex(cmpCode);
    beginPos = USR_UD_POS + UD_IDX_SIZE + udHashIdx[idx];
    endPos = USR_UD_POS + UD_IDX_SIZE + udHashIdx[idx+1];
    if ((range=endPos-beginPos) <= 0 || range > 1024) return(0);

    if ((u_fp=fopen(user_dic, "r+")) == NULL) return(0);
    fseek(u_fp, (long)beginPos,  0);
    fread(toutbuf,1, range,u_fp);
    fclose(u_fp);

    for (i=0; i<range; i+=byteCount<<1) {
	for (byteCount=4; toutbuf[i+byteCount]<0x80; byteCount += 2);
	if (strncmp((char *)cmpCode, (char *)toutbuf+i, 4) == 0)
	    Core[st>>1].ulenmark |= 1 << ((byteCount-2)>>1);
	if ((mode == SEARCH_ALL || chlen == oldchlen) &&
	    byteCount == len && strncmp((char *)cmpCode, (char *)toutbuf+i, len) == 0 &&
	    (mode == SEARCH_ALL || big5cmp(toutbuf+i+len, st, len) == 0)) {
	    AppendToBuffer(toutbuf+i+len, len, st, len, mode);
	}
    }
    freq_fg = FRQ_FG;
    return(chlen-oldchlen);
}

/*********************************************************************
    Function --- UDInsertWord
    Synopsis --- insert word in UD
    Argument ---
*********************************************************************/
short	UDInsertWord(PBYTE cmpCode, PBYTE bigCode, short len)
{
    FILE	*u_fp;
    BYTE	cmp[11], temp;
    short	i, j, idx, byteCount;
    short	udNeedSize, udUsedSize;
    short	beginPos, endPos, range;

    if (len < 4 || len > 10) 
		return RC_INVALIDWORD;

    for (i=0; i<len; i+=2) 
	{
		if (*(cmpCode+i) >= '~' || *(bigCode+i) < 0xa4) 
			return RC_INVALIDWORD;
    }

    udNeedSize = len << 1;
    udUsedSize = udHashIdx[UD_MAX_IDX];
    if (udUsedSize+udNeedSize > UD_MAX_SIZE) 
		return RC_USERWORDFULL;
    
    strncpy((char *)cmp, (char *)cmpCode, len);
    temp = cmp[1];
    cmp[1] = cmp[2];
    cmp[2] = temp;

    idx = UDGetHashIndex(cmp);
    beginPos = USR_UD_POS + UD_IDX_SIZE + udHashIdx[idx];
    endPos = USR_UD_POS + UD_IDX_SIZE + udHashIdx[idx+1];
    if ((range=endPos-beginPos)+udNeedSize > 1024) 
		return RC_USERWORDFULL;

    if ((u_fp=fopen(user_dic, "r+")) == NULL)
		return RC_OTHERERROR;

    fseek(u_fp, (long)beginPos,  0);
    fread(toutbuf,1, range,u_fp);
    fclose(u_fp);

    for (i=0; i<range; i+=byteCount<<1) 
	{
		for (byteCount=4; toutbuf[i+byteCount]<0x80; byteCount += 2);
		if (byteCount == len && strncmp((char *)cmp, (char *)toutbuf+i, len) == 0) 
		{
			for (j=0; j<len; j++)
			{
				if (*(bigCode+j) != (toutbuf[i+len+j] ^ HANIN_CRYPT)) 
					break;
			}
			if (j == len) 
				return RC_WORDEXIST;   /* UD word exist */
		}
    }

    UDWrite(cmp, bigCode, len);
    return RC_SUCCESS;
}

void	UDWrite(PBYTE cmpCode, PBYTE bigCode, short len)
{
    FILE	*u_fp;
    BYTE	cryptCode[11];
    short	idx;
    short	pos, size, i;
    short	move_sta, nxt_end;
	PBYTE	u;

	if ((u_fp=fopen(user_dic, "r+")) == NULL) 
		return;

    idx = UDGetHashIndex(cmpCode);
    move_sta = USR_UD_POS + UD_IDX_SIZE + udHashIdx[idx+1];
    nxt_end = USR_UD_POS + UD_IDX_SIZE + udHashIdx[UD_MAX_IDX];
    for (pos=nxt_end-512; ; pos-=512) 
	{
		if (pos < move_sta) 
		{
			pos = move_sta;
			if ((size=nxt_end-pos) <= 0) 
				break;
		}
		else 
		{
			nxt_end = pos;
			size = 512;
		}
		fseek(u_fp, (long)pos, 0);
		fread((unsigned char *)toutbuf,1, size,u_fp);
		fseek(u_fp, (long)(pos+(len<<1)), 0);
		fwrite((unsigned char *)toutbuf,1, size,u_fp);
		if (size < 512) 
			break;
    }

    for (i=0; i<len; i++)  
		cryptCode[i] = bigCode[i] ^ HANIN_CRYPT;
    fseek(u_fp,(long)move_sta, 0);
    fwrite((unsigned char *)cmpCode,1, len,u_fp);
    fwrite((unsigned char *)cryptCode,1, len,u_fp);

    for (i=idx+1; i<=UD_MAX_IDX; i++)
		udHashIdx[i] += len<<1;

	u = (unsigned char *)udHashIdx;
	for(i=0; i<UD_IDX_SIZE; i+=2)
		two_invert(u+i);

    fseek(u_fp, (long)USR_UD_POS, 0);
    fwrite((unsigned char *)udHashIdx,1, (unsigned short)UD_IDX_SIZE,u_fp);

	u = (unsigned char *)udHashIdx;
	for(i=0; i<UD_IDX_SIZE; i+=2)
		two_invert(u+i);

    
    fclose(u_fp);
}

short	q_extend()
{
	FILE	*u_fp;
	short   i,ccount,loc;

	if (j_off != 4 && j_off != 6)
		return 0;

	if ((u_fp=fopen(user_dic, "r+")) == NULL) 
		return(0);

	fseek(u_fp, (long)USR_AD_POS, 0);
	fread(toutbuf,1, (unsigned short)AD_IDX_SIZE,u_fp);

	for (ccount=i=0; i<AD_IDX_SIZE; ccount++, i+=6) 
	{
		if ((j_off != 4 || toutbuf[i+4] == '\0') &&
		    strncmp((char *)youwant, (char *)toutbuf+i, j_off) == 0) 
			break;
	}

	if (i >= AD_IDX_SIZE)
	{
		fclose(u_fp);
		return 0;
	}
	else 
	{
		loc = USR_AD_POS + AD_IDX_SIZE + 4 + (ccount<<6);
		fseek(u_fp, (long)loc, 0);
		fread(chbuf,1, 64,u_fp);
		for (i=0; i<64; i++) 
			chbuf[i] ^= HANIN_CRYPT;
	}
	fclose(u_fp);

	return 1;
}

short	UDLoad()
{
	FILE	*u_fp;
	BYTE	*u;
	short	i;

	if ((u_fp=fopen(user_dic, "rb")) == NULL) 
		return(-1);

	fseek(u_fp, (long)USR_UD_POS, 0);
	fread((unsigned char *)udHashIdx,1, (unsigned short)UD_IDX_SIZE,u_fp);
	fread((unsigned char *)udHashBuf,1, (unsigned short)UD_MAX_SIZE,u_fp);
	
	u = (unsigned char *)udHashIdx;
	for(i=0; i<UD_IDX_SIZE; i+=2)
		two_invert(u+i);

	fclose(u_fp);
	return 0;
}

/********************************************************************
Function : hash_to_sort
Purpose  : fetch words in HASHBANK buffer and place them into sort_list
	   buffer.
Note	 : modify on 10/14,1992
********************************************************************/
int		hash_to_sort ()
{
	int		i, j, strSize;
	BYTE	tempbuf[10];
    
	sort_num = 0;
	sort_list = (PSORTEDLIST) calloc (2048, sizeof(SORTEDLIST)); 
   // max 2048 word(2 Chinese char)
	
	for (i = 0; i < udHashIdx [UD_MAX_IDX]; i += (strSize << 1))
	{
		for (strSize = 0; i + (strSize << 1) < udHashIdx [UD_MAX_IDX] && 
			udHashBuf [i + strSize] < 0x80; strSize += 2);
		if (strSize == 0)
			break;
		for (j = 0; j < strSize; j++) 
			udHashBuf [i + strSize + j] ^= HANIN_CRYPT2A;
		if (i+strSize >= UD_MAX_SIZE) 
		{
			free (sort_list); 
			return -2;	
		}	
		insert_list (udHashBuf + i, udHashBuf + i + strSize, strSize);
	}
	if (i < udHashIdx[UD_MAX_IDX])
	{
		for (j = 0; j <= UD_MAX_IDX; j++)
			udHashIdx [j] = min (udHashIdx [j], i);
		
		for (i = 0; i < udHashIdx[UD_MAX_IDX]; i += (strSize << 1))
		{
			for (strSize = 0; i + (strSize << 1) < udHashIdx[UD_MAX_IDX] && 
				udHashBuf [i + strSize] < 0x80; strSize += 2);
			if (strSize == 0)
				break;
			for (j = 0; j < strSize; j++) 
				udHashBuf [i + strSize + j] ^= HANIN_CRYPT2A;
		}
		//UDWrite ();
		for (i = 0; i < udHashIdx [UD_MAX_IDX]; i += (strSize << 1))
		{
			for (strSize = 0; i + (strSize << 1) < udHashIdx [UD_MAX_IDX] && 
				udHashBuf [i + strSize] < 0x80; strSize += 2);
			if (strSize == 0)
				break;
			for (j = 0; j < strSize; j++) 
				udHashBuf [i + strSize + j] ^= HANIN_CRYPT2A;
		}
	}

	for (i = 0; i < sort_num; i++)
	{
		for (j = 0; j < (sort_list+i)->strSize; j++)
			tempbuf[j] = *((sort_list+i)->char_code+j);
	    tempbuf[j]=0;
	    //SendDlgItemMessage (hDlg, IDC_LISTWORD, LB_ADDSTRING, 0, (LONG)(LPSTR) tempbuf);
	}
	return (sort_num);
}
/********************************************************************
Function : insert_list
Purpose  : insert new record to sort_list in ascending order
********************************************************************/
void	insert_list (PBYTE in_code, PBYTE in_char, int strSize)
{
	int		i, j;

	for (i = 0; i < sort_num; i++)
	{
		if ((j = strcmp2 (in_code, (sort_list + i)->cmp_code)) < 0)
			break;
	}
	if (i < sort_num)
	{
		for (j = sort_num; j >= i; j--)
			*(sort_list + j + 1) = *(sort_list + j);
    }
	(sort_list + i)->strSize = strSize;
	(sort_list + i)->cmp_code = in_code;
	(sort_list + i)->char_code = in_char;
	sort_num++;
}

/********************************************************************
Function : Mk2_Word
Purpose  : place sorted records to hash memory buffer
********************************************************************/
void	Mk2_Word()
{
	int 	num, cmp, size;
	int 	i, j, k, n, byteLen;

	num = 0;
	size = 0;
	for (i = 0; i < UD_MAX_IDX; i++) 
	{
		for (j = udHashIdx [i]; j < udHashIdx [i + 1]; j += byteLen << 1) 
		{
			for (byteLen = 4; udHashBuf [j + byteLen] < 0x80; byteLen += 2);
			for (k = 0; k < sort_num; k++) 
			{
				if ((sort_list + k)->cmp_code) 
				{
					for (cmp = 0; cmp < byteLen; cmp++) 
					{
						if (*((sort_list + k)->cmp_code + cmp) != udHashBuf [j + cmp] ||
							*((sort_list + k)->char_code + cmp) != udHashBuf [j + byteLen + cmp])
							break;
					}
					if (cmp == byteLen && *((sort_list + k)->cmp_code + cmp) > 0x80) 
					{
						(sort_list + k)->cmp_code = 0;
						for (n = 0; n < byteLen; n++) 
							udHashBuf [num + n] = udHashBuf [j + n];
						
						for (n = 0; n < byteLen; n++) 
							udHashBuf [num + byteLen + n] = udHashBuf [j + byteLen + n] ^ HANIN_CRYPT2A;
						
						num += (byteLen << 1);
					}
				}
			}
		}
		udHashIdx [i] = size;
		size = num;
	}
    udHashBuf [num + 1] = 0;
	udHashIdx [UD_MAX_IDX] = size;
}

/********************************************************************
Function :  GetUWordIndex
Purpose  :  focus to nearest indexing record
********************************************************************/
short	GetUWordIndex (short nn)
{
	char	cmp_char[3], cmp[3];
	short	i,j;

	cmp_char[2] = cmp[2] = 0x00;
	if (nn < 21)
	{
		cmp_char[0] = (char)(0x21+nn*4);
		cmp_char[1] = 0x22;
	}
	else if (nn < 34)
	{
		cmp_char[0] = 'u';
		cmp_char[1] = (char)(0x28+(nn-21)*6);
	}
	else
	{
		cmp_char[0] = (char)(0x76+(nn-34));
		cmp_char[1] = 0x22;
	}
	for (i = 0;i < sort_num; i++)
	{
		cmp[0] = (sort_list+i)->cmp_code[0];
		cmp[1] = (sort_list+i)->cmp_code[2];
		if ((j = strcmp2(cmp_char, cmp)) <= 0)
			return(i);
	}
	return (sort_num);
}

/********************************************************************
Function : SaveUerWord
Purpose  : save records to user-defined file
********************************************************************/
void	SaveUerWord()
{
	FILE 	*file;
	PBYTE	u;
	short	j;

	file = fopen(user_dic, "r+");
	if (file == NULL)
		return;

	Mk2_Word ();

	fseek (file, USR_UD_POS, 0);
	u = (unsigned char *)udHashIdx;
	for(j=0; j<UD_IDX_SIZE; j+=2)
		two_invert(u+j);
	fwrite(udHashIdx, 1, (UD_MAX_IDX+1)<<1, file);
	u = (unsigned char *)udHashIdx;
	for(j=0; j<UD_IDX_SIZE; j+=2)
		two_invert(u+j);

	fwrite(udHashBuf, 1, UD_MAX_SIZE, file);
	fclose(file);

	free (sort_list);
}

short	GetFreeWords ()
{
	return (short) ((int)UD_MAX_SIZE - udHashIdx[UD_MAX_IDX]) / 4; 
}

/********************************************************************
Function : do_omit
Purpose  : omit the specified record
********************************************************************/
void	do_omit (short nn)
{
	int		j;

	for (j = nn; j<sort_num; j++)
		*(sort_list+j) = *(sort_list+j+1);
    sort_num--;
}

/********************************************************************
Function : GetJuinList
Purpose  : display Juin symbol to query dialog
********************************************************************/
short	GetJuinList (PBYTE pJuin)
{
	int		i, len = 0;
	BYTE	tmp[4];

	tmp[0]=0xa3;
	tmp[1]=0x74;
	tmp[2]=0x00;
	for (i = 0; i < 11; i++)
	{
		memcpy (&pJuin[len], tmp, 2);
		len += 2;
		tmp[1]++;
	}
	tmp[1] = 0xa1;
	for (i = 0; i < 26; i++)
	{  
		memcpy (&pJuin[len], tmp, 2);
		len += 2;
		tmp[1]++;
	}
	return len;
}

/********************************************************************
Function : strcmp2
Purpose  : string compare
********************************************************************/
int		strcmp2 (char *s,char *t)
{
    for (;*s == *t; s++, t++)
    {
		if (*s == '\0')
	    	return(0);
	}
    return (*s - *t);
}

// extensions
short	DelExt (PBYTE extbuf)
{
	FILE	*file;
	BYTE	szTemp[6];
	short 	n;

    n = FindExtID (extbuf);
    
    if (n >= 0)
	{
		file = fopen(user_dic, "r+");
		if (file == NULL)
			return RC_OTHERERROR;

		memset (szTemp, 0, 6);
		fseek (file, OFF_QUICKIDX + 6 * n, 0);
		fwrite (szTemp, 1, 6, file);
		memset (&idxbuff[6 * n], 0, 6);

		fclose (file);
		return RC_SUCCESS;
	}
	else
		return RC_NOTFOUND;
}

short	RegExt (PBYTE extbuf, PEXTENSION pExtension)
{
	int 	i, num, n;
	long	loc;
	FILE	*file;
	BYTE	szTemp[42];

	n = strlen (extbuf);
	if (n < 4 || n > 6 || n / 2 * 2 != n)
		return RC_INVALIDEXTID;

	for (i = 0; i < n; i += 2)
	{
		if (extbuf[i] < 0xA0)
			return RC_INVALIDEXTID;

		if (extbuf[i] == 0xa1 && extbuf[i+1] == 0x40)
			return RC_INVALIDEXTID;
	}

	if (strlen (pExtension->szLong) == 0)
		return RC_NOLONGEXT;

    n = FindExtID (extbuf);

    if(n >= 0)
		return RC_ALREADYEXIST;
    else if (n != RC_NOTFOUND)
		return RC_OTHERERROR;
    else
    {
		num = GetExtVacancy ();
		if (num == -1)
			return RC_EXTENSIONFULL;

		file = fopen(user_dic, "r+");
		if (file == NULL)
			return RC_OTHERERROR;

		memset (szTemp, 0, 6);
		strcpy (szTemp, extbuf);
		fseek (file, OFF_QUICKIDX + 6 * num, 0);
		fwrite (szTemp, 1, 6, file);
		memcpy (&idxbuff[6 * num], szTemp, 6);
		
		loc = OFF_QUICK + (long) num * 64;
		fseek (file, loc, 0);
		strcpy (szTemp, pExtension->szLong);
		for (i = 0; i < 40; i++)
			szTemp[i] = szTemp[i] ^ HANIN_CRYPT2A;
		fwrite (szTemp, 1, 40, file);

		strcpy (szTemp, pExtension->szShort);
		for (i = 0; i < 24; i++)
			szTemp[i] = szTemp[i] ^ HANIN_CRYPT2A;
		fwrite (szTemp, 1, 24, file);

		fclose (file);
    }
    return RC_SUCCESS;
}

short	FindExtID (PBYTE extbuf)
{
	int 	state, len;
	int 	i,j,num,findflag;

	len = strlen (extbuf);

	state = 0;
	if (len < 4 || len > 6)
		state = RC_RANGEERROR; // range error
	for (i = 0; state == 0, i < len; i += 2)
    {
		if (extbuf[i] < 0xA0)
		{
			state = RC_INVALIDEXTID;
			break;
		}
		if (extbuf[i] == 0xa1 && extbuf[i+1] == 0x40)
		{
			state = RC_INVALIDEXTID;
			break;
		}
    }
    num = 0;
    findflag = 0;
	for (i = 0; i < QD_IDXTABLE_SIZE; i += QD_IDX_SIZE)
    {
		for (j = 0; j < len; j++)
		{
			if (extbuf[j] != idxbuff[i + j])
				break;
		}
		if (j == 6 || (j == len && idxbuff[i + len] == 0))
		{
			findflag = 1;
			break;
		}
		num++;
	}
	if (state != 0)
		return (state);

	if (findflag == 1)
		return (num);
	else
		return RC_NOTFOUND;
}

short	GetExt (short idx, PEXTENSION pExtension)
{
	int 	i;
	long	loc;
	FILE 	*file;

	if (idx < 0 || idx > AD_MAX_WORD) 
	{
		pExtension->LongExtLength = 0;
		pExtension->ShortExtLength = 0;
		pExtension->szLong[0] = 0;
		pExtension->szShort[0] = 0;
		return 0;
	}

	loc = OFF_QUICK + (long)idx * 64L;
	
	file = fopen(user_dic, "rb");
	if (file == NULL)
	{
		pExtension->LongExtLength = 0;
		pExtension->ShortExtLength = 0;
		pExtension->szLong[0] = 0;
		pExtension->szShort[0] = 0;
		return 0;
	}
	
	pExtension->szLong[40] = 0;
	pExtension->szShort[24] = 0;

	fseek (file,loc,0);
	fread (pExtension->szLong, 1, 40, file);
	fread (pExtension->szShort, 1, 24, file);
	fclose (file);
	for (i = 0; i < 40; i++)
		pExtension->szLong[i] ^= HANIN_CRYPT2A;
    for (i = 0; i < 24; i++)
		pExtension->szShort[i] ^= HANIN_CRYPT2A;

	pExtension->LongExtLength = strlen (pExtension->szLong);
	pExtension->ShortExtLength = strlen (pExtension->szShort);

	return 1;
}

short	GetExtVacancy ()
{
	short	i;

	for (i = 0; i < AD_MAX_WORD; i++)
    {
		if (idxbuff[i * 6] == 0)
			return i;
	}
	return -1;
}

/*
short	HasHalfWord (short num)
{
	short	i = num * QD_IDX_SIZE;
	
	if (idxbuff[i] < 0xA1) 
		return True;
	if (idxbuff[i+2] < 0xA1) 
		return True;	
	if (idxbuff[i+4] == 0) 
		return False;
	if (idxbuff[i+4] < 0xA1) 
		return True;

	return False;
}                                                  
*/

