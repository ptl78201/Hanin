#include "hanin.h"

void	two_invert(unsigned char *s);
void	Encrypt (PBYTE pByte, short nLength, BYTE CryptCode);
void	Decrypt (PBYTE pByte, short nLength, BYTE CryptCode);

short	sear_aimei(char *pron, char *aimei_pron)
{
    long	loc;
    FILE	*a_fp;
    short	len;
    USHORT	offset[2];

    if ((a_fp=fopen(aimei_dic, "rb")) == NULL) return(-1);

    loc = ((*pron-'!') * 84 + (*(pron+1)-'!') - 1) << 1;
    fseek(a_fp, loc, 0);

    fread(offset, 1, 4 ,a_fp);

	two_invert((unsigned char *)&offset[1]);
	two_invert((unsigned char *)&offset[0]);
    len = offset[1] - offset[0];
    loc = (long)offset[0];
    fseek(a_fp, loc, 0);
    fread(aimei_pron,1, len, a_fp);
    fclose(a_fp);

    return(len>>1);
}

short	cmp2zhuin(PBYTE	cmpCode, PBYTE	zhuin)
{
	unsigned char  hi, lo;
	unsigned short na, nb, nc;
	unsigned short nh[4];
	short i, j;

	hi = *cmpCode;
	lo = *(cmpCode+1) & 0x7f;

	na = hi - 0x21;
	nb = na >> 2;
	nc = na % 4;

	nh[0] = (hi >= 0x21 && hi <= 0x74) ? 0xa374 + nb : 0xa3bc;
	if (nh[0] > 0xa37e && nh[0] != 0xa3bc)
		nh[0] = 0xa3a1 + nh[0] - 0xa37e - 1;

	nh[1] = (nc != 0) ? 0xa3b7 + nc : 0xa3bc;

	na = lo - 0x21;
	nb = na / TONE_COUNT;
	nc = na % TONE_COUNT;

	nh[2] = (lo > 0x26 && lo <= 0x74) ? 0xa3aa + nb : 0xa3bc;

	if (nc == 1) nh[3] = 0xa1c8;
	else nh[3] = (nc != 5)	? 0xa3bb + nc : 0xa3bb;

	for (i=j=0; i<=3; i++) {
		if (nh[i] == 0xa3bc) continue;
		zhuin[j++] = nh[i] >> 8;
		zhuin[j++] = nh[i] & 0xff;
	}
	zhuin[j] = 0;
	return(j);
}

