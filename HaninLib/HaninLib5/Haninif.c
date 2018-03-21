#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hanin.h"

typedef	struct
{
	unsigned short	wStart, wEnd;

}	SPECRANGE;

SPECRANGE	SpecTable [] =
{
	{0xA140, 0xA17E},
	{0xA1A1, 0xA1FE},
	{0xA240, 0xA27E},
	{0xA2A1, 0xA2FE},
	{0xA340, 0xA37E},
	{0xA3A1, 0xA3BF},
	{0xF9DD, 0xF9FE}
};

void	Encrypt (PBYTE pByte, short nLength, BYTE CryptCode);
void	Decrypt (PBYTE pByte, short nLength, BYTE CryptCode);
char	SetInputMode (char cInputMode, char cDisplayMode);
char	GetInputMode ();
short	IsAllSpecByte (PHANIN pHanin);
short	DoSelectSpecChar (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult);
short	GetAllSpecChars (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */);

WORKAREA	*pWA;

Char	inkey, inkey_bak;
IDX1	mdIdxTable1[MAXPROFDICOPENED + 2][MD_MAX_IDX];
LDRec	ldWord[LD_MAX_WORD+1];   /* last one for ldWordHead */
short	udHashIdx[UD_MAX_IDX+1];
BYTE	udHashBuf[UD_MAX_SIZE];				//16384
Char	idxbuff[AD_IDX_SIZE];
Char	toutbuf[AD_IDX_SIZE];
short	chlen, oldchlen;	// choice buffer len > 255

char   *jui_iner;
char	PininMode;
char	DisplayMode;	/* 0(zhuin), 1(alpha) */
char	InputMode;		/* 1(std), 2(ibm), 3(et), 4(mitec) */
char	AddressState;

char	ldDirty;
Char	ldWordHead;

char	hanin_dic[256];		// Hanin Dictionary
char	add_dic[256];		// Address Dictionary
char	aimei_dic[256];		// Aimei Dictionary
char	user_dic[256];		// User Dictionary
char	big5e_dic[256];		// big5-E char
char	prof_dic[3][256];	// Professional Dictionary

short	g_bReady = False;
short	g_bInUWMaintenance = False;


short	HAN_Start (PHANININIT pHaninInit)
{
	int		i;

	strcpy (hanin_dic, pHaninInit->hanin_dic);
	strcpy (add_dic, pHaninInit->add_dic);
	strcpy (aimei_dic, pHaninInit->aimei_dic);
	strcpy (user_dic, pHaninInit->user_dic);
	strcpy (big5e_dic, pHaninInit->big5e_dic);
	for (i = 0; i < 3; i++)
		strcpy (prof_dic[i], pHaninInit->prof_dic[i]);

	CheckDictionary();
	// check aimeiSpec, prefSpec remarked by Kathy
	 
	if (big5e_dic[0] && ReadBig5E (big5e_dic) == 0)
		big5e_dic[0] = 0;

	AddressState = (pHaninInit->AddressState == 1)? 1: 0;

	SetInputMode (pHaninInit->InputMode, pHaninInit->DisplayMode);

	g_bReady = True;

	return True;
}

short	HAN_Query (PHANININIT pHaninInit)
{
	int		i;

	if (g_bReady == False)
		return False;

	strcpy (pHaninInit->hanin_dic, hanin_dic);
	strcpy (pHaninInit->add_dic, add_dic);
	strcpy (pHaninInit->aimei_dic, aimei_dic);
	strcpy (pHaninInit->user_dic, user_dic);
	strcpy (pHaninInit->big5e_dic, big5e_dic);
	for (i = 0; i < 3; i++)
		strcpy (pHaninInit->prof_dic[i], prof_dic[i]);

	pHaninInit->DisplayMode = DisplayMode;
	pHaninInit->AddressState = AddressState;

	pHaninInit->InputMode = GetInputMode ();

	return True;
}

char	HAN_SetInputMode (char cInputMode)
{
	if (g_bReady == False)
		return -1;

	return SetInputMode (cInputMode, DisplayMode);
}

