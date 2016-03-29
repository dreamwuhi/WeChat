#include "StdAfx.h"
#include "MainFrame.h"
#include "resource.h"
#include "SettingFrame.h"

#define WM_MYMSG_01    9999			//聊天窗口滚动条消息（目的：实现实时显示最后一条消息）
#define WM_ICON	WM_USER + 1024     //托盘消息
CMainFrame::CMainFrame(void)
{
	icon = NULL;
	Msgcount = 0;						//消息条数
	pButton_MsgTip = NULL;			//显示消息条数控件
	m_history = new CHistory();
	selectItemNode = NULL;
}


CMainFrame::~CMainFrame(void)
{
	m_history->SaveHisToLocal("../Debug/cached/001.txt");
	if(icon)
	{
		delete icon;
		icon = NULL;
	}
	if(m_history)
	{
		delete m_history;
		m_history = NULL;
	}
}

LPCTSTR CMainFrame::GetWindowClassName() const
{
	return _T("DUIMainFrame");
}

DuiLib::CDuiString CMainFrame::GetSkinFile()
{
	return _T("OwnerUI.xml");
}

DuiLib::CDuiString CMainFrame::GetSkinFolder()
{
	return _T("skin");
}

void CMainFrame::OnPrepare()
{
	icon = new CDuiTrayIcon();
	icon->CreateTrayIcon(m_hWnd,/*IDI_WECHAT*/IDI_WE_CHAT,L"微信呵呵",WM_ICON);  //启动托盘
	//icon->StartTwinkling();
	pButton_MsgTip =  static_cast<CButtonUI*>(m_PaintManager.FindControl(L"Msg_Tip_Main"));
	m_history->LoadHistoryFromLocal("../Debug/cached/001.txt");
	UpgrateFriends();
	UpgrateContacts();
}
 
void CMainFrame::Notify(TNotifyUI& msg)
{
	CDuiString SenderName = msg.pSender->GetName();
	if(msg.sType == _T("click"))				//按钮点击事件
	{
		if(SenderName == L"send")
			OnSendMessage();
		else if(SenderName == L"setting")  //点击主界面上的设置按钮
		{
			CSettingFrame *settingFrame =new CSettingFrame();
			settingFrame->Create(NULL, _T(""), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
			settingFrame->CenterWindow();
			settingFrame->ShowModal();
			delete settingFrame;
			settingFrame = NULL;
		}
		else if(SenderName == L"Closebtn")
		{
			ShowWindow(SW_HIDE);  //点击关闭最小化到托盘
		}
	}
	else if(msg.sType == _T("windowinit"))
	{
		OnPrepare();
	}
	else if(msg.sType  == _T("itemclick"))
	{
		CFriendListUI* pFriendsList = static_cast<CFriendListUI*>(m_PaintManager.FindControl(L"friends"));
		if(pFriendsList != NULL && pFriendsList->GetItemIndex(msg.pSender) != -1)
			OnSelectFriendList(msg, pFriendsList);

		CContactUI* pContactsList = static_cast<CContactUI*>(m_PaintManager.FindControl(L"contacts"));
		if(pContactsList != NULL && pContactsList->GetItemIndex(msg.pSender) != -1)
			OnSelectContactList(msg, pContactsList);
	}
	__super::Notify(msg);
}

LRESULT CMainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch(uMsg)
	{
	case WM_KEYDOWN:
		lRes = OnKeyReturn(uMsg,wParam,lParam,bHandled);
		break;
	case WM_TIMER:
		lRes = OnOwnTimer(uMsg,wParam,lParam,bHandled);
		break;
	case WM_NCLBUTTONDBLCLK:
		lRes= 0;
		break;
	case WM_NCHITTEST:     
		lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); 
		break;
	case WM_ICON:
		lRes = On_TroyIcon(uMsg, wParam, lParam, bHandled);   //处理托盘消息
		break;
	default:
		bHandled = FALSE;
	}
	if(bHandled ) return lRes;
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMainFrame::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) )//窗口放大
	{
		RECT rcSizeBox = m_PaintManager.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top )
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom )
		{
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}

		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_PaintManager.GetCaptionRect();
	//鼠标点击的位置在标题栏内
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) 
	{
		CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
		if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
			_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("SliderUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("EditUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("RichEditUI")) != 0 &&
			_tcsicmp(pControl->GetClass(), _T("ComboUI")) != 0)
			return HTCAPTION;
	}
	return HTCLIENT;
}

