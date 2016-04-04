#pragma once
#include "Node.h"

namespace DuiLib
{
	typedef struct TroyMsgData
	{
		CDuiString	LogoPath;//头像资源路径
		CDuiString	name;		//名称
		int				weixing_id;
	}TroyMsgData;
	class CMsgList : public CListUI
	{
		public:
			CMsgList(CPaintManagerUI& paint_manager);
			~CMsgList(void);
		public:
			Node* AddNode(const TroyMsgData& item, Node* parent);
			void RemoveAll();
			bool RemoveNode(Node* node);			//删除节点
		private:
			CDialogBuilder				m_dlgBuilder;
			CPaintManagerUI&		paint_manager_;
			Node*							root_node_;				//父节点
	};
}
