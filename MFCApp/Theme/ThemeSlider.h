/********************************************************************/
/*																	*/
/*		FILE NAME		:	ThemeSlider.h							*/
/*		DISCRIPTION		:	Theme Slider Skin Control Class			*/
/*																	*/
/*		PROGRAMMER		:	Jung, Sang-Soo							*/
/*		Mail			:	topofsan@hotmail.com					*/
/*		Home			:	http://rlxk.co.kr						*/
/*		LAST UPDATE		:	2008.10.30								*/
/********************************************************************/

#if !defined(AFX_THEMESLIDER_H__7E39F9DC_378E_4FAB_AA95_2116C0A801B8__INCLUDED_)
#define AFX_THEMESLIDER_H__7E39F9DC_378E_4FAB_AA95_2116C0A801B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeSlider.h : header file
//

#include "ThemeButton.h"
#include "ThemeToolTip.h"

#define dfBACK_DRAW_TYPE_NONE						0					// 배경 그리기 방식 - 배경 그리지 않음
#define dfBACK_DRAW_TYPE_COLOR						1					// 배경 그리기 방식 - 컬러로 배경 그리기
#define dfBACK_DRAW_TYPE_IMAGE						2					// 배경 그리기 방식 - Image로 배경 그리기

#define dfBACK_IMAGE_FILL_TYPE_BLOCK				0					// 배경 이미지 채우기 방식 - 블럭
#define dfBACK_IMAGE_FILL_TYPE_RESIZE				1					// 배경 이미지 채우기 방식 - 늘이기
#define dfBACK_IMAGE_FILL_TYPE_PATTERN				2					// 배경 이미지 채우기 방식 - 바둑판식
#define dfBACK_IMAGE_FILL_TYPE_ONCE					3						// 배경 이미지 채우기 방식 - 1회

//#define dfCOLOR_TRANSPARENT							RGB(255,0,255)		// 투명을 의미하는 색깔

#define dfIDC_BUTTON_CTRL							1003				// 버튼 컨트롤 ID

#define dfWM_SLIDER_MOUSE_MOVE						WM_USER+114			// 슬라이더 컨트롤 마우스 이동 메세지

/////////////////////////////////////////////////////////////////////////////
// CThemeSlider window

class CThemeSlider : public CWnd
{
// Construction
public:
	static struct ST_THEME_SLIDER_INFO
	{
		int			nBackDrawType;			// 배경 그리기 방식
		CBitmap		bmpBack;				// 배경 이미지
		int			nBackImageFillType;		// 배경 이미지 채우기 방식
		UINT		nBackImageAlignFormat;	// 배경 이미지 정렬방식
		COLORREF	crBackground;			// 배경 컬러
		COLORREF	crOutlineLT;			// 테두리 컬러(Left-Top)
		COLORREF	crOutlineRB;			// 테두리 컬러(Right-Bottom)
		int			nOutlineThick;			// 테두리 두께
		int			nEdgeRoundValue;		// 모서리 둥근 정도
		BOOL		bVertical;				// 방향-수직 여부

		int			nMin;					// 범위-최소값
		int			nMax;					// 범위-최대값
		int			nPos;					// 현재 설정값
		int			nPageUnit;				// 스크롤 페이지 단위
		
		BOOL		bAutoToolTip;			// 자동 툴팁
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

	static struct ST_BTN_CTRL_INFO{
		UINT		uIDImage;
		CString		strImageFile;
		int			nImageCount;
		BOOL		bMakeRgn;
		int			nWidth;
		int			nHeight;
		BOOL		bLButtonDown;
	};

// Attributes
public:
	ST_THEME_SLIDER_INFO		m_stThemeInfo;
	ST_BTN_CTRL_INFO			m_stBtnBarInfo;

	CThemeButton*				m_pBtnBar;				// Bar 버튼 컨트롤

	CThemeToolTip				m_ToolTip;

// Operations
public:
	CThemeSlider();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeSlider)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitThemeVariable();
	CRect GetInitPos()						{ return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos)			{ m_stThemeInfo.rInitPos = rInitPos; }
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	void ProcessMouseMove(CPoint point);
	// 페이지 단위 설정
	void SetPageUnit(int nPageUnit)			{ m_stThemeInfo.nPageUnit = nPageUnit; }
	int GetPageUnit()						{ return m_stThemeInfo.nPageUnit; }
	LRESULT OnBtnMouseMove(WPARAM wParam, LPARAM lParam);
	LRESULT OnBtnLButtonUp(WPARAM wParam, LPARAM lParam);
	LRESULT OnBtnLButtonDown(WPARAM wParam, LPARAM lParam);
	void SetToolTip(CString strText);
	int GetPosValueByMouseV(CPoint point);
	CRect GetBarPositionByMouseV(CPoint point);
	int GetBarPositionV(int nPosValue);
	CRect GetBarRectV(int nPosValue);
	void SetVertical(BOOL bVertical);
	void SetToolTipByMouse(CPoint point);
	int GetPosValueByMouse(CPoint point);
	void SetAutoToolTip(BOOL bAutoToolTip);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void ProcessLButtonUp(CPoint point);
	void GetRangeMax(int &nMax);
	void GetRangeMin(int &nMin);
	void GetRange(int &nMin, int &nMax);
	void SetRangeMax(int nMax);
	void SetRangeMin(int nMin);
	void SetRange(int nMin, int nMax);
	void SetPos(int nPos);
	int GetPos();
	LRESULT SendMessageToParent(int nPos, int nMessage);
	void ProcessLButtonDown(CPoint point);
	int GetPosValueByMouseH(CPoint point);
	CRect GetBarPositionByMouseH(CPoint point);
	void CreateControl();
	CRect GetBarRectH(int nPosValue);
	int GetBarPositionH(int nPosValue);
	void RepositionControl();
	void SetBarImageBlockCount(int nBlockCount);
	BOOL SetBarImage(UINT nImageID);
	BOOL SetBarImage(CString strFileName);
	void CalculateBtnImageInfo();
	void SetOutlineThick(int nThick);
	void SetOutlineRBColor(COLORREF crOutline);
	void SetOutlineLTColor(COLORREF crOutline);
	void SetOutlineColor(COLORREF crOutline);
	void SetEdgeRoundValue(int nRoundValue);
	void SetBackImageFillType(int nImageFillType);
	void SetBackImageAlignFormat(UINT nAlignFormat);
	BOOL SetBackImage(CString strFileName);
	BOOL SetBackImage(UINT nImageID);
	void SetBackDrawType(int nDrawType);
	void SetBackColor(COLORREF crBack);
	void RedrawControl();
	void DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea);
	void DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen);
	void DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen);
	virtual ~CThemeSlider();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemeSlider)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMESLIDER_H__7E39F9DC_378E_4FAB_AA95_2116C0A801B8__INCLUDED_)
