/****************************************************************************
Copyright (C) 1999~2008 Panasonic Taiwan Laboratories Co., Ltd.
-----------------------------------------------------------------------------
 Project    :
*****************************************************************************
File Name   : hdic_def.h
Description : definition, data structure for Hanin dicitonary maker tool
History     :
 No.   Ver.     Date       Designed by          Comment
 -----+--------+----------+--------------------+-----------------------------
  000   0.00    1999.02.08  PTL Wu Jia-Kwan     received from Hanin project
  001   1.00    2007.12.28  PTL Wu Jia-Kwan     revised for HDicMaker tool
*****************************************************************************/

#ifndef _HDIC_DEF_H_
#define _HDIC_DEF_H_

/**************************************
  typedef
**************************************/
typedef unsigned char   UCHAR;
typedef unsigned short  UWORD;

/**************************************
  Definition
**************************************/
#define HAN_FAILED				0
#define HAN_SUCCESS				1
#define HAN_ENCRYPTION			0x0

#define WORD_AREA_SIZE          5120
#define MAX_CHAR_LINE_CNT       2400
#define MAX_WORD_LINE_CNT       100000
#define MAX_AIMEI_LINE_CNT		64
#define MAX_NUM_WORD_LINE		64
#define MAX_NUM_FSTCHAR			60

#define MAX_ASSIGNED_CHAR       60
#define MAX_FIRST_ARR_S         57		/* v5: 70 */
#define MAX_SECOND_ARR_S        3550    /* v5: 5000 */

/* compressed code */
#define TONE_COUNT				6
#define CMPCODE_FIRST			0x21	/* '!' */
#define LAST_1ST_CMPCODE		0x78	/* 'x' */
#define LAST_2ND_CMPCODE		0x74	/* 't' */
#define WORDFRQ_FIRST			0x30	/* '0' */
#define LAST_ASCII_CODE			0x7F
#define COMMENT_HEADER			('|')

/* specific position in Hanin dictionary */
#define HDIC_VERSION4           "漢音4.0"
#define HDIC_VERSION5           "漢音5.0"

#define HDIC_NO_OFFSET			(-1)
#define HDIC_OFF_FST_IDX_B		0
#define HDIC_OFF_ID_AREA_B		512		/* 0x0200 */
#define HDIC_OFF_SND_IDX_B		1024	/* 0x0400 */
#define HDIC_OFF_SND_CMP_B		23552	/* 0x5C00, v5:0x8000 */
#define HDIC_OFF_DIC_B			44032	/* 0xAC00, v5:0xD000 */
#define HDIC_OFF_LABEL			(HDIC_OFF_ID_AREA_B+SIZE_LABEL_VERSION)

/*--- part of speech ---*/
#define NOUN                    0x1     /* 名詞 */
#define VERB                    0x2     /* 動詞 */
#define ADJECTIVE               0x4     /* 形容詞 */
#define ADVERB                  0x8     /* 副詞 */
#define CONJUNCTION             0x10    /* 連接詞 */
#define TITLE                   0x20    /* 頭銜 */

/*--- catogory of source text dictionary ---*/
#define DIC_ZHUYIN              1
#define DIC_AIMEI               2
#define DIC_CANTON              3

/*--- limit buffer size in structure ---*/
#define SIZE_CMPCODE_CHAR       2
#define SIZE_2BYTE				2
#define SIZE_CMPCODE_SAME       500
#define SIZE_CMPCODE_WORD       20
#define SIZE_WORDFREQ           4
#define SIZE_WORDATTR           2
#define SIZE_FIRST_CHAR         5
#define SIZE_INDEX_FSTCHAR		60
#define SIZE_INDEX_TABLE		4096
#define SIZE_LABEL_VERSION		12

#define SIZE_WORD_AREA			5120
#define SIZE_NEW2WORD_LEN		100
#define SIZE_INDEX3_BUFF		100

#define SIZE_LINE_CHAR      	511
#define SIZE_LINE_WORD      	127

