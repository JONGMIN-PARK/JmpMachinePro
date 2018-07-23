// ThemeHeaderCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "ThemeFactory.h"
#include "HeadCtrl.h"
#include "ThemeListCtrl.h"
#include <vector>
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define NORML		0x00000000
#define MERGE		0x00000001
#define UNUSE		0x00000002

#define SetFlag(n)  DWORD(BYTE(n)<<16)
#define GetFlag(d)  int((d & 0x00FF0000)>>16)
#define SetDown(n)  DWORD(BYTE(n)<<8)
#define GetDown(d)  int((d & 0x0000FF00)>>8)
#define SetRight(n) DWORD(BYTE(n))
#define GetRight(d) int((d & 0x000000FF))

/////////////////////////////////////////////////////////////////////////////
// CHeadCtrl

CHeadCtrl::CHeadCtrl()
{
	m_stThemeInfo.bCheckAll		= FALSE;
	m_stThemeInfo.bUncheckAll   = FALSE;
	m_stThemeInfo.bCheckBoxOn	= FALSE;
	m_stThemeInfo.pBmpCheckBox = NULL;				// 체크박스 이미지
	m_stThemeInfo.bCheckBoxImageSettled = FALSE;	// 체크박스 이미지 존재여부

	m_stThemeInfo.count		= 1;
	m_Info.SetSize(1);
	m_stThemeInfo.nCheckGapLeft = 1;

	// 정렬 방식
	m_stThemeInfo.unSortState = dfSORT_TYPE_NONE;				// 정렬하지 않음
	// 최근 정렬 기준 컬럼 초기화
	m_stThemeInfo.nRecentSortColumn = -1;

	// 어레이 초기화
	m_pArrItemList = new CArray <_item_info, _item_info>;

	m_stThemeInfo.crFrame = RGB(59,98,157);
	m_stThemeInfo.crBack = RGB(207,234,255);
	m_stThemeInfo.crText = RGB(0,0,0);

	m_stThemeInfo.nFontSize = 12;			// '폰트 크기' 초기화
}

CHeadCtrl::~CHeadCtrl()
{
	m_Info.RemoveAll();
}


