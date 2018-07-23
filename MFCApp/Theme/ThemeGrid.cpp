// ThemeGrid.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "ThemeGrid.h"
#include "BaseEdit.h"
#include "BaseCombo.h"

// OLE stuff for clipboard operations
#include <afxadv.h>            // For CSharedFile
#include <afxconv.h>           // For LPTSTR -> LPSTR macros

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define HEADER_HEIGHT       2    // For printing
#define FOOTER_HEIGHT       2
#define LEFT_MARGIN         4
#define RIGHT_MARGIN        4
#define TOP_MARGIN          1
#define BOTTOM_MARGIN       1 
#define GAP                 1

#define SELECTED_CELL_FONT_WEIGHT 600    // weight of text for selected items


IMPLEMENT_DYNCREATE(CThemeGrid, CWnd)

void AFXAPI DDX_GridControl(CDataExchange* pDX, int nIDC, CThemeGrid& rControl)
{
    if (rControl.GetSafeHwnd() == NULL)    // not subclassed yet
    {
        ASSERT(!pDX->m_bSaveAndValidate);

        HWND hWndCtrl = pDX->PrepareCtrl(nIDC);

        if (!rControl.SubclassWindow(hWndCtrl))
        {
            ASSERT(FALSE);      // possibly trying to subclass twice?
            AfxThrowNotSupportedException();
        }
#ifndef _AFX_NO_OCC_SUPPORT
        else
        {
            // If the control has reparented itself (e.g., invisible control),
            // make sure that the CWnd gets properly wired to its control site.
            if (pDX->m_pDlgWnd->GetSafeHwnd() != ::GetParent(rControl.GetSafeHwnd()))
                rControl.AttachControlSite(pDX->m_pDlgWnd);
        }
#endif //!_AFX_NO_OCC_SUPPORT

    }
}

// Get the number of lines to scroll with each mouse wheel notch
// Why doesn't windows give us this function???
UINT GetMouseScrollLines()
{
    int nScrollLines = 3;            // reasonable default
    HKEY hKey;

    if (RegOpenKeyEx(HKEY_CURRENT_USER,  _T("Control Panel\\Desktop"),
                     0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        TCHAR szData[128];
        DWORD dwKeyDataType;
        DWORD dwDataBufSize = sizeof(szData);

        if (RegQueryValueEx(hKey, _T("WheelScrollLines"), NULL, &dwKeyDataType,
                           (LPBYTE) &szData, &dwDataBufSize) == ERROR_SUCCESS)
        {
            nScrollLines = _tcstoul(szData, NULL, 10);
        }
        RegCloseKey(hKey);
    }

    return nScrollLines;
}

/////////////////////////////////////////////////////////////////////////////
// CThemeGrid

/*CThemeGrid::CThemeGrid()
{
}*/

CThemeGrid::CThemeGrid(int nRows, int nCols, int nFixedRows, int nFixedCols)
{
    RegisterWindowClass();

    // Initialize OLE libraries
    m_bMustUninitOLE = FALSE;

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    _AFX_THREAD_STATE* pState = AfxGetThreadState();
    if (!pState->m_bNeedTerm)
    {
        SCODE sc = ::OleInitialize(NULL);
        if (FAILED(sc))
            AfxMessageBox(_T("OLE initialization failed. Make sure that the OLE libraries are the correct version"));
        else
            m_bMustUninitOLE = TRUE;
    }
#endif

    // Store the system colours in case they change. The gridctrl uses
    // these colours, and in OnSysColorChange we can check to see if 
    // the gridctrl colours have been changed from the system colours.
    // If they have, then leave them, otherwise change them to reflect
    // the new system colours.
	
    m_nRows              = 0;
    m_nCols              = 0;
    m_nFixedRows         = 0;
    m_nFixedCols         = 0;

    m_nDefCellHeight     = 10;        // These will get changed to something meaningful
    m_nDefCellWidth      = 30;        //    when the window is created or subclassed

    m_nVScrollMax        = 0;         // Scroll position
    m_nHScrollMax        = 0;
    m_nMargin            = 0;         // cell padding
    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
                                                  // per mouse wheel notch to scroll
	m_nAlignFormat       = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
    m_MouseMode          = MOUSE_NOTHING;
    m_nGridLines         = GVL_BOTH;
    m_bEditable          = TRUE;
	m_bEditDblClk        = TRUE;
    m_bListMode          = FALSE;
	m_bSingleRowSelection = FALSE;
    m_bAllowDraw         = TRUE;      // allow draw updates
    m_bEnableSelection   = TRUE;
    m_bAllowRowResize    = TRUE;
    m_bAllowColumnResize = TRUE;
    m_bSortOnClick       = FALSE;      // Sort on header row click if in list mode
    m_bHandleTabKey      = TRUE;
#ifdef _WIN32_WCE
    m_bDoubleBuffer      = FALSE;     // Use double buffering to avoid flicker?
#else
    m_bDoubleBuffer      = TRUE;      // Use double buffering to avoid flicker?
#endif
    //m_bTitleTips         = TRUE;      // show cell title tips
	
    m_bAscending         = TRUE;      // sorting stuff
    m_SortColumn         = -1;

    m_nTimerID           = 0;         // For drag-selection
    m_nTimerInterval     = 25;        // (in milliseconds)
    m_nResizeCaptureRange = 3;        // When resizing columns/row, the cursor has to be 
                                      // within +/-3 pixels of the dividing line for 
                                      // resizing to be possible
	// CheckBox
	//m_bCheckBoxImageSettled = FALSE;	// 체크박스 이미지 존재 여부
    m_pImageList         = NULL;
    m_bAllowDragAndDrop  = FALSE;     // for drag and drop

#ifndef _WIN32_WCE
    // Initially use the system message font for the GridCtrl font
    NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
	// VC++ .net 일때
	if( ncm.cbSize == 344 )
		ncm.cbSize = 340;
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));    
    memcpy(&m_Logfont, &(ncm.lfMessageFont), sizeof(LOGFONT));
#else
    LOGFONT lf;
    GetObject(GetStockObject(SYSTEM_FONT), sizeof(LOGFONT), &lf);
    memcpy(&m_Logfont, &lf, sizeof(LOGFONT));
#endif
	
    // Set up the initial grid size
    SetRowCount(nRows);
    SetColCount(nCols);
    SetFixedRowCount(nFixedRows);
    SetFixedColCount(nFixedCols);

    // set initial selection range (ie. none)
    m_SelectedCellMap.RemoveAll();
    m_PrevSelectedCellMap.RemoveAll();

	// add SKC -------------------------------------
	m_fpExInfoFun = NULL;
	// end SKC -------------------------------------

	m_stThemeInfo.strIniCtrlName = "";							// INI 파일 조회용 컨트롤명
	m_stThemeInfo.poCell = CPoint(-1,-1);

	// Is Base Control ListCtrl?
	m_bBaseCtrlList = FALSE;

	// 테마 관련 변수 초기화
	InitThemeVariable();
}

// 테마 관련 변수 초기화
void CThemeGrid::InitThemeVariable()
{

	m_nFontSize			= 12;			// 텍스트 폰트 크기
	
    // 고정 항목 텍스트 컬러
	m_crFixedTextColour	= ::GetSysColor(COLOR_WINDOWTEXT);
	// 고정 항목 배경 컬러
	m_crFixedBkColour    = ::GetSysColor(COLOR_3DFACE);
	// 고정 항목 프레임 컬러(Left-Top)
	m_crFixedFrameLT	= ::GetSysColor(COLOR_3DHIGHLIGHT);
	// 고정 항목 프레임 컬러(Right-Bottom)
	m_crFixedFrameRB	= ::GetSysColor(COLOR_3DDKSHADOW);

	// 일반 항목 텍스트 컬러
	m_crTextColour       = ::GetSysColor(COLOR_WINDOWTEXT);
    // 일반 항목 배경 컬러
	m_crTextBkColour = RGB(255, 255, 224);
	// 일반 항목 프레임 컬러
	m_crGridColour       = RGB(192,192,192);
	
	// 배경 컬러
	m_crBkColour		= ::GetSysColor(COLOR_3DSHADOW);
	
	m_stThemeInfo.rInitPos = CRect(-1,-1,-1,-1);				// 초기 위치 정보
	
}

CThemeGrid::~CThemeGrid()
{
	// add SKC ---------------------------
	while (!m_ColInfoList.IsEmpty()) delete m_ColInfoList.RemoveHead();
	while (!m_ColGroupList.IsEmpty()) delete m_ColGroupList.RemoveHead();
	// end SKC ---------------------------

    DeleteAllItems();

    DestroyWindow();

    m_Font.DeleteObject();

#if !defined(GRIDCONTROL_NO_DRAGDROP) || !defined(GRIDCONTROL_NO_CLIPBOARD)
    // Uninitialize OLE support
    if (m_bMustUninitOLE)
        ::OleUninitialize();
#endif
}

// Register the window class if it has not already been registered.
BOOL CThemeGrid::RegisterWindowClass()
{
    WNDCLASS wndcls;
    HINSTANCE hInst = AfxGetInstanceHandle();
   // HINSTANCE hInst = AfxGetResourceHandle(); 

    if (!(::GetClassInfo(hInst, GRIDCTRL_CLASSNAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
        wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = GRIDCTRL_CLASSNAME;

        if (!AfxRegisterClass(&wndcls)) {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

// 클래스 생성 전에 실행되는 함수
BOOL CThemeGrid::PreCreateWindow(CREATESTRUCT& cs) 
{
	// 테두리 표시
	//cs.style |= WS_BORDER;
	
	return CWnd::PreCreateWindow(cs);
}

// 클래스 생성
BOOL CThemeGrid::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle/*=WS_TABSTOP|WS_CHILD|WS_VISIBLE|WS_BORDER*/)
{
    ASSERT(pParentWnd->GetSafeHwnd());

	//if (!CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID)) 
    if (!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID)) 
        return FALSE;

	// 클래스 생성 후에 처리하는 함수
	return PorcessAfterCreate();
}

// 클래스 생성
BOOL CThemeGrid::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
    ASSERT(pParentWnd->GetSafeHwnd());

    //if (!CWnd::Create(NULL, NULL, dwStyle, rect, pParentWnd, nID)) 
    if (!CWnd::Create(GRIDCTRL_CLASSNAME, NULL, dwStyle, rect, pParentWnd, nID)) 
        return FALSE;

	// 클래스 생성 후에 처리하는 함수
	return PorcessAfterCreate();
}

// 클래스 생성 후에 처리하는 함수
BOOL CThemeGrid::PorcessAfterCreate()
{
/*#ifndef GRIDCONTROL_NO_DRAGDROP
    m_DropTarget.Register(this);
#endif*/

/*    // Create titletips
#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_bTitleTips)        
        m_TitleTip.Create(this);
#endif*/

	// 텍스트 폰트를 생성하는 함수
	CreateTextFont(m_nFontSize);

    TRY {
        m_arRowHeights.SetSize(m_nRows);    // initialize row heights
        m_arColWidths.SetSize(m_nCols);     // initialize column widths
    }
    CATCH (CMemoryException, e) {
        e->ReportError();
        e->Delete();
        return FALSE;
    }
    END_CATCH

	int i=0;
    for (i = 0; i < m_nRows; i++) m_arRowHeights[i] = m_nDefCellHeight;
    for (i = 0; i < m_nCols; i++)      m_arColWidths[i] = m_nDefCellWidth;

    ResetScrollBars();

	return TRUE;
}

void CThemeGrid::PreSubclassWindow() 
{    
    CWnd::PreSubclassWindow();

    HFONT hFont = ::CreateFontIndirect(&m_Logfont);
	
    OnSetFont((LPARAM)hFont, 0);
    DeleteObject(hFont);
    
    ResetScrollBars();   
}

BOOL CThemeGrid::SubclassWindow(HWND hWnd) 
{    
    if (!CWnd::SubclassWindow(hWnd))
        return FALSE;

/*#ifndef GRIDCONTROL_NO_DRAGDROP
    m_DropTarget.Register(this);
#endif*/

/*#ifndef GRIDCONTROL_NO_TITLETIPS
    if (m_bTitleTips && !IsWindow(m_TitleTip.m_hWnd))
        m_TitleTip.Create(this);
#endif*/

    return TRUE;
}

LRESULT CThemeGrid::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    if (!IsWindow(m_hWnd))
        return 0;

    NM_GRIDVIEW nmgv;
    nmgv.iRow         = nRow;
    nmgv.iColumn      = nCol;
    nmgv.hdr.hwndFrom = m_hWnd;
    nmgv.hdr.idFrom   = GetDlgCtrlID();
    nmgv.hdr.code     = nMessage;

    CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        return pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
    else
        return 0;
}


BEGIN_MESSAGE_MAP(CThemeGrid, CWnd)
	//{{AFX_MSG_MAP(CThemeGrid)
	ON_WM_PAINT()
    ON_WM_HSCROLL()
    ON_WM_VSCROLL()
    ON_WM_SIZE()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_GETDLGCODE()
    ON_WM_KEYDOWN()
    ON_WM_CHAR()
    ON_WM_LBUTTONDBLCLK()
    ON_WM_ERASEBKGND()
    ON_COMMAND(ID_EDIT_SELECT_ALL, OnEditSelectAll)
    ON_UPDATE_COMMAND_UI(ID_EDIT_SELECT_ALL, OnUpdateEditSelectAll)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
    ON_WM_DESTROY()
	//}}AFX_MSG_MAP
#ifndef _WIN32_WCE_NO_CURSOR
    ON_WM_SETCURSOR()
#endif
#ifndef _WIN32_WCE
    ON_WM_SYSCOLORCHANGE()
    ON_WM_CAPTURECHANGED()
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
    ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
    ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
    ON_COMMAND(ID_EDIT_CUT, OnEditCut)
    ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
    ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
    ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
#endif
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
    ON_WM_MOUSEWHEEL()
#endif
#if (_WIN32_WCE >= 210)
    ON_WM_SETTINGCHANGE()
#endif
    ON_MESSAGE(WM_SETFONT, OnSetFont)
    ON_MESSAGE(WM_GETFONT, OnGetFont)
    ON_NOTIFY(GVN_ENDLABELEDIT, IDC_INPLACE_CONTROL, OnEndInPlaceEdit)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CThemeGrid message handlers

void CThemeGrid::OnPaint() 
{
    CPaintDC dc(this);      // device context for painting

    if (m_bDoubleBuffer)    // Use a memory DC to remove flicker
    {
		CMemoryDC MemDC(&dc);
        OnDraw(&MemDC);
    }
    else                    // Draw raw - this helps in debugging vis problems.
        OnDraw(&dc);
}

BOOL CThemeGrid::OnEraseBkgnd(CDC* /*pDC*/) 
{
    return TRUE;    // Don't erase the background.
}

// Custom background erasure. This gets called from within the OnDraw function,
// since we will (most likely) be using a memory DC to stop flicker. If we just
// erase the background normally through OnEraseBkgnd, and didn't fill the memDC's
// selected bitmap with colour, then all sorts of vis problems would occur
void CThemeGrid::EraseBkgnd(CDC* pDC) 
{
    CRect  VisRect, ClipRect, rect;
    CBrush FixedBack(GetFixedBackColor()),
           TextBack(GetNormalBackColor()),
		   //TextBack(GetNormalItemBackColor()),
           Back(GetBackColor());

    if (pDC->GetClipBox(ClipRect) == ERROR) return;

    GetVisibleNonFixedCellRange(VisRect);

    // Draw Fixed columns background
    int nFixedColumnWidth = GetFixedColWidth();
    if (ClipRect.left < nFixedColumnWidth && ClipRect.top < VisRect.bottom)
        pDC->FillRect(CRect(ClipRect.left, ClipRect.top, nFixedColumnWidth, VisRect.bottom), &FixedBack);
        
    // Draw Fixed rows background
    int nFixedRowHeight = GetFixedRowHeight();
    if (ClipRect.top < nFixedRowHeight && 
        ClipRect.right > nFixedColumnWidth && ClipRect.left < VisRect.right)
        pDC->FillRect(CRect(nFixedColumnWidth-1, ClipRect.top, VisRect.right, nFixedRowHeight), &FixedBack);

    // Draw non-fixed cell background
    if (rect.IntersectRect(VisRect, ClipRect)) 
    {
        CRect CellRect(max(nFixedColumnWidth, rect.left), 
                       max(nFixedRowHeight, rect.top),
                       rect.right, rect.bottom);
        pDC->FillRect(CellRect, &TextBack);
    }

    // Draw right hand side of window outside grid
    if (VisRect.right < ClipRect.right) 
        pDC->FillRect(CRect(VisRect.right, ClipRect.top, ClipRect.right, ClipRect.bottom), &Back);

    // Draw bottom of window below grid
    if (VisRect.bottom < ClipRect.bottom && ClipRect.left < VisRect.right) 
        pDC->FillRect(CRect(ClipRect.left, VisRect.bottom, VisRect.right, ClipRect.bottom), &Back);
}

void CThemeGrid::OnSize(UINT nType, int cx, int cy) 
{
    static BOOL bAlreadyInsideThisProcedure = FALSE;
    if (bAlreadyInsideThisProcedure) return;

    if (!::IsWindow(m_hWnd)) return;

    // Start re-entry blocking
    bAlreadyInsideThisProcedure = TRUE;

	//if (::IsWindow(GetSafeHwnd()) && GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
    //    SetFocus();        // Auto-destroy any InPlaceEdit's

    CWnd::OnSize(nType, cx, cy);
    ResetScrollBars();    

    // End re-entry blocking
    bAlreadyInsideThisProcedure = FALSE;

	int nLimitSize=8;			// 최소 크기

	if( cx < nLimitSize || cy < nLimitSize )
		return;

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

UINT CThemeGrid::OnGetDlgCode() 
{
    UINT nCode = DLGC_WANTARROWS | DLGC_WANTCHARS; // DLGC_WANTALLKEYS; //

    if (m_bHandleTabKey && !IsCTRLpressed()) 
        nCode |= DLGC_WANTTAB;

    return nCode;
}

#ifndef _WIN32_WCE
// If system colours change, then redo colours
void CThemeGrid::OnSysColorChange() 
{
    CWnd::OnSysColorChange();
    
    if (GetNormalFontColor() == m_crWindowText)                   // Still using system colours
        SetNormalFontColor(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetNormalBackColor() == m_crWindowColour)
        SetNormalBackColor(::GetSysColor(COLOR_WINDOW));
    if (GetBackColor() == m_crShadow)
        SetBackColor(::GetSysColor(COLOR_3DSHADOW));
    if (GetFixedFontColor() == m_crWindowText)
        SetFixedFontColor(::GetSysColor(COLOR_WINDOWTEXT));
    if (GetFixedBackColor() == m_cr3DFace)
        SetFixedBackColor(::GetSysColor(COLOR_3DFACE));

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);
}
#endif

#ifndef _WIN32_WCE_NO_CURSOR
// If we are drag-selecting cells, or drag and dropping, stop now
void CThemeGrid::OnCaptureChanged(CWnd *pWnd) 
{
    if (pWnd->GetSafeHwnd() == GetSafeHwnd()) return;

    // kill timer if active
    if (m_nTimerID != 0)
    {
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // Kill drag and drop if active
    if (m_MouseMode == MOUSE_DRAGGING)
        m_MouseMode = MOUSE_NOTHING;
#endif
}
#endif

#if (_MFC_VER >= 0x0421) || (_WIN32_WCE >= 210)
// If system settings change, then redo colours
void CThemeGrid::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
    CWnd::OnSettingChange(uFlags, lpszSection);
    
    if (GetNormalFontColor() == m_crWindowText)                   // Still using system colours
        SetNormalFontColor(::GetSysColor(COLOR_WINDOWTEXT));      // set to new system colour
    if (GetNormalBackColor() == m_crWindowColour)
        SetNormalBackColor(::GetSysColor(COLOR_WINDOW));
    if (GetBackColor() == m_crShadow)
        SetBackColor(::GetSysColor(COLOR_3DSHADOW));
    if (GetFixedFontColor() == m_crWindowText)
        SetFixedFontColor(::GetSysColor(COLOR_WINDOWTEXT));
    if (GetFixedBackColor() == m_cr3DFace)
        SetFixedBackColor(::GetSysColor(COLOR_3DFACE));

    m_crWindowText   = ::GetSysColor(COLOR_WINDOWTEXT);
    m_crWindowColour = ::GetSysColor(COLOR_WINDOW);
    m_cr3DFace       = ::GetSysColor(COLOR_3DFACE);
    m_crShadow       = ::GetSysColor(COLOR_3DSHADOW);

    m_nRowsPerWheelNotch = GetMouseScrollLines(); // Get the number of lines
}
#endif

// For drag-selection. Scrolls hidden cells into view
// TODO: decrease timer interval over time to speed up selection over time
void CThemeGrid::OnTimer(UINT_PTR nIDEvent)
{
    ASSERT(nIDEvent == WM_LBUTTONDOWN);
    if (nIDEvent != WM_LBUTTONDOWN)
        return;

    CPoint pt, origPt;

#ifdef _WIN32_WCE
    if (m_MouseMode == MOUSE_NOTHING)
        return;
    origPt = GetMessagePos();
#else
    if (!GetCursorPos(&origPt))
        return;
#endif

    ScreenToClient(&origPt);

    CRect rect;
    GetClientRect(rect);

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth = GetFixedColWidth();

	pt = origPt;

	// add SKC ---------------------------------------------------
	BOOL bOn = FALSE;
	if ((pt.x > rect.left && pt.x < rect.right) &&
		(pt.y > rect.top && pt.y < rect.bottom)) bOn = TRUE;
	if (!bOn) return;
	// end SKC ---------------------------------------------------
    
    if (pt.y > rect.bottom)
    {
        //SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        SendMessage(WM_KEYDOWN, VK_DOWN, 0);

        if (pt.x < rect.left)  
            pt.x = rect.left;
        if (pt.x > rect.right) 
            pt.x = rect.right;
        pt.y = rect.bottom;
        OnSelecting(GetCellFromPt(pt));
    }
    else if (pt.y < nFixedRowHeight)
    {
        //SendMessage(WM_VSCROLL, SB_LINEUP, 0);
        SendMessage(WM_KEYDOWN, VK_UP, 0);

        if (pt.x < rect.left)  
            pt.x = rect.left;
        if (pt.x > rect.right) 
            pt.x = rect.right;
        pt.y = nFixedRowHeight + 1;
        OnSelecting(GetCellFromPt(pt));
    }

    pt = origPt;
    if (pt.x > rect.right)
    {
        // SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        SendMessage(WM_KEYDOWN, VK_RIGHT, 0);

        if (pt.y < rect.top)    
            pt.y = rect.top;
        if (pt.y > rect.bottom) 
            pt.y = rect.bottom;
        pt.x = rect.right;
        OnSelecting(GetCellFromPt(pt));
    } 
    else if (pt.x < nFixedColWidth)
    {
        //SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        SendMessage(WM_KEYDOWN, VK_LEFT, 0);

        if (pt.y < rect.top)    
            pt.y = rect.top;
        if (pt.y > rect.bottom)
            pt.y = rect.bottom;
        pt.x = nFixedColWidth + 1;
        OnSelecting(GetCellFromPt(pt));
    }
}

// move about with keyboard
void CThemeGrid::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (!IsValid(m_idCurrentCell)) 
    {
        CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
        return;
    }

    CCellID next = m_idCurrentCell;
    BOOL bChangeLine = FALSE;

    if (IsCTRLpressed())
    {
        switch (nChar)
        {
           case 'A': OnEditSelectAll();  break;
#ifndef GRIDCONTROL_NO_CLIPBOARD
           case 'X': OnEditCut();        break;
           case 'C': OnEditCopy();       break;
           case 'V': OnEditPaste();      break;
#endif
        }
    }

    switch (nChar)
    {
        case VK_DELETE: 
            if (IsCellEditable(m_idCurrentCell.row, m_idCurrentCell.col))
            {
				SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_BEGINLABELEDIT);
                SetItemText(m_idCurrentCell.row, m_idCurrentCell.col, _T(""));
                SetModified(TRUE, m_idCurrentCell.row, m_idCurrentCell.col);
 				SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_ENDLABELEDIT);
                RedrawCell(m_idCurrentCell);
            }
            break;

        case VK_TAB:    
            if (IsSHIFTpressed())
            {
                if (next.col > m_nFixedCols) 
                    next.col--;
                else if (next.col == m_nFixedCols && next.row > m_nFixedRows) 
                {
                    next.row--; 
                    next.col = GetColCount() - 1; 
                    bChangeLine = TRUE;
                }
                else
                    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            }
            else
            {
                if (next.col < (GetColCount() - 1)) 
                    next.col++;
                else if (next.col == (GetColCount() - 1) && 
                         next.row < (GetRowCount() - 1) )
                {
                    next.row++; 
                    next.col = m_nFixedCols; 
                    bChangeLine = TRUE;
                }
                else
                    CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
            } 
            break;

        case VK_DOWN:   
            if (next.row < (GetRowCount() - 1))
                next.row++; 
            break;            
            
        case VK_UP:     
            if (next.row > m_nFixedRows)           
                next.row--; 
            break;

        case VK_RIGHT:  
            if (next.col < (GetColCount() - 1)) 
                next.col++; 
            break;
            
        case VK_LEFT:   
            if (next.col > m_nFixedCols)           
                next.col--; 
            break;

        case VK_NEXT:   
            {
            CCellID idOldTopLeft = GetTopleftNonFixedCell();
            SendMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
            CCellID idNewTopLeft = GetTopleftNonFixedCell();

            int increment = idNewTopLeft.row - idOldTopLeft.row;
            if (increment) {
                next.row += increment;
                if (next.row > (GetRowCount() - 1)) 
                    next.row = GetRowCount() - 1;
            }
            else
                next.row = GetRowCount() - 1;
                break;
            }
    
            case VK_PRIOR:  
            {
            CCellID idOldTopLeft = GetTopleftNonFixedCell();
            SendMessage(WM_VSCROLL, SB_PAGEUP, 0);
            CCellID idNewTopLeft = GetTopleftNonFixedCell();

            int increment = idNewTopLeft.row - idOldTopLeft.row;
            if (increment) 
            {
                next.row += increment;
                if (next.row < m_nFixedRows) 
                    next.row = m_nFixedRows;
            } else
                next.row = m_nFixedRows;
                break;
            }
    
        case VK_HOME:   
            SendMessage(WM_VSCROLL, SB_TOP, 0);
            next.row = m_nFixedRows;
            break;
        
        case VK_END:    
            SendMessage(WM_VSCROLL, SB_BOTTOM, 0);
            next.row = GetRowCount() - 1;
            break;
                
        default:
            CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
    }
  
    if (next != m_idCurrentCell) 
    {
        // While moving with the Cursorkeys the current ROW/CELL will get selected
        // OR Selection will get expanded when SHIFT is pressed
        // Cut n paste from OnLButtonDown - Franco Bez 
        // Added check for NULL mouse mode - Chris Maunder.
        if (m_MouseMode == MOUSE_NOTHING)
        {
            m_PrevSelectedCellMap.RemoveAll();
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            if (!IsSHIFTpressed() || nChar == VK_TAB)
                m_SelectionStartCell = next;
            OnSelecting(next);
            m_MouseMode = MOUSE_NOTHING;
        }

        SetFocusCell(next);

        if (!IsCellVisible(next))
        {   
            EnsureVisible(next); // Make sure cell is visible

            switch (nChar) {
                case VK_RIGHT:  
                    SendMessage(WM_HSCROLL, SB_LINERIGHT, 0); 
                    break;

                case VK_LEFT:   
                    SendMessage(WM_HSCROLL, SB_LINELEFT, 0);  
                    break;

                case VK_DOWN:   
                    SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);  
                    break;
                
                case VK_UP:     
                    SendMessage(WM_VSCROLL, SB_LINEUP, 0);    
                    break;                
                
                case VK_TAB:    
                    if (IsSHIFTpressed())
                    {
                        if (bChangeLine) 
                        {
                            SendMessage(WM_VSCROLL, SB_LINEUP, 0);
                            SetScrollPos32(SB_HORZ, m_nHScrollMax);
                            break;
                        }
                        else 
                            SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
                    }
                    else
                    {
                        if (bChangeLine) 
                        {
                            SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
                            SetScrollPos32(SB_HORZ, 0);
                            break;
                        }
                        else 
                            SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
                    }
                    break;

            }
            Invalidate();
        }
    }
}

