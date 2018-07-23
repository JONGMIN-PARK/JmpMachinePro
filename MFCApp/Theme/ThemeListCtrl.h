/********************************************************************/
/*																	*/
/*		FILE NAME		:	ThemeListCtrl.h							*/
/*		DISCRIPTION		:	Theme List Ctrl Skin Control Class		*/
/*																	*/
/*		PROGRAMMER		:	Jung, Sang-Soo							*/
/*		Mail			:	topofsan@hotmail.com					*/
/*		Home			:	http://rlxk.co.kr						*/
/*		LAST UPDATE		:	2008.11.06								*/
/********************************************************************/

#if !defined(AFX_THEMELISTCTRL_H__0675F75D_9AAF_4DF5_97E1_C2DA78388F89__INCLUDED_)
#define AFX_THEMELISTCTRL_H__0675F75D_9AAF_4DF5_97E1_C2DA78388F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeListCtrl.h : header file
//

#include <afxtempl.h>
#include "HeadCtrl.h"
#include "ThemeToolTip.h"

#define WM_LIST_BTN_CLICK													WM_USER + 113	// 버튼 클릭 메시지
#define WM_LIST_ITEM_CLICK													WM_USER + 114	// 항목 클릭 메시지
#define WM_HEADER_CLICK														WM_USER + 115	// 헤더 클릭 메시지
#define dfDATA_FILED_CNT													8
#define dfIDC_EDIT_INPUT													1009
#define dfIDC_COB_INPUT														1010

#define dfFONT_NAME_COMMON													TEXT("돋움")

#define dfCONTROL_TYPE_NONE													0
#define dfCONTROL_TYPE_EDITBOX												1
#define dfCONTROL_TYPE_COMBOBOX												2
#define dfCONTROL_TYPE_BUTTON												3

/////////////////////////////////////////////////////////////////////////////
// CThemeListCtrl window

class CThemeListCtrl : public CListCtrl
{
// Construction
public:
	static struct ST_COLUMN_INFO{
		int			nControlType;
		CString		strComboItemList;
	};

	static struct ST_COLUMN_COLOR_INFO
	{
		int			nRow;
		int			nColumn;

		COLORREF	crColorText;
		COLORREF	crColorBack;
	};

	static struct ST_ITEM_COLOR_INFO
	{
		int			nRow;
		COLORREF	crColor;
	};

	static struct ST_THEME_LIST_CTRL_INFO
	{
		int			nSelectItem;
		int			nSelectCol;

		HBRUSH		brushBk;

		BOOL		bEditDblClk;			// 마우스 더블클릭으로 편집 여부

		// Check Box
		BOOL		bCheckBoxOn;			// 체크박스 사용 여부
		BOOL		bCheckBoxImageSettled;	// 체크박스 이미지 존재 여부
		CBitmap		bmpCheckBox;			// 체크박스 이미지
		BOOL		bHeadCheckBoxOn;		// 헤더 체크박스 사용 여부
		
		int			nItemHeight;
		CFont		fntEdit;
		CFont		ftItemHeight;
		int			nFontSize;
		COLORREF	crSelectItemBack;
		COLORREF	crSelectItemText;
		COLORREF	crNormalItemBack[2];
		COLORREF	crNormalItemText[2];
		COLORREF	crBackGround;
		COLORREF	crFrame;
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};
	
	CThemeListCtrl();

// Attributes
public:
	ST_THEME_LIST_CTRL_INFO	m_stThemeInfo;

	// EDIT
	CEdit*			m_pEditItem;

	//Combo
	CComboBox*		m_pComboItem;

	//Button
	CPoint			m_poClickBtn;					// 현재 클릭된 버튼 위치

	CArray <ST_COLUMN_INFO, ST_COLUMN_INFO> m_arrColInfo;
	CUIntArray		m_UIntArrCheckBox;
	CPtrArray		m_PtrArrColumnColor;
	CPtrArray		m_PtrArrItemColor;

	CHeadCtrl	m_HeaderCtrl;