BEGIN_MESSAGE_MAP(CHeadCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CHeadCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeadCtrl message handlers

int CHeadCtrl::GetHeadHeight()
{
	CRect rect;
	GetItemRect(0, &rect);
	return rect.Height();
}

void CHeadCtrl::SetCount(int nCount, CFont *pFont)
{
	m_stThemeInfo.count = nCount;
	
	m_Info.SetSize(nCount);
}

void CHeadCtrl::DeleteColumn(int nCol)
{
	int nSize = (int)m_Info.GetSize();
	for( int i=0; i<nSize; i++)
	{
		if(m_Info[i].GetSize()>nCol)
			m_Info[i].RemoveAt(nCol);
	}
}

void CHeadCtrl::InsertColumn(int nCol)
{
	for( int i=0; i < m_stThemeInfo.count; i++)
	{
		CHeadCell hCell;
		m_Info[i].SetAtGrow(nCol, hCell);
	}
}

BOOL CHeadCtrl::SetItemText(int nItem, int nSubItem, LPCSTR lpcstr)
{
	if ( m_Info.GetSize() <= nItem )
		return 0;
	if ( m_Info[nItem].GetSize() <= nSubItem )
		return 0;

	m_Info[nItem][nSubItem].string = lpcstr;
	return 1;
}

CString CHeadCtrl::GetItemText(int nItem, int nSubItem)
{
	if(m_Info.GetSize()<=nItem)
		return TEXT("");
	if(m_Info[nItem].GetSize()<=nSubItem)
		return TEXT("");

	return m_Info[nItem][nSubItem].string;
}

BOOL CHeadCtrl::GetRect(int nItem, int nSubItem, CRect &rect)
{
	CRect rectTemp;
	int		down	= 0;
	int		right	  = 0;
	int		nHeight = 0;
	int		nWidth = 0;
	UINT	nFlag	= GetItemState(nItem, nSubItem, down, right);

	if(nFlag==UNUSE)
		return 0;

	GetItemRect(nSubItem, &rect);
	nHeight = rect.Height()/m_stThemeInfo.count;
	rect.top   = nHeight * nItem ;
	rect.bottom= rect.top + ( nHeight * (down+1));

	for ( int i=nSubItem+1; i<= nSubItem + right; i++ )
	{
		GetItemRect(i, &rectTemp);
		nWidth += rectTemp.Width();
	}
	rect.right = rect.right + nWidth;

	return 1;
}

BOOL CHeadCtrl::SetItemMerge(int nsItem, int nsSubItem, int neItem, int neSubItem)
{
	if(m_Info.GetSize()<=neItem)	return 0;
	if(m_Info[neItem].GetSize()<=neSubItem)return 0;

	for( int i=nsItem; i<=neItem; i++)
	{
		for( int j=nsSubItem; j<=neSubItem; j++)
		{
			if(i==nsItem && j==nsSubItem)
				m_Info[i][j].state= SetFlag(MERGE) | 
									SetDown(neItem-nsItem) | 
									SetRight(neSubItem-nsSubItem);
			else
				m_Info[i][j].state=SetFlag(UNUSE);
		}
	}

	return 1;
}

UINT CHeadCtrl::GetItemState(int nItem, int nSubItem, int &down, int &right)
{
	if(m_Info.GetSize()<=nItem)
		return 0;
	if(m_Info[nItem].GetSize()<=nSubItem)
		return 0;

	DWORD dw = m_Info[nItem][nSubItem].state;
	down =GetDown(dw);
	right=GetRight(dw);

	return GetFlag(dw);
}

void CHeadCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CBrush brushBack;
	brushBack.CreateSolidBrush(m_stThemeInfo.crBack);
	CBrush brushFrame;
	brushFrame.CreateSolidBrush(m_stThemeInfo.crFrame);

	dc.SetBkMode(TRANSPARENT);

	CRect rectCC;
	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();
	pCC->GetClientRect(&rectCC);

	CString strText;
	CRect rect;
	CRect RectTemp;
	
	CFont *pftText, *pftOld;
	int nFontSize=m_stThemeInfo.nFontSize;
	//폰트 생성
	pftText = new CFont;
	pftText->CreateFont(nFontSize, 0,0,0, FW_THIN, 0,0,0, DEFAULT_CHARSET, OUT_CHARACTER_PRECIS, 
		CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("돋움"));
	//폰트 등록
	pftOld = dc.SelectObject(pftText);
	dc.SetBkMode(TRANSPARENT);
	
	for ( int i = 0; i < m_stThemeInfo.count; i++ )
	{
		for ( int j=0; j<GetItemCount(); j++ )
		{
			if ( !GetRect(i, j, rect ) )
				continue;

			if ( !IsDraw(rect) )				
				continue;

			CString strText = GetItemText(i, j);

			dc.FrameRect(&rect, &brushFrame);
			
			// FillRect
			RectTemp.left	= rect.left + 1;
			RectTemp.right	= rect.right - 0;
			RectTemp.top	= rect.top + 1;
			RectTemp.bottom = rect.bottom - 1;
			dc.FillRect(&RectTemp, &brushBack);
			
			dc.SetBkColor(m_stThemeInfo.crBack);
			dc.SetTextColor(m_stThemeInfo.crText);


			/////////////////////////////////////////////////////////////////////////

			BOOL bMulti = GetTextRect(&dc, strText, rect);
			if(bMulti)
			{
				dc.DrawText( strText, -1, rect, 
					DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);
			}
			else
			{
				dc.DrawText( strText, -1, rect, 
							DT_CENTER | DT_SINGLELINE | DT_VCENTER  | DT_END_ELLIPSIS);
			}
		}
	}

	// 마지막 컬럼 오른쪽 나머지
	CRect rCtrl;
	this->GetClientRect(&rCtrl);
	rCtrl.right -= 2;
	rCtrl.left = rect.right - 1;
	if( rCtrl.left <= rCtrl.right )
	{
		// 테두리를 그린다
		dc.FrameRect(&rCtrl, &brushFrame);
		// 빈여백을 채운다
		RectTemp.left = rCtrl.left+1;
		RectTemp.right = rCtrl.right - 1;
		RectTemp.top = rCtrl.top + 1;
		RectTemp.bottom = rCtrl.bottom - 1;
		dc.FillRect(&RectTemp, &brushBack);
	}

//////
////// CheckBox
	if ( m_stThemeInfo.bCheckBoxOn )
	{
		CRect RectCheck(m_stThemeInfo.nCheckGapLeft, RectTemp.top + 2, m_stThemeInfo.nCheckGapLeft + 12, RectTemp.top + 13);
		dc.Rectangle(&RectCheck);
		m_stThemeInfo.RectCheck = RectCheck;
		DrawCheckBox();
	}

//////
//////	

	CRect RectClient;
	GetClientRect(&RectClient);
	RectClient.left = rect.right - 1;
	RectClient.top -= 1;
	RectClient.right -= 2;
	// 헤더 전체 테두리를 그린다
	dc.FrameRect(&RectClient, &brushFrame);

	// 폰트를 예전 형식으로 되돌린다
	dc.SelectObject(pftOld);
	delete pftText;

}