// Instant editing of cells when keys are pressed
void CThemeGrid::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if (!IsCTRLpressed() && m_MouseMode == MOUSE_NOTHING)
    {
        if (!m_bHandleTabKey || (m_bHandleTabKey && nChar != VK_TAB))
            OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, nChar);
    }

    CWnd::OnChar(nChar, nRepCnt, nFlags);
}

// Callback from any CInPlaceEdits that ended. This just calls OnEndEditCell,
// refreshes the edited cell and moves onto next cell if the return character
// from the edit says we should.
void CThemeGrid::OnEndInPlaceEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{    
    GV_DISPINFO *pgvDispInfo = (GV_DISPINFO *)pNMHDR;
    GV_ITEM     *pgvItem = &pgvDispInfo->item;

    // In case OnEndInPlaceEdit called as window is being destroyed
    if (!IsWindow(GetSafeHwnd()))
        return;

    OnEndEditCell(pgvItem->row, pgvItem->col, pgvItem->szText);
    InvalidateCellRect(CCellID(pgvItem->row, pgvItem->col));

	// 셀 편집 종료 메시지를 부모 윈도우로 전달
	if( m_bBaseCtrlList )
		SendMessageToParent(pgvItem->row, pgvItem->col, LVN_ENDLABELEDIT);
		//SendMessageToParent(pgvItem->row, pgvItem->col, LVN_ITEMCHANGED);
	else
		SendMessageToParent(pgvItem->row, pgvItem->col, GVN_ENDLABELEDIT);
	
	

    switch (pgvItem->lParam) 
    {
        case VK_TAB:
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:  
        case VK_END:    
            OnKeyDown(pgvItem->lParam, 0, 0);
            OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, pgvItem->lParam);
    }

    *pResult = 0;
}

// Handle horz scrollbar notifications
void CThemeGrid::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();  // Auto-destroy any InPlaceEdit's

/*#ifndef GRIDCONTROL_NO_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif*/

    int scrollPos = GetScrollPos32(SB_HORZ);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
        case SB_LINERIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                int xScroll = GetColWidth(idTopLeft.col);
                SetScrollPos32(SB_HORZ, scrollPos + xScroll);
                if (GetScrollPos32(SB_HORZ) == scrollPos) break;          // didn't work

                rect.left = GetFixedColWidth() + xScroll;
                ScrollWindow(-xScroll, 0, rect);
                rect.left = rect.right - xScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_LINELEFT:
            if (scrollPos > 0 && idTopLeft.col > GetFixedColCount())
            {
                int xScroll = GetColWidth(idTopLeft.col-1);
                SetScrollPos32(SB_HORZ, max(0,scrollPos - xScroll));
                rect.left = GetFixedColWidth();
                ScrollWindow(xScroll, 0, rect);
                rect.right = rect.left + xScroll;
                //InvalidateRect(rect); // removed by SKC
				Invalidate(); // changed by SKC
            }
            break;

        case SB_PAGERIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                rect.left = GetFixedColWidth();
                int offset = rect.Width();
                int pos = min(m_nHScrollMax, scrollPos + offset);
                SetScrollPos32(SB_HORZ, pos);
                rect.left = GetFixedColWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGELEFT:
            if (scrollPos > 0)
            {
                rect.left = GetFixedColWidth();
                int offset = -rect.Width();
                int pos = max(0, scrollPos + offset);
                SetScrollPos32(SB_HORZ, pos);
                rect.left = GetFixedColWidth();
                InvalidateRect(rect);
            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
                SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
                CCellID idNewTopLeft = GetTopleftNonFixedCell();
                if (idNewTopLeft != idTopLeft)
                {
                    rect.left = GetFixedColWidth();
                    InvalidateRect(rect);
                }
            }
            break;

        case SB_LEFT:
            if (scrollPos > 0)
            {
                SetScrollPos32(SB_HORZ, 0);
                Invalidate();
            }
            break;

        case SB_RIGHT:
            if (scrollPos < m_nHScrollMax)
            {
                SetScrollPos32(SB_HORZ, m_nHScrollMax);
                Invalidate();
            }
            break;


        default: break;
    }
}

// Handle vert scrollbar notifications
void CThemeGrid::OnVScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
    if (GetFocus()->GetSafeHwnd() != GetSafeHwnd()) 
        SetFocus();        // Auto-destroy any InPlaceEdit's

/*#ifndef GRIDCONTROL_NO_TITLETIPS
    m_TitleTip.Hide();  // hide any titletips
#endif*/

    // Get the scroll position ourselves to ensure we get a 32 bit value
    int scrollPos = GetScrollPos32(SB_VERT);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    CRect rect;
    GetClientRect(rect);

    switch (nSBCode)
    {
        case SB_LINEDOWN:
            if (scrollPos < m_nVScrollMax)
            {
                int yScroll = GetRowHeight(idTopLeft.row);
                SetScrollPos32(SB_VERT, scrollPos + yScroll);
                if (GetScrollPos32(SB_VERT) == scrollPos) 
                    break;          // didn't work

                rect.top = GetFixedRowHeight() + yScroll;
                ScrollWindow( 0, -yScroll, rect);
                rect.top = rect.bottom - yScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_LINEUP:
            if (scrollPos > 0 && idTopLeft.row > GetFixedRowCount())
            {
                int yScroll = GetRowHeight(idTopLeft.row-1);
                SetScrollPos32(SB_VERT, max(0, scrollPos - yScroll));
                rect.top = GetFixedRowHeight();
                ScrollWindow(0, yScroll, rect);
                rect.bottom = rect.top + yScroll;
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEDOWN:
            if (scrollPos < m_nVScrollMax)
            {
                rect.top = GetFixedRowHeight();
                scrollPos = min(m_nVScrollMax, scrollPos + rect.Height());
                SetScrollPos32(SB_VERT, scrollPos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_PAGEUP:
            if (scrollPos > 0)
            {
                rect.top = GetFixedRowHeight();
                int offset = -rect.Height();
                int pos = max(0, scrollPos + offset);
                SetScrollPos32(SB_VERT, pos);
                rect.top = GetFixedRowHeight();
                InvalidateRect(rect);
            }
            break;

        case SB_THUMBPOSITION:
        case SB_THUMBTRACK:
            {
                SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
                CCellID idNewTopLeft = GetTopleftNonFixedCell();
                if (idNewTopLeft != idTopLeft)
                {
                    rect.top = GetFixedRowHeight();
                    InvalidateRect(rect);
                }
            }
            break;

        case SB_TOP:
            if (scrollPos > 0)
            {
                SetScrollPos32(SB_VERT, 0);
                Invalidate();
            }
            break;

        case SB_BOTTOM:
            if (scrollPos < m_nVScrollMax)
            {
                SetScrollPos32(SB_VERT, m_nVScrollMax);
                Invalidate();
            }

        default: break;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl implementation functions

void CThemeGrid::OnDraw(CDC* pDC)
{
    CRect rect;
    int row,col;

    CRect clipRect;
    if (pDC->GetClipBox(&clipRect) == ERROR) 
        return;

    EraseBkgnd(pDC);            // OnEraseBkgnd does nothing, so erase bkgnd here.
                                // This necessary since we may be using a Memory DC.

    int nFixedRowHeight = GetFixedRowHeight();
    int nFixedColWidth  = GetFixedColWidth();

    CCellID idTopLeft = GetTopleftNonFixedCell();
    int minVisibleRow = idTopLeft.row,
        minVisibleCol = idTopLeft.col;

    CRect VisRect;
    CCellRange VisCellRange = GetVisibleNonFixedCellRange(VisRect);
    int maxVisibleRow = VisCellRange.GetMaxRow(),
        maxVisibleCol = VisCellRange.GetMaxCol();

    // draw top-left cells 0..m_nFixedRows-1, 0..m_nFixedCols-1
    rect.bottom = -1;
    for (row = 0; row < m_nFixedRows; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;
        rect.right = -1;
        for (col = 0; col < m_nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColWidth(col)-1;  
            
            DrawFixedCell(pDC, row, col, rect);
        }
    }
     
    // draw fixed column cells:  m_nFixedRows..n, 0..m_nFixedCols-1
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) 
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top)
            continue;             // Reached cliprect yet?

        rect.right = -1;
        for (col = 0; col < m_nFixedCols; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColWidth(col)-1;

            if (rect.left > clipRect.right) 
                break;            // gone past cliprect
            if (rect.right < clipRect.left) 
                continue;         // Reached cliprect yet?

            DrawFixedCell(pDC, row, col, rect);
        }
    }
    
    // draw fixed row cells  0..m_nFixedRows, m_nFixedCols..n
    rect.bottom = -1;
    for (row = 0; row < m_nFixedRows; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) 
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) 
            continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {                                       
            rect.left = rect.right+1;
            rect.right = rect.left + GetColWidth(col)-1;

            if (rect.left > clipRect.right)
                break;        // gone past cliprect
            if (rect.right < clipRect.left) 
                continue;     // Reached cliprect yet?

            DrawFixedCell(pDC, row, col, rect);
        }
    }
    
    // draw rest of non-fixed cells
    rect.bottom = nFixedRowHeight-1;
    for (row = minVisibleRow; row <= maxVisibleRow; row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row)-1;

        // rect.bottom = bottom pixel of previous row
        if (rect.top > clipRect.bottom) 
            break;                // Gone past cliprect
        if (rect.bottom < clipRect.top) 
            continue;             // Reached cliprect yet?

        rect.right = nFixedColWidth-1;
        for (col = minVisibleCol; col <= maxVisibleCol; col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColWidth(col)-1;

            if (rect.left > clipRect.right)
                break;        // gone past cliprect
            if (rect.right < clipRect.left) 
                continue;     // Reached cliprect yet?

            DrawCell(pDC, row, col, rect);
        }
    }


    CPen pen;
    TRY {
        pen.CreatePen(PS_SOLID, 0, m_crGridColour);
    }
    CATCH (CResourceException, e)
    {
        e->Delete();
        return;
    }
    END_CATCH
    CPen *pOldPen = pDC->SelectObject(&pen);

    // draw vertical lines (drawn at ends of cells)
    if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
    {
        int x = nFixedColWidth;
        for (col = minVisibleCol; col <= maxVisibleCol; col++) {
            x += GetColWidth(col);
            pDC->MoveTo(x-1, nFixedRowHeight);
            pDC->LineTo(x-1, VisRect.bottom);   
        }
    }
    
    // draw horizontal lines (drawn at bottom of each cell)
    if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
    {
        int y = nFixedRowHeight;
        for (row = minVisibleRow; row <= maxVisibleRow; row++) {
            y += GetRowHeight(row);
            pDC->MoveTo(nFixedColWidth, y-1);    
            pDC->LineTo(VisRect.right,  y-1);
        }
    }

	pDC->SelectObject(pOldPen);
    //pDC->SelectStockObject(NULL_PEN);

#ifdef USE_MEMDC                        // Use a memDC for flicker free update
}
#else                                   // Use normal DC - this helps in debugging
}
#endif

////////////////////////////////////////////////////////////////////////////////////////
// CGridCtrl Cell selection stuff

BOOL CThemeGrid::IsValid(int nRow, int nCol) const
{
    return (nRow >= 0 && nRow < m_nRows && nCol >= 0 && nCol < m_nCols);
}

BOOL CThemeGrid::IsValid(const CCellID& cell) const
{
    return IsValid(cell.row, cell.col);
}

BOOL CThemeGrid::IsValid(const CCellRange& range) const
{
     return (range.GetMinRow() >= 0 && range.GetMinCol() >= 0 && 
            range.GetMaxRow() >= 0 && range.GetMaxCol() >= 0 &&
            range.GetMaxRow() < m_nRows && range.GetMaxCol() < m_nCols &&
             range.GetMinRow() <= range.GetMaxRow() && range.GetMinCol() <= range.GetMaxCol());
}

// Enables/Disables redraw for certain operations like columns auto-sizing etc,
// but not for user caused things such as selection changes.
void CThemeGrid::SetRedraw(BOOL bAllowDraw, BOOL bResetScrollBars /* = FALSE */)
{
    //TRACE(_T("%s: Setting redraw to %s\n"), GetRuntimeClass()->m_lpszClassName, bAllowDraw? _T("TRUE") : _T("FALSE"));

    if (bAllowDraw && !m_bAllowDraw) 
        Invalidate();
    m_bAllowDraw = bAllowDraw;
    if (bResetScrollBars) 
        ResetScrollBars();
}

// Forces a redraw of a cell immediately (using a direct DC construction, 
// or the supplied dc)
BOOL CThemeGrid::RedrawCell(const CCellID& cell, CDC* pDC /* = NULL */)
{
    return RedrawCell(cell.row, cell.col, pDC);
}

BOOL CThemeGrid::RedrawCell(int nRow, int nCol, CDC* pDC /* = NULL */)
{
    BOOL bResult = TRUE;
    BOOL bMustReleaseDC = FALSE;

    if (!m_bAllowDraw || !IsCellVisible(nRow, nCol))
        return FALSE;

    CRect rect;
    if (!GetCellRect(nRow, nCol, rect)) 
        return FALSE;

    if (!pDC) {
        pDC = GetDC();
        if (pDC)
            bMustReleaseDC = TRUE;
    }

    if (pDC)
    {
        // Redraw cells directly
        if (nRow < m_nFixedRows || nCol < m_nFixedCols)
        {
            bResult = DrawFixedCell(pDC, nRow, nCol, rect, TRUE);
        }
        else
        {
            bResult = DrawCell(pDC, nRow, nCol, rect, TRUE);
            
            // Since we have erased the background, we will need to redraw the gridlines
            CPen pen;
            TRY {
                pen.CreatePen(PS_SOLID, 0, m_crGridColour);
            } CATCH (CException, e) {e->Delete();} END_CATCH
                
            CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
            {
                pDC->MoveTo(rect.left,    rect.bottom);
                pDC->LineTo(rect.right+1, rect.bottom);
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
            {
                pDC->MoveTo(rect.right, rect.top);
                pDC->LineTo(rect.right, rect.bottom+1);    
            }
            pDC->SelectObject(pOldPen);
        }
    } else
        InvalidateRect(rect, TRUE);     // Could not get a DC - invalidate it anyway
                                        // and hope that OnPaint manages to get one

    if (bMustReleaseDC) 
        ReleaseDC(pDC);

    return bResult;
}

// redraw a complete row
BOOL CThemeGrid::RedrawRow(int row)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int col = 0; col < GetColCount(); col++)
        bResult = RedrawCell(row, col, pDC) && bResult;

    if (pDC) ReleaseDC(pDC);

    return bResult;
}

// redraw a complete column
BOOL CThemeGrid::RedrawCol(int col)
{
    BOOL bResult = TRUE;

    CDC* pDC = GetDC();
    for (int row = 0; row < GetRowCount(); row++)
        bResult = RedrawCell(row, col, pDC) && bResult;

    if (pDC) ReleaseDC(pDC);

    return bResult;
}

void CThemeGrid::SetCellColor(int row, int col, COLORREF color)
{
	CGridCell* pGridCell = nullptr;
	GRID_ROW* pRow = m_RowData[row];
	if (pRow == nullptr)
	{
		return;
	}

	pGridCell = pRow->GetAt(col);
	if (pGridCell == nullptr)
	{
		return;
	}

	pGridCell->crBkClr = color;
	Invalidate();
}

COLORREF CThemeGrid::GetCellColor(int row, int col)
{
	CGridCell* pGridCell = nullptr;
	GRID_ROW* pRow = m_RowData[row];
	if (pRow == nullptr)
	{
		return RGB(0, 0, 0);
	}

	pGridCell = pRow->GetAt(col);
	if (pGridCell == nullptr)
	{
		return RGB(0, 0, 0);
	}

	return pGridCell->crBkClr;
}