char	HAN_SetDisplayMode (char cDisplayMode)
{
	char	cOldDisplayMode;

	if (g_bReady == False)
		return -1;

	cOldDisplayMode = DisplayMode;

	if (PininMode == 1 || PininMode == 2)
		DisplayMode = (cDisplayMode == DISPLAY_ALPHA) ? 
			DISPLAY_ALPHA : DISPLAY_ZHUIN;

	return cOldDisplayMode;
}

char	HAN_SetAddressState (PHANIN pHanin, char cAddressState)
{
	char	cOldAddressState;
	short	i;

	if (g_bReady == False)
		return -1;

	if (pHanin == NULL)
		return -1;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	cOldAddressState = AddrState;

	AddrState = (cAddressState == 1) ? 1 : 0;
	AddressState = AddrState;

	if (AddrState != cOldAddressState)
	{
		for (i = 0; i < (L_STNCE>>1); i++)
			init_len_mark(i);
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return cOldAddressState;
}

PHANIN	HAN_Open ()
{
	if (g_bReady == False)
		return NULL;

	pWA = (PWORKAREA) calloc (1, sizeof (WORKAREA));
	if (pWA == NULL)
		return NULL;

	reset_work_area();

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return (PHANIN) pWA;
}

void	HAN_Close (PHANIN pHanin)
{
	if (pHanin)
	{
		pWA = (PWORKAREA) pHanin;
		Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

		LDWrite();

		free (pHanin);
	}
}

char	HAN_SetInputState (PHANIN pHanin, char cInputState, PCONVRESULT pConvResult)
{
	char	cOldInputState;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	cOldInputState = InputState;

	InputState = cInputState;
	if (InputState < IS_CHINESE || InputState > IS_FULLABC)
		InputState = IS_CHINESE;

	//if (InputState != cOldInputState)
	//	initial ();

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	HAN_GetConvResult (pHanin, pConvResult);

	return cOldInputState;
}

short	HAN_InputKey (PHANIN pHanin, PINPUTKEY pInputKey, PCONVRESULT pConvResult)
{
	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	inkey = pInputKey->inkey;
	KB_STATUS = 0;
	if (pInputKey->KB_Caps)
		KB_STATUS |= CAPS_LOCK;
	if (pInputKey->KB_Shift)
		KB_STATUS |= SHIFT;
	scancode = pInputKey->scancode;

	realtime ();

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	HAN_GetConvResult (pHanin, pConvResult);

	return True;
}

short	HAN_IsAllConverted (PHANIN pHanin)
{
	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return False;
	}
	else
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return True;
	}
}

short	HAN_IsSpecByte (PHANIN pHanin)
{
	short	rc = False;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 1)
		rc = False;
	else
	{
		if (nowkey_buf[0] == SPEC_CHAR)
			rc = True;
		else
			rc = False;
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return rc;
}

void	HAN_ClearEditBuf (PHANIN pHanin, PCONVRESULT pConvResult)
{
	if (pHanin)
	{
		pWA = (PWORKAREA) pHanin;
		Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

		initial();

		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

		HAN_GetConvResult (pHanin, pConvResult);
	}
}

void	HAN_GetConvResult (PHANIN pHanin, PCONVRESULT pConvResult)
{
	int		i, j, k, len;

	if (pConvResult == NULL)
		return;
	
	pConvResult->szEdit[0] = 0;
	pConvResult->EditLength = 0;
	pConvResult->szSendBack[0] = 0;
	pConvResult->SendBackLength = 0;
	pConvResult->CursorPos = 0;

	if (g_bReady == False)
		return;

	if (pHanin == NULL)
		return;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	len = strlen (pWA->chDisplay);
	for (i = 0; i < len; i++)
		pConvResult->szEdit[i] = pWA->chDisplay[i];
	pConvResult->szEdit[i] = 0;
	pConvResult->EditLength = i;

	for (j = 0; j < i; j++)
		pConvResult->szAttr[j] = ATTRIB_CONVERTED;
	
	for (j = f_want, k = i - j_off; k > 0; k--, j++)
		pConvResult->szAttr[j] = ATTRIB_INPUT;
	pConvResult->szAttr[i] = 0;

	if (i != j_off)
		pConvResult->CursorPos = f_want + i - j_off;
	else
		pConvResult->CursorPos = f_want;

	len = strlen (pWA->chResult);
	if (len)
	{
		for (i = 0; i < len;i++)
			pConvResult->szSendBack[i] = pWA->chResult[i];
		pConvResult->szSendBack[i] = 0;
		pConvResult->SendBackLength = i;
	}
	else
	{
		pConvResult->szSendBack[0] = 0;
		pConvResult->SendBackLength = 0;
	}
	pWA->chResult[0] = 0;
	pWA->cchResult = 0;
	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
}

