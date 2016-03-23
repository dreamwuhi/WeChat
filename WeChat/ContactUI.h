#pragma once
#include "Node.h"

namespace DuiLib
{
struct ContactListItemInfo			//好友列表信息 结构体
{
	bool				folder;					//是否为根目录
	//bool empty;							//是否空
	NodeType		id;	
	CDuiString	logo;						//图标资源
	CDuiString	nick_name;			//昵称资源
	CDuiString	description;			//个性签名资源
	CDuiString	back_name;			//备注名资源
	CDuiString	weixin_id;				//微信号
	CDuiString	zone;					//地区
	UINT				group_id;				//所属群ID  
};

class CContactUI : public CListUI
{
public:
	CContactUI(CPaintManagerUI& paint_manager);
	~CContactUI(void);
public:
public:
	enum {SCROLL_TIMERID = 10};
	Node* AddNode(const ContactListItemInfo& item, Node* parent);
	void DoEvent(TEventUI& event);
	void RemoveAll();
	bool RemoveNode(Node* node);			//删除节点
private:
	CDialogBuilder				m_dlgBuilder;
	CPaintManagerUI&		paint_manager_;
	LONG							delay_deltaY_;			//与滚动条滑动速度有关
	DWORD						delay_number_;		
	DWORD						delay_left_;
	Node*							root_node_;				//父节点
};
}
