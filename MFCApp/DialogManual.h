#pragma once


// CDialogManual dialog

class CDialogManual : public CThemeDialog
{
	DECLARE_DYNAMIC(CDialogManual)

public:
	CDialogManual(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDialogManual();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MANUAL };
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
