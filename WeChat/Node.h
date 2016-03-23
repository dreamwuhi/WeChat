#pragma once
#include <math.h>

namespace DuiLib
{
	enum NodeType
	{
		FRIENDS = 1000,
		PUBLIC_NO,
		GROUP_CHAT,
		LAST_CONTACT,
		OTHERS
	};

	struct NodeData
	{
		//int	level_;
		bool folder_;
		//bool child_visible_;
		//bool has_child_;
		CDuiString text_;
		//CDuiString value;
		NodeType  type_;
		CListContainerElementUI* list_elment_;
		UINT group_id;
	};

	double CalculateDelay(double state);

	class Node
	{
	public:
		Node(void);
		~Node(void);
	public:
		explicit Node(NodeData t);
		Node(NodeData t, Node* parent);
		NodeData& data();
		int num_children() const;
		Node* child(int i);
		Node* parent();
		bool folder() const;
		bool has_children() const;
		void add_child(Node* child);
		void remove_child(Node* child);
		Node* get_last_child();
	private:
		void set_parent(Node* parent);
	private:
		typedef std::vector <Node*> Children;

		Children	children_;  //孩子节点集
		Node*		parent_;	//父节点

		NodeData    data_;	//自己节点的信息
	};
}

