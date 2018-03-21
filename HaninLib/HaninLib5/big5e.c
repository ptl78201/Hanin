#include "hanin.h"

#define		MAXEXTIDX		1100
#define		MAXEXTCHAR		5400

char	szTone[] = "¡È£½£¾£¿£»";

char	szZhuin[] = "£t£u£v£w£x£y£z£{£|£}£~£¡£¢£££¤£¥£¦£§£¨£©£ª£¸£¹£º£«£¬£­£®£¯£°£±£²£³£´£µ£¶£·";

typedef	struct	t_big5eidx
{
	char	szCmp[2];
	short	nStart;
	short	nLen;

}	BIG5EIDX, *PBIG5EIDX;

short		nIdxLen = 0;
short		nCharLen = 0;
char		big5echar[MAXEXTCHAR + MAXEXTCHAR];
BIG5EIDX	big5eidx[MAXEXTIDX];

char	big5e_dic[] = "D:\\Apple\\big5-e\\BIG5E-P1_last.txt";

short	ReadBig5E (char	*big5e_dic);
short	GetBig5EChar (char *pCmp, char *pCharBuf, short num);
short	AddACmp (PBIG5EIDX pIdx, char *pZhuin, short len);
void	CompressZhuin(PBYTE zCode, PBYTE cmpCode);
short	CompareCmp (char *pCmp1, char *pCmp2);

short	ReadBig5E (char	*big5e_dic)
{
    FILE	*fp;
	char	szRead[128];
	char	szCurZhuin[10];
	short	i, len;
	short	nLine = 0;

    if ((fp = fopen (big5e_dic, "rt")) == NULL) 
		return 0;

	szCurZhuin[0] = 0;
	nIdxLen = 0;
	nCharLen = 0;

	while (fscanf (fp, "%s", szRead) != EOF)
	{
		nLine++;

		len = strlen (szRead);
		if (len < 4 || len > 10)
			continue;

		for (i = 0; i < 10; i += 2)
		{
			if (szRead[len - 2] == szTone[i] && szRead[len - 1] == szTone[i + 1])
				break;
		}
		if (i == 10)
		{
			szRead[len] = szTone[0];
			szRead[len + 1] = szTone[1];
			szRead[len + 2] = 0;
			len += 2;
		}

		if (szCurZhuin[0] == 0 || strcmp (szCurZhuin, &szRead[2]) != 0)
		{
			if (nIdxLen == MAXEXTIDX)
				continue;
			
			if (AddACmp (&big5eidx[nIdxLen], &szRead[2], len - 2) == 0)
				continue;
			else if (nIdxLen > 0)
			{
				if (CompareCmp (big5eidx[nIdxLen].szCmp, big5eidx[nIdxLen - 1].szCmp) != 1)
					continue;
			}
			big5eidx[nIdxLen].nStart = nCharLen;
			big5eidx[nIdxLen].nLen = 0;
			strcpy (szCurZhuin, &szRead[2]);
			nIdxLen++;
		}
		big5echar[nCharLen++] = szRead[0];
		big5echar[nCharLen++] = szRead[1];
		big5eidx[nIdxLen - 1].nLen += 2;
	}
	fclose (fp);
	return 1;
}

// num = 1: one char
// num = others: all chars
short	GetBig5EChar (char *pCmp, char *pCharBuf, short num)
{
	short	rc;
	short	midIdx, lowIdx, highIdx;

	if (nIdxLen == 0)
		return 0;

	lowIdx = 0;
	highIdx = nIdxLen - 1;
	midIdx = (lowIdx + highIdx) / 2;

	while (highIdx >= lowIdx)
	{
		midIdx = (lowIdx + highIdx) / 2;
		rc = CompareCmp (pCmp, big5eidx[midIdx].szCmp);
		if (rc < 0)
			highIdx = midIdx - 1;
		else if (rc > 0)
			lowIdx = midIdx + 1;
		else
		{
			if (num == 1)
			{
				memcpy (pCharBuf, &big5echar[big5eidx[midIdx].nStart], 2);
				return 2;
			}
			else
			{
				memcpy (pCharBuf, &big5echar[big5eidx[midIdx].nStart], big5eidx[midIdx].nLen);
				return big5eidx[midIdx].nLen;
			}
		}
	}
	return 0;
}

short	AddACmp (PBIG5EIDX pIdx, char *pZhuin, short len)
{
	BYTE	szCode[5];
	short	step, curstep = 0;
	short	i, j;

	for (i = 0; i < len; i += 2)
	{
		for (j = 0; j < 37; j++)
		{
			if (pZhuin[i] == szZhuin[j << 1] &&
				pZhuin[i + 1] == szZhuin[(j << 1) + 1])
			{
				szCode[i >> 1] = (char) j;
				if (j <= 20)
					step = 1;
				else if (j <= 23)
					step = 2;
				else
					step = 3;

				if (step <= curstep)
					return 0;
				curstep = step;
				break;
			}
		}
		if (j == 37)
		{
			for (j = 0; j < 5; j++)
			{
				if (pZhuin[i] == szTone[j << 1] &&
					pZhuin[i + 1] == szTone[(j << 1) + 1])
				{
					szCode[i >> 1] = 38 + j;
					step = 4;

					if (step <= curstep)
						return 0;
					curstep = step;
					break;
				}
			}
		}
	}

	szCode[(i >> 1) + 1] = 0;
	CompressZhuin (szCode, (PBYTE) pIdx->szCmp);

	return 1;
}
/*
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
	cmpCode[1] += (zCode[i++] - 0x17) * 6;
    cmpCode[1] += zCode[i] - 0x25;
}
*/
short	CompareCmp (char *pCmp1, char *pCmp2)
{
	if (pCmp1[0] < pCmp2[0])
		return -1;
	else if (pCmp1[0] == pCmp2[0])
	{
		if (pCmp1[1] < pCmp2[1])
			return -1;
		else if (pCmp1[1] == pCmp2[1])
			return 0;
		else
			return 1;
	}
	else
		return 1;
}
