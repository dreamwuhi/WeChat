#pragma once
class CSettingFrame : public WindowImplBase
{
public:
	CSettingFrame(void);
	~CSettingFrame(void);
public:
	LPCTSTR    GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void Notify( TNotifyUI& msg );
	void OnPrepare();
private:
	CButtonUI* pButton_account ;
	CButtonUI* pButton_generic ;
	CButtonUI* pButton_shortcut ;
	CButtonUI* pButton_backup ;
	CButtonUI* pButton_about ;
	CTabLayoutUI* pTabLayout;
};

