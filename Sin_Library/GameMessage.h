#pragma once
#include <list>
#include "CriticalSection.h"
#include "header.h"

#define MAX_GAME_MSG_SIZE 1024

struct GameMSG
{
	DWORD msg;
	ULONG64 wParam;
	ULONG64 lParam;
	char packet[MAX_GAME_MSG_SIZE];
};

typedef std::list<GameMSG*> LIST_MSG;

class GameMessageManager
{
public:
	GameMessageManager();
	~GameMessageManager();

	void SendGameMessage(DWORD msg, ULONG64 wParam, ULONG64 lParam, char* packet);

	static GameMessageManager* Instnace();

	GameMSG* GetGameMessage() { return PopMsg(); }
	void PushIocpMsg(GameMSG* msg);
	void PushMsg(GameMSG* msg);

	static DWORD WINAPI GameMsgLoop(LPVOID pArg);

private:

	void WaitEvent();
	void SetEvent();
	void ResetEvent();

	//void PushIocpMsg(GameMSG* msg);
	GameMSG* PopIocpMsg();
	GameMSG* PopMsg();


	void DestoryMsg();
private:

	LIST_MSG m_iocpmsg; // IOCP ������鿡�� ������ �޽��� ����Ʈ ���� �� �޼����� ������.
	LIST_MSG m_msg; // ���� �����忡�� ó���ϱ� ���� �޽��� ����Ʈ
	SCriticalSection m_cs;
	HANDLE m_event; // ������ ��� �� 

private:
	static GameMessageManager* m_instance;
};

//------------------------------------------------------------//

extern bool InitDataBase();
extern void GameMessageProcedure(DWORD msg, DWORD wParam, DWORD lParam, const char *pPacket);