BOOL CHeadCtrl::GetTextRect(CDC *pDC, CString strText, CRect &rect)
{
	int nCount=1, nStart=0;
	TEXTMETRIC metrics;
	while(1)
	{
		nStart = strText.Find('\n', nStart)+1;
		if(nStart < 1)	break;
		nCount++;
	}

	if(nCount==1)	
		return 0;

	pDC->GetOutputTextMetrics(&metrics) ;
	rect.top += ((rect.Height() - metrics.tmHeight * nCount)/2);
	return 1;
}

BOOL CHeadCtrl::IsDraw(CRect rect)
{
	CRect rectCC;
	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();
	pCC->GetClientRect(&rectCC);

	int nPos = pCC->GetScrollPos(SB_HORZ);

	if ( nPos > rect.right)
		return 0;

	if ( (nPos+rectCC.Width()) < rect.left)
		return 0;
	
	return 1;
}

BOOL CHeadCtrl::IsChangedColumnWidth()
{
	static int nColWidthOld = 0;

	CRect RectItem;
	int   nColWidth = 0;
	for (int i = 0; i < GetItemCount(); i++)
	{
		GetItemRect(i, &RectItem);
		nColWidth += RectItem.Width();
	}

	if (nColWidth == nColWidthOld)
		return FALSE;

	nColWidthOld = nColWidth;

	return TRUE;
}

LRESULT CHeadCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == 0x1205)
	{
		if (IsChangedColumnWidth())
			Invalidate();
	}
	
	return CHeaderCtrl::DefWindowProc(message, wParam, lParam);
}

void CHeadCtrl::SetCheckGapLeft(int nCheckGap)
{
	m_stThemeInfo.nCheckGapLeft = nCheckGap;
}

void CHeadCtrl::SetColor(COLORREF crFrame, COLORREF crBack, COLORREF crText)
{
	m_stThemeInfo.crFrame = crFrame;
	m_stThemeInfo.crBack = crBack;
	m_stThemeInfo.crText = crText;
}

// '컬럼 테두리 컬러'를 변경하는 함수
void CHeadCtrl::SetFrameColor(COLORREF crFrame)
{
	m_stThemeInfo.crFrame = crFrame;
}

// '컬럼 배경 컬러'를 변경하는 함수
void CHeadCtrl::SetBackColor(COLORREF crBack)
{
	m_stThemeInfo.crBack = crBack;
}

// '컬럼 Text 컬러'를 변경하는 함수
void CHeadCtrl::SetTextColor(COLORREF crText)
{
	m_stThemeInfo.crText = crText;
}

void CHeadCtrl::SetFontSize(int nFontSize)
{
	m_stThemeInfo.nFontSize = nFontSize;
}

// 마우스 포인터로 컬럼 번호를 구해서 반환하는 함수
int CHeadCtrl::GetColNumByPoint(CPoint point)
{
	int i=0, nColCount=0, nSelCol=0, nWidthSum=0;
	CRect rCol;

	// 리스트컨트롤 핸들을 구한다
	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();

	// 컬럼 개수를 구한다
	nColCount = pCC->GetHeaderCtrl()->GetItemCount();

	for(i=0 ; i < nColCount ; i++)
	{
		pCC->GetHeaderCtrl()->GetItemRect(i, &rCol);
		nWidthSum += rCol.Width();
		if( nWidthSum >= point.x )
			return i;
	}

	return -1;
}