short	HAN_GetSamePronChars (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */, short bSpecCharOnly)
{
	short	len;

	if ((len = GetAllSpecChars (pHanin, pCandBuf, CandBufSize)) != 0)
		return len;
	else
		return GetSameProns (pHanin, 2, pCandBuf, CandBufSize, bSpecCharOnly);
}

short	HAN_GetSamePronWords (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize /* in bytes */)
{
	return GetSameProns (pHanin, 4, pCandBuf, CandBufSize, 0);
}

short	HAN_SelectSamePronChar (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	if (DoSelectSpecChar (pHanin, pSelBuf, pConvResult) == False)
		return SelectSamePron (pHanin, 2, pSelBuf, pConvResult);
	else
		return True;
}

short	HAN_SelectSamePronWord (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	return SelectSamePron (pHanin, 4, pSelBuf, pConvResult);
}

PAIMEI	HAN_BeginGetAimei (PHANIN pHanin)
{
	PAIMEISTRUCT	pAimeiStruct;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return NULL;
	}

	if (j_off < 2)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return NULL;
	}

	if (f_want == j_off)
	{
		shift_cursor (-2, 1);
		ENDSEL = 1;
	}

	if (cmpkigo[f_want] == '~')
	{
		if (ENDSEL)
		{
			shift_cursor (2, 1);
			ENDSEL = 0;
		}
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return NULL;
	}

	pAimeiStruct = (PAIMEISTRUCT) calloc (1, sizeof (AIMEISTRUCT));
	if (pAimeiStruct == NULL)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return NULL;
	}

	pAimeiStruct->nPos = f_want;
	pAimeiStruct->aimei_cmp[0] = cmpkigo[f_want];
	pAimeiStruct->aimei_cmp[1] = cmpkigo[f_want + 1];
	pAimeiStruct->nCount = sear_aimei (&cmpkigo[f_want], pAimeiStruct->szAimeiPron);

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}
	Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return (PAIMEI) pAimeiStruct;
}

short	HAN_GetAimeiPronCount (PHANIN pHanin, PAIMEI pAimei)
{
	short			count;
	PAIMEISTRUCT	pAimeiStruct;

	if (g_bReady == False)
		return 0;

	if (pHanin == NULL)
		return 0;

	if (pAimei == NULL)
		return 0;

	pAimeiStruct = (PAIMEISTRUCT) pAimei;
	Decrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);

	count = pAimeiStruct->nCount;

	Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);

	return count;
}

short	HAN_GetAimeiChars (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pCandBuf, short CandBufSize /* in bytes */, PBYTE pZhuin, short *pZhuinLength)
{
	short			i, j;
	short			len;
	PAIMEISTRUCT	pAimeiStruct;

	if (g_bReady == False)
		return 0;

	if (pHanin == NULL)
		return 0;

	if (pAimei == NULL)
		return 0;

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

	if (f_want == j_off)
	{
		shift_cursor (-2, 1);
		ENDSEL = 1;
	}

	cmpkigo[f_want] = pAimeiStruct->szAimeiPron[idx<<1];
	cmpkigo[f_want + 1] = pAimeiStruct->szAimeiPron[(idx<<1)+1];

	Core[f_want>>1].fchar[0] = 0;
	init_len_mark ((short)(f_want >> 1));
	if (f_want > 0)
		init_len_mark ((short) ((f_want>>1) - 1));
		
	if (pZhuin)
	{
		if (DisplayMode == DISPLAY_ALPHA)
			cmp2pinin(&cmpkigo[f_want], pZhuin);
		else
			cmp2zhuin(&cmpkigo[f_want], pZhuin);

		*pZhuinLength = strlen (pZhuin);
	}
    chlen = 0; *chbuf = '\0';
	SearchAllChars ();

	cmpkigo[f_want] = pAimeiStruct->aimei_cmp[0];
	cmpkigo[f_want + 1] = pAimeiStruct->aimei_cmp[1];

	Core[f_want>>1].fchar[0] = 0;
	init_len_mark ((short) ((int) f_want >> 1));
	if (f_want >  0)
		init_len_mark ((short) ((int) (f_want >> 1) - 1));

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}

	if (chlen > 0 && CandBufSize >= 2)
	{
		len = 0;
		for (i = 0; i < chlen; i += 2)
		{
			for (j = 0; j < len; j += 2)
			{
				if (strncmp (chbuf + i, pCandBuf + j, 2) == 0)
					break;
			}
			if (j == len)
			{
				strncpy (pCandBuf + len, chbuf + i, 2);
				len += 2;

				if (len + 2 > CandBufSize)
				{
					pCandBuf[len] = 0;

					Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
					Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

					return len;
				}
			}
		}
	}
	else
		len = 0;

	pCandBuf[len] = 0;

	Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return len;
}

