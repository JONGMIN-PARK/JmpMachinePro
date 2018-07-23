// ThemePicture.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemePicture.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/* TGIFControlExt */
inline int CThemePicture::TGIFControlExt::GetPackedValue(enum ControlExtValues Value)
{
	int nRet = (int)m_cPacked;
	switch (Value)
	{
	case GCX_PACKED_DISPOSAL:
		nRet = (nRet & 28) >> 2;
		break;

	case GCX_PACKED_USERINPUT:
		nRet = (nRet & 2) >> 1;
		break;

	case GCX_PACKED_TRANSPCOLOR:
		nRet &= 1;
		break;
	}; /* end of switch */

	return nRet;
} /* end of TGIFControlExt*/


/* TGIFLSDescriptor */
inline int CThemePicture::TGIFLSDescriptor::GetPackedValue(enum LSDPackedValues Value)
{
	int nRet = (int)m_cPacked;

	switch (Value)
	{
	case LSD_PACKED_GLOBALCT:
		nRet = nRet >> 7;
		break;

	case LSD_PACKED_CRESOLUTION:
		nRet = ((nRet & 0x70) >> 4) + 1;
		break;

	case LSD_PACKED_SORT:
		nRet = (nRet & 8) >> 3;
		break;

	case LSD_PACKED_GLOBALCTSIZE:
		nRet &= 7;
		break;
	}; /* end of switch */

	return nRet;
} /* end of TGIFLSDescriptor */


/* TGIFImageDescriptor */
inline int CThemePicture::TGIFImageDescriptor::GetPackedValue(enum IDPackedValues Value)
{
	int nRet = (int)m_cPacked;

	switch (Value)
	{
	case ID_PACKED_LOCALCT:
		nRet >>= 7;
		break;

	case ID_PACKED_INTERLACE:
		nRet = ((nRet & 0x40) >> 6);
		break;

	case ID_PACKED_SORT:
		nRet = (nRet & 0x20) >> 5;
		break;

	case ID_PACKED_LOCALCTSIZE:
		nRet &= 7;
		break;
	}; /* end of switch */

	return nRet;
} /* end of TGIFImageDescriptor */

/////////////////////////////////////////////////////////////////////////////
// CThemePicture

CThemePicture::CThemePicture()
{

	/* check structures size */	
	ASSERT(sizeof(TGIFImageDescriptor) == 10);
	ASSERT(sizeof(TGIFAppExtension)    == 14);
	ASSERT(sizeof(TGIFPlainTextExt)    == 15);
	ASSERT(sizeof(TGIFLSDescriptor)    ==  7);
	ASSERT(sizeof(TGIFControlExt)	   ==  8);
	ASSERT(sizeof(TGIFCommentExt)	   ==  2);
	ASSERT(sizeof(TGIFHeader)		   ==  6);

	m_pGIFLSDescriptor = NULL;
	m_pGIFHeader	   = NULL;
	m_pPicture		   = NULL;
	m_pRawData		   = NULL;
	m_hThread		   = NULL;
	m_hBitmap          = NULL;
	m_hMemDC		   = NULL;

	m_hDispMemDC       = NULL;
	m_hDispMemBM       = NULL;
	m_hDispOldBM       = NULL;

	m_bIsInitialized   = FALSE;
	m_bExitThread	   = FALSE;
	m_bIsPlaying       = FALSE;
	m_bIsGIF		   = FALSE;
	m_clrBackground    = RGB(255,255,255);
	m_nGlobalCTSize    = 0;
	m_nCurrOffset	   = 0;
	m_nCurrFrame	   = 0;
	m_nDataSize		   = 0;
	m_PictureSize.cx = m_PictureSize.cy = 0;
	SetRect(&m_PaintRect,0,0,0,0);

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.strCaption = "";								// Caption 문자열

	// 전역 변수 초기화
	InitThemeVariable();

	m_hExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	/* end of constructor */
}

// 전역 변수 초기화
void CThemePicture::InitThemeVariable()
{

	// 테마 컨트롤 옵션 초기화
	m_stThemeInfo.rCaption = CRect(0,0,0,0);					// Caption 영역좌표
	m_stThemeInfo.nFontSize = 12;								// 폰트 크기
	m_stThemeInfo.bFontBold = FALSE;							// 폰트 두께
	m_stThemeInfo.crFont = RGB(0,0,0);							// 폰트 컬러
	m_stThemeInfo.crShadow = dfCOLOR_TRANSPARENT;				// 그림자 컬러
	m_stThemeInfo.nAlignFormat = DT_LEFT|DT_VCENTER;			// Caption 정렬방식
	//m_stThemeInfo.nBackDrawType = dfBACK_DRAW_TYPE_NONE;		// 배경 그리기 방식
	m_stThemeInfo.nBackImageFillType = dfBACK_IMAGE_FILL_TYPE_ONCE;	// 배경 이미지 채우기 방식
	//m_stThemeInfo.nBackImageFillType = dfBACK_IMAGE_FILL_TYPE_BLOCK;
	m_stThemeInfo.nBackImageAlignFormat = DT_LEFT|DT_TOP;		// 배경 이미지 정렬방식
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보
	
}

CThemePicture::~CThemePicture()
{
	UnLoad();
	CloseHandle(m_hExitEvent);
}


BEGIN_MESSAGE_MAP(CThemePicture, CWnd)
	//{{AFX_MSG_MAP(CThemePicture)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemePicture message handlers

/* Load */
BOOL CThemePicture::Load(HGLOBAL hGlobal, DWORD dwSize)
{
	IStream *pStream = NULL;
	UnLoad();

	if (!(m_pRawData = reinterpret_cast<unsigned char*> (GlobalLock(hGlobal))) )
	{
		TRACE(_T("Load: Error locking memory\n"));
		return FALSE;
	}; /* end of if */

	m_nDataSize = dwSize;
	m_pGIFHeader = reinterpret_cast<TGIFHeader *> (m_pRawData);

	if ((memcmp(&m_pGIFHeader->m_cSignature,"GIF",3) != 0) &&
		((memcmp(&m_pGIFHeader->m_cVersion,"87a",3) != 0) ||
		 (memcmp(&m_pGIFHeader->m_cVersion,"89a",3) != 0)) )
	{
		/* it's neither GIF87a nor GIF89a */
		/* do the default processing */
		/* clear GIF variables */
		m_pRawData = NULL;
		GlobalUnlock(hGlobal);

		/* don't delete memory on object's release */
		if (CreateStreamOnHGlobal(hGlobal,FALSE,&pStream) != S_OK)
			return FALSE;
		/* end of if */

		if (OleLoadPicture(pStream,dwSize,FALSE,IID_IPicture,
			reinterpret_cast<LPVOID *>(&m_pPicture)) != S_OK)
		{
			pStream->Release();
			return FALSE;
		};/* end of if */
		pStream->Release();

		/* store picture's size */
		long hmWidth;
		long hmHeight;
		m_pPicture->get_Width(&hmWidth);
		m_pPicture->get_Height(&hmHeight);

		HDC hDC = ::GetDC(m_hWnd);
		m_PictureSize.cx = MulDiv(hmWidth, GetDeviceCaps(hDC,LOGPIXELSX), 2540);
		m_PictureSize.cy = MulDiv(hmHeight, GetDeviceCaps(hDC,LOGPIXELSY), 2540);
		::ReleaseDC(m_hWnd,hDC);
	}
	else
	{
		/* it's a GIF */
		m_bIsGIF = TRUE;
		m_pGIFLSDescriptor = reinterpret_cast<TGIFLSDescriptor *>
			(m_pRawData + sizeof(TGIFHeader));
		if (m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCT) == 1)
		{
			/* calculate the globat color table size */
			m_nGlobalCTSize = static_cast<int>
				(3* (1 << (m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCTSIZE)+1)));
			/* get the background color if GCT is present */
			unsigned char *pBkClr = m_pRawData + sizeof(TGIFHeader) + 
				sizeof(TGIFLSDescriptor) + 3*m_pGIFLSDescriptor->m_cBkIndex;
			m_clrBackground = RGB(pBkClr[0],pBkClr[1],pBkClr[2]);
		}; /* end of if */

		/* store the picture's size */
		m_PictureSize.cx = m_pGIFLSDescriptor->m_wWidth;
		m_PictureSize.cy = m_pGIFLSDescriptor->m_wHeight;

		/* determine frame count for this picture */
		UINT nFrameCount=0;
		ResetDataPointer();
		while (SkipNextGraphicBlock())
			nFrameCount++;

