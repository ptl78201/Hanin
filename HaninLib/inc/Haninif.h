#ifndef _Haninif_
#define _Haninif_

# undef		False
# undef		True
# define	False		0
# define	True		1

// return code for HAN_RegUserWord & HAN_RegExtension
#define		RC_SUCCESS			0
#define		RC_OTHERERROR		-1

// return code for HAN_RegUserWord
#define		RC_WORDEXIST		-2
#define		RC_USERWORDFULL		-3
#define		RC_INVALIDWORD		-4
#define		RC_INUWMAINTENANCE	-5
#define		RC_ERRORUSERWORDLEN	-6

// return code for HAN_RegExtension
#define		RC_INVALIDEXTID		-2
#define		RC_NOLONGEXT		-3
#define		RC_ALREADYEXIST		-4
#define		RC_EXTENSIONFULL	-5
#define		RC_NOTFOUND			-6


/*********************** DEFINE SCAN CODES ********************************/
#define		DEL			0x53
#define		HOME		0x47
#define		END			0x4f

#define		RIGHT		0x4d
#define		LEFT		0x4b
#define		UP			0x48

#define		ESC			0x01
#define		BCKSP		0x0e
#define		RET			0x1c
#define		TAB			0x0f

// InputMode
#define		STDKBD	    1	// 標準鍵盤
#define		IBMKBD	    2	// IBM 鍵盤
#define		ETKBD	    3	// 倚天鍵盤
#define		MITECKBD    4	// 神通鍵盤
#define		JUIN2		5	// 注音二式拼音
#define		ROMAN		6	// 羅馬拼音

// DisplayMode
#define		DISPLAY_ZHUIN	0	// 注音顯示
#define		DISPLAY_ALPHA	1	// 英文字母顯示

// InputState
#define		IS_CHINESE		1	// 中文變換
#define		IS_PASS 		2	// 半形字母
#define		IS_FULLABC		3	// 全形英文字

// attributes of the characters in conversion buffer
#define		ATTRIB_INPUT		1	// 音
#define		ATTRIB_CONVERTED	2	// 中文字

//#pragma options align=mac68k

typedef unsigned char 	BYTE;
typedef unsigned char 	*PBYTE;

#define		PHANIN		void *
#define		PAIMEI		void *
#define		PUWM		void *

typedef	struct	t_hanininit
{
	char	hanin_dic[256];		// Hanin Dictionary
	char	aimei_dic[256];		// Aimei Dictionary
	char	add_dic[256];		// Address Dictionary
	char	user_dic[256];		// User Dictionary
	char	big5e_dic[256];		// big5-E char
	char	prof_dic[3][256];	// Professional Dictionary
	char	AddressState;		// 1: use address dictionary, 0: use Hanin dictionary
	char	InputMode;			// see InputMode definitions in this file
	char	DisplayMode;		// see DisplayMode definitions in this file

}	HANININIT, *PHANININIT;

typedef struct	t_inputkey
{
	BYTE	inkey;		// ascii
	BYTE	scancode;	// see DEFINE SCAN CODES above
	char 	KB_Caps;	// CapsLock key flag 1/0:on/off
	char	KB_Shift;	// Shift key flag 1/0:on/off

}	INPUTKEY, *PINPUTKEY;

typedef struct	t_convresult
{
	BYTE	szEdit[96];
	BYTE	szAttr[96];
	short	EditLength;
	BYTE	szSendBack[82];
	short	SendBackLength;
	short	CursorPos;

}	CONVRESULT,	*PCONVRESULT;

typedef struct	t_extension
{
	BYTE	szLong[42];
	short	LongExtLength;
	BYTE	szShort[26];
	short	ShortExtLength;

}	EXTENSION, *PEXTENSION;

typedef struct	t_userword
{
	BYTE	szZhuin[42];
	BYTE	szUserWord[12];

}	USERWORD, *PUSERWORD;

// function types
typedef short (*StartFunPtr) (PHANININIT pHaninInit);
typedef short (*QueryFunPtr) (PHANININIT pHaninInit);
typedef char (*SetInputModeFunPtr) (char cInputMode);
typedef char (*SetDisplayModeFunPtr) (char cDisplayMode);
typedef char (*SetAddressStateFunPtr) (PHANIN pHanin, char cAddressState);
typedef PHANIN (*OpenFunPtr) ();
typedef void (*CloseFunPtr) (PHANIN pHanin);
typedef char (*SetInputStateFunPtr) (PHANIN pHanin, char cInputState, PCONVRESULT pConvResult);
typedef short (*InputKeyFunPtr) (PHANIN pHanin, PINPUTKEY pInputKey, PCONVRESULT pConvResult);
typedef short (*IsAllConvertedFunPtr) (PHANIN pHanin);
typedef short (*IsSpecByteFunPtr) (PHANIN pHanin);
typedef void (*GetConvResultFunPtr) (PHANIN pHanin, PCONVRESULT pConvResult);
typedef void (*ClearEditBufFunPtr) (PHANIN pHanin, PCONVRESULT pConvResult);
typedef short (*GetSamePronCharsFunPtr) (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */, short bSpecCharOnly);
typedef short (*GetSamePronWordsFunPtr) (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */);
typedef short (*SelectSamePronCharFunPtr) (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult);
typedef short (*SelectSamePronWordFunPtr) (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult);
typedef PAIMEI (*BeginGetAimeiFunPtr) (PHANIN pHanin);
typedef short (*GetAimeiPronCountFunPtr) (PHANIN pHanin, PAIMEI pAimei);
typedef short (*GetAimeiCharsFunPtr) (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pCandBuf, short CandBufSize /* in bytes */, PBYTE pZhuin, short *pZhuinLength);
typedef short (*GetAimeiWordsFunPtr) (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pCandBuf, short CandBufSize /* in bytes */, PBYTE pZhuin, short *pZhuinLength);
typedef short (*SelectAimeiCharFunPtr) (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pSelBuf, PCONVRESULT pConvResult);
typedef short (*SelectAimeiWordFunPtr) (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pSelBuf, PCONVRESULT pConvResult);
typedef short (*EndGetAimeiFunPtr) (PHANIN pHanin, PAIMEI pAimei);

