#include "StdAfx.h"
#include "History.h"


CHistory::CHistory(void)
{
}


CHistory::~CHistory(void)
{
	FreeMemory();
}

void CHistory::SaveHistory(Node* node,TCHAR* Buffer)
{
	if(Buffer)
	{
		Log *stLog = new Log();
		stLog->szBuffer = new TCHAR[wcslen(Buffer)+1];
		if(stLog->szBuffer != NULL)
			wcscpy(stLog->szBuffer,Buffer);
		stLog->node = node;
		LogHis.push_back(stLog);
	}
}

void CHistory::FreeMemory()
{
	for (UINT i=0; i<LogHis.size(); ++i)
	{
		if(LogHis[i] != NULL)
		{
			delete [] LogHis[i]->szBuffer;
			LogHis[i]->szBuffer = NULL;
			LogHis[i]->node = NULL;
		}
	}
	LogHis.clear();
}

void CHistory::UpdateHis(Node* node,CBubbleChat* pBubbleList)
{
	for(int i=0; i<LogHis.size(); ++i)
	{
		if(node == LogHis[i]->node)
		{
			BubbleItemInfo item;
			memset(&item,0,sizeof(BubbleItemInfo));
			wcscpy(item.buf,LogHis[i]->szBuffer);
			pBubbleList->AddNode(item,NULL);
		}
	}
}

void CHistory::DeleteMsg()
{
	if(LogHis.size() > 0)
	{
// 		delete [] LogHis[0];
// 		LogHis[0] = NULL;
// 		LogHis.erase(LogHis.begin());
	}
}
