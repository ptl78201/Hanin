/****************************************************************************
 * File Name   : hdic_rw.c
 * Description : read text character and word file, write Hanin binary
 *               dictionary
 * Revision History     :
 *  2007.12.28  PTL Wu Jia-Kwan     for HDicMaker tool
 ****************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hdic_def.h"

/**************************************
  Definition
**************************************/

/**************************************
  Global Variables
**************************************/
UCHAR hdicbuf[MAX_WORD_LINE_CNT][MAX_NUM_WORD_LINE] ;
UCHAR wordbuf[MAX_WORD_LINE_CNT][MAX_NUM_WORD_LINE] ;
FILE *fBinDic ;
enum eDICVERSION eDicVer ;

/**************************************
  Local Functions
**************************************/
int CompressBopomofo ( UCHAR *, int ) ;
int CompareCmpcode ( UCHAR *, UCHAR * ) ;

/********************************************************************
 * Function Name: ReadCharFile
 * Description: load all the entries of character file
 * Parameters:
 *     [input] char *fname
 *    [return] int <0: fail to open file or retrieve date
 *                  1: success
 * Revision Histor:
 *  2007.12.28  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
ReadCharFile ( char *fname )
{
    FILE *fpChar ;
	int i, retCode ;
    int datalen ;
    char data[SIZE_LINE_CHAR+1] ;

    if ( (fpChar = fopen ( fname, "r" )) == NULL ) {
        return 0 ;
    }

	iCharSrcCount = 0 ;
	retCode = 1 ;
    while ( !feof ( fpChar ) ) {
        if ( fgets ( data, SIZE_LINE_CHAR, fpChar ) == NULL ) {
            break ;
        }
        datalen = strlen ( data ) ;
        if ( data[datalen-1] == '\n' ) {
            datalen -= 1 ;
        }
        if ( datalen <= 2 ) {
            continue ;
        }
        if ( datalen >= SIZE_LINE_CHAR ) {
            retCode = X2_READCHAR_SIZE_OVERFLOW ;
        	break ;
        }
        data[datalen] = 0 ;
    	/* check cmpcode */
    	if ( ( data[0] & 0x80 ) || ( data[1] & 0x80) ) {
    		retCode = X2_READCHAR_NO_CMPCODE ;
    		break ;
    	}
    	/* check characters */
    	for ( i=2; i < datalen; i+=2 ) {
    		if ( ( data[i] & 0x80 ) == 0 ) {
    			retCode = X2_READCHAR_NOT_2BYTECHAR ;
    			break ;
    		}
    	}
    	if ( retCode < 0 ) {
    		break ;
    	}
    	/* duplicate cmpcode and characters  */
    	strncpy ( stCharSource[iCharSrcCount].cmp, data, SIZE_CMPCODE_CHAR ) ;
    	strcpy ( stCharSource[iCharSrcCount].code, data+SIZE_CMPCODE_CHAR ) ;
    	stCharSource[iCharSrcCount].len = datalen - SIZE_CMPCODE_CHAR ;
    	if ( stCharSource[iCharSrcCount].len > 0 ) {
    		iCharSrcCount ++ ;
    	}
    	if ( iCharSrcCount >= MAX_CHAR_LINE_CNT ) {
    		retCode = X2_READCHAR_MAX_OVERFLOW ;
    		break ;
    	}
    }

    fclose ( fpChar ) ;
    return retCode ;
}

