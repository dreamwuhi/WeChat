#include "StdAfx.h"
#include "MessageWnd.h"
#include <process.h>

CMessageWnd::CMessageWnd(RECT rect)
{
	iconRect = rect;
}


CMessageWnd::~CMessageWnd(void)
{
}

LPCTSTR CMessageWnd::GetWindowClassName() const
{
		return L"hhhFrame";
}

DuiLib::CDuiString CMessageWnd::GetSkinFile()
{
	return L"MessageWnd.xml";
}

DuiLib::CDuiString CMessageWnd::GetSkinFolder()
{
	return L"";
}

void CMessageWnd::Notify(TNotifyUI& msg)
{
	if(msg.sType == _T("windowinit"))
	{
		OnPrepare();
	}
	__super::Notify(msg);
}

unsigned int WINAPI threadProc(void* p)
{
	RECT WndRect;
	POINT point;
	CMessageWnd* ptr = (CMessageWnd*)p;
	GetWindowRect(ptr->GetHWND(),&WndRect);
	 while(ptr->GetStatus())
	 {
		 GetCursorPos(&point);
		 if((point.x >= WndRect.left && point.x <= WndRect.right && point.y >= WndRect.top && point.y <= WndRect.bottom) ||
			 (point.x >= ptr->GetIconRect().left && point.x <= ptr->GetIconRect().right && point.y >= ptr->GetIconRect().top && point.y <= ptr->GetIconRect().bottom))
		 {
			 //鼠标在窗口内
			 continue;
		 }
		 else
		 {
			 //鼠标移出窗口
			 Sleep(500);
			 break;
		 }
		 Sleep(300);
	 }
	 ptr->Close();
	return 0;
}

void CMessageWnd::OnPrepare()
{
	b_start = true;
	CloseHandle((HANDLE)_beginthreadex(NULL,0,threadProc,this,0,NULL));
}

RECT CMessageWnd::GetIconRect()
{
	return iconRect;
}

bool CMessageWnd::GetStatus()
{
	return b_start;
}

void CMessageWnd::SetStatus(bool flag)
{
	b_start = flag;
}
