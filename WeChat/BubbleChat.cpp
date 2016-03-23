#include "StdAfx.h"
#include "BubbleChat.h"

static int GetTextWidth(const TCHAR *text);
CBubbleChat::CBubbleChat(CPaintManagerUI& paint_manager) : paint_manager_(paint_manager), delay_deltaY_(0), delay_number_(0), delay_left_(0)
{
	root_node_ = new Node;
	root_node_->data().list_elment_ = NULL;
}


CBubbleChat::~CBubbleChat(void)
{
	if (root_node_)
		delete root_node_;
	root_node_ = NULL;
}

Node* DuiLib::CBubbleChat::AddNode(const BubbleItemInfo& item, Node* parent)
{
	if(parent == NULL)
		parent = root_node_;

	CListContainerElementUI* pListElement = NULL;
	if( !m_dlgBuilder.GetMarkup()->IsValid() )
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(L"bubble_edit.xml", (UINT)0, NULL, &paint_manager_));
	else
	{
		pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
	}
	if (pListElement == NULL)
		return NULL;

	Node* node = new Node;
	node->data().list_elment_ = pListElement;		//关联列表控件
	//node->data().text_ = item.text;

	CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, L"chat_logo"));//找图标按钮
	if(log_button != NULL)
	{
		log_button->SetNormalImage(L"c2.png");
		log_button->SetFixedHeight(34);
		log_button->SetFixedWidth(34);
	}

	RECT rect={0,10,0,10};
	pListElement->SetInset(rect);

	CRichEditUI* log_text = static_cast<CRichEditUI*>(paint_manager_.FindSubControlByName(pListElement, L"chat_text"));
	if(log_text !=NULL)
	{
		int cx = GetTextWidth(item.buf);
		int off = cx / 337;
		if(off >=1) 
		{
			log_text->SetFixedWidth(336);
			log_text->SetFixedHeight(34*(off+1)-off*10);
			pListElement->SetFixedHeight(log_text->GetFixedHeight()+20);
			log_text->SetNormalImage(L"file='RichEdit_normal.png' corner='10,22,5,5'");
		}
		else
		{
			log_text->SetFixedWidth(cx+15+5+5);
			log_text->SetFixedHeight(34);
			pListElement->SetFixedHeight(log_text->GetFixedHeight()+20);
			log_text->SetNormalImage(L"file='RichEdit_normal.png' corner='10,5,5,5'");
		}
		log_text->SetText(item.buf);
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

static int GetTextWidth(const TCHAR *text)
{
	double nLen = 0;
	int length = wcslen(text);
	for (int i=0; i<length; ++i)
	{
		if((text[i] >='0' && text[i] <='9') || (text[i] >='a' && text[i] <= 'z'))
		{
			nLen += 8;
		}
		else if(text[i] >='A' && text[i] <= 'Z')
		{
			nLen += 8.5; 
		}
		else 
		{
			nLen += 13;
		}
	}
	return ceil(nLen);
}