/********************************************************************
 * Function Name: ReadWordFile
 * Description: load all the entries of word file
 * Parameters:
 *     [input] char *fname
 *    [return] int <0: fail to open file or retrieve date
 *                  1: success
 * Revision Histor:
 *  2008.01.09  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
ReadWordFile ( char *fname )
{
    FILE *fpWord ;
	int i, retCode ;
	int iCmpIdx, iFrqIdx ;
    int datalen ;
    char data[SIZE_LINE_WORD+1] ;

    if ( (fpWord = fopen ( fname, "r" )) == NULL ) {
        return 0 ;
    }

	iWordSrcCount = 0 ;
	retCode = 1 ;
	while ( ! feof ( fpWord ) ) {
       	if ( fgets ( data, SIZE_LINE_WORD, fpWord ) == NULL ) {
       		break ;
       	}
		datalen = strlen ( data ) ;
        if ( data[datalen-1] == '\n' ) {
            datalen -= 1 ;
        }
		if ( datalen <= 2 ) {
			continue ;
		}
		else if ( datalen > SIZE_LINE_WORD ) {
			retCode = X2_READWORD_SIZE_OVERFLOW ;
			break ;
		}
		data[datalen] = 0 ;
		/* duplicate word frequence and attribute */
		if ( datalen & 1 ) {
			iFrqIdx = datalen - 3 ;
			stWordSource[iWordSrcCount].attr[0] = data[datalen-1] ;
			stWordSource[iWordSrcCount].attr[1] = 0 ;
			eDicVer = HANIN_V5 ;
		}
		else {
			iFrqIdx = datalen - 2 ;
			stWordSource[iWordSrcCount].attr[0] = 0 ;
			eDicVer = HANIN_V4 ;
		}
		stWordSource[iWordSrcCount].freq[0] = data[iFrqIdx] ;
		stWordSource[iWordSrcCount].freq[1] = data[iFrqIdx+1] ;
		stWordSource[iWordSrcCount].freq[2] = 0 ;
		/* check zhuyin cmpcode of word */
		for ( iCmpIdx = 0; ( data[iCmpIdx] & 0x80 ) == 0 && data[iCmpIdx]; 
			  iCmpIdx++ ) ;
		if ( iCmpIdx >= SIZE_CMPCODE_WORD ) {
			retCode = X2_READWORD_CMPCODE_OVERFLOW ;
			break ;
		}
		/* check word */
		for ( i=iCmpIdx; (i < iFrqIdx) && (data[i] & 0x80); i+=2 ) ;
		if ( i != iFrqIdx ) {
			retCode = X2_READWORD_NOT_2BYTECHAR ;
			break ;
		}
		/* duplicate word & zhuyin cmpcode */
		strncpy ( stWordSource[iWordSrcCount].cmp, data, iCmpIdx ) ;
		strncpy ( stWordSource[iWordSrcCount].code, data+iCmpIdx, iCmpIdx ) ;
		
		stWordSource[iWordSrcCount].len = iCmpIdx ;
		if ( iCmpIdx > 0 ) {
			iWordSrcCount ++ ;
		}
	}
	fclose ( fpWord ) ;
    return retCode ;
}