// 텍스트 2개를 받아서 Sort가 필요한지 여부를 판단하는 함수
BOOL CHeadCtrl::IsNeedSort(CString strText1, CString strText2, UINT unSortType)
{
	int nValue1=0, nValue2=0;
	BOOL bAsending=TRUE;
	CString str1, str2;

	// 좌우 여백 제거
	strText1.TrimLeft();
	strText1.TrimRight();
	strText2.TrimLeft();
	strText2.TrimRight();

	nValue1 = _wtoi(strText1);
	nValue2 = _wtoi(strText2);
	str1.Format(TEXT("%d"), nValue1);
	str2.Format(TEXT("%d"), nValue2);

	// 문자열이 숫자로만 이루어져 있다면
	if( str1 == strText1 && str2 == strText2 )
	{
		if( nValue1 < nValue2 )
			bAsending = TRUE;
		else
			bAsending = FALSE;
	}
	// 문자열이 문자로 이루어져 있다면
	else
	{
		if( strText1 < strText2 )
			bAsending = TRUE;
		else
			bAsending = FALSE;
	}

	// 정렬방식에 따라서
	switch( unSortType )
	{
	case dfSORT_TYPE_ASENDING :				// '오름차순' 일때
	case dfSORT_TYPE_TOGGLE_ASEND :			// '토글-오름차순' 일때
		return !bAsending;
		break;
	case dfSORT_TYPE_DESENDING :			// '내림차순' 일때
	case dfSORT_TYPE_TOGGLE_DESEND :		// '토글-내림차순' 일때
		return bAsending;
		break;
	default :
		return FALSE;
		break;
	}

	return FALSE;
}

// 2개의 항목 내용을 교환하는 함수
BOOL CHeadCtrl::ChangeTwoItems(int nItem1, int nItem2)
{
	int i=0, nItemCount=0, nColCount=0;
	CString strText1=TEXT(""), strText2=TEXT("");

	// 리스트컨트롤 핸들을 구한다
	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();

	// 항목 개수를 구한다
	nItemCount = pCC->GetItemCount();
	// 교환할 항목이 범위를 벗어나면
	if( nItem1 >= nItemCount || nItem2 >= nItemCount )
		return FALSE;

	// 컬럼 개수를 구한다
	nColCount = pCC->GetHeaderCtrl()->GetItemCount();

	for(i=0 ; i < nColCount ; i++)
	{
		strText1 = pCC->GetItemText(nItem1, i);
		strText2 = pCC->GetItemText(nItem2, i);
		pCC->SetItemText(nItem1, i, strText2);
		pCC->SetItemText(nItem2, i, strText1);
	}

	return TRUE;
}

// '정렬 방식' 설정 함수
void CHeadCtrl::SetSortType(UINT nSortType)
{
	// 정렬 방식
	m_stThemeInfo.unSortState = nSortType;
}

// 클래스 종료시 실행되는 함수
void CHeadCtrl::OnDestroy() 
{
	CHeaderCtrl::OnDestroy();
	
	delete m_pArrItemList;
}

// 항목 리스트 어레이를 정렬하는 함수
void CHeadCtrl::SortArrayItemList()
{
	INT_PTR nItemCount=0, i=0, j=0;
	CString strItemText1=TEXT(""), strItemText2=TEXT("");
	_item_info rItemInfo1, rItemInfo2;

	// 어레이에 저장된 항목 개수
	nItemCount = m_pArrItemList->GetSize();

	for(i=0 ; i < nItemCount-1 ; i++)
	{
		for(j=i+1 ; j < nItemCount ; j++)
		{
			strItemText1 = (*m_pArrItemList)[i].strItemText;		// 비교할 항목 첫번째
			strItemText2 = (*m_pArrItemList)[j].strItemText;		// 비교할 항목 두번째
			// 텍스트 2개를 받아서 Sort가 필요하지 여부를 판단하는 함수
			if( IsNeedSort(strItemText1, strItemText2, m_stThemeInfo.unSortState) == TRUE )
			{
				rItemInfo1 = m_pArrItemList->GetAt(i);
				rItemInfo2 = m_pArrItemList->GetAt(j);
				m_pArrItemList->SetAt(i, rItemInfo2);
				m_pArrItemList->SetAt(j, rItemInfo1);
			}
		}
	}
}

// 기준 컬럼의 소팅 결과에 따라 전체 리스트컨트롤 내용을 재배치하는 함수
void CHeadCtrl::MoveItemTextBySelColumn(int nSelCol)
{
	//CStringArray strArrRow;
	CString strText=TEXT("");
	int i=0, j=0, nRowCount=0, nColCount=0, nRowTo=0, nRowFrom=0, nRowFirstNew=0;

	// 리스트컨트롤 핸들을 구한다
	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();

	// 리스트컨트롤의 항목 개수 구하기
	nRowCount = pCC->GetItemCount();
	// 컬럼 개수를 구한다
	nColCount = this->GetItemCount();

	// 항목 개수를 2배로 늘린다
	for(i=0 ; i < nRowCount ; i++)
		pCC->InsertItem(nRowCount+i, TEXT(""));

	// 리스트컨트롤의 기존 내용을 새로운 위치로 이동
	for(i=0 ; i < nRowCount ; i++)
	{
		for(j=0 ; j < nColCount ; j++)
		{
			strText = pCC->GetItemText((*m_pArrItemList)[i].nOriginNo, j);
			pCC->SetItemText(nRowCount+i, j, strText);
		}
	}

	// 필요없는 기존 내용을 삭제
	for(i=0 ; i < nRowCount ; i++)
		pCC->DeleteItem(0);
}

