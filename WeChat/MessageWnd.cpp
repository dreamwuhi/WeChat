#include "StdAfx.h"
#include "MessageWnd.h"
#include <process.h>

CMessageWnd::CMessageWnd(RECT rect)
{
	iconRect = rect;
}


CMessageWnd::~CMessageWnd(void)
{
	int a = 1;
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
	else if(msg.sType == _T("itemclick"))
	{
		CMsgList* pMsgList = static_cast<CMsgList*>(m_PaintManager.FindControl(L"msgList"));
		if(pMsgList != NULL && pMsgList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();
				if(node == NULL) return;
				 click_weixing_id = node->data().weixing_id;
				 Close();
			}
		}
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
	 _endthreadex(0);
	return 0;
}

void CMessageWnd::OnPrepare()
{
	b_start = true;
	UpdateMsgList();
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

CControlUI* CMessageWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("MsgList")) == 0)   //自定义控件（列表框）
	{
		return new CMsgList(m_PaintManager);
	}
	return NULL;
}

void CMessageWnd::UpdateMsgList()
{
	TroyMsgData item;
	memset(&item,0,sizeof(TroyMsgData));
	item.LogoPath = L"c4.png";
	item.name = L"程强";
	item.weixing_id = 001;
	CMsgList* pMsgList = static_cast<CMsgList*>(m_PaintManager.FindControl(L"msgList"));
	if(pMsgList)
	{
		if(pMsgList->GetCount() > 0)
			 pMsgList->RemoveAll();
		Node* root = pMsgList->AddNode(item,NULL);
		root = pMsgList->AddNode(item,NULL);
	}
}

int CMessageWnd::GetClickID() const
{
	return click_weixing_id;
}