// Sets the currently selected cell, returning the previous current cell
CCellID CThemeGrid::SetFocusCell(int nRow, int nCol)
{
    return SetFocusCell(CCellID(nRow, nCol));
}

CCellID CThemeGrid::SetFocusCell(CCellID cell)
{
    if (cell == m_idCurrentCell) 
        return m_idCurrentCell;

    CCellID idPrev = m_idCurrentCell;
    m_idCurrentCell = cell;

    if (IsValid(idPrev)) 
    {
		if( !m_bBaseCtrlList )
			SendMessageToParent(idPrev.row, idPrev.col, GVN_SELCHANGING); 

        SetItemState(idPrev.row, idPrev.col, 
                     GetItemState(idPrev.row, idPrev.col) & ~GVIS_FOCUSED);
        RedrawCell(idPrev);

        if (idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++)
                RedrawCell(row, idPrev.col);
        if (idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++) 
                RedrawCell(idPrev.row, col);
    }

    if (IsValid(m_idCurrentCell)) {
        SetItemState(m_idCurrentCell.row, m_idCurrentCell.col, 
                     GetItemState(m_idCurrentCell.row, m_idCurrentCell.col) | GVIS_FOCUSED);

        RedrawCell(m_idCurrentCell);

        if (idPrev.col != m_idCurrentCell.col)
            for (int row = 0; row < m_nFixedRows; row++) 
                RedrawCell(row, m_idCurrentCell.col);
        if (idPrev.row != m_idCurrentCell.row)
            for (int col = 0; col < m_nFixedCols; col++) 
                RedrawCell(m_idCurrentCell.row, col);

		if( !m_bBaseCtrlList )
			SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_SELCHANGED); 
    }

    return idPrev;
}

// Sets the range of currently selected cells
void CThemeGrid::SetSelectedRange(const CCellRange& Range, 
                                 BOOL bForceRepaint /* = FALSE */)
{
    SetSelectedRange(Range.GetMinRow(), Range.GetMinCol(), 
                     Range.GetMaxRow(), Range.GetMaxCol(),
                     bForceRepaint);
}

void CThemeGrid::SetSelectedRange(int nMinRow, int nMinCol, int nMaxRow, int nMaxCol,
                                 BOOL bForceRepaint /* = FALSE */)
{
    if (!m_bEnableSelection) return;

    CDC* pDC = NULL;
    if (bForceRepaint) pDC = GetDC();

	POSITION pos;
    // Unselect all previously selected cells
    for (pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        // Reset the selection flag on the cell
        if (IsValid(cell)) {
            SetItemState(cell.row, cell.col, 
                         GetItemState(cell.row, cell.col) & ~GVIS_SELECTED);

            // If this is to be reselected, continue on past the redraw
            if (nMinRow <= cell.row && cell.row <= nMaxRow &&
                nMinCol <= cell.col && cell.col <= nMaxCol)
                    continue;

            if (bForceRepaint && pDC)                    // Redraw NOW
                RedrawCell(cell.row, cell.col, pDC);
            else
                InvalidateCellRect(cell);                // Redraw at leisure
        }
    }
    
    // if any previous selected cells are to be retained (eg Ctrl is being held down)
    // then copy them to the newly created list, and mark all these cells as
    // selected
    if (!GetSingleRowSelection())
    for (pos = m_PrevSelectedCellMap.GetStartPosition(); pos != NULL; /* nothing */)
    {
        DWORD key;
        CCellID cell;
        m_PrevSelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        if (!IsValid(cell))
            continue;

        int nState = GetItemState(cell.row, cell.col);

        // Set state as Selected. This will add the cell to m_SelectedCells[]
        SetItemState(cell.row, cell.col, nState | GVIS_SELECTED);

        // Redraw (immediately or at leisure)
        if (bForceRepaint && pDC)
            RedrawCell(cell.row, cell.col, pDC);
        else
            InvalidateCellRect(cell);
    }

    // Now select all cells in the cell range specified. If the cell has already
    // been marked as selected (above) then ignore it.
    if (nMinRow >= 0 && nMinCol >= 0 && nMaxRow >= 0 && nMaxCol >= 0 &&
        nMaxRow < m_nRows && nMaxCol < m_nCols &&
         nMinRow <= nMaxRow && nMinCol <= nMaxCol)
    {
        for (int row = nMinRow; row <= nMaxRow; row++)
            for (int col = nMinCol; col <= nMaxCol; col++) 
            {
                int nState = GetItemState(row, col);
                if (nState & GVIS_SELECTED)
                    continue;    // Already selected - ignore

                // Add to list of selected cells
                CCellID cell(row, col);

                // Set state as Selected. This will add the cell to m_SelectedCells[]
                SetItemState(row, col, nState | GVIS_SELECTED);

                // Redraw (immediately or at leisure)
                if (bForceRepaint && pDC)
                    RedrawCell(row, col, pDC);
                else
                    InvalidateCellRect(cell);
            }
    }
//    TRACE(_T("%d cells selected.\n"), m_SelectedCellMap.GetCount());

    if (pDC != NULL) ReleaseDC(pDC);
} 

// selects all cells
void CThemeGrid::SelectAllCells()
{
    if (!m_bEnableSelection)
        return;

    SetSelectedRange(m_nFixedRows, m_nFixedCols, GetRowCount()-1, GetColCount()-1);
}

// selects columns
void CThemeGrid::SelectCols(CCellID currentCell)
{
    if (!m_bEnableSelection)
        return;

    //if (currentCell.col == m_idCurrentCell.col) return;
    if (currentCell.col < m_nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    SetSelectedRange(GetFixedRowCount(), 
                     min(m_SelectionStartCell.col, currentCell.col), 
                     GetRowCount()-1,    
                     max(m_SelectionStartCell.col, currentCell.col)); 
}

// selects rows
void CThemeGrid::SelectRows(CCellID currentCell)
{  
    if (!m_bEnableSelection)
        return;

    //if (currentCell.row; == m_idCurrentCell.row) return;
    if (currentCell.row < m_nFixedRows)
        return;
    if (!IsValid(currentCell))
        return;

    if (GetListMode() && GetSingleRowSelection())
        SetSelectedRange(currentCell.row, GetFixedColCount(),
                         currentCell.row, GetColCount()-1);
    else 
        SetSelectedRange(min(m_SelectionStartCell.row, currentCell.row),
                         GetFixedColCount(), 
                         max(m_SelectionStartCell.row, currentCell.row), 
                         GetColCount()-1); 
}

// selects cells
void CThemeGrid::SelectCells(CCellID currentCell)
{
    if (!m_bEnableSelection) 
        return;

    int row = currentCell.row;
    int col = currentCell.col;
    if (row < m_nFixedRows || col < m_nFixedCols)
        return;
    if (!IsValid(currentCell))
        return;

    // Prevent unnecessary redraws
    //if (currentCell == m_LeftClickDownCell)  return;
    //else if (currentCell == m_idCurrentCell) return;

    SetSelectedRange(min(m_SelectionStartCell.row, row), 
                     min(m_SelectionStartCell.col, col), 
                     max(m_SelectionStartCell.row, row),
                     max(m_SelectionStartCell.col, col)); 
}

void CThemeGrid::OnSelecting(const CCellID& currentCell)
{
    if (!m_bEnableSelection) 
        return;

    switch(m_MouseMode)
    {
        case MOUSE_SELECT_ALL:   SelectAllCells();            break;
        case MOUSE_SELECT_COL:   SelectCols(currentCell);  break;
        case MOUSE_SELECT_ROW:   SelectRows(currentCell);     break;
        case MOUSE_SELECT_CELLS: SelectCells(currentCell);    break;
    }
}

#ifndef GRIDCONTROL_NO_CLIPBOARD

////////////////////////////////////////////////////////////////////////////////////////
// Clipboard functions

void CThemeGrid::CutSelectedText()
{
    if (!IsEditable())
        return;

    // Clear contents of selected cells.
    for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        if (!IsCellEditable(cell))
            continue;

        CGridCell* pCell = GetCell(cell.row, cell.col);
        if (pCell)
        {
		    SendMessageToParent(cell.row, cell.col, GVN_BEGINLABELEDIT);
            EmptyCell(pCell, cell.row, cell.col);
            SetModified(TRUE, cell.row, cell.col);
		    SendMessageToParent(cell.row, cell.col, GVN_ENDLABELEDIT);
        }
    }
    Invalidate();
}

COleDataSource* CThemeGrid::CopyTextFromGrid()
{
    USES_CONVERSION;

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection)) return NULL;

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    CString str;
    CGridCell *pCell;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++) 
    {
        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            pCell = GetCell(row,col);
            if (pCell && (pCell->state  & GVIS_SELECTED))
            {
                if (pCell->szText.IsEmpty())
                    str += _T(" ");
                else 
                   str += pCell->szText;
            }
            if (col != Selection.GetMaxCol()) 
                str += _T("\t");
        }
        if (row != Selection.GetMaxRow()) 
            str += _T("\n");

        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }

    char c = '\0';
    sf.Write(&c, 1);

    DWORD dwLen = (DWORD)sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem) 
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
    if (!hMem) 
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}

BOOL CThemeGrid::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPWSTR szBuffer = new wchar_t[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

    sf.Read(szBuffer, ::GlobalSize(hmem));
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    CString strText = szBuffer;
    delete szBuffer;

    // Parse text data and set in cells...
    strText.LockBuffer();
    CString strLine = strText;
    int nLine = 0;

    // Find the end of the first line
    int nIndex;
    do {
        int nColumn = 0;
        nIndex = strLine.Find(_T("\n"));

        // Store the remaining chars after the newline
        CString strNext = (nIndex < 0)? _T("")  : strLine.Mid(nIndex+1);

        // Remove all chars after the newline
        if (nIndex >= 0) 
            strLine = strLine.Left(nIndex);

        // Make blank entries a "space"
        if (strLine.IsEmpty() && nIndex >= 0)
            strLine = _T(" ");

        LPTSTR szLine = strLine.GetBuffer(1);

        // Break the current line into tokens (tab or comma delimited)
		TCHAR* next_token;
        //LPTSTR pszCellText = _tcstok(szLine, _T("\t,\n"));
		LPTSTR pszCellText = _tcstok_s(szLine, _T("\t,\n"), &next_token);
        while (pszCellText != NULL)
        {
            CCellID TargetCell(cell.row + nLine, cell.col + nColumn);
            if (IsValid(TargetCell))
            {
                CString strCellText = pszCellText;
                strCellText.TrimLeft();  strCellText.TrimRight();

				SendMessageToParent(TargetCell.row, TargetCell.col, GVN_BEGINLABELEDIT);
                SetItemText(TargetCell.row, TargetCell.col, strCellText);
                SetModified(TRUE, TargetCell.row, TargetCell.col);
				SendMessageToParent(TargetCell.row, TargetCell.col, GVN_ENDLABELEDIT);

                // Make sure cell is not selected to avoid data loss
                SetItemState(TargetCell.row, TargetCell.col,
                             GetItemState(TargetCell.row, TargetCell.col) & ~GVIS_SELECTED);
            }

            //pszCellText = _tcstok(NULL, _T("\t,\n"));
			pszCellText = _tcstok_s(NULL, _T("\t,\n"), &next_token);
            nColumn++;
        }

        strLine.ReleaseBuffer();
        strLine = strNext;
        nLine++;

    } while (nIndex >= 0);

    strText.UnlockBuffer();
    Invalidate();

    return TRUE;
}
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP

void CThemeGrid::OnBeginDrag()
{        
    if (!m_bAllowDragAndDrop) return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (pSource) 
    {    
        SendMessageToParent(GetSelectedCellRange().GetTopLeft().row,
                            GetSelectedCellRange().GetTopLeft().col,
                            GVN_BEGINDRAG);

        m_MouseMode = MOUSE_DRAGGING;
        DROPEFFECT dropEffect = pSource->DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);

        if (dropEffect & DROPEFFECT_MOVE)
            CutSelectedText();

        if (pSource) 
            delete pSource;    // Did not pass source to clipboard, so must delete
    }    
}

DROPEFFECT CThemeGrid::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, 
                                 CPoint point)
{
    // Any text data available for us?
    if (!m_bAllowDragAndDrop || !IsEditable() || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    CCellID cell = GetCellFromPt(point, FALSE);

    // If not valid, set the previously drop-highlighted cell as no longer drop-highlighted
    if (!IsValid(cell))
    {
        OnDragLeave();
        m_LastDragOverCell = CCellID(-1,-1);
        return DROPEFFECT_NONE;
    }

    if (!IsCellEditable(cell))
        return DROPEFFECT_NONE;

    // Have we moved over a different cell than last time?
    if (cell != m_LastDragOverCell) 
    {
        // Set the previously drop-highlighted cell as no longer drop-highlighted
        if (IsValid(m_LastDragOverCell)) {
            UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
            SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                         nState & ~GVIS_DROPHILITED);
            RedrawCell(m_LastDragOverCell);
        }

        m_LastDragOverCell = cell;

        // Set the new cell as drop-highlighted
        if (IsValid(m_LastDragOverCell)) {
            UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
            SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                         nState | GVIS_DROPHILITED);
            RedrawCell(m_LastDragOverCell);
        }
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

DROPEFFECT CThemeGrid::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, 
                                  CPoint point)
{
    // Any text data available for us?
    if (!m_bAllowDragAndDrop || !pDataObject->IsDataAvailable(CF_TEXT))
        return DROPEFFECT_NONE;

    // Find which cell we are over and drop-highlight it
    m_LastDragOverCell = GetCellFromPt(point, FALSE);
    if (!IsValid(m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (!IsCellEditable(m_LastDragOverCell))
        return DROPEFFECT_NONE;

    if (IsValid(m_LastDragOverCell)) 
    {
        UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
        SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                     nState | GVIS_DROPHILITED);
        RedrawCell(m_LastDragOverCell);
    }

    // Return an appropraite value of DROPEFFECT so mouse cursor is set properly
    if (dwKeyState & MK_CONTROL)
        return DROPEFFECT_COPY;
    else
        return DROPEFFECT_MOVE;
}

void CThemeGrid::OnDragLeave()
{
    // Set the previously drop-highlighted cell as no longer drop-highlighted
    if (IsValid(m_LastDragOverCell)) {
        UINT nState = GetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col);
        SetItemState(m_LastDragOverCell.row, m_LastDragOverCell.col,
                     nState & ~GVIS_DROPHILITED);
        RedrawCell(m_LastDragOverCell);
    }
}

BOOL CThemeGrid::OnDrop(COleDataObject* pDataObject, DROPEFFECT /*dropEffect*/, 
                       CPoint /* point */) 
{
    BOOL bResult = FALSE;

    if (!m_bAllowDragAndDrop || !IsCellEditable(m_LastDragOverCell))
        return bResult;

    m_MouseMode = MOUSE_NOTHING;
    OnDragLeave();

    return PasteTextToGrid(m_LastDragOverCell, pDataObject);
}
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CThemeGrid::OnEditCut()
{
    if (!IsEditable())
        return;

    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource) return;

    pSource->SetClipboard();
    CutSelectedText();
}

void CThemeGrid::OnEditCopy()
{
    COleDataSource* pSource = CopyTextFromGrid();
    if (!pSource) return;

    pSource->SetClipboard();
}

void CThemeGrid::OnEditPaste()
{
    if (!IsEditable())
        return;

    // Get the Focus cell, or if none, get the topleft (non-fixed) cell
    CCellID cell = GetFocusCell();  
    if (!IsValid(cell)) cell = GetTopleftNonFixedCell();  
    if (!IsValid(cell)) return;

    // Attach a COleDataObject to the clipboard and paste the data to the grid
    COleDataObject obj;
    if (obj.AttachClipboard()) 
        PasteTextToGrid(cell, &obj);
}
#endif

void CThemeGrid::OnEditSelectAll() 
{
    SelectAllCells();
}

#ifndef GRIDCONTROL_NO_CLIPBOARD
void CThemeGrid::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
    CCellRange Selection = GetSelectedCellRange();
    pCmdUI->Enable(Selection.Count() && IsValid(Selection));
}

void CThemeGrid::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
    CCellRange Selection = GetSelectedCellRange();
    pCmdUI->Enable(IsEditable() && Selection.Count() && IsValid(Selection));
}

void CThemeGrid::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
    CCellID cell = GetFocusCell();

    BOOL bCanPaste = IsValid(cell) && IsCellEditable(cell) &&
                     ::IsClipboardFormatAvailable(CF_TEXT);

    pCmdUI->Enable(bCanPaste);
}
#endif

void CThemeGrid::OnUpdateEditSelectAll(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(m_bEnableSelection);
}

////////////////////////////////////////////////////////////////////////////////////////
// hitGridCtrlEx-like functions

BOOL CThemeGrid::MouseOverRowResizeArea(CPoint& point) const
{
    if (point.x >= GetFixedColWidth())
        return FALSE;
    
    CCellID idCurrentCell = GetCellFromPt(point);
    CPoint start;
    if (!GetCellOrigin(idCurrentCell, &start)) 
        return FALSE;
    
    int endy = start.y + GetRowHeight(idCurrentCell.row);
    
    if ((point.y - start.y <= m_nResizeCaptureRange && idCurrentCell.row != 0) || 
        endy - point.y <= m_nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

BOOL CThemeGrid::MouseOverColResizeArea(CPoint& point) const
{
    if (point.y >= GetFixedRowHeight())
        return FALSE;
    
    CCellID idCurrentCell = GetCellFromPt(point);
    CPoint start;
    if (!GetCellOrigin(idCurrentCell, &start)) 
        return FALSE;
    
    int endx = start.x + GetColWidth(idCurrentCell.col);
    
    if ((point.x - start.x <= m_nResizeCaptureRange && idCurrentCell.col != 0) || 
        endx - point.x <= m_nResizeCaptureRange)
    {
        return TRUE;
    }
    else
        return FALSE;
}

// Get cell from point
CCellID CThemeGrid::GetCellFromPt(CPoint point, BOOL bAllowFixedCellCheck /*=TRUE*/) const
{  
    CCellID idTopLeft = GetTopleftNonFixedCell();
    CCellID cellID; // return value

    // calculate column index
    int fixedColWidth = GetFixedColWidth();

    if (point.x < 0 || (!bAllowFixedCellCheck && point.x < fixedColWidth)) // not in window
        cellID.col = -1;
    else if (point.x < fixedColWidth) // in fixed col
    {
        int xpos=0, col=0;
        for (col = 0; col < m_nFixedCols; col++)
        {
            xpos += GetColWidth(col);
            if (xpos > point.x) break;
        }
        cellID.col = col;
    }
    else    // in non-fixed col
    {
        int xpos = fixedColWidth, col=0;
        for (col = idTopLeft.col; col < GetColCount(); col++)
        {
            xpos += GetColWidth(col);
            if (xpos > point.x) break;
        }

        if (col >= GetColCount())
            cellID.col = -1;
        else
            cellID.col = col;
    }
    
    // calculate row index
    int fixedRowHeight = GetFixedRowHeight();
    if (point.y < 0 || (!bAllowFixedCellCheck && point.y < fixedRowHeight)) // not in window
        cellID.row = -1;
    else if (point.y < fixedRowHeight) // in fixed col
    {
        int ypos = 0, row=0;
        for (row = 0; row < m_nFixedRows; row++)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y) break;
        }
        cellID.row = row;
    }
    else
    {
        int ypos = fixedRowHeight, row=0;
        for (row = idTopLeft.row; row < GetRowCount(); row++)
        {
            ypos += GetRowHeight(row);
            if (ypos > point.y) break;
        }

        if (row >= GetRowCount())
            cellID.row = -1;
        else
            cellID.row = row;
    }

    return cellID;
}

////////////////////////////////////////////////////////////////////////////////
// CGridCtrl cellrange functions

CCellID CThemeGrid::GetTopleftNonFixedCell() const
{
    int nVertScroll = GetScrollPos(SB_VERT), 
        nHorzScroll = GetScrollPos(SB_HORZ);

    int nColumn = m_nFixedCols, nRight = 0;
    while (nRight < nHorzScroll && nColumn < (GetColCount()-1))
        nRight += GetColWidth(nColumn++);

    int nRow = m_nFixedRows, nTop = 0;
    while (nTop < nVertScroll && nRow < (GetRowCount()-1))
        nTop += GetRowHeight(nRow++);

    //TRACE("TopLeft cell is row %d, col %d\n",nRow, nColumn);
    return CCellID(nRow, nColumn);
}

// This gets even partially visible cells
CCellRange CThemeGrid::GetVisibleNonFixedCellRange(LPRECT pRect /*=NULL*/) const
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i=0;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom) {
            bottom = rect.bottom;
            break;
        }
    }                                
    int maxVisibleRow = min(i, GetRowCount() - 1);
    
    // calc right
    int right = GetFixedColWidth();
    for (i = idTopLeft.col; i < GetColCount(); i++)
    {
        right += GetColWidth(i);
        if (right >= rect.right) {
            right = rect.right;
            break;
        }
    }
    int maxVisibleCol = min(i, GetColCount() - 1);
    if (pRect) {
        pRect->left = pRect->top = 0;
        pRect->right = right;
        pRect->bottom = bottom;
    }

    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// used by ResetScrollBars() - This gets only fully visible cells
CCellRange CThemeGrid::GetUnobstructedNonFixedCellRange() const
{
    CRect rect;
    GetClientRect(rect);

    CCellID idTopLeft = GetTopleftNonFixedCell();

    // calc bottom
    int bottom = GetFixedRowHeight();
	int i=0;
    for (i = idTopLeft.row; i < GetRowCount(); i++)
    {
        bottom += GetRowHeight(i);
        if (bottom >= rect.bottom) break;
    }
    int maxVisibleRow = min(i, GetRowCount() - 1);
    if (maxVisibleRow > 0 && bottom > rect.bottom) maxVisibleRow--;
    
    // calc right
    int right = GetFixedColWidth();
    for (i = idTopLeft.col; i < GetColCount(); i++)
    {
        right += GetColWidth(i);
        if (right >= rect.right) break;
    }
    int maxVisibleCol = min(i, GetColCount() - 1);
    if (maxVisibleCol > 0 && right > rect.right) maxVisibleCol--;


    return CCellRange(idTopLeft.row, idTopLeft.col, maxVisibleRow, maxVisibleCol);
}