CControlUI* CMainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)   //自定义控件（列表框）
	{
		return new CFriendListUI(m_PaintManager);
	}
	else if(_tcsicmp(pstrClass, _T("BubbleChatList")) == 0)
	{
		return new CBubbleChat(m_PaintManager);
	}
	else if(_tcsicmp(pstrClass, _T("ContactList")) == 0)
	{
		return new CContactUI(m_PaintManager);
	}
	return NULL;
}

void CMainFrame::UpgrateContacts()
{
	CContactUI* pContactsList = static_cast<CContactUI*>(m_PaintManager.FindControl(L"contacts"));
	if(pContactsList != NULL)
	{
		if (!contact_.empty())					//清空 列表（群+公众号+好友）
			contact_.clear();

		ContactListItemInfo item={0};
		item.nick_name = _T("公众号");
		item.folder = true;
		item.id = OTHERS;
		Node* root_parent = pContactsList->AddNode(item, NULL);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("公众号");
		item.logo = _T("logo.png");
		item.folder = false;
		item.id = PUBLIC_NO;
		pContactsList->AddNode(item, root_parent);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("群聊");
		item.folder = true;
		item.id = GROUP_CHAT;
		Node* root_parent1 = pContactsList->AddNode(item, NULL);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("10安全1");
		item.logo = _T("qun1.png");
		item.group_id = 1;		//1号群
		item.folder = false;
		item.id = GROUP_CHAT;
		pContactsList->AddNode(item, root_parent1);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("九哥、王辉、豪达");
		item.logo = _T("qun2.png");
		item.group_id = 2;		//1号群
		item.folder = false;
		item.id = GROUP_CHAT;
		pContactsList->AddNode(item, root_parent1);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("宝宝想打麻将");
		item.logo = _T("qun3.png");
		item.group_id = 3;		//1号群
		item.folder = false;
		item.id = GROUP_CHAT;
		pContactsList->AddNode(item, root_parent1);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("浩哥全球后援团红包营");
		item.logo = _T("qun4.png");
		item.group_id = 4;		//1号群
		item.folder = false;
		item.id = GROUP_CHAT;
		pContactsList->AddNode(item, root_parent1);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("C");
		item.folder = true;
		item.id = OTHERS;
		Node* root_parent2 = pContactsList->AddNode(item, NULL);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("陈哲");
		item.logo = _T("c1_1.png");
		item.description = L"除了知情权，人应当还拥有不知情权";
		item.back_name = L"草哥";
		item.weixin_id = L"zhe5960";
		item.zone = L"浙江 杭州";
		item.group_id = 1;		//1号群
		item.folder = false;
		item.id = FRIENDS;
		pContactsList->AddNode(item, root_parent2);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("柴泽磊");
		item.logo = _T("c2.png");
		item.description = L"王者伐道，顺势而为。";
		item.back_name = L"";
		item.weixin_id = L"czllry";
		item.zone = L"浙江 杭州";
		item.group_id = 1;		//1号群
		item.folder = false;
		item.id = FRIENDS;
		pContactsList->AddNode(item, root_parent2);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("坏小孩");
		item.logo = _T("c3.png");
		item.description = L"";
		item.back_name = L"程方";
		item.group_id = 2;		//1号群
		item.weixin_id = L"";
		item.zone = L"浙江 舟山";
		item.folder = false;
		item.id = FRIENDS;
		pContactsList->AddNode(item, root_parent2);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("程强");
		item.description = L"合适才是最好";
		item.group_id = 2;		//1号群
		item.back_name = L"";
		item.weixin_id = L"cq_52apple1314";
		item.zone = L"浙江 衢州";
		item.logo = _T("c4.png");
		item.folder = false;
		item.id = FRIENDS;
		pContactsList->AddNode(item, root_parent2);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("玉麒麟qs杰");
		item.description = L"情菲 得意";
		item.back_name = L"陈杰";
		item.group_id = 3;		//1号群
		item.weixin_id = L"qinshoujie";
		item.zone = L"浙江 舟山";
		item.logo = _T("c5.png");
		item.folder = false;
		item.id = FRIENDS;
		pContactsList->AddNode(item, root_parent2);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		memset(&item,0,sizeof(ContactListItemInfo));
		item.nick_name = _T("D");
		item.id = OTHERS;
		item.folder = true;
		Node* root_parent3 = pContactsList->AddNode(item, NULL);		//添加根节点，返回自身节点（根）
		contact_.push_back(item);

		for (int i=0; i<5; ++i)
		{
			item.nick_name = _T("陈哲");
			item.logo = _T("c1.png");
			item.description = L"除了知情权，人应当还拥有不知情权";
			item.back_name = L"草哥";
			item.group_id = 1;		//1号群
			item.weixin_id = L"zhe5960";
			item.zone = L"浙江 杭州";
			item.folder = false;
			item.id = FRIENDS;
			pContactsList->AddNode(item, root_parent3);		//添加根节点，返回自身节点（根）
			contact_.push_back(item);

			item.nick_name = _T("柴泽磊");
			item.logo = _T("c2.png");
			item.description = L"王者伐道，顺势而为。";
			item.back_name = L"";
			item.weixin_id = L"czllry";
			item.group_id = 1;		//1号群
			item.zone = L"浙江 杭州";
			item.folder = false;
			item.id = FRIENDS;
			pContactsList->AddNode(item, root_parent3);		//添加根节点，返回自身节点（根）
			contact_.push_back(item);

			item.nick_name = _T("坏小孩");
			item.logo = _T("c3.png");
			item.description = L"";
			item.back_name = L"程方";
			item.group_id = 2;		//1号群
			item.weixin_id = L"";
			item.zone = L"浙江 舟山";
			item.folder = false;
			item.id = FRIENDS;
			pContactsList->AddNode(item, root_parent3);		//添加根节点，返回自身节点（根）
			contact_.push_back(item);

			item.nick_name = _T("程强");
			item.description = L"合适才是最好";
			item.group_id = 2;		//1号群
			item.back_name = L"";
			item.weixin_id = L"cq_52apple1314";
			item.zone = L"浙江 衢州";
			item.logo = _T("c4.png");
			item.folder = false;
			item.id = FRIENDS;
			pContactsList->AddNode(item, root_parent3);		//添加根节点，返回自身节点（根）
			contact_.push_back(item);

			item.nick_name = _T("玉麒麟qs杰");
			item.description = L"情菲 得意";
			item.group_id = 3;		//1号群
			item.back_name = L"陈杰";
			item.weixin_id = L"qinshoujie";
			item.zone = L"浙江 舟山";
			item.logo = _T("c5.png");
			item.folder = false;
			item.id = FRIENDS;
			pContactsList->AddNode(item, root_parent3);		//添加根节点，返回自身节点（根）
			contact_.push_back(item);
		}
	}
}

