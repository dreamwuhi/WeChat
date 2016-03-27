#pragma once
#include <vector>
#include "BubbleChat.h"

typedef struct Log
{
	Node* node;
	TCHAR* szBuffer;
}Log;

class CHistory
{
public:
	CHistory(void);
	~CHistory(void);
public:
	void SaveHistory(Node* node,TCHAR* Buffer);
	void FreeMemory();
	void UpdateHis(Node* node ,CBubbleChat* pBubbleChat);
	void DeleteMsg();
private:
	std::vector<Log*> LogHis;
};

