/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeGroup.h						*/
/*		DISCRIPTION		:	Theme Group Skin Control Class		*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.10.22							*/
/****************************************************************/

#if !defined(AFX_THEMEGROUP_H__B174504C_EC0A_4786_AF5C_FE227268B16D__INCLUDED_)
#define AFX_THEMEGROUP_H__B174504C_EC0A_4786_AF5C_FE227268B16D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeGroup.h : header file
//

#include "ThemeToolTip.h"

#define dfBACK_DRAW_TYPE_NONE					0						// 배경 그리기 방식 - 배경 그리지 않음
#define dfBACK_DRAW_TYPE_COLOR					1						// 배경 그리기 방식 - 컬러로 배경 그리기
#define dfBACK_DRAW_TYPE_IMAGE					2						// 배경 그리기 방식 - Image로 배경 그리기

#define dfBACK_IMAGE_FILL_TYPE_BLOCK			0						// 배경 이미지 채우기 방식 - 블럭
#define dfBACK_IMAGE_FILL_TYPE_RESIZE			1						// 배경 이미지 채우기 방식 - 늘이기
#define dfBACK_IMAGE_FILL_TYPE_PATTERN			2						// 배경 이미지 채우기 방식 - 바둑판식
#define dfBACK_IMAGE_FILL_TYPE_ONCE				3						// 배경 이미지 채우기 방식 - 1회

//#define dfCOLOR_TRANSPARENT						RGB(255,0,255)			// 투명을 의미하는 색깔

/////////////////////////////////////////////////////////////////////////////
// CThemeGroup window

class CThemeGroup : public CStatic
{
// Construction
public:
	static struct ST_THEME_GROUP_INFO
	{
		CString		strCaption;				// Caption 문자열
		CRect		rCaption;				// Caption 영역좌표
		UINT		nAlignFormat;			// Caption 정렬방식
		int			nFontSize;				// 폰트 크기
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFont;					// 폰트 컬러
		COLORREF	crShadow;				// 그림자 컬러
		int			nBackDrawType;			// 배경 그리기 방식
		CBitmap		bmpBack;				// 배경 이미지
		int			nBackImageFillType;		// 배경 이미지 채우기 방식
		UINT		nBackImageAlignFormat;	// 배경 이미지 정렬방식
		COLORREF	crBackground;			// 배경 컬러
		COLORREF	crOutlineLT;			// 테두리 컬러(Left-Top)
		COLORREF	crOutlineRB;			// 테두리 컬러(Right-Bottom)
		int			nOutlineThick;			// 테두리 두께
		int			nEdgeRoundValue;		// 모서리 둥근 정도
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

// Attributes
public:
	ST_THEME_GROUP_INFO	m_stThemeInfo;
	CThemeToolTip			m_ToolTip;

// Operations
public:
	CThemeGroup();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeGroup)
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
	void SetWindowText(CString strText);
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	//void SetToolTip(CString strText);
	void SetBackImageAlignFormat(UINT nAlignFormat);
	void SetOutlineColor(COLORREF crOutline);
	void DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea);
	void DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen);
	void SetBackImageFillType(int nImageFillType);
	void SetBackDrawType(int nDrawType);
	void SetOutlineRBColor(COLORREF crOutline);
	void SetOutlineLTColor(COLORREF crOutline);
	void SetOutlineThick(int nThick);
	void SetEdgeRoundValue(int nRoundValue);
	void SetBackColor(COLORREF crBack);
	BOOL SetBackImage(UINT nImageID);
	BOOL SetBackImage(CString strFileName);
	void SetFont(int nFontSize, BOOL bFontBold, COLORREF crFont, COLORREF crShadow, UINT nAlignFormat);
	void SetFontBold(BOOL bBold);
	void SetFontSize(int nFontSize);
	void SetFontColor(COLORREF crFont, COLORREF crShadow);
	void SetShadowColor(COLORREF crShadow);
	void SetFontColor(COLORREF crFont);
	void SetAlignFormat(UINT nAlignFormat);
	void SetCaptionArea(CRect rCaption);
	void SetCaptionText(CString strCaption);
	void DrawCaptionText(CPaintDC *pdc);
	void DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen);
	virtual ~CThemeGroup();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeGroup)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEGROUP_H__B174504C_EC0A_4786_AF5C_FE227268B16D__INCLUDED_)