void CMainFrame::UpgrateFriends()    //最近联系人
{
	CFriendListUI* pFriendsList = static_cast<CFriendListUI*>(m_PaintManager.FindControl(L"friends"));
	if(pFriendsList != NULL)
	{
		if (!friends_.empty())					//清空 最近联系列表
			friends_.clear();
		if(pFriendsList->GetCount() > 0)
			pFriendsList->RemoveAll();

		FriendListItemInfo item = {0};
		item.nick_name = _T("马化腾");
		item.logo = _T("c2.png");
		item.id = LAST_CONTACT;
		item.folder = false;
		item.weixing_id = 001;
		Node* root = pFriendsList->AddNode(item, NULL);		//添加根节点，返回自身节点（根）
		friends_.push_back(item);
		//添加缓存信息
		AddCachMsg(root,item.weixing_id);
		for (int i=0; i<1; ++i)
		{
			memset(&item,0,sizeof(FriendListItemInfo));
			item.nick_name = _T("马化腾");
			item.logo = _T("c2.png");
			item.id = LAST_CONTACT;
			item.weixing_id = 002;
			root = pFriendsList->AddNode(item, NULL);		//添加根节点，返回自身节点（根）
			friends_.push_back(item);
			//添加缓存信息
			AddCachMsg(root,item.weixing_id);
		}
	}
}