/********************************************************************
 * Function Name: ReadBig5WordFile
 * Description: load all the entries of word file, compress zhuyin
 *              fuhao, and sort by compressed code
 * Parameters:
 *     [input] char *fname
 *    [return] int 0: fail to open file or retrieve date
 *                 1: success
 * Revision Histor:
 *  2008.01.17  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
ReadBig5WordFile ( char *fname )
{
    FILE *fpWord ;
	int curLine, topLine, endLine ;
	int i, j, k, retCode ;
	int datalen, freqoff ;
	int iFrqIdx, iWordLine ;
	int sortoff1, sortoff2 ;
	int proItem, freq0, freq1, iCmpIdx ;
	int len0, len1 ;
	int coff0, coff1 ;
	UCHAR sortbuf1[200][MAX_NUM_WORD_LINE] ;
	UCHAR sortbuf2[200][MAX_NUM_WORD_LINE] ;
	int sortLen[200] ;
    char data[SIZE_LINE_WORD+1] ;
	UCHAR hcmp0, hcmp1, hcmp2, hcmp3 ;
	UCHAR ncmp0, ncmp1, ncmp2, ncmp3 ;
	enum eDICVERSION fgVersion ;

    if ( (fpWord = fopen ( fname, "r" )) == NULL ) {
        return 0 ;
    }
	retCode = 1 ;
	topLine = 0 ;
	curLine = 0 ;
	while ( 1 ) {
       	if ( fgets ( hdicbuf[curLine], MAX_NUM_WORD_LINE, fpWord ) == NULL ) {
       		break ;
       	}
		if ( hdicbuf[curLine][0] == '\n' || hdicbuf[curLine][0] == '\t' ||
			 hdicbuf[curLine][0] == '|'  || hdicbuf[curLine][0] == ' ' ) {
			continue ;
		}
		CompressBopomofo ( hdicbuf[curLine], 1 ) ;
		curLine ++ ;
		if ( curLine >= MAX_WORD_LINE_CNT ) {
			retCode = X2_READWORD_SIZE_OVERFLOW ;
			break ;
		}
	}
	fclose ( fpWord ) ;
	if ( retCode < 0 ) {
		return retCode ;
	}
	qsort ( hdicbuf, curLine, MAX_NUM_WORD_LINE, CompareCmpcode ) ;
	/* sort Hanin dictionary */
	iWordLine = 0 ;
	while ( topLine < curLine ) {
		endLine = topLine + 1 ;
		while ( endLine < curLine ) {
			hcmp0 = hdicbuf[topLine][0] ;
			hcmp1 = hdicbuf[topLine][1] ;
			hcmp2 = hdicbuf[topLine][2] ;
			ncmp0 = hdicbuf[endLine][0] ;
			ncmp1 = hdicbuf[endLine][1] ;
			ncmp2 = hdicbuf[endLine][2] ;
			if ( ( hcmp0 != ncmp0 ) || ( hcmp1 != ncmp1 ) ) {
				break ;
			}
			if ( ( ( hcmp2 - CMPCODE_FIRST ) / TONE_COUNT ) !=
				 ( ( ncmp2 - CMPCODE_FIRST) / TONE_COUNT ) ) {
				break ;
			}
			endLine ++ ;
		}
		endLine -- ;
NEXT_SORT:
		for ( i=topLine; i <= endLine; i++ ) {
			hcmp2 = hdicbuf[i][2] ;
			if ( hcmp2 != ' ' ) {
				break ;
			}
		}
		if ( i > endLine ) {
			topLine = endLine + 1 ;
			continue ;
		}
		hcmp2 = hdicbuf[i][2] ;
		hcmp3 = hdicbuf[i][3] ;
		for ( sortoff1=0; i <= endLine; i++ ) {
			ncmp2 = hdicbuf[i][2] ;
			ncmp3 = hdicbuf[i][3] ;
			if ( ( ( ( hcmp2 - CMPCODE_FIRST ) / TONE_COUNT ) ==
				   ( ( ncmp2 - CMPCODE_FIRST ) / TONE_COUNT ) ) &&
				 ( ( ( hcmp3 - CMPCODE_FIRST ) / TONE_COUNT ) ==
				   ( ( ncmp3 - CMPCODE_FIRST ) / TONE_COUNT ) ) ) {
		   		len0 = strlen ( hdicbuf[i] ) - 1 ;
				sortLen[sortoff1] = len0 ;
			   	strcpy ( (char *)sortbuf1[sortoff1], (char *)hdicbuf[i] ) ;
				sortbuf1[sortoff1][len0] = '\0' ;
			   	for ( k=0; k < MAX_NUM_WORD_LINE; k++ ) {
			   		hdicbuf[i][k] = ' ' ;
			   	}
			   	sortoff1 ++ ;
			}
		}
NEXT_SORT_1:
		for ( i=0; i < sortoff1; i++ ) {
			hcmp2 = sortbuf1[i][2] ;
			if ( hcmp2 == ' ' ) {
				continue ;
			}
			hcmp3 = sortbuf1[i][3] ;
			len0 = sortLen[i] ;
			freqoff = ( (sortbuf1[i][len0-1] >= '0') && (sortbuf1[i][len0-1] <= '9') ) ?
				2 : 3 ;
			freq0 = ( sortbuf1[i][len0-freqoff] - WORDFRQ_FIRST ) +
					( sortbuf1[i][len0-freqoff+1] - WORDFRQ_FIRST ) ;
			proItem = i ;
			for ( j=i+1; j < sortoff1; j++ ) {
				ncmp2 = sortbuf1[j][2] ;
				if ( ncmp2 == ' ' ) {
					continue ;
				}
				ncmp3 = sortbuf1[j][3] ;
				len1 = sortLen[j] ;
				if ( hcmp2 == ncmp2 && hcmp3 == ncmp3 && len0 == len1 ) {
					freqoff = ( (sortbuf1[j][len1-1] >= '0') && (sortbuf1[j][len1-1] <= '9') ) ?
							2 : 3 ;
					freq1 = ( sortbuf1[j][len1-freqoff] - WORDFRQ_FIRST ) +
							( sortbuf1[j][len1-freqoff+1] - WORDFRQ_FIRST ) ;
					if ( freq1 > freq0 ) {
						freq0 = freq1 ;
						proItem = j ;
					}
				}
			}
			
			sortoff2 = 0 ;
			strcpy ( (char *)sortbuf2[sortoff2], (char *)sortbuf1[proItem] ) ;
			for ( k=0; k < MAX_NUM_WORD_LINE; k++ ) {
				sortbuf1[proItem][k] = ' ' ;
			}
			sortoff2 ++ ;
			
			for ( j=proItem+1; j < sortoff1; j++ ) {
				len1 = sortLen[j] ;
				if ( len1 < len0 ) {
					continue ;
				}
				freqoff = ( (sortbuf2[0][len0-1] >= '0') && (sortbuf2[0][len0-1] <= '9') ) ?
						2 : 3 ;
				coff0 = (len0-freqoff) >> 1 ;
				freqoff = ( (sortbuf1[j][len1-1] >= '0') && (sortbuf1[j][len1-1] <= '9') ) ?
						2 : 3 ;
				coff1 = (len1-freqoff) >> 1 ;
				if ( strncmp ( sortbuf2[0], sortbuf1[j], 4 ) == 0 &&
				strncmp ( sortbuf2[0]+coff0, sortbuf1[j]+coff1, 4 ) == 0 ) {
					strcpy ( sortbuf2[sortoff2], sortbuf1[j] ) ;
					for ( k=0; k < MAX_NUM_WORD_LINE; k++ ) {
						sortbuf1[j][k] = ' ' ;
					}
					sortoff2 ++ ;
				}
			}
			for ( k=0; k < sortoff2; k++ ) {
				strcpy ( wordbuf[iWordLine], sortbuf2[k] ) ;
				iWordLine ++ ;
			}
			break ;
		}
		if ( i == sortoff1 ) {
			goto NEXT_SORT ;
		}
		else {
			goto NEXT_SORT_1 ;
		}
	}

	iWordSrcCount = 0 ;
	for ( k=0; k < iWordLine; k++ ) {
       	strcpy ( data, wordbuf[k] ) ;
		if ( data[0] == COMMENT_HEADER ) {		/* this line has been comment out */
			continue ;
		}
		datalen = strlen ( data ) ;
		if ( data[datalen-1] == '\n' ) {
			datalen -- ;
		}
		fgVersion = ( (data[datalen-1] >= '0') && (data[datalen-1] <= '9') ) ?
				HANIN_V4 : HANIN_V5 ;
		if ( datalen <= 2 ) {
			continue ;
		}
		else if ( datalen > SIZE_LINE_WORD ) {
			retCode = X2_READWORD_SIZE_OVERFLOW ;
			break ;
		}
		if ( fgVersion == HANIN_V5 ) {
			iFrqIdx = datalen - 3 ;
			stWordSource[iWordSrcCount].attr[0] = data[datalen-1] ;
			stWordSource[iWordSrcCount].attr[1] = 0 ;
		}
		else {	/* HANIN_V4 */
			iFrqIdx = datalen - 2 ;
			stWordSource[iWordSrcCount].attr[0] = 0 ;
		}
		stWordSource[iWordSrcCount].freq[0] = data[iFrqIdx] ;
		stWordSource[iWordSrcCount].freq[1] = data[iFrqIdx+1] ;
		stWordSource[iWordSrcCount].freq[2] = 0 ;
		/* check zhuyin cmpcode of word */
		for ( iCmpIdx = 0; ( data[iCmpIdx] & 0x80 ) == 0 && data[iCmpIdx]; 
			  iCmpIdx++ ) ;
		if ( iCmpIdx >= SIZE_CMPCODE_WORD ) {
			retCode = X2_READWORD_CMPCODE_OVERFLOW ;
			break ;
		}
		/* check word */
		for ( i=iCmpIdx; (i < iFrqIdx) && (data[i] & 0x80); i+=2 ) ;
		if ( i != iFrqIdx ) {
			retCode = X2_READWORD_NOT_2BYTECHAR ;
			break ;
		}
		/* duplicate word & zhuyin cmpcode */
		strncpy ( stWordSource[iWordSrcCount].cmp, data, iCmpIdx ) ;
		strncpy ( stWordSource[iWordSrcCount].code, data+iCmpIdx, iCmpIdx ) ;
		
		stWordSource[iWordSrcCount].len = iCmpIdx ;
		if ( iCmpIdx > 0 ) {
			iWordSrcCount ++ ;
		}
	}

    return retCode ;
}

