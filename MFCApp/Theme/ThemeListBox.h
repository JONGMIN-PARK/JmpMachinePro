/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeListBox.h						*/
/*		DISCRIPTION		:	Theme List Box Skin Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.10.27							*/
/****************************************************************/

#if !defined(AFX_THEMELISTBOX_H__ECE83142_304B_4534_9082_6FFA522BB635__INCLUDED_)
#define AFX_THEMELISTBOX_H__ECE83142_304B_4534_9082_6FFA522BB635__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeListBox.h : header file
//

#include "ThemeToolTip.h"

#define dfIDC_LISTBOX_CTRL						1001				// 수직 스크롤바 컨트롤 ID
#define dfSCROLLBAR_WIDTH						17					// 스크롤바 컨트롤 폭값

// 윈도우 설정 flag
#define WS_EX_LAYERED						0x00080000
#define LWA_COLORKEY						0x00000001
#define LWA_ALPHA							0x00000002

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

/////////////////////////////////////////////////////////////////////////////
// CThemeListBox window

class CThemeListBox : public CWnd
{
// Construction
public:
	CThemeListBox();

	typedef struct tagNM_LISTBOXVIEW { 
		NMHDR hdr; 
		int   iRow; 
		int   iColumn; 
	} NM_LISTBOXVIEW;

	static struct ST_THEME_LISTBOX_INFO {
		int			nFontSize;				// 폰트 크기
		int			nAlignFormat;			// 정렬 방식
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFontState1;			// 폰트 색깔 - 상태1
		COLORREF	crFontState2;			// 폰트 색깔 - 상태2
		COLORREF	crFontState3;			// 폰트 색깔 - 상태3
		int			nSelItemIndex;			// 선택 항목 번호
		COLORREF	crSelItemFont;			// 선택 항목 글짜 색깔
		COLORREF	crSelItemBack;			// 선택 항목 배경색
		COLORREF	crNormalItemBack[2];	// 일반 항목 배경색
		COLORREF	crFrame;				// 항목 구분선 색깔
		int			nItemHeight;			// 항목 높이
		int			nLeftGap;				// 왼쪽 여백
		int			nRightGap;				// 오른쪽 여백
		int			nOutlineThick;			// 외곽선 두께
		COLORREF	crOutlineLT;			// 외곽선 컬러(Left-Top)
		COLORREF	crOutlineRB;			// 외곽선 컬러(Right-Bottom)
		COLORREF	crBackGround;			// 배경화면 색깔
		BOOL		bVScroll;				// 수직 스크롤바 사용 여부
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};
	
// Attributes
public:
	CListBox*					m_pScrollListBox;
	ST_THEME_LISTBOX_INFO		m_stThemeInfo;

	CThemeToolTip			m_ToolTip;

	BOOL					m_bStylePopUp;

	CWnd*					m_pParent;
	UINT					m_unCtrlID;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeListBox)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL SetTransparent(BYTE byteDegree);
	void SetOutlineColor(COLORREF crOutlineLT, COLORREF crOutlineRB);
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	int GetItemState(int nIndex);
	int SetTopIndex(int nIndex);
	int GetTopIndex();
	int GetItemHeight(int nIndex);
	void SetVScroll(BOOL bVScroll);
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void SetToolTip(CString strText);
	void SetParent(CWnd *pParent);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetOutlineThick(int nOutlineThick);
	void SetRightGap(int nRightGap);
	void SetLeftGap(int nLeftGap);
	void SetNormalItem2BackColor(COLORREF crItem2Back);
	void SetNormalItem1BackColor(COLORREF crItem1Back);
	void SetNormalItemBackColor(COLORREF crItemBack);
	void SetSelectItemBackColor(COLORREF crBack);
	void SetSelectItemFontColor(COLORREF crFont);
	void SetFontBold(BOOL bBold);
	void SetFontState3Color(COLORREF crFontState3);
	void SetFontState2Color(COLORREF crFontState2);
	void SetFontState1Color(COLORREF crFontState1);
	void SetFontColor(COLORREF crFont);
	void SetOutlineRBColor(COLORREF crOutlineRB);
	void SetOutlineLTColor(COLORREF crOutlineLT);
	void SetOutlineColor(COLORREF crOutline);
	void OnSelcancelListBox();
	void OnErrspaceListBox();
	void SetItemState(int nIndex, int nState);
	int AddString(CString strItem, int nState);
	void SetLeftRightGap(int nLeft, int nRight);
	void SetItemHeight(int nItemHeight);
	int SelectItemByMousePos(CPoint poMouse);
	void SetNormalItemColor(COLORREF crFontNormal, COLORREF crFontWarning, COLORREF crFontError, COLORREF crItemBack1, COLORREF crItemBack2);
	void SetFontColor(COLORREF crFontState1, COLORREF crFontState2, COLORREF crFontState3);
	CRect GetItemArea(int nIndex);
	void SetNormalItemBackColor(COLORREF crItem1, COLORREF crItem2);
	void SetOutline(COLORREF crEdgeLeftTop, COLORREF crEdgeRightBottom, int nLineThick, COLORREF crFrame, COLORREF crBackGround);
	void SetSelectItemColor(COLORREF crSelItemFont, COLORREF crSelItemBack);
	void SetOutline(long crEdgeLeftTop, long crEdgeRightBottom, int nLineThick);
	void SetBackColor(COLORREF crBack);
	void SetItemFrameColor(COLORREF crFrame);
	int AddString(CString strItem);
	void DrawItemList(CPaintDC *pdc);
	LRESULT SendMessageToParent(long nRow, long nCol, int nMessage);
	CRect GetItemListArea();
	BOOL IsItemListOverPage();
	void SetCheckScrollBarShow();
	int InsertString(int nIndex, LPCTSTR lpszItem);
	int DeleteString(UINT nIndex);
	void SetAlignFormat(int nAlignFormat);
	void SetFontColorState1(COLORREF crFontState1);
	void SetFontSize(int nFontSize);
	void OnDblClickItem();
	void OnSelItemChange();
	int GetCurSel();
	int SelectString(int nStart, LPCTSTR lpszBuffer);
	void GetText(int nIndex, CString &rString);
	void GetText(int nIndex, LPTSTR lpszBuffer );
	int GetCount();
	int SetCurSel(int nIndex);
	void DrawOutline(CPaintDC *pdc);
	void RepositionControl();
	void CreateControl();
	virtual ~CThemeListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeListBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMELISTBOX_H__ECE83142_304B_4534_9082_6FFA522BB635__INCLUDED_)
