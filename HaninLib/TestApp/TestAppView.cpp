// TestAppView.cpp : implementation of the CTestAppView class
//

#include "stdafx.h"
#include "TestApp.h"

#include "TestAppDoc.h"
#include "TestAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void	OutResult (PCONVRESULT pConvResult);

/////////////////////////////////////////////////////////////////////////////
// CTestAppView

IMPLEMENT_DYNCREATE(CTestAppView, CEditView)

BEGIN_MESSAGE_MAP(CTestAppView, CEditView)
	//{{AFX_MSG_MAP(CTestAppView)
	ON_UPDATE_COMMAND_UI(ID_INPUT, OnUpdateInput)
	ON_COMMAND(ID_INPUT, OnInput)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestAppView construction/destruction

CTestAppView::CTestAppView()
{
	HANININIT	HaninInit;

	//strcpy (HaninInit.hanin_dic, "d:\\common\\hanin502\\Hanin.Dic");
	strcpy (HaninInit.hanin_dic, "d:\\hanin5\\HaninDic");
	strcpy (HaninInit.add_dic, "d:\\hanin5\\AdrDic");
	strcpy (HaninInit.aimei_dic, "d:\\hanin5\\AimeiDic");
	strcpy (HaninInit.user_dic, "d:\\hanin5\\UserDic");
	strcpy (HaninInit.big5e_dic, "D:\\Apple\\big5-e\\big5edic");
	HaninInit.prof_dic[1][0] = 0;
	HaninInit.prof_dic[2][0] = 0;
	strcpy (HaninInit.prof_dic[0], "d:\\hanin5\\Expert\\Biology");
	//strcpy (HaninInit.prof_dic[0], "d:\\hanin5\\Expert\\Military");
	HaninInit.AddressState = 0;
	HaninInit.InputMode = ROMAN;
	HaninInit.DisplayMode = /*DISPLAY_ALPHA; */DISPLAY_ZHUIN;

	HAN_Start (&HaninInit);

	m_pHanin = NULL;
	m_bKeyIn = FALSE;
}

CTestAppView::~CTestAppView()
{
	if (m_pHanin)
		HAN_Close (m_pHanin);
}

BOOL CTestAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// Enable word-wrapping

	return bPreCreated;
}

/////////////////////////////////////////////////////////////////////////////
// CTestAppView drawing

void CTestAppView::OnDraw(CDC* pDC)
{
	CTestAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CTestAppView printing

BOOL CTestAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default CEditView preparation
	return CEditView::OnPreparePrinting(pInfo);
}

void CTestAppView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView begin printing.
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CTestAppView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// Default CEditView end printing
	CEditView::OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CTestAppView diagnostics

#ifdef _DEBUG
void CTestAppView::AssertValid() const
{
	CEditView::AssertValid();
}

void CTestAppView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CTestAppDoc* CTestAppView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestAppDoc)));
	return (CTestAppDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestAppView message handlers

void CTestAppView::OnUpdateInput(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CTestAppView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (m_pHanin)
		m_bKeyIn = True;	
	CEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestAppView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BYTE		szKeyState[256];
	INPUTKEY	InputKey;
	CONVRESULT	ConvResult;

	if (m_pHanin)
	{
		if (m_bKeyIn == FALSE)
		{
			CEditView::OnChar(nChar, nRepCnt, nFlags);
			return;
		}
		switch (nChar)
		{
		case VK_DELETE:
			InputKey.scancode = DEL;
			break;
		case VK_HOME:
			InputKey.scancode = HOME;
			break;
		case VK_END:
			InputKey.scancode = END;
			break;
		case VK_RIGHT:
			InputKey.scancode = RIGHT;
			break;
		case VK_LEFT:
			InputKey.scancode = LEFT;
			break;
		case VK_UP:
			InputKey.scancode = UP;
			break;
		case VK_ESCAPE:
			InputKey.scancode = ESC;
			break;
		case VK_BACK:
			InputKey.scancode = BCKSP;
			break;
		case VK_RETURN:
			InputKey.scancode = RET;
			break;
		default:
			InputKey.scancode = 0;
			break;
		}
		InputKey.inkey = nChar;
		m_bKeyIn = FALSE;
		
		GetKeyboardState (szKeyState);
		InputKey.KB_Caps = szKeyState[VK_CAPITAL];
		InputKey.KB_Shift = (szKeyState[VK_SHIFT] & 0x80) ? 1 : 0;

		HAN_InputKey (m_pHanin, &InputKey, &ConvResult);

		OutResult (&ConvResult);
	}

	CEditView::OnChar(nChar, nRepCnt, nFlags);
}

void CTestAppView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	BYTE		szKeyState[256];
	INPUTKEY	InputKey;
	CONVRESULT	ConvResult;

	if (m_pHanin)
	{
		if (m_bKeyIn == FALSE)
		{
			CEditView::OnKeyUp(nChar, nRepCnt, nFlags);
			return;
		}
		m_bKeyIn = FALSE;

		GetKeyboardState (szKeyState);
		InputKey.KB_Caps = szKeyState[VK_CAPITAL];
		InputKey.KB_Shift = (szKeyState[VK_SHIFT] & 0x80) ? 1 : 0;

		switch (nChar)
		{
		case VK_DELETE:
			InputKey.scancode = DEL;
			break;
		case VK_HOME:
			InputKey.scancode = HOME;
			break;
		case VK_END:
			InputKey.scancode = END;
			break;
		case VK_RIGHT:
			InputKey.scancode = RIGHT;
			break;
		case VK_LEFT:
			InputKey.scancode = LEFT;
			break;
		case VK_UP:
			InputKey.scancode = UP;
			break;
		case VK_ESCAPE:
			InputKey.scancode = ESC;
			break;
		case VK_BACK:
			InputKey.scancode = BCKSP;
			break;
		case VK_RETURN:
			InputKey.scancode = RET;
			break;
		}

		InputKey.inkey = 0;

		HAN_InputKey (m_pHanin, &InputKey, &ConvResult); 

		OutResult (&ConvResult);
	}
	CEditView::OnKeyUp(nChar, nRepCnt, nFlags);
}