#ifdef GIF_TRACING
		TRACE(
			_T(" -= GIF encountered\n"
			   "Logical Screen dimensions = %dx%d\n"
			   "Global color table = %d\n"
			   "Color depth = %d\n"
			   "Sort flag = %d\n"
			   "Size of Global Color Table = %d\n"
			   "Background color index = %d\n"
			   "Pixel aspect ratio = %d\n"
			   "Frame count = %d\n"
			   "Background color = %06Xh\n\n"
			  ),
			m_pGIFLSDescriptor->m_wWidth,
			m_pGIFLSDescriptor->m_wHeight,
			m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCT),
			m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_CRESOLUTION),
			m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_SORT),
			m_pGIFLSDescriptor->GetPackedValue(LSD_PACKED_GLOBALCTSIZE),
			m_pGIFLSDescriptor->m_cBkIndex,
			m_pGIFLSDescriptor->m_cPixelAspect,
			nFrameCount,
			m_clrBackground
			);
		EnumGIFBlocks();
#endif

		if (nFrameCount == 0) /* it's an empty GIF! */
		{
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);
			return FALSE;
		}; /* end of if */


		/* now check the frame count */
		/* if there's only one frame, no need to animate this GIF */
		/* therefore, treat it like any other pic */
		if (nFrameCount == 1)
		{
			/* clear GIF variables */
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			/* don't delete memory on object's release */
			if (CreateStreamOnHGlobal(hGlobal,FALSE,&pStream) != S_OK)
				return FALSE;
			/* end of if */
			if (OleLoadPicture(pStream,dwSize,FALSE,IID_IPicture,
				(LPVOID *)&m_pPicture) != S_OK)
			{
				pStream->Release();
				return FALSE;
			}; /* end of if */

			pStream->Release();
		}
		else
		{
			/* if, on the contrary, there are several frames */
			/* then store separate frames in an array */
			TFrame frame;
			UINT nBlockLen;
			HGLOBAL hFrameData;
			UINT nCurFrame = 0;

			ResetDataPointer();
			while (hFrameData = GetNextGraphicBlock(&nBlockLen,
				&frame.m_nDelay, &frame.m_frameSize,
				&frame.m_frameOffset, &frame.m_nDisposal) )
			{
				#ifdef GIF_TRACING
				#endif

				IStream *pStream = NULL;

				/* delete memory on object's release */
				if (CreateStreamOnHGlobal(hFrameData,TRUE,&pStream) != S_OK)
				{
					GlobalFree(hFrameData);
					continue;
				}; /* end of if */

				if (OleLoadPicture(pStream,nBlockLen,FALSE,
					IID_IPicture,
					reinterpret_cast<LPVOID *>(&frame.m_pPicture)) != S_OK)
				{
					pStream->Release();
					continue;
				}; /* end of if */
				pStream->Release();
			
				/* everything went well, add this frame */
				m_arrFrames.push_back(frame);
			}; /* end of while */

			/* clean after ourselves */
			m_pRawData = NULL;
			GlobalUnlock(hGlobal);

			if (m_arrFrames.empty())return FALSE;
		}; /* end of if */
	}; /* end of if */

	//CRect rScreen;
	//this->GetWindowRect(&rScreen);
	
	//return PrepareDC(rScreen.Width(),rScreen.Height());
	// 이미지 사이즈 크기로 메모리 DC 생성
	BOOL bResult = PrepareDC(m_PictureSize.cx,m_PictureSize.cy);

	// 컨트롤 화면 크기를 원래대로 되돌린다
	//this->MoveWindow(rScreen);

	return bResult;
} /* end of Load */


/* PrepareDC */
BOOL CThemePicture::PrepareDC(int nWidth, int nHeight)
{
	//SetWindowPos(NULL,0,0,nWidth,nHeight,SWP_NOMOVE | SWP_NOZORDER);

	HDC hWinDC = ::GetDC(m_hWnd);
	if (!hWinDC) return FALSE;
	/* end of if */

	m_hMemDC = CreateCompatibleDC(hWinDC);
	if (!m_hMemDC) 
	{
		::ReleaseDC(m_hWnd,hWinDC);
		return FALSE;
	};/* end of if */

	m_hBitmap  = CreateCompatibleBitmap(hWinDC,nWidth,nHeight);
	if (!m_hBitmap) 
	{
		::ReleaseDC(m_hWnd,hWinDC);
		::DeleteDC(m_hMemDC);
		return FALSE;
	};/* end of if */

	m_hOldBitmap = reinterpret_cast<HBITMAP> 
						(SelectObject(m_hMemDC,m_hBitmap));
	
	/* fill the background */
	m_clrBackground = GetSysColor(COLOR_3DFACE);
	RECT rect = {0,0,nWidth,nHeight};
	FillRect(m_hMemDC,&rect,(HBRUSH)(COLOR_WINDOW));

	::ReleaseDC(m_hWnd,hWinDC);
	m_bIsInitialized = TRUE;
	return TRUE;
} /* end of PrepareDC */


/* UnLoad */
void CThemePicture::UnLoad()
{
	Stop();
	if (m_pPicture)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
	}; /* end of if */
	
	std::vector<TFrame>::iterator it;
	for (it=m_arrFrames.begin();it<m_arrFrames.end();it++)
		(*it).m_pPicture->Release();
	/* end of for */

	m_arrFrames.clear();

	if (m_hMemDC)
	{
		SelectObject(m_hMemDC,m_hOldBitmap);
		::DeleteDC(m_hMemDC);
		::DeleteObject(m_hBitmap);
		m_hMemDC  = NULL;
		m_hBitmap = NULL;
	}; /* end of if */

	if (m_hDispMemDC)
	{
		SelectObject(m_hDispMemDC,m_hDispOldBM);
		::DeleteDC(m_hDispMemDC);
		::DeleteObject(m_hDispMemBM);
		m_hDispMemDC  = NULL;
		m_hDispMemBM = NULL;
	}; /* end of if */

	SetRect(&m_PaintRect,0,0,0,0);
	m_pGIFLSDescriptor = NULL;
	m_pGIFHeader	   = NULL;
	m_pRawData		   = NULL;
	m_hThread		   = NULL;
	m_bIsInitialized   = FALSE;
	m_bExitThread	   = FALSE;
	m_bIsGIF		   = FALSE;
	m_clrBackground    = RGB(255,255,255);
	m_nGlobalCTSize	   = 0;
	m_nCurrOffset	   = 0;
	m_nCurrFrame	   = 0;
	m_nDataSize		   = 0;
} /* end of UnLoad */


