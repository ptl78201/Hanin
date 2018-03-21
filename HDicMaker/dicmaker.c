/****************************************************************************
 * File Name  : dicmaker.c
 * Description: main function of Hanin dicitonary maker tool
 * Revision History:
 *  1999.02.08  PTL Wu Jia-Kwan     received from Hanin project
 *  2007.12.28  PTL Wu Jia-Kwan     revised for HDicMaker tool
 ****************************************************************************/
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "hdic_def.h"

/**************************************
  External Functions
**************************************/
extern int ReadCharFile ( char * ) ;
extern int ReadWordFile ( char * ) ;
extern int ReadBig5WordFile ( char * ) ;
extern int OpenBinDictionary ( char * ) ;
extern int CloseBinDictionary ( void ) ;
extern int SetBinDicOffset ( int ) ;
extern int WriteBinDictionary ( int , char *, int , int ) ;
extern int GetCharOffset ( char *, int ) ;
extern void WriteFirstChar ( PST_SINDEX1 ) ;
extern int CompressBopomofo ( UCHAR *, int ) ;
extern int CompareCmpcode ( UCHAR *, UCHAR * ) ;

/********************************************************************
 * Function Name: MakeHaninDictionary
 * Description: build Hanin binrary dictionary
 * Parameters:
 *     [input] char *wdfname:   Word dictionary filename
 *             char *chfname:   Character dictionary filename
 *             char *hdfname:   filename of result Hanin dictionary
 *             char *diclabel:  label of Hanin dictionary
 *    [output]
 *    [return]
 * LIMITATION:
 *  1. Input file should be sorted in ASCII order
 *  2. Compressed codes should be placed before word/character
 * syntax:
 *      mkdic_v4 char.cmp hv4.cmp hv4.dic ¨t²Î
 * Revision History:
 *  1999.02.08  PTL Wu Jia-Kwan     received from Hanin project
 *  2007.12.28  PTL Wu Jia-Kwan     revised for HDicMaker tool
********************************************************************/
int
MakeHaninDictionary ( char *wdfname, char *chfname, char *hdfname,
                      char *diclabel )
{
	int hdicFileoff ;		/* file size of dictionary */
	int hdicWordgroup ;		/* number of word group (the same first 2 cmpcode) */
	int hdicWGmaxnum ;		/* max number in word group */
	int hdicMaxsamepronChar ;
	char hdicWGm0cmp ;		/* the first 2 cmpcode of max number word group */
	char hdicWGm1cmp ;

	int i ;
    int iRet ;
	int iChar, iWord ;
	int idx1count ;
	int idx1index ;
	int iclen ;
	int iCharCode ;
	int j0, j1, cmp_order ;
	int iResult, iOutbuf ;
	int cmpHead ;
	UWORD temp1, temp2 ;
	UWORD off_bak, off1 ;
    ST_SINDEX1 sindex1[MAX_FIRST_ARR_S] ;
    ST_SINDEX2 sindex2[MAX_SECOND_ARR_S] ;
	ST_NEW2 new2_word ;
	char sindex3[SIZE_INDEX3_BUFF] ;
	char *cpindex3 ;
	UWORD *wpindex3 ;
	char buftot[SIZE_WORD_AREA] ;
	UCHAR outbuf[SIZE_WORD_AREA] ;
	char cmpword[SIZE_CMPCODE_WORD] ;
	char *pBuff ;
	char fg_nochar, fg_eofchar, fg_eofword, fg_first ;
	char c0char, c1char, c0word, c1word ;	/* cmpcode of char, word */
	char x0cmp, x1cmp ;
	char cmpbuf[2] ;
	char d0cmp, d1cmp, e0cmp, e1cmp ;
	char goto_check_next_word ;

    /* initialization */
    for ( i=0; i < MAX_FIRST_ARR_S; i++ ) {
        sindex1[i].schar = (char)0x7F ;
    	sindex1[i].fst_char_off = 0 ;
    	sindex1[i].snd_idx_off = 0 ;
    	sindex1[i].hb_snd_idx = 0 ;
    	sindex1[i].hb_fst_char = 0 ;
    }
    for ( i=0; i < MAX_SECOND_ARR_S; i++ ) {
        sindex2[i].schar = (char)0x7F ;
    	sindex2[i].seq = 0 ;
    	sindex2[i].foff = 0 ;
    	sindex2[i].clen = 0 ;
    }

	if ( iRet = ReadCharFile ( chfname ) < 0 ) {
		return X1_MKDIC_CHAR_FAILED ;
	}
//	if ( iRet = ReadWordFile ( wdfname ) < 0 ) {
	if ( iRet = ReadBig5WordFile ( wdfname ) < 0 ) {
		return X1_MKDIC_WORD_FAILED ;
	}
	
	if ( OpenBinDictionary ( hdfname ) == HAN_FAILED ) {
		return X1_MKDIC_OPENDIC_FAILED ;
	}
	/* initialize index table */
	memset ( buftot, '\0', SIZE_INDEX_TABLE ) ;
	for ( i=0; i < HDIC_OFF_DIC_B; ) {
		if ( ( HDIC_OFF_DIC_B - i ) > SIZE_INDEX_TABLE ) {
			WriteBinDictionary ( i, buftot, sizeof ( char ), SIZE_INDEX_TABLE ) ;
			i += SIZE_INDEX_TABLE ;
		}
		else {
			WriteBinDictionary ( i, buftot, sizeof ( char ),
								 HDIC_OFF_DIC_B - i ) ;
			break ;
		}
	}
	/* write version name & label name */
	if ( eDicVer == HANIN_V4 ) {
		WriteBinDictionary ( HDIC_OFF_ID_AREA_B, HDIC_VERSION4, 
							 sizeof ( char ), SIZE_LABEL_VERSION ) ;
	}
	else if ( eDicVer == HANIN_V5 ) {
		WriteBinDictionary ( HDIC_OFF_ID_AREA_B, HDIC_VERSION5, 
							 sizeof ( char ), SIZE_LABEL_VERSION ) ;
	}
	i = ( sizeof ( diclabel ) < SIZE_LABEL_VERSION ) ? 
			sizeof ( diclabel ) : SIZE_LABEL_VERSION ;
	WriteBinDictionary ( HDIC_OFF_LABEL, diclabel, sizeof ( char ), i ) ;
	/* deal with index of dictionary */
	/*--- initialize local variables ---*/
	hdicFileoff = HDIC_OFF_DIC_B ;
	hdicWordgroup = 0 ;
	hdicWGmaxnum = 0 ;
	hdicMaxsamepronChar = 0 ;
	idx1count = 0 ;
	idx1index = 0 ;
	fg_nochar = 0 ;
	fg_eofchar = 0 ;
	fg_eofword = 0 ;
	new2_word.len = SIZE_NEW2WORD_LEN ;
	c0word = stWordSource[0].cmp[0] ;
	c1word = stWordSource[0].cmp[1] ;
	x0cmp = c0word ;
	x1cmp = c1word ;
	cmpbuf[0] = 0 ;
	cmpbuf[1] = 0 ;
	iWord = 0 ;
	iChar = 0 ;
	d0cmp = 0 ;
	d1cmp = 0 ;
	/*--- ????? ---*/
	while ( !fg_eofchar || !fg_eofword ) {
		if ( iWord >= iWordSrcCount ) {
			fg_eofword = 1 ;
		}
		if ( fg_eofchar ) {
			fg_nochar = 1 ;
			c0char = x0cmp ;
			c1char = x1cmp ;
		}
		else {
			fg_nochar = 0 ;
			c0char = stCharSource[iChar].cmp[0] ;
			c1char = stCharSource[iChar].cmp[1] ;
		}
		if ( !fg_eofword ) {
			if ( ( c0char > x0cmp ) || 
				 ( c0char == x0cmp && c1char > x1cmp ) ) {
				if ( x0cmp != 0 && x0cmp != -1 ) {
					fg_nochar = 1 ;
					c0char = x0cmp ;
					c1char = x1cmp ;
				}
			}
		}
		/*--- index1 ---*/
		if ( c0char != cmpbuf[0] ) {
			sindex1[idx1count].schar = c0char ;
			sindex1[idx1count].snd_idx_off = (UCHAR)(idx1index & 0xFF) ;
			sindex1[idx1count].hb_snd_idx = (UCHAR)((idx1index & 0xFF00) >> 8 ) ;
			idx1count ++ ;
		}
		if ( idx1index >= MAX_SECOND_ARR_S ) {
			iRet = X1_MKDIC_IDX1INDEX_OVERFLOW ;
			break ;
		}
		/*--- index2 ---*/
		sindex2[idx1index].schar = c1char ;
		sindex2[idx1index].seq = (char)( ( hdicFileoff >> 16 ) + '0' ) ;
		temp1 = ( hdicFileoff & 0x00FF ) << 8 ;
		temp2 = ( hdicFileoff & 0xFF00 ) >> 8 ;
		sindex2[idx1index].foff = (UWORD)( temp1 + temp2 ) ;
		if ( hdicFileoff == 0 ) {
			iRet = X1_MKDIC_FILEINDEX_ZERO ;
			break ;
		}
		/*--- insert char ---*/
		pBuff = buftot ;
		cmpbuf[0] = c0char ;
		cmpbuf[1] = c1char ;
		iclen = 0 ;
		if ( fg_nochar == 0 ) {
			iCharCode = 0 ;
			while((c0char=stCharSource[iChar].code[iCharCode++]) != '\0') {
				if ( c0char & 0x80 ) {
					*pBuff++ = c0char ^ HAN_ENCRYPTION ;
					c1char = stCharSource[iChar].code[iCharCode++] ;
					*pBuff++ = c1char ^ HAN_ENCRYPTION ;
					iclen += 2 ;
				}
			}
			if ( iclen > hdicMaxsamepronChar ) {
				hdicMaxsamepronChar = iclen ;
			}
		}
		sindex2[idx1index].clen = (UWORD)( ( ( iclen << 8 ) & 0xFF00 ) |
										   ( ( iclen >> 8 ) & 0x00FF ) ) ;
		/*--- summarize total chinese ---*/
		idx1index ++ ;
		if ( cmpbuf[0] > x0cmp || 
			 ( cmpbuf[1] == x0cmp && cmpbuf[1] > x1cmp ) ) {
			c0char = stWordSource[iWord].cmp[0] ;
			c1char = stWordSource[iWord].cmp[1] ;
			x0cmp = c0char ;
			x1cmp = c1char ;
		}
		if ( iclen > 0 ) {
			WriteBinDictionary ( hdicFileoff, buftot, 1, iclen ) ;
			hdicFileoff += iclen ;
			if ( ++iChar >= iCharSrcCount ) {
				fg_eofchar = 1 ;
			}
		}
		if ( fg_eofword || cmpbuf[0] != x0cmp || cmpbuf[1] != x1cmp ) {
			continue ;
		}
		
		iclen |= 0x8000 ;
		sindex2[idx1index-1].clen = (UWORD)( ( ( iclen << 8 ) & 0xFF00 ) |
										     ( ( iclen >> 8 ) & 0x00FF ) ) ;

		wpindex3 = (UWORD *)sindex3 ;
		cpindex3 = sindex3 + 2 ;
		sindex3[0] = 0 ;
		sindex3[1] = (char)0x80 ;	/* end boundary of wordoff */
		new2_word.cmp[0] = 0 ;
		e1cmp = (char)(-1) ;
		cmp_order = 0 ;
		off_bak = 0 ;
		off1 = 0 ;
		iOutbuf = 0 ;
		while ( fg_eofword == 0 ) {
			e0cmp = stWordSource[iWord].cmp[2] ;	/* 3rd byte */
			if ( ( ( e0cmp - CMPCODE_FIRST ) / TONE_COUNT ) != 
				 ( ( e1cmp - CMPCODE_FIRST ) / TONE_COUNT ) ) {
				j0 = e0cmp - CMPCODE_FIRST ;
				e1cmp = e0cmp ;
				if ( j0 >= 0 ) {
					j0 = j0 / TONE_COUNT ;
					if ( j0 < cmp_order ) {
						iRet = X1_MKDIC_CMPORDER ;
						break ;
					}
					cmp_order = j0 ;
					j1 = j0 & 0x7 ;		/* j1 = j0 % 8 */
					j1 = 1 << j1 ;
					if ( sindex3[j0>>3] & (char)j1 ) {
						fg_first = 0 ;
					}
					else {
						sindex3[j0>>3] |= (char)j1 ;
						fg_first = 1 ;
					}
				}
				if ( fg_first ) {
					if ( hdicWGmaxnum < off1 - off_bak ) {
						hdicWGmaxnum = off1 - off_bak ;
						hdicWGm0cmp = d0cmp ;
						hdicWGm1cmp = d1cmp ;
					}
					off_bak = off1 ;
					if ( off1 > 0 ) {
						wpindex3 = (UWORD *)cpindex3 ;
						temp1 = ( ( off1 & 0x00FF ) << 8 ) | 
								( ( off1 & 0xFF00 ) >> 8 ) ;
						temp2 = ( temp1 >> 8 ) & 0xFF ;
						*wpindex3 = ( ( temp1 << 8 ) & 0xFF00 ) | temp2 ;
						wpindex3 += 1 ;
						cpindex3 += 2 ;
						hdicWordgroup ++ ;
					}
				}
			}
/* jout: */
			/* process extended word */
			goto_check_next_word = 0 ;
			iResult = strncmp ( new2_word.cmp, stWordSource[iWord].cmp, 4 ) ;
			if ( iResult == 0 ) {
				iResult = strncmp ( (char *)new2_word.code, 
					(char *)stWordSource[iWord].code, 4 ) ;
				if ( iResult == 0 ) {
					for ( i=4; i < stWordSource[iWord].len; i++ ) {
						cmpword[i-4] = stWordSource[iWord].cmp[i] ;
						outbuf[iOutbuf++] = (UCHAR)cmpword[i-4] ;
						off1 ++ ;
					}
					for ( i=4; i < stWordSource[iWord].len; i+=2 ) {
						iCharCode = 
							( ( stWordSource[iWord].code[i] & 0x00FF ) << 8 ) | 
							( stWordSource[iWord].code[i+1] & 0xFF ) ;
						iResult = GetCharOffset ( cmpword+(i-4), iCharCode ) ;
						if ( iResult < 0 ) {
							iRet = iResult ;
							break ;
						}
						outbuf[iOutbuf++] = (UCHAR)( ( iResult + 1 ) | 0x80 ) ;
						off1 ++ ;
					}
					if ( iWord+1 >= iWordSrcCount ) {
						fg_eofword = 1 ;
						continue ;
					}
					goto_check_next_word = 1 ;
				}
			}
			if ( iOutbuf+SIZE_CMPCODE_WORD >= SIZE_WORD_AREA ) {
				iRet = X1_MKDIC_WORDAREA_OVERFLOW ;
				break ;
			}
			/*--- normal new word ---*/
			if ( goto_check_next_word == 0 ) {
				cmpHead = iOutbuf ;
				outbuf[iOutbuf++] = ((e0cmp-CMPCODE_FIRST) % TONE_COUNT) - 1 ;
				off1 ++ ;
				for ( i=3; i < stWordSource[iWord].len; i++ ) {
					outbuf[iOutbuf++] = stWordSource[iWord].cmp[i] ;
					off1 ++ ;
				}
				for ( i=0; i < stWordSource[iWord].len; i+=2 ) {
					iCharCode = 
						( ( stWordSource[iWord].code[i] & 0x00FF ) << 8 ) | 
						( stWordSource[iWord].code[i+1] & 0xFF ) ;
					if ( i == 0 ) {
						cmpword[0] = x0cmp ;
						cmpword[1] = e0cmp ;
					}
					else if ( i == 2 ) {
						cmpword[0] = x1cmp ;
						cmpword[1] = outbuf[cmpHead+1] ;
					}
					else {
						cmpword[0] = outbuf[cmpHead+i-2] ;
						cmpword[1] = outbuf[cmpHead+i-1] ;
					}
					iResult = GetCharOffset ( cmpword, iCharCode ) ;
					if ( iResult < 0 ) {
						iRet = iResult ;
						break ;
					}
					outbuf[iOutbuf++] = (UCHAR)( ( iResult + 1 ) | 0x80 ) ;
					off1 ++ ;
				}
				outbuf[cmpHead] |= (stWordSource[iWord].freq[0]-WORDFRQ_FIRST) << 5 ;
				outbuf[cmpHead] |= (stWordSource[iWord].freq[1]-WORDFRQ_FIRST) << 2 ;
				outbuf[cmpHead+1] |= 0x80 ;		/* new word */
				outbuf[iOutbuf++] = 0x20 ;
				off1 ++ ;
				/*--- backup new word ---*/
//				lstrcpy ( new2_word.cmp, stWordSource[iWord].cmp ) ;
				strcpy ( new2_word.cmp, stWordSource[iWord].cmp ) ;
//				lstrcpy ( new2_word.code, stWordSource[iWord].code ) ;
				strcpy ( new2_word.code, stWordSource[iWord].code ) ;
				new2_word.len = stWordSource[iWord].len ;
			}
/* check_next_word: */
			iWord ++ ;
			if ( iWord >= iWordSrcCount ) {
				fg_eofword = 1 ;
			}
			d0cmp = stWordSource[iWord].cmp[0] ;
			d1cmp = stWordSource[iWord].cmp[1] ;
			if ( d0cmp != cmpbuf[0] || d1cmp != cmpbuf[1] ) {
				wpindex3 = (UWORD *)cpindex3 ;
				temp1 = ( ( off1 & 0x00FF ) << 8 ) | ( ( off1 & 0xFF00 ) >> 8 ) ;
				*wpindex3 = ( ( temp1 << 8 ) & 0xFF00 ) |
							( ( temp1 >> 8 ) & 0x00FF ) ;
				wpindex3 ++ ;
				hdicWordgroup ++ ;
				break ;
			}
		}
		if ( iRet < 0 ) {
			break ;
		}
		x0cmp = d0cmp ;
		x1cmp = d1cmp ;
		if ( off1 > 0 ) {
			i = (int)( (char *)wpindex3 - sindex3 ) ;
			WriteBinDictionary ( hdicFileoff, sindex3, 1, i ) ;
			hdicFileoff += i ;
			WriteBinDictionary ( hdicFileoff, outbuf, 1, off1 ) ;
			hdicFileoff += off1 ;
		}
		if ( fg_eofword ) {
			off1 = 0 ;
		}
		cpindex3 = sindex3 ;
	}
	/* dump binary dictionary information */
#ifdef DUMP_DIC_INFO
	{
		char szBuf[128] ;
		sprintf ( szBuf, "File size of dictionary is %ld bytes\n",
				  hdicFileoff ) ;
		sprintf ( szBuf, "The number of word group is %d \n",
				  hdicWordgroup ) ;
		sprintf ( szBuf, "The max number of words among word group is %d bytes\n",
				  hdicWGmaxnum ) ;
		sprintf ( szBuf, "The cmpcode of max number word group is %c%c \n",
				  hdicWGm0cmp, hdicWGm1cmp ) ;
		sprintf ( szBuf, "The max number of samepron characters is %d\n",
				  hdicMaxsamepronChar ) ;
	}
#endif /* DUMP_DIC_INFO */

	sindex1[idx1count].schar = (char)LAST_ASCII_CODE ;
	temp1 = idx1index & 0xFF ;
	temp2 = ( idx1index & 0xFF00 ) << 8 ;
	sindex1[idx1count].snd_idx_off = (UCHAR)temp1 ;
	sindex1[idx1count].hb_snd_idx = (UCHAR)temp2 ;
	idx1count ++ ;
	
	hdicFileoff ++ ;
	sindex2[idx1index].schar = (char)LAST_ASCII_CODE ;
	sindex2[idx1index].seq = (char)( (hdicFileoff >> 16) + WORDFRQ_FIRST ) ;
	temp1 = (hdicFileoff & 0x00FF) << 8 ;
	temp2 = (hdicFileoff & 0xFF00) >> 8 ;
	sindex2[idx1index].foff = (UWORD)( temp1 + temp2 ) ;
#ifdef DUMP_DIC_INFO
	{
		sprintf ( szBuf, "1st layer of index: count = %d, size = %d bytes\n",
			idx1count+1, (idx1count+1)*sizeof(ST_SINDEX1) ) ;
		sprintf ( szBuf, "2nd layer of index: count = %d, size = %d bytes\n",
			idx1index+1, (idx1index+1)*sizeof(ST_SINDEX2) ) ;
	}
#endif /* DUMP_DIC_INFO */
	WriteFirstChar ( sindex1 ) ;
	for (i=idx1count; i < MAX_FIRST_ARR_S; i++ ) {
		sindex1[i] = sindex1[i-1] ;
	}
	WriteBinDictionary ( HDIC_OFF_FST_IDX_B, (char *)sindex1, sizeof (ST_SINDEX1),
						 MAX_FIRST_ARR_S ) ;

	for (i=0; i < MAX_SECOND_ARR_S; i++ ) {
		temp1 = ((sindex2[i].foff >> 8) & 0x00FF) ;
		sindex2[i].foff = (UWORD)(((sindex2[i].foff << 8) & 0xFF00) | temp1) ;
		temp1 = ((sindex2[i].clen >> 8) & 0x00FF) ;
		sindex2[i].clen = (UWORD)(((sindex2[i].clen << 8) & 0xFF00) | temp1) ;
	}
	WriteBinDictionary ( HDIC_OFF_SND_IDX_B, (char *)sindex2, sizeof (ST_SINDEX2),
						 MAX_SECOND_ARR_S ) ;

	CloseBinDictionary () ;
	return iRet ;
}