void	OutResult (PCONVRESULT pConvResult)
{
	return;

	HDC		hDC;
	hDC = ::GetDC (NULL);

	strcat ((char *)pConvResult->szEdit, "                                ");
	strcat ((char *)pConvResult->szSendBack, "                                ");
	::TextOut (hDC, 0, 100, (const char *) pConvResult->szEdit, strlen ((char *)pConvResult->szEdit));
	::TextOut (hDC, 0, 150, (const char *) pConvResult->szSendBack, strlen ((char *)pConvResult->szSendBack));

	::ReleaseDC (NULL, hDC);
}

void CTestAppView::OnInput() 
{
	PUWM		pUwm;
	BYTE		szZhuin[76];
	BYTE		szCandBuf[900];
	BYTE		szUserWord[8];
	short		i, len, n;
	short		nZhuinLength;
	PHANIN		pHanin;
	PAIMEI		pAimei;
	INPUTKEY	InputKey;
	USERWORD	UserWord;
	EXTENSION	Extension, ExtensionTemp;
	CONVRESULT	ConvResult;

	if (pHanin = HAN_Open ())
	{
		InputKey.KB_Caps = 0;
		InputKey.KB_Shift = 0;
		InputKey.scancode = 0;

        InputKey.inkey = 'b';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'a';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '2';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 

		n = HAN_GetSamePronChars (pHanin, szCandBuf, sizeof (szCandBuf), 0);

        InputKey.inkey = 'n';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'a';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '2';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 

		n = HAN_GetSamePronChars (pHanin, szCandBuf, sizeof (szCandBuf), 0);

		InputKey.scancode = 0;
/* 3356186
		// 1
        strcpy ((char *)Extension.szLong, "松下電器技術開發");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "松下電器");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"松下", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 2
        strcpy ((char *)Extension.szLong, "蘋果電腦");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "香港");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"蘋果", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 3
        strcpy ((char *)Extension.szLong, "中國大陸");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "大陸");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"中國", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 4
        strcpy ((char *)Extension.szLong, "股份有限公司");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "有限公司");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"公司", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 5
        strcpy ((char *)Extension.szLong, "導遊");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "遊覽車");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"旅行", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 6
        strcpy ((char *)Extension.szLong, "交通工具");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "旅遊");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"飛機", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 7
        strcpy ((char *)Extension.szLong, "國內旅遊");
        //strcpy ((char *)Extension.szShort, "27556183");
        strcpy ((char *)Extension.szShort, "國外旅遊");
        Extension.LongExtLength = strlen ((char *)Extension.szLong);
        Extension.ShortExtLength = strlen ((char *)Extension.szShort);
        n = HAN_RegExtension ((unsigned char *)"渡假", &Extension);//1 is True, 0 is false
        if(n == RC_SUCCESS)//HAN_RegExtension successfully register the chars but return 0
            MessageBeep (0);

        len = HAN_GetExtIdxCount ();
        i = HAN_GetAllExtIdx (szZhuin);

		// 松下       
        InputKey.inkey = 's';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'o';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'n';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = ' ';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'x';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'a';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '4';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult);

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "松下", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "松下");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();

		InputKey.scancode = BCKSP;
        InputKey.inkey = 0;
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
		InputKey.scancode = 0;

		// 蘋果       
        InputKey.inkey = 'p';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'n';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '2';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'u';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'o';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '3';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult);

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "蘋果", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "蘋果");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();

		InputKey.scancode = BCKSP;
        InputKey.inkey = 0;
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
		InputKey.scancode = 0;

		// 中國       
        InputKey.inkey = 'z';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'h';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'o';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'n';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = ' ';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'u';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'o';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult);
        InputKey.inkey = '2';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult);

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "中國", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "中國");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();

		InputKey.scancode = BCKSP;
        InputKey.inkey = 0;
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
		InputKey.scancode = 0;

		// 公司       
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'o';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'n';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = ' ';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 's';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = ' ';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "公司", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "公司");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();

		InputKey.scancode = BCKSP;
        InputKey.inkey = 0;
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
		InputKey.scancode = 0;

		// 旅行       
        InputKey.inkey = 'l';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'v';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '3';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'x';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'n';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'g';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '2';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "旅行", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "旅行");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();

		InputKey.scancode = BCKSP;
        InputKey.inkey = 0;
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
		InputKey.scancode = 0;

		// 飛機       
        InputKey.inkey = 'f';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'e';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = ' ';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'j';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = ' ';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "飛機", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "飛機");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();

		// 渡假       
        InputKey.inkey = 'd';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'u';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '4';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'j';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'i';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = 'a';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 
        InputKey.inkey = '4';
        n = HAN_InputKey (pHanin, &InputKey, &ConvResult); 

        n = HAN_GetExtension (pHanin, &ExtensionTemp);	// 1 is True, 0 is false
        if(n == True)//HAN_GetExtension always return 0
            MessageBeep (0);
        
        i = HAN_FindExtension ((PBYTE) "飛機", &Extension);		// 1 is True, 0 is false
        if(i == True)
            MessageBeep (0);

        i = HAN_GetExtIdxCount ();								
        n = HAN_DelExtension ((PBYTE) "飛機");
        if(n == True)
            MessageBeep (0);
        i = HAN_GetExtIdxCount ();
*/
		HAN_Close (pHanin);
	}
}