DuiLib::ContactListItemInfo* CMainFrame::FindContactItem(CDuiString &name)
{
	for (unsigned int i=0; i<contact_.size(); ++i)
	{
		if(contact_[i].nick_name == name)
			return &contact_[i];
	}
	return NULL;
}

std::vector<ContactListItemInfo*> CMainFrame::FindContactItemBygroupID(UINT id)
{
	std::vector<ContactListItemInfo*> res;
	for (UINT i=0; i<contact_.size(); ++i)
	{
		if(contact_[i].group_id == id && contact_[i].id == FRIENDS)
		{
			res.push_back(&contact_[i]);
		}
	}
	return res;
}

void CMainFrame::OnSelectFriendList(TNotifyUI& msg, CFriendListUI* pFriendsList)
{
	if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
	{
		Node* node = (Node*)msg.pSender->GetTag();
		if(node == NULL) return;
		//selectNode.insert(node);
		selectItemNode = node;
		weixing_id = node->data().weixing_id;
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("default_bk")));
		if(pControl == NULL) return;

		if(node->data().type_ == LAST_CONTACT)			//最近联系人
		{
			CTileLayoutUI* m_group_list = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(L"group_list"));	
			if(m_group_list)
			{
				pControl->SelectItem(3);
				CBubbleChat* pBubbleList = static_cast<CBubbleChat*>(m_PaintManager.FindControl(L"Bubble_Chat"));
				if(pBubbleList != NULL)
				{
					int n = pBubbleList->GetCount();
					if(pBubbleList->GetCount() > 0)
						pBubbleList->RemoveAll();
					//m_history->DeleteMsg();
					m_history->LoadHistory(weixing_id,pBubbleList);   //加载该微信号的聊天记录
				}
			}
		}
	}
}

void CMainFrame::OnSelectContactList(TNotifyUI& msg, CContactUI* pFriendsList)
{
	if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
	{
		Node* node = (Node*)msg.pSender->GetTag();
		if(node == NULL) return;
		CTabLayoutUI* pControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("default_bk")));
		if(pControl == NULL) return;

		if(node->data().type_ == GROUP_CHAT)				//选择了群聊
		{
			CTileLayoutUI* m_group_list = static_cast<CTileLayoutUI*>(m_PaintManager.FindControl(L"group_list"));			
			if(m_group_list)
			{
				static std::vector<CButtonUI*> vec;
				if(m_group_list->GetCount() > 0)
					m_group_list->RemoveAll();
				if(vec.size() > 0)
					vec.clear();
				std::vector<ContactListItemInfo*> res = FindContactItemBygroupID(node->data().group_id);
				if(res.size() > 0)
				{
					TCHAR szPath[MAX_PATH]={0};
					RECT rcTextPadding={0,73,0,0};
					for (UINT i=0; i<res.size();++i)
					{
						CButtonUI *pBtn = new CButtonUI;   //不清楚是在哪里delete
						_stprintf(szPath, _T("file='%s' dest='5,10,66,71'"),res[i]->logo);
						pBtn->SetBkImage(szPath);
						pBtn->SetHotImage(L"file='bk_hover.png' corner='5,10,5,37' hole='true'");
						pBtn->SetPushedImage(L"file='bk_hover.png' corner='5,10,5,37' hole='true'");
						pBtn->SetTextPadding(rcTextPadding);
						pBtn->SetText(res[i]->nick_name);
						pBtn->SetFixedHeight(108);
						pBtn->SetFixedWidth(71);
						m_group_list->Add(pBtn);
						vec.push_back(pBtn);
					}
				}
			}
			pControl->SelectItem(2);
		}
		else if(node->data().type_ == PUBLIC_NO)		//选择了公众号
		{
			//icon->StopTwinkling();
			pControl->SelectItem(0);
		}
		else if(node->data().type_ == FRIENDS)			//选择了好友
		{
			CButtonUI* m_friend_face = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"friend_face"));
			CLabelUI* m_friend_name = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"friend_name"));
			CLabelUI* m_friend_sign = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"friend_sign"));
			CLabelUI* m_back_name = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"back_name"));
			CLabelUI* m_weixin_id = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"weixin_id"));
			CLabelUI* m_zone = static_cast<CLabelUI*>(m_PaintManager.FindControl(L"zone"));

			ContactListItemInfo* item = FindContactItem(node->data().text_);
			if(item != NULL)
			{
				m_friend_face->SetNormalImage(item->logo);
				m_friend_name->SetText(item->nick_name);
				if(item->description.IsEmpty())
					m_friend_sign->SetVisible(false);
				else
				{
					m_friend_sign->SetVisible(true);
					m_friend_sign->SetText(item->description);
				} 
				if(item->back_name.IsEmpty())
				{
					m_back_name->SetText(L"点击添加备注");
					//m_back_name->SetBkColor(1000);
				}
				else
					m_back_name->SetText(item->back_name);
				if(item->weixin_id.IsEmpty())
					m_weixin_id->SetVisible(false);
				else
				{
					m_weixin_id->SetVisible(true);
					m_weixin_id->SetText(item->weixin_id);
				}
				m_zone->SetText(item->zone);
			}
			pControl->SelectItem(1);
		}
	}
}

