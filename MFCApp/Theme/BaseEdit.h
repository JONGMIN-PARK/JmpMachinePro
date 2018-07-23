/****************************************************************/
/*																*/
/*		FILE NAME		:	BaseEdit.h							*/
/*		DISCRIPTION		:	Theme Base EditBox Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.07							*/
/****************************************************************/

#if !defined(AFX_BASEEDIT_H__27228245_E8EC_4D22_A580_87E87947EFC3__INCLUDED_)
#define AFX_BASEEDIT_H__27228245_E8EC_4D22_A580_87E87947EFC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseEdit.h : header file
//

#include "ThemeToolTip.h"

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit window

class CBaseEdit : public CEdit
{
// Construction
public:	
	// Used for Get/SetItem calls.
	typedef struct _GV_ITEM { 
		int      row,col;     // Row and Column of item
		UINT     mask;        // Mask for use in getting/setting cell data
		UINT     state;       // cell state (focus/hilighted etc)
		UINT     nFormat;     // Format of cell
		CString  szText;      // Text in cell
		int      iImage;      // index of the list view item’s icon
		COLORREF crBkClr;     // Background colour (or CLR_DEFAULT)
		COLORREF crFgClr;     // Forground colour (or CLR_DEFAULT)
		LPARAM   lParam;      // 32-bit value to associate with item 
		LOGFONT  lfFont;      // Cell font
	} GV_ITEM; 

	typedef struct tagGV_DISPINFO { 
		NMHDR   hdr; 
		GV_ITEM item; 
	} GV_DISPINFO;

	CBaseEdit();
    CBaseEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                 int nRow, int nColumn, CString sInitText, UINT nFirstChar);

// Attributes
public:
	BOOL					m_bInPlace;
	CThemeToolTip			m_ToolTip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void EndEdit();
	void SetToolTip(CString strText);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	virtual ~CBaseEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
    int     m_nRow;
    int     m_nColumn;
    CString m_sInitText;
    UINT    m_nLastChar;
    BOOL    m_bExitOnArrows;
    CRect   m_Rect;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEEDIT_H__27228245_E8EC_4D22_A580_87E87947EFC3__INCLUDED_)