	CThemeToolTip	m_ToolTip;						// 툴팁

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle = WS_TABSTOP|WS_CHILD|WS_VISIBLE|WS_BORDER);
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetItemFrameColor(COLORREF crFrame);
	void SetFontColor(COLORREF crFont);
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	int GetItemHeight() { return m_stThemeInfo.nItemHeight; }
	void SetNormalItemFontColor(COLORREF crFont);
	void SetNormalItemBackColor(COLORREF crBack);
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void SetColumnComboBox(int nCol, CString strComboItemList);
	void SetColumnButton(int nCol);
	void SetColumnEditBox(int nCol);
	void DeleteToolTip();
	void SetToolTip(CString strText);
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetHeadCheckBox();
	void SetHeadCheckBoxOn(BOOL bFlag);
	void DrawCheckBoxByColor(CDC *pDC, CRect RectCheck, BOOL bCheck);
	void DrawCheckBoxByImage(CDC *pDC, CRect RectCheck, BOOL bCheck);
	BOOL SetCheckBoxImage(CString strFileName);
	BOOL SetCheckBoxImage(UINT nImageID);
	void SetCheckBoxOn(BOOL bFlag);
	void OnHeaderColumnClick(CPoint point);
	LRESULT SendMessageToParent(UINT nMsgId, int nItem, int nSubItem);
	int GetColumnCount();
	CString GetSelfControlComboItemList(int nCol);
	int GetSelfControlType(int nCol);
	void ShowButtonUnclicked();
	void ShowButtonClicked(CPoint point);
	void DrawButtonEdgeRect(CPaintDC *pDC, int nItem, int nSubItem, CRect rCell);
	void SetEditDblClk(BOOL bDblClk);
	void ShowSelfControl(CPoint point);
	void HideSelfControl();
	int FindRowIdx(CPoint point);
	void SetComboItemList(CString strItemList);
	int GetColumnWidth(int nCol);
	void CreateCtrlFont(int nFontSize);
	void ShowSelfControlAtCell(int nRow, int nCol);
	int FindColIdx(CPoint point);
	void SetSelfControlType(int nCol, int nControlType, CString strComboItemList = TEXT(""));
	void SetSortType(UINT nSortType);
	void CreateBtn(BOOL bBtnDel);
	void OnKillfocusCobInput();
	void OnSelchangeCobInput();
	void MoveListCombo(int nItem, int nSubItem);
	void CreateListCombo(CRect* rcBound, CWnd* pParent, LPCTSTR pszDefault = NULL);
	void OnKillfocusEditInput();
	void MoveListEdit(int nItem, int nSubItem);
	void CreateListEdit(CRect* rcBound, CWnd* pParent, LPCTSTR pszDefault = NULL);
	void CreateCtl();
	void SetItemHeight(int nItemHeight);
	void CreateItemHeightFont(int nItemHeight);
	BOOL SetCheck(int nItem, BOOL bCheck);
	BOOL GetCheck(int nItem);
	void SetFontSize(int nFontSize);
	void SetHeadFontColor(COLORREF crFont);
	void SetHeadBackColor(COLORREF crBack);
	void SetHeadFrameColor(COLORREF crFrame);
	void SetBackColor(COLORREF crBack);
	void SetFrameColor(COLORREF crFrame);
	void SetNormalItem2BackColor(COLORREF crBack);
	void SetNormalItem2FontColor(COLORREF crFont);
	void SetNormalItem1BackColor(COLORREF crBack);
	void SetNormalItem1FontColor(COLORREF crFont);
	void SetSelectItemBackColor(COLORREF crBack);
	void SetSelectItemFontColor(COLORREF crFont);
	void SetHeadColor(COLORREF crFrame, COLORREF crBack, COLORREF crText);
	BOOL DeleteColumn( int nCol );
	int InsertColumn(int nCol, LPCWSTR lpstTxt, int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1);
	void SetHeadItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem);
	void SetHeadItemText(int nItem, int nSubItem, LPCSTR lpstTxt);
	BOOL GetSafeHead();
	int FindArrIdx(int nItem);
	int FindRowIdx();
	void SetHeadItemCount(int nCount);
	BOOL GetItemColor(int nRow, COLORREF& rcrColor);
	void SetItemColor(int nRow, COLORREF crColor);
	void SetColumnColor(int nRow, int nColumn, COLORREF crColorText, COLORREF crColorBack /*= RGB(0,0,0)*/);
	BOOL GetSubItemColor(int nRow, int nColumn, COLORREF& rcrColorText, COLORREF& rcrColorBack);
	void DrawCheckBox(CDC* pDC, CRect RectCheck, int nItem);
	virtual ~CThemeListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMELISTCTRL_H__0675F75D_9AAF_4DF5_97E1_C2DA78388F89__INCLUDED_)
