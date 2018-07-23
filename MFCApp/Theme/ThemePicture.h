/****************************************************************/
/*																*/
/*		FILE NAME		:	ThemePicture.h						*/
/*		DISCRIPTION		:	Theme Picture Skin Control Class	*/
/*																*/
/*		PROGRAMMER		:	Jung, Sang-Soo						*/
/*		Mail			:	topofsan@hotmail.com				*/
/*		Home			:	http://rlxk.co.kr					*/
/*		LAST UPDATE		:	2008.11.02							*/
/****************************************************************/

#if !defined(AFX_THEMEPICTURE_H__527C6C28_E917_46B3_854A_60BC9F8B33B3__INCLUDED_)
#define AFX_THEMEPICTURE_H__527C6C28_E917_46B3_854A_60BC9F8B33B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThemePicture.h : header file
//

#include <vector>

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
// CThemePicture window

class CThemePicture : public CWnd
{
// Construction
public:
	static struct ST_THEME_PICTURE_INFO
	{
		CString		strCaption;				// Caption 문자열
		CRect		rCaption;				// Caption 영역좌표
		UINT		nAlignFormat;			// Caption 정렬방식
		int			nFontSize;				// 폰트 크기
		BOOL		bFontBold;				// 폰트 두께
		COLORREF	crFont;					// 폰트 컬러
		COLORREF	crShadow;				// 그림자 컬러
		int			nBackImageFillType;		// 배경 이미지 채우기 방식
		UINT		nBackImageAlignFormat;	// 배경 이미지 정렬방식
		CString		strIniCtrlName;			// INI 파일 조회용 컨트롤명
		CRect		rInitPos;				// 초기 위치 정보
	};

	/* structure that keeps a single frame info */
	struct TFrame    
	{
		IPicture *m_pPicture;  /* pointer to the interface used for drawing */
		SIZE     m_frameSize;
		SIZE     m_frameOffset;
		UINT     m_nDelay;     /* delay (in 1/100s of a second) */
		UINT     m_nDisposal;  /* disposal method */
	};

	#pragma pack(1)   /* turn byte alignment on */

	enum GIFBlockTypes
	{
		BLOCK_UNKNOWN,
		BLOCK_APPEXT,
		BLOCK_COMMEXT,
		BLOCK_CONTROLEXT,
		BLOCK_PLAINTEXT,
		BLOCK_IMAGE,
		BLOCK_TRAILER
	};

	/* graphic control extension packed field values */
	enum ControlExtValues 
	{
		GCX_PACKED_DISPOSAL,  /* disposal method */
		GCX_PACKED_USERINPUT,
		GCX_PACKED_TRANSPCOLOR
	};

	/* logical screen descriptor packed field values */
	enum LSDPackedValues  
	{
		LSD_PACKED_GLOBALCT,
		LSD_PACKED_CRESOLUTION,
		LSD_PACKED_SORT,
		LSD_PACKED_GLOBALCTSIZE
	};

	/* image descriptor packed field values */
	enum IDPackedValues   
	{
		ID_PACKED_LOCALCT,
		ID_PACKED_INTERLACE,
		ID_PACKED_SORT,
		ID_PACKED_LOCALCTSIZE
	};

	/* GIF header */
	struct TGIFHeader       
	{
		char m_cSignature[3];
		char m_cVersion[3];
	};


	/* Logical Screen Descriptor */
	struct TGIFLSDescriptor 
	{
		WORD m_wWidth;
		WORD m_wHeight;
		unsigned char m_cPacked;
		unsigned char m_cBkIndex;
		unsigned char m_cPixelAspect;
		inline int GetPackedValue(enum LSDPackedValues Value);
	};

	/* application extension block */
	struct TGIFAppExtension 
	{
		unsigned char m_cExtIntroducer;
		unsigned char m_cExtLabel;
		unsigned char m_cBlockSize;
		char m_cAppIdentifier[8];
		char m_cAppAuth[3];
	};

	/* graphic control extension block */
	struct TGIFControlExt 
	{
		unsigned char m_cExtIntroducer;
		unsigned char m_cControlLabel;
		unsigned char m_cBlockSize;
		unsigned char m_cPacked;
		WORD m_wDelayTime;
		unsigned char m_cTColorIndex;
		unsigned char m_cBlockTerm;
		public:
		inline int GetPackedValue(enum ControlExtValues Value);
	};

	/* comment extension block */
	struct TGIFCommentExt  
	{
		unsigned char m_cExtIntroducer;
		unsigned char m_cCommentLabel;
	};


	/* plain text extension block*/
	struct TGIFPlainTextExt 
	{
		unsigned char m_cExtIntroducer;
		unsigned char m_cPlainTextLabel;
		unsigned char m_cBlockSize;
		WORD m_wLeftPos;   
		WORD m_wTopPos;    
		WORD m_wGridWidth; 
		WORD m_wGridHeight; 
		unsigned char m_cCellWidth; 
		unsigned char m_cCellHeight;
		unsigned char m_cFgColor; 
		unsigned char m_cBkColor;
	public:
	};

