#pragma once
#include "Node.h"

namespace DuiLib
{
struct FriendListItemInfo			//好友列表的信息 结构体
{
	bool folder;	
	bool empty;		//是否空
	NodeType id;	
	CDuiString logo;		//图标资源
	CDuiString nick_name;	//昵称资源
	int			weixing_id;
	//CDuiString description;	//附加描述资源
};

class CFriendListUI : public CListUI
{
public:
	CFriendListUI(CPaintManagerUI& paint_manager);
	~CFriendListUI(void);
public:
	enum {SCROLL_TIMERID = 10};
	Node* AddNode(const FriendListItemInfo& item, Node* parent,int insertIndex=-1);
	void DoEvent(TEventUI& event);
	void RemoveAll();
	bool RemoveNode(Node* node);			//删除节点
	//bool SelectItem(int iIndex, bool bTakeFocus);
	Node* GetRoot();
private:
	CDialogBuilder m_dlgBuilder;
	CPaintManagerUI& paint_manager_;
	LONG	delay_deltaY_;			//与滚动条滑动速度有关
	DWORD	delay_number_;		
	DWORD	delay_left_;
	Node*	root_node_;				//父节点
};
}