/* Draw */
BOOL CThemePicture::Draw()
{
	if (!m_bIsInitialized)
	{
		TRACE(_T("Call one of the CThemePicture::Load() member functions before calling Draw()\n"));
		return FALSE;
	}; /* end of if */

	if (IsAnimatedGIF())
	{
		/* the picture needs animation */
		/* we'll start the thread that will handle it for us */
		unsigned int nDummy;
		m_hThread = (HANDLE) _beginthreadex(NULL,0,_ThreadAnimation,this,
			CREATE_SUSPENDED,&nDummy);
		if (!m_hThread)
		{
			TRACE(_T("Draw: Couldn't start a GIF animation thread\n"));
			return FALSE;
		} 
		else 
			ResumeThread(m_hThread);
		/* end of if */
	} 
	else
	{
		if (m_pPicture)
		{
			long hmWidth;
			long hmHeight;
			m_pPicture->get_Width(&hmWidth);
			m_pPicture->get_Height(&hmHeight);
			if (m_pPicture->Render(m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, 
				0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
			{
				CRect	rect;
				GetClientRect(rect);
				int	x = (rect.Width()-m_PictureSize.cx)/2;
				int y = (rect.Height()-m_PictureSize.cy)/2;

				Invalidate(FALSE);
				return TRUE;
			}; /* end of if */
		}; /* end of if */
	}; /* end of if */

	return FALSE;	
} /* end of Draw */


/* GetSize */
SIZE CThemePicture::GetSize() const
{
	return m_PictureSize;
} /* end of GetSize */


/* Load Gif */
BOOL CThemePicture::Load(LPCTSTR szFileName)
{
	ASSERT(szFileName);
	
	CFile file;
	HGLOBAL hGlobal;
	DWORD dwSize;

	if (!file.Open(szFileName,
				CFile::modeRead | 
				CFile::shareDenyWrite) )
	{
		TRACE(_T("Load (file): Error opening file %s\n"),szFileName);
		return FALSE;
	} /* end of if */

	dwSize = (DWORD)file.GetLength();
	hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	if (!hGlobal)
	{
		TRACE(_T("Load (file): Error allocating memory\n"));
		return FALSE;
	} /* end of if */
	
	char *pData = reinterpret_cast<char*>(GlobalLock(hGlobal));
	if (!pData)
	{
		TRACE(_T("Load (file): Error locking memory\n"));
		GlobalFree(hGlobal);
		return FALSE;
	} /* end of if */

	TRY
	{
		file.Read(pData,dwSize);
	}
	CATCH(CFileException, e)
	{
		TRACE(_T("Load (file): An exception occured while reading the file %s\n"),
			szFileName);
		GlobalFree(hGlobal);
		e->Delete();
		file.Close();
		return FALSE;
	} /* end of try~catch */

	END_CATCH
	GlobalUnlock(hGlobal);
	file.Close();

	BOOL bRetValue = Load(hGlobal,dwSize);
	GlobalFree(hGlobal);
	return bRetValue;
} /* end load */


/* Load */
BOOL CThemePicture::Load(LPCTSTR szResourceName, LPCTSTR szResourceType)
{
	ASSERT(szResourceName);
	ASSERT(szResourceType);

	HRSRC hPicture = FindResource(AfxGetResourceHandle(),szResourceName,szResourceType);
	HGLOBAL hResData;
	if (!hPicture || !(hResData = LoadResource(AfxGetResourceHandle(),hPicture)))
	{
		TRACE(_T("Load (resource): Error loading resource %s\n"),szResourceName);
		return FALSE;
	}; /* end of if */
	DWORD dwSize = SizeofResource(AfxGetResourceHandle(),hPicture);

	/* hResData is not the real HGLOBAL (we can't lock it) */
	/* let's make it real */
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE | GMEM_NODISCARD,dwSize);
	if (!hGlobal)
	{
		TRACE(_T("Load (resource): Error allocating memory\n"));
		FreeResource(hResData);
		return FALSE;
	}; /* end of if */
	
	char *pDest = reinterpret_cast<char *> (GlobalLock(hGlobal));
	char *pSrc = reinterpret_cast<char *> (LockResource(hResData));
	if (!pSrc || !pDest)
	{
		TRACE(_T("Load (resource): Error locking memory\n"));
		GlobalFree(hGlobal);
		FreeResource(hResData);
		return FALSE;
	}; /* end of if */

	CopyMemory(pDest,pSrc,dwSize);
	FreeResource(hResData);
	GlobalUnlock(hGlobal);

	BOOL bRetValue = Load(hGlobal,dwSize);
	GlobalFree(hGlobal);
	return bRetValue;
} /* end of load */


/* ResetDataPointer */
void CThemePicture::ResetDataPointer()
{
	/* skip header and logical screen descriptor */
	m_nCurrOffset = sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize;
} /* end of ResetDataPointer */


/* SkipNextGralphickBlock */
BOOL CThemePicture::SkipNextGraphicBlock()
{
	if (!m_pRawData) return FALSE;
	/* end of if */
	enum GIFBlockTypes nBlock;

	nBlock = GetNextBlock();

	while ((nBlock != BLOCK_CONTROLEXT) &&
		   (nBlock != BLOCK_IMAGE) &&
		   (nBlock != BLOCK_PLAINTEXT) &&
		   (nBlock != BLOCK_UNKNOWN) &&
		   (nBlock != BLOCK_TRAILER) )
	{
		if (!SkipNextBlock()) return NULL;
		/* end of if */
		nBlock = GetNextBlock();
	};/* end of while */

	if ((nBlock == BLOCK_UNKNOWN) ||
		(nBlock == BLOCK_TRAILER))
		return FALSE;
	/* end of if */
	
	/* it's either a control ext.block, an image or a plain text */
	if (GetNextBlockLen() <= 0) return FALSE;
	/* end of if */
	if (nBlock == BLOCK_CONTROLEXT)
	{
		if (!SkipNextBlock()) return FALSE;
		/* end of if */
		nBlock = GetNextBlock();

		/* skip everything until we meet an image block or a plain-text block */
		while ((nBlock != BLOCK_IMAGE) &&
			   (nBlock != BLOCK_PLAINTEXT) &&
			   (nBlock != BLOCK_UNKNOWN) &&
			   (nBlock != BLOCK_TRAILER) )
		{
			if (!SkipNextBlock()) return NULL;
			/* end of if */
			nBlock = GetNextBlock();
		}; /* end of while  */

		if ((nBlock == BLOCK_UNKNOWN) ||
			(nBlock == BLOCK_TRAILER))
			return FALSE;
		/* end of if */
	};

	/* skip the found data block (image or plain-text) */
	if (!SkipNextBlock()) return FALSE;
	/* end of if */
	return TRUE;
} /* end of SkipNextGraphicBlock */


/* GetSubBlocksLen */
UINT CThemePicture::GetSubBlocksLen(UINT nStartingOffset) const
{
	UINT nRet = 0;
	UINT nCurOffset = nStartingOffset;
	
	while (m_pRawData[nCurOffset] != 0)
	{
		nRet += m_pRawData[nCurOffset]+1;
		nCurOffset += m_pRawData[nCurOffset]+1;
	};/* end of while */ 

	return nRet+1;
} /* end of GetSubBlocksLen */


/*GetNextBlock */
enum CThemePicture::GIFBlockTypes CThemePicture::GetNextBlock() const
{
	switch(m_pRawData[m_nCurrOffset])
	{
	case 0x21:
		/* extension block */
		switch(m_pRawData[m_nCurrOffset+1])
		{
		case 0x01:
			/* plain text extension */
			return BLOCK_PLAINTEXT;
			break;

		case 0xF9:
			/* graphic control extension */
			return BLOCK_CONTROLEXT;
			break;

		case 0xFE:
			/* comment extension */
			return BLOCK_COMMEXT;
			break;

		case 0xFF:
			/* application extension */
			return BLOCK_APPEXT;
			break;
		};
		break;
	
	case 0x3B:
		/* trailer */
		return BLOCK_TRAILER;
		break;

	case 0x2C:
		/* image data */
		return BLOCK_IMAGE;
		break;
	};/* end of switch */

	return BLOCK_UNKNOWN;
} /* end of GetNextBlock */


/* SkipNextBlock */
BOOL CThemePicture::SkipNextBlock()
{
	if (!m_pRawData) return FALSE;
	/* end of if */

	int nLen = GetNextBlockLen();
	if ((nLen <= 0) || ((m_nCurrOffset+nLen) > m_nDataSize))
		return FALSE;
	/* end of if */

	m_nCurrOffset += nLen;
	return TRUE;
} /* end of SkipNextBlock */


/*GetNextBlockLen*/
int CThemePicture::GetNextBlockLen() const
{
	GIFBlockTypes nBlock = GetNextBlock();

	int nTmp;

	switch(nBlock)
	{
	case BLOCK_UNKNOWN:
		return -1;
		break;

	case BLOCK_TRAILER:
		return 1;
		break;

	case BLOCK_APPEXT:
		nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFAppExtension));
		if (nTmp > 0)	return sizeof(TGIFAppExtension)+nTmp;
		/* end of if */
		break;

	case BLOCK_COMMEXT:
		nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFCommentExt));
		if (nTmp > 0)	return sizeof(TGIFCommentExt)+nTmp;
		/* end of if */
		break;

	case BLOCK_CONTROLEXT:
		return sizeof(TGIFControlExt);
		break;

	case BLOCK_PLAINTEXT:
		nTmp = GetSubBlocksLen(m_nCurrOffset+sizeof(TGIFPlainTextExt));
		if (nTmp > 0)	return sizeof(TGIFPlainTextExt)+nTmp;
		/* end of if */
		break;

	case BLOCK_IMAGE:
		TGIFImageDescriptor *pIDescr = 
			reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
		int nLCTSize = (int)
			(pIDescr->GetPackedValue(ID_PACKED_LOCALCT)*3*
			(1 << (pIDescr->GetPackedValue(ID_PACKED_LOCALCTSIZE)+1)));

		int nTmp = GetSubBlocksLen(m_nCurrOffset+
			sizeof(TGIFImageDescriptor) + nLCTSize + 1);
		if (nTmp > 0)	return sizeof(TGIFImageDescriptor) + nLCTSize + 1 + nTmp;
		/* end of if */
		break;
	};/* end of switch */

	return 0;
} /* end of GetNextBlockLen*/