/********************************************************************
 * Function Name: MakeAimeiDictionary
 * Description: build binary aimei dictionay
 * Parameters:
 *     [input] char *tfname: aimei text dictionary filename
 *             char *bfname: aimei binary dictionary filename
 *    [output]
 *    [return]
 * Revision History:
 *  2008.01.18  PTL Wu Jia-Kwan     revised for HDicMaker tool
********************************************************************/
int
MakeAimeiDictionary ( char *tfname, char *bfname )
{
	FILE *txtAimei, *binAimei ;
	int i , j ;
	int iRet ;
	int iline ;
	UCHAR last_cmp[100] ;
	int	cmpline, cmp_count ;
	UWORD addr ;
	char buffer[SIZE_LINE_CHAR+1] ;
	UWORD cmpidx[LAST_1ST_CMPCODE-CMPCODE_FIRST+1][LAST_2ND_CMPCODE-CMPCODE_FIRST+1] ;
	
	if ( ( txtAimei = fopen ( tfname, "r" ) ) == NULL ) {
		return X2_AIMEI_OPENTXT_FAILED ;
	}
	if ( ( binAimei = fopen ( bfname, "w+b" ) ) == NULL ) {
		fclose ( txtAimei ) ;
		return X2_AIMEI_OPENBIN_FAILED ;
	}

	iRet = 0 ;
	iline = 1 ;
	cmpline = 0 ;
	for( ; ; iline++ ) {
		cmp_count = 0 ;
		if ( fgets ( buffer, SIZE_LINE_CHAR, txtAimei ) == NULL ) {
			break ;
		}
		if ( buffer[0] == '\n' || buffer[0] == ' ' || buffer[0] == '|' ) {
			continue ;
		}
		i = strlen ( buffer ) ;
		if ( buffer[i-1] == '\n' ) {
			buffer[i-1] = 0 ;
		}
		cmp_count = CompressBopomofo ( buffer, 0 ) ;
		if ( iline > 1 ) {
			if ( strncmp ( last_cmp, buffer, 2 ) >= 0 ) { 
				iRet = X2_AIMEI_ORDER_ERROR ;
				break ;
			}
		}
		if ( cmp_count > MAX_AIMEI_LINE_CNT ) {
			iRet = X2_AIMEI_DATA_TOO_LONG ;
			break;
		}
		buffer[cmp_count] = '\0' ;
	
		for (i=0; i < cmp_count; i+=2 ) {
			for ( j=i+2; j < cmp_count; j+=2) {
				if ( buffer[i] == buffer[j] &&
					 buffer[i+1] == buffer[j+1] ) {
					iRet = X2_AIMEI_PRON_REPEAT ;
				}
			}
		}
		strcpy ( last_cmp, buffer ) ;
		strcpy ( wordbuf[cmpline++], buffer ) ;
		buffer[cmp_count] = '\0' ;
	}
	if ( iRet >= 0 ) {
		qsort ( wordbuf, cmpline, MAX_NUM_WORD_LINE, CompareCmpcode ) ;
		addr = 0 ;
		for ( i=CMPCODE_FIRST; i <= LAST_1ST_CMPCODE; i++ ) {
			for ( j=CMPCODE_FIRST; j <= LAST_2ND_CMPCODE; j++ ) {
				fwrite ( (char *)&addr, sizeof(UWORD), 1, binAimei ) ;
			}
		}
		addr = (LAST_1ST_CMPCODE-CMPCODE_FIRST+1) * 
			   (LAST_2ND_CMPCODE-CMPCODE_FIRST+1) * 2 ;
		iline = 0 ;
		for ( i=CMPCODE_FIRST; i <= LAST_1ST_CMPCODE; i++ ) {
			for ( j=CMPCODE_FIRST; j <= LAST_2ND_CMPCODE; j++ ) {
				cmpidx[i-CMPCODE_FIRST][j-CMPCODE_FIRST] = addr ;
				if ( wordbuf[iline][0] == i && wordbuf[iline][1] == j ) {
					cmp_count = strlen ( wordbuf[iline] ) ;
					fwrite ( (char *)wordbuf[iline], 1, cmp_count, binAimei ) ;
					addr += cmp_count ;
					iline ++ ;
				}
			}
		}
		fseek ( binAimei, 0L, SEEK_SET ) ;
		fwrite ( (char *)cmpidx, 1, sizeof ( cmpidx ), binAimei ) ;
	}
	fclose ( txtAimei ) ;
	fclose ( binAimei ) ;
	return iRet ;
}
/****************************************************************************
 * Copyright (C) 1999~2008 Panasonic Taiwan Laboratories Co., Ltd.
 ****************************************************************************/

