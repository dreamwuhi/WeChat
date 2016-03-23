#pragma once
#include "FriendListUI.h"
#include "ContactUI.h"
#include "BubbleChat.h"
#include "DuiTrayIcon.h"

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
private:
	std::vector<FriendListItemInfo> friends_;
	std::vector<ContactListItemInfo> contact_;
	CDuiTrayIcon* icon;   //Õ–≈Ã≤Àµ•
};

