#pragma once
#include <set>
#include "FriendListUI.h"
#include "ContactUI.h"
#include "BubbleChat.h"
#include "DuiTrayIcon.h"
#include "History.h"
#include "MessageWnd.h"
#include "SettingFrame.h"

class CMainFrame : public WindowImplBase
{
public:
	CMainFrame(void);
	~CMainFrame(void);
public:
public:
	LPCTSTR    GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	void OnPrepare();
	virtual void Notify( TNotifyUI& msg );
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyReturn(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOwnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT On_TroyIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void UpgrateFriends();
	void UpgrateContacts();
	ContactListItemInfo* FindContactItem(CDuiString &name);
	std::vector<ContactListItemInfo*> FindContactItemBygroupID(UINT id);
	void OnSelectFriendList(TNotifyUI& msg, CFriendListUI* pFriendsList);
	void OnSelectContactList(TNotifyUI& msg, CContactUI* pFriendsList);
	void OnSendMessage();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void AddMegTip(Node* ClickNode);
	void AddLastMsg(LPCTSTR buffer);
	void AddCachMsg(Node* root, int id);
	void OnSetting(bool flag=0);
	Node* FindClickNodeByID(const int id);
	void CloseALLWindows();
	void OnBegChat(TNotifyUI& msg);
	void AddNodeToFriendList();
private:
	std::vector<FriendListItemInfo> friends_;		//保存好友信息列表
	std::vector<ContactListItemInfo> contact_;	//保存通讯录信息列表
	std::vector<Node*> friendsNode;					//已添加的最近联系人节点
	
	bool b_msg;													//是否有消息
	int Msgcount;												//消息总条数

	CHistory*	m_history;										//聊天记录

	CDuiTrayIcon* icon;										//托盘菜单
	bool isShow;													//托盘窗口是否创建
	CMessageWnd *pMsgWnd;							//托盘消息窗口

	CSettingFrame *settingFrame;						//设置窗口 

	Node* selectItemNode_Fri;							//当前选中的好友列表节点
	Node* selectItemNode_Con;							//当前选中的联系人列表节点
	int weixing_id;												//当前选中的节点的微信号

	CButtonUI*						pButton_MsgTip;						//消息条数控件
	CTabLayoutUI*					pTabLayout;							
	CFriendListUI*					pFriendsList;
	CContactUI*						pContactsList;			
	CTileLayoutUI*					m_group_list;					
	CBubbleChat*					pBubbleList;							//显示聊天记录的编辑框（列表）
	CRichEditUI*						pInputEdit;								//聊天窗口的输入编辑框
	CButtonUI*						pButtonSet;								//主界面的设置按钮
	COptionUI*						pOptionChat;
	CTabLayoutUI*					pTabs;
	CLabelUI*							pChatName;	
};

