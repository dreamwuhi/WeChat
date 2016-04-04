#pragma once
#include "MsgList.h"

class CMessageWnd : public WindowImplBase
{
public:
	CMessageWnd(RECT rect);
	~CMessageWnd(void);
public:
	LPCTSTR    GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual void Notify( TNotifyUI& msg );
	void  OnPrepare();
	RECT GetIconRect();
	bool GetStatus();
	void SetStatus(bool flag);
	CControlUI* CMessageWnd::CreateControl(LPCTSTR pstrClass);
	void UpdateMsgList();
	int GetClickID() const;
private:
	RECT iconRect;
	bool b_start;
	int click_weixing_id;   
};

