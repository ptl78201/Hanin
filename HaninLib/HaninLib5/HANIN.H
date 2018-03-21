#ifndef _Hanin_
#define _Hanin_

//#define		_MAC_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include "haninif.h"

#define	MAXPROFDICOPENED	3

#ifndef	NIL
#define	NIL				0
#endif

#ifndef max
#define max(a,b)        (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)        (((a) < (b)) ? (a) : (b))
#endif

#define	SUCCESS			2

#define	STD_BIT			0x01
#define	IBM_BIT			0x02
#define	ET_BIT			0x04
#define	MITEC_BIT		0x08

#define	CAPS_LOCK		0x40
#define	SHIFT			0x03


#define LENGTH_OF_RESULT        64	/* 32 double-byte codes + NULL    */
#define LENGTH_OF_MODE          64	/* 32 Max of  Mode buffer length  */
#define LENGTH_OF_DISPLAY       64      /* 32 double-byte codes + NULL    */
#define PIMIHDTBLSIZE           16	/* size of handle table           */

#define ERASE_CURRENT_RADICAL_KEY 0x0008	/* Backspace key  */
#define ESC_KEY                   0x001b	/* ESC key        */

//#define L_CHUIN			24     /* length of phonetic symbols input */
#define L_STNCE			40     /* length of phonetic symbols input */ /* еееее 6/22/95 */

#define W_START			21
#define	W_END			(W_START+L_STNCE+7+3)	// 8 -> 7
#define C_END			(W_START+L_STNCE+1)

/************************** DEFINE SYMBOLS ********************************/
#define SEARCH_ONE		0
#define SEARCH_ALL		1
#define SEARCH_CHAR		0
#define SEARCH_WORD		1

#define SP_INSERT		0x10	/* show_pinin mode */
#define SP_DELETE		0x20
#define SP_REPLACE		0x40
#define SP_POS			0x0f

#define	HANIN_CRYPT5C	0x5c
#define	HANIN_CRYPT		0x2a
#define	HANIN_CRYPT2A	0x2a
#define	TONE_COUNT		6	
#define	SPEC_CHAR		'\\'

#define ROLL_UP      1
#define ROLL_DN      2

/***************************** LD definition *******************************/
typedef struct 
{
    unsigned char last;
    unsigned char next;
    unsigned char cmpCode[4];
    unsigned char bigCode[4];

}	LDRec;
#define LD_MAX_WORD    100
#define LD_WORD_SIZE   sizeof(LDRec)

/***************************** UD definition *******************************/
#define UD_MAX_IDX     47
#define UD_MAX_SIZE    16384
#define UD_IDX_SIZE    ((UD_MAX_IDX+1)*2)
#define UD_MAX_RANGE   1024

/***************************** AD definition *******************************/
#define AD_MAX_WORD    256
#define AD_IDX_SIZE    (AD_MAX_WORD*6)

#define  OFF_LDSTART            24
#define  OFF_DDSTART            1048
#define  OFF_QDSTART            17528
#define  OFF_DEFINEDIDX         1048
#define  OFF_DEFINED            1144
#define  OFF_QUICKIDX           17528
#define  OFF_QUICK              19068
#define  QD_IDX_SIZE            6
#define  QD_IDXTABLE_SIZE       1536

/***************************** MD definition *******************************/
typedef struct 
{
    char schar;
    unsigned char	charoff2;
    unsigned char	idxoff2;
#ifdef	_MAC_
    unsigned char	charoff1:4;				// change for mac
    unsigned char	idxoff1:4;
#else
    unsigned char	idxoff1:4;
    unsigned char	charoff1:4;
#endif
}	IDX1;

typedef struct 
{
    unsigned char   schar;
    char			fseg;
    unsigned short  offset;
    unsigned short  clen;

} IDX2;
#define MD_MAX_IDX     58
#define MD_IDX_SIZE    (MD_MAX_IDX*sizeof(IDX1))