/*--- Error code ---*/
#define X1_MKDIC_CHAR_FAILED			(-1)
#define X1_MKDIC_WORD_FAILED			(-2)
#define X1_MKDIC_OPENDIC_FAILED			(-3)
#define X1_MKDIC_IDX1INDEX_OVERFLOW		(-4)
#define X1_MKDIC_FILEINDEX_ZERO			(-5)
#define X1_MKDIC_CMPORDER				(-6)
#define X1_MKDIC_WORDAREA_OVERFLOW		(-7)

#define X2_READCHAR_SIZE_OVERFLOW		(-1)
#define X2_READCHAR_NO_CMPCODE			(-2)
#define X2_READCHAR_NOT_2BYTECHAR		(-3)
#define X2_READCHAR_MAX_OVERFLOW		(-4)
#define X2_READWORD_SIZE_OVERFLOW		(-5)
#define X2_READWORD_CMPCODE_OVERFLOW	(-6)
#define X2_READWORD_NOT_2BYTECHAR		(-7)
#define X2_CHAROFF_CHAR_NOTFOUND		(-8)
#define X2_CHAROFF_OFFSET_OVERFLOW		(-9)
#define X2_AIMEI_OPENTXT_FAILED			(-10)
#define X2_AIMEI_OPENBIN_FAILED			(-11)
#define X2_AIMEI_ORDER_ERROR			(-12)
#define X2_AIMEI_DATA_TOO_LONG			(-13)
#define X2_AIMEI_PRON_REPEAT			(-14)

/**************************************
  Enumeration
**************************************/
enum eDICVERSION {
    HANIN_V4,
    HANIN_V5
} ;

/**************************************
  Data Structure
**************************************/
typedef struct _char_cmp {
    char    cmp[SIZE_CMPCODE_CHAR] ;
    char    code[SIZE_CMPCODE_SAME] ;
    int     len ;
    int     status ;
} CHAR_CMP_BUF, *PCHAR_CMP_BUF ;

typedef struct _word_cmp {
    char    cmp[SIZE_CMPCODE_WORD] ;
    char    code[SIZE_CMPCODE_WORD] ;
    char    freq[SIZE_WORDFREQ] ;
    char    attr[SIZE_WORDATTR] ;
    int     len ;
    int     status ;
} WORD_CMP_BUF, *PWORD_CMP_BUF ;

typedef struct _new2_stru {
    char    cmp[SIZE_CMPCODE_WORD] ;
    char    code[SIZE_CMPCODE_WORD] ;
    int     len ;
} ST_NEW2 ;

typedef struct _stru_sindex1 {
    char    schar ;
    UCHAR   fst_char_off ;
    UCHAR   snd_idx_off ;
    UCHAR   hb_snd_idx:4 ;
    UCHAR   hb_fst_char:4 ;
} ST_SINDEX1, *PST_SINDEX1 ;

typedef struct _stru_sindex2 {
    char    schar ;
    char    seq ;
    UWORD   foff ;
    UWORD   clen ;
} ST_SINDEX2, *PST_INDEX2 ;

typedef struct _stru_attr_freq_char {
    UCHAR   char_code[SIZE_FIRST_CHAR][SIZE_CMPCODE_CHAR] ;
    UCHAR   attr_freq[SIZE_WORDATTR] ;
} ST_ATTRFREQ_CHAR, *PST_ATTRFREQ_CHAR ;

typedef struct _stru_1st_cmpcode {
	UCHAR	ccode[SIZE_FIRST_CHAR][SIZE_2BYTE] ;
	UCHAR	attr1:5 ;
	UCHAR	attr2:3 ;
	UCHAR	attr3:3 ;
	UCHAR	attr4:3 ;
} ST_FSTCHAR, *PST_FSTCHAR ;

/**************************************
  Global Functions
**************************************/

/**************************************
  Global Variables
**************************************/
/*--- hdic_rw.c ---*/
extern enum eDICVERSION eDicVer ;

/*--- hdicmain.c ---*/
extern int iCharSrcCount ;
extern int iWordSrcCount ;
extern CHAR_CMP_BUF stCharSource[MAX_CHAR_LINE_CNT] ;
extern WORD_CMP_BUF stWordSource[MAX_WORD_LINE_CNT] ;
extern UCHAR wordbuf[MAX_WORD_LINE_CNT][MAX_NUM_WORD_LINE] ;

#endif /* _HDIC_DEF_H_ */