// 리스트컨트롤 헤더를 더블클릭 했을때 실행되는 함수
void CHeadCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// 정렬 금지 상태라면 함수 탈출
	if( m_stThemeInfo.unSortState == dfSORT_TYPE_NONE )
		return;

	// 마우스 포인터 모양 변경
	SetCapture();
    BeginWaitCursor();

	_item_info rItemInfo;

	int i=0, j=0, nItemCount=0, nColCount=0, nSelCol=0;
	CString	strText1=TEXT(""), strText2=TEXT("";)

	// 리스트컨트롤 핸들을 구한다
	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();

	// 마우스 포인터로 컬럼 번호를 구해서 반환하는 함수
	nSelCol = GetColNumByPoint( point );

	// 정렬 상태를 초기화하는 함수
	InitSortState( nSelCol );

	// 항목 개수를 구한다
	nItemCount = pCC->GetItemCount();
	// 컬럼 개수를 구한다
	nColCount = this->GetItemCount();

	// 항목 어레이 리스트 내용 모두 삭제
	m_pArrItemList->RemoveAll();

	// 해당 컬럼의 내용을 어레이에 저장
	for(i=0 ; i < nItemCount ; i ++)
	{
		// 원래 항목 번호
		rItemInfo.nOriginNo = i;
		// 기준 컬럼 셀 내용
		rItemInfo.strItemText = pCC->GetItemText(i, nSelCol);
		
		m_pArrItemList->Add(rItemInfo);
	}

	// 항목 리스트 어레이를 정렬하는 함수
	SortArrayItemList();

	// 기준 컬럼의 소팅 결과에 따라 전체 리스트컨트롤 내용을 재배치하는 함수
	MoveItemTextBySelColumn( nSelCol );

	// 항목 어레이 리스트 내용 모두 삭제
	m_pArrItemList->RemoveAll();

	// 마우스 포인터 모양 원상 복귀
	ReleaseCapture();
    RestoreWaitCursor();
	
	CHeaderCtrl::OnLButtonDblClk(nFlags, point);
}

// 정렬 상태를 초기화하는 함수
void CHeadCtrl::InitSortState(int nSelCol)
{
	// 정렬 상태가 토글 모드가 아니라면 함수 탈출
	if( m_stThemeInfo.unSortState != dfSORT_TYPE_TOGGLE_ASEND && m_stThemeInfo.unSortState != dfSORT_TYPE_TOGGLE_DESEND )
		return;

	// 정렬기준 컬럼이 최근 정렬 기준 컬럼과 같다면
	if( nSelCol == m_stThemeInfo.nRecentSortColumn )
	{
		// 정렬 방식을 변경하고 함수 탈출
		m_stThemeInfo.unSortState = ( m_stThemeInfo.unSortState == dfSORT_TYPE_TOGGLE_ASEND ) ? dfSORT_TYPE_TOGGLE_DESEND : dfSORT_TYPE_TOGGLE_ASEND;
		return;
	}

	//****** 정렬기준 컬럼이 기존과 다르다면 ******//
	// 정렬 상태를 '토글-오름차순'으로 설정
	m_stThemeInfo.unSortState = dfSORT_TYPE_TOGGLE_ASEND;
	// 정렬기준 컬럼을 전역변수에 저장해둔다
	m_stThemeInfo.nRecentSortColumn = nSelCol;

}

void CHeadCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CHeaderCtrl::OnLButtonUp(nFlags, point);

	CThemeListCtrl* pCC=(CThemeListCtrl*)GetParent();

	pCC->OnHeaderColumnClick(point);

	// Check if the select-all-checkbox was selected or not.
	if ( !m_stThemeInfo.bCheckBoxOn )
		return;

	if ( !m_stThemeInfo.RectCheck.PtInRect(point) )
		return;

	m_stThemeInfo.bUncheckAll = m_stThemeInfo.bCheckAll;
	m_stThemeInfo.bCheckAll = m_stThemeInfo.bCheckAll ? FALSE : TRUE;
	DrawCheckBox();
	
