
// stdafx.cpp : source file that includes just the standard includes
// MFCApp.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "resource.h"

void gf_SetSkinGroup(CThemeGroup *pGroup)
{
	pGroup->SetBackImage(IDB_GROUP_BACK);
}

void gf_SetSkinStaticTitle(CThemeStatic *pStatic)
{
	pStatic->SetBackColor(dfCOLOR_DLG_BACK);
	pStatic->SetOutlineThick(0);
	pStatic->SetFontColor(dfCOLOR_STATIC_TITLE);
}

void gf_SetSkinEdit(CThemeEdit *pEdit)
{
	pEdit->SetBackColor(dfCOLOR_EDIT_BACK);
	pEdit->SetOutlineLTColor(dfCOLOR_EDIT_OUTLINE_LT);
	pEdit->SetOutlineRBColor(dfCOLOR_EDIT_OUTLINE_RB);
	pEdit->SetFontColor(dfCOLOR_STATIC_TITLE);
}

void gf_SetSkinListBox(CThemeListBox *pListBox)
{
	pListBox->SetBackColor(dfCOLOR_LISTBOX_BACK);
	pListBox->SetOutlineColor(dfCOLOR_LISTBOX_OUTLINE);
	pListBox->SetFontColor(dfCOLOR_LISTBOX_FONT);
	pListBox->SetNormalItemBackColor(dfCOLOR_LISTBOX_NOMAL_ITEM_BACK);
	pListBox->SetItemFrameColor(dfCOLOR_LISTBOX_ITEM_FRAME);
	pListBox->SetSelectItemFontColor(dfCOLOR_LISTBOX_SELECT_ITEM_FONT);
	pListBox->SetSelectItemBackColor(dfCOLOR_LISTBOX_SELECT_ITEM_BACK);
	pListBox->SetItemHeight(dfLISTBOX_ITEM_HEIGHT);
}

void gf_SetSkinBtnExit(CThemeButton *pButton)
{
	pButton->SetWindowText(TEXT(" "));
	pButton->SetBackImage(IDB_BTN_EXIT);
	pButton->SetBackImageFillType(dfBACK_IMAGE_FILL_TYPE_ONCE);
}

void gf_SetSkinBtnInside(CThemeButton *pButton)
{
	pButton->SetBackImage(IDB_BTN_DLG_IN);
	pButton->SetFontColor(dfCOLOR_BTN_INSIDE_FONT);
	pButton->SetFontHoverColor(dfCOLOR_BTN_INSIDE_FONT_HOVER);
	pButton->SetFontPressedColor(dfCOLOR_BTN_INSIDE_FONT_PRESSED);
	pButton->SetShadowColor(dfCOLOR_BTN_INSIDE_SHADOW);
	pButton->SetShadowHoverColor(dfCOLOR_BTN_INSIDE_SHADOW_HOVER);
	pButton->SetShadowPressedColor(dfCOLOR_BTN_INSIDE_SHADOW_PRESSED);
}

void gf_SetSkinComboBox(CThemeCombo *pCombo)
{
	pCombo->SetBackColor(dfCOLOR_LISTBOX_BACK);
	pCombo->SetOutlineColor(dfCOLOR_LISTBOX_OUTLINE);
	pCombo->SetFontColor(dfCOLOR_LISTBOX_FONT);
	pCombo->SetNormalItemBackColor(dfCOLOR_LISTBOX_NOMAL_ITEM_BACK);
	pCombo->SetItemFrameColor(dfCOLOR_LISTBOX_ITEM_FRAME);
	pCombo->SetSelectItemFontColor(dfCOLOR_LISTBOX_SELECT_ITEM_FONT);
	pCombo->SetSelectItemBackColor(dfCOLOR_LISTBOX_SELECT_ITEM_BACK);
	pCombo->SetItemHeight(dfLISTBOX_ITEM_HEIGHT);
	pCombo->SetListBoxHeight(95);
	pCombo->SetButtonImage(IDB_BTN_COMBOBOX_DROP);
}

void gf_SetSkinCheck(CThemeCheck *pCheck)
{
	pCheck->SetBackColor(dfCOLOR_RADIO_BACK);
	pCheck->SetFontColor(dfCOLOR_RADIO_FONT);
	pCheck->SetFontHoverColor(dfCOLOR_RADIO_FONT_HOVER);
	pCheck->SetFontPressedColor(dfCOLOR_RADIO_FONT_PRESSED);
	pCheck->SetShadowColor(dfCOLOR_RADIO_SHADOW);
	pCheck->SetShadowHoverColor(dfCOLOR_RADIO_SHADOW_HOVER);
	pCheck->SetShadowPressedColor(dfCOLOR_RADIO_SHADOW_PRESSED);
	pCheck->SetMarkImage(IDB_CHECK_MARK);
}