/*_ThreadAnimation*/
UINT WINAPI CThemePicture::_ThreadAnimation(LPVOID pParam)
{
	ASSERT(pParam);
	CThemePicture *pPic = reinterpret_cast<CThemePicture *> (pParam);

	pPic->m_bIsPlaying = TRUE;
	pPic->ThreadAnimation();
	pPic->m_bIsPlaying = FALSE;

	/* this thread has finished its work so we close the handle */
	CloseHandle(pPic->m_hThread); 
	/* and init the handle to zero (so that Stop() doesn't Wait on it) */
	pPic->m_hThread = 0;
	return 0;
} /* end of _ThreadAnimation */


/*ThreadAnimation*/
void CThemePicture::ThreadAnimation()
{
	/* first, restore background (for stop/draw support) */
	/* disposal method #2 */
	if (m_arrFrames[m_nCurrFrame].m_nDisposal == 2)
	{
		HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
		if (hBrush)
		{
			RECT rect = {
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx + m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy + m_arrFrames[m_nCurrFrame].m_frameSize.cy };
			FillRect(m_hMemDC,&rect,hBrush);
			DeleteObject(hBrush);
		};/* end of if */
	} 
	else
		/* disposal method #3 */
		if (m_hDispMemDC && (m_arrFrames[m_nCurrFrame].m_nDisposal == 3) )
		{
			/* put it back */
			BitBlt(m_hMemDC,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
				m_arrFrames[m_nCurrFrame].m_frameSize.cx,
				m_arrFrames[m_nCurrFrame].m_frameSize.cy,
				m_hDispMemDC,0,0, SRCCOPY);
			/* init variables */
			SelectObject(m_hDispMemDC,m_hDispOldBM);
			DeleteDC(m_hDispMemDC); m_hDispMemDC = NULL;
			DeleteObject(m_hDispMemBM); m_hDispMemBM = NULL;
		};/* end of if */
	/* end of if */

	while (!m_bExitThread)
	{
		if (m_arrFrames[m_nCurrFrame].m_pPicture)
		{
			/* Before rendering a frame we should take care of what's  */
			/* behind that frame. TFrame::m_nDisposal will be our guide: */
			/*   0 - no disposal specified (do nothing) */
			/*   1 - do not dispose (again, do nothing) */
			/*   2 - restore to background color (m_clrBackground) */
			/*   3 - restore to previous */
			/* disposal method #3 */
			if (m_arrFrames[m_nCurrFrame].m_nDisposal == 3)
			{
				/* prepare a memory DC and store the background in it */
				m_hDispMemDC = CreateCompatibleDC(m_hMemDC);
				m_hDispMemBM = CreateCompatibleBitmap(m_hMemDC,
							m_arrFrames[m_nCurrFrame].m_frameSize.cx,
							m_arrFrames[m_nCurrFrame].m_frameSize.cy);
				
				if (m_hDispMemDC && m_hDispMemBM)
				{
					m_hDispOldBM = reinterpret_cast<HBITMAP> (SelectObject(m_hDispMemDC,m_hDispMemBM));
					BitBlt(m_hDispMemDC,0,0,
						m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameSize.cy,
						m_hMemDC,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						SRCCOPY);
				}; /* end of if */
			};/* end of if */

			long hmWidth;
			long hmHeight;
			m_arrFrames[m_nCurrFrame].m_pPicture->get_Width(&hmWidth);
			m_arrFrames[m_nCurrFrame].m_pPicture->get_Height(&hmHeight);

			if (m_arrFrames[m_nCurrFrame].m_pPicture->Render(m_hMemDC, 
				m_arrFrames[m_nCurrFrame].m_frameOffset.cx, 
				m_arrFrames[m_nCurrFrame].m_frameOffset.cy, 
				m_arrFrames[m_nCurrFrame].m_frameSize.cx, 
				m_arrFrames[m_nCurrFrame].m_frameSize.cy, 
				0, hmHeight, hmWidth, -hmHeight, NULL) == S_OK)
			{
				Invalidate(FALSE);
			};/* end of if */
			
			if (m_bExitThread) break;
			/* end of if */

			/* if the delay time is too short (like in old GIFs), wait for 100ms */
			if (m_arrFrames[m_nCurrFrame].m_nDelay < 5) 
				WaitForSingleObject(m_hExitEvent, 100);
			else
				WaitForSingleObject(m_hExitEvent, 10*m_arrFrames[m_nCurrFrame].m_nDelay);
			/* end of if */

			if (m_bExitThread) break;
			/* end of if */

			/* disposal method #2 */
			if (m_arrFrames[m_nCurrFrame].m_nDisposal == 2)
			{
				HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
				if (hBrush)
				{
					RECT rect = {
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx + m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy + m_arrFrames[m_nCurrFrame].m_frameSize.cy };
					FillRect(m_hMemDC,&rect,hBrush);
					DeleteObject(hBrush);
				};/* end of if */
			} 
			else
				if (m_hDispMemDC && (m_arrFrames[m_nCurrFrame].m_nDisposal == 3) )
				{
					/* put it back */
					BitBlt(m_hMemDC,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cx,
						m_arrFrames[m_nCurrFrame].m_frameOffset.cy,
						m_arrFrames[m_nCurrFrame].m_frameSize.cx,
						m_arrFrames[m_nCurrFrame].m_frameSize.cy,
						m_hDispMemDC,0,0, SRCCOPY);
					/* init variables */
					SelectObject(m_hDispMemDC,m_hDispOldBM);
					DeleteDC(m_hDispMemDC); m_hDispMemDC = NULL;
					DeleteObject(m_hDispMemBM); m_hDispMemBM = NULL;
				};/* end of if */
			/* end of if */
		};/* end of if */
		m_nCurrFrame++;
		if (m_nCurrFrame == m_arrFrames.size())
		{
			m_nCurrFrame
				= 0; 
			/* init the screen for the first frame, */
			HBRUSH hBrush = CreateSolidBrush(m_clrBackground);
			if (hBrush)
			{
				RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
				FillRect(m_hMemDC,&rect,hBrush);
				DeleteObject(hBrush);
			};/* end of if */
		};/* end of if */
	};/* end of while */

} /* end of ThreadAnimation */