/********************************************************************
 * Function Name: OpenBinDictionary
 * Description: open/create binary dictionary
 * Parameters:
 *     [input] char *fname
 *    [return] int 0: fail to open file
 *                 1: success
 * Revision Histor:
 *  2008.01.10  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
OpenBinDictionary ( char *fname )
{
	fBinDic = fopen ( fname, "w+b" ) ;
	return ( fBinDic == NULL ) ? HAN_FAILED : HAN_SUCCESS ;
}

/********************************************************************
 * Function Name: CloseBinDictionary
 * Description: close binary dictionary
 * Parameters:
 *     [input] char *fname
 *    [return] int 0: not able to close file
 *                 1: success
 * Revision Histor:
 *  2008.01.10  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
CloseBinDictionary ( void )
{
	if ( fBinDic == NULL ) {
		return HAN_FAILED ;
	}
	fclose ( fBinDic ) ;
	return HAN_SUCCESS ;
}

/********************************************************************
 * Function Name: SetBinDicOffset
 * Description: set the file offset for the binary dictionary
 * Parameters:
 *     [input] int fileoff
 *    [return] int 0: not able to close file
 *                 1: success
 * Revision Histor:
 *  2008.01.11  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
SetBinDicOffset ( int fileoff )
{
	if ( fBinDic == NULL ) {
		return HAN_FAILED ;
	}
	fseek ( fBinDic, fileoff, SEEK_SET ) ;
	return HAN_SUCCESS ;
}

/********************************************************************
 * Function Name: WriteBinDictionary
 * Description: write data into binary dictionary
 * Parameters:
 *     [input] int  offset: number of bytes to offset from origin
 *             char *wdata: pointer to the array of elements to be
 *                          written
 *               int wsize: size in bytes of each element
 *               int count: number of elements
 *    [return] int 0: dictionary is not ready
 *                 1: success
 * Revision Histor:
 *  2008.01.10  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
WriteBinDictionary ( int offset, char *wdata, int wsize, int count )
{
	if ( fBinDic == NULL ) {
		return HAN_FAILED ;
	}
	if ( offset >= 0 ) {
		fseek ( fBinDic, offset, SEEK_SET ) ;
	}
	fwrite ( wdata, wsize, count, fBinDic ) ;
	return HAN_SUCCESS ;
}

/********************************************************************
 * Function Name: WriteFirstChar
 * Description: write the fast(first) searching characters
 * Parameters:
 *     [input] PST_SINDEX1 sindex1: pointer to sindex1
 *    [return] void
 * Revision Histor:
 *  2008.01.15  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
void
WriteFirstChar ( PST_SINDEX1 sindex1 )
{
	int iLine ;
	int iIdx1 ;
	int c0cmp ;
	int iCount ;
	long fst_offset ;
	long fileoff ;
	ST_FSTCHAR stFirstChar[SIZE_INDEX_FSTCHAR] ;
	UCHAR ucFstCharCmp[SIZE_INDEX_FSTCHAR] ;
	
	iLine = 0 ;
	iIdx1 = 0 ;
	fst_offset = 0 ;
	fileoff = HDIC_OFF_SND_CMP_B ;
	memset ( stFirstChar, '\0', SIZE_INDEX_FSTCHAR * sizeof (ST_FSTCHAR) ) ;
	for ( c0cmp=CMPCODE_FIRST; c0cmp <= LAST_1ST_CMPCODE; c0cmp++ ) {
		if ( c0cmp > sindex1[iIdx1].schar ) {
			iIdx1 ++ ;
		}
		else if ( c0cmp == sindex1[iIdx1].schar ) {
			sindex1[iIdx1].fst_char_off = (UCHAR)(fst_offset & 0xFF) ;
			sindex1[iIdx1].hb_fst_char = (UCHAR)((fst_offset >> 8) & 0xFF) ;
			iIdx1 ++ ;
			iCount = 0 ;
			for ( ; iLine < iCharSrcCount; iLine++ ) {
				if ( stCharSource[iLine].cmp[0] != c0cmp ) {
					break ;
				}
				ucFstCharCmp[iCount] = stCharSource[iLine].cmp[1] ;
				strncpy ((char *)stFirstChar[iCount].ccode,
						 (char *)stCharSource[iLine].code, 10 ) ;
				iCount ++ ;
			}
			WriteBinDictionary ( fileoff, (char *)ucFstCharCmp, 1, iCount ) ;
			fileoff += iCount ;
			WriteBinDictionary ( fileoff, (char *)stFirstChar, 
								 sizeof ( ST_FSTCHAR ), iCount ) ;
			fileoff += iCount * sizeof ( ST_FSTCHAR ) ;
			fst_offset += iCount ;
		}
	}
	sindex1[iIdx1].fst_char_off = (UCHAR)(fst_offset & 0xFF) ;
	sindex1[iIdx1].hb_fst_char = (UCHAR)((fst_offset >> 8) & 0xFF) ;
}

/********************************************************************
 * Function Name: GetCharOffset
 * Description: get char offset regarding specified word
 * Parameters:
 *     [input] char *cmpbuf: pointer to cmpcode buffer
 *               int twcode: Chinese word
 *    [return] int <0: fail to find char offset
 *               iOff: char offset
 * Revision Histor:
 *  2008.01.11  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
GetCharOffset ( char *cmpbuf, int twcode )
{
	int iL, iH ;
	int iOff, cidx ;
	int iCharCode ;
	int iRet, iResult ;

	iL = 0 ;
	iH = iCharSrcCount - 1 ;
	while ( 1 ) {
		if ( iH < iL ) {
			iRet = X2_CHAROFF_CHAR_NOTFOUND ;
			break ;
		}
		iOff = ( iL + iH ) >> 1 ;
		iResult = strncmp ( cmpbuf, stCharSource[iOff].cmp, 2 ) ;
		if ( iResult == 0 ) {
			for ( cidx=0; cidx < stCharSource[iOff].len; cidx+=2 ) {
				iCharCode = 
					( ( stCharSource[iOff].code[cidx] & 0x00FF ) << 8 ) | 
					( stCharSource[iOff].code[cidx+1] & 0xFF ) ;
				if ( twcode == iCharCode ) {	/* found */
					cidx >>= 1 ;
					if ( cidx >= 64 ) {		/* index overflow */
						iRet = X2_CHAROFF_OFFSET_OVERFLOW ;
					}
					else {
						iRet = cidx ;
					}
					break ;
				}
			}
			if ( cidx == stCharSource[iOff].len ) {
				iRet = X2_CHAROFF_CHAR_NOTFOUND ;
			}
			break ;
		}
		else if ( iResult < 0 ) {
			iH = iOff - 1 ;
		}
		else {
			iL = iOff + 1 ;
		}
	}
	return iRet ;
}

