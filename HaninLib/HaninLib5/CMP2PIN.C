extern char	PininMode;

#define TC 6	 /* tone count */
#define TT 0x80  /* zhuin2 pinin */
#define MM 0x40  /* Roman pinin */
#define YY 0x20  /* with consonant */
#define NN 0x10  /* without consonant */
#define XX 0x00  /* without i,u,v */
#define II 0x01  /* with i */
#define UU 0x02  /* with u */
#define VV 0x03  /* with v */
#define OO 0x04  /* any i,u,v */
#define RR 0x08  /* replace */

struct {
  unsigned char flag;
  char c1;
  char c2;
} c2p[] = {
/*  0 */   TT|MM|YY|NN|OO,  'b', ' ',
/*  1 */   TT|MM|YY|NN|OO,  'p', ' ',
/*  2 */   TT|MM|YY|NN|OO,  'm', ' ',
/*  3 */   TT|MM|YY|NN|OO,  'f', ' ',
/*  4 */   TT|MM|YY|NN|OO,  'd', ' ',
/*  5 */   TT|MM|YY|NN|OO,  't', ' ',
/*  6 */   TT|MM|YY|NN|OO,  'n', ' ',
/*  7 */   TT|MM|YY|NN|OO,  'l', ' ',
/*  8 */   TT|MM|YY|NN|OO,  'g', ' ',
/*  9 */   TT|MM|YY|NN|OO,  'k', ' ',
/* 10 */   TT|MM|YY|NN|OO,  'h', ' ',
/* 11 */   TT|MM|YY|NN|OO,  'j', ' ',
/* 12 */   TT|YY|NN|OO,     'c', 'h',
/* 13 */   MM|YY|NN|OO,     'q', ' ',
/* 14 */   TT|YY|NN|OO,     's', 'h',
/* 15 */   MM|YY|NN|OO,     'x', ' ',
/* 16 */   TT|YY|NN|OO,     'j', ' ',
/* 17 */   MM|YY|NN|OO,     'z', 'h',
/* 18 */   TT|MM|YY|NN|OO,  'c', 'h',
/* 19 */   TT|MM|YY|NN|OO,  's', 'h',
/* 20 */   TT|MM|YY|NN|OO,  'r', ' ',
/* 21 */   TT|YY|NN|OO,     't', 'z',
/* 22 */   MM|YY|NN|OO,     'z', ' ',
/* 23 */   TT|YY|NN|OO,     't', 's',
/* 24 */   MM|YY|NN|OO,     'c', ' ',
/* 25 */   TT|MM|YY|NN|OO,  's', ' ',
/* 26 */   TT|MM|NN|OO,     'y', ' ',
/* 27 */   TT|MM|YY|OO,     'i', ' ',
/* 28 */   TT|MM|NN|OO,     'w', ' ',
/* 29 */   TT|MM|YY|OO,     'u', ' ',
/* 30 */   TT|NN|OO,	    'y', 'u',
/* 31 */   TT|YY|OO,	    'i', 'u',
/* 32 */   MM|NN|OO,	    'y', 'v',
/* 33 */   MM|YY|OO,	    'v', ' ',
/* 34 */   TT|MM|YY|NN|OO,  'a', ' ',
/* 35 */   TT|MM|YY|NN|OO,  'o', ' ',
/* 36 */   TT|MM|YY|NN|OO,  'e', ' ',
/* 37 */   TT|MM|NN|XX,     'e', '^',
/* 38 */   TT|MM|YY|NN|OO,  'e', ' ',
/* 39 */   TT|MM|YY|NN|OO,  'a', 'i',
/* 40 */   MM|YY|UU,	    'i', ' ',
/* 41 */   TT|MM|YY|NN|OO,  'e', 'i',
/* 42 */   TT|YY|NN|OO,     'a', 'u',
/* 43 */   MM|YY|NN|OO,     'a', 'o',
/* 44 */   MM|YY|II,	    'u', ' ',
/* 45 */   TT|MM|YY|NN|OO,  'o', 'u',
/* 46 */   MM|YY|NN|RR|VV,  'u', 'a',
/* 47 */   TT|MM|YY|NN|OO,  'a', ' ',
/* 48 */   TT|MM|NN|II,     'i', ' ',
/* 49 */   TT|MM|YY|II,     ' ', ' ',
/* 50 */   TT|YY|NN|VV,     ' ', ' ',
/* 51 */   MM|YY|NN|RR|VV,  'u', ' ',
/* 52 */   MM|YY|UU,	    ' ', ' ',
/* 53 */   TT|MM|YY|NN|OO,  'e', ' ',
/* 54 */   TT|MM|YY|NN|OO,  'a', ' ',
/* 55 */   TT|MM|NN|II,     'i', ' ',
/* 56 */   TT|MM|YY|II,     ' ', ' ',
/* 57 */   TT|YY|NN|VV,     ' ', ' ',
/* 58 */   MM|NN|RR|VV,     'o', ' ',
/* 59 */   MM|YY|RR|VV,     'i', 'o',
/* 60 */   TT|YY|UU,	    ' ', ' ',
/* 61 */   MM|YY|RR|UU,     'o', ' ',
/* 62 */   TT|MM|YY|NN|OO,  'e', ' ',
/* 63 */   TT|MM|YY|NN|OO,  'e', 'r',
};

