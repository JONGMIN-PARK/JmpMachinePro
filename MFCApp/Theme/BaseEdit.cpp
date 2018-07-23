// BaseEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "BaseEdit.h"
#include "TCHAR.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit

CBaseEdit::CBaseEdit()
{
	// 내부 편집용 아님
	m_bInPlace = FALSE;
}

CBaseEdit::CBaseEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, CString sInitText, 
                           UINT nFirstChar)
{
	// 내부 편집용
	m_bInPlace = TRUE;

    m_sInitText     = sInitText;
    m_nRow          = nRow;
    m_nColumn       = nColumn;
    m_nLastChar     = 0; 
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
                                                     // then no exit on arrows

    m_Rect = rect;  // For bizarre CE bug.
    
    DWORD dwEditStyle = WS_BORDER|WS_CHILD|WS_VISIBLE| ES_AUTOHSCROLL //|ES_MULTILINE
        | dwStyle;
    if (!Create(dwEditStyle, rect, pParent, nID)) return;
    
    SetFont(pParent->GetFont());
    
    SetWindowText(sInitText);
    SetFocus();
    
    switch (nFirstChar){
        case VK_LBUTTON: 
        case VK_RETURN:   SetSel((int)_tcslen(m_sInitText), -1); return;
        case VK_BACK:     SetSel((int)_tcslen(m_sInitText), -1); break;
        case VK_TAB:
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:
        case VK_SPACE:
        case VK_END:      SetSel(0,-1); return;
        default:          SetSel(0,-1);
    }
    
    SendMessage(WM_CHAR, nFirstChar);
}

CBaseEdit::~CBaseEdit()
{
}


BEGIN_MESSAGE_MAP(CBaseEdit, CEdit)
	//{{AFX_MSG_MAP(CBaseEdit)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_GETDLGCODE()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseEdit message handlers

// 툴팁을 생성한다
void CBaseEdit::MakeToolTip(CString strToolTip)
{
	// 현 컨트롤 ID를 구한다
	UINT unWindowID = (UINT)this->GetSafeHwnd();

	// 툴팁 생성
	m_ToolTip.CreateToolTip(this, unWindowID, strToolTip);
}

// 툴팁 텍스트 변경
void CBaseEdit::ChangeToolTip(CString strToolTip)
{
	// 툴팁 텍스트 변경
	m_ToolTip.ChangeToolTip(strToolTip);
}

// 컨트롤 화면 크기 변경시 실행되는 함수
void CBaseEdit::OnSize(UINT nType, int cx, int cy) 
{
	CEdit::OnSize(nType, cx, cy);
	
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

// 툴팁 삭제
void CBaseEdit::DeleteToolTip()
{
	if( !m_ToolTip.GetSafeHwnd() )
		return;

	m_ToolTip.DelTool(this);
}

// 클래스 종료시에 실행되는 함수
void CBaseEdit::OnDestroy() 
{
	CEdit::OnDestroy();
	
	// 툴팁 삭제
	DeleteToolTip();
}

// 툴팁을 위한 마우스 이벤트를 받는 함수
LRESULT CBaseEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
	
	return CEdit::WindowProc(message, wParam, lParam);
}

// 툴팁 문자열 설정
void CBaseEdit::SetToolTip(CString strText)
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

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CBaseEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// 내부 편집용이라면
	if( m_bInPlace )
	{
		if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
			nChar == VK_DOWN  || nChar == VK_UP   ||
			nChar == VK_RIGHT || nChar == VK_LEFT) &&
			(m_bExitOnArrows || GetKeyState(VK_CONTROL) < 0))
		{
			m_nLastChar = nChar;
			GetParent()->SetFocus();
			return;
		}
	}
    
    CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

// As soon as this edit loses focus, kill it.
void CBaseEdit::OnKillFocus(CWnd* pNewWnd)
{
    CEdit::OnKillFocus(pNewWnd);

	// 내부 편집용이라면
	if( m_bInPlace )
	{
		EndEdit();
	}
}

void CBaseEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if (nChar == VK_TAB || nChar == VK_RETURN)
    {
        m_nLastChar = nChar;
        GetParent()->SetFocus();    // This will destroy this window
        return;
    }
    if (nChar == VK_ESCAPE) 
    {
        SetWindowText(m_sInitText);    // restore previous text
        m_nLastChar = nChar;
        GetParent()->SetFocus();
        return;
    }
    
    CEdit::OnChar(nChar, nRepCnt, nFlags);
    
    // Resize edit control if needed
    
	// 내부 편집용이라면
	if( m_bInPlace )
	{
		// Get text extent
		CString str;
		GetWindowText( str );

		// add some extra buffer
		str += _T("  ");
    
		CWindowDC dc(this);
		CFont *pFontDC = dc.SelectObject(GetFont());
		CSize size = dc.GetTextExtent( str );
		dc.SelectObject( pFontDC );
       
		// Get client rect
		CRect ParentRect;
		GetParent()->GetClientRect( &ParentRect );
    
		// Check whether control needs to be resized
		// and whether there is space to grow
		if (size.cx > m_Rect.Width())
		{
			if( size.cx + m_Rect.left < ParentRect.right )
				m_Rect.right = m_Rect.left + size.cx;
			else
				m_Rect.right = ParentRect.right;
			MoveWindow( &m_Rect );
		}
	}
}

UINT CBaseEdit::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CBaseEdit::PreTranslateMessage(MSG* pMsg) 
{
    // Catch the Alt key so we don't choke if focus is going to an owner drawn button
    if (pMsg->message == WM_SYSCHAR)
        return TRUE;
    
    return CWnd::PreTranslateMessage(pMsg);
}

// Auto delete
void CBaseEdit::PostNcDestroy() 
{
    CEdit::PostNcDestroy();
    
	// 내부 편집용이라면
	if( m_bInPlace )
	{
		delete this;
	}
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit implementation

void CBaseEdit::EndEdit()
{
    CString str;
    GetWindowText(str);
    
    // Send Notification to parent
    GV_DISPINFO dispinfo;
    
    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = LVN_ENDLABELEDIT;//GVN_ENDLABELEDIT;
    
    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.szText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar; 
    
    CWnd* pOwner = GetOwner();
    if (pOwner)
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
    
    // Close this window (PostNcDestroy will delete this)
    PostMessage(WM_CLOSE, 0, 0);
}