/***************************************************************************/
#define BASIC_SIZE		512
#define MD_IDX1_POS		0L
#define MD_IDX1_LEN		BASIC_SIZE
#define MD_ID_POS		(MD_IDX1_POS + MD_IDX1_LEN)
#define MD_ID_LEN		BASIC_SIZE
#define MD_IDX2_POS		(MD_ID_POS + MD_ID_LEN)
#define MD_IDX2_LEN		(BASIC_SIZE * 44)
#define MD_FCHAR_POS	23552L
#define MD_DIC_POS		44032L

#define USR_ID_POS		0L
#define USR_ID_LEN		12
#define USR_LAB_POS		(USR_ID_POS + USR_ID_LEN)
#define USR_LAB_LEN		12
#define USR_LD_POS		(USR_LAB_POS + USR_LAB_LEN)
#define USR_LD_LEN		1024
#define USR_UD_POS		(USR_LD_POS + USR_LD_LEN)
#define USR_UD_LEN		(UD_IDX_SIZE + UD_MAX_SIZE)
#define USR_AD_POS		(USR_UD_POS + USR_UD_LEN)
#define USR_AD_LEN		(AD_IDX_SIZE + 4 + 64 * AD_MAX_WORD)

/**************************************************************************/
typedef struct 
{
    short			clen[MAXPROFDICOPENED + 1];	  // length of char(s)
    short			wlen[MAXPROFDICOPENED + 1];	  // length of word(s)
    long			cidx[MAXPROFDICOPENED + 1];
    long			widx[MAXPROFDICOPENED + 1];
    short			wrange[MAXPROFDICOPENED + 1];
    char			mlenmark[MAXPROFDICOPENED + 1];  // length mark for MD
    char			ulenmark;  // length mark for UD
    char			nowkey[10];
    BYTE			fchar[10];
    char			flag;
    char			flag_bak;

}	CORE;
#define WRD_FG	      0x08
#define CHG_FG	      0x80
#define FRQ_FG	      0x70
#define LEN_FG	      0x07

/**************************************************************************/
struct spec_key 
{
    char    spec_key;
    char    key_fg;

};
extern struct spec_key spec_key[];

/******************************************************************/

#define NOR		0
#define REV		1
#define GREEN	2
#define RED		3
#define	BLUE	4
#define HILITE	5

#define BELL()					bell()
#define clr_line(x1, x2)		Clr_line(x1, x2)
#define TCAFLAG					1
#define codeconv(a)				a
#define toLong(lo, hi)			(Long)((Long)(lo) | ((Long)hi << 16))
#define ck_open(n, mode)		fopen(n, "r+")
#define ck_close(fd)			fclose(fd)
#define ck_lseek(fd, lo, hi, x)	fseek(fd, toLong(lo, hi), SEEK_SET)
#define ck_read(fd, buf, s)		fread(buf, 1, s, fd)
#define ck_write(fd, buf, s)	fwrite(buf, 1, s, fd)

#define LOBYTE(w)				((BYTE)(w))
#define HIBYTE(w)				((BYTE)(((unsigned short)(w) >> 8) & 0xFF))


typedef unsigned char	Char;
typedef unsigned short	Int;
typedef unsigned long	Long;

typedef unsigned char	*PSZ;
typedef unsigned short	USHORT;


extern  Char	inkey, inkey_bak;
extern	Char	scancode;
extern	Int		KB_STATUS;
extern	IDX1	mdIdxTable1[MAXPROFDICOPENED + 2][MD_MAX_IDX];
extern	LDRec	ldWord[LD_MAX_WORD+1];		/* last one for ldWordHead */
extern	short	udHashIdx[UD_MAX_IDX+1];
extern	BYTE	udHashBuf[UD_MAX_SIZE];
extern	Char	idxbuff[AD_IDX_SIZE];
extern	Char	toutbuf[AD_IDX_SIZE];
extern	short	chlen, oldchlen;			// choice buffer len > 255
extern	char	std_jui_iner[];
extern	char	ibm_jui_iner[];
extern	char	et_jui_iner[];
extern	char	mitec_jui_iner[];

