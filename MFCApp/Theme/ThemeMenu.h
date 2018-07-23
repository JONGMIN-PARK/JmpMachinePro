/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeMenu.h							*/
/*		DISCRIPTION		:	Theme Menu Skin Control Class		*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.04							*/
/****************************************************************/

#if !defined(AFX_THEMEMENU_H__D263A76A_5F62_4DC9_ABD5_6DD0FF1C10F5__INCLUDED_)
#define AFX_THEMEMENU_H__D263A76A_5F62_4DC9_ABD5_6DD0FF1C10F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeMenu.h : header file
//

#include <afxtempl.h>
#include "BaseMenu.h"
#include "ThemeToolTip.h"

#define dfIDC_SUB_MENU1								5760					// 서브메뉴1 ID
#define dfIDC_SUB_MENU2								dfIDC_SUB_MENU1+1		// 서브메뉴2 ID
#define dfIDC_SUB_MENU3								dfIDC_SUB_MENU1+2		// 서브메뉴3 ID
#define dfIDC_SUB_MENU4								dfIDC_SUB_MENU1+3		// 서브메뉴4 ID
#define dfIDC_SUB_MENU5								dfIDC_SUB_MENU1+4		// 서브메뉴5 ID
#define dfIDC_SUB_MENU6								dfIDC_SUB_MENU1+5		// 서브메뉴6 ID
#define dfIDC_SUB_MENU7								dfIDC_SUB_MENU1+6		// 서브메뉴7 ID
#define dfIDC_SUB_MENU8								dfIDC_SUB_MENU1+7		// 서브메뉴8 ID
#define dfIDC_SUB_MENU9								dfIDC_SUB_MENU1+8		// 서브메뉴9 ID
#define dfIDC_SUB_MENU10							dfIDC_SUB_MENU1+9		// 서브메뉴10 ID

#define dfID_TIMER_DELETE_SUB_MENU					WM_USER+112				// 서브 메뉴 컨트롤 삭제 타이머 ID
#define dfMAX_SUB_MENU_LEVEL						10						// 서브 메뉴 레벨 최대치

#define dfCONTROL_TYPE_NONE							0						// 컨트롤 종류-없음
#define dfCONTROL_TYPE_CHECK						1						// 컨트롤 종류-체크
#define dfCONTROL_TYPE_RADIO						2						// 컨트롤 종류-라디오

/////////////////////////////////////////////////////////////////////////////
// CThemeMenu window

class CThemeMenu : public CWnd
{
// Construction
public:

	static struct ST_MENU_ITEM_PROPERTY
	{
		UINT nMenuID1;
		UINT nMenuID2;
		int nControlType;
		//BOOL bCheck;
	};

	static struct ST_THEME_MENU_INFO
	{
		COLORREF	crNormalItemBack;
		COLORREF	crHoverItemBack;
		COLORREF	crPressedItemBack;
		COLORREF	crFontEnable;
		COLORREF	crFontDisable;
		COLORREF	crBack;
		COLORREF	crOutlineLT;
		COLORREF	crOutlineRB;
		COLORREF	crSeperateUp;
		COLORREF	crSeperateDown;

		int			nItemHeight;
		int			nItemLeftGap;
		int			nItemRightGap;
		int			nSeperateHeight;
		int			nBackThick;
		int			nOutlineThick;
		int			nFontSize;
		int			nFocusMenuLevel;
		short		nSubMenuNumber;

		BOOL		bItemImageSettled;		// 항목 이미지 존재 여부
		BOOL		bSeperateImageSettled;	// 구분선 이미지 존재 여부
		CBitmap		bmpItem;				// 항목 이미지
		CBitmap		bmpSeperate;			// 구분선 이미지
		int			nItemImageCount;		// 항목 이미지 개수

		int			nTransparent;			// 투명도
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

// Attributes
public:
	ST_THEME_MENU_INFO	m_stThemeInfo;

	CArray <ST_MENU_ITEM_PROPERTY, ST_MENU_ITEM_PROPERTY>		m_arMenuItemProperty;

	CMenu			m_menuMain;
	CBaseMenu*		m_pSubMenu[dfMAX_SUB_MENU_LEVEL];

// Operations
public:
	CThemeMenu();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeMenu)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOutlineColor(COLORREF crOutline);
	BOOL GetCheck(UINT nMenuID);
	BOOL SetCheck(UINT nMenuID, BOOL bCheck);
	void SetSeperateDownColor(COLORREF crSeperate);
	void SetSeperateUpColor(COLORREF crSeperate);
	void SetFontColor(COLORREF crFont);
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void SetTransparent(BYTE byteDegree);
	BOOL SetSeperateImage(CString strFileName);
	BOOL SetItemImage(CString strFileName);
	BOOL SetSeperateImage(UINT nImageID);
	BOOL SetItemImage(UINT nImageID);
	void ShowSubMenu(CPoint poMenu);
	void SetSubMenuNumber(int nSubMenuNumber);
	CMenu* GetSubMenuPointer(int nMenuLevel);
	void SetItemImageCount(int nImageCount);
	void SetFontSize(int nFontSize);
	void SetOutlineThick(int nOutlineThick);
	void SetBackThick(int nBackThick);
	void SetSeperateHeight(int nSeperateHeight);
	void SetItemRightGap(int nItemRightGap);
	void SetItemLeftGap(int nItemLeftGap);
	void SetItemHeight(int nItemHeight);
	void SetOutlineRBColor(COLORREF crOutlineRB);
	void SetOutlineLTColor(COLORREF crOutlineLT);
	void SetBackColor(COLORREF crBack);
	void SetFontDisableColor(COLORREF crFontDisable);
	void SetFontEnableColor(COLORREF crFontEnable);
	void SetPressedBackColor(COLORREF crPressedBack);
	void SetHoverBackColor(COLORREF crHoverBack);
	void SetNormalBackColor(COLORREF crNormalBack);
	void SetRadioMenuItem(UINT nIdFirst, UINT nIdLast);
	void SetItemControlState(CMenu *pSubMenu, UINT nMenuID);
	int FindPositionInArrayMenuItem(UINT nMenuID);
	void SetCheckMenuItem(UINT nMenuID);
	CMenu* GetMenu();
	void OnSetFocusSubMenu(int nLevel);
	void OnKillFocusSubMenu(int nLevel);
	void OnMenuItemMouseOver(int nLevel, UINT nMenuID, int nItemNumber, CRect rItem);
	void DeleteSubMenu();
	void OnMenuItemSelect(int nLevel, UINT nMenuID, int nItemNumber);
	void ShowSubMenu(int nLevel, CMenu *pSubMenu, CPoint poMenu);
	void ShowSubMenu(int nIndex, CPoint poMenu);
	void LoadMenu(UINT nIDResource);
	virtual ~CThemeMenu();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeMenu)
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEMENU_H__D263A76A_5F62_4DC9_ABD5_6DD0FF1C10F5__INCLUDED_)
