#pragma once
#include <vector>
#include "BubbleChat.h"
#include <map>
#include <string>
#include <sstream>
using namespace std;

typedef struct MsgData 
{
	TCHAR* buffer;
	TCHAR time[50];
}MsgData;

class CHistory
{
public:
	CHistory(void);
	~CHistory(void);
public:
	void AddHistory(int id,TCHAR* Buffer,TCHAR* time, int flag=1); //默认从历史聊天对话框添加
	void FreeMemory(std::map<int, std::vector<MsgData*> >& loghis);
	void LoadHistory(int id ,CBubbleChat* pBubbleChat);
	void LoadHistoryFromLocal(const char* path);
	void DeleteMsg();
	void SaveHisToLocal(const char* path);
	MsgData* GetLastHistory(int id);
private:
	std::map<int , std::vector<MsgData*> > LocalLoghis;
	std::map<int , std::vector<MsgData*> > NewLoghis;
};

