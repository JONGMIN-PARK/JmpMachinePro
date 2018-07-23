/********************************************************************/
/*																	*/
/*		FILE NAME		:	HeaderCtrl.h							*/
/*		DISCRIPTION		:	Theme Header Ctrl Skin Control Class	*/
/*																	*/
/*		PROGRAMMER		:	Jung, Sang-Soo							*/
/*		Mail			:	topofsan@hotmail.com					*/
/*		Home			:	http://rlxk.co.kr						*/
/*		LAST UPDATE		:	2008.11.06								*/
/********************************************************************/

#if !defined(AFX_THEMEHEADERCTRL_H__6B2B4F90_E48A_4AA2_9D7B_231788234F3A__INCLUDED_)
#define AFX_THEMEHEADERCTRL_H__6B2B4F90_E48A_4AA2_9D7B_231788234F3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeHeaderCtrl.h : header file
//

#ifndef __AFXTEMPL_H
#include <afxtempl.h>
#endif

#define dfSORT_TYPE_NONE								0				// 정렬 방식 - 정렬하지 않음
#define dfSORT_TYPE_ASENDING							1				// 정렬 방식 - 오름차순
#define dfSORT_TYPE_DESENDING							2				// 정렬 방식 - 내림차순
#define dfSORT_TYPE_TOGGLE_ASEND						3				// 정렬 방식 - 토글-오름차순
#define dfSORT_TYPE_TOGGLE_DESEND						4				// 정렬 방식 - 토글-내림차순

/////////////////////////////////////////////////////////////////////////////
// CHeadCtrl window

class CHeadCtrl : public CHeaderCtrl
{
// Construction
public:
	static struct _item_info{
		int nOriginNo;
		int nNewNo;
		CString strItemText;
	};

	struct HEADCELL
	{
		DWORD   state;
		CString string;
	};

	class CHeadCell : public HEADCELL
	{
	public:
		CHeadCell()
		{
			state=0;
			string="";
		}
		CHeadCell(HEADCELL& hcell)
		{
			*this=hcell;
		}
		CHeadCell operator=(HEADCELL& hcell)
		{
			state=hcell.state;
			string=hcell.string;
			return *this;
		}
	};

	static struct ST_THEME_HEADER_CTRL_INFO
	{
		// Sort
		UINT		unSortState;						// 정렬 상태
		int			nRecentSortColumn;				// 최근 정렬 기준 컬럼

		// Check All
		BOOL		bCheckAll;
		BOOL		bUncheckAll;
		BOOL		bCheckBoxOn;
		CRect		RectCheck;
		int			nCheckGapLeft;
		CBitmap*	pBmpCheckBox;				// 체크박스 이미지
		BOOL		bCheckBoxImageSettled;	// 체크박스 이미지 존재여부

		COLORREF	crBack;
		COLORREF	crFrame;
		COLORREF	crText;

		int			nFontSize;
		int			count;
	};

	CHeadCtrl();

// Attributes
public:
	ST_THEME_HEADER_CTRL_INFO	m_stThemeInfo;

	typedef CArray<CHeadCell, CHeadCell&> CHeadItem;
	typedef CArray<CHeadItem, CHeadItem&> CHeadInfo;
	CHeadInfo	m_Info;

	CArray <_item_info, _item_info> *m_pArrItemList;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeadCtrl)
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawCheckBoxByColor();
	void DrawCheckBoxByImage();
	void SetCheckBoxImage(CBitmap *pBitmap);
	void SetCheckBoxOn(BOOL bFlag);
	void InitSortState(int nSelCol);
	void MoveItemTextBySelColumn(int nSelCol);
	void SortArrayItemList();
	void SetSortType(UINT nSortType);
	BOOL ChangeTwoItems(int nItem1, int nItem2);
	BOOL IsNeedSort(CString strText1, CString strText2, UINT unSortType);
	int GetColNumByPoint(CPoint point);
	void SetFontSize(int nFontSize);
	void SetTextColor(COLORREF crText);
	void SetBackColor(COLORREF crBack);
	void SetFrameColor(COLORREF crFrame);
	void SetColor(COLORREF crFrame, COLORREF crBack, COLORREF crText);
	void SetCheckGapLeft(int nCheckGap);
	BOOL IsChangedColumnWidth();
	void DrawCheckBox();
	BOOL IsDraw(CRect rect);
	BOOL GetTextRect(CDC* pDC, CString strText, CRect& rect);
	UINT GetItemState(int nItem, int nSubItem, int& down, int& right);
	BOOL SetItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem);
	BOOL GetRect(int nItem, int nSubItem, CRect& rect);
	CString GetItemText(int nItem, int nSubItem);
	BOOL SetItemText(int nItem, int nSubItem, LPCSTR lpcstr);
	void InsertColumn(int nCol);
	void DeleteColumn(int nCol);
	void SetCount(int nCount, CFont* pFont);
	int GetHeadHeight();
	virtual ~CHeadCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHeadCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEHEADERCTRL_H__6B2B4F90_E48A_4AA2_9D7B_231788234F3A__INCLUDED_)