// Returns the minimum bounding range of the current selection
// If no selection, then the returned CCellRange will be invalid
CCellRange CThemeGrid::GetSelectedCellRange() const
{
    CCellRange Selection(GetRowCount(), GetColCount(), -1,-1);

    for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
    {
        DWORD key;
        CCellID cell;
        m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);

        Selection.SetMinRow( min(Selection.GetMinRow(), cell.row) );
        Selection.SetMinCol( min(Selection.GetMinCol(), cell.col) );
        Selection.SetMaxRow( max(Selection.GetMaxRow(), cell.row) );
        Selection.SetMaxCol( max(Selection.GetMaxCol(), cell.col) );
    }

    return Selection;
}

// Returns ALL the cells in the grid
CCellRange CThemeGrid::GetCellRange() const
{
    return CCellRange(0, 0, GetRowCount() - 1, GetColCount() - 1);
}

void CThemeGrid::ResetSelectedRange()
{
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

// Get/Set scroll position using 32 bit functions
int CThemeGrid::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);

    if (bGetTrackPos)
    {
        if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
            return si.nTrackPos;
    }
    else 
    {
        if (GetScrollInfo(nBar, &si, SIF_POS))
            return si.nPos;
    }

    return 0;
}

BOOL CThemeGrid::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_POS;
    si.nPos   = nPos;
    return SetScrollInfo(nBar, &si, bRedraw);
}

void CThemeGrid::ResetScrollBars()
{
    if (!m_bAllowDraw || !::IsWindow(GetSafeHwnd())) 
        return;

    CRect rect;
    GetClientRect(rect);
    rect.left  += GetFixedColWidth();
    rect.top   += GetFixedRowHeight();
    if (rect.left >= rect.right || rect.top >= rect.bottom) return;

    CRect VisibleRect(GetFixedColWidth(), GetFixedRowHeight(), rect.right, rect.bottom+16);
    CRect VirtualRect(GetFixedColWidth(), GetFixedRowHeight(), GetVirtualWidth(), GetVirtualHeight());

    CCellRange visibleCells = GetUnobstructedNonFixedCellRange();
    if (!IsValid(visibleCells)) {
		m_nVScrollMax = 0;
		m_nHScrollMax = 0;
		SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
		SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
		return;
	}

    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask  = SIF_PAGE;
    si.nPage  = VisibleRect.Width();    SetScrollInfo(SB_HORZ, &si, FALSE); 
    si.nPage  = VisibleRect.Height();   SetScrollInfo(SB_VERT, &si, FALSE); 

	//TRACE("\n GetFixedRowHeight()=%d", GetFixedRowHeight());
	//TRACE("\n VisibleRect.Height()=%d < VirtualRect.Height()=%d", VisibleRect.Height(), VirtualRect.Height());
	//TRACE("\n VisibleRect.Width()=%d < VirtualRect.Width()=%d", VisibleRect.Width(), VirtualRect.Width());
    if (VisibleRect.Height() < VirtualRect.Height())
        m_nVScrollMax = VirtualRect.Height()-1; //+ GetRowHeight(visibleCells.GetTopLeft().row);
    else
        m_nVScrollMax = 0;

    if (VisibleRect.Width() < VirtualRect.Width())
        m_nHScrollMax = VirtualRect.Width()-1; //+ GetColWidth(visibleCells.GetTopLeft().col);
    else
        m_nHScrollMax = 0;

    ASSERT(m_nVScrollMax < INT_MAX && m_nHScrollMax < INT_MAX); // This should be fine :)
    SetScrollRange(SB_VERT, 0, m_nVScrollMax, TRUE);
    SetScrollRange(SB_HORZ, 0, m_nHScrollMax, TRUE);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column position functions

// returns the top left point of the cell. Returns FALSE if cell not visible.
BOOL CThemeGrid::GetCellOrigin(int nRow, int nCol, LPPOINT p) const
{
    int i;

    if (!IsValid(nRow, nCol)) return FALSE;

    CCellID idTopLeft;
    if (nCol >= m_nFixedCols || nRow >= m_nFixedRows)
        idTopLeft = GetTopleftNonFixedCell();

    if ((nRow >= m_nFixedRows && nRow < idTopLeft.row) ||
        (nCol>= m_nFixedCols && nCol < idTopLeft.col))
        return FALSE;

    p->x = 0;
    if (nCol < m_nFixedCols)                      // is a fixed column
        for (i = 0; i < nCol; i++)
            p->x += GetColWidth(i);
    else {                                        // is a scrollable data column
        for (i = 0; i < m_nFixedCols; i++)
            p->x += GetColWidth(i);
        for (i = idTopLeft.col; i < nCol; i++)
            p->x += GetColWidth(i);
    }

    p->y = 0;
    if (nRow < m_nFixedRows)                      // is a fixed row
        for (i = 0; i < nRow; i++)
            p->y += GetRowHeight(i);
    else {                                        // is a scrollable data row
        for (i = 0; i < m_nFixedRows; i++)
            p->y += GetRowHeight(i);
        for (i = idTopLeft.row; i < nRow; i++)
            p->y += GetRowHeight(i);
    }

    return TRUE;
}

BOOL CThemeGrid::GetCellOrigin(const CCellID& cell, LPPOINT p) const
{
    return GetCellOrigin(cell.row, cell.col, p);
}

// Returns the bounding box of the cell
BOOL CThemeGrid::GetCellRect(const CCellID& cell, LPRECT pRect) const
{
    return GetCellRect(cell.row, cell.col, pRect);
}

BOOL CThemeGrid::GetCellRect(int nRow, int nCol, LPRECT pRect) const
{    
    CPoint CellOrigin;
    if (!GetCellOrigin(nRow, nCol, &CellOrigin)) return FALSE;

    pRect->left   = CellOrigin.x;
    pRect->top    = CellOrigin.y;
    pRect->right  = CellOrigin.x + GetColWidth(nCol)-1;
    pRect->bottom = CellOrigin.y + GetRowHeight(nRow)-1;

    //TRACE("Row %d, col %d: L %d, T %d, W %d, H %d:  %d,%d - %d,%d\n",
    //      nRow,nCol, CellOrigin.x, CellOrigin.y, GetColWidth(nCol), GetRowHeight(nRow),
    //      pRect->left, pRect->top, pRect->right, pRect->bottom);

    return TRUE;
}

BOOL CThemeGrid::GetTextRect(const CCellID& cell, LPRECT pRect)
{
    return GetTextRect(cell.row, cell.col, pRect);
}

BOOL CThemeGrid::GetTextRect(int nRow, int nCol, LPRECT pRect)
{
    GV_ITEM Item;
    Item.mask = GVIF_IMAGE;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item))
        return FALSE;

    if (!GetCellRect(nRow, nCol, pRect))
        return FALSE;

	if (m_pImageList && Item.iImage >= 0) {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info)) {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            pRect->left += (nImageWidth+m_nMargin);
        }
    }
	else if (Item.iImage >= 0) {
		pRect->left += (11+m_nMargin);
	}

    return TRUE;
}

// Returns the bounding box of a range of cells
BOOL CThemeGrid::GetCellRangeRect(const CCellRange& cellRange, LPRECT lpRect) const
{
    CPoint MinOrigin,MaxOrigin;

    if (!GetCellOrigin(cellRange.GetMinRow(), cellRange.GetMinCol(), &MinOrigin)) 
        return FALSE;
    if (!GetCellOrigin(cellRange.GetMaxRow(), cellRange.GetMaxCol(), &MaxOrigin)) 
        return FALSE;

    lpRect->left   = MinOrigin.x;
    lpRect->top    = MinOrigin.y;
    lpRect->right  = MaxOrigin.x + GetColWidth(cellRange.GetMaxCol()-1);
    lpRect->bottom = MaxOrigin.y + GetRowHeight(cellRange.GetMaxRow()-1);

    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Grid attribute functions

LRESULT CThemeGrid::OnSetFont(WPARAM hFont, LPARAM /*lParam */)
{
    LRESULT result = Default();

    // Get the logical font
    LOGFONT lf;
    if (!GetObject((HFONT) hFont, sizeof(LOGFONT), &lf))
        return result;
    
    // Store font as the global default
    memcpy(&m_Logfont, &lf, sizeof(LOGFONT));
    
    // reset all cells' fonts
    for (int row = 0; row < GetRowCount(); row++)
        for (int col = 0; col < GetColCount(); col++)
            SetItemFont(row, col, &lf);

    // Get the font size and hence the default cell size
    CDC* pDC = GetDC();
    if (pDC) 
    {
        m_Font.DeleteObject();
        m_Font.CreateFontIndirect(&m_Logfont);
        CFont* pOldFont = pDC->SelectObject(&m_Font);
        
        TEXTMETRIC tm;
        pDC->GetTextMetrics(&tm);
        
        m_nMargin = pDC->GetTextExtent(_T(" "),1).cx;
        pDC->SelectObject(pOldFont);
        ReleaseDC(pDC);

        m_nDefCellHeight = tm.tmHeight+tm.tmExternalLeading + 2*m_nMargin;
        m_nDefCellWidth  = tm.tmAveCharWidth*12 + 2*m_nMargin;
    }

    if (::IsWindow(GetSafeHwnd())) 
        Invalidate();

    return result;
}

LRESULT CThemeGrid::OnGetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
    return (LRESULT) (HFONT) m_Font;
}

#ifndef _WIN32_WCE_NO_CURSOR
BOOL CThemeGrid::OnSetCursor(CWnd* pWnd, UINT nHitGridCtrlEx, UINT message) 
{
    if (nHitGridCtrlEx == HTCLIENT)
    {
        switch (m_MouseMode) 
        {
            case MOUSE_OVER_COL_DIVIDE: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE)); break;
            case MOUSE_OVER_ROW_DIVIDE: SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS)); break;
#ifndef GRIDCONTROL_NO_DRAGDROP
            case MOUSE_DRAGGING:        break;
#endif
            default:                    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        }
        return TRUE;
    }

    return CWnd::OnSetCursor(pWnd, nHitGridCtrlEx, message);
}
#endif

////////////////////////////////////////////////////////////////////////////////////
// Row/Column count functions

BOOL CThemeGrid::SetFixedRows(int nFixedRows)
{
	return SetFixedRowCount(nFixedRows);
}

BOOL CThemeGrid::SetFixedRowCount(int nFixedRows)
{
    ASSERT(nFixedRows >= 0);

    if (nFixedRows > GetRowCount()) 
        if (!SetRowCount(nFixedRows)) return FALSE;

    if (m_idCurrentCell.row < nFixedRows)
        SetFocusCell(-1,-1);

    m_nFixedRows = nFixedRows;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CThemeGrid::SetFixedCols(int nFixedCols)
{
	return SetFixedColCount(nFixedCols);
}

BOOL CThemeGrid::SetFixedColCount(int nFixedCols)
{
    ASSERT(nFixedCols >= 0);
    if (nFixedCols > GetColCount())
        if (!SetColCount(nFixedCols)) return FALSE;

    if (m_idCurrentCell.col < nFixedCols)
        SetFocusCell(-1,-1);

    m_nFixedCols = nFixedCols;

    if (GetSafeHwnd() && m_bAllowDraw)
        Invalidate();

    return TRUE;
}

BOOL CThemeGrid::SetRows(int nRows)
{
	return SetRowCount(nRows);
}

BOOL CThemeGrid::SetRowCount(int nRows)
{
    ASSERT(nRows >= 0);
    if (nRows == GetRowCount()) return TRUE;

    if (nRows < m_nFixedRows) 
        m_nFixedRows = nRows;

    if (m_idCurrentCell.row >= nRows)
        SetFocusCell(-1,-1);

	int nOldRowCount = GetRowCount();

    int addedRows = nRows - nOldRowCount;


    // If we are about to lose rows, then we need to delete the GridCell objects 
    // in each column within each row
    if (addedRows < 0) {
        for (int row = nRows; row < m_nRows; row++)
        {
            // Delete cells
            for (int col = 0; col < m_nCols; col++) 
            {
                CGridCell* pCell = GetCell(row, col);
                if (pCell) {
                    EmptyCell(pCell, row, col);
                    delete pCell;
                }
            }
            // Delete rows
            GRID_ROW* pRow = m_RowData[row];
            if (pRow) delete pRow;
        }
    }

    // Change the number of rows.
    m_nRows = nRows;
    m_RowData.SetSize(m_nRows);
    m_arRowHeights.SetSize(nRows);

    // If we have just added rows, we need to construct new elements for each cell
    // and set the default row height
    if (addedRows > 0) {
        // initialize row heights and data
        int startRow = nRows - addedRows;
        for (int row = startRow; row < GetRowCount(); row++) {
            m_arRowHeights[row] = m_nDefCellHeight;
            m_RowData[row] = new GRID_ROW;
            m_RowData[row]->SetSize(m_nCols);
            for (int col = 0; col < m_nCols; col++)
            {
                GRID_ROW* pRow = m_RowData[row];
                if (pRow) pRow->SetAt(col, CreateCell(row, col));
            }
        }
		
		int i=0, j=0, nCheckColCount=0;

		// 체크박스 컬럼 갯수
		nCheckColCount = m_arColumnCheckbox.GetSize();

		// 체크박스 컬럼에 이미지 표시
		for(j=0 ; j < nCheckColCount ; j++)
		{
			i = (nOldRowCount > GetFixedRowCount()) ? nOldRowCount : GetFixedRowCount();
			for( ; i < nRows ; i++)
			{
				SetItemImage(i, (int)m_arColumnCheckbox[j], 0);
			}
		}
    }
    //else
    //    ResetSelectedRange();
    
    if (GetSafeHwnd() && m_bAllowDraw)
    {
        ResetScrollBars();
        Invalidate();
    }
	
    return TRUE;
}

BOOL CThemeGrid::SetCols(int nCols)
{
	return SetColCount(nCols);
}

BOOL CThemeGrid::SetColCount(int nCols)
{
    ASSERT(nCols >= 0);

    if (nCols == GetColCount()) return TRUE;

	while (!m_ColInfoList.IsEmpty()) delete m_ColInfoList.RemoveHead();
	for (int c = 0; c < nCols; c++)
	{
		CColInfoObject *pCombo = new CColInfoObject;
		m_ColInfoList.AddTail(pCombo);
	}

    if (nCols < m_nFixedCols) 
        m_nFixedCols = nCols;

    if (m_idCurrentCell.col >= nCols)
        SetFocusCell(-1,-1);

    int addedCols = nCols - GetColCount();

    // If we are about to lose columns, then we need to delete the GridCell objects 
    // within each column
    if (addedCols < 0) {
        for (int row = 0; row < m_nRows; row++)
            for (int col = nCols; col < GetColCount(); col++)
            {
                CGridCell* pCell = GetCell(row, col);
                if (pCell) {
                    EmptyCell(pCell, row, col);
                    delete pCell;
                }
            }
    }

    // Change the number of columns.
    m_nCols = nCols;
    m_arColWidths.SetSize(nCols);

    // Change the number of columns in each row.
    for (int i = 0; i < m_nRows; i++)
        if (m_RowData[i]) m_RowData[i]->SetSize(nCols);

    // If we have just added columns, we need to construct new elements for each cell
    // and set the default column width
    if (addedCols > 0)
    {
        // initialized column widths
        int startCol = nCols - addedCols;
		int col=0;
        for (col = startCol; col < GetColCount(); col++)
            m_arColWidths[col] = m_nDefCellWidth;

        // initialise column data
        for (int row = 0; row < m_nRows; row++)
            for (col = startCol; col < GetColCount(); col++)
            {
                GRID_ROW* pRow = m_RowData[row];
                if (pRow) pRow->SetAt(col, CreateCell(row,col));
            }
    }
    //else    // check for selected cell ranges
    //    ResetSelectedRange();
    
    if (GetSafeHwnd() && m_bAllowDraw)
    {
        ResetScrollBars();
        Invalidate();
    }
    return TRUE;
}

// Insert a column at a given position, or add to end of columns (if nColumn = -1)
int CThemeGrid::InsertCol(LPCTSTR strHeading, 
                            UINT nFormat /* = DT_CENTER|DT_VCENTER|DT_SINGLELINE */,
                            int nColumn  /* = -1 */)
{
    // If the insertion is for a specific column, check it's within range.
    if (nColumn >= 0 && nColumn >= GetColCount())
        return -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the column.
    if (m_nRows < 1)
        SetRowCount(1);    

    if (nColumn < 0)
    {
        nColumn = m_nCols;
        m_arColWidths.Add(0);
        for (int row = 0; row < m_nRows; row++) 
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow) return -1;
            pRow->Add(CreateCell(row, nColumn));
        }
    } 
    else
    {
        m_arColWidths.InsertAt(nColumn, (UINT)0);
        for (int row = 0; row < m_nRows; row++) 
        {
            GRID_ROW* pRow = m_RowData[row];
            if (!pRow) return -1;
            pRow->InsertAt(nColumn, CreateCell(row, nColumn));
        }
    }

    m_nCols++;

    // Initialise column data
    SetItemText(0, nColumn, strHeading);
    for (int row = 0; row < m_nRows; row++) 
    {
        SetItemFormat(row, nColumn, nFormat);
    }

    // initialized column width
    m_arColWidths[nColumn] = GetTextExtent(strHeading).cx;

    if (m_idCurrentCell.col != -1 && nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col++;
    
    ResetScrollBars();

    return nColumn;
}

// Insert a row at a given position, or add to end of rows (if nRow = -1)
int CThemeGrid::InsertRow(LPCTSTR strHeading, int nRow /* = -1 */)
{
    // If the insertion is for a specific row, check it's within range.
    if (nRow >= 0 && nRow >= GetRowCount())
        return -1;

    ResetSelectedRange();

    // Gotta be able to at least _see_ some of the row.
    if (m_nCols < 1) 
        SetColCount(1);    

    // Adding a row to the bottom
    if (nRow < 0) 
    {
        nRow = m_nRows;
        m_arRowHeights.Add(0);
        m_RowData.Add(new GRID_ROW);
    } 
    else 
    {
        m_arRowHeights.InsertAt(nRow, (UINT)0);
        m_RowData.InsertAt(nRow, new GRID_ROW);
    }
    
    m_nRows++;
    m_RowData[nRow]->SetSize(m_nCols);

    // Initialise cell data
    for (int col = 0; col < m_nCols; col++)
    {
        GRID_ROW* pRow = m_RowData[nRow];
        if (!pRow) return -1;
        pRow->SetAt(col, CreateCell(nRow, col));
    }

    // Set row title
    SetItemText(nRow, 0, strHeading);

    // initialized row height
    m_arRowHeights[nRow] = GetTextExtent(strHeading).cy;

    if (m_idCurrentCell.row != -1 && nRow < m_idCurrentCell.row)
        m_idCurrentCell.row++;
    
    ResetScrollBars();

    return nRow;
}

// Creates a new grid cell and performs any necessary initialisation
CGridCell* CThemeGrid::CreateCell(int nRow, int nCol)
{
    CGridCell* pCell = new CGridCell;
    if (!pCell)
        return NULL;
    
    // Make format same as cell above
    //if (nRow > 0 && nCol >= 0 && nCol < m_nCols)
    //    pCell->nFormat = GetItemFormat(nRow-1, nCol);
	pCell->nFormat = m_nAlignFormat;
 
    // Make font default grid font
    memcpy(&(pCell->lfFont), &m_Logfont, sizeof(LOGFONT));
    
    return pCell;
}

// Performs any cell cleanup necessary to maintain grid integrity
void CThemeGrid::EmptyCell(CGridCell* pCell, int nRow, int nCol)
{
    // Set the cells state to 0. If the cell is selected, this
    // will remove the cell from the selected list.
    SetItemState(nRow, nCol, 0);

    // Empty strings
    pCell->szText.Empty();
}

BOOL CThemeGrid::DeleteCol(int nColumn)
{
    if (nColumn < 0 || nColumn >= GetColCount())
        return FALSE;

    ResetSelectedRange();

	POSITION pos = m_ColInfoList.FindIndex(nColumn);
	if (pos) m_ColInfoList.RemoveAt(pos);

    for (int row = 0; row < GetRowCount(); row++) 
    {
        GRID_ROW* pRow = m_RowData[row];
        if (!pRow) return FALSE;

        CGridCell* pCell = pRow->GetAt(nColumn);
        if (pCell) {
            EmptyCell(pCell, row, nColumn);
            delete pCell;
        }
        pRow->RemoveAt(nColumn);
    }
    m_arColWidths.RemoveAt(nColumn);
    m_nCols--;
    if (nColumn < m_nFixedCols) m_nFixedCols--;

    if (nColumn == m_idCurrentCell.col)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nColumn < m_idCurrentCell.col)
        m_idCurrentCell.col--;

    ResetScrollBars();

    return TRUE;
}

BOOL CThemeGrid::DeleteRow(int nRow)
{
    if ( nRow < 0 || nRow >= GetRowCount())
        return FALSE;

    GRID_ROW* pRow = m_RowData[nRow];
    if (!pRow) return FALSE;

    ResetSelectedRange();

    for (int col = 0; col < GetColCount(); col++) 
    {
        CGridCell* pCell = pRow->GetAt(col);
        if (pCell) {
            EmptyCell(pCell, nRow, col);
            delete pCell;
        }
    }
    delete pRow;
    m_RowData.RemoveAt(nRow);
    m_arRowHeights.RemoveAt(nRow);

    m_nRows--;
    if (nRow < m_nFixedRows) m_nFixedRows--;

    if (nRow == m_idCurrentCell.row)
        m_idCurrentCell.row = m_idCurrentCell.col = -1;
    else if (nRow < m_idCurrentCell.row)
        m_idCurrentCell.row--;

    ResetScrollBars();

    return TRUE;
}