	/* image descriptor block */
	struct TGIFImageDescriptor 
	{
		unsigned char m_cImageSeparator; // image separator byte (0x2C)
		WORD m_wLeftPos; // image left position
		WORD m_wTopPos;  // image top position
		WORD m_wWidth;   // image width
		WORD m_wHeight;  // image height
		unsigned char m_cPacked; // packed field
		inline int GetPackedValue(enum IDPackedValues Value);
	};

	/* turn byte alignment off */
	#pragma pack() 

// Attributes
public:
	ST_THEME_PICTURE_INFO	m_stThemeInfo;
	CThemeToolTip			m_ToolTip;

	RECT m_PaintRect;
	SIZE m_PictureSize;
	COLORREF m_clrBackground;
	UINT m_nCurrFrame;
	UINT m_nDataSize;
	UINT m_nCurrOffset;
	UINT m_nGlobalCTSize;
	BOOL m_bIsGIF;
	BOOL m_bIsPlaying;
	volatile BOOL m_bExitThread;
	BOOL m_bIsInitialized;
	HDC m_hMemDC;

	HDC m_hDispMemDC;
	HBITMAP m_hDispMemBM;
	HBITMAP m_hDispOldBM;

	HBITMAP m_hBitmap;
	HBITMAP m_hOldBitmap;
	HANDLE m_hThread;
	HANDLE m_hExitEvent;
	IPicture * m_pPicture;
	TGIFHeader * m_pGIFHeader;
	unsigned char * m_pRawData;
	TGIFLSDescriptor * m_pGIFLSDescriptor;
	std::vector<TFrame> m_arrFrames;

// Operations
public:
	CThemePicture();
	CString GetIniCtrlName()				{ return m_stThemeInfo.strIniCtrlName; }
	void SetIniCtrlName(CString strName)	{ m_stThemeInfo.strIniCtrlName = strName; }
	BOOL GetPaintRect(RECT *lpRect);
	BOOL SetPaintRect(const RECT *lpRect);
	void Stop(); 
	void UnLoad();

	BOOL IsGIF() const;
	BOOL IsPlaying() const;
	BOOL IsAnimatedGIF() const;
	SIZE GetSize() const;
	int GetFrameCount() const;
	COLORREF GetBkColor() const;
	void SetBackColor(COLORREF clr);

	/* draws the picture (starts an animation thread if needed) */
	/* if an animation was previously stopped by Stop(), */
	/* continues it from the last displayed frame */
	BOOL Draw();

	/* loads a picture from a file */
	/* i.e. Load(_T("mypic.gif")); */
	BOOL Load(LPCTSTR szFileName);

	/* loads a picture from a global memory block (allocated by GlobalAlloc) */
	/* Warning: this function DOES NOT free the global memory, pointed to by hGlobal */
	BOOL Load(HGLOBAL hGlobal, DWORD dwSize);

	/* loads a picture from a program resource */
	/* i.e. Load(MAKEINTRESOURCE(IDR_MYPIC),_T("GIFTYPE")); */
	BOOL Load(LPCTSTR szResourceName,LPCTSTR szResourceType);

protected:

#ifdef GIF_TRACING
	void EnumGIFBlocks();
	void WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize);
#endif

	void ThreadAnimation();
	static UINT WINAPI _ThreadAnimation(LPVOID pParam);

	int GetNextBlockLen() const;
	BOOL SkipNextBlock();
	BOOL SkipNextGraphicBlock();
	BOOL PrepareDC(int nWidth, int nHeight);
	void ResetDataPointer();
	enum GIFBlockTypes GetNextBlock() const;
	UINT GetSubBlocksLen(UINT nStartingOffset) const;
	HGLOBAL GetNextGraphicBlock(UINT *pBlockLen, UINT *pDelay, 
		SIZE *pBlockSize, SIZE *pBlockOffset, UINT *pDisposal);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThemePicture)
	public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitThemeVariable();
	CRect GetInitPos() { return m_stThemeInfo.rInitPos; }
	void SetInitPos(CRect rInitPos) { m_stThemeInfo.rInitPos = rInitPos; }
	void SetWindowText(CString strText);
	void SetBackImageAlignFormat(UINT nAlignFormat);
	void SetBackImageFillType(int nBackImageFillType);
	void DrawImageResizeBlock(CPaintDC *pdc, HDC hMemDC, CRect rScreen);
	void DrawImageByPattern(CPaintDC *pdc, HDC hMemDC, CRect rScreen);
	void DrawImageResize(CPaintDC *pdc, HDC hMemDC, CRect rScreen);
	void DrawImageOnce(CPaintDC *pdc, HDC hMemDC, CRect rScreen, UINT unAlignFormat);
	void DrawBackImage(CPaintDC *pdc);
	void SetBackImage(CString strFileName);
	void DeleteToolTip();
	void ChangeToolTip(CString strToolTip);
	void MakeToolTip(CString strToolTip);
	void SetToolTip(CString strText);
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
	virtual ~CThemePicture();

	// Generated message map functions
protected:
	//{{AFX_MSG(CThemePicture)
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THEMEPICTURE_H__527C6C28_E917_46B3_854A_60BC9F8B33B3__INCLUDED_)