extern	short	g_bReady;

extern	char	hanin_dic[];	// Hanin Dictionary
extern	char	add_dic[];		// Address Dictionary
extern	char	aimei_dic[];	// Aimei Dictionary
extern	char	user_dic[];		// User Dictionary
extern	char	prof_dic[][256];// Professional Dictionary

typedef struct _WORKAREA 
{
    USHORT      cchResult;		/** result packet             **/
    char        chResult[LENGTH_OF_RESULT];

    char        chMode[LENGTH_OF_MODE];
    char        attrMode[LENGTH_OF_MODE];
    char        chDisplay[LENGTH_OF_DISPLAY];
    char        attrDisplay[LENGTH_OF_DISPLAY];

	char		PininPos;	/* pinin symbol length (initial=-1) */
	char		InputState;
	char		AddrState;
	char		ENDSEL; 	// when cursor in the end, to select or modify
	char		born_flag;	/* recover zhuin */
	char		f_want; 	/* cursor position (byte) */
	char		j_off;		/* edit string length (byte) */
	char		ownoff; 	/* juiin symbol length (initial=0) */
	char		freq_fg;
	char		word_fg;
	Char		distmp[6];
	Char		youwant[L_STNCE];
	Char		youwant_bak[L_STNCE];
	Char		cmpkigo[L_STNCE];
	char		nowkey_buf[10]; 	       /* current input keys */
	CORE		Core[L_STNCE>>1];	       /* char information */
	Char		liang[3];
	Char		chbuf[600];					// choice buffer
	char		undoStart, undoLen;
	Char		rule_buf[10], pos_buf[10];
	Long		outoff;
	char		oldfreq;
	char		specChoice;

}	WORKAREA, *PWORKAREA;

typedef	struct t_aimeistruct
{
	short	nPos;
	short	nCount;
	BYTE	aimei_cmp[2];
	BYTE	szAimeiPron[80];

}	AIMEISTRUCT, *PAIMEISTRUCT;

typedef struct t_sortedlist
{
    int     		strSize;
    unsigned char   *cmp_code;
    unsigned char  	*char_code;

}	SORTEDLIST, *PSORTEDLIST;

// ud_fun.c
extern	short		sort_num;
extern	PSORTEDLIST	sort_list;


//extern WORKAREA far   *WPT[PIMIHDTBLSIZE];/* work area pointer table  */
extern WORKAREA 	*pWA;
extern char			*jui_iner;
extern char			PininMode;
extern char			DisplayMode;	/* 0(zhuin), 1(alpha) */
extern char			InputMode;		/* 1(std), 2(ibm), 3(et), 4(mitec) */
extern char			AddressState;	/* address mode */
extern char			ldDirty;
extern Char			ldWordHead;

#define InputState		pWA->InputState
#define	AddrState		pWA->AddrState
#define ENDSEL			pWA->ENDSEL
#define born_flag		pWA->born_flag
#define f_want			pWA->f_want
#define j_off			pWA->j_off
#define ownoff			pWA->ownoff
#define PininPos		pWA->PininPos
#define freq_fg			pWA->freq_fg
#define word_fg			pWA->word_fg
#define distmp			pWA->distmp
#define youwant			pWA->youwant
#define youwant_bak		pWA->youwant_bak
#define cmpkigo			pWA->cmpkigo
#define nowkey_buf		pWA->nowkey_buf
#define Core			pWA->Core
#define chbuf			pWA->chbuf
#define liang			pWA->liang
#define undoStart		pWA->undoStart
#define undoLen			pWA->undoLen
#define rule_buf		pWA->rule_buf
#define pos_buf			pWA->pos_buf
#define outoff			pWA->outoff
#define oldfreq			pWA->oldfreq
#define specChoice		pWA->specChoice

// functions
short	CheckDictionary();
void	reset_work_area();
short	realtime();
char	convert_attr(short attr);
void	initial();

// real.c
void	enter();
void	recover();

// allget.c
short	SearchAllWords();
short	SearchAllChars();