// Handy function that removes all non-fixed rows
BOOL CThemeGrid::DeleteNonFixedRows()
{
    int nFixed = GetFixedRowCount();
    int nCount = GetRowCount();

    // Delete all data rows
    for (int nRow = nCount; nRow >= nFixed; nRow--) 
        DeleteRow(nRow);

    return TRUE;
}

// Removes all rows, columns and data from the grid.
BOOL CThemeGrid::DeleteAllItems()
{
    ResetSelectedRange();

	// add SKC ---------------------------
	while (!m_ColInfoList.IsEmpty()) delete m_ColInfoList.RemoveHead();
	while (!m_ColGroupList.IsEmpty()) delete m_ColGroupList.RemoveHead();
	// end SKC ---------------------------

    m_arColWidths.RemoveAll();
    m_arRowHeights.RemoveAll();

    // Delete all cells in the grid
    for (int row = 0; row < m_nRows; row++) 
    {
        GRID_ROW* pRow = m_RowData[row];
        if (!pRow) continue;
        for (int col = 0; col < m_nCols; col++)
        {
            CGridCell* pCell = pRow->GetAt(col);
            if (pCell) {
                EmptyCell(pCell, row, col);  // TODO - this is a bit of a performance hit.
                delete pCell;                // better to call m_SelectedCells.RemoveAll()
            }                                // instead. This is safer for changes though.
        }
        delete pRow;
    }

    // Remove all rows
    m_RowData.RemoveAll();

    m_idCurrentCell.row = m_idCurrentCell.col = -1;
    m_nRows = m_nFixedRows = m_nCols = m_nFixedCols = 0;

    ResetScrollBars();

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

// Set CListCtrl::GetNextItem for details
CCellID CThemeGrid::GetNextItem(CCellID& cell, int nFlags) const
{
    if (nFlags & GVNI_ALL)
    {	// GVNI_ALL Search whole Grid beginning from cell
		//          First row (cell.row) -- ONLY Columns to the right of cell
		//          following rows       -- ALL  Columns
        int row = cell.row , col = cell.col + 1; 
        if (row <= 0) row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            if (col <= 0 ) col = GetFixedColCount();
            for (; col < GetColCount(); col++) 
            {
                int nState = GetItemState(row, col);
                if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                    return CCellID(row, col);
                if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     
                    return CCellID(row, col);
                if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   
                    return CCellID(row, col);
                if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   
                    return CCellID(row, col);    
				if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
					return CCellID(row, col);
            }
			// go to First Column
			col=GetFixedColCount();
        }
    }
	else if ((nFlags & GVNI_BELOW) && (nFlags & GVNI_TORIGHT))
    {   // GVNI_AREA Search Grid beginning from cell to Lower-Right of Grid
		//           Only rows starting with  cell.row and below
		//           All rows   -- ONLY Columns to the right of cell
        int row = cell.row;
        if (row <= 0) row = GetFixedRowCount();
        for (; row < GetRowCount(); row++)
        {
            int col = cell.col + 1;
            if (col <= 0) col = GetFixedColCount();
            for (; col < GetColCount(); col++) 
            {
                int nState = GetItemState(row, col);
                if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                    return CCellID(row, col);
                if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)     
                    return CCellID(row, col);
                if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)   
                    return CCellID(row, col);
                if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)   
                    return CCellID(row, col);    
				if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
					return CCellID(row, col);
            }
        }
    }
    else if (nFlags & GVNI_ABOVE) 
    {
        for (int row = cell.row-1; row >= GetFixedRowCount(); row--) 
        {
            int nState = GetItemState(row, cell.col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED)
                return CCellID(row, cell.col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)    
                return CCellID(row, cell.col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)  
                return CCellID(row, cell.col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY)  
                return CCellID(row, cell.col);
            if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
                return CCellID(row, cell.col);
        }
    }
    else if (nFlags & GVNI_BELOW)
    {
        for (int row = cell.row+1; row < GetRowCount(); row++) 
        {
            int nState = GetItemState(row, cell.col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                return CCellID(row, cell.col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)   
                return CCellID(row, cell.col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)  
                return CCellID(row, cell.col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY) 
                return CCellID(row, cell.col);
            if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
                return CCellID(row, cell.col);
        }
    } 
    else if (nFlags & GVNI_TOLEFT)
    {
        for (int col = cell.col-1; col >= GetFixedColCount(); col--) 
        {
            int nState = GetItemState(cell.row, col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                return CCellID(cell.row, col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)    
                return CCellID(cell.row, col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED)  
                return CCellID(cell.row, col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY) 
                return CCellID(cell.row, col);
            if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
                return CCellID(cell.row, col);
       }
    }
    else if (nFlags & GVNI_TORIGHT) 
    {
        for (int col = cell.col+1; col < GetColCount(); col++) 
        {
            int nState = GetItemState(cell.row, col);
            if (nFlags & GVNI_DROPHILITED && nState & GVIS_DROPHILITED) 
                return CCellID(cell.row, col);
            if (nFlags & GVNI_FOCUSED && nState & GVIS_FOCUSED)  
                return CCellID(cell.row, col);
            if (nFlags & GVNI_SELECTED && nState & GVIS_SELECTED) 
                return CCellID(cell.row, col);
            if (nFlags & GVNI_READONLY && nState & GVIS_READONLY) 
                return CCellID(cell.row, col);
            if (nFlags & GVNI_MODIFIED && nState & GVIS_MODIFIED)   
                return CCellID(cell.row, col);
        }
    }

    return CCellID(-1, -1);
}

// Sorts on a given column using the cell text
BOOL CThemeGrid::SortTextItems(int nCol, BOOL bAscending)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortTextItems(nCol, bAscending, GetFixedRowCount(),-1);
}

// recursive sort implementation
BOOL CThemeGrid::SortTextItems(int nCol, BOOL bAscending, int low, int high)
{
    if (nCol >= GetColCount()) return FALSE;

    if (high == -1) high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    CString midItem = GetItemText( (lo+hi)/2, nCol );

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && GetItemText(lo, nCol) < midItem)
                ++lo;
        else
            while (lo < high && GetItemText(lo, nCol) > midItem)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && GetItemText(hi, nCol) > midItem)
                --hi;
        else
            while (hi > low && GetItemText(hi, nCol) < midItem)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (GetItemText(lo, nCol) != GetItemText(hi, nCol))
            {
                for (int col = 0; col < GetColCount(); col++)
                {
                    CGridCell *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);
                }
                UINT nRowHeight = m_arRowHeights[lo];
                m_arRowHeights[lo] = m_arRowHeights[hi];
                m_arRowHeights[hi] = nRowHeight;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortTextItems(nCol, bAscending, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortTextItems(nCol, bAscending, lo, high);

    return TRUE;
}

// Sorts on a given column using the supplied compare function (see CListCtrl::SortItems)
BOOL CThemeGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, 
                          LPARAM data /* = 0 */)
{
    ResetSelectedRange();
    SetFocusCell(-1,-1);
    return SortItems(pfnCompare, nCol, bAscending, data, GetFixedRowCount(), -1);
}

// recursive sort implementation
BOOL CThemeGrid::SortItems(PFNLVCOMPARE pfnCompare, int nCol, BOOL bAscending, LPARAM data,
                          int low, int high)
{
    if (nCol >= GetColCount()) return FALSE;

    if (high == -1) high = GetRowCount() - 1;

    int lo = low;
    int hi = high;

    if( hi <= lo ) return FALSE;

    LPARAM midItem = GetItemData( (lo+hi)/2, nCol );

    // loop through the list until indices cross
    while( lo <= hi )
    {
        // Find the first element that is greater than or equal to the partition 
        // element starting from the left Index.
        if( bAscending )
            while (lo < high  && pfnCompare(GetItemData(lo, nCol), midItem, data) < 0)
                ++lo;
        else
            while (lo < high && pfnCompare(GetItemData(lo, nCol), midItem, data) > 0)
                ++lo;

        // Find an element that is smaller than or equal to  the partition 
        // element starting from the right Index.
        if( bAscending )
            while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) > 0)
                --hi;
        else
            while (hi > low && pfnCompare(GetItemData(hi, nCol), midItem, data) < 0)
                --hi;

        // If the indexes have not crossed, swap if the items are not equal
        if (lo <= hi)
        {
            // swap only if the items are not equal
            if (pfnCompare(GetItemData(lo, nCol), GetItemData(hi, nCol), data) != 0)
            {
                for (int col = 0; col < GetColCount(); col++)
                {
                    CGridCell *pCell = GetCell(lo, col);
                    SetCell(lo, col, GetCell(hi, col));
                    SetCell(hi, col, pCell);
                }
                UINT nRowHeight = m_arRowHeights[lo];
                m_arRowHeights[lo] = m_arRowHeights[hi];
                m_arRowHeights[hi] = nRowHeight;
            }

            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if( low < hi )
        SortItems(pfnCompare, nCol, bAscending, data, low, hi);

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if( lo < high )
        SortItems(pfnCompare, nCol, bAscending, data, lo, high);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl data functions

BOOL CThemeGrid::SetItem(const GV_ITEM* pItem)
{
    if (!pItem) 
        return FALSE;
    CGridCell* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell) 
        return FALSE;

    if (pItem->mask & GVIF_TEXT)   pCell->szText  = pItem->szText;
    if (pItem->mask & GVIF_PARAM)  pCell->lParam  = pItem->lParam;
    if (pItem->mask & GVIF_IMAGE)  pCell->iImage  = pItem->iImage;
    if (pItem->mask & GVIF_STATE)  pCell->state   = pItem->state;
    if (pItem->mask & GVIF_FORMAT) pCell->nFormat = pItem->nFormat;
    if (pItem->mask & GVIF_BKCLR)  pCell->crBkClr = pItem->crBkClr;
    if (pItem->mask & GVIF_FGCLR)  pCell->crFgClr = pItem->crFgClr;
    if (pItem->mask & GVIF_FONT)   memcpy(&(pCell->lfFont), &(pItem->lfFont), sizeof(LOGFONT));

    return TRUE;
}

BOOL CThemeGrid::GetItem(GV_ITEM* pItem)
{
    if (!pItem)
        return FALSE;
    CGridCell* pCell = GetCell(pItem->row, pItem->col);
    if (!pCell) 
        return FALSE;

    if (pItem->mask & GVIF_TEXT)   pItem->szText  = GetItemText(pItem->row, pItem->col);
    if (pItem->mask & GVIF_PARAM)  pItem->lParam  = pCell->lParam;
    if (pItem->mask & GVIF_IMAGE)  pItem->iImage  = pCell->iImage;
    if (pItem->mask & GVIF_STATE)  pItem->state   = pCell->state;
    if (pItem->mask & GVIF_FORMAT) pItem->nFormat = pCell->nFormat;
    if (pItem->mask & GVIF_BKCLR)  pItem->crBkClr = pCell->crBkClr;
    if (pItem->mask & GVIF_FGCLR)  pItem->crFgClr = pCell->crFgClr;
    if (pItem->mask & GVIF_FONT)   memcpy(&(pItem->lfFont), &(pCell->lfFont), sizeof(LOGFONT));

    return TRUE;
}

LPARAM CThemeGrid::GetItemData(int nRow, int nCol) const
{    
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) 
        return (LPARAM) 0;

    return pCell->lParam;
}

BOOL CThemeGrid::SetItemImage(int nRow, int nCol, int iImage)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) 
        return FALSE;

    pCell->iImage = iImage;
    return TRUE;
}

int CThemeGrid::GetItemImage(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) 
        return -1;

    return pCell->iImage;
}

BOOL CThemeGrid::SetItemState(int nRow, int nCol, UINT state)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    // If the cell is being unselected, remove it from the selected list
    if ((pCell->state & GVIS_SELECTED) && !(state & GVIS_SELECTED))
    {
        CCellID cell;
        DWORD key = MAKELONG(nRow, nCol);

        if (m_SelectedCellMap.Lookup(key, (CCellID&)cell))
            m_SelectedCellMap.RemoveKey(key);
    }

    // If cell is being selected, add it to the list of selected cells
    else if (!(pCell->state & GVIS_SELECTED) && (state & GVIS_SELECTED))
    {
        CCellID cell(nRow, nCol);
        m_SelectedCellMap.SetAt(MAKELONG(nRow, nCol), cell);
    }

    // Set the cell's state
    pCell->state = state;

    return TRUE;
}

UINT CThemeGrid::GetItemState(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->state;
}

BOOL CThemeGrid::SetItemFormat(int nRow, int nCol, UINT nFormat)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) 
        return FALSE;

    pCell->nFormat = nFormat;
    return TRUE;
}

UINT CThemeGrid::GetItemFormat(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->nFormat;
}

BOOL CThemeGrid::SetItemBkColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) 
        return FALSE;

    pCell->crBkClr = cr;
    return TRUE;
}

COLORREF CThemeGrid::GetItemBkColour(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->crBkClr;
}

BOOL CThemeGrid::SetItemFgColour(int nRow, int nCol, COLORREF cr /* = CLR_DEFAULT */)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    pCell->crFgClr = cr;
    return TRUE;
}

COLORREF CThemeGrid::GetItemFgColour(int nRow, int nCol) const
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return 0;

    return pCell->crFgClr;
}

BOOL CThemeGrid::SetItemFont(int nRow, int nCol, LOGFONT* lf)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell)
        return FALSE;

    memcpy(&(pCell->lfFont), lf, sizeof(LOGFONT));
    return TRUE;
}

LOGFONT* CThemeGrid::GetItemFont(int nRow, int nCol)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) 
        return &m_Logfont;

    return &(pCell->lfFont);
}

////////////////////////////////////////////////////////////////////////////////////
// Row/Column size functions

long CThemeGrid::GetVirtualWidth() const
{
    long lVirtualWidth = 0;
    int iColCount = GetColCount();
    for (int i = 0; i < iColCount; i++)
        lVirtualWidth += m_arColWidths[i];

    return lVirtualWidth;
}

long CThemeGrid::GetVirtualHeight() const
{
    long lVirtualHeight = 0;
    int iRowCount = GetRowCount();
    for (int i = 0; i < iRowCount; i++)
        lVirtualHeight += m_arRowHeights[i];

    return lVirtualHeight;
}

int CThemeGrid::GetRowHeight(int nRow) const
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows) return -1;

    return m_arRowHeights[nRow];
}

int CThemeGrid::GetColWidth(int nCol) const
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols) return -1;

    return m_arColWidths[nCol];
}

BOOL CThemeGrid::SetRowHeight(int nRow, int height)
{
    ASSERT(nRow >= 0 && nRow < m_nRows && height >= 0);
    if (nRow < 0 || nRow >= m_nRows || height < 0) return FALSE;

    m_arRowHeights[nRow] = height;    
    return TRUE;
}

BOOL CThemeGrid::SetColWidth(int nCol, int width)
{
    ASSERT(nCol >= 0 && nCol < m_nCols && width >= 0);
    if (nCol < 0 || nCol >= m_nCols || width < 0) return FALSE;

    m_arColWidths[nCol] = width;
    return TRUE;
}

int CThemeGrid::GetFixedRowHeight() const
{
    int nHeight = 0;
    for (int i = 0; i < m_nFixedRows; i++)
        nHeight += GetRowHeight(i);

    return nHeight;
}

int CThemeGrid::GetFixedColWidth() const
{
    int nWidth = 0;
    for (int i = 0; i < m_nFixedCols; i++)
        nWidth += GetColWidth(i);

    return nWidth;
}

BOOL CThemeGrid::AutoSizeCol(int nCol)
{
    ASSERT(nCol >= 0 && nCol < m_nCols);
    if (nCol < 0 || nCol >= m_nCols) return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC) return FALSE;

    int nWidth = 0;
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
    {
        size = GetCellExtent(nRow, nCol, pDC);
        if (size.cx > nWidth) nWidth = size.cx;
    }

    m_arColWidths[nCol] = nWidth;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

BOOL CThemeGrid::AutoSizeRow(int nRow)
{
    ASSERT(nRow >= 0 && nRow < m_nRows);
    if (nRow < 0 || nRow >= m_nRows) return FALSE;

    CSize size;
    CDC* pDC = GetDC();
    if (!pDC) return FALSE;

    int nHeight = 0;
    int nNumColumns = GetColCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
    {  
        size = GetCellExtent(nRow, nCol, pDC);
        if (size.cy > nHeight) nHeight = size.cy;
    }  
    m_arRowHeights[nRow] = nHeight;

    ReleaseDC(pDC);
    ResetScrollBars();

    return TRUE;
}

void CThemeGrid::AutoSizeCols()
{
    int nNumColumns = GetColCount();
    for (int nCol = 0; nCol < nNumColumns; nCol++)
        AutoSizeCol(nCol);
}

void CThemeGrid::AutoSizeRows()
{
    int nNumRows = GetRowCount();
    for (int nRow = 0; nRow < nNumRows; nRow++)
        AutoSizeRow(nRow);
}

// sizes all rows and columns
// faster than calling both AutoSizeCols() and AutoSizeRows()
void CThemeGrid::AutoSize()
{
    CDC* pDC = GetDC();
    if (!pDC) return;

    int nNumColumns = GetColCount();
    int nNumRows = GetRowCount();
    
	int nCol=0, nRow=0;
    // initialize column widths to zero
    for (nCol = 0; nCol < nNumColumns; nCol++)
        m_arColWidths[nCol] = 0;
    
    // initialize row heights to zero
    for (nRow = 0; nRow < nNumRows; nRow++)
        m_arRowHeights[nRow] = 0;
    
    CSize size;
    for (nCol = 0; nCol < nNumColumns; nCol++)
        for (nRow = 0; nRow < nNumRows; nRow++)
        {
            size = GetCellExtent(nRow, nCol, pDC);
            if (size.cx > (int) m_arColWidths[nCol])  m_arColWidths[nCol] = size.cx;
            if (size.cy > (int) m_arRowHeights[nRow]) m_arRowHeights[nRow] = size.cy;
        }
    
    ReleaseDC(pDC);

    if (m_bAllowDraw) {
        ResetScrollBars();
        Invalidate();
    }
}

