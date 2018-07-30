/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeEdit.h							*/
/*		DISCRIPTION		:	Theme EditBox Skin Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.10.23							*/
/****************************************************************/

#if !defined(AFX_THEMEEDIT_H__3261FC6F_5682_487D_AAD9_993969019AD7__INCLUDED_)
#define AFX_THEMEEDIT_H__3261FC6F_5682_487D_AAD9_993969019AD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeEdit.h : header file
//

#include "ThemeToolTip.h"
#include "BaseEdit.h"
#include <mutex>

//#define dfCOLOR_TRANSPARENT						RGB(255,0,255)			// 투명을 의미하는 색깔

#define dfIDC_EDITBOX_CTRL						1002					// 에디트박스 컨트롤 ID

/////////////////////////////////////////////////////////////////////////////
// CThemeEdit window

class CThemeEdit : public CWnd
{
// Construction
public:
	static struct ST_THEME_EDIT_INFO
	{
		int			nFontSize;				// 폰트 크기
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFont;					// 폰트 컬러
		COLORREF	crBackground;			// 배경 컬러
		COLORREF	crOutlineLT;			// 테두리 컬러(Left-Top)
		COLORREF	crOutlineRB;			// 테두리 컬러(Right-Bottom)
		int			nOutlineThick;			// 테두리 두께
		double		nMinValue;
		double		nMaxValue;
		BOOL		bIntValue;
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

// Attributes
public:
	ST_THEME_EDIT_INFO		m_stThemeInfo;

	CFont					m_ftEdit;
	//CEdit*					m_pEditbox;
	CBaseEdit*				m_pEditbox;
	HBRUSH					m_brushBk;
	//CThemeToolTip			m_ToolTip;

// Operations
public:
	CThemeEdit();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void SetToolTip(CString strText);
	void SetOutlineColor(COLORREF crOutline);
	void SetOutlineThick(int nThick);
	void SetOutlineRBColor(COLORREF crOutline);
	void SetOutlineLTColor(COLORREF crOutline);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetFont(CFont *pFont);
	void SetFontToEditbox(int nFontSize, BOOL bFontBold);
	void SetFontColorToEditbox(COLORREF crFont);
	void SetBackColorToEditbox(COLORREF crBack);
	void SetFontColor(COLORREF crFont);
	void SetFontBold(BOOL bFontBold);
	void SetFontSize(int nFontSize);
	void SetMinMaxValue( double minValue, double maxValue , BOOL valueIsInteger );
	void SetFocus(BOOL f);
	void SetSel(int nStart, int nEnd);
	void ModifyEditStyle(DWORD remove, DWORD add);
	void GetWindowText(CString &strText);
	void SetWindowText(CString strText);
	void SetBackColor(COLORREF crBack);
	void DrawEdgeLine(CPaintDC *pdc);
	void RepositionControl();
	void OnButtonUpDown( BOOL up );
	virtual ~CThemeEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeEdit)
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMaxtextEditbox();
	afx_msg void OnKillfocusEditbox();
	afx_msg void OnHscrollEditbox();
	afx_msg void OnErrspaceEditbox();
	afx_msg void OnChangeEditbox();
	afx_msg void OnVscrollEditbox();
	afx_msg void OnUpdateEditbox();
	afx_msg void OnSetfocusEditbox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void Save();
	void Load();
	double GetDouble();
	int GetInt();
	CString GetString();

private:
	std::mutex mMtxLock;
	double mNumericValue = 10;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEEDIT_H__3261FC6F_5682_487D_AAD9_993969019AD7__INCLUDED_)