short	GetSameProns (PHANIN pHanin, short searchlen, PBYTE pCandBuf, short CandBufSize, short bSpecCharOnly)
{
	short	i, j;
	short	len;

	if (g_bReady == False)
		return 0;

	if (pHanin == NULL)
		return 0;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	specChoice = bSpecCharOnly? 1:0;

	if (ownoff != 0)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return 0;
	}

	if ((j_off == f_want && j_off < searchlen) || (j_off != f_want && j_off - f_want < searchlen))
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return 0;
	}
	if (j_off == f_want)
	{
		shift_cursor ((short)(-searchlen), 1);
		ENDSEL = 1;
	}

	chlen = 0; *chbuf = '\0';
	if (searchlen == 4)
		SearchAllWords();
	else
		SearchAllChars();

	if (chlen > 0 && CandBufSize >= searchlen)
	{
		len = 0;
		for (i = 0; i < chlen; i += searchlen)
		{
			// skip some chars to meet apple's requirement
			if (searchlen == 2 && cmpkigo[f_want] == '~')
			{
				switch (cmpkigo[f_want + 1])
				{
				case '!':
				case '1':
				case ' ':
					if (chbuf[i] == 0xa1 && chbuf[i + 1] == 0x49)
						continue;

					if (cmpkigo[f_want + 1] != ' ')
						break;

				case '@':
				case '2':
					if (chbuf[i] == 0xa2 && chbuf[i + 1] == 0x49)
						continue;

					if (cmpkigo[f_want + 1] != ' ')
						break;

				case '#':
				case '3':
					if (chbuf[i] == 0xa1 && chbuf[i + 1] == 0xad)
						continue;

					if (cmpkigo[f_want + 1] != ' ')
						break;

				case '$':
				case '4':
					if (chbuf[i] == 0xa2 && chbuf[i + 1] == 0x43)
						continue;

					if (cmpkigo[f_want + 1] != ' ')
						break;

				case '%':
				case '5':
					if (chbuf[i] == 0xa2 && chbuf[i + 1] == 0x48)
						continue;

					if (cmpkigo[f_want + 1] != ' ')
						break;

				case '&':
				case '7':
					if (chbuf[i] == 0xa1 && chbuf[i + 1] == 0xae)
						continue;

					if (cmpkigo[f_want + 1] != ' ')
						break;

				case '?':
				case '/':
					if (chbuf[i] == 0xa1 && chbuf[i + 1] == 0x48)
						continue;

					break;
				}
			}

			for (j = 0; j < len; j += searchlen)
			{
				if (strncmp (chbuf + i, pCandBuf + j, searchlen) == 0)
					break;
			}
			if (j == len)
			{
				strncpy (pCandBuf + len, chbuf + i, searchlen);
				len += searchlen;

				if (len + searchlen > CandBufSize)
				{
					pCandBuf[len] = 0;
					Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

					return len;
				}
			}
		}
	}
	else
		len = 0;

	if (ENDSEL)
	{
		shift_cursor (searchlen, 1);
		ENDSEL = 0;
	}

	pCandBuf[len] = 0;
	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return len;
}

short	SelectSamePron (PHANIN pHanin, short selectlen, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	short		i;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return 0;
	}

	if ((j_off == f_want && j_off < selectlen) || (j_off != f_want && j_off - f_want < selectlen))
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return 0;
	}

	if (j_off == f_want)
	{
		shift_cursor ((short)(-selectlen), 1);
		ENDSEL = 1;
	}

	for (i = 0; i < selectlen; i++)
	{
		youwant[f_want + i] = pSelBuf[i];
		pWA->chDisplay[f_want + i] = pSelBuf[i];
	}
	InsertLearnCode (selectlen);

	if (ENDSEL)
	{
		shift_cursor (selectlen, 1);
		ENDSEL = 0;
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	HAN_GetConvResult (pHanin, pConvResult);

	return True;
}

short	SelectAimeiPron (PHANIN pHanin, PAIMEI pAimei, short idx, short selectlen, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	short			i;
	PAIMEISTRUCT	pAimeiStruct;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	if (pAimei == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	pAimeiStruct = (PAIMEISTRUCT) pAimei;
	Decrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);

	if (idx < 0 || idx >= pAimeiStruct->nCount)
	{
		Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return 0;
	}

	if (ownoff != 0)
	{
		Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return False;
	}

	if ((j_off == f_want && j_off < selectlen) || (j_off != f_want && j_off - f_want < selectlen))
	{
		Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return False;
	}

	if (j_off == f_want)
	{
		shift_cursor (-2, 1);
		ENDSEL = 1;
	}

	for (i = 0; i < 2; i++)
		cmpkigo[f_want + i] = pAimeiStruct->szAimeiPron[(idx<<1)+i];
	
	if (DisplayMode == DISPLAY_ALPHA)
		cmp2pinin(&cmpkigo[f_want], Core[f_want>>1].nowkey);
	else
		cmp2zhuin(&cmpkigo[f_want], Core[f_want>>1].nowkey);
	
	if (selectlen == 4 && ENDSEL)
		shift_cursor (-2, 1);

	for (i = 0; i < selectlen; i++)
	{
		youwant[f_want + i] = pSelBuf[i];
		pWA->chDisplay[f_want + i] = pSelBuf[i];
	}
	InsertLearnCode (selectlen);

	if (selectlen == 4 && ENDSEL)
		shift_cursor (2, 1);

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}

	Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	HAN_GetConvResult (pHanin, pConvResult);

	return True;
}
