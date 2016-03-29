#include "StdAfx.h"
#include "History.h"
#include <fstream>

CHistory::CHistory(void)
{
}


CHistory::~CHistory(void)
{
	FreeMemory(LocalLoghis);
	FreeMemory(NewLoghis);
}


void CHistory::AddHistory(int id,TCHAR* Buffer,TCHAR* time, int flag)
{
	std::map<int, std::vector<MsgData*> > ::iterator itBeg;
	std::map<int, std::vector<MsgData*> > ::iterator itEnd;
	if(flag)
	{
		itBeg = NewLoghis.begin();
		itEnd = NewLoghis.end();
	}
	else
	{
		itBeg = LocalLoghis.begin();
		itEnd = LocalLoghis.end();
	}

	if(Buffer)
	{
		while(itBeg != itEnd)
		{
			if(id == itBeg->first)
			{
				MsgData *stBuffer = new MsgData(); 
				stBuffer->buffer = new TCHAR[wcslen(Buffer)+1];
				if(stBuffer->buffer != NULL)
					wcscpy(stBuffer->buffer,Buffer);
				wcscpy(stBuffer->time,time);
				itBeg->second.push_back(stBuffer);
				//新的消息记录复制到LocalLoghis
				if(flag)
					AddHistory(id,Buffer,time,0);
				break;
			}
			itBeg++;
		}
		if(itBeg == itEnd)
		{
			std::vector<MsgData*>tmp;
			//TCHAR* szBuffer = new TCHAR[wcslen(Buffer)+1];
			MsgData *stBuffer = new MsgData(); 
			stBuffer->buffer = new TCHAR[wcslen(Buffer)+1];
			wcscpy(stBuffer->time,time);
			//swprintf(stBuffer.time,L"%04d:%02d:%02d",time.wYear,time.wHour,time.wMinute);
			if(stBuffer->buffer != NULL)
				wcscpy(stBuffer->buffer,Buffer);
			tmp.push_back(stBuffer);
			if(flag)
			{
				NewLoghis.insert(make_pair(id,tmp));
				//新的消息记录复制到LocalLoghis
				AddHistory(id,Buffer,time,0);
			}
			else
			{
				LocalLoghis.insert(make_pair(id,tmp));
			}
		}
	}
}

void CHistory::FreeMemory(std::map<int, std::vector<MsgData*> >& loghis)
{
	std::map<int, std::vector<MsgData*> > ::iterator itBeg = loghis.begin();
	std::map<int, std::vector<MsgData*> > ::iterator itEnd = loghis.end();

	while(itBeg != itEnd)
	{
		for (UINT i=0; i<itBeg->second.size(); ++i)
		{
			if(itBeg->second[i] != NULL)
			{
				delete [] itBeg->second[i]->buffer;
				itBeg->second[i]->buffer = NULL;
			}
			delete itBeg->second[i];
			itBeg->second[i] = NULL;
		}
		itBeg->second.clear();
		itBeg++;
	}
	loghis.clear();
}

void CHistory::LoadHistory(int id, CBubbleChat* pBubbleList)
{
	std::map<int, std::vector<MsgData*> > ::iterator itBeg = LocalLoghis.begin();
	while(itBeg != LocalLoghis.end())
	{
		if(itBeg->first == id)
		{
			for (UINT i=0; i<itBeg->second.size(); ++i)
			{
				BubbleItemInfo item;
				memset(&item,0,sizeof(BubbleItemInfo));
				wcscpy(item.buf,itBeg->second[i]->buffer);
				pBubbleList->AddNode(item,NULL);
			}
			break;
		}
		itBeg++;
	}
}

void CHistory::DeleteMsg()
{

}

void CHistory::SaveHisToLocal(const char* path)
{
	std::map<int, std::vector<MsgData*> > ::iterator itBeg = NewLoghis.begin();
	std::ofstream file;
	file.open(path,std::ios::app);
	if(file == NULL)
		return;
	while(itBeg != NewLoghis.end())
	{
		for (UINT i=0; i<itBeg->second.size(); ++i)
		{
			int iLength = WideCharToMultiByte(CP_ACP, 0, itBeg->second[i]->buffer, -1, NULL, 0, NULL, NULL);  
			char * buffer = new char[iLength];
			WideCharToMultiByte(CP_ACP, 0, itBeg->second[i]->buffer, -1, buffer, iLength, NULL, NULL);
			char time_buffer[50];
			WideCharToMultiByte(CP_ACP, 0, itBeg->second[i]->time, -1, time_buffer, 50, NULL, NULL);
			file<<itBeg->first<<"\t"<<buffer<<"\t"<<time_buffer<<std::endl;
			delete []buffer;
			buffer = NULL;
		}
		itBeg++;
	}
	file.close();
}

void CHistory::LoadHistoryFromLocal(const char* path)
{
	std::ifstream file;
	file.open(path,std::ios::in);
	if(file == NULL)
		return;
	string line, id, buffer,time;
	while (getline(file, line)) 
	{
		std::stringstream liness(line);
		getline(liness, id,'\t');
		getline(liness, buffer,'\t');
		getline(liness, time);
		int iLength = MultiByteToWideChar(CP_ACP, 0, buffer.c_str(), strlen(buffer.c_str())+1, NULL, 0);  
		TCHAR* Tbuffer = new TCHAR[iLength];
		MultiByteToWideChar(CP_ACP, 0, buffer.c_str(), strlen(buffer.c_str())+1,Tbuffer,iLength);   

		iLength = MultiByteToWideChar(CP_ACP, 0, time.c_str(), strlen(time.c_str())+1, NULL, 0); 
		TCHAR* time_buffer = new TCHAR[iLength];
		MultiByteToWideChar(CP_ACP, 0, time.c_str(), strlen(time.c_str())+1,time_buffer,iLength);  

		AddHistory(atoi(id.c_str()),Tbuffer,time_buffer,0);
		delete []Tbuffer;
		delete []time_buffer;
		Tbuffer = NULL;
		time_buffer = NULL;
	}
	file.close();
}

MsgData* CHistory::GetLastHistory(int id)
{
	std::map<int, std::vector<MsgData*> > ::iterator itBeg = LocalLoghis.begin();
	while(itBeg != LocalLoghis.end())
	{
		if(itBeg->first == id)
		{
			return itBeg->second.back();
		}
		itBeg++;
	}
	return NULL;
}
