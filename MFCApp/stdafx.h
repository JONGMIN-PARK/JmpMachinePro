
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS         // remove support for MFC controls in dialogs

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
// Theme Control
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式
#include "ThemeDialog.h"

#define dfCOLOR_DLG_BACK						RGB(62,62,62)
#define dfCOLOR_STATIC_TITLE					RGB(255,255,255)
#define dfCOLOR_EDIT_BACK						RGB(47,46,46)
#define dfCOLOR_EDIT_OUTLINE_LT					RGB(36,35,35)
#define dfCOLOR_EDIT_OUTLINE_RB					RGB(110,105,103)

#define dfCOLOR_BTN_INSIDE_FONT					RGB(251,255,253)
#define dfCOLOR_BTN_INSIDE_FONT_HOVER			RGB(25,42,52)
#define dfCOLOR_BTN_INSIDE_FONT_PRESSED			RGB(251,255,253)
#define dfCOLOR_BTN_INSIDE_SHADOW				RGB(100,144,140)
#define dfCOLOR_BTN_INSIDE_SHADOW_HOVER			RGB(29,88,131)
#define dfCOLOR_BTN_INSIDE_SHADOW_PRESSED		RGB(100,144,140)

#define dfCOLOR_BTN_OUTSIDE_FONT				RGB(72,69,69)
#define dfCOLOR_BTN_OUTSIDE_FONT_HOVER			RGB(0,0,0)
#define dfCOLOR_BTN_OUTSIDE_FONT_PRESSED		RGB(48,52,60)
#define dfCOLOR_BTN_OUTSIDE_SHADOW				RGB(179,124,69)
#define dfCOLOR_BTN_OUTSIDE_SHADOW_HOVER		RGB(211,140,56)
#define dfCOLOR_BTN_OUTSIDE_SHADOW_PRESSED		RGB(194,137,67)

#define dfCOLOR_LISTBOX_BACK					RGB(43,42,42)
#define dfCOLOR_LISTBOX_OUTLINE					RGB(85,85,85)
#define dfCOLOR_LISTBOX_FONT					RGB(251,255,251)
#define dfCOLOR_LISTBOX_NOMAL_ITEM_BACK			RGB(43,42,42)
#define dfCOLOR_LISTBOX_ITEM_FRAME				RGB(43,42,42)
#define dfCOLOR_LISTBOX_SELECT_ITEM_FONT		RGB(251,255,251)
#define dfCOLOR_LISTBOX_SELECT_ITEM_BACK		RGB(68,68,68)
#define dfLISTBOX_ITEM_HEIGHT					22

#define dfCOLOR_LISTCTRL_HEAD_BACK				RGB(63,60,56)
#define dfCOLOR_LISTCTRL_HEAD_FONT              RGB(218,221,218)
#define dfCOLOR_LISTCTRL_HEAD_FRAME				RGB(85,85,85)

#define dfCOLOR_RADIO_BACK						RGB(62,62,62)
#define dfCOLOR_RADIO_FONT						RGB(248,251,248)
#define dfCOLOR_RADIO_FONT_HOVER				RGB(205,211,206)
#define dfCOLOR_RADIO_FONT_PRESSED				RGB(137,147,139)
#define dfCOLOR_RADIO_SHADOW					RGB(104,79,67)
#define dfCOLOR_RADIO_SHADOW_HOVER				RGB(99,78,68)
#define dfCOLOR_RADIO_SHADOW_PRESSED			RGB(69,65,63)

#define dfCOLOR_PROGRESS_BACK					RGB(41,40,40)
#define dfCOLOR_PROGRESS_OUTLINE_LT				RGB(25,24,23)
#define dfCOLOR_PROGRESS_OUTLINE_RB				RGB(131,127,124)
#define dfCOLOR_PROGRESS_BAR1_UP				RGB(252,204,86)
#define dfCOLOR_PROGRESS_BAR1_DOWN				RGB(251,178,0)
#define dfCOLOR_PROGRESS_BAR1_OUTLINE			RGB(224,185,92)
#define dfCOLOR_PROGRESS_BAR2_UP				RGB(85,189,66)
#define dfCOLOR_PROGRESS_BAR2_DOWN				RGB(42,173,19)
#define dfCOLOR_PROGRESS_BAR2_OUTLINE			RGB(100,219,19)

void gf_SetSkinStaticTitle(CThemeStatic *pStatic);
void gf_SetSkinGroup(CThemeGroup *pGroup);
void gf_SetSkinEdit(CThemeEdit *pEdit);
void gf_SetSkinListBox(CThemeListBox *pListBox);
void gf_SetSkinBtnExit(CThemeButton *pButton);
void gf_SetSkinBtnInside(CThemeButton *pButton);
void gf_SetSkinComboBox(CThemeCombo *pCombo);
void gf_SetSkinCheck(CThemeCheck *pCheck);
void gf_SetSkinRadio(CThemeRadio *pRadio);
void gf_SetSkinProgress(CThemeProgress *pPregress);
void gf_SetSkinSlider(CThemeSlider *pSlider);
void gf_SetSkinScrollbarH(CThemeScroll *pScroll);
void gf_SetSkinBtnTab(CThemeRadio *pRadio);
void gf_SetSkinListCtrl(CThemeListCtrl *pListCtrl);
void gf_SetSkinGrid(CThemeGrid *pGrid);
void gf_SetSkinMenu(CThemeMenu *pMenu);
void gf_SetSkinBtnMenu(CThemeButton *pButton);
void gf_SetSkinBtnOutside(CThemeButton *pButton);
void gf_SetSkinDlgPopup(CThemeDialog *pDialog);
void gf_SetSkinBtnToolbar(CThemeButton *pButton, CString strFileName);
//式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <iostream>
#include <map>
#include <vector>

#ifdef JMP_EXPORTS
#define JMPDLL_API __declspec(dllexport)
#else
#define JMPDLL_API __declspec(dllimport)
#endif

#include "IScanLabRtc.h"
#include "CScanLabRtc5.h"
#include "CScanLabRtc6.h"
#include "IMotionManager.h"
#include "CTurboPmacManager.h"


#include "UIDataManager.h"

#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

const int MAIN_DISPLAY_WIDTH = 1920;
const int MAIN_DISPLAY_HEIGHT = 1080;
const COLORREF kSubDialogBackColor = RGB(62, 62, 62);