/* Stop */
void CThemePicture::Stop()
{
	m_bIsPlaying = FALSE;
	m_bExitThread = TRUE;
	SetEvent(m_hExitEvent);
	if (m_hThread)
	{
		/* we'll wait for 5 seconds then continue execution */
		WaitForSingleObject(m_hThread,5000);
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}/* end of if */

	/* make it possible to Draw() again */
	ResetEvent(m_hExitEvent);
	m_bExitThread = FALSE;
} /* Stop */


/*GetNextGraphicBlock*/
HGLOBAL CThemePicture::GetNextGraphicBlock(UINT *pBlockLen, 
	UINT *pDelay, SIZE *pBlockSize, SIZE *pBlockOffset, 
	UINT *pDisposal)
{
	if (!m_pRawData) return NULL;
	/* end of if */

	*pDisposal = 0;
	enum GIFBlockTypes nBlock;
	nBlock = GetNextBlock();

	while (
		(nBlock != BLOCK_CONTROLEXT) &&
		(nBlock != BLOCK_IMAGE) &&
		(nBlock != BLOCK_PLAINTEXT) &&
		(nBlock != BLOCK_UNKNOWN) &&
		(nBlock != BLOCK_TRAILER)
		)
	{
		if (!SkipNextBlock()) return NULL;
		nBlock = GetNextBlock();
	};/* end of while  */

	if ((nBlock == BLOCK_UNKNOWN) ||
		(nBlock == BLOCK_TRAILER))
		return NULL;
	/* end of if */

	/* it's either a control ext.block, an image or a plain text */
	int nStart = m_nCurrOffset;
	int nBlockLen = GetNextBlockLen();

	if (nBlockLen <= 0) return NULL;
	/* end of if */

	if (nBlock == BLOCK_CONTROLEXT)
	{
		/* get the following data */
		TGIFControlExt *pControl = reinterpret_cast<TGIFControlExt *> (&m_pRawData[m_nCurrOffset]);
		/* store delay time */
		*pDelay = pControl->m_wDelayTime;
		/* store disposal method */
		*pDisposal = pControl->GetPackedValue(GCX_PACKED_DISPOSAL);

		if (!SkipNextBlock()) return NULL;
		/* end of if */
		nBlock = GetNextBlock();
		
		/* skip everything until we find data to display  */
		/* (image block or plain-text block) */
		
		while (
			(nBlock != BLOCK_IMAGE) &&
			(nBlock != BLOCK_PLAINTEXT) &&
			(nBlock != BLOCK_UNKNOWN) &&
			(nBlock != BLOCK_TRAILER)
			)
		{
			if (!SkipNextBlock()) return NULL;
			nBlock = GetNextBlock();
			nBlockLen += GetNextBlockLen();
		}; /* end of while  */

		if ((nBlock == BLOCK_UNKNOWN) || (nBlock == BLOCK_TRAILER))	return NULL;
		/* end of if */
		nBlockLen += GetNextBlockLen();
	}
	else
		*pDelay = -1;
	/* end of if */

	if (nBlock == BLOCK_IMAGE)
	{
		/* store size and offsets */
		TGIFImageDescriptor *pImage = 
			reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
		pBlockSize->cx = pImage->m_wWidth;
		pBlockSize->cy = pImage->m_wHeight;
		pBlockOffset->cx = pImage->m_wLeftPos;
		pBlockOffset->cy = pImage->m_wTopPos;
	};/* end of if */

	if (!SkipNextBlock()) return NULL;
	/* end of if */

	HGLOBAL hGlobal = GlobalAlloc(GMEM_FIXED,
		sizeof(TGIFHeader) +
		sizeof(TGIFLSDescriptor) +
		m_nGlobalCTSize +
		nBlockLen + 
		1);

	if (!hGlobal) return NULL;
	/* end of if */

	int nOffset = 0; 

	/* GMEM_FIXED means we get a pointer */
	unsigned char *pGlobal = reinterpret_cast<unsigned char *> (hGlobal);

	CopyMemory(pGlobal,m_pRawData, 
		sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize);
	nOffset += sizeof(TGIFHeader)+sizeof(TGIFLSDescriptor)+m_nGlobalCTSize;

	CopyMemory(pGlobal + nOffset,&m_pRawData[nStart], nBlockLen);
	nOffset += nBlockLen;

	pGlobal[nOffset] = 0x3B;
	nOffset++;

	*pBlockLen = nOffset;

	return hGlobal;
} /* end of GetNextGraphicBlock */


/*IsGIF*/
BOOL CThemePicture::IsGIF() const
{
	return m_bIsGIF;
} /* end of IsGIF*/


/*IsAnimatedGIF*/
BOOL CThemePicture::IsAnimatedGIF() const
{
	return (m_bIsGIF && (m_arrFrames.size() > 1));
} /* end of IsAnimatedGIF*/


/*IsPlaying*/
BOOL CThemePicture::IsPlaying() const
{
	return m_bIsPlaying;
} /* end of IsPlaying*/


/* GetFrameCount */
int CThemePicture::GetFrameCount() const
{
	if (!IsAnimatedGIF())	return 0;
	/* end of if */

	return m_arrFrames.size();
} /* end of GetFrameCount*/


/* GetBkColor (white) */
COLORREF CThemePicture::GetBkColor() const
{
	return m_clrBackground;
} /* end of GetBkColor */


/*SetBkColor*/
void CThemePicture::SetBackColor(COLORREF clr)
{
	if (!m_bIsInitialized) return;
	/* end of if */

	m_clrBackground = clr;

	HBRUSH hBrush = CreateSolidBrush(clr);
	if (hBrush)
	{
		RECT rect = {0,0,m_PictureSize.cx,m_PictureSize.cy};
		FillRect(m_hMemDC,&rect,hBrush);
		DeleteObject(hBrush);
	};/* end of if */
} /* end of SetBkColor*/

#ifdef GIF_TRACING
/*WriteDataOnDisk*/
void CThemePicture::WriteDataOnDisk(CString szFileName, HGLOBAL hData, DWORD dwSize)
{
	CFile file;

	if (!file.Open(szFileName,
			CFile::modeCreate |
			CFile::modeWrite |
			CFile::shareDenyNone))
	{
		TRACE(_T("WriteData: Error creating file %s\n"),szFileName);
		return;
	};/* end of if */

	char *pData = reinterpret_cast<char *> (GlobalLock(hData));
	if (!pData)
	{
		TRACE(_T("WriteData: Error locking memory\n"));
		return;
	};/* end of if */

	TRY
	{
		file.Write(pData,dwSize);
	}
	CATCH(CFileException, e);                                          
	{
		TRACE(_T("WriteData: An exception occured while writing to the file %s\n"),
			szFileName);
		e->Delete();
		GlobalUnlock(hData);
		file.Close();
		return;
	}
	END_CATCH
	/* end of try~catch */

	GlobalUnlock(hData);
	file.Close();
} /* end of WriteDataOnDisk*/