short	HAN_GetAimeiWords (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pCandBuf, short CandBufSize /* in bytes */, PBYTE pZhuin, short *pZhuinLength)
{
	short			i, j;
	short			len;
	PAIMEISTRUCT	pAimeiStruct;

	if (g_bReady == False)
		return 0;

	if (pHanin == NULL)
		return 0;

	if (pAimei == NULL)
		return 0;

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

	if ((j_off == f_want && j_off < 4) || (j_off != f_want && j_off - f_want < 4))
	{
		Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return 0;
	}

	if (f_want == j_off)
	{
		shift_cursor (-2, 1);
		ENDSEL = 1;
	}

	cmpkigo[f_want] = pAimeiStruct->szAimeiPron[idx<<1];
	cmpkigo[f_want + 1] = pAimeiStruct->szAimeiPron[(idx<<1)+1];

	if (pZhuin)
	{
		if (DisplayMode == DISPLAY_ALPHA)
			cmp2pinin(&cmpkigo[f_want], pZhuin);
		else
			cmp2zhuin(&cmpkigo[f_want], pZhuin);

		*pZhuinLength = strlen (pZhuin);
	}

	Core[f_want>>1].fchar[0] = 0;
	init_len_mark ((short)(f_want >> 1));
	if (f_want > 0)
		init_len_mark ((short) ((f_want>>1) - 1));
		
	if (ENDSEL)
		shift_cursor (-2, 1);

    chlen = 0; *chbuf = '\0';
	SearchAllWords ();

	if (ENDSEL)
		shift_cursor (2, 1);

	cmpkigo[f_want] = pAimeiStruct->aimei_cmp[0];
	cmpkigo[f_want + 1] = pAimeiStruct->aimei_cmp[1];

	Core[f_want>>1].fchar[0] = 0;
	init_len_mark ((short) ((int) f_want >> 1));
	if (f_want >  0)
		init_len_mark ((short) ((int) (f_want >> 1) - 1));

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}

	if (chlen > 0 && CandBufSize >= 4)
	{
		len = 0;
		for (i = 0; i < chlen; i += 4)
		{
			for (j = 0; j < len; j += 4)
			{
				if (strncmp (chbuf + i, pCandBuf + j, 4) == 0)
					break;
			}
			if (j == len)
			{
				strncpy (pCandBuf + len, chbuf + i, 4);
				len += 4;

				if (len + 4 > CandBufSize)
				{
					pCandBuf[len] = 0;
					Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
					Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

					return len;
				}
			}
		}
	}
	else
		len = 0;

	pCandBuf[len] = 0;
	Encrypt ((PBYTE) pAimeiStruct, sizeof (AIMEISTRUCT), HANIN_CRYPT);
	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return len;
}

short	HAN_SelectAimeiChar (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	return SelectAimeiPron (pHanin, pAimei, idx, 2, pSelBuf, pConvResult);
}

short	HAN_SelectAimeiWord (PHANIN pHanin, PAIMEI pAimei, short idx, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	return SelectAimeiPron (pHanin, pAimei, idx, 4, pSelBuf, pConvResult);
}

