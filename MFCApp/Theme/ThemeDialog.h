/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemeDialog.h						*/
/*		DISCRIPTION		:	Theme Dialog Skin Class				*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.21							*/
/****************************************************************/

#if !defined(AFX_THEMEDIALOG_H__7AA787AD_1790_4821_8AB2_C9CE6867D06F__INCLUDED_)
#define AFX_THEMEDIALOG_H__7AA787AD_1790_4821_8AB2_C9CE6867D06F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemeDialog.h : header file
//

#include <afxtempl.h>
#include "ThemeToolTip.h"
#include "ThemeStatic.h"
#include "ThemeGroup.h"
#include "ThemeButton.h"
#include "ThemeCheck.h"
#include "ThemeRadio.h"
#include "ThemeEdit.h"
#include "ThemeListBox.h"
#include "ThemeCombo.h"
#include "ThemeProgress.h"
#include "ThemeSlider.h"
#include "ThemeScroll.h"
#include "ThemePicture.h"
#include "ThemeMenu.h"
#include "ThemeListCtrl.h"
#include "ThemeGrid.h"
#include "DlgResizeWnd.h"

//Macro Defines
#define M_DELETE(p)								{ if(p) { delete p;	p = NULL; } }
#define M_A_DELETE(p)							{ if(p) { delete [] p;	p = NULL; } }
#define M_W_DELETE(p)							{ if(p) { p->DestroyWindow(); delete p;	p = NULL; } }
#define M_N_DELETE(p)							{ if(p) { p->DestroyWindow(); p = NULL; } }

// 윈도우 설정 flag
#define WS_EX_LAYERED						0x00080000
#define LWA_COLORKEY						0x00000001
#define LWA_ALPHA							0x00000002

typedef BOOL(WINAPI *SLWA)(HWND, COLORREF, BYTE, DWORD);

#define dfBACK_DRAW_TYPE_NONE					0						// 배경 그리기 방식 - 배경 그리지 않음
#define dfBACK_DRAW_TYPE_COLOR					1						// 배경 그리기 방식 - 컬러로 배경 그리기
#define dfBACK_DRAW_TYPE_IMAGE					2						// 배경 그리기 방식 - Image로 배경 그리기

#define dfBACK_IMAGE_FILL_TYPE_BLOCK			0						// 배경 이미지 채우기 방식 - 블럭
#define dfBACK_IMAGE_FILL_TYPE_RESIZE			1						// 배경 이미지 채우기 방식 - 늘이기
#define dfBACK_IMAGE_FILL_TYPE_PATTERN			2						// 배경 이미지 채우기 방식 - 바둑판식
#define dfBACK_IMAGE_FILL_TYPE_ONCE				3						// 배경 이미지 채우기 방식 - 1회

#define dfRESIZE_DIRECTION_NONE					0						// Resize 방향 - 없음
#define dfRESIZE_DIRECTION_LEFT					1						// Resize 방향 - 왼쪽
#define dfRESIZE_DIRECTION_TOP					2						// Resize 방향 - 위쪽
#define dfRESIZE_DIRECTION_RIGHT				3						// Resize 방향 - 오른쪽
#define dfRESIZE_DIRECTION_BOTTOM				4						// Resize 방향 - 아래쪽
#define dfRESIZE_DIRECTION_LEFT_TOP				5						// Resize 방향 - 왼쪽위
#define dfRESIZE_DIRECTION_RIGHT_TOP			6						// Resize 방향 - 오른쪽위
#define dfRESIZE_DIRECTION_LEFT_BOTTOM			7						// Resize 방향 - 왼쪽아래
#define dfRESIZE_DIRECTION_RIGHT_BOTTOM			8						// Resize 방향 - 오른쪽아래

#define dfWM_DLG_POPUP_BTN_OK					WM_USER+101				// 팝업창 '확인' 버튼 클릭 메시지
#define dfWM_DLG_POPUP_BTN_CANCEL				WM_USER+102				// 팝업창 '취소' 버튼 클릭 메시지