// ld_fun.c
short	LDSearchWord(short st, short len, short mode);
short	LDInsertWord(PBYTE cmpCode, PBYTE bigCode);
void	LDMoveWord(BYTE ldWordCur);
short	LDWrite();
void	InsertLearnCode (short learnlen);

// md_fun.c
short	MDSearchChar(short nDic, short st, short mode);
short	MDSearchWord(short nDic, short st, short len, short mode);
short	SearchOneWord(short st, short len);
short	SearchOneChar(short st);
short	RepSearchWord(short st);
short	BinarySearch(PBYTE table, BYTE item, short low, short high, short size);
short	MDGetCharAddress(short nDic, short st, FILE *wfp, short mode);

short	MDGetWordAddress(short nDic, short st, FILE *wfp);

short	ReadBuffer(PBYTE buf, FILE *wfp, short num);

short	CompareCmpcode(PBYTE cmpCode1, PBYTE cmpCode2, short len);
void	two_invert(unsigned char *s);

// ud_fun.c
short	UDSearchWord(short st, short len, short mode);
short	UDInsertWord(PBYTE cmpCode, PBYTE bigCode, short len);
void	UDWrite(PBYTE cmpCode, PBYTE bigCode, short len);
short	q_extend();
short	UDLoad();
int		hash_to_sort ();
short	GetUWordIndex (short nn);
void	do_omit (short nn);
short	GetJuinList (PBYTE pJuin);
short	GetFreeWords ();
void	SaveUerWord();

short	RegExt (PBYTE extbuf, PEXTENSION pExtension);
short	FindExtID (PBYTE extbuf);
short	GetExt (short idx, PEXTENSION pExtension);
short	DelExt (PBYTE extbuf);

// unit.c
short	UnitSearchWord(short st,short len,short mode);
short	fndx(short st);
short	Spatch();
short	chk_unit(short k);

// scren.c
void	shift_cursor(short count, short show);
short	backsp();

// choice.c
short	cmp2zhuin(PBYTE	cmpCode, PBYTE	zhuin);
short	sear_aimei(char *pron, char *aimei_pron);
short	GetSameProns (PHANIN pHanin, short searchlen, PBYTE pCandBuf, short CandBufSize, short bSpecCharOnly);
short	SelectSamePron (PHANIN pHanin, short selectlen, PBYTE pSelBuf, PCONVRESULT pConvResult);
short	SelectAimeiPron (PHANIN pHanin, PAIMEI pAimei, short idx, short selectlen, PBYTE pSelBuf, PCONVRESULT pConvResult);

// pinin.c
short	Code2Level(char code);
short	PininMatch();
void	PininDelete();

// compress.c
short	Handelete (short isBackSpace);
void	CompressZhuin(PBYTE zCode, PBYTE cmpCode);
void	RecoverZhuin(short loc);

// match.c
short	match();
void	lshift_data(short start);
short	SymbolMatch();
void	rshift_data();

// show_pin.c
void	show_pinin(short mode, char code);

// esc.c
void	fillback(short j, short i);
void	scroll();

// up_down.c
short	big5cmp(const unsigned char *s, short start, short len);
void	AppendToBuffer(PBYTE code, short count, short st, short len, short mode);
void	init_len_mark(short i);

// chuin.c
short	ZhuinMatch();

// rule.c
short	SpecialRuleMatch();

// cmp2pin.c
short	cmp2pinin(unsigned char *cmpCode, unsigned char *pinin);
short	zcmp2kcode(char dispMode, char pinMode, unsigned char *zCmp, unsigned char *kCode);

// os2api.c
void	bell();
void	Clr_line(short x1, short x2);
void	put_ch(short x, Char ch, short mode);
void	words_dis(short loc, PBYTE s, short len, short mode);
short	MDLoad(short i);
short	ProfDicLoad(short n);
short	ckUDLoad();

// big5e.c
short	ReadBig5E (char	*big5e_dic);
short	GetBig5EChar (char *pCmp, char *pCharBuf, short num);

#endif /* _Hanin_ */