short	HAN_EndGetAimei (PHANIN pHanin, PAIMEI pAimei)
{
	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	if (pAimei == NULL)
		return False;

	if (pAimei)
		free (pAimei);

	return True;
}
/*
short	HAN_IsSpecChar (PHANIN pHanin)
{
	short	rc = False;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0 || j_off == 0)
		return False;

	if (f_want == j_off)
	{
		shift_cursor (-2, 1);
		ENDSEL = 1;
	}

	if (cmpkigo[f_want] == '~')
		rc = True;

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return rc;
}
*/
short	GetAllSpecChars (PHANIN pHanin, PBYTE pCandBuf, short CandBufSize)
{
	short			n, i, len;
	unsigned short	w;

	if (IsAllSpecByte (pHanin) == False)
		return 0;

	if (g_bReady == False)
		return 0;

	if (pHanin == NULL)
		return 0;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0 || j_off == 0)
	{
		if (ownoff != 1 || nowkey_buf[0] != SPEC_CHAR)
		{
			Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
			return 0;
		}
	}
	else
	{
		if (f_want == j_off)
		{
			shift_cursor (-2, 1);
			ENDSEL = 1;
		}
	}

	n = sizeof (SpecTable) / sizeof (SPECRANGE);

	for (len = 0, i = 0; i < n; i++)
	{
		for (w = SpecTable [i].wStart; w <= SpecTable [i].wEnd; w++)
		{
			pCandBuf [len] = HIBYTE (w);
			pCandBuf [len + 1] = LOBYTE (w);

			len += 2;
			if (len + 3 >= CandBufSize)
				break;
		}
		if (len + 3 >= CandBufSize)
			break;
	}

	pCandBuf[len] = 0;

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return len;
}
/*
short	HAN_SelectSpecChar (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	short	rc = False;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0)
		return False;

	if (f_want == j_off)
	{
		shift_cursor (-2, 1);
		ENDSEL = 1;
	}

	if (cmpkigo[f_want] == '~')
	{
		cmpkigo[f_want + 1] = 0;

		youwant[f_want] = pSelBuf[0];
		youwant[f_want + 1] = pSelBuf[1];
		pWA->chDisplay[f_want] = pSelBuf[0];
		pWA->chDisplay[f_want + 1] = pSelBuf[1];

		init_len_mark (f_want >> 1);

		rc = True;
	}

	if (ENDSEL)
	{
		shift_cursor (2, 1);
		ENDSEL = 0;
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	HAN_GetConvResult (pHanin, pConvResult);

	return rc;
}
*/
short	HAN_RegUserWord (PHANIN pHanin, short nLength)
{
	short	rc;

	if (g_bReady == False)
		return RC_OTHERERROR;

	if (g_bInUWMaintenance)
		return RC_INUWMAINTENANCE;

	if (pHanin == NULL)
		return RC_OTHERERROR;

	if (nLength / 2 * 2 != nLength || nLength < 0 || nLength == 2)
		return RC_ERRORUSERWORDLEN;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff != 0)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return RC_OTHERERROR;
	}
	if (nLength == 0)
		nLength = j_off - f_want;

	if (j_off - f_want < nLength)
	{
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return RC_ERRORUSERWORDLEN;
	}
	rc = UDInsertWord(cmpkigo+f_want, youwant+f_want, nLength);

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return rc;
}

PUWM	HAN_BeginUWordMaintenance ()
{
	if (g_bReady == False)
		return NULL;

	if (g_bInUWMaintenance)
		return NULL;

	if (UDLoad () == -1)
		return NULL;

	if (hash_to_sort () <= 0)
		return NULL;

	g_bInUWMaintenance = True;

	return (PUWM) 100;
}

short	HAN_GetZhuinList (PBYTE pZhuin)
{
	if (g_bReady == False)
		return 0;

	return GetJuinList (pZhuin);
}

short	HAN_GetUserWordStartIndex (PUWM pUwm, short iZhuin)
{
	if (g_bInUWMaintenance == False)
		return 0;

	return GetUWordIndex (iZhuin);
}

short	HAN_GetUserWordCount (PUWM pUwm)
{
	if (g_bInUWMaintenance == False)
		return 0;

	return sort_num;
}