void CMainFrame::OnSendMessage()
{
	//聊天窗口添加文字
	CBubbleChat* pBubbleList = static_cast<CBubbleChat*>(m_PaintManager.FindControl(L"Bubble_Chat"));
	//pBubbleList->SetHwnd(m_hWnd);
	BubbleItemInfo item;
	memset(&item,0,sizeof(BubbleItemInfo));
	CRichEditUI* pInputEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"input_edit"));
	if(pBubbleList)
	{	
		memset(item.buf,0,MAX_PATH);
		wcscpy(item.buf,pInputEdit->GetText().GetData());
		if(wcslen(item.buf)>0)
		{
			//保存聊天记录
			TCHAR buffer[50];
			SYSTEMTIME sys; 
			GetLocalTime( &sys );
			swprintf(buffer,L"%04d-%02d-%02d-%02d:%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute);
			m_history->AddHistory(weixing_id,item.buf,buffer);
			//............
			pBubbleList->AddNode(item,NULL);
			//添加缓存内容
			CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(selectItemNode->data().list_elment_, L"Buffer_Text"));
			if(pLabel)
				pLabel->SetText(pInputEdit->GetText().GetData());
			//设置时间
			GetLocalTime( &sys );
			CLabelUI* pLabel_Time = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(selectItemNode->data().list_elment_, L"time"));
			if(pLabel_Time)
			{
				swprintf(buffer,L"%02d:%02d",sys.wHour,sys.wMinute);
				pLabel_Time->SetText(buffer);
			}
			//清空 输入内容
			pInputEdit->SetFocus();
			pInputEdit->SetText(L"");
			SetTimer(m_hWnd,WM_MYMSG_01,80,NULL);
			//消息提示
			CButtonUI* pControl = static_cast<CButtonUI*>(m_PaintManager.FindSubControlByName(selectItemNode->data().list_elment_, L"MsgTip"));
			if(!pControl->IsVisible()) 
				pControl->SetVisible();
			char buf[10];
			WideCharToMultiByte(CP_ACP, 0, pControl->GetText().GetData(), -1, buf, 10, NULL, NULL);
			int num = atoi(buf);
			num++;
			memset(buffer,0,sizeof(buffer));
			swprintf(buffer,L"%d",num);
			pControl->SetText(buffer);
			AddMegTip(1);
			//AddLastMsg(pInputEdit->GetText().GetData());
		}
		else
			MessageBox(m_hWnd,L"不能发送空白消息",L"error",MB_OK);
	}
}

