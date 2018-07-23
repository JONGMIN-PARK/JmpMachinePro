/****************************************************************/
/*																*/
/*		FILE NAME		:	DlgResizeWnd.h						*/
/*		DISCRIPTION		:	Theme Dialog Resize Frame Class		*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.21							*/
/****************************************************************/

#if !defined(AFX_DLGRESIZEWND_H__5547AA80_AAD0_4C27_BE21_FB2FE9C6B2EB__INCLUDED_)
#define AFX_DLGRESIZEWND_H__5547AA80_AAD0_4C27_BE21_FB2FE9C6B2EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgResizeWnd.h : header file
//

// 윈도우 설정 flag
#define WS_EX_LAYERED						0x00080000
#define LWA_COLORKEY						0x00000001
#define LWA_ALPHA							0x00000002

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

#define dfRESIZE_DIRECTION_NONE					0						// Resize 방향 - 없음
#define dfRESIZE_DIRECTION_LEFT					1						// Resize 방향 - 왼쪽
#define dfRESIZE_DIRECTION_TOP					2						// Resize 방향 - 위쪽
#define dfRESIZE_DIRECTION_RIGHT				3						// Resize 방향 - 오른쪽
#define dfRESIZE_DIRECTION_BOTTOM				4						// Resize 방향 - 아래쪽
#define dfRESIZE_DIRECTION_LEFT_TOP				5						// Resize 방향 - 왼쪽위
#define dfRESIZE_DIRECTION_RIGHT_TOP			6						// Resize 방향 - 오른쪽위
#define dfRESIZE_DIRECTION_LEFT_BOTTOM			7						// Resize 방향 - 왼쪽아래
#define dfRESIZE_DIRECTION_RIGHT_BOTTOM			8						// Resize 방향 - 오른쪽아래

/////////////////////////////////////////////////////////////////////////////
// CDlgResizeWnd window

class CDlgResizeWnd : public CWnd
{
// Construction
public:
	static struct ST_DLG_RESIZE_WND_INFO
	{
		CPoint		poMouse;
		int			nResizingDirection;		// Resize 방향
		int			nEdgeRange;				// 가장자리 범위
	};
	
	CDlgResizeWnd();

// Attributes
public:
	ST_DLG_RESIZE_WND_INFO	m_stThemeInfo;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgResizeWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	CRect OnTargetWindowLButtonUp(CWnd *pWnd, CPoint point);
	int IsMouseInEdgeRange(CWnd *pWnd, CPoint poMouse);
	CRect GetNewAreaByPoint(CWnd *pWnd, CPoint point);
	void OnTargetWindowLButtonDown(CWnd *pWnd, CPoint point);
	void OnTargetWindowMouseDrag(CWnd *pWnd, CPoint point);
	BOOL SetTransparentBack(COLORREF crTrans);
	void SetSizeToParent();
	void SetSizeToWnd(CWnd *pWnd);
	void DrawRectLine(CRect rLine, COLORREF crLeft, COLORREF crTop, COLORREF crRight, COLORREF crBottom, CPaintDC *pdc, int nPenStyle=PS_SOLID);
	void DrawResizeFrame(CPaintDC *pdc);
	virtual ~CDlgResizeWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDlgResizeWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGRESIZEWND_H__5547AA80_AAD0_4C27_BE21_FB2FE9C6B2EB__INCLUDED_)