short	HAN_GetUserWord (PUWM pUwm, short idx, PUSERWORD pUserWord)
{
	BYTE	szTemp[10], byte;
	short	i;

	if (g_bInUWMaintenance == False)
		return 0;

	if (idx < 0 || idx >= sort_num)
		return 0;

	strncpy (pUserWord->szUserWord, 
		(sort_list + idx)->char_code, (sort_list + idx)->strSize);
	pUserWord->szUserWord[(sort_list + idx)->strSize] = 0;

	if (pUserWord->szZhuin)
	{
		memcpy (szTemp, (sort_list + idx)->cmp_code, (sort_list + idx)->strSize);
		byte = szTemp[1];
		szTemp[1] = szTemp[2];
		szTemp[2] = byte;

		pUserWord->szZhuin[0] = 0;
		for (i = 0; i < (sort_list + idx)->strSize; i += 2)
			cmp2zhuin(&szTemp[i], &pUserWord->szZhuin[strlen (pUserWord->szZhuin)]);
	}
	return (sort_list + idx)->strSize;
}

short	HAN_DelUserWord (PUWM pUwm, short idx)
{
	if (g_bInUWMaintenance == False)
		return False;

	if (idx < 0 || idx >= sort_num)
		return False;

	do_omit (idx);

	return True;
}

short	HAN_GetFreeUserChars (PUWM pUwm)
{
	if (g_bInUWMaintenance == False)
		return 0;

	return GetFreeWords ();
}

void	HAN_EndUWordMaintenance (PUWM pUwm)
{
	if (g_bInUWMaintenance == False)
		return;

	SaveUerWord ();

	g_bInUWMaintenance = False;
}

short	HAN_GetExtIdxCount ()
{
	short	i, count = 0;

	if (g_bReady == False)
		return 0;

	for (i = 0; i < QD_IDXTABLE_SIZE; i += QD_IDX_SIZE)
	{
		if (idxbuff[i])
			count++;
	}
	return count;
}

short	HAN_GetAllExtIdx (PBYTE pExtIdx)
{
	short	i, count = 0;

	if (g_bReady == False)
		return 0;

	for (i = 0; i < QD_IDXTABLE_SIZE; i += QD_IDX_SIZE)
	{
		if (idxbuff[i])
		{
			memcpy (&pExtIdx[count * QD_IDX_SIZE], &idxbuff[i], QD_IDX_SIZE); 
			count++;
		}
	}
	return count;
}

short	HAN_FindExtension (PBYTE pExtIdx, PEXTENSION pExtension)
{
	short	idx;

	if (g_bReady == False)
		return False;

	if ((idx = FindExtID (pExtIdx)) >= 0)
	{
		if (GetExt (idx, pExtension))
			return True;
		else
			return False;
	}
	else
		return False;
}

short	HAN_RegExtension (PBYTE pExtIdx, PEXTENSION pExtension)
{
	if (g_bReady == False)
		return RC_OTHERERROR;

	return RegExt (pExtIdx, pExtension);
}

short	HAN_DelExtension (PBYTE pExtIdx)
{
	if (g_bReady == False)
		return False;

	if (DelExt (pExtIdx) == RC_SUCCESS)
		return True;
	else
		return False;
}