/*EnumGIFBlocks*/
void CThemePicture::EnumGIFBlocks()
{
	enum GIFBlockTypes nBlock;

	ResetDataPointer();
	while(m_nCurrOffset < m_nDataSize)
	{
		nBlock = GetNextBlock();
		switch(nBlock)
		{
		case BLOCK_UNKNOWN:
			TRACE(_T("- Unknown block\n"));
			return;
			break;

		case BLOCK_TRAILER:
			TRACE(_T("- Trailer block\n"));
			break;

		case BLOCK_APPEXT:
			TRACE(_T("- Application extension block\n"));
			break;

		case BLOCK_COMMEXT:
			TRACE(_T("- Comment extension block\n"));
			break;

		case BLOCK_CONTROLEXT:
			{
			TGIFControlExt *pControl = 
				reinterpret_cast<TGIFControlExt *> (&m_pRawData[m_nCurrOffset]);
			TRACE(_T("- Graphic control extension block (delay %d, disposal %d)\n"),
					pControl->m_wDelayTime, pControl->GetPackedValue(GCX_PACKED_DISPOSAL));
			};
			break;

		case BLOCK_PLAINTEXT:
			TRACE(_T("- Plain text extension block\n"));
			break;

		case BLOCK_IMAGE:
			TGIFImageDescriptor *pIDescr = 
				reinterpret_cast<TGIFImageDescriptor *> (&m_pRawData[m_nCurrOffset]);
			TRACE(_T("- Image data block (%dx%d  %d,%d)\n"),
					pIDescr->m_wWidth,
					pIDescr->m_wHeight,
					pIDescr->m_wLeftPos,
					pIDescr->m_wTopPos);
			break;
		};/* end of switch  */

		SkipNextBlock();	
	};/* end of whlie  */

	TRACE(_T("\n"));
} /* end of EnumGIFBlocks */
#endif /* GIF_TRACING */


/*SetPaintRect*/
BOOL CThemePicture::SetPaintRect(const RECT *lpRect)
{
	return CopyRect(&m_PaintRect, lpRect);
} /* end of SetPaintRect */


/* GetPaintRect */
BOOL CThemePicture::GetPaintRect(RECT *lpRect)
{
	return CopyRect(lpRect, &m_PaintRect);
} /* end of SetPaintRect */

/*OnDestroy*/
void CThemePicture::OnDestroy() 
{	
	// 툴팁 삭제
	DeleteToolTip();

	Stop();	
	CWnd::OnDestroy();
} /* end of OnDestroy*/

// 클래스 생성시 실행되는 함수
int CThemePicture::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Text 관련 변수 설정
	/*m_stThemeInfo.strCaption.Format("%s", lpCreateStruct->lpszName);
	m_stThemeInfo.rCaption.left = lpCreateStruct->x;
	m_stThemeInfo.rCaption.top = lpCreateStruct->y;
	m_stThemeInfo.rCaption.right = lpCreateStruct->cx + m_stThemeInfo.rCaption.left;
	m_stThemeInfo.rCaption.bottom = lpCreateStruct->cy + m_stThemeInfo.rCaption.top;*/
	
	return 0;
}

// Caption 문자열을 화면에 표시한다
void CThemePicture::DrawCaptionText(CPaintDC *pdc)
{
	CRect rCaption, rShadow;

	CString strText[50], strTemp;
	int nLineEnd=0, nLineCount=0, i=0;
	CRect rText[50];

	// Caption 문자열을 구한다
	strTemp = m_stThemeInfo.strCaption;
	
	// 캡션 텍스트가 여러줄 일경우에 각 라인을 분할
	do {
		if( (nLineEnd = strTemp.Find(TEXT("\n"))) >= 0 )
		{
			strText[nLineCount] = strTemp.Left(nLineEnd);
			strTemp = strTemp.Mid(nLineEnd+1);
		}
		else if( (nLineEnd = strTemp.Find(TEXT("\\n"))) >= 0 )
		{
			strText[nLineCount] = strTemp.Left(nLineEnd);
			strTemp = strTemp.Mid(nLineEnd+2);
		}
		else
		{
			strText[nLineCount] = strTemp;
			strTemp = "";
		}

		nLineCount ++;
		
	} while( strTemp.GetLength() > 0 );

	if( nLineCount < 1 )
		return;

	double fTextAreaHeight=0.;

	// Caption 영역좌표를 절대 영역좌표로 변환
	rCaption = gf_RelativeToAbsolute(m_stThemeInfo.rCaption, this);
	if( rCaption.right <= 0 || rCaption.bottom <= 0 )
		this->GetClientRect(&rCaption);
	// 문자열 한줄의 높이
	fTextAreaHeight = (double)rCaption.Height() / (double)nLineCount;
	
	// 각 문자열 라인을 표시할 영역좌표 계산
	rText[0] = rCaption;
	//rText[0].top = rCaption.top + ((rCaption.Height() - (nTextAreaHeight * nLineCount)) / 2);
	rText[0].bottom = rText[0].top + (int)fTextAreaHeight;

	for(i=1 ; i < nLineCount ; i++)
	{
		rText[i] = rCaption;
		rText[i].top = rText[0].top + (int)(fTextAreaHeight * (double)i);
		rText[i].bottom = rText[i].top + (int)fTextAreaHeight;
	}

	// Caption 문자를 화면에 표시
	for(i=0 ; i < nLineCount ; i++)
	{
		// Caption 그림자 컬러가 투명이 아니라면
		if( m_stThemeInfo.crShadow != dfCOLOR_TRANSPARENT )
		{			
			// 그림자 영역좌표를 구한다
			rShadow = rText[i];
			rShadow.left ++;
			rShadow.top ++;
			rShadow.right ++;
			rShadow.bottom ++;

			// Caption 그림자를 화면에 표시
			gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crShadow, rShadow, 
				m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
		}

		// Caption 텍스트를 화면에 표시
		gf_DrawText(pdc, strText[i], m_stThemeInfo.nFontSize, 
				m_stThemeInfo.crFont, rText[i], 
				m_stThemeInfo.nAlignFormat|DT_SINGLELINE, m_stThemeInfo.bFontBold);
	}
}

// Caption 영역좌표 설정
void CThemePicture::SetCaptionArea(CRect rCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.rCaption = rCaption;
}

