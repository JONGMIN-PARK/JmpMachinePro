/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeRadio.h						*/
/*		DISCRIPTION		:	Theme Radio Btn Skin Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.10.26							*/
/****************************************************************/

#if !defined(AFX_THEMERADIO_H__9DB33982_E414_437C_95CF_4E0BEA081D60__INCLUDED_)
#define AFX_THEMERADIO_H__9DB33982_E414_437C_95CF_4E0BEA081D60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeRadio.h : header file
//

#include "ThemeToolTip.h"

#define dfBACK_DRAW_TYPE_NONE					0						// 배경 그리기 방식 - 배경 그리지 않음
#define dfBACK_DRAW_TYPE_COLOR					1						// 배경 그리기 방식 - 컬러로 배경 그리기
#define dfBACK_DRAW_TYPE_IMAGE					2						// 배경 그리기 방식 - Image로 배경 그리기

#define dfMARK_DRAW_TYPE_NONE					0						// 마크 그리기 방식 - 마크 그리지 않음
#define dfMARK_DRAW_TYPE_COLOR					1						// 마크 그리기 방식 - 컬러로 배경 그리기
#define dfMARK_DRAW_TYPE_IMAGE					2						// 마크 그리기 방식 - Image로 배경 그리기

#define dfBACK_IMAGE_FILL_TYPE_BLOCK			0						// 배경 이미지 채우기 방식 - 블럭
#define dfBACK_IMAGE_FILL_TYPE_RESIZE			1						// 배경 이미지 채우기 방식 - 늘이기
#define dfBACK_IMAGE_FILL_TYPE_PATTERN			2						// 배경 이미지 채우기 방식 - 바둑판식
#define dfBACK_IMAGE_FILL_TYPE_ONCE				3						// 배경 이미지 채우기 방식 - 1회

//#define dfCOLOR_TRANSPARENT						RGB(255,0,255)			// 투명을 의미하는 색깔
#define WM_CXSHADE_RADIO WM_USER+0x100

/////////////////////////////////////////////////////////////////////////////
// CThemeRadio window

class CThemeRadio : public CButton
{
// Construction
public:
	const static enum {
		ST_NORMAL,
		ST_HOVER,
		ST_PRESSED,
		ST_DISABLED,
		
		NUM_OF_STATES,
	};

	static struct ST_THEME_RADIO_INFO
	{
		CString		strCaption;				// Caption 문자열
		CRect		rCaption;				// Caption 영역좌표
		UINT		nAlignFormat;			// Caption 정렬방식
		int			nFontSize;				// 폰트 크기
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFont[NUM_OF_STATES];	// 폰트 컬러
		COLORREF	crShadow[NUM_OF_STATES];			// 그림자 컬러
		int			nBackDrawType;			// 배경 그리기 방식
		CBitmap		bmpBack;				// 배경 이미지
		int			nBackImageFillType;		// 배경 이미지 채우기 방식
		UINT		nBackImageAlignFormat;	// 배경 이미지 정렬방식
		int			nBackImageBlockCount;	// 배경 이미지 블럭 개수
		int			nBackImageStateBlock[NUM_OF_STATES];	// 배경 이미지 상태별 블럭 번호
		COLORREF	crBackground[NUM_OF_STATES];			// 배경 컬러
		COLORREF	crOutlineLT;			// 테두리 컬러(Left-Top)
		COLORREF	crOutlineRB;			// 테두리 컬러(Right-Bottom)
		int			nOutlineThick;			// 테두리 두께
		int			nEdgeRoundValue;		// 모서리 둥근 정도
		int			nBtnState;				// 버튼 상태(Normal, Pressed, Hover, Disabled)
		BOOL		bPressed;				// 마우스 클릭 상태
		BOOL		bHover;					// 마우스 Hover 상태
		BOOL		bCheck;					// Check 상태
		BOOL		bPushLike;				// PushLike 여부
		int			nMarkDrawType;			// 마크 그리기 방식
		COLORREF	crMark;					// 마크 컬러
		CBitmap		bmpMark;				// 마크 이미지
		int			nMarkSize;				// 마크 크기
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

// Attributes
public:
	ST_THEME_RADIO_INFO		m_stThemeInfo;

