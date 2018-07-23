/****************************************************************/
/*																*/
/*		FILE NAME		:	BaseCombo.h							*/
/*		DISCRIPTION		:	Theme Base ComboBox Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.07							*/
/****************************************************************/

#if !defined(AFX_BASECOMBO_H__F574922A_88B2_428E_9E90_0505A720D590__INCLUDED_)
#define AFX_BASECOMBO_H__F574922A_88B2_428E_9E90_0505A720D590__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseCombo.h : header file
//

#define IDC_COMBOEDIT 1001

/////////////////////////////////////////////////////////////////////////////
// CComboEdit window

class CComboEdit : public CEdit
{
// Construction
public:
	CComboEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CComboEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CComboEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CComboEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CBaseCombo window

class CBaseCombo : public CComboBox
{
    friend class CComboEdit;

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

	CBaseCombo(CWnd* pParent,         // parent
                 CRect& rect,           // dimensions & location
                 DWORD dwStyle,         // window/combobox style
                 UINT nID,              // control ID
                 int nRow, int nColumn, // row and column
				 CStringArray& Items,   // Items in list
                 CString sInitText,     // initial selection
				 UINT nFirstChar);      // first character to pass to control
	
	CBaseCombo();

// Attributes
public:
   CComboEdit m_edit;  // subclassed edit control

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseCombo)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseCombo();
protected:
    void EndEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseCombo)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	int		m_nNumLines;
	CString m_sInitText;
	int		m_nRow;
	int		m_nCol;
 	UINT    m_nLastChar; 
	BOOL	m_bExitOnArrows; 
	BOOL	m_bFirstEnter;
	CWnd	*m_pParent;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASECOMBO_H__F574922A_88B2_428E_9E90_0505A720D590__INCLUDED_)