void CThemeGrid::ExpandColsToFit()
{
    if (GetColCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() - (int) virtualWidth;
    int nColumnAdjustment = nDifference / GetColCount();
	int i=0;

    for (i = 0; i < GetColCount(); i++)
       m_arColWidths[i] += nColumnAdjustment;

    if (nDifference > 0)
    {
        int leftOver = nDifference % GetColCount();
        for (i = 0; i < leftOver; i++)
            m_arColWidths[i] += 1;
    } 
    else 
    {
        int leftOver = (-nDifference) % GetColCount();
        for (i = 0; i < leftOver; i++)
            m_arColWidths[i] -= 1;
    }

    if (m_bAllowDraw) 
        Invalidate();
}

void CThemeGrid::ExpandRowsToFit()
{
    if (GetRowCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualHeight = GetVirtualHeight();
    int nDifference = rect.Height() - (int) virtualHeight;
    int nRowAdjustment = nDifference / GetRowCount();
	int i=0;

    for (i = 0; i < GetRowCount(); i++)
       m_arRowHeights[i] += nRowAdjustment;

    if (nDifference > 0)
    {
        int leftOver = nDifference % GetRowCount();
        for (i = 0; i < leftOver; i++)
            m_arRowHeights[i] += 1;
    } 
    else 
    {
        int leftOver = (-nDifference) % GetRowCount();
        for (i = 0; i < leftOver; i++)
            m_arRowHeights[i] -= 1;
    }

    if (m_bAllowDraw) 
        Invalidate();
}

void CThemeGrid::ExpandToFit()
{
    ExpandColsToFit();   // This will remove any existing horz scrollbar
    ExpandRowsToFit();      // This will remove any existing vert scrollbar
    ExpandColsToFit();   // Just in case the first adjustment was with a vert
                            // scrollbar in place
}

BOOL CThemeGrid::IsCellFixed(int nRow, int nCol)
{
    return (nRow < GetFixedRowCount() || nCol < GetFixedColCount());
}

void CThemeGrid::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
    if (nRow > 0 && nCol > 0)
    {
        if (bModified)
        {
	        SetItemState(nRow, nCol, GetItemState(nRow, nCol) | GVIS_MODIFIED);
            m_bModified = TRUE; 
        }
        else
	        SetItemState(nRow, nCol, GetItemState(nRow, nCol) & ~GVIS_MODIFIED);
    }
    else
        m_bModified = bModified;

    if (!m_bModified)
    {
        for (int row = 0; row < GetRowCount(); row++)
            for (int col = 0; col < GetColCount(); col++) 
                SetItemState(row, col, GetItemState(row, col) & ~GVIS_MODIFIED);
    }
}

BOOL CThemeGrid::GetModified(int nRow /*=-1*/, int nCol /*=-1*/)
{
    if (nRow > 0 && nCol > 0)
        return ( (GetItemState(nRow, nCol) & GVIS_MODIFIED) == GVIS_MODIFIED );
    else
        return m_bModified;               
}

/////////////////////////////////////////////////////////////////////////////////////
// GridCtrl cell visibility GridCtrlExs and invalidation/redraw functions

// EnsureVisible supplied by Roelf Werkman
void CThemeGrid::EnsureVisible(int nRow, int nCol)
{
    CCellRange VisibleCells = GetVisibleNonFixedCellRange();

    int right = nCol - VisibleCells.GetMaxCol();
    int left  = VisibleCells.GetMinCol() - nCol;
    int down  = nRow - VisibleCells.GetMaxRow();
    int up    = VisibleCells.GetMinRow() - nRow;

    while (right > 0)
    {
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
        right--;
    }
    while (left > 0)
    {
        SendMessage(WM_HSCROLL, SB_LINELEFT, 0);
        left--;
    }
    while (down > 0)
    {
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
        down--;
   }
   while (up > 0)
   {
       SendMessage(WM_VSCROLL, SB_LINEUP, 0);
       up--;
   }

   // Move one more if we only see a snall bit of the cell
   CRect rectCell, rectWindow;
   GetCellRect(nRow, nCol, rectCell);
   GetClientRect(rectWindow);
   if (rectCell.right > rectWindow.right)
        SendMessage(WM_HSCROLL, SB_LINERIGHT, 0);
   if (rectCell.bottom > rectWindow.bottom)
        SendMessage(WM_VSCROLL, SB_LINEDOWN, 0);
}

BOOL CThemeGrid::IsCellEditable(CCellID &cell) const
{
    return IsCellEditable(cell.row, cell.col); 
}

BOOL CThemeGrid::IsCellEditable(int nRow, int nCol) const
{
    return IsEditable() && ((GetItemState(nRow, nCol) & GVIS_READONLY) != GVIS_READONLY);
}

BOOL CThemeGrid::IsCellVisible(CCellID cell) const
{  
    return IsCellVisible(cell.row, cell.col);
}

BOOL CThemeGrid::IsCellVisible(int nRow, int nCol) const
{  
    if (!IsWindow(m_hWnd))
        return FALSE;

    int x,y;

    CCellID TopLeft;
    if (nCol >= GetFixedColCount() || nRow >= GetFixedRowCount())
    {
        TopLeft = GetTopleftNonFixedCell();
        if (nCol >= GetFixedColCount() && nCol < TopLeft.col) return FALSE;
        if (nRow >= GetFixedRowCount() && nRow < TopLeft.row) return FALSE;
    }

    CRect rect;
    GetClientRect(rect);
    if (nCol < GetFixedColCount())
    {
        x = 0;
        for (int i = 0; i <= nCol; i++) 
        {
            if (x >= rect.right) return FALSE;
            x += GetColWidth(i);    
        }
    } 
    else 
    {
        x = GetFixedColWidth();
        for (int i = TopLeft.col; i <= nCol; i++) 
        {
            if (x >= rect.right) return FALSE;
            x += GetColWidth(i);    
        }
    }

    if (nRow < GetFixedRowCount())
    {
        y = 0;
        for (int i = 0; i <= nRow; i++) 
        {
            if (y >= rect.bottom) return FALSE;
            y += GetRowHeight(i);    
        }
    } 
    else 
    {
        if (nRow < TopLeft.row) return FALSE;
        y = GetFixedRowHeight();
        for (int i = TopLeft.row; i <= nRow; i++) 
        {
            if (y >= rect.bottom) return FALSE;
            y += GetRowHeight(i);    
        }
    }

    return TRUE;
}

BOOL CThemeGrid::InvalidateCellRect(const CCellID& cell)
{
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw)
        return FALSE;

    ASSERT(IsValid(cell));

    if (!IsCellVisible(cell.row, cell.col)) return FALSE;

    CRect rect;
    if (!GetCellRect(cell, rect)) return FALSE;
    rect.right++; rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

BOOL CThemeGrid::InvalidateCellRect(const CCellRange& cellRange)
{
    ASSERT(IsValid(cellRange));
    if (!::IsWindow(GetSafeHwnd()) || !m_bAllowDraw) return FALSE;

    CCellRange visibleCellRange = GetVisibleNonFixedCellRange().Intersect(cellRange);

    CRect rect;
    if (!GetCellRangeRect(visibleCellRange, rect)) return FALSE;

    rect.right++; rect.bottom++;
    InvalidateRect(rect, TRUE);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl Mouse stuff

// Handles mouse wheel notifications
// Note - if this doesn't work for win95 then use OnRegisteredMouseWheel instead
#if !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)
BOOL CThemeGrid::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
    // A m_nRowsPerWheelNotch value less than 0 indicates that the mouse
    // wheel scrolls whole pages, not just lines.
    if (m_nRowsPerWheelNotch == -1)
    {
        int nPagesScrolled = zDelta / 120;

        if (nPagesScrolled > 0)
            for (int i = 0; i < nPagesScrolled; i++)
                PostMessage(WM_VSCROLL, SB_PAGEUP, 0);
        else
            for (int i = 0; i > nPagesScrolled; i--)
                PostMessage(WM_VSCROLL, SB_PAGEDOWN, 0);
    }
    else
    {
        int nRowsScrolled = m_nRowsPerWheelNotch * zDelta / 120;

        if (nRowsScrolled > 0)
            for (int i = 0; i < nRowsScrolled; i++)
                PostMessage(WM_VSCROLL, SB_LINEUP, 0);
        else
            for (int i = 0; i > nRowsScrolled; i--)
                PostMessage(WM_VSCROLL, SB_LINEDOWN, 0);
    }

    return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
#endif // !defined(_WIN32_WCE) && (_MFC_VER >= 0x0421)

void CThemeGrid::OnMouseMove(UINT nFlags, CPoint point)
{
    CRect rect;
    GetClientRect(rect);

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If outside client area, return (unless we are drag n dropping)
    if (m_MouseMode != MOUSE_DRAGGING && !rect.PtInRect(point))
        return;
#endif

    // If the left mouse button is up, then GridCtrlEx to see if row/column sizing is imminent
    if (!(nFlags & MK_LBUTTON) 
        || (m_MouseMode == MOUSE_NOTHING && (nFlags & MK_LBUTTON)))
    {
        if (m_bAllowColumnResize && MouseOverColResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
                m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (m_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
                m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        else if (m_MouseMode != MOUSE_NOTHING)
        {
            SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
            m_MouseMode = MOUSE_NOTHING;
        }

/*#ifndef GRIDCONTROL_NO_TITLETIPS
        if (m_MouseMode == MOUSE_NOTHING && m_bTitleTips)
        {
            CCellID idCurrentCell = GetCellFromPt(point);
            CRect TextRect, CellRect;
            if (GetTextRect(idCurrentCell.row, idCurrentCell.col, TextRect) &&
                GetCellRect(idCurrentCell.row, idCurrentCell.col, CellRect))
                m_TitleTip.Show(TextRect, 
                                GetItemText(idCurrentCell.row, idCurrentCell.col), 
                                0,
                                CellRect,
                                GetItemFont(idCurrentCell.row, idCurrentCell.col));
        }
#endif*/

        m_LastMousePoint = point;
        return;
    }

    if (!IsValid(m_LeftClickDownCell))
    {
        m_LastMousePoint = point;
        return;
    }

    // If the left mouse button is down, then process appropriately
    if (nFlags & MK_LBUTTON) 
    {
        switch(m_MouseMode)
        {
            case MOUSE_SELECT_ALL:        
                break;

            case MOUSE_SELECT_COL:
            case MOUSE_SELECT_ROW:    
            case MOUSE_SELECT_CELLS:    
                {
                    CCellID idCurrentCell = GetCellFromPt(point);
                    if (!IsValid(idCurrentCell)) 
                        return;

                    if (idCurrentCell != GetFocusCell())
                    {
                        OnSelecting(idCurrentCell);
                        //SetFocusCell(max(idCurrentCell.row, m_nFixedRows),
                        //             max(idCurrentCell.col, m_nFixedCols));
                        if (idCurrentCell.row >= m_nFixedRows &&
                            idCurrentCell.col >= m_nFixedCols)
                        {
                            SetFocusCell(idCurrentCell);
                        }
                    }
                    break;
                }

            case MOUSE_SIZING_COL:   
                {
                    CDC* pDC = GetDC();
                    if (!pDC) break;
                    
                    CRect oldInvertedRect(m_LastMousePoint.x, rect.top, 
                        m_LastMousePoint.x + 2, rect.bottom);
                    pDC->InvertRect(&oldInvertedRect);
                    CRect newInvertedRect(point.x, rect.top, 
                        point.x + 2, rect.bottom);
                    pDC->InvertRect(&newInvertedRect);
                    ReleaseDC(pDC);
                }
                break;
                
            case MOUSE_SIZING_ROW:        
                {
                    CDC* pDC = GetDC();
                    if (!pDC) break;
                    
                    CRect oldInvertedRect(rect.left, m_LastMousePoint.y, 
                        rect.right, m_LastMousePoint.y + 2);
                    pDC->InvertRect(&oldInvertedRect);
                    CRect newInvertedRect(rect.left, point.y, 
                        rect.right, point.y + 2);
                    pDC->InvertRect(&newInvertedRect);
                    ReleaseDC(pDC);
                }
                break;

#ifndef GRIDCONTROL_NO_DRAGDROP
            case MOUSE_PREPARE_DRAG:   
                OnBeginDrag();    
                break;
#endif
        }    
    }

    m_LastMousePoint = point;
}

void CThemeGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) 
    {
        CCellID cell = GetCellFromPt(point);
        ASSERT(IsValid(cell));

        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start)) return;

        if (point.x - start.x <= m_nResizeCaptureRange)     // Clicked right of border
            cell.col--;

        AutoSizeCol(cell.col);
        Invalidate();
    } 
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE)
    {
        CCellID cell = GetCellFromPt(point);
        ASSERT(IsValid(cell));

        CPoint start;
        if (!GetCellOrigin(0, cell.col, &start)) return;

        if (point.y - start.y <= m_nResizeCaptureRange)     // Clicked below border
            cell.row--;

        AutoSizeRow(cell.row);
        Invalidate();
    }
    else if (m_MouseMode == MOUSE_NOTHING)
    {
        if (m_LeftClickDownCell.row >= m_nFixedRows && 
            IsValid(m_LeftClickDownCell) &&
            m_LeftClickDownCell.col >= m_nFixedCols)
        {
			// 마우스 더블 클릭 메시지를 부모 윈도우로 전달
			if( m_bBaseCtrlList )
				SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, NM_DBLCLK);
			else
				SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_DOUBLECLICKED);

            OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, VK_LBUTTON);
        }
        else if (m_bListMode)
        {
            CCellID cell = GetCellFromPt(point);
            if (!IsValid(cell)) return;
            if (cell.row >= m_nFixedRows && cell.col < GetFixedColCount())
                OnEditCell(cell.row, cell.col, VK_LBUTTON);
        }
    }

    CWnd::OnLButtonDblClk(nFlags, point);
}

void CThemeGrid::OnLButtonDown(UINT nFlags, CPoint point)
{
    HWND hOldFocusWnd = ::GetFocus();

    m_LeftClickDownPoint = point;
    m_LeftClickDownCell = GetCellFromPt(point);

	// 배경을 클릭했다면
    if (!IsValid(m_LeftClickDownCell))
	{
		SetFocus();        // Auto-destroy any InPlaceEdit's
		return;
	}

    m_SelectionStartCell = (nFlags & MK_SHIFT)? m_idCurrentCell : m_LeftClickDownCell;

    SetFocus();        // Auto-destroy any InPlaceEdit's

    // If the user clicks on the current cell, then prepare to edit it.
    // (If the user moves the mouse, then dragging occurs)
    if (m_LeftClickDownCell == m_idCurrentCell)
    {
        m_MouseMode = MOUSE_PREPARE_EDIT;
        return;
    }   
    else if (m_MouseMode != MOUSE_OVER_COL_DIVIDE &&
             m_MouseMode != MOUSE_OVER_ROW_DIVIDE)        
    {
        SetFocusCell(-1,-1);
        SetFocusCell(max(m_LeftClickDownCell.row, m_nFixedRows),
                     max(m_LeftClickDownCell.col, m_nFixedCols));
    }

#ifndef GRIDCONTROL_NO_DRAGDROP
    // If the user clicks on a selected cell, then prepare to drag it.
    // (If the user moves the mouse, then dragging occurs)
    if (m_bAllowDragAndDrop && hOldFocusWnd == GetSafeHwnd() && 
        GetItemState(m_LeftClickDownCell.row, m_LeftClickDownCell.col) & GVNI_SELECTED)
    {
        m_MouseMode = MOUSE_PREPARE_DRAG;
        return;
    }
#endif

    SetCapture();

    if (m_MouseMode == MOUSE_NOTHING)
    {
        if (m_bAllowColumnResize && MouseOverColResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_COL_DIVIDE)
            {
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
                m_MouseMode = MOUSE_OVER_COL_DIVIDE;
            }
        }
        else if (m_bAllowRowResize && MouseOverRowResizeArea(point))
        {
            if (m_MouseMode != MOUSE_OVER_ROW_DIVIDE)
            {
                SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZENS));
                m_MouseMode = MOUSE_OVER_ROW_DIVIDE;
            }
        }
        //else if (m_MouseMode != MOUSE_NOTHING)
        //{
        //    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
        //    m_MouseMode = MOUSE_NOTHING;
        //}
    }

    if (m_MouseMode == MOUSE_OVER_COL_DIVIDE) // sizing column
    {
        m_MouseMode = MOUSE_SIZING_COL;
        CPoint start;
        if (!GetCellOrigin(0, m_LeftClickDownCell.col, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(point.x, rect.top, point.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.x - start.x <= m_nResizeCaptureRange)        // clicked right of border
            if (!GetCellOrigin(0, --m_LeftClickDownCell.col, &start)) return;

        rect.left = start.x;
        ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else if (m_MouseMode == MOUSE_OVER_ROW_DIVIDE) // sizing row
    {
        m_MouseMode = MOUSE_SIZING_ROW;
        CPoint start;
        if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, point.y, rect.right, point.y + 2);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (point.y - start.y <= m_nResizeCaptureRange)            // clicked below border
            if (!GetCellOrigin(--m_LeftClickDownCell.row, 0, &start)) return;

        rect.top = start.y;
        ClientToScreen(rect);
#ifndef _WIN32_WCE_NO_CURSOR
        ClipCursor(rect);
#endif
    }
    else // not sizing or editing -- selecting
    {    
        // If Ctrl pressed, save the current cell selection. This will get added
        // to the new cell selection at the end of the cell selection process
        m_PrevSelectedCellMap.RemoveAll();
        if (nFlags & MK_CONTROL) {
            for (POSITION pos = m_SelectedCellMap.GetStartPosition(); pos != NULL; )
            {
                DWORD key;
                CCellID cell;
                m_SelectedCellMap.GetNextAssoc(pos, key, (CCellID&)cell);
                m_PrevSelectedCellMap.SetAt(key, cell);
            }
        }
        
        if (m_LeftClickDownCell.row < GetFixedRowCount())
		{
			// 헤더 클릭 메시지를 부모 윈도우로 전달
			if( !m_bBaseCtrlList )
				SendMessageToParent(m_LeftClickDownCell.row, m_LeftClickDownCell.col, GVN_COLUMNCLICK);

            OnFixedRowClick(m_LeftClickDownCell);
		}
        else if (m_LeftClickDownCell.col < GetFixedColCount())
            OnFixedColClick(m_LeftClickDownCell);
        else
        {
            m_MouseMode = m_bListMode? MOUSE_SELECT_ROW : MOUSE_SELECT_CELLS;
            OnSelecting(m_LeftClickDownCell);
			
			// 마우스 클릭으로 에디트 한다면
			if( !m_bEditDblClk )
				//정상수 추가
				OnLButtonDown(nFlags, point);
        }

        m_nTimerID = SetTimer(WM_LBUTTONDOWN, m_nTimerInterval, 0);

    }   
    m_LastMousePoint = point;
}

void CThemeGrid::OnLButtonUp(UINT nFlags, CPoint point)
{
    CWnd::OnLButtonUp(nFlags, point);

#ifndef _WIN32_WCE_NO_CURSOR
    ClipCursor(NULL);
#endif

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
    {
        ReleaseCapture();
        KillTimer(m_nTimerID);
        m_nTimerID = 0;
    }

    // m_MouseMode == MOUSE_PREPARE_EDIT only if user clicked down on current cell
    // and then didn't move mouse before clicking up (releasing button)
    if (m_MouseMode == MOUSE_PREPARE_EDIT)    
    {
        OnEditCell(m_idCurrentCell.row, m_idCurrentCell.col, VK_LBUTTON);
    }
#ifndef GRIDCONTROL_NO_DRAGDROP
    // m_MouseMode == MOUSE_PREPARE_DRAG only if user clicked down on a selected cell
    // and then didn't move mouse before clicking up (releasing button)
    else if (m_MouseMode == MOUSE_PREPARE_DRAG) 
    {
        ResetSelectedRange();
    }
#endif
    else if (m_MouseMode == MOUSE_SIZING_COL)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(m_LastMousePoint.x, rect.top, m_LastMousePoint.x + 2, rect.bottom);

        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }

        if (m_LeftClickDownPoint != point) 
        {   
            CPoint start;
            if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;
            SetColWidth(m_LeftClickDownCell.col, point.x - start.x);
            ResetScrollBars();
            Invalidate();
        }
    }
    else if (m_MouseMode == MOUSE_SIZING_ROW)
    {
        CRect rect;
        GetClientRect(rect);
        CRect invertedRect(rect.left, m_LastMousePoint.y, rect.right, m_LastMousePoint.y + 2);
    
        CDC* pDC = GetDC();
        if (pDC) {
            pDC->InvertRect(&invertedRect);
            ReleaseDC(pDC);
        }
    
        if (m_LeftClickDownPoint != point) 
        {
            CPoint start;
            if (!GetCellOrigin(m_LeftClickDownCell, &start)) return;

            SetRowHeight(m_LeftClickDownCell.row, point.y - start.y);
            ResetScrollBars();
            Invalidate();
        }
    } 

    m_MouseMode = MOUSE_NOTHING;

#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif

    if (!IsValid(m_LeftClickDownCell)) return;

	// 마우스 클릭 메시지를 부모 윈도우로 전달
	if( m_bBaseCtrlList )
		SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, NM_CLICK);
	else
		SendMessageToParent(m_idCurrentCell.row, m_idCurrentCell.col, GVN_BUTTONDOWN);
	
    /*CWnd *pOwner = GetOwner();
    if (pOwner && IsWindow(pOwner->m_hWnd))
        pOwner->PostMessage(WM_COMMAND, MAKELONG(GetDlgCtrlID(), BN_CLICKED), 
                            (LPARAM) GetSafeHwnd());*/
}

#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing 

void CThemeGrid::Print() 
{
    CDC dc;
    CPrintDialog printDlg(FALSE);

    if (printDlg.DoModal() != IDOK)             // Get printer settings from user
        return;

    dc.Attach(printDlg.GetPrinterDC());         // attach a printer DC
    dc.m_bPrinting = TRUE;

    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    DOCINFO di;                                 // Initialise print doc details
    memset(&di, 0, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

     CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);                // Initialise printing
    for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                    // Print page
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
    OnEndPrinting(&dc, &Info);                  // Clean up after printing

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC
}

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl printing overridables - for Doc/View print/print preview framework

void CThemeGrid::OnBeginPrinting(CDC *pDC, CPrintInfo *pInfo)
{
    // OnBeginPrinting() is called after the user has committed to
    // printing by OK'ing the Print dialog, and after the framework
    // has created a CDC object for the printer or the preview view.

    // This is the right opportunity to set up the page range.
    // Given the CDC object, we can determine how many rows will
    // fit on a page, so we can in turn determine how many printed
    // pages represent the entire document.

    ASSERT(pDC && pInfo);
    if (!pDC || !pInfo) return;

    int nMaxRowCount = GetRowCount() - GetFixedRowCount();
    if (!nMaxRowCount) return;

    // Get a DC for the current window (will be a screen DC for print previewing)
    CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if (!pCurrentDC) return;

    CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));

    // Create the printer font
    int nFontSize = -9;
    CString strFontName = TEXT("Times New Roman");
    m_PrinterFont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
                             OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, strFontName);
            
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Get the average character width (in GridCtrl units) and hence the margins
    m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
    m_CharSize.cx /= 52;
    int nMargins = (LEFT_MARGIN+RIGHT_MARGIN)*m_CharSize.cx;

    // Get the page sizes (physical and logical)
    m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));
    m_LogicalPageSize.cx = GetVirtualWidth()+nMargins;

#ifdef _WIN32_WCE
    m_LogicalPageSize.cy = (m_LogicalPageSize.cx * m_PaperSize.cy) / m_PaperSize.cx;
#else
    m_LogicalPageSize.cy = MulDiv(m_LogicalPageSize.cx, m_PaperSize.cy, m_PaperSize.cx);
#endif

    m_nPageHeight = m_LogicalPageSize.cy - GetFixedRowHeight()
                       - (HEADER_HEIGHT+FOOTER_HEIGHT + 2*GAP)*m_CharSize.cy;

    // Get the number of pages. Assumes no row is bigger than the page size.
    int nTotalRowHeight = 0;
    int nNumPages = 1;
    for (int row = GetFixedRowCount(); row < GetRowCount(); row++)
    {
        nTotalRowHeight += GetRowHeight(row);
        if (nTotalRowHeight > m_nPageHeight) {
            nNumPages++;
            nTotalRowHeight = GetRowHeight(row);
        }
    }

    // Set up the print info
    pInfo->SetMaxPage(nNumPages);
    pInfo->m_nCurPage = 1;                        // start printing at page# 1

    ReleaseDC(pCurrentDC);
    pDC->SelectObject(pOldFont);
}

