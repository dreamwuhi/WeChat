#pragma once
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
private:
	RECT iconRect;
	bool b_start;
};