//#define dfCOLOR_TRANSPARENT						RGB(255,0,255)			// 투명을 의미하는 색깔
#define dfTIMER_INITIALIZE						WM_USER+103				// 초기화를 위한 타이머

#define dfIDC_DLG_RESIZE_WND					1012

#define dfTHEME_CONTROL_TYPE_STATIC				1						// 테마 컨트롤 종류 - Static
#define dfTHEME_CONTROL_TYPE_GROUP				2						// 테마 컨트롤 종류 - Group
#define dfTHEME_CONTROL_TYPE_BUTTON				3						// 테마 컨트롤 종류 - Button
#define dfTHEME_CONTROL_TYPE_CHECK				4						// 테마 컨트롤 종류 - CheckBox
#define dfTHEME_CONTROL_TYPE_RADIO				5						// 테마 컨트롤 종류 - Radio
#define dfTHEME_CONTROL_TYPE_EDIT				6						// 테마 컨트롤 종류 - EditBox
#define dfTHEME_CONTROL_TYPE_LISTBOX			7						// 테마 컨트롤 종류 - ListBox
#define dfTHEME_CONTROL_TYPE_COMBO				8						// 테마 컨트롤 종류 - ComboBox
#define dfTHEME_CONTROL_TYPE_PROGRESS			9						// 테마 컨트롤 종류 - ProgressBar
#define dfTHEME_CONTROL_TYPE_LISTCTRL			10						// 테마 컨트롤 종류 - ListCtrl
#define dfTHEME_CONTROL_TYPE_SLIDER				11						// 테마 컨트롤 종류 - Slider
#define dfTHEME_CONTROL_TYPE_SCROLL				12						// 테마 컨트롤 종류 - Horizon ScrollBar
#define dfTHEME_CONTROL_TYPE_PICTURE			13						// 테마 컨트롤 종류 - Picture
#define dfTHEME_CONTROL_TYPE_GRID				14						// 테마 컨트롤 종류 - Grid
#define dfTHEME_CONTROL_TYPE_MENU				15						// 테마 컨트롤 종류 - MENU

// INI 파일 폴더 경로
#define dfINI_TEXT_FILE_NAME														TEXT("ConfigText.ini")
#define dfINI_POSITION_FILE_NAME													TEXT("ConfigPos.ini")
#define dfINI_IMAGE_FILE_NAME														TEXT("ConfigImage.ini")
#define dfINI_COLOR_FILE_NAME														TEXT("ConfigColor.ini")
#define dfINI_VALUE_FILE_NAME														TEXT("ConfigValue.ini")

/////////////////////////////////////////////////////////////////////////////
// CThemeDialog dialog

class CThemeDialog : public CDialog
{
// Construction
public:
	static struct ST_THEME_DIALONG_INFO
	{
		CString		strImageFilePath;		// 이미지 파일 폴더 경로
		CString		strConfigFilePath;		// Config 파일 폴더 경로
		CString		strCaption;				// Caption 문자열
		CRect		rCaption;				// Caption 영역좌표
		UINT		nAlignFormat;			// Caption 정렬방식
		int			nFontSize;				// 폰트 크기
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFont;					// 폰트 컬러
		COLORREF	crShadow;				// 그림자 컬러
		int			nBackDrawType;			// 배경 그리기 방식
		int			nBackImageFillType;		// 배경 이미지 채우기 방식
		UINT		nBackImageAlignFormat;	// 배경 이미지 정렬방식
		COLORREF	crBackground;			// 배경 컬러
		CBitmap		bmpBackOnFocus;			// 배경 이미지(포커스 설정)
		CBitmap		bmpBackKillFocus;		// 배경 이미지(포커스 해제)
		BOOL		bSettledImageKillFocus;	// 배경 이미지(포커스 해제) 존재여부
		int			nResizingDirection;		// Resize 방향
		int			nEdgeRange;				// 가장자리 범위
		BOOL		bFocus;					// 포커스 지정 여부
		CPoint		poMouse;				// 마우스 포인터 위치좌표
		//BOOL		bResizeState;			// Resize 상태
		BOOL		bStyleResize;			// 스타일 - Resize
		BOOL		bMouseDragMove;			// 마우스 드래그로 이동 여부
		CString		strThemeGroupName;		// 테마 그룹명
		int			nMinWidth;				// 최소 넓이
		int			nMinHeight;				// 최소 높이
		int			nMaxWidth;				// 최대 넓이
		int			nMaxHeight;				// 최대 높이
		CRect		rInitPos;				// 초기 위치 정보
		BOOL		bTransparentByColor;	// 컬러로 투명 처리
		COLORREF	crTransparent;			// 투명으로 처리할 컬러
		BYTE		byteTransparent;		// 투명도
		BOOL		bChild;					// 차일드 형식 여부
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
	};

