#include "hanin.h"


short big5cmp(const unsigned char *s, short start, short len)
{
    short i;

    for (i=0; i<len-2; i+=2) {
	if ((Core[(start+i)>>1].flag & CHG_FG) &&
	    (youwant[start+i] != (*(s+i)^HANIN_CRYPT) ||
	    youwant[start+i+1] != (*(s+i+1)^HANIN_CRYPT))) return(-1);
    }
    return(0);
}

void	init_len_mark(short i)
{
	short	n;

	for (n = 0; n < MAXPROFDICOPENED + 1; n++)
	{
		Core[i].clen[n] = Core[i].wlen[n] = -1;
		Core[i].cidx[n] = Core[i].widx[n] = -1;
		Core[i].wrange[n] = -1;
		Core[i].mlenmark[n] = -1;
	}
	Core[i].ulenmark = -1;
}

void	AppendToBuffer(unsigned char *code, short count, short st, short len, short mode)
{
    short i, j, k, olen;

    for (i=0; i<count; i+=len) {
	for (j=0; j<len; j++) chbuf[chlen+j] = *(code+i+j) ^ HANIN_CRYPT;
	olen = chlen;
	chlen += len;	
	if (mode == SEARCH_ALL) {
	    for (j=0; j<len && chbuf[olen+j] == youwant[st+j]; j++);
	    if (j == len) continue;
	    for (k=0; k<oldchlen; k+=len) {
		for (j=0; j<len && chbuf[olen+j] == chbuf[k+j]; j++);
		if (j == len) break;
	    }
	    if (k < oldchlen) continue;
	}
    }
    chbuf[chlen] = '\0';
}

