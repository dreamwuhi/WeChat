#include "StdAfx.h"
#include "Node.h"

namespace DuiLib
{
	double CalculateDelay(double state)
	{
		return pow(state, 2);
	}

	Node::Node(void) : parent_(NULL)
	{
	}


	Node::~Node(void)
	{
		for (int i = 0; i < num_children(); ++i)
			delete children_[i]; 
	}

	void Node::set_parent(Node* parent)  //设置父节点
	{
		parent_ = parent;
	}

	Node::Node(NodeData t) : data_(t), parent_(NULL)		//带有节点信息的构造函数
	{

	}

	Node::Node(NodeData t, Node* parent): data_ (t), parent_ (parent)	//带有节点信息和父节点的构造函数
	{

	}

	NodeData& Node::data()		//返回 自身节点信息的结构体
	{
		return data_;
	}

	int Node::num_children() const			//孩子数量
	{
		return static_cast<int>(children_.size());
	}

	Node* Node::child(int i)			//第i个孩子
	{
		return children_[i];
	}

	Node* Node::parent()			//父节点
	{
		return ( parent_);
	}

	bool Node::has_children() const		//是否有孩子节点
	{
		return num_children() > 0;
	}

	bool Node::folder() const				//是否folder
	{
		return data_.folder_;
	}

	void Node::add_child(Node* child)		//添加孩子
	{
		child->set_parent(this); 
		children_.push_back(child); 
	}

	void Node::remove_child(Node* child)			//删除孩子
	{
		Children::iterator iter = children_.begin();
		for (; iter < children_.end(); ++iter)
		{
			
			if (*iter == child) 
			{
				children_.erase(iter);
				return;
			}
		}
	}

	Node* Node::get_last_child()			//获取最后一个孩子
	{
		if (has_children())
		{
			return child(num_children() - 1)->get_last_child();
		}
		return this;
	}
}