#define BIG5_HANJI_START_HI		0xA4
#define BIG5_ZHUYIN_B			0xA374
#define BIG5_ZHUYIN_H			0xA37E
#define BIG5_ZHUYIN_J			0xA3A1
#define BIG5_ZHUYIN_S			0xA3AA
#define BIG5_ZHUYIN_A			0xA3AB
#define BIG5_ZHUYIN_ER			0xA3B7
#define BIG5_ZHUYIN_YI			0xA3B8
#define BIG5_ZHUYIN_YU			0xA3BA
#define BIG5_ZHUYIN_TONE1		0xA1C8
#define BIG5_ZHUYIN_TONE2		0xA3BD
#define BIG5_ZHUYIN_TONE4		0xA3BF
#define BIG5_ZHUYIN_TONE0		0xA3BB
#define BIG5_ZHUYIN_NOTONE		0xA1CA
#define BIG5_NUM_0				0xA2AF
#define BIG5_NUM_5				0xA2B4


/********************************************************************
 * Function Name: CompressBopomofo
 * Description: compress bopomofo into Hanin compressed code
 * Parameters:
 *     [input] char * wdbuf: word data
 *             int bIsWord: 1 for word, 0 for aimei
 *    [return] int iLen: length of compressed code
 * Revision Histor:
 *  2008.01.10  PTL Wu Jia-Kwan     for HDicMaker tool
********************************************************************/
int
CompressBopomofo ( UCHAR *wdbuf, int bIsWord )
{
	int i, j, iStart, iCount ;
	int code ;
	int iCmp ;
	UCHAR cmpbuf[SIZE_LINE_WORD+1] ;
	UCHAR tmpbuf[SIZE_LINE_WORD+1] ;
	UCHAR bt1, bt2, sion ;
	UCHAR symb ;
	
	iStart = 0 ;
	iCount = 0 ;
	iCmp = 0 ;
	for ( i=0; (int)(wdbuf[i]) < BIG5_HANJI_START_HI; i+=2 ) {
		if ( bIsWord == 0 && i >= (int)(strlen ( wdbuf )) ) {
			break ;
		}
		code = wdbuf[i] ;
		code = ( code << 8 ) + wdbuf[i+1] ;
		if ( (code >= BIG5_ZHUYIN_B) && (code <= BIG5_ZHUYIN_H) ) {
			symb = (UCHAR)(code - BIG5_ZHUYIN_B) ;
		}
		else if ( (code >= BIG5_ZHUYIN_J) && (code <= BIG5_ZHUYIN_S) ) {
			symb = (UCHAR)(code - BIG5_ZHUYIN_J + 11) ;
		}
		else if ( (code >= BIG5_ZHUYIN_YI) && (code <= BIG5_ZHUYIN_YU) ) {
			symb = (UCHAR)(code - BIG5_ZHUYIN_YI + 21) ;
		}
		else if ( (code >= BIG5_ZHUYIN_A) && (code <= BIG5_ZHUYIN_ER) ) {
			symb = (UCHAR)(code - BIG5_ZHUYIN_A + 24) ;
		}
		else if ( (code >= BIG5_ZHUYIN_TONE2) && (code <= BIG5_ZHUYIN_TONE4) ) {
			symb = (UCHAR)(code - BIG5_ZHUYIN_TONE2 + 39) ;
		}
		else if ( code == BIG5_ZHUYIN_TONE0 ) {
			symb = 42 ;
		}
		else if ( code == BIG5_ZHUYIN_TONE1 ) {
			symb = 38 ;
		}
		else if ( code == BIG5_ZHUYIN_NOTONE ) {
			symb = 37 ;
		}
		else if ( (code >= BIG5_NUM_0) && (code <= BIG5_NUM_5) ) {
			symb = (UCHAR)(code - BIG5_NUM_0 + 37) ;
		}
		tmpbuf[iCount++] = symb ;
		if ( symb >= 37 ) {
			bt1 = 0 ;
			bt2 = 0 ;
			sion = 0 ;
			for ( j=iStart; j < iCount; j++ ) {
				symb = tmpbuf[j] ;
				if ( (symb >= 0) && (symb <= 20) ) {
					bt1 += symb << 2 ;
					sion = 1 ;
				}
				else if ( (symb >= 21) && (symb <= 23) ) {
					bt1 += symb - 20 ;
				}
				else if ( (symb >= 24) && (symb <= 36) ) {
					bt2 += (symb - 23) * 6 ;
				}
				else if ( (symb >= 37) && (symb <= 42) ) {
					bt2 += symb - 37 ;
					if ( sion == 0 ) {
						bt1 += 84 ;
					}
					bt1 += (UCHAR)CMPCODE_FIRST ;
					bt2 += (UCHAR)CMPCODE_FIRST ;
				}
			}
			cmpbuf[iCmp++] = bt1 ;
			cmpbuf[iCmp++] = bt2 ;
			iStart = iCount ;
		}
	}
	for ( ; i < (int)(strlen ( wdbuf )) ; i++ ) {
		cmpbuf[iCmp++] = wdbuf[i] ;
	}
	cmpbuf[iCmp] = 0 ;
	if ( bIsWord ) {
		symb = cmpbuf[1] ;
		cmpbuf[1] = cmpbuf[2] ;
		cmpbuf[2] = symb ;
	}
	strcpy ( wdbuf, cmpbuf ) ;
	
	return iCmp ;
}

int
CompareCmpcode ( UCHAR *bufa, UCHAR *bufb )
{
	int	i ;

#ifdef OLD_VERSION
	for ( i = 0; ;i++ ) {
		if ( i < 4 ) {
			if ( *(bufa+i) < *(bufb+i) ) {
				return (-1) ;
			}
			else if( *(bufa+i) > *(bufb+i) ) {
				return (1) ;
			}
			continue;
		}
		else {
			if ( *(bufa+i) == '\0' ) {
				return (1) ;
			}
			if ( *(bufb+i) == '\0' ) {
				return (-1) ;
			}
		}
	}
#endif /* OLD_VERSION */
	i = 0 ;
	while ( 1 ) {
		if ( *((char *)bufa + i) < *((char *)bufb + i) ) {
			return (-1) ;
		}
		if ( *((char *)bufa + i) > *((char *)bufb + i) ) {
			return (1) ;
		}
		if ( *((char *)bufa + i) == '\0' ) {
			return 0 ;
		}
		i ++ ;
	}
}

/****************************************************************************
 * Copyright (C) 1999~2008 Panasonic Taiwan Laboratories Co., Ltd.
 ****************************************************************************/