void gf_SetSkinRadio(CThemeRadio *pRadio)
{
	pRadio->SetBackColor(dfCOLOR_RADIO_BACK);
	pRadio->SetFontColor(dfCOLOR_RADIO_FONT);
	pRadio->SetFontHoverColor(dfCOLOR_RADIO_FONT_HOVER);
	pRadio->SetFontPressedColor(dfCOLOR_RADIO_FONT_PRESSED);
	pRadio->SetShadowColor(dfCOLOR_RADIO_SHADOW);
	pRadio->SetShadowHoverColor(dfCOLOR_RADIO_SHADOW_HOVER);
	pRadio->SetShadowPressedColor(dfCOLOR_RADIO_SHADOW_PRESSED);
	pRadio->SetMarkImage(IDB_RADIO_MARK);
}

void gf_SetSkinProgress(CThemeProgress *pPregress)
{
	pPregress->SetBackColor(dfCOLOR_PROGRESS_BACK);
	pPregress->SetOutlineLTColor(dfCOLOR_PROGRESS_OUTLINE_LT);
	pPregress->SetOutlineRBColor(dfCOLOR_PROGRESS_OUTLINE_RB);
	pPregress->SetBar1UpColor(dfCOLOR_PROGRESS_BAR1_UP);
	pPregress->SetBar1DownColor(dfCOLOR_PROGRESS_BAR1_DOWN);
	pPregress->SetBar1OutlineColor(dfCOLOR_PROGRESS_BAR1_OUTLINE);
	pPregress->SetBar2UpColor(dfCOLOR_PROGRESS_BAR2_UP);
	pPregress->SetBar2DownColor(dfCOLOR_PROGRESS_BAR2_DOWN);
	pPregress->SetBar2OutlineColor(dfCOLOR_PROGRESS_BAR2_OUTLINE);
	pPregress->SetColorChangePercent(70);
	pPregress->SetDrawText(TRUE);
	pPregress->SetTextUnit(TEXT(" %"));
	pPregress->SetShadowColor(dfCOLOR_DLG_BACK);
}

void gf_SetSkinSlider(CThemeSlider *pSlider)
{
	pSlider->SetBackImage(IDB_SLIDER_BACK_H);
	pSlider->SetBarImage(IDB_SLIDER_BAR);
	pSlider->SetAutoToolTip(TRUE);
}

void gf_SetSkinScrollbarH(CThemeScroll *pScroll)
{
	pScroll->SetBackImage(IDB_SCROLL_BACK_H);
	pScroll->SetBarImage(IDB_SCROLL_BAR_H);
	pScroll->SetPrevImage(IDB_SCROLL_LEFT);
	pScroll->SetNextImage(IDB_SCROLL_RIGHT);
}

void gf_SetSkinBtnTab(CThemeRadio *pRadio)
{
	pRadio->SetBackImage(IDB_BTN_TAB);
	pRadio->SetFontColor(RGB(72, 69, 69));
	pRadio->SetFontHoverColor(RGB(47, 52, 60));
	pRadio->SetFontPressedColor(RGB(47, 52, 60));
	pRadio->SetShadowColor(RGB(166, 143, 77));
	pRadio->SetShadowHoverColor(RGB(140, 105, 53));
	pRadio->SetShadowPressedColor(RGB(140, 105, 53));
	pRadio->SetAlignFormat(DT_CENTER | DT_VCENTER);
}

void gf_SetSkinListCtrl(CThemeListCtrl *pListCtrl)
{
	pListCtrl->SetBackColor(dfCOLOR_LISTBOX_BACK);
	pListCtrl->SetFontColor(dfCOLOR_LISTBOX_FONT);
	pListCtrl->SetNormalItemBackColor(dfCOLOR_LISTBOX_NOMAL_ITEM_BACK);
	pListCtrl->SetItemFrameColor(dfCOLOR_LISTBOX_ITEM_FRAME);
	pListCtrl->SetSelectItemFontColor(dfCOLOR_LISTBOX_SELECT_ITEM_FONT);
	pListCtrl->SetSelectItemBackColor(dfCOLOR_LISTBOX_SELECT_ITEM_BACK);
	pListCtrl->SetItemHeight(dfLISTBOX_ITEM_HEIGHT);
	pListCtrl->SetHeadBackColor(dfCOLOR_LISTCTRL_HEAD_BACK);
	pListCtrl->SetHeadFontColor(dfCOLOR_LISTCTRL_HEAD_FONT);
	pListCtrl->SetHeadFrameColor(dfCOLOR_LISTCTRL_HEAD_FRAME);
}