	static struct ST_NONE_THEME_CTRL
	{
		CWnd*		pCtrl;
		CString		strCtrlID;
		CRect		rCtrlInit;
		DWORD		dwStyle;
	};

// Attributes
public:
	ST_THEME_DIALONG_INFO	m_stThemeInfo;
	CThemeToolTip			m_ToolTip;
	UINT					m_unDlgID;
	CDlgResizeWnd*			m_pDlgResizeWnd;

	// 스킨 컨트롤을 저장하는 CArray
	CArray <CThemeStatic*, CThemeStatic*>			m_arrThemeStatic;
	CArray <CThemeGroup*, CThemeGroup*>				m_arrThemeGroup;
	CArray <CThemeButton*, CThemeButton*>			m_arrThemeButton;
	CArray <CThemeCheck*, CThemeCheck*>				m_arrThemeCheck;
	CArray <CThemeRadio*, CThemeRadio*>				m_arrThemeRadio;
	CArray <CThemeEdit*, CThemeEdit*>				m_arrThemeEdit;
	CArray <CThemeListBox*, CThemeListBox*>			m_arrThemeListBox;
	CArray <CThemeCombo*, CThemeCombo*>				m_arrThemeCombo;
	CArray <CThemeProgress*, CThemeProgress*>		m_arrThemeProgress;
	CArray <CThemeListCtrl*, CThemeListCtrl*>		m_arrThemeListCtrl;
	CArray <CThemeSlider*, CThemeSlider*>			m_arrThemeSlider;
	CArray <CThemeScroll*, CThemeScroll*>			m_arrThemeScroll;
	CArray <CThemePicture*, CThemePicture*>			m_arrThemePicture;
	CArray <CThemeGrid*, CThemeGrid*>				m_arrThemeGrid;
	CArray <CThemeMenu*, CThemeMenu*>				m_arrThemeMenu;
	CArray <CThemeDialog*, CThemeDialog*>			m_arrThemeDialog;
	CArray <ST_NONE_THEME_CTRL, ST_NONE_THEME_CTRL>	m_arrNoneThemeCtrl;

// Dialog Data
	//{{AFX_DATA(CThemeDialog)
	//enum { IDD = _UNKNOWN_RESOURCE_ID_ };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThemeDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CThemeMenu* AddThemeMenuCtrl(UINT nMenuID, int nSubMenuNo, CString strIniCtrlName = TEXT(""));
	BOOL SetBackImage(UINT nImageID);
	BOOL SetBackImage(CString strFileName);
	void InitThemeVariable();
	void RepositionThemeDialog();
	void AddThemeDialog(CThemeDialog *pDlg, CString strIniCtrlName = TEXT(""));
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	BOOL GetChild() { return m_stThemeInfo.bChild; }
	void SetChild(BOOL bChild) { m_stThemeInfo.bChild = bChild; }
	void SetTransparentColor(COLORREF crTransparent);
	void SetTransparentByColor(BOOL bFlag);
	void RepositionNoneThemeCtrl();
	void AddNoneThemeCtrl(CWnd *pCtrl, CString strIniCtrlName = TEXT(""));
	void SetThemePath(CString strThemePath);
	void SetStyleAllCtrl();
	CString GetThemeFolderList();
	CString GetConfigFilePath() { return m_stThemeInfo.strConfigFilePath; }
	CString GetImageFilePath() { return m_stThemeInfo.strImageFilePath; }
	void SaveDlgPosToIni();
	void DrawAllDialogItem(CPaintDC *pdc);
	BOOL SetTransparentBack(COLORREF crTrans);
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	BOOL IsAbsoluteArea(CRect rArea);
	void RepositionAllCtrlByInitPos();
	void SetMaxHeight(int nMaxHeight);
	void SetMaxWidth(int nMaxWidth);
	void SetMinHeight(int nMinHeight);
	void SetMinWidth(int nMinWidth);
	void SaveAllCtrlInfoToIni();
	BOOL SetCtrlValueFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, int nValue);
	BOOL SetCtrlPosFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CRect RectCtrl);
	BOOL SetCtrlImageFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText);
	BOOL SetCtrlColorFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, COLORREF crCtrlColor);
	BOOL SetCtrlTextToIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString strText);
	void SetStyleGridCtrl(CThemeGrid *pCtrl, CString strIniItemName);
	CThemeGrid* AddThemeGridCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleListCtrl(CThemeListCtrl *pCtrl, CString strIniItemName);
	CThemeListCtrl* AddThemeListCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	CThemeMenu* AddThemeMenuCtrl(UINT nCtrlID, UINT nMenuID, int nSubMenuNo, CString strIniCtrlName=TEXT(""));
	void SetStyleMenuCtrl(CThemeMenu *pCtrl, CString strIniItemName);
	CThemeMenu* AddThemeMenuCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStylePictureCtrl(CThemePicture *pCtrl, CString strIniItemName);
	CThemePicture* AddThemePictureCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleScrollCtrl(CThemeScroll *pCtrl, CString strIniItemName);
	CThemeScroll* AddThemeScrollCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleSliderCtrl(CThemeSlider *pCtrl, CString strIniItemName);
	void SetToolTip(CString strText);
	CThemeSlider* AddThemeSliderCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleProgressCtrl(CThemeProgress *pCtrl, CString strIniItemName);
	CThemeProgress* AddThemeProgressCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	BOOL CheckChildControl(CWnd *pWndFocus);
	void InvalidateTitleBar();
	void SetStyleComboCtrl(CThemeCombo *pCtrl, CString strIniItemName);
	CThemeCombo* AddThemeComboCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleListBoxCtrl(CThemeListBox *pCtrl, CString strIniItemName);
	CThemeListBox* AddThemeListBoxCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleRadioCtrl(CThemeRadio *pCtrl, CString strIniItemName);
	CThemeRadio* AddThemeRadioCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleCheckCtrl(CThemeCheck *pCtrl, CString strIniItemName);
	CThemeCheck* AddThemeCheckCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetStyleEditCtrl(CThemeEdit *pCtrl, CString strIniItemName);
	CThemeEdit* AddThemeEditCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	//INI 파일에서 컨트롤 관련숫자 정보를 가져오는 함수
	BOOL GetCtrlValueFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, int& nValue);
	//INI 파일에서 숫자 정보를 가져오는 함수
	BOOL GetValueFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, int& nValue);
	// INI 파일에서 컨트롤 관련 컬러 정보를 가져오는 함수
	BOOL GetCtrlColorFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, COLORREF& crCtrlColor);
	// INI 파일에서 컬러 정보를 가져오는 함수
	BOOL GetColorFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, COLORREF& crCtrlColor);
	// 문자열에서 컬러 정보를 추출하여 반환하는 함수
	COLORREF CnvtStringToColor(CString sRect);
	//문자열에서 Rect 정보를 추출하여 반환하는 함수
	CRect CnvtStringToRect(CString sRect);
	// 컨트롤 INI파일에서 컨트롤 관련 텍스트 정보를 가져오는 함수
	BOOL GetCtrlTextFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText);
	// 컨트롤 INI파일에서 컨트롤 관련 이미지 파일 이름을 가져오는 함수
	BOOL GetCtrlImageFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText);
	// INI파일에서 텍스트 정보를 가져오는 함수
	BOOL GetTextFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CString &strText);
	//INI 파일에서 컨트롤 관련 위치정보를 가져오는 함수
	BOOL GetCtrlPosFromIniFile(LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CRect& RectCtrl);
	//INI 파일에서 위치 정보를 가져오는 함수
	BOOL GetPosFromIniFile(CString strIniFileName, LPCTSTR lpGrpName, LPCTSTR lpCtrlName, CRect& RectCtrl);
	void SetStyleButtonCtrl(CThemeButton *pCtrl, CString strIniItemName);
	void SetStyleGroupCtrl(CThemeGroup *pCtrl, CString strIniItemName);
	void SetStyleStaticCtrl(CThemeStatic *pCtrl, CString strIniItemName);
	CThemeButton* AddThemeButtonCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	CThemeGroup* AddThemeGroupCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	BOOL SetBackImageKillFocus(UINT nImageID);
	BOOL SetBackImageKillFocus(CString strFileName);
	void OnBtnClickMaximize();
	void OnBtnClickMinimize();
	void OnBtnClickExit();
	BOOL SetTransparent(BYTE byteDegree);
	void MakeToolTip(CString strToolTip);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	BOOL SetBackImageOnFocus(UINT nImageID);
	BOOL SetBackImageOnFocus(CString strFileName);
	void SetFont(int nFontSize, BOOL bFontBold=FALSE, COLORREF crFont=RGB(255,255,255), COLORREF crShadow=dfCOLOR_TRANSPARENT, UINT nAlignFormat=DT_LEFT|DT_VCENTER|DT_SINGLELINE);
	void SetFontBold(BOOL bBold);
	void DrawCaptionText(CPaintDC *pdc);
	void SetFontSize(int nFontSize);
	void SetFontColor(COLORREF crFont, COLORREF crShadow);
	void SetShadowColor(COLORREF crShadow);
	void SetFontColor(COLORREF crFont);
	void SetAlignFormat(UINT nAlignFormat);
	void SetCaptionArea(CRect rCaption);
	void SetCaptionText(CString strCaption);
	void RemoveStyleResize();
	void ResizeWndInit(int nDirection);
	void ResizeWndByDirection(int nDirection, CPoint poMouseOld, CPoint poMouseNew);
	CPoint Point2Absolute(CPoint poPoint);
	void ChangeMousePointImage(int nDirection);
	int IsMouseInEdgeRange(CPoint poMouse);
	void SetMouseDragMove(BOOL bMouseDragMove);
	void SetStyleResize(BOOL bStyleResize);
	void DrawImageByPatternBrush(CPaintDC *pdc, CBitmap *pBitmap, CRect rArea);
	void DrawBackByImage(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen);
	void SetImageFilePath(CString strFilePath);
	CString FilteringImageFilePath(CString strFileName);
	void DrawBackByColor(CPaintDC *pdc, COLORREF crBack, CRect rScreen);
	CThemeDialog(UINT uIDTemplate, CWnd* pParent = NULL);
	CWnd* AddThemeCtrl(UINT nCtrlID, int &nCtrlType, CString strIniCtrlName=TEXT(""));
	CWnd* AddThemeCtrl(UINT nCtrlID, CString strIniCtrlName);
	void SetStyleDlgFromIniFile();
	BOOL IsThemeGroup();
	void SetThemeGroupName(CString strGroupName);
	void DrawImageTransparent(CPaintDC *pdc, CBitmap *pBitmap, CRect rScreen, UINT unAlignFormat);
	CWnd* AddThemeCtrl(UINT nCtrlID);
	CThemeStatic* AddThemeStaticCtrl(UINT nCtrlID, CString strIniCtrlName=TEXT(""));
	void SetConfigFilePath(CString strFilePath);
	void SetBackImageFillType(int nImageFillType);
	void SetBackDrawType(int nDrawType);
	void SetBackColor(COLORREF crBack);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEDIALOG_H__7AA787AD_1790_4821_8AB2_C9CE6867D06F__INCLUDED_)