void CThemeGrid::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
    if (!pDC || !pInfo) return;

    //CRect rcPage(pInfo->m_rectDraw);
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Set the page map mode to use GridCtrl units, and setup margin
    pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowExt(m_LogicalPageSize);
    pDC->SetViewportExt(m_PaperSize);
    pDC->SetWindowOrg(-LEFT_MARGIN*m_CharSize.cx, 0);

    // Header
    pInfo->m_rectDraw.top    = 0;
    pInfo->m_rectDraw.left   = 0;
    pInfo->m_rectDraw.right  = m_LogicalPageSize.cx - (LEFT_MARGIN+RIGHT_MARGIN)*m_CharSize.cx;
    pInfo->m_rectDraw.bottom = HEADER_HEIGHT*m_CharSize.cy;
    PrintHeader(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -HEADER_HEIGHT*m_CharSize.cy);

    // Gap between header and column headings
    pDC->OffsetWindowOrg(0, -GAP*m_CharSize.cy);

    // Print the column headings
    pInfo->m_rectDraw.bottom = GetFixedRowHeight(); 
    PrintColHeadings(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -GetFixedRowHeight()); 

    // We need to find out which row to start printing for this page.
    int nTotalRowHeight = 0;
    UINT nNumPages = 1;
    int nCurrPrintRow = GetFixedRowCount();

    while (nCurrPrintRow < GetRowCount() && nNumPages < pInfo->m_nCurPage)
    {
        nTotalRowHeight += GetRowHeight(nCurrPrintRow);
        if (nTotalRowHeight > m_nPageHeight) {
            nNumPages++;
            if (nNumPages == pInfo->m_nCurPage) break;
            nTotalRowHeight = GetRowHeight(nCurrPrintRow);
        }
        nCurrPrintRow++;
    }
    if (nCurrPrintRow >= GetRowCount()) return;

    // Draw as many rows as will fit on the printed page.
    // Clip the printed page so that there is no partially shown
    // row at the bottom of the page (the same row which will be fully
    // shown at the top of the next page).

    BOOL bFirstPrintedRow = TRUE;
    CRect rect;
    rect.bottom = -1;
    while (nCurrPrintRow < GetRowCount())
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(nCurrPrintRow) - 1;

        if (rect.bottom > m_nPageHeight) break;            // Gone past end of page

        rect.right = -1;
        for (int col = 0; col < GetColCount(); col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColWidth(col) - 1;

            DrawCell(pDC, nCurrPrintRow, col, rect);

            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
            {
                int Overlap = (col == 0)? 0:1;
                pDC->MoveTo(rect.left-Overlap, rect.bottom);
                pDC->LineTo(rect.right, rect.bottom);
                if (nCurrPrintRow == 0) {
                    pDC->MoveTo(rect.left-Overlap, rect.top);
                    pDC->LineTo(rect.right, rect.top);
                }
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
            {
                int Overlap = (bFirstPrintedRow)? 0:1;
                pDC->MoveTo(rect.right, rect.top-Overlap);
                pDC->LineTo(rect.right, rect.bottom);    
                if (col == 0) {
                    pDC->MoveTo(rect.left, rect.top-Overlap);
                    pDC->LineTo(rect.left, rect.bottom);    
                }
            }

        }
        nCurrPrintRow++;
        bFirstPrintedRow = FALSE;
    }

    // Footer
    pInfo->m_rectDraw.bottom = FOOTER_HEIGHT*m_CharSize.cy;
    pDC->SetWindowOrg(-LEFT_MARGIN*m_CharSize.cx, -m_LogicalPageSize.cy + FOOTER_HEIGHT*m_CharSize.cy);
    PrintFooter(pDC, pInfo);

    // SetWindowOrg back for next page
    pDC->SetWindowOrg(0,0);

    pDC->SelectObject(pOldFont);
}

void CThemeGrid::PrintColHeadings(CDC *pDC, CPrintInfo* /*pInfo*/)
{
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    CRect rect;
    rect.bottom = -1;
    for (int row = 0; row < GetFixedRowCount(); row++)
    {
        rect.top = rect.bottom+1;
        rect.bottom = rect.top + GetRowHeight(row) - 1;

        rect.right = -1;
        for (int col = 0; col < GetColCount(); col++)
        {
            rect.left = rect.right+1;
            rect.right = rect.left + GetColWidth(col) - 1;

            DrawFixedCell(pDC, row, col, rect);

            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_HORZ) 
            {
                int Overlap = (col == 0)? 0:1;
                pDC->MoveTo(rect.left-Overlap, rect.bottom);
                pDC->LineTo(rect.right, rect.bottom);
                if (row == 0) {
                    pDC->MoveTo(rect.left-Overlap, rect.top);
                    pDC->LineTo(rect.right, rect.top);
                }
            }
            if (m_nGridLines == GVL_BOTH || m_nGridLines == GVL_VERT) 
            {
                int Overlap = (row == 0)? 0:1;
                pDC->MoveTo(rect.right, rect.top-Overlap);
                pDC->LineTo(rect.right, rect.bottom);    
                if (col == 0) {
                    pDC->MoveTo(rect.left, rect.top-Overlap);
                    pDC->LineTo(rect.left, rect.bottom);    
                }
            }

        }
    }

    pDC->SelectObject(pOldFont);
}

void CThemeGrid::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
    CRect   rc(pInfo->m_rectDraw);
    CString strHeaderString;
    CFont   BoldFont;
    LOGFONT lf;

    //create bold font for header and footer
    VERIFY(m_PrinterFont.GetLogFont(&lf));
    lf.lfWeight = FW_BOLD;
    VERIFY(BoldFont.CreateFontIndirect(&lf));
 
    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // print App title on top right margin
    strHeaderString.LoadString(AFX_IDS_APP_TITLE);
    pDC->DrawText(strHeaderString, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    // print parent window title in the centre (Gert Rijs)
    CWnd *pParentWnd = GetParent();
    while (pParentWnd)
    {
        pParentWnd->GetWindowText(strHeaderString);
        if (strHeaderString.GetLength())  // can happen if it is a CView, CChildFrm has the title
            break;
        pParentWnd = pParentWnd->GetParent();
    }
    pDC->DrawText(strHeaderString, &rc, DT_CENTER | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);


    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();

    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rc.left, rc.bottom);
    pDC->LineTo(rc.right, rc.bottom);
}

//print footer with a line and date, and page number
void CThemeGrid::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
    CRect rc(pInfo->m_rectDraw);
    CFont BoldFont;
    LOGFONT lf;

    //draw line
    pDC->MoveTo(rc.left, rc.top);
    pDC->LineTo(rc.right, rc.top);

    //create bold font for header and footer
    m_PrinterFont.GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;
    BoldFont.CreateFontIndirect(&lf);

    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // draw page number
    CString   sTemp ;
    rc.OffsetRect(0, m_CharSize.cy/2);
    sTemp.Format(_T("Page %d of %d"), pInfo->m_nCurPage, pInfo->GetMaxPage());
    pDC->DrawText(sTemp,-1,rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    CTime t = CTime::GetCurrentTime();
    sTemp = t.Format(_T("%c"));
    pDC->DrawText(sTemp,-1,rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_NOCLIP | DT_VCENTER);

    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();
}

void CThemeGrid::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
    m_PrinterFont.DeleteObject();
}

#endif  // !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)


#ifndef _WIN32_WCE
/////////////////////////////////////////////////////////////////////////////
// CGridCtrl persistance

BOOL CThemeGrid::Save(LPCTSTR filename)
{
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeWrite | CFile::modeCreate| CFile::typeText, &ex)) {
        ex.ReportError();
        return FALSE;
    }

    TRY {

        int nNumColumns = GetColCount();
		int i=0, j=0;
        for (i = 0; i < nNumColumns; i++) {
			File.WriteString(GetItemText(0,i));
            File.WriteString((i==(nNumColumns-1))? _T("\n"):_T(","));
        }

        for (i = 0; i < GetRowCount(); i++) {
            for (j = 0; j < nNumColumns; j++) {
                File.WriteString(GetItemText(i,j));
                File.WriteString((j==(nNumColumns-1))? _T("\n"):_T(","));
            }
        }

        File.Close();
    }

    CATCH (CFileException, e) {
        AfxMessageBox(_T("Unable to save grid list"));
        e->Delete();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}

BOOL CThemeGrid::Load(LPCTSTR filename)
{
    TCHAR *token, *end;
    TCHAR buffer[1024];
    CStdioFile File;
    CFileException ex;

    if (!File.Open(filename, CFile::modeRead | CFile::typeText)) {
        ex.ReportError();
        return FALSE;
    }

    DeleteAllItems();

    TRY {

        // Read Header off file
        File.ReadString(buffer, 1024);

        // Get first token
        for (token=buffer, end=buffer; 
             *end && (*end != _T(',')) && (*end != _T('\n')); end++);
        if ((*end == _T('\0')) && (token == end)) token = NULL;
        *end = _T('\0');

        while (token) 
        {
            InsertCol(token);
                
            // Get next token
            for (token=++end; *end && (*end != _T(',')) && (*end != _T('\n'));
                 end++);
            if ((*end == _T('\0')) && (token == end)) token = NULL;
            *end = _T('\0');
        }

        // Read in rest of data
        int nItem = 0;
        while (File.ReadString(buffer, 1024)) {

            // Get first token
            for (token=buffer, end=buffer; 
                 *end && (*end != _T(',')) && (*end != _T('\n')); end++);
            if ((*end == _T('\0')) && (token == end)) token = NULL;
            *end = _T('\0');

            int nSubItem = 0;
            while (token) {

                if (!nSubItem)
                    InsertRow(token);
                else
                    SetItemText(nItem, nSubItem, token);

                // Get next token
                for (token=++end; *end && (*end != _T(',')) && (*end != _T('\n'));
                     end++);
                if ((*end == _T('\0')) && (token == end)) token = NULL;
                *end = _T('\0');

                nSubItem++;
            }
            nItem++;
        }

        AutoSizeCols();
        File.Close();
    }

    CATCH (CFileException, e) {
        AfxMessageBox(_T("Unable to load grid data"));
        e->Delete();
        return FALSE;
    }
    END_CATCH

    return TRUE;
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridCtrl overrideables

#ifndef GRIDCONTROL_NO_DRAGDROP
// This is no longer needed since I've changed to OLE drag and drop - but it's
// still cool code. :)
CImageList* CThemeGrid::CreateDragImage(CPoint *pHotSpot)
{
    CDC* pDC = GetDC();
    if (!pDC) return NULL;

    CRect rect;
    CCellID cell = GetFocusCell();
    if (!GetCellRect(cell.row, cell.col, rect)) return NULL;

    // Translate coordinate system
    rect.BottomRight() = CPoint(rect.Width(), rect.Height());
    rect.TopLeft()     = CPoint(0,0);
    *pHotSpot = rect.BottomRight(); 

    // Create a new imagelist (the caller of this function has responsibility
    // for deleting this list)
    CImageList* pList = new CImageList;
    if (!pList || !pList->Create(rect.Width(), rect.Height(), ILC_MASK, 1,1))
    {    
        if (pList) delete pList;
        return NULL;
    }

    // Create mem DC and bitmap
    CDC MemDC;
    CBitmap bm;
    MemDC.CreateCompatibleDC(pDC);
    bm.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOldBitmap = MemDC.SelectObject(&bm);
    MemDC.SetWindowOrg(0,0);

    // Draw cell onto bitmap in memDC
    DrawCell(&MemDC, cell.row, cell.col, rect, TRUE);

    // Clean up
    MemDC.SelectObject(pOldBitmap);
    ReleaseDC(pDC);

    // Add the bitmap we just drew to the image list.
    pList->Add(&bm, GetNormalBackColor());
	//pList->Add(&bm, GetNormalItemBackColor());
    bm.DeleteObject();

    return pList;
}
#endif

void CThemeGrid::OnFixedRowClick(CCellID& cell)
{
    if (!IsValid(cell)) 
        return;

    //if (m_bListMode)
    {
        if (!m_bSortOnClick) 
            return;

        CWaitCursor waiter;
        if (cell.col == m_SortColumn)
            m_bAscending = !m_bAscending;
        else 
        {
            m_bAscending = TRUE;
            m_SortColumn = cell.col;
        }
        SortTextItems(m_SortColumn, m_bAscending);
        Invalidate();
    } 
    //else if (cell.col < GetFixedColCount()) 
	if (cell.col < GetFixedColCount()) 
    {
        m_MouseMode = MOUSE_SELECT_ALL;
        OnSelecting(cell);
    } 
    else 
    {
        m_MouseMode = MOUSE_SELECT_COL;
        OnSelecting(cell);
    }
}

void CThemeGrid::OnFixedColClick(CCellID& cell)
{
    if (!IsValid(cell))
        return;

//    if (m_bListMode && (GetItemState(cell.row, m_nFixedCols) & GVNI_SELECTED))
//    {
//        OnEditCell(cell.row, cell.col, VK_LBUTTON);
//        return;
//    }

    if (cell.row < GetFixedRowCount()) 
    {
        m_MouseMode = MOUSE_SELECT_ALL;
        OnSelecting(cell);
    }
    else 
    {
        m_MouseMode = MOUSE_SELECT_ROW;
        OnSelecting(cell);
    }
}

// Gets the extent of the text pointed to by str (no CDC needed)
// By default this uses the selected font (which is a bigger font)
CSize CThemeGrid::GetTextExtent(LPCTSTR str, BOOL bUseSelectedFont /* = TRUE */)
{
    CDC* pDC = GetDC();
    if (!pDC) return CSize(0,0);

    CFont *pOldFont, font;

    if (bUseSelectedFont)
    {
        LOGFONT lf;
        memcpy(&lf, &m_Logfont, sizeof(LOGFONT));
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
            
        font.CreateFontIndirect(&lf);
    
        pOldFont = pDC->SelectObject(&font);
    }
    else
        pOldFont = pDC->SelectObject(&m_Font);

    CSize size = pDC->GetTextExtent(str);
    pDC->SelectObject(pOldFont);
    ReleaseDC(pDC);

    return size + CSize(2*m_nMargin, 2*m_nMargin);
}

CSize CThemeGrid::GetCellExtent(int nRow, int nCol, CDC* pDC)
{
    LOGFONT *pLF = GetItemFont(nRow, nCol);
    
    // use selected font since it's thicker   
    LOGFONT lf;
    memcpy(&lf, pLF, sizeof(LOGFONT));
    
    if (nRow < m_nFixedRows || nCol < m_nFixedCols)
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
            
    CFont font;
    font.CreateFontIndirect(&lf);
    
    CFont* pOldFont = pDC->SelectObject(&font);
	CSize size = pDC->GetTextExtent(GetItemText(nRow, nCol));

	// add SKC ------------------------------------------
	CStringArray Items;
	if (IsColComboBox(nCol, Items))
	{
		CSize sz(0,0);
		int maxn = Items.GetSize();
		for (int i = 0; i < maxn; i++) {
			sz = pDC->GetTextExtent(Items.GetAt(i));
			if (sz.cx > size.cx) size.cx = sz.cx;
		}
		Items.RemoveAll();
	}
	////////////////////////

    pDC->SelectObject(pOldFont);

    size += CSize(4*m_nMargin, 2*m_nMargin);

    CSize ImageSize(0,0);
	// 체크박스 이미지가 존재한다면
    if (m_pImageList) 
	{
        int nImage = GetItemImage(nRow, nCol);
        if (nImage >= 0) {
            IMAGEINFO Info;
            if (m_pImageList->GetImageInfo(nImage, &Info))
                ImageSize = CSize(Info.rcImage.right-Info.rcImage.left+1, 
                                  Info.rcImage.bottom-Info.rcImage.top+1);
        }
    }
	// 체크박스 이미지가 존재하지 않는다면
	else
	{
		int nImage = GetItemImage(nRow, nCol);
        if (nImage >= 0) {
			ImageSize = CSize(12, 12);
        }
	}
    
    return CSize(size.cx + ImageSize.cx, max(size.cy, ImageSize.cy));
}

BOOL CThemeGrid::DrawFixedCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
    if (!m_bAllowDraw) return FALSE;

    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT | GVIF_IMAGE | GVIF_BKCLR | GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return FALSE;

	// add SKC ---------------------------------------
	int half;
	int nGroupIndex;
	int FirstId, LastId;
	CString csGroupText;
	BOOL bColGroup = IsColGroup(nCol, nGroupIndex, FirstId, LastId, csGroupText);
	if (bColGroup) {
		half = (rect.bottom - rect.top) / 2;
		rect.top += half+1;
	}
	// end SKC ----------------------------------------

    // Force redraw of background if custom colour
    //if (Item.crBkClr != CLR_DEFAULT)
    bEraseBk = TRUE;

    if (bEraseBk
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
        && !pDC->IsPrinting()
#endif
        )
    {
        CBrush brush((Item.crBkClr == CLR_DEFAULT)? GetFixedBackColor() : Item.crBkClr);
        pDC->FillRect(rect, &brush);
    }
    pDC->SetTextColor((Item.crFgClr == CLR_DEFAULT)? GetFixedFontColor() : Item.crFgClr);
    
    int nSavedDC = pDC->SaveDC();
    
    // Create the appropriate font and select into DC
    LOGFONT *pLF = GetItemFont(nRow, nCol);

    CCellID FocusCell = GetFocusCell();

    if (FocusCell.row == nRow || FocusCell.col == nCol)
    {
        static LOGFONT lf;
        memcpy(&lf, pLF, sizeof(LOGFONT));
        lf.lfWeight = SELECTED_CELL_FONT_WEIGHT;
        pLF = &lf;
    }
   
    CFont Font;
    Font.CreateFontIndirect(pLF);
    CFont *pOldFont = pDC->SelectObject(&Font);

    if (IsValid(FocusCell) &&  (FocusCell.row == nRow || FocusCell.col == nCol))
    {
        rect.right++; rect.bottom++;
        pDC->DrawEdge(rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT);
        rect.DeflateRect(1,1);
    }
    else
    {
        //CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
		//		darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
		CPen lightpen(PS_SOLID, 1,  m_crFixedFrameLT),
				darkpen(PS_SOLID,  1, m_crFixedFrameRB),
             *pOldPen = pDC->GetCurrentPen();
    
        pDC->SelectObject(&lightpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.left, rect.top);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(&darkpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(pOldPen);
        rect.DeflateRect(1,1);
    }

    pDC->SetBkMode(TRANSPARENT);
    rect.DeflateRect(m_nMargin, 0);

	// 체크박스를 그려야 하고, 체크박스 이미지가 존재한다면
    if (m_pImageList && Item.iImage >= 0)
    {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            m_pImageList->Draw(pDC, Item.iImage, rect.TopLeft(), ILD_NORMAL);
            rect.left += (nImageWidth + m_nMargin);
        }
    }
	// 체크박스를 그려야하고, 체크박스 이미지가 존재하지 않는다면 직접 그린다
	else if (Item.iImage >= 0)
	{
		// 체크박스를 컬러로 그린다
		DrawCheckBoxByColor(pDC, rect, Item.iImage);
		// 셀 영역에서 체크박스 영역을 제외
		rect.left += (11 + m_nMargin);
	}

    DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);
	pDC->SelectObject(pOldFont);
	Font.DeleteObject();

	// add SKC ---------------------------------------------------------
	if (bColGroup)
	{
		CCellID idTopLeft = GetTopleftNonFixedCell();
		if (FirstId < idTopLeft.col) FirstId = idTopLeft.col;

		CRect FirstRect, LastRect, grect;
		if (GetCellRect(0, FirstId, FirstRect))
		{

			GetCellRect(0, LastId, LastRect);

			grect.CopyRect(FirstRect);
			grect.right = LastRect.right;
			grect.bottom -= half+1;

			CPen lightpen(PS_SOLID, 1, ::GetSysColor(COLOR_3DHIGHLIGHT)),
				  darkpen(PS_SOLID, 1, ::GetSysColor(COLOR_3DDKSHADOW)),
				 *pOldPen = pDC->GetCurrentPen();
    
			pDC->SelectObject(&lightpen);
			pDC->MoveTo(grect.right, grect.top);
			pDC->LineTo(grect.left, grect.top);
			pDC->LineTo(grect.left, grect.bottom);

			pDC->SelectObject(&darkpen);
			pDC->MoveTo(grect.right, grect.top);
			pDC->LineTo(grect.right, grect.bottom);
			pDC->LineTo(grect.left, grect.bottom);

			pDC->SelectObject(pOldPen);

			LOGFONT tlf;
			memcpy(&tlf, &m_Logfont, sizeof(LOGFONT));
			CFont Font;
			Font.CreateFontIndirect(&tlf);
			CFont *pOldFont = pDC->SelectObject(&Font);

			DrawText(pDC->m_hDC, csGroupText, -1, grect, DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

			pDC->SelectObject(pOldFont);
			Font.DeleteObject();

		}
	}
	// end SKC ------------------------------------------------

    pDC->RestoreDC(nSavedDC);
    
    return TRUE;
}

void CThemeGrid::CreateInPlaceEditControl(CRect& rect, DWORD dwStyle, UINT nID,
                                         int nRow, int nCol,
                                         LPCTSTR szText, int nChar)
{
    // InPlaceEdit auto-deletes itself
    //new CInPlaceEdit(this, rect, dwStyle, nID, nRow, nCol, szText, nChar);
	new CBaseEdit(this, rect, dwStyle, nID, nRow, nCol, szText, nChar);
}

void CThemeGrid::OnEndEditCell(int nRow, int nCol, CString str)
{
	CString strCurrent = GetItemText(nRow,nCol);
	if (strCurrent != str)
    {
		SetModified(TRUE, nRow, nCol);
        SetItemText(nRow, nCol, str);
	}

	if( m_bBaseCtrlList )
		SendMessageToParent(nRow, nCol, LVN_ITEMCHANGED);
}

CString CThemeGrid::GetItemText(int nRow, int nCol)
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return TEXT("");

    CGridCell* pCell = GetCell(nRow, nCol);
    ASSERT(pCell);
    if (!pCell) return TEXT("");

    return pCell->szText;
}

////////////////////////////////////////////////////////////////////////////
//
// added by SKC 1999.8.7
//
////////////////////////////////////////////////////////////////////////////

void CThemeGrid::CreateInPlaceComboControl(CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, 
						   CStringArray& Items, CString sInitText, 
						   UINT nFirstChar)
{
	// GridInPlaceList auto-deletes itself
	new CBaseCombo(this, rect, dwStyle, nID, nRow, nColumn, Items, sInitText, nFirstChar);
	//new CInPlaceList(this, rect, dwStyle, nID, nRow, nColumn, Items, sInitText, nFirstChar);
}

void CThemeGrid::SetColGroup(int *pGroupArray, int NumArray, LPCTSTR szGroupName)
{
	CColGroupObject *pGroup = new CColGroupObject;
	pGroup->nArray = NumArray;
	pGroup->csText = szGroupName;
	for (int i = 0; i < NumArray; i++) {
		int nCol = pGroupArray[i];
		pGroup->iArray.Add(nCol);
	}
	m_ColGroupList.AddTail(pGroup);
}

