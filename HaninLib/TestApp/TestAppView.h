// TestAppView.h : interface of the CTestAppView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTAPPVIEW_H__64A638C0_2557_466D_BB0A_B8B934957E93__INCLUDED_)
#define AFX_TESTAPPVIEW_H__64A638C0_2557_466D_BB0A_B8B934957E93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifdef	__cplusplus
extern "C" {
#endif

#include "Haninif.h"

#ifdef	__cplusplus
}
#endif

class CTestAppView : public CEditView
{
protected: // create from serialization only
	CTestAppView();
	DECLARE_DYNCREATE(CTestAppView)

// Attributes
public:
	CTestAppDoc* GetDocument();
	PHANIN		m_pHanin;
	BOOL		m_bKeyIn;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestAppView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestAppView)
	afx_msg void OnUpdateInput(CCmdUI* pCmdUI);
	afx_msg void OnInput();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestAppView.cpp
inline CTestAppDoc* CTestAppView::GetDocument()
   { return (CTestAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTAPPVIEW_H__64A638C0_2557_466D_BB0A_B8B934957E93__INCLUDED_)
