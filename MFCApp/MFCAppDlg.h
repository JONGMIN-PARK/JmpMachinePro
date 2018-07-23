
// MFCAppDlg.h : header file
//

#pragma once

class CDialogAuto;
class CDialogManual;
class CDialogRecipe;
class CDialogMaint;
class CDialogConfig;
class CDialogLog;

// CMFCAppDlg dialog
class CMFCAppDlg : public CThemeDialog
{
// Construction
public:
	CMFCAppDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:	
	afx_msg void OnSize(UINT nType, int cx, int cy);

//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// User Define
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
public:
private:
	// Display
	CDialogAuto* mDialogAuto = nullptr;
	CDialogManual* mDialogManual = nullptr;
	CDialogRecipe* mDialogRecipe = nullptr;
	CDialogMaint* mDialogMaint = nullptr;
	CDialogConfig* mDialogConfig = nullptr;
	CDialogLog* mDialogLog = nullptr;

	CThemeDialog* mLastDisplay = nullptr;
	int mLastDisplayIndex = 0;

	// Control Position
	CArray<CRect, CRect> mControlPositionArray;
	CRect m_SaveRect;
	void SaveControlInitPosition();
	void ChangeControlPosition();

	// Theme
	void InitDialogSkin();
	void InitControlSkin();

	void OnSelectedMainMenu(UINT id);	
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();		
};