BOOL CThemeGrid::IsColGroup(int nCol, int &Id, int &FirstCol, int &LastCol, CString &csText)
{
	POSITION pos = m_ColGroupList.GetHeadPosition();
	while (pos)
	{
		CColGroupObject *pGroup = (CColGroupObject *)m_ColGroupList.GetNext(pos);
		for (int i = 0; i < pGroup->nArray; i++)
		{
			UINT col = pGroup->iArray.GetAt(i);
			if (nCol == (int)col)
			{
				if (i == (pGroup->nArray-1)) Id = -1;
				else Id = i;
				csText = pGroup->csText;
				FirstCol = pGroup->iArray.GetAt(0);
				LastCol = pGroup->iArray.GetAt(pGroup->nArray-1);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CThemeGrid::ExpandColsToFit(int nColumn)
{
    if (GetColCount() <= 0) return;

    CRect rect;
    GetClientRect(rect);

    long virtualWidth = GetVirtualWidth();
    int nDifference = rect.Width() - (int) virtualWidth;

    m_arColWidths[nColumn] += nDifference;

    if (m_bAllowDraw) Invalidate();
}

void CThemeGrid::ClearSelectedRange()
{
    SetSelectedRange(-1,-1,-1,-1);
    SetFocusCell(-1,-1);
}

void CThemeGrid::SetExInfoCallback(BOOL (CWnd::*fpExInfoFun)(int nRow, int nCol, COLORREF &BkColor)) 
{ 
	m_fpExInfoFun = fpExInfoFun;
}

/////////////////////////////////////////////////////////

BOOL CThemeGrid::IsColComboBox(int nCol, CStringArray &sArray)
{
	POSITION pos = m_ColInfoList.FindIndex(nCol);
	if (pos) {
		CColInfoObject *pCombo = (CColInfoObject *)m_ColInfoList.GetAt(pos);
		if (pCombo == NULL) return FALSE;
		if (pCombo->nArray > 0)
		{
			sArray.Copy(pCombo->sArray);
			return TRUE;
		}
	}
	return FALSE;
}

// 컬럼에 체크박스 설정
void CThemeGrid::SetColCheckBox(UINT nCol)
{
	int i=0, nArrSize=0;

	// 체크박스 컬럼 
	nArrSize = m_arColumnCheckbox.GetSize();

	// 이미 체크박스로 설정된 컬럼이 존재한다면 함수 탈출
	for(i=0 ; i < nArrSize ; i++)
		if( m_arColumnCheckbox[i] == nCol )
			return;

	// 체크박스 정보를 전역변수에 저장
	m_arColumnCheckbox.Add(nCol);

	// 해당 셀에 체크박스 이미지 표시
	for (int nRow = GetFixedRowCount() ; nRow < GetRowCount(); nRow++)
	{
		SetItemImage(nRow, nCol, 0);
		RedrawCell(nRow, nCol);
	}
}

// 컬럼이 체크박스인지 여부를 반환
BOOL CThemeGrid::IsColCheckBox(UINT nCol)
{
	int i=0, nArrSize=0;

	// 체크박스 컬럼 
	nArrSize = m_arColumnCheckbox.GetSize();

	// 이미 체크박스로 설정된 컬럼이 존재한다면 TRUE를 반환
	for(i=0 ; i < nArrSize ; i++)
		if( m_arColumnCheckbox[i] == nCol )
			return TRUE;

	// 해당 컬럼에 체크박스가 아니므로 FALSE를 반환
	return FALSE;
}

// 셀이 에디트 될때 실행되는 함수
void CThemeGrid::OnEditCell(int nRow, int nCol, UINT nChar)
{
    EnsureVisible(nRow, nCol);

    CCellID cell(nRow, nCol);
    if (!IsValid(cell) || !IsCellEditable(nRow, nCol) || !IsCellVisible(nRow, nCol)) 
        return;

    CRect rect;
    if (!GetCellRect(cell, rect)) return;

	// 셀 에디트 메시지를 부모 윈도우로 전달
	if( m_bBaseCtrlList )
		SendMessageToParent(nRow, nCol, LVN_BEGINLABELEDIT);
	else
		SendMessageToParent(nRow, nCol, GVN_BEGINLABELEDIT);

	// 해당 셀이 체크박스 일때
	if( IsColCheckBox(nCol) )
	{
		int id = GetItemImage(nRow, nCol);
		// id가 이미지 번호이니 이것을 토글하면 되겠지요..

		int rid = (id == 0) ? 1: 0; 
		SetItemImage(nRow, nCol, rid);
		RedrawCell(nRow, nCol);
		return;
	}

    GV_ITEM Item;
    Item.mask = GVIF_TEXT | GVIF_FORMAT;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item)) return;

    DWORD dwStyle = ES_LEFT;
    if (Item.nFormat & DT_RIGHT) dwStyle = ES_RIGHT;
    else if (Item.nFormat & DT_CENTER) dwStyle = ES_CENTER;

	// add SKC ------------------------------------------
	CStringArray Items;
	if (IsColComboBox(nCol, Items))
	{
		dwStyle = CBS_DROPDOWN; //CBS_DROPDOWNLIST;
		CString cs = GetItemText(nRow, nCol);
		int id = -1;
		if (!cs.IsEmpty()) {
			int maxn = Items.GetSize();
			for (int i = 0; i < maxn; i++) {
				if (cs.CompareNoCase(Items.GetAt(i)) == 0) {
					id = i;
					break;
				}
			}
		}
		if (id == -1) {
			Items.InsertAt(0, cs);
			id = 0;
		}
		CString sInitText = Items.GetAt(id);
		CreateInPlaceComboControl(rect, dwStyle, IDC_INPLACE_CONTROL, nRow, nCol, Items, sInitText, nChar);
		Items.RemoveAll();
	}
	else
	// end SKC -------------------------------
		CreateInPlaceEditControl(rect, dwStyle, IDC_INPLACE_CONTROL, nRow, nCol, Item.szText, nChar);

}

// 셀 높이를 설정하는 함수
void CThemeGrid::SetCellHeight(int nHeight)
{
	m_nDefCellHeight = nHeight;

	for (int i = 0; i < m_nRows; i++) m_arRowHeights[i] = m_nDefCellHeight;

	SetRowCount(m_nRows);	
}

// 셀 폭을 설정하는 함수
void CThemeGrid::SetCellWidth(int nWidth)
{
	m_nDefCellWidth = nWidth;

	for (int i = 0; i < m_nCols; i++) m_arColWidths[i] = m_nDefCellWidth;

	SetColCount(m_nCols);	

}

// 텍스트 폰트를 생성하는 함수
void CThemeGrid::CreateTextFont(int nFontSize)
{
	// 생성하기 전에 먼저 삭제
	m_ftText.DeleteObject();
	
	// 폰트 생성
	m_ftText.CreateFont(nFontSize, 0,0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));

	// 폰트 등록
	this->SetFont(&m_ftText);
}

// 폰트 크기를 설정하는 함수
void CThemeGrid::SetFontSize(int nFontSize)
{
	m_nFontSize = nFontSize;

	// 텍스트 폰트를 생성하는 함수
	CreateTextFont(m_nFontSize);
}

LRESULT CThemeGrid::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		case WM_MOUSEMOVE:
			{
				if( m_ToolTip.GetSafeHwnd() )
				{
					MSG msg;
					msg.hwnd = m_hWnd;
					msg.message = message;
					msg.wParam = wParam;
					msg.lParam = lParam;

					m_ToolTip.RelayEvent(&msg);
				}
			}
			break;
		default:
			break;
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}

// 클래스 종료시 실행되는 함수
void CThemeGrid::OnDestroy()
{
	// 툴팁 삭제 함수
	DeleteToolTip();
}

// 컬럼에 콤보박스를 설정
void CThemeGrid::SetColComboBox(int nCol, wchar_t *pArray[], int NumArray)
{
	POSITION pos = m_ColInfoList.FindIndex(nCol);
	if (pos) {
		CColInfoObject *pCombo = (CColInfoObject *)m_ColInfoList.GetAt(pos);
		pCombo->nArray = NumArray;
		for (int i = 0; i < NumArray; i++) {
			CString cs = pArray[i];
			pCombo->sArray.Add(cs);
		}
	}
}

// 컬럼에 콤보박스를 설정
void CThemeGrid::SetColComboBox(int nCol, CString strArray)
{
	POSITION pos = m_ColInfoList.FindIndex(nCol);
	if (pos) 
	{
		CColInfoObject *pCombo = (CColInfoObject *)m_ColInfoList.GetAt(pos);

		CString strNextLine = TEXT("\n"), strItem = TEXT("");
		int nPos=0, nItemCount=0;

		// 문자열 리스트에서 한 라인씩 분리
		while( strArray.GetLength() > 0 )
		{
			nPos = strArray.Find(strNextLine);
			if( nPos >= 0 )
			{
				strItem = strArray.Left(nPos);
				strArray = strArray.Mid(nPos+1);
			}
			else
			{
				strItem = strArray;
				strArray = "";
			}

			// 콤보박스에 항목 문자열을 추가
			pCombo->sArray.Add(strItem);
			
			// 항목 번호 증가
			nItemCount ++;
		}

		// 콤보박스 항목 개수 설정
		pCombo->nArray = nItemCount;
	}

}

// 현재 포커스가 위치한 셀 번호를 반환
BOOL CThemeGrid::GetFocusCell(int &nRow, int &nCol)
{
	if( m_idCurrentCell.row < 0 || m_idCurrentCell.col < 0 )
		return FALSE;

	nRow = m_idCurrentCell.row;
	nCol = m_idCurrentCell.col;

	return TRUE;
}

// 텍스트 정렬 방식 설정
void CThemeGrid::SetAlignFormat(UINT nAlignFormat)
{
	m_nAlignFormat = nAlignFormat;
}

// 체크박스 이미지를 설정(Resource ID)
BOOL CThemeGrid::SetCheckBoxImage(UINT nImageID)
{
	// 이미지 넓이를 구한다
	CBitmap bmpCheckBox;
	BITMAP bm;
	int cx=0;

	// 이미지를 로딩
	bmpCheckBox.LoadBitmap(nImageID);
	bmpCheckBox.GetBitmap(&bm);

	// 단위 이미지 넓이
	cx = bm.bmWidth / 2;
	if( cx < 0 )
		return FALSE;

	// 체크박스용 이미지 리스트를 설정
	return SetImageList(nImageID, cx, 2, RGB(255,0,255));
}

// 체크박스용 이미지 리스트를 설정(Resource ID)
BOOL CThemeGrid::SetImageList(UINT nBitmapID, int cx, int nGrow, COLORREF crMask)
{
	// 기존 이미지가 존재한다면 기존 이미지를 삭제
	if( m_pImageList )
		m_pImageList->DeleteImageList();

	m_pImageList = new CImageList;
	
	return m_pImageList->Create(nBitmapID, cx, nGrow, crMask);
}

// 체크박스 이미지를 설정(파일)
BOOL CThemeGrid::SetCheckBoxImage(CString strFileName)
{
	// 이미지 넓이를 구한다
	CBitmap bmpCheckBox;
	BITMAP bm;
	int cx=0;
	
	// 이미지 파일을 로딩
	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strFileName,
        IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    bmpCheckBox.Attach(hBitmap);

	// 이미지를 로딩
	bmpCheckBox.GetBitmap(&bm);

	// 단위 이미지 넓이
	cx = bm.bmWidth / 2;
	if( cx < 1 )
		return FALSE;

	// 체크박스용 이미지 리스트를 설정
	return SetImageList(strFileName, cx, 2, RGB(255,0,255));
}

// 체크박스용 이미지 리스트를 설정(파일)
BOOL CThemeGrid::SetImageList(CString strFileName, int cx, int nGrow, COLORREF crMask)
{
	// 기존 이미지가 존재한다면 기존 이미지를 삭제
	if( m_pImageList )
		m_pImageList->DeleteImageList();

	CBitmap bmpCheckBox;
	BITMAP bm;
	
	// 이미지 파일을 읽어서 CBitmap으로 반환
	if( !gf_LoadPictureFile(strFileName, &bmpCheckBox) )
		return FALSE;

	// 이미지를 로딩
	bmpCheckBox.GetBitmap(&bm);

	// 이미지 리스트를 생성
	m_pImageList = new CImageList;
	m_pImageList->Create(cx, bm.bmHeight, ILC_COLOR8 |ILC_MASK , 2, 2);

	// 이미지 리스트에 비트맵 이미지 설정
	return m_pImageList->Add(&bmpCheckBox, RGB(255,0,255));
}

BOOL CThemeGrid::DrawCell(CDC* pDC, int nRow, int nCol, CRect rect, BOOL bEraseBk)
{
    if (!m_bAllowDraw) return FALSE;

    GV_ITEM Item;
    Item.mask = GVIF_TEXT|GVIF_FORMAT|GVIF_STATE|GVIF_IMAGE|GVIF_BKCLR|GVIF_FGCLR;
    Item.row = nRow;
    Item.col = nCol;
    if (!GetItem(&Item))
        return FALSE;

	COLORREF TextBkClr = m_crTextBkColour;
    COLORREF TextClr   = (Item.crFgClr == CLR_DEFAULT)? GetNormalFontColor()   : Item.crFgClr;

    // Force redraw of background if custom colour
    bEraseBk = TRUE;

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (Item.state & GVIS_FOCUSED 
	#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
        && !pDC->IsPrinting()
	#endif
        ) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;    
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
        pDC->SetTextColor(TextClr);

        rect.DeflateRect(1,1);

    }
    else if (Item.state & GVIS_SELECTED
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
        && !pDC->IsPrinting()
#endif
        ) 
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        pDC->FillSolidRect(rect, ::GetSysColor(COLOR_HIGHLIGHT));
        rect.right--; rect.bottom--;
        pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
	else
	{
		// add SKC -----------------------------------------------------------------
		if (m_fpExInfoFun) {
			if ((GetOwner()->*m_fpExInfoFun)(nRow, nCol, TextBkClr)) bEraseBk = TRUE;
		}
		// end SKC -----------------------------------------------------------------

        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBk) 
        {
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
        }
        rect.right--; rect.bottom--;
        pDC->SetTextColor(TextClr);
    }

    if (Item.state & GVIS_DROPHILITED 
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
        && !pDC->IsPrinting()
#endif
        )
    {
        pDC->SelectStockObject(BLACK_PEN);
        pDC->SelectStockObject(NULL_BRUSH);
        pDC->Rectangle(rect);
    }

    // Create the appropriate font and select into DC
    LOGFONT *pLF = GetItemFont(nRow, nCol);
   
    CFont Font;
    Font.CreateFontIndirect(pLF);
    CFont *pOldFont = pDC->SelectObject(&Font);

    rect.DeflateRect(m_nMargin, 0);

	// 체크박스를 그려야 하고, 체크박스 이미지가 존재한다면
    if (m_pImageList && Item.iImage >= 0)
    {
        IMAGEINFO Info;
        if (m_pImageList->GetImageInfo(Item.iImage, &Info))
        {
            CRect rImage;
			int nImageHeight=0;
			rImage = rect;

			nImageHeight = Info.rcImage.bottom-Info.rcImage.top;
			if( rImage.Height() > nImageHeight )
			{
				rImage.top = rImage.top + (rImage.Height() - nImageHeight) / 2;
				rImage.bottom = rImage.top + nImageHeight;
			}

            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            m_pImageList->Draw(pDC, Item.iImage, rImage.TopLeft(), ILD_NORMAL);
			// 셀 영역에서 체크박스 영역을 제외
            rect.left += (nImageWidth + m_nMargin);
        }
    }
	// 체크박스를 그려야하고, 체크박스 이미지가 존재하지 않는다면 직접 그린다
	else if (Item.iImage >= 0)
	{
		// 체크박스를 컬러로 그린다
		DrawCheckBoxByColor(pDC, rect, Item.iImage);
		// 셀 영역에서 체크박스 영역을 제외
		rect.left += (11 + m_nMargin);
	}

	// 셀 텍스트를 화면에 표시
	DrawText(pDC->m_hDC, Item.szText, -1, rect, Item.nFormat);

	pDC->SelectObject(pOldFont);
	Font.DeleteObject();

    pDC->RestoreDC(nSavedDC);
    

    return TRUE;
}

// 체크박스를 컬러로 그린다
void CThemeGrid::DrawCheckBoxByColor(CDC *pDC, CRect RectCheck, BOOL bCheck)
{
	CRect rMark;
	CPoint po1, po2;
	
	// 마크 영역좌표를 계산
	rMark.left = RectCheck.left + 1;
	rMark.right = rMark.left + 11;
	rMark.top = RectCheck.top + (RectCheck.Height() - 11) / 2;
	rMark.bottom = rMark.top + 11;
	
	// 마크 배경을 그린다
	pDC->FillRect(rMark, &CBrush(RGB(255,255,255)));
	// 테두리 사각형을 그린다
	gf_DrawRectLine(rMark, RGB(0,0,0), RGB(0,0,0), RGB(192,192,192), RGB(192,192,192), (CPaintDC*)pDC);

	// 현재 Check 상태라면
	if( bCheck )
	{
		// 라인 위치좌표를 구한다
		po1.x = rMark.left + 3;
		po2.x = rMark.right - 3;
		po1.y = rMark.top + 3;
		po2.y = rMark.bottom - 3;

		// 라인(선)을 그리는 함수
		gf_DrawLine(po1, po2, RGB(0,0,0), 2, (CPaintDC*)pDC);

		po1.y = rMark.bottom - 3;
		po2.y = rMark.top + 3;

		// 라인(선)을 그리는 함수
		gf_DrawLine(po1, po2, RGB(0,0,0), 2, (CPaintDC*)pDC);
	}
}

// 툴팁을 생성한다
void CThemeGrid::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CThemeGrid::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 툴팁 삭제
void CThemeGrid::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 툴팁 문자열 설정
void CThemeGrid::SetToolTip(CString strText)
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

inline CGridCell* CThemeGrid::GetCell(int nRow, int nCol) const
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return NULL;

    GRID_ROW* pRow = m_RowData[nRow];
    if (!pRow) return NULL;
    return pRow->GetAt(nCol);
}

inline BOOL CThemeGrid::SetCell(int nRow, int nCol, CGridCell* pCell)
{
    if (nRow < 0 || nRow >= m_nRows || nCol < 0 || nCol >= m_nCols) return FALSE;

    GRID_ROW* pRow = m_RowData[nRow];
    if (!pRow) return FALSE;

    pRow->SetAt(nCol, pCell);
    return TRUE;
}

BOOL CThemeGrid::SetItemText(int nRow, int nCol, LPCTSTR str)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell)
        return FALSE;

    pCell->szText = str;
    return TRUE;
}

BOOL CThemeGrid::SetItemData(int nRow, int nCol, LPARAM lParam)
{
    CGridCell* pCell = GetCell(nRow, nCol);
    if (!pCell) 
        return FALSE;

    pCell->lParam = lParam;
    return TRUE;
}

// 행 번호 설정
void CThemeGrid::SetRow(long nNewValue)
{
	m_stThemeInfo.poCell.y = nNewValue;
}

// 열 번호 설정
void CThemeGrid::SetCol(long nNewValue)
{
	m_stThemeInfo.poCell.x = nNewValue;
}

// 셀에 문자열 설정
void CThemeGrid::SetText(CString strText)
{
	// 현재 셀 번호가 범위를 벗어나면 함수 탈출
	if( m_stThemeInfo.poCell.x < 0 || m_stThemeInfo.poCell.y < 0 )
		return;

	SetItemText(m_stThemeInfo.poCell.y, m_stThemeInfo.poCell.x, strText);
	RedrawCell(m_stThemeInfo.poCell.y, m_stThemeInfo.poCell.x);
}

// 셀에 Data 설정
void CThemeGrid::SetData(LPARAM lParam)
{
	// 현재 셀 번호가 범위를 벗어나면 함수 탈출
	if( m_stThemeInfo.poCell.x < 0 || m_stThemeInfo.poCell.y < 0 )
		return;

	SetItemData(m_stThemeInfo.poCell.y, m_stThemeInfo.poCell.x, lParam);
	RedrawCell(m_stThemeInfo.poCell.y, m_stThemeInfo.poCell.x);
}

void CThemeGrid::OnRButtonDown(UINT nFlags, CPoint point) 
{
	HWND hOldFocusWnd = ::GetFocus();

	CCellID RightClickDownCell = GetCellFromPt(point);
	if (!IsValid(RightClickDownCell)) return;
	
    SetFocus();  // Auto-destroy any GridInPlaceEdit's

	ResetSelectedRange();
    SetFocusCell(RightClickDownCell.row, RightClickDownCell.col);

	// 오른쪽 마우스 버튼 클릭 다운 이벤트를 부모 윈도우로 전달
	if( !m_bBaseCtrlList )
		SendMessageToParent(RightClickDownCell.row, RightClickDownCell.col, GVN_RBUTTONDOWN); 

	CWnd::OnRButtonDown(nFlags, point);
}

void CThemeGrid::OnRButtonUp(UINT nFlags, CPoint point) 
{
	HWND hOldFocusWnd = ::GetFocus();

	CCellID RightClickDownCell = GetCellFromPt(point);
	if (!IsValid(RightClickDownCell)) return;
	
    SetFocus();  // Auto-destroy any GridInPlaceEdit's
	ResetSelectedRange();

	// 오른쪽 마우스 버튼 클릭 이벤트를 부모 윈도우로 전달
	if( m_bBaseCtrlList )
		SendMessageToParent(RightClickDownCell.row, RightClickDownCell.col, NM_RCLICK);
}

void CThemeGrid::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	HWND hOldFocusWnd = ::GetFocus();

	CCellID RightClickDownCell = GetCellFromPt(point);
	if (!IsValid(RightClickDownCell)) return;
	
    SetFocus();  // Auto-destroy any GridInPlaceEdit's
	ResetSelectedRange();

	// 오른쪽 마우스 버튼 더블클릭 이벤트를 부모 윈도우로 전달
	if( m_bBaseCtrlList )
		SendMessageToParent(RightClickDownCell.row, RightClickDownCell.col, NM_RDBLCLK);
}

// Set Base Control is ListCtrl
void CThemeGrid::SetBaseCtrlList(BOOL bFlag)
{
	m_bBaseCtrlList = bFlag;
}
