#include "StdAfx.h"
#include "MsgList.h"


CMsgList::CMsgList(CPaintManagerUI& paint_manager)  : paint_manager_(paint_manager)
{
	root_node_ = new Node;
	root_node_->data().list_elment_ = NULL;
}


CMsgList::~CMsgList(void)
{
	RemoveAll();
	if (root_node_)
		delete root_node_;
	root_node_ = NULL;
}

void DuiLib::CMsgList::RemoveAll()
{
	CListUI::RemoveAll();
	for (int i=0; i<root_node_->num_children();)
	{
		Node* child = root_node_->child(i);
		RemoveNode(child);
	}
	delete root_node_;

	root_node_ = new Node;
	root_node_->data().list_elment_ = NULL;
}

bool DuiLib::CMsgList::RemoveNode(Node* node)
{
	if(!node || node == root_node_)
		return false;
	for (int i=0; i<node->num_children(); ++i)
	{
		Node* child = node->child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->data().list_elment_);
	if(node->data().list_elment_)
	{
		delete node->data().list_elment_;
		node->data().list_elment_ = NULL;
	}
	node->parent()->remove_child(node);
	delete node;
	node = NULL;
	return true;
}

Node* DuiLib::CMsgList::AddNode(const TroyMsgData& item, Node* parent)
{
	if(parent == NULL)
		parent = root_node_;

	CListContainerElementUI* pListElement = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() )
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(L"MsgList.xml", (UINT)0, NULL, &paint_manager_));
	else
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return NULL;

	Node* node = new Node;
	node->data().list_elment_ = pListElement;		//关联列表控件
	node->data().weixing_id = item.weixing_id;

	CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, L"logo"));//找图标按钮
	if(log_button != NULL)
	{
		log_button->SetNormalImage(item.LogoPath);
	}
	CLabelUI* name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, L"nick_name"));
	if(name !=NULL)
	{
		name->SetText(item.name);
	}
	int index = 0;
	if(parent->has_children())
	{
		Node* prev = parent->get_last_child();
		index = prev->data().list_elment_->GetIndex()+1;
	}
	else
	{
		if(parent == root_node_)
			index = 0;
		else
			index = parent->data().list_elment_->GetIndex()+1;
	}

	pListElement->SetTag((UINT_PTR)node);

	bool ret = CListUI::AddAt(pListElement, index);
	if(ret == false)
	{
		delete pListElement;
		delete node;
		node = NULL;
		return NULL;
	}
	parent->add_child(node);
// 	delete node;
// 	node = NULL;
//	delete pListElement;
//	pListElement = NULL;
	return node;
}
