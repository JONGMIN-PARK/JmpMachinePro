/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeCombo.h						*/
/*		DISCRIPTION		:	Theme Combo Box Skin Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.10.28							*/
/****************************************************************/

#if !defined(AFX_THEMECOMBO_H__06716C4D_7C8C_4045_AAE9_1853DFFE8641__INCLUDED_)
#define AFX_THEMECOMBO_H__06716C4D_7C8C_4045_AAE9_1853DFFE8641__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeCombo.h : header file
//

#include "ThemeButton.h"
#include "ThemeEdit.h"
#include "ThemeListBox.h"

#define dfIDC_EDITBOX_CTRL							1002				// 에디트박스 컨트롤 ID
#define dfIDC_BUTTON_CTRL							1003				// 버튼 컨트롤 ID
#define dfIDC_LISTBOX_DROP_CTRL						1004				// 리스트박스 컨트롤 ID

#define dfID_TIMER_SHOW_LISTBOX						1005				// 리스트박스를 보여주는 타이머

#define dfEDITBOX_HEIGHT							20					// 에디트박스 컨트롤 높이
#define dfBUTTON_WIDTH								21					// 버튼 컨트롤 폭
#define dfLISTBOX_HEIGHT_MIN						49					// 리스트박스 높이 최소값

/////////////////////////////////////////////////////////////////////////////
// CThemeCombo window

class CThemeCombo : public CWnd
{
// Construction
public:
	CThemeCombo();

	typedef struct tagNM_COMBOBOXVIEW { 
		NMHDR hdr; 
		int   iRow; 
		int   iColumn; 
	} NM_COMBOBOXVIEW;

	static struct ST_THEME_COMBOBOX_INFO {
		int			nFontSize;
		BOOL		bFontBold;
		COLORREF	crFontColor;
		int			nOutlineThick;
		COLORREF	crOutlineLT;
		COLORREF	crOutlineRB;
		COLORREF	crEditboxBack;
		COLORREF	crFrame;
		int			nAlignFormat;
		int			nBtnWidth;
		int			nEditboxHeight;
		int			nListBoxHeight;
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	} ;

	typedef struct {
		UINT uIDImage;
		CString strImageFile;
		int nImageCount;
		BOOL bMakeRgn;
	} ST_BTN_CTRL_INFO;

// Attributes
public:
	CThemeListBox*				m_pListbox;					// 리스트박스 컨트롤
	CThemeButton*				m_pBtnDropDown;				// 아래쪽 방향 버튼 컨트롤
	//CEdit*						m_pEditbox;					// 에디트박스 컨트롤
	//CRichEditCtrl*				m_pEditbox;
	CThemeEdit*					m_pEditbox;
	ST_THEME_COMBOBOX_INFO		m_stThemeInfo;
	ST_BTN_CTRL_INFO			m_stBtnCtrlInfo;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeCombo)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetListBoxTransparent(BYTE byteDegree);
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void SetToolTip(CString strText);
	void SetRightGap(int nRightGap);
	void SetLeftGap(int nLeftGap);
	void SetItemHeight(int nItemHeight);
	void SetFontBold(BOOL bFontBold);
	void SetFontSize(int nFontSize);
	void SetSelectItemBackColor(COLORREF crBack);
	void SetSelectItemFontColor(COLORREF crFont);
	void SetListBoxHeight(int nHeight);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetAlignFormat(int nAlignFormat);
	void SetOutlineThick(int nOutlineThick);
	void SetOutlineRBColor(COLORREF crOutlineRB);
	void SetOutlineLTColor(COLORREF crOutlineLT);
	void SetOutlineColor(COLORREF crOutlineLT, COLORREF crOutlineRB);
	void SetOutlineColor(COLORREF crOutline);
	void SetItemFrameColor(COLORREF crFrame);
	void SetNormalItem2BackColor(COLORREF crItem2Back);
	void SetNormalItem1BackColor(COLORREF crItem1Back);
	void SetNormalItemBackColor(COLORREF crItem1, COLORREF crItem2);
	void SetNormalItemBackColor(COLORREF crItemBack);
	void SetFontColor(COLORREF crFont);
	void SetBackColor(COLORREF crBack);
	void ResizeThis();
	CRect GetListboxRect();
	void RepositionControl();
	void OnSelchangeListBox();
	void HideListbox();
	void ShowListbox();
	void OnBtnDropDown();
	void OnKillfocusListBox();
	long OnMsgListboxSelChange(WPARAM wParam, LPARAM lParam);
	void CreateControl(DWORD dwStyle);
	void SetButtonImageBlockCount(int nBlockCount);
	BOOL SetButtonImage(UINT nImageID);
	BOOL SetButtonImage(CString strFileName);
	void CalculateBtnImageInfo();
	void SetSelectItemColor(COLORREF crSelItemFont, COLORREF crSelItemBack);
	void SetFontStyle(COLORREF crFontColor, COLORREF crBackColor, int nFontSize, BOOL bBold, int nAlignFormat);
	void SetFontStyle(COLORREF crFontColor, COLORREF crBackColor, int nFontSize, BOOL bBold);
	void SetFontEditbox();
	void SetOutline(COLORREF crEdgeLeftTop, COLORREF crEdgeRightBottom, int nLineTick, COLORREF crItemDivideLine);
	void SetOutline(COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineThick);
	int SetCurSel(int nIndex);
	void OnDblclkListbox();
	void GetLBText(int nIndex, CString &rString);
	int GetCount();
	int InsertString(int nIndex, LPCTSTR lpszItem);
	int DeleteString(UINT nIndex);
	int FindString(int nStartAfter, LPCTSTR lpszString);
	void GetWindowText(CString &rString);
	int GetCurSel();
	LRESULT SendMessageToParent(int nRow, int nCol, int nMessage);
	void RedrawParentListCtrlArea();
	int AddString(CString strItem);
	CRect GetOutlineRect();
	void SetFocus(BOOL f);
	void SetSel(int nStart, int nEnd);
	void ModifyEditStyle(DWORD remove, DWORD add);
	void SetWindowText(CString strText);
	void SetFont(CFont *pFont);
	void DrawEdgeLine(CPaintDC *pdc);
	virtual ~CThemeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeCombo)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMECOMBO_H__06716C4D_7C8C_4045_AAE9_1853DFFE8641__INCLUDED_)
