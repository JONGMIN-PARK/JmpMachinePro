#pragma once


// CDialogLog dialog

class CDialogLog : public CThemeDialog
{
	DECLARE_DYNAMIC(CDialogLog)

public:
	CDialogLog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogLog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

//───────────────────────────────────────────────────────────────────────────────────────
// User Define
//───────────────────────────────────────────────────────────────────────────────────────
public:
private:
	bool mOnShowed = false;
	// Contol Position
	CArray<CRect, CRect> mControlPositionArray;
	CRect m_SaveRect;
	void SaveControlInitPosition();
	void ChangeControlPosition();

	// Theme
	void InitDialogSkin();
	void InitControlSkin();
//───────────────────────────────────────────────────────────────────────────────────────
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