	CThemeToolTip			m_ToolTip;

// Operations
public:
	CThemeRadio();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeRadio)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetCheck(BOOL bCheck);
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void SetToolTip(CString strText);
	void DrawMarkByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen);
	CRect GetCtrlRectByParent();
	CRect GetMarkArea();
	void DrawMarkByColor(CPaintDC *pdc, CRect rScreen);
	BOOL SetMarkImage(UINT nImageID);
	BOOL SetMarkImage(CString strFileName);
	void SetMarkColor(COLORREF crMark);
	void SetMarkDrawType(int nDrawType);
	void SetPushLike(BOOL bPushLike);
	void SetWindowText(CString strText);
	void SetShadowDisabledColor(COLORREF crShadowDisabled);
	void SetShadowPressedColor(COLORREF crShadowPressed);
	void SetShadowHoverColor(COLORREF crShadowHover);
	void SetFontDisabledColor(COLORREF crFontDisabled);
	void SetFontPressedColor(COLORREF crFontPressed);
	void SetFontHoverColor(COLORREF crFontHover);
	void SetShadowColor(COLORREF crShadowNormal, COLORREF crShadowHover=dfCOLOR_TRANSPARENT, COLORREF crShadowPressed=dfCOLOR_TRANSPARENT, COLORREF crShadowDisabled=dfCOLOR_TRANSPARENT);
	void SetFontColor(COLORREF crFontNormal, COLORREF crFontHover=dfCOLOR_TRANSPARENT, COLORREF crFontPressed=dfCOLOR_TRANSPARENT, COLORREF crFontDisabled=dfCOLOR_TRANSPARENT);
	void SetBackColor(COLORREF crBackNormal, COLORREF crBackHover=dfCOLOR_TRANSPARENT, COLORREF crBackPressed=dfCOLOR_TRANSPARENT, COLORREF crBackDisabled=dfCOLOR_TRANSPARENT);
	void DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen);
	CRect GetImagePartByState(int nState);
	void SetBackImageBlockCount(int nBlock);
	BOOL SetBackImage(UINT nImageID);
	BOOL SetBackImage(CString strFileName);
	void RedrawCtrl();
	void CheckHover(CPoint point);
	void SetFont(int nFontSize, BOOL bFontBold, COLORREF crFont, COLORREF crShadow, UINT nAlignFormat);
	void SetFontAndShadowColor(COLORREF crFont, COLORREF crShadow);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetBackImageAlignFormat(UINT nAlignFormat);
	void SetBackImageFillType(int nImageFillType);
	void SetBackDrawType(int nDrawType);
	void SetOutlineColor(COLORREF crOutline);
	void SetOutlineRBColor(COLORREF crOutline);
	void SetOutlineLTColor(COLORREF crOutline);
	void SetOutlineThick(int nThick);
	void SetEdgeRoundValue(int nRoundValue);
	void SetFontBold(BOOL bBold);
	void SetFontSize(int nFontSize);
	void SetAlignFormat(UINT nAlignFormat);
	void SetCaptionArea(CRect rCaption);
	void SetCaptionText(CString strCaption);
	void DrawCaptionText(CPaintDC *pdc, CRect rScreen);
	void DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen);
	void DrawControlTotal(CPaintDC *pdc);
	virtual ~CThemeRadio();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeRadio)
	afx_msg LRESULT OnRadioInfo(WPARAM, LPARAM);
	afx_msg LRESULT OnBMSetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnBMGetCheck(WPARAM, LPARAM);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMERADIO_H__9DB33982_E414_437C_95CF_4E0BEA081D60__INCLUDED_)