//////
	CRect rectCC;
	
	pCC->GetClientRect(&rectCC);

	if ( m_stThemeInfo.bCheckAll )
	{
		pCC->m_UIntArrCheckBox.RemoveAll();				// remove all first, then add items.
		for ( int i = 0; i < pCC->GetItemCount(); i ++ )
		{
			pCC->m_UIntArrCheckBox.Add(i);
			pCC->RedrawItems(i, i);
		}
	}

	if ( m_stThemeInfo.bUncheckAll )
	{
		pCC->m_UIntArrCheckBox.RemoveAll();
		pCC->RedrawItems(0, pCC->GetItemCount());
	}
//////

	//리스트컨트롤 전체 선택/해제
	int nItemCount=0, i=0;

	nItemCount = pCC->GetItemCount();
	for(i=0 ; i < nItemCount ; i++)
		if( m_stThemeInfo.bCheckAll )
			pCC->SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
		else
			pCC->SetItemState(i, 0, LVIS_SELECTED);
}

// 헤더 체크박스 사용 여부를 설정
void CHeadCtrl::SetCheckBoxOn(BOOL bFlag)
{
	m_stThemeInfo.bCheckBoxOn = bFlag;
}

// 헤더 체크박스 이미지 설정
void CHeadCtrl::SetCheckBoxImage(CBitmap *pBitmap)
{
	m_stThemeInfo.pBmpCheckBox = pBitmap;

	m_stThemeInfo.bCheckBoxImageSettled = TRUE;
}

void CHeadCtrl::DrawCheckBox()
{
	
	// 체크박스 이미지가 존재한다면
	if( m_stThemeInfo.bCheckBoxImageSettled )
		// 체크박스를 이미지로 그린다
		DrawCheckBoxByImage();
	else 
		// 체크박스를 컬러로 그린다
		DrawCheckBoxByColor();
}

// 체크박스를 이미지로 그린다
void CHeadCtrl::DrawCheckBoxByImage()
{
	CClientDC WinDC(this);
	CDC MemDC;

	MemDC.CreateCompatibleDC(&WinDC);

	BITMAP bm;
	m_stThemeInfo.pBmpCheckBox->GetBitmap(&bm);
	CBitmap*	pOldBitmap = (CBitmap*)MemDC.SelectObject(m_stThemeInfo.pBmpCheckBox);
	
	if ( !m_stThemeInfo.bCheckAll )
		WinDC.BitBlt(m_stThemeInfo.RectCheck.left+1, m_stThemeInfo.RectCheck.top+1, bm.bmWidth/2, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	else
		WinDC.BitBlt(m_stThemeInfo.RectCheck.left+1, m_stThemeInfo.RectCheck.top+1, bm.bmWidth/2, bm.bmHeight, &MemDC, bm.bmWidth/2, 0, SRCCOPY);

	MemDC.SelectObject(pOldBitmap);
}

// 체크박스를 컬러로 그린다
void CHeadCtrl::DrawCheckBoxByColor()
{
	CRect rMark;
	CPoint po1, po2;
	CClientDC WinDC(this);

	rMark = CRect(m_stThemeInfo.RectCheck.left+1, m_stThemeInfo.RectCheck.top+1, m_stThemeInfo.RectCheck.left+12, m_stThemeInfo.RectCheck.top+12);

	// 마크 배경을 그린다
	WinDC.FillRect(rMark, &CBrush(RGB(255,255,255)));
	// 테두리 사각형을 그린다
	gf_DrawRectLine(rMark, RGB(0,0,0), RGB(0,0,0), RGB(192,192,192), RGB(192,192,192), (CPaintDC*)&WinDC);

	// 현재 Check 상태라면
	if( m_stThemeInfo.bCheckAll )
	{
		// 라인 위치좌표를 구한다
		po1.x = rMark.left + 3;
		po2.x = rMark.right - 3;
		po1.y = rMark.top + 3;
		po2.y = rMark.bottom - 3;

		// 라인(선)을 그리는 함수
		gf_DrawLine(po1, po2, RGB(0,0,0), 2, (CPaintDC*)&WinDC);

		po1.y = rMark.bottom - 3;
		po2.y = rMark.top + 3;

		// 라인(선)을 그리는 함수
		gf_DrawLine(po1, po2, RGB(0,0,0), 2, (CPaintDC*)&WinDC);
	}
}