short	HAN_GetExtension (PHANIN pHanin, PEXTENSION pExtension)
{
	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

    if (q_extend())
	{
		memcpy (pExtension->szLong, chbuf, 40);
		pExtension->szLong[40] = 0;
		pExtension->LongExtLength = strlen (pExtension->szLong);
		memcpy (pExtension->szShort, chbuf+40, 24);
		pExtension->szShort[24] = 0;
		pExtension->ShortExtLength = strlen (pExtension->szShort);
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		return True;
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return False;
}

void	Encrypt (PBYTE pByte, short nLength, BYTE CryptCode)
{
	int		i;

	for (i = 0; i < nLength; i++)
		pByte[i] = pByte[i] ^ CryptCode;
}

void	Decrypt (PBYTE pByte, short nLength, BYTE CryptCode)
{
	Encrypt (pByte, nLength, CryptCode);
}

char	SetInputMode (char cInputMode, char cDisplayMode)
{
	char	cOldInputMode;

	cOldInputMode = GetInputMode ();
	InputMode = cInputMode;
	if (InputMode < STDKBD || InputMode > ROMAN)
		InputMode = STDKBD;

	switch (InputMode)
	{
	case STDKBD:
	case IBMKBD:
	case ETKBD:
	case MITECKBD:
		PininMode = 0;
		DisplayMode = DISPLAY_ZHUIN;
		break;
	case JUIN2:
		InputMode = STDKBD;
		PininMode = 1;
		DisplayMode = (cDisplayMode == DISPLAY_ALPHA) ? 
			DISPLAY_ALPHA : DISPLAY_ZHUIN;
		break;
	case ROMAN:
		InputMode = STDKBD;
		PininMode = 2;
		DisplayMode = (cDisplayMode == DISPLAY_ALPHA) ? 
			DISPLAY_ALPHA : DISPLAY_ZHUIN;
		break;
	}
	if (PininMode == 0)
	{
		switch (InputMode)
		{
		case STDKBD:
			jui_iner = std_jui_iner;
			break;
		case IBMKBD:
			jui_iner = ibm_jui_iner;
			break;
		case ETKBD:
			jui_iner = et_jui_iner;
			break;
		case MITECKBD:
			jui_iner = mitec_jui_iner;
			break;
		}
	}
	return cOldInputMode;
}

char	GetInputMode ()
{
	switch (InputMode)
	{
	case STDKBD:
		if (PininMode == 1)
			return JUIN2;
		else if (PininMode == 2)
			return ROMAN;
	}
	return InputMode;
}

short	IsAllSpecByte (PHANIN pHanin)
{
	short	rc = False;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff > 1)
		rc = False;
	else if (ownoff == 1)
	{
		if (nowkey_buf[0] == SPEC_CHAR)
			rc = True;
		else
			rc = False;
	}
	else if (j_off == 0)
		rc = False;
	else
	{
		if (f_want == j_off)
		{
			shift_cursor (-2, 1);
			ENDSEL = 1;
		}

		if (cmpkigo[f_want] == '~' && cmpkigo[f_want + 1] == ' ')
			rc = True;
		else
			rc = False;
	
		if (ENDSEL)
		{
			shift_cursor (2, 1);
			ENDSEL = 0;
		}
	}

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	return rc;
}

short	DoSelectSpecChar (PHANIN pHanin, PBYTE pSelBuf, PCONVRESULT pConvResult)
{
	short		ownoff_bak;
	INPUTKEY	InputKey;

	if (IsAllSpecByte (pHanin) == False)
		return False;

	if (g_bReady == False)
		return False;

	if (pHanin == NULL)
		return False;

	pWA = (PWORKAREA) pHanin;
	Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	ownoff_bak = ownoff;

	Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

	if (ownoff_bak == 1)
	{
		InputKey.inkey = 'A';
		InputKey.scancode = 0;
		InputKey.KB_Caps = InputKey.KB_Shift = 0;
		HAN_InputKey (pHanin, &InputKey, NULL); 

		InputKey.inkey = 0;
		InputKey.scancode = LEFT;
		InputKey.KB_Caps = InputKey.KB_Shift = 0;
		HAN_InputKey (pHanin, &InputKey, NULL); 

		pWA = (PWORKAREA) pHanin;
		Decrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);
		cmpkigo[f_want + 1] = ' ';
		Encrypt ((PBYTE) pWA, sizeof (WORKAREA), HANIN_CRYPT5C);

		SelectSamePron (pHanin, 2, pSelBuf, NULL);

		InputKey.inkey = 0;
		InputKey.scancode = RIGHT;
		InputKey.KB_Caps = InputKey.KB_Shift = 0;
		HAN_InputKey (pHanin, &InputKey, pConvResult); 
	}
	else
		SelectSamePron (pHanin, 2, pSelBuf, pConvResult);

	return True;
}

// temp
void reset_work_area()
{
    pWA->cchResult = 0;
    pWA->chResult[0] = 0;

    memset(pWA->chMode, ' ', LENGTH_OF_MODE);
    pWA->chMode[LENGTH_OF_MODE-1] = 0;
    memset(pWA->attrMode, convert_attr(NOR), LENGTH_OF_MODE);

    memset(pWA->chDisplay, 0, LENGTH_OF_DISPLAY);
    memset(pWA->attrDisplay, 0, LENGTH_OF_DISPLAY);
    
    initial();

	InputState = IS_CHINESE;
	AddrState = AddressState;
	specChoice = 0;
}
