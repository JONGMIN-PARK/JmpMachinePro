#pragma once


// CDialogAuto dialog

class CDialogAuto : public CThemeDialog
{
	DECLARE_DYNAMIC(CDialogAuto)

public:
	CDialogAuto(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDialogAuto();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AUTO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// User Define
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
public:
private:
	bool mOnShowed = false;
	CThemeGrid* m_pGridPerson;
	// Control Position
	CArray<CRect, CRect> mControlPositionArray;
	CRect m_SaveRect;
	void SaveControlInitPosition();
	void ChangeControlPosition();

	// Theme
	void InitDialogSkin();
	void InitControlSkin();
	void CreateControl();
	void InitGridPerson();	
	
	std::unique_ptr<jmp::CUIDataManager> mUIDataManager;

	static unsigned int executeLoop(void* obj);
	std::thread loopThread_;	
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

public:
//	afx_msg void OnEnChangeEdit1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnNMDblclkGridPerson(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_pListCtrlPerson;
	afx_msg void OnBnClickedButton2();
};