char	idx[] = {
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,
 10, 11, 12, 14, 16, 18, 19, 20, 21, 23,
 25, 26, 28, 30, 34, 35, 36, 37, 39, 40,
 42, 44, 46, 48, 54, 55, 63, 64
};

// DisplayMode: display mode(0: zhuin, 1:english)
// PininMode: pinin mode(0: zhuin, 1: 2nd format, 2: Roman)
//
// cmpCode: compressed codes(2 bytes)
// pinin: zhuin codes or pinin codes

void	append (short code, short flag, unsigned char *pinin, short *pc);

short	cmp2pinin(unsigned char *cmpCode, unsigned char *pinin)
{
    short 			pc, tmp, tone, flag;
    short 			ttmm, yynn, iiuuvv;
    unsigned char 	hi, lo;

    hi = *cmpCode - 0x21;
    lo = *(cmpCode+1) - 0x21;
    pc = 0;

    iiuuvv = hi % 4;
    yynn = ((hi=hi>>2) > 20) ? NN : YY;
    tone = (lo % TC) % 5;
    tmp = lo / TC;

	ttmm = (PininMode == 1) ? TT : MM;
	flag = ttmm | yynn | iiuuvv;
	if (yynn == YY) 
		append((short) hi, flag, pinin, &pc);
	if (iiuuvv) 
		append((short) (iiuuvv+20), flag, pinin, &pc);
	if (tmp) 
		append((short) (tmp+23), flag, pinin, &pc);
	if (tmp >= 9 && tmp <= 12) 
	{
		pinin[pc++] = 'n';
	    if (tmp >= 11) 
	    	pinin[pc++] = 'g';
	}
	else if ((ttmm == MM && yynn == YY && !iiuuvv && !tmp) ||
		(pinin[0] == 'y' && pc == 1))
	    pinin[pc++] = 'i';
	else if (pinin[0] == 'w' && pc == 1)
	    pinin[pc++] = 'u';
	pinin[pc++] = '0' + (unsigned char)tone;
    pinin[pc] = 0;
    
    return(pc);
}

void	append (short code, short flag, unsigned char *pinin, short *pc)
{
    short 	i;

    for (i=idx[code]; i<idx[code+1]; i++) 
    {
		if ((flag & c2p[i].flag & (TT|MM)) &&
		    (flag & c2p[i].flag & (YY|NN)) &&
		    ((c2p[i].flag & 0x07) == OO || (c2p[i].flag & 0x07) == (flag & 0x07))) 
		{
	    	if (c2p[i].flag & RR) 
	    		(*pc)--;
	    	if (c2p[i].c1 != ' ') 
	    		pinin[(*pc)++] = c2p[i].c1;
	    	if (c2p[i].c2 != ' ') 
	    		pinin[(*pc)++] = c2p[i].c2;
	    	break;
		}
    }
}