// Caption 정렬 방식 설정
void CThemePicture::SetAlignFormat(UINT nAlignFormat)
{
	// 전역변수에 저장
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 폰트 컬러 설정
void CThemePicture::SetFontColor(COLORREF crFont)
{
	m_stThemeInfo.crFont = crFont;
}

// 그림자 컬러 설정
void CThemePicture::SetShadowColor(COLORREF crShadow)
{
	m_stThemeInfo.crShadow = crShadow;
}

// 폰트 컬러 설정
void CThemePicture::SetFontColor(COLORREF crFont, COLORREF crShadow)
{
	m_stThemeInfo.crFont = crFont;
	m_stThemeInfo.crShadow = crShadow;
}

// 폰트 크기 설정
void CThemePicture::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 폰트 두께 설정
void CThemePicture::SetFontBold(BOOL bBold)
{
	m_stThemeInfo.bFontBold = bBold;
}

// 폰트 특성 설정
void CThemePicture::SetFont(int nFontSize, BOOL bFontBold, COLORREF crFont, COLORREF crShadow, UINT nAlignFormat)
{
	// 폰트 특성을 전역변수에 저장
	m_stThemeInfo.nFontSize = nFontSize;
	m_stThemeInfo.bFontBold = bFontBold;
	m_stThemeInfo.crFont = crFont;
	m_stThemeInfo.crShadow = crShadow;
	m_stThemeInfo.nAlignFormat = nAlignFormat;
}

// 툴팁 문자열 설정
void CThemePicture::SetToolTip(CString strText)
{
	// 툴팁이 이미 생성되어 있다면
	if( m_ToolTip.GetSafeHwnd() )
		// 툴팁 문자열 변경
		ChangeToolTip(strText);
	// 툴팁이 아직 생성되지 않았다면
	else
		// 툴팁 생성
		MakeToolTip(strText);
}

// 툴팁을 생성한다
void CThemePicture::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemePicture::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 컨트롤 화면 크기 변경시 실행되는 함수
void CThemePicture::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	int nLimitSize=8;			// 최소 크기

	if( cx < nLimitSize || cy < nLimitSize )
		return;
	
	// 화면을 새로 그린다
	Invalidate(TRUE);

	// 툴팁 변경
	if( m_ToolTip.GetSafeHwnd() )
	{
		CString strToolTip;
		// 현재 툴팁 캡션을 구한다
		strToolTip = m_ToolTip.GetToolTipText();
		// 툴팁 캡션 변경함수를 호출
		ChangeToolTip(strToolTip);
	}	
}

// 툴팁 삭제
void CThemePicture::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 배경 이미지 설정
void CThemePicture::SetBackImage(CString strFileName)
{
	// 이미지 로딩에 성공했다면 이미지를 그린다
	if( Load(strFileName) )
		Draw();
}

/*OnPaint*/
void CThemePicture::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	/*LONG nPaintWidth = m_PaintRect.right-m_PaintRect.left;

	if (nPaintWidth > 0)
	{
		LONG nPaintHeight = m_PaintRect.bottom - m_PaintRect.top;
		::BitBlt(dc.m_hDC, 0, 0, nPaintWidth, nPaintHeight,	
			m_hMemDC, m_PaintRect.left, m_PaintRect.top, SRCCOPY);
	}
	else
	{
		::BitBlt(dc.m_hDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy,
			m_hMemDC, 0, 0, SRCCOPY);
	};/* end of if */

	// 배경 이미지를 그린다
	DrawBackImage(&dc);

	// Caption 문자열을 화면에 표시한다
	DrawCaptionText(&dc);

} /* end of OnPaint */