void gf_SetSkinGrid(CThemeGrid *pGrid)
{
	pGrid->SetBackColor(dfCOLOR_LISTBOX_BACK);
	pGrid->SetNormalFontColor(dfCOLOR_LISTBOX_FONT);
	pGrid->SetNormalBackColor(RGB(55, 55, 55));
	pGrid->SetFrameColor(dfCOLOR_LISTBOX_ITEM_FRAME);
	pGrid->SetCellHeight(dfLISTBOX_ITEM_HEIGHT);
	pGrid->SetFixedBackColor(dfCOLOR_LISTCTRL_HEAD_BACK);
	pGrid->SetFixedFontColor(dfCOLOR_LISTCTRL_HEAD_FONT);
	pGrid->SetFixedFrameLTColor(RGB(160, 160, 160));
	pGrid->SetFixedFrameRBColor(RGB(64, 64, 64));
}

void gf_SetSkinMenu(CThemeMenu *pMenu)
{
	pMenu->SetBackColor(RGB(65, 59, 55));
	pMenu->SetOutlineLTColor(RGB(46, 40, 33));
	pMenu->SetOutlineRBColor(RGB(27, 24, 19));
	pMenu->SetFontColor(RGB(255, 255, 255));
	pMenu->SetFontDisableColor(RGB(109, 106, 104));
	pMenu->SetNormalBackColor(RGB(65, 59, 55));
	pMenu->SetHoverBackColor(RGB(68, 68, 68));
	pMenu->SetPressedBackColor(RGB(199, 171, 1));
	pMenu->SetSeperateUpColor(RGB(109, 106, 104));
	pMenu->SetSeperateDownColor(RGB(160, 157, 155));
}

void gf_SetSkinBtnMenu(CThemeButton *pButton)
{
	pButton->SetBackImage(IDB_BTN_MENU);
	pButton->SetFontColor(RGB(255, 255, 255));
	pButton->SetFontHoverColor(RGB(0, 0, 0));
	pButton->SetFontPressedColor(RGB(244, 186, 47));
}

void gf_SetSkinBtnOutside(CThemeButton *pButton)
{
	pButton->SetBackImage(IDB_BTN_DLG_OUT);
	pButton->SetFontColor(dfCOLOR_BTN_OUTSIDE_FONT);
	pButton->SetFontHoverColor(dfCOLOR_BTN_OUTSIDE_FONT_HOVER);
	pButton->SetFontPressedColor(dfCOLOR_BTN_OUTSIDE_FONT_PRESSED);
	pButton->SetShadowColor(dfCOLOR_BTN_OUTSIDE_SHADOW);
	pButton->SetShadowHoverColor(dfCOLOR_BTN_OUTSIDE_SHADOW_HOVER);
	pButton->SetShadowPressedColor(dfCOLOR_BTN_OUTSIDE_SHADOW_PRESSED);
}

void gf_SetSkinDlgPopup(CThemeDialog *pDialog)
{
	pDialog->SetBackImage(TEXT("IDB_DLG_POPUP_COMMON.gif"));
	pDialog->SetTransparentByColor(TRUE);
	pDialog->SetMouseDragMove(TRUE);
	pDialog->SetCaptionText(TEXT("Input Item"));
	pDialog->SetFontSize(14);
	pDialog->SetFontBold(FALSE);
	pDialog->SetCaptionArea(CRect(1, 1, -1, 57));
	pDialog->SetAlignFormat(DT_CENTER | DT_VCENTER);
}

void gf_SetSkinBtnToolbar(CThemeButton *pButton, CString strFileName)
{
	pButton->SetBackImage(strFileName);
	pButton->SetBackImageFillType(dfBACK_IMAGE_FILL_TYPE_RESIZE);
	pButton->SetFontColor(RGB(255, 255, 255));
	pButton->SetFontHoverColor(RGB(64, 64, 64));
	pButton->SetFontPressedColor(RGB(0, 0, 0));
	pButton->SetFontSize(24);
	pButton->SetAlignFormat(DT_CENTER | DT_VCENTER);
}