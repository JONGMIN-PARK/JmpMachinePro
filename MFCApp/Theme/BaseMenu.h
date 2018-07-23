/****************************************************************/
/*																*/
/*		FILE NAME		:	BaseMenu.h							*/
/*		DISCRIPTION		:	Theme Base Menu Control Class		*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.04							*/
/****************************************************************/

#if !defined(AFX_BASEMENU_H__CAC45371_B90A_4E06_B2EF_38DC7E3AFD14__INCLUDED_)
#define AFX_BASEMENU_H__CAC45371_B90A_4E06_B2EF_38DC7E3AFD14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BaseMenu.h : header file
//

#define dfPOPUP_MENU_ITEM_ID						4294967295

#define dfITEM_STATE_NORMAL							0					// 항목 상태 - 일반
#define dfITEM_STATE_MOUSE_OVER						1					// 항목 상태 - 마우스 오버
#define dfITEM_STATE_SELECT							2					// 항목 상태 - 선택

#define dfID_TIMER_VIEW_SUB_MENU					WM_USER+113			// 서브메뉴를 표시할 타이머 ID

// 윈도우 설정 flag
#define WS_EX_LAYERED						0x00080000
#define LWA_COLORKEY						0x00000001
#define LWA_ALPHA							0x00000002

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);
	
/////////////////////////////////////////////////////////////////////////////
// CBaseMenu window

class CBaseMenu : public CWnd
{
// Construction
public:
	CString					m_strMessage;

	COLORREF				m_crNormalItemBack;
	COLORREF				m_crHoverItemBack;
	COLORREF				m_crPressedItemBack;
	COLORREF				m_crFontEnable;
	COLORREF				m_crFontDisable;
	COLORREF				m_crBack;
	COLORREF				m_crOutLineLT;
	COLORREF				m_crOutLineRB;
	COLORREF				m_crSeperateUp;
	COLORREF				m_crSeperateDown;

	int						m_nItemHeight;
	int						m_nItemLeftGap;
	int						m_nItemRightGap;
	int						m_nSeperateHeight;
	int						m_nBackTick;
	int						m_nOutLineTick;
	int						m_nFontSize;

	int						m_nItemNumberMouseOver;				// 마우스 위치한 항목 번호
	int						m_nItemNumberSelected;				// 선택된 항목 번호
	int						m_nItemNumberViewSub;				// 서브메뉴를 표시할 항목 번호

	int						m_nMenuLevel;

	CBitmap*				m_pBmpItem;
	CBitmap*				m_pBmpSeperate;
	BOOL					m_bItemImageSettled;
	BOOL					m_bSeperateImageSettled;
	int						m_nItemImageCount;

	CMenu*					m_pMenuPointer;
	CWnd*					m_pParent;

// Attributes
public:

// Operations
public:
	CBaseMenu(CWnd* pParent);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseMenu)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSeperateImage(CBitmap *pBitmap);
	void SetItemImage(CBitmap *pBitmap);
	BOOL SetTransParent(BYTE byteDegree);
	void ShowItemSubMenu(int nItemNumber);
	CMenu* GetMenuPointer();
	void ShowMenuItemCount();
	int GetItemImageNumberByState(int nState);
	void SetItemImageCount(int nImageCount);
	void DrawItemByImage(CPaintDC *pdc, CRect rArea, int nState);
	void DrawSeperateByImage(CPaintDC *pdc, CRect rArea);
	void ResizeMenuCtrl(CPaintDC *pdc);
	int GetMaxItemTextWidth(CPaintDC *pdc);
	void SendSelectItemMessage(CPoint point);
	void ShowSelectItem(CPoint point);
	void ShowMouseOverItem(CPoint point);
	CRect GetItemRect(int nItemNumber);
	int CalcItemNumberByPosition(CPoint point);
	void DrawSeperate(CPaintDC *pdc, CRect rArea);
	void DrawAllMenuItem(CPaintDC *pdc);
	CRect CalcMenuCtrlSize(CPaintDC *pdc);
	void DrawOutLine(CPaintDC *pdc);
	void DrawBackground(CPaintDC *pdc);
	void SetMenu(int nLevel, CMenu* pSubMenu);
	void DrawText(CPaintDC *pdc, CRect &rArea, int nFontSize, COLORREF crFontColor, CString strItem, BOOL bBold=FALSE);
	virtual ~CBaseMenu();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseMenu)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEMENU_H__CAC45371_B90A_4E06_B2EF_38DC7E3AFD14__INCLUDED_)
