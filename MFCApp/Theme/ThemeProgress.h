/********************************************************************/
/*																	*/
/*		FILE NAME		:	ThemeProgress.h							*/
/*		DISCRIPTION		:	Theme Progress Bar Skin Control Class	*/
/*																	*/
/*		PROGRAMMER		:	Jung, Sang-Soo							*/
/*		Mail			:	topofsan@hotmail.com					*/
/*		Home			:	http://rlxk.co.kr						*/
/*		LAST UPDATE		:	2008.10.29								*/
/********************************************************************/

#if !defined(AFX_THEMEPROGRESS_H__A63FF609_3768_4B60_9957_F65612D4EA03__INCLUDED_)
#define AFX_THEMEPROGRESS_H__A63FF609_3768_4B60_9957_F65612D4EA03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeProgress.h : header file
//

#include "ThemeToolTip.h"

//#define dfCOLOR_TRANSPARENT						RGB(255,0,255)			// 투명을 의미하는 색깔

/////////////////////////////////////////////////////////////////////////////
// CThemeProgress window

class CThemeProgress : public CWnd
{
// Construction
public:
	CThemeProgress();

	static struct ST_THEME_PROGRESS_INFO {
		COLORREF	crBarColor1_Up;
		COLORREF	crBarColor1_Down;
		COLORREF	crBarColor1_Outline;
		COLORREF	crBarColor2_Up;
		COLORREF	crBarColor2_Down;
		COLORREF	crBarColor2_Outline;
		double		fBarColorChangePercent;
		BOOL		bGradient;
		
		int			nOutlineThick;
		COLORREF	crOutlineLT;
		COLORREF	crOutlineRB;
		COLORREF	crBack;
		
		int			nMin;
		int			nMax;
		int			nPos;

		BOOL		bDrawText;				// 문자열 표시 여부
		CString		strTextUnit;			// 문자열 단위
		int			nFontSize;				// 폰트 크기
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFont;					// 폰트 컬러
		COLORREF	crShadow;				// 그림자 컬러
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

// Attributes
public:
	ST_THEME_PROGRESS_INFO		m_stThemeInfo;

	CThemeToolTip			m_ToolTip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeProgress)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
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
	void SetShadowColor(COLORREF crShadow);
	void SetFontColor(COLORREF crFont);
	void SetFontBold(BOOL bFontBold);
	void SetFontSize(int nFontSize);
	void SetTextUnit(CString strTextUnit);
	void SetDrawText(BOOL bDrawText);
	void DrawPosText(CPaintDC *pdc);
	void SetGradient(BOOL bGradient);
	void SetRangeMin(int nMin);
	void SetRangeMax(int nMax);
	void SetBackColor(COLORREF crBack);
	void SetOutlineRBColor(COLORREF crOutlineRB);
	void SetOutlineLTColor(COLORREF crOutlineLT);
	void SetOutlineColor(COLORREF crOutline);
	void SetOutlineThick(int nOutlineThick);
	void SetBar2Color(COLORREF crBar2);
	void SetBar1Color(COLORREF crBar1);
	void SetBar2OutlineColor(COLORREF crBar2Outline);
	void SetBar2DownColor(COLORREF crBar2Down);
	void SetBar2UpColor(COLORREF crBar2Up);
	void SetBar1OutlineColor(COLORREF crBar1Outline);
	void SetBar1DownColor(COLORREF crBar1Down);
	void SetBar1UpColor(COLORREF crBar1Up);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetGradientInfo(BOOL bGradient, COLORREF crBegin, COLORREF crEnd);
	void DrawGradientRect(CRect rArea, COLORREF crBegin, COLORREF crEnd, int nDirection, CPaintDC *pdc);
	void DrawBarGradient(CPaintDC *pdc);
	void DrawBar(CPaintDC *pdc);
	void SetColorChangePercent(double fColorChangePercent);
	double GetBarColorChangePercent();
	void SetBarColor(COLORREF crBar1Up, COLORREF crBar1Down, COLORREF crBar1Outline, COLORREF crBar2Up, COLORREF crBar2Down, COLORREF crBar2Outline, double fColorChangePercent);
	void SetBarColor(COLORREF crBar1, COLORREF crBar2, double fColorChangePercent);
	void GetRange(int &nMin, int &nMax);
	int GetPos();
	void SetPos(int nPos);
	void SetRange(int nMin, int nMax);
	void SetBarColor(COLORREF crBar);
	void SetOutline(COLORREF crOutlineLT, COLORREF crOutlineRB, int nOutlineThick, COLORREF crBack);
	void DrawOutline(CPaintDC *pdc);
	virtual ~CThemeProgress();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeProgress)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEPROGRESS_H__A63FF609_3768_4B60_9957_F65612D4EA03__INCLUDED_)