typedef short (*RegUserWordFunPtr) (PHANIN pHanin, short nLength);
typedef PUWM (*BeginUWordMaintenanceFunPtr) ();
typedef short (*GetZhuinListFunPtr) (PBYTE pZhuin);
typedef short (*GetUserWordStartIndexFunPtr) (PUWM pUwm, short iZhuin);
typedef short (*GetUserWordCountFunPtr) (PUWM pUwm);
typedef short (*GetUserWordFunPtr) (PUWM pUwm, short idx, PUSERWORD pUserWord);
typedef short (*DelUserWordFunPtr) (PUWM pUwm, short idx);
typedef short (*GetFreeUserCharsFunPtr) (PUWM pUwm);
typedef void (*EndUWordMaintenanceFunPtr) (PUWM pUwm);
typedef short (*GetExtensionFunPtr) (PHANIN pHanin, PEXTENSION pExtension);
typedef short (*GetExtIdxCountFunPtr) ();
typedef short (*GetAllExtIdxFunPtr) (PBYTE pExtIdx);
typedef short (*FindExtensionFunPtr) (PBYTE pExtIdx, PEXTENSION pExtension);
typedef short (*RegExtensionFunPtr) (PBYTE pExtIdx, PEXTENSION pExtension);
typedef short (*DelExtensionFunPtr) (PBYTE pExtIdx);

//#pragma options align=reset

// initialization and settings
extern short	HAN_Start (PHANININIT pHaninInit);			// start Hanin, return True:success/False:failure
extern short	HAN_Query (PHANININIT pHaninInit);			// query Hanin settings, return True:success/False:failure

extern PHANIN	HAN_Open ();				// create a Hanin instance, return a pointer to Hanin working data structure
extern void		HAN_Close (PHANIN pHanin);	// delete a Hanin instance
extern char		HAN_SetInputMode (char cInputMode);			// return old InputMode, -1:failure
extern char		HAN_SetDisplayMode (char cDisplayMode);		// return old DisplayMode, -1:failure
extern char		HAN_SetInputState (PHANIN pHanin, char cInputState, PCONVRESULT pConvResult);	// return old InputState, -1:failure
extern char		HAN_SetAddressState (PHANIN pHanin, char cAddressState);	// return old AddressState, -1:failure

// input and editing functions
extern short	HAN_InputKey (PHANIN pHanin, PINPUTKEY pInputKey, PCONVRESULT pConvResult);	// input a key, return True:success/False:failure
extern short	HAN_IsAllConverted (PHANIN pHanin);
extern short	HAN_IsSpecByte (PHANIN pHanin);
extern void		HAN_GetConvResult (PHANIN pHanin, PCONVRESULT pConvResult);
extern void		HAN_ClearEditBuf (PHANIN pHanin, PCONVRESULT pConvResult);

// utilities
extern short	HAN_GetSamePronChars (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */, short bSpecCharOnly);
extern short	HAN_GetSamePronWords (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */);
extern short	HAN_SelectSamePronChar (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult);
extern short	HAN_SelectSamePronWord (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult);

extern PAIMEI	HAN_BeginGetAimei (PHANIN pHanin);
extern short	HAN_GetAimeiPronCount (PHANIN pHanin, PAIMEI pAimei);
extern short	HAN_GetAimeiChars (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pCandBuf, short CandBufSize /* in bytes */, PBYTE pZhuin, short *pZhuinLength);
extern short	HAN_GetAimeiWords (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pCandBuf, short CandBufSize /* in bytes */, PBYTE pZhuin, short *pZhuinLength);
extern short	HAN_SelectAimeiChar (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pSelBuf, PCONVRESULT pConvResult);
extern short	HAN_SelectAimeiWord (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pSelBuf, PCONVRESULT pConvResult);
extern short	HAN_EndGetAimei (PHANIN pHanin, PAIMEI pAimei);

// user dictionary maintenance functions
extern short	HAN_RegUserWord (PHANIN pHanin, short nLength);

extern short	HAN_GetZhuinList (PBYTE pZhuin);

extern PUWM		HAN_BeginUWordMaintenance ();
extern short	HAN_GetUserWordStartIndex (PUWM pUwm, short iZhuin);
extern short	HAN_GetUserWordCount (PUWM pUwm);
extern short	HAN_GetUserWord (PUWM pUwm, short idx, PUSERWORD pUserWord);
extern short	HAN_DelUserWord (PUWM pUwm, short idx);
extern short	HAN_GetFreeUserChars (PUWM pUwm);
extern void		HAN_EndUWordMaintenance (PUWM pUwm);

extern short	HAN_GetExtension (PHANIN pHanin, PEXTENSION pExtension);

extern short	HAN_GetExtIdxCount ();
extern short	HAN_GetAllExtIdx (PBYTE pExtIdx);
extern short	HAN_FindExtension (PBYTE pExtIdx, PEXTENSION pExtension);
extern short	HAN_RegExtension (PBYTE pExtIdx, PEXTENSION pExtension);
extern short	HAN_DelExtension (PBYTE pExtIdx);

#endif /* _Haninif_ */