LRESULT CMainFrame::OnKeyReturn(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(wParam == VK_RETURN)  //按下回车键
	{
		CRichEditUI* pInputEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"input_edit"));
		if(pInputEdit && pInputEdit->IsFocused())
			OnSendMessage();	
	}
	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnOwnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CBubbleChat* pBubbleList = static_cast<CBubbleChat*>(m_PaintManager.FindControl(L"Bubble_Chat"));
	if(wParam == WM_MYMSG_01 && pBubbleList)
	{
		SIZE sz = pBubbleList->GetScrollPos();
		int cy = pBubbleList->GetScrollRange().cy;
		if(sz.cy == cy)
			KillTimer(m_hWnd,WM_MYMSG_01);
		else
		{
			sz.cy = cy;
			pBubbleList->SetScrollPos(sz);
		}
	}
	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)
	{
		int a = 1;
// 		CButtonUI* pControl = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("setting_quit")));
 		CDuiString *strMenuName = (CDuiString*)wParam;
// 		BOOL bChecked = (BOOL)lParam;	
 		if (*strMenuName == _T("quit")) 
 		{
 			Close();
		}
 		delete strMenuName;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CMainFrame::On_TroyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(lParam)
	{
	case WM_RBUTTONDOWN:
		{
			CMenuWnd* pMenu = new CMenuWnd();   //不清楚 什么时候 释放内存的
			tagPOINT point;
			GetCursorPos(&point);
			point.y = point.y - 165;
			pMenu->Init(NULL, _T("menu_troy.xml"), point, &m_PaintManager,NULL);
			break;
		}
	case WM_LBUTTONDOWN:
		{
			ShowWindow(SW_SHOWNORMAL);
			break;
		}
	}
	bHandled = FALSE;
	return 0;
}

void CMainFrame::AddMegTip(int num)
{
	if(pButton_MsgTip)
	{
		pButton_MsgTip->SetVisible();
		Msgcount += num;
		TCHAR buffer[10];
		swprintf(buffer,L"%d",Msgcount);
		pButton_MsgTip->SetText(buffer);
	}
}

void CMainFrame::AddLastMsg(LPCTSTR buffer)
{

}

void CMainFrame::AddCachMsg(Node* root, int id)
{
	MsgData* tmpBuffer = m_history->GetLastHistory(id);
	if(tmpBuffer)
	{
		//添加最近消息
		CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(root->data().list_elment_, L"Buffer_Text"));
		if(pLabel && tmpBuffer->buffer)
			pLabel->SetText(tmpBuffer->buffer);
		//添加时间信息
		SYSTEMTIME sys; 
		GetLocalTime(&sys);
		TCHAR curTime[50];
		swprintf(curTime,L"%02d-%02d-%02d-%02d:%02d",sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute);
		int nLenPre = wcslen(tmpBuffer->time);
		int nLenCur = wcslen(curTime);
		CLabelUI* pLabel_Time = static_cast<CLabelUI*>(m_PaintManager.FindSubControlByName(root->data().list_elment_, L"time"));
		if(pLabel_Time == NULL) 
			return;
		int i,j;
		for (i=0,j=0; i<nLenPre && j<nLenCur; ++i,++j)
		{
			if(tmpBuffer->time[i] != curTime[j])
			{
				if(i<10)
				{
					if((tmpBuffer->time[8] == curTime[8] && tmpBuffer->time[9]+1 == curTime[9])
						|| (tmpBuffer->time[8]=='3' && curTime[8] == '0' && tmpBuffer->time[9] == curTime[9]))
					{
						pLabel_Time->SetText(L"昨天");
					}
					else
					{
						tmpBuffer->time[10] = L'\0';
						TCHAR buf[10] = {0};
						wcscpy(buf,tmpBuffer->time+2);
						pLabel_Time->SetText(buf);
					}
				}
				break;
			}
		}
		if(i == nLenPre || i>=10)
		{
			//同一天
			TCHAR buf[10] = {0};
			wcscpy(buf,tmpBuffer->time+11);
			pLabel_Time->SetText(buf);
		}
	}
}
