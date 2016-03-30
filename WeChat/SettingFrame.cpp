#include "StdAfx.h"
#include "SettingFrame.h"


CSettingFrame::CSettingFrame(void)
{
	pButton_account = NULL;
	pButton_generic = NULL;
	pButton_shortcut = NULL;
	pButton_backup = NULL;
	pButton_about = NULL;
	pTabLayout = NULL;
}


CSettingFrame::~CSettingFrame(void)
{
}

LPCTSTR CSettingFrame::GetWindowClassName() const
{
	return L"settingFrame";
}

DuiLib::CDuiString CSettingFrame::GetSkinFile()
{
	return L"setting.xml";
}

DuiLib::CDuiString CSettingFrame::GetSkinFolder()
{
	return L"";
}

void CSettingFrame::Notify(TNotifyUI& msg)
{
	CDuiString SenderName = msg.pSender->GetName();
	if(msg.sType == _T("click"))				//按钮点击事件
	{
		if(SenderName == L"account_set")
		{
			pButton_account->SetTextColor(0x3dce3d);
			pButton_generic->SetTextColor(0);
			pButton_shortcut->SetTextColor(0);
			pButton_backup->SetTextColor(0);
			pButton_about->SetTextColor(0);
			if(pTabLayout)
			{
				pTabLayout->SelectItem(0);
			}
		}
		else if(SenderName == L"generic_set")
		{
			pButton_account->SetTextColor(0);
			pButton_generic->SetTextColor(0x3dce3d);
			pButton_shortcut->SetTextColor(0);
			pButton_backup->SetTextColor(0);
			pButton_about->SetTextColor(0);
			if(pTabLayout)
			{
				pTabLayout->SelectItem(1);
			}
		}
		else if(SenderName == L"shortcut_set")
		{
			pButton_account->SetTextColor(0);
			pButton_generic->SetTextColor(0);
			pButton_shortcut->SetTextColor(0x3dce3d);
			pButton_backup->SetTextColor(0);
			pButton_about->SetTextColor(0);
			if(pTabLayout)
			{
				pTabLayout->SelectItem(2);
			}
		}
		else if(SenderName == L"backup_set")
		{
			pButton_account->SetTextColor(0);
			pButton_generic->SetTextColor(0);
			pButton_shortcut->SetTextColor(0);
			pButton_backup->SetTextColor(0x3dce3d);
			pButton_about->SetTextColor(0);
			if(pTabLayout)
			{
				pTabLayout->SelectItem(3);
			}
		}
		else if(SenderName == L"about_set")
		{
			pButton_account->SetTextColor(0);
			pButton_generic->SetTextColor(0);
			pButton_shortcut->SetTextColor(0);
			pButton_backup->SetTextColor(0);
			pButton_about->SetTextColor(0x3dce3d);
			if(pTabLayout)
			{
				pTabLayout->SelectItem(4);
			}
		}
	}
	else if(msg.sType == _T("windowinit"))
	{
		OnPrepare();
	}
	__super::Notify(msg);
}

void CSettingFrame::OnPrepare()
{
	pButton_account  = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"account_set"));
	pButton_generic  = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"generic_set"));
	pButton_shortcut = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"shortcut_set"));;
	pButton_backup = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"backup_set"));;
	pButton_about = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"about_set"));;
	pTabLayout = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(L"default_bk"));

	pButton_account->SetTextColor(0x3dce3d);  //默认选中账号设置
}
