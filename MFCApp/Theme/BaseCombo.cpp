// BaseCombo.cpp : implementation file
//

#include "stdafx.h"
//#include "testdlg.h"
#include "BaseCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboEdit

CComboEdit::CComboEdit()
{
}

CComboEdit::~CComboEdit()
{
}

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CComboEdit::PreTranslateMessage(MSG* pMsg) 
{
	// Make sure that the keystrokes continue to the appropriate handlers
	if (pMsg->message == WM_KEYDOWN || pMsg->message == WM_KEYUP)
	{
		::TranslateMessage(pMsg);
		::DispatchMessage(pMsg);
		return TRUE;
	}	

	// Catch the Alt key so we don't choke if focus is going to an owner drawn button
	if (pMsg->message == WM_SYSCHAR)
		return TRUE;

	return CEdit::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CComboEdit, CEdit)
	//{{AFX_MSG_MAP(CComboEdit)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboEdit message handlers

void CComboEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
    CBaseCombo* pOwner = (CBaseCombo*)GetOwner();  // This MUST be a CBaseCombo
    if (pOwner) pOwner->EndEdit();	
}

void CComboEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
		 nChar == VK_DOWN  || nChar == VK_UP   ||
		 nChar == VK_RIGHT || nChar == VK_LEFT) &&
		(GetKeyState(VK_CONTROL) < 0 && GetDlgCtrlID() == IDC_COMBOEDIT))
    {
        CWnd* pOwner = GetOwner();
        if (pOwner)
            pOwner->SendMessage(WM_KEYDOWN, nChar, nRepCnt+ (((DWORD)nFlags)<<16));
        return;
    }

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CComboEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE) 
	{
        CWnd* pOwner = GetOwner();
        if (pOwner)
            pOwner->SendMessage(WM_KEYUP, nChar, nRepCnt + (((DWORD)nFlags)<<16));
        return;
    }

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
    {
        CWnd* pOwner = GetOwner();
        if (pOwner)
            pOwner->SendMessage(WM_KEYUP, nChar, nRepCnt + (((DWORD)nFlags)<<16));
        return;
    }

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CBaseCombo

CBaseCombo::CBaseCombo()
{
}

CBaseCombo::CBaseCombo(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, 
						   CStringArray& Items, CString sInitText, 
						   UINT nFirstChar)
{
	m_pParent = pParent;
	m_bFirstEnter	= TRUE;
	m_nNumLines = 4;
	m_sInitText = sInitText;
 	m_nRow		= nRow;
 	m_nCol      = nColumn;
 	m_nLastChar = 0; 
	m_bExitOnArrows = FALSE; //(nFirstChar != VK_LBUTTON);	// If mouse click brought us here,

	// Create the combobox
 	DWORD dwComboStyle = WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_AUTOHSCROLL | dwStyle;

	int nHeight = rect.Height();
	rect.bottom = rect.bottom + m_nNumLines*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);

	if (!Create(dwComboStyle, rect, pParent, nID)) return;

	int i=0;
	// Add the strings
	for (i = 0; i < Items.GetSize(); i++) 
		AddString(Items[i]);

	// Get the maximum width of the text strings
	int nMaxLength = 0;
	CClientDC dc(GetParent());
	CFont* pOldFont = dc.SelectObject(pParent->GetFont());

	for (i = 0; i < Items.GetSize(); i++) 
		nMaxLength = max(nMaxLength, dc.GetTextExtent(Items[i]).cx);

	nMaxLength += (::GetSystemMetrics(SM_CXVSCROLL) + dc.GetTextExtent(_T(" ")).cx*2);
	dc.SelectObject(pOldFont);

    if (nMaxLength > rect.Width())
	    rect.right = rect.left + nMaxLength;

	// Resize the edit window and the drop down window
	MoveWindow(rect);

	SetFont(pParent->GetFont());
	SetItemHeight(-1, nHeight);

	SetDroppedWidth(nMaxLength);
	SetHorizontalExtent(0); // no horz scrolling

	// Set the initial text to m_sInitText
	if (SelectString(-1, m_sInitText) == CB_ERR) 
		SetWindowText(m_sInitText);		// No text selected, so restore what was there before

    // Subclass the combobox edit control if style includes CBS_DROPDOWN
    if ((dwStyle & CBS_DROPDOWNLIST) != CBS_DROPDOWNLIST)
    {
        m_edit.SubclassDlgItem(IDC_COMBOEDIT, this);
 	    SetFocus(); // edited by SKC
        switch (nFirstChar)
        {
            case VK_LBUTTON: 
            case VK_RETURN:   m_edit.SetSel((int)_tcslen(m_sInitText), -1); return;
            case VK_BACK:     m_edit.SetSel((int)_tcslen(m_sInitText), -1); break;
            case VK_DOWN: 
            case VK_UP:   
            case VK_RIGHT:
            case VK_LEFT:  
            case VK_NEXT:  
            case VK_PRIOR: 
            case VK_HOME:  
            case VK_END:      m_edit.SetSel(0,-1); return;
            default:          m_edit.SetSel(0,-1);
        }
        SendMessage(WM_CHAR, nFirstChar);
    }
    else SetFocus(); // edited by SKC
}

CBaseCombo::~CBaseCombo()
{
}

void CBaseCombo::EndEdit()
{
    CString str;
    GetWindowText(str);
 
    // Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = LVN_ENDLABELEDIT;
 
    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nCol;
    dispinfo.item.szText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar; 
 
    CWnd* pOwner = GetOwner();
    if (IsWindow(pOwner->GetSafeHwnd()))
        pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );
 
    // Close this window (PostNcDestroy will delete this)
    PostMessage(WM_CLOSE, 0, 0);
}

void CBaseCombo::PostNcDestroy() 
{
	CComboBox::PostNcDestroy();

	delete this;
}


BEGIN_MESSAGE_MAP(CBaseCombo, CComboBox)
	//{{AFX_MSG_MAP(CBaseCombo)
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseCombo message handlers

void CBaseCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	if (GetSafeHwnd() == pNewWnd->GetSafeHwnd())
        return;

    // Only end editing on change of focus if we're using the CBS_DROPDOWNLIST style
    if ((GetStyle() & CBS_DROPDOWNLIST) == CBS_DROPDOWNLIST)
        EndEdit();
}

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CBaseCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

// Need to keep a lookout for Tabs, Esc and Returns.
void CBaseCombo::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE) 
		SetWindowText(m_sInitText);	// restore previous text

	if (nChar == VK_TAB || nChar == VK_RETURN || nChar == VK_ESCAPE)
	{
		// Enter key가 처음이면 아무일 도 하지 않는다.
		if (nChar == VK_RETURN && m_bFirstEnter) { 
			m_bFirstEnter = FALSE;
			return;
		}

		m_nLastChar = nChar;
		GetParent()->SetFocus();	// This will destroy this window
		return;
	}

	CComboBox::OnKeyUp(nChar, nRepCnt, nFlags);
}
