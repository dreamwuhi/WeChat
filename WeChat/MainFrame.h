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
	void AddMegTip(int num);
	void AddLastMsg(LPCTSTR buffer);
	void AddCachMsg(Node* root, int id);
	void OnSetting(bool flag=0);
private:
	std::vector<FriendListItemInfo> friends_;
	std::vector<ContactListItemInfo> contact_;
	CDuiTrayIcon* icon;				//托盘菜单
	//std::set<Node*> selectNode;			//选中的列表项
	Node* selectItemNode;
	int weixing_id;
	int Msgcount;						//消息条数
	CButtonUI* pButton_MsgTip;			//显示消息条数控件
	CHistory*	m_history;
	bool b_msg;
	bool isShow;
	CMessageWnd *pMsgWnd; //托盘消息窗口
	CSettingFrame *settingFrame; // 设置窗口 
};