// 배경 이미지를 그린다
void CThemePicture::DrawBackImage(CPaintDC *pdc)
{
	CRect rScreen;
	GetClientRect(&rScreen);

	//pdc->FillRect(rScreen, &CBrush(RGB(0,128,255)));

	// '배경 이미지 채우기 방식'에 따라서
	switch( m_stThemeInfo.nBackImageFillType )
	{
	case dfBACK_IMAGE_FILL_TYPE_BLOCK :				// 블럭
		// 이미지를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수
		DrawImageResizeBlock(pdc, m_hMemDC, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_RESIZE :			// 늘이기
		// 이미지 크기 변경하여 그리는 함수
		DrawImageResize(pdc, m_hMemDC, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_PATTERN :			// 바둑판식
		// 이미지를 Pattern 형식으로 화면 영역에 그리는 함수(바둑판 형식)
		DrawImageByPattern(pdc, m_hMemDC, rScreen);
		break;
	case dfBACK_IMAGE_FILL_TYPE_ONCE :				// 1회
		// 이미지를 화면에 그리는 함수(원본 그대로 1회)
		DrawImageOnce(pdc, m_hMemDC, rScreen, m_stThemeInfo.nBackImageAlignFormat);
		break;
	default :
		break;
	}
		//::StretchBlt(pdc->m_hDC, 0, 0, 70, 70,
		//	m_hMemDC, 0, 0, 35, 35, SRCCOPY);
		//::BitBlt(pdc->m_hDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy,
		//	m_hMemDC, 0, 0, SRCCOPY);
}

// 이미지를 Pattern 형식으로 화면 영역에 그리는 함수(바둑판 형식)
void CThemePicture::DrawImageByPattern(CPaintDC *pdc, HDC hMemDC, CRect rScreen)
{
	int nImgCx=0, nImgCy=0;
	CRect rPart;

	// 이미지 크기
	nImgCx = m_PictureSize.cx;
	nImgCy = m_PictureSize.cy;

	// 부분 영역 top/bottom 초기화
	rPart.top = 0;
	rPart.bottom = nImgCy;

	while( rPart.top < rScreen.bottom )
	{
		// 부분 영역 left/right
		rPart.left = 0;
		rPart.right = nImgCx;

		if( rPart.bottom > rScreen.bottom )
			rPart.bottom = rScreen.bottom;
		
		while( rPart.left < rScreen.right )
		{
			if( rPart.right > rScreen.right )
				rPart.right = rScreen.right;

			// 이미지를 화면에 그린다
			::BitBlt(pdc->m_hDC, rPart.left, rPart.top, rPart.Width(), rPart.Height(),
				m_hMemDC, 0, 0, SRCCOPY);

			rPart.left += nImgCx;
			rPart.right += nImgCx;
		}

		rPart.top += nImgCy;
		rPart.bottom += nImgCy;
	}

}

// 이미지 크기 변경하여 그리는 함수
void CThemePicture::DrawImageResize(CPaintDC *pdc, HDC hMemDC, CRect rScreen)
{
	::StretchBlt(pdc->m_hDC, 0, 0, rScreen.Width(), rScreen.Height(),
			m_hMemDC, 0, 0, m_PictureSize.cx, m_PictureSize.cy, SRCCOPY);
}

// 이미지를 화면에 그리는 함수(원본 그대로 1회)
void CThemePicture::DrawImageOnce(CPaintDC *pdc, HDC hMemDC, CRect rScreen, UINT unAlignFormat)
{
	CRect rImgPart, rImgView;

	// 이미지 영역좌표
	rImgPart.left = 0;
	rImgPart.top = 0;
	rImgPart.right = m_PictureSize.cx;
	rImgPart.bottom = m_PictureSize.cy;

	// 이미지를 그릴 화면 영역좌표
	rImgView = rImgPart;
	// 이미지 정렬 방식 - Right 일때
	if( (unAlignFormat & DT_RIGHT) == DT_RIGHT )
	{
		rImgView.right = rScreen.right;
		rImgView.left = rImgView.right - rImgPart.Width();
	}
	// 이미지 정렬 방식 - Center 일때
	else if( (unAlignFormat & DT_CENTER) == DT_CENTER )
	{
		rImgView.left = rScreen.CenterPoint().x - (rImgPart.Width()/2);
		rImgView.right = rScreen.CenterPoint().x + (rImgPart.Width()/2);
	}
	// 이미지 정렬 방식 - Left 일때
	else if( (unAlignFormat & DT_LEFT) == DT_LEFT )
	{
		rImgView.left = 0;
		rImgView.right = rImgPart.Width();
	}
	// 이미지 정렬 방식 - Bottom 일때
	if( (unAlignFormat & DT_BOTTOM) == DT_BOTTOM )
	{
		rImgView.bottom = rScreen.bottom;
		rImgView.top = rImgView.bottom - rImgPart.Height();
	}
	// 이미지 정렬 방식 - VCenter 일때
	else if( (unAlignFormat & DT_VCENTER) == DT_VCENTER )
	{
		rImgView.top = rScreen.CenterPoint().y - (rImgPart.Height()/2);
		rImgView.bottom = rScreen.CenterPoint().y + (rImgPart.Height()/2);
	}	
	// 이미지 정렬 방식 - Top 일때
	else if( (unAlignFormat & DT_TOP) == DT_TOP )
	{
		rImgView.top = 0;
		rImgView.bottom = rImgPart.Height();
	}

	// 이미지를 화면에 그린다
	::BitBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, 0, 0, SRCCOPY);
}

// 이미지를 9조각으로 분할, 크기 변경하여 화면에 표시하는 함수
void CThemePicture::DrawImageResizeBlock(CPaintDC *pdc, HDC hMemDC, CRect rScreen)
{
	int nImgCx=0, nImgCy=0;
	//CRect rPart;

	// 이미지 크기
	nImgCx = m_PictureSize.cx;
	nImgCy = m_PictureSize.cy;

	int nBlockWidth1=0, nBlockWidth3=0, nBlockWidth2=0, nBlockHeight1=0, nBlockHeight2=0, nBlockHeight3=0;
	int nHorizonBlock[4] = {0,50,150,200}, nVerticalBlock[4] = {0,50,150,200};
	int nGapX=50, nGapY=50;
	//CRect rImage;

	/*BITMAP bm;
    pBitmap->GetBitmap(&bm);
	
	// 이미지 크기	
	rImage.left = 0;
	rImage.top = 0;
	rImage.right  = bm.bmWidth;
	rImage.bottom = bm.bmHeight;*/

	// 이미지를 자를 블럭 수치
	
	if( nImgCx-2 <= nGapX * 2 )
		nGapX = nImgCx / 3;

	if( nImgCy-2 <= nGapY * 2 )
		nGapY = nImgCy / 3;

	nHorizonBlock[1] = nGapX;
	nHorizonBlock[3] = nImgCx;
	nHorizonBlock[2] = nImgCx - nGapX;
	nVerticalBlock[1] = nGapY;
	nVerticalBlock[3] = nImgCy;
	nVerticalBlock[2] = nImgCy - nGapY;
	
	// 이미지의 가운데 블럭 크기를 구한다
	nBlockWidth1 = nHorizonBlock[1] - nHorizonBlock[0];
	nBlockWidth3 = nHorizonBlock[3] - nHorizonBlock[2];
	nBlockWidth2 = nImgCx - nBlockWidth1 - nBlockWidth3;
	nBlockHeight1 = nVerticalBlock[1] - nVerticalBlock[0];
	nBlockHeight3 = nVerticalBlock[3] - nVerticalBlock[2];
	nBlockHeight2 = nImgCy - nBlockHeight1 - nBlockHeight3;
	// 화면 영역 가운데 블럭의 크기를 구한다
	int nScrBlockWidth2 = rScreen.Width() - nBlockWidth1 - nBlockWidth3;
	int nScrBlockHeigh2 = rScreen.Height() - nBlockHeight1 - nBlockHeight3;

	CRect rImgPart, rImgView;

	// Mid-Mid 이미지 화면에 표시
	rImgView.left = rScreen.left+nBlockWidth1;
	rImgView.top = rScreen.top+nBlockHeight1;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgPart.left = nBlockWidth1;
	rImgPart.top = nBlockHeight1;
	rImgPart.right = rImgPart.left + nBlockWidth2;
	rImgPart.bottom = rImgPart.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Mid-Top 이미지 화면에 표시
	rImgView.left = rScreen.left+nBlockWidth1;
	rImgView.top = rScreen.top;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgPart.left = nBlockWidth1;
	rImgPart.top = 0;
	rImgPart.right = rImgPart.left + nBlockWidth2;
	rImgPart.bottom = rImgPart.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Mid-Bottom 이미지 화면에 표시
	rImgView.left = rScreen.left+nBlockWidth1;
	rImgView.top = rScreen.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nScrBlockWidth2;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgPart.left = nBlockWidth1;
	rImgPart.top = nImgCy-nBlockHeight3;
	rImgPart.right = rImgPart.left + nBlockWidth2;
	rImgPart.bottom = rImgPart.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Left-Mid 이미지 화면에 표시
	rImgView.left = rScreen.left;
	rImgView.top = rScreen.top+nBlockHeight1;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgPart.left = 0;
	rImgPart.top = nBlockHeight1;
	rImgPart.right = rImgPart.left + nBlockWidth1;
	rImgPart.bottom = rImgPart.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Right-Mid 이미지 화면에 표시
	rImgView.left = rScreen.right-nBlockWidth3;
	rImgView.top = rScreen.top+nBlockHeight1;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nScrBlockHeigh2;
	rImgPart.left = nImgCx-nBlockWidth3;
	rImgPart.top = nBlockHeight1;
	rImgPart.right = rImgPart.left + nBlockWidth3;
	rImgPart.bottom = rImgPart.top + nBlockHeight2;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Left-Top 이미지 화면에 표시
	rImgView.left = rScreen.left;
	rImgView.top = rScreen.top;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgPart.left = 0;
	rImgPart.top = 0;
	rImgPart.right = rImgPart.left + nBlockWidth1;
	rImgPart.bottom = rImgPart.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Right-Top 이미지 화면에 표시
	rImgView.left = rScreen.right-nBlockWidth3;
	rImgView.top = rScreen.top;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nBlockHeight1;
	rImgPart.left = nImgCx-nBlockWidth3;
	rImgPart.top = 0;
	rImgPart.right = rImgPart.left + nBlockWidth3;
	rImgPart.bottom = rImgPart.top + nBlockHeight1;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Left-Bottom 이미지 화면에 표시
	rImgView.left = rScreen.left;
	rImgView.top = rScreen.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nBlockWidth1;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgPart.left = 0;
	rImgPart.top = nImgCy-nBlockHeight3;
	rImgPart.right = rImgPart.left + nBlockWidth1;
	rImgPart.bottom = rImgPart.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);

	// Right-Bottom 이미지 화면에 표시
	rImgView.left = rScreen.right-nBlockWidth3;
	rImgView.top = rScreen.bottom-nBlockHeight3;
	rImgView.right = rImgView.left + nBlockWidth3;
	rImgView.bottom = rImgView.top + nBlockHeight3;
	rImgPart.left = nImgCx-nBlockWidth3;
	rImgPart.top = nImgCy-nBlockHeight3;
	rImgPart.right = rImgPart.left + nBlockWidth3;
	rImgPart.bottom = rImgPart.top + nBlockHeight3;
	// 이미지 일부분을 크기 변경하여 그리는 함수
	::StretchBlt(pdc->m_hDC, rImgView.left, rImgView.top, rImgView.Width(), rImgView.Height(),
		m_hMemDC, rImgPart.left, rImgPart.top, rImgPart.Width(), rImgPart.Height(), SRCCOPY);
}

// 클래스 생성
BOOL CThemePicture::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID, pContext);
}

// 배경 이미지 채우기 방식 설정
void CThemePicture::SetBackImageFillType(int nBackImageFillType)
{
	m_stThemeInfo.nBackImageFillType = nBackImageFillType;
}

// 배경 이미지 정렬 방식 설정
void CThemePicture::SetBackImageAlignFormat(UINT nAlignFormat)
{
	m_stThemeInfo.nBackImageAlignFormat = nAlignFormat;
}

// Caption 문자열 설정
void CThemePicture::SetWindowText(CString strText)
{
	SetCaptionText(strText);
}

// Caption 문자열 설정
void CThemePicture::SetCaptionText(CString strCaption)
{
	// 전역변수에 저장
	m_stThemeInfo.strCaption = strCaption;
	
	// 화면을 새로 그린다
	Invalidate(FALSE);
}
