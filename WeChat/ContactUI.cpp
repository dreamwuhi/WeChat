#include "StdAfx.h"
#include "ContactUI.h"


CContactUI::CContactUI(CPaintManagerUI& paint_manager) : paint_manager_(paint_manager)	, delay_deltaY_(0), delay_number_(0), delay_left_(0)
{
	root_node_ = new Node;
	//root_node_->data().level_ = -1;
	//root_node_->data().child_visible_ = true;
	//root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
}


CContactUI::~CContactUI(void)
{
	RemoveAll();
	if (root_node_)
		delete root_node_;
	root_node_ = NULL;
}

Node* CContactUI::AddNode(const ContactListItemInfo& item, Node* parent)
{
	if(parent == NULL)
		parent = root_node_;

	CListContainerElementUI* pListElement = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() )
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(L"contact_list.xml", (UINT)0, NULL, &paint_manager_));
	else
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return NULL;

	Node* node = new Node;
	node->data().list_elment_ = pListElement;		//关联列表控件
	node->data().folder_ = item.folder;				//标记是否为根节点
	node->data().text_ = item.nick_name;
	node->data().type_ = item.id;
	node->data().group_id = item.group_id;
	if(item.folder == false)
	{
		CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, L"logo_container"));
		if(logo_container != NULL)
			logo_container->SetVisible(true);
		CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, L"logo"));//找图标按钮
		if(log_button != NULL)
		{
			if(!item.logo.IsEmpty())
			{
				TCHAR buf[MAX_PATH];
				memset(buf,0,MAX_PATH);
				wsprintf(buf,L"%s",item.logo);
				log_button->SetNormalImage(buf);

				//log_button->SetTag((UINT_PTR)pListElement);
				//log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent);
			}
		}
	}
	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, L"nickname"));
	if(nick_name != NULL)
	{
		TCHAR buf[MAX_PATH];
		memset(buf,0,MAX_PATH);
		wsprintf(buf,L"%s",item.nick_name);
		nick_name->SetText(buf);
		RECT rc = {0,0,0,0};
		if(item.folder)
		{
			rc.left = 10;
			rc.top = rc.right = rc.bottom = 0;
		}
		else
		{
			rc.top = 15;
		}
		nick_name->SetPadding(rc);
	}

	pListElement->SetTag((UINT_PTR)node);
	if(item.folder)
	{
		pListElement->SetFixedHeight(20);
		pListElement->SetMouseEnabled(false);
		pListElement->SetBkColor(4281611316);//设置联系人 根目录的背景
	}
	else
	{ 
		pListElement->SetFixedHeight(50);
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
	bool ret = CListUI::AddAt(pListElement, index);
	if(ret == false)
	{
		delete pListElement;
		delete node;
		node = NULL;
	}
	parent->add_child(node);
	return node;
}

void CContactUI::DoEvent(TEventUI& event) 
{
	// 	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	// 	{
	// 		if (m_pParent != NULL)
	// 			m_pParent->DoEvent(event);
	// 		else
	// 			CVerticalLayoutUI::DoEvent(event);
	// 		return;
	// 	}
	if( event.Type == UIEVENT_MOUSEENTER )
	{
		int a = 1;
	}
	if (event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID)
	{
		if (delay_left_ > 0)
		{
			--delay_left_;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
			if ((lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ))
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		delay_deltaY_ = 0;
		delay_number_ = 0;
		delay_left_ = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (delay_number_ > 0)
			lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (delay_deltaY_ >= 0)
				delay_deltaY_ = lDeltaY + 7;
			else
				delay_deltaY_ = lDeltaY + 11;
			break;
		case SB_LINEDOWN:
			if (delay_deltaY_ <= 0)
				delay_deltaY_ = lDeltaY - 7;
			else
				delay_deltaY_ = lDeltaY - 11 ;
			break;
		}
		if
			(delay_deltaY_ > 100) delay_deltaY_ = 100;
		else if
			(delay_deltaY_ < -100) delay_deltaY_ = -100;

		delay_number_ = (DWORD)sqrt((double)abs(delay_deltaY_)) * 5;
		delay_left_ = delay_number_;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 20U);
		return;
	}

	CListUI::DoEvent(event);
}

void CContactUI::RemoveAll()
{
	CListUI::RemoveAll();
	for (int i=0; i<root_node_->num_children(); ++i)
	{
		Node* child = root_node_->child(i);
		RemoveNode(child);
	}
	delete root_node_;

	root_node_ = new Node;
	//root_node_->data().level_ = -1;
	//root_node_->data().child_visible_ = true;
	//root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
}

bool CContactUI::RemoveNode(Node* node)
{
	if(!node || node == root_node_)
		return false;
	for (int i=0; i<node->num_children(); ++i)
	{
		Node* child = node->child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->data().list_elment_);
	node->parent()->remove_child(node);
	delete node;
	node = NULL;
	return true;
}

