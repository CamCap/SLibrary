#include "stdafx.h"
#include "GameMessage.h"
#include "IOCP.h"


GameMessageManager* GameMessageManager::m_instance = NULL;

GameMessageManager::GameMessageManager()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}

GameMessageManager::~GameMessageManager()
{
	CloseHandle(m_event); m_event = NULL;

	DestoryMsg();
}

GameMessageManager * GameMessageManager::Instnace()
{
	if (m_instance == NULL)
		m_instance = new GameMessageManager();

	return m_instance;
}

void GameMessageManager::DestoryMsg()
{
	GameMSG* msg = NULL;
	for (int i = 0; i < m_iocpmsg.size(); i++)
	{
		msg = m_iocpmsg.front();

		if (msg != NULL)
		{
			m_iocpmsg.pop_front();
			delete msg;
		}
	}

	for (int i = 0; i < m_msg.size(); i++)
	{
		msg = m_msg.front();

		if (msg != NULL)
		{
			m_msg.pop_front();
			delete msg;
		}
	}
}


void GameMessageManager::SendGameMessage(DWORD msg, DWORD wParam, DWORD lParam, char * packet)
{
	GameMSG* pmsg = PopIocpMsg();

	pmsg->msg = msg;
	pmsg->wParam = wParam;
	pmsg->lParam = lParam;

	if (packet != NULL)
		memcpy(pmsg->packet, packet, wParam);

	PushMsg(pmsg);
}

void GameMessageManager::WaitEvent()
{
	if (m_event != NULL)
		WaitForSingleObject(m_event, 100); // 1/1000�ʸ�ŭ ���
}

void GameMessageManager::SetEvent()
{
	if (m_event != NULL)
		::SetEvent(m_event); // signal ���·� ����
}

void GameMessageManager::ResetEvent()
{
	if (m_event != NULL)
		::ResetEvent(m_event); // non-signal ���·� ����
}

void GameMessageManager::PushIocpMsg(GameMSG * msg)
{
	if (msg == NULL)
		return;

	CSLOCK(m_cs)
	{
		m_msg.push_back(msg);
	}
}

GameMSG * GameMessageManager::PopIocpMsg()
{
	GameMSG* msg = NULL;


	CSLOCK(m_cs)
	{
		if (m_iocpmsg.size() > 0)
		{
			msg = m_iocpmsg.front();
			if (msg != NULL)
				m_iocpmsg.pop_front();
			else
				msg = new GameMSG;
		}
		else
		{
			msg = new GameMSG;
		}
	}
	return msg;
}



void GameMessageManager::PushMsg(GameMSG * msg)
{
	if (msg == NULL) return;

	CSLOCK(m_cs)
	{
		m_msg.push_back(msg);
	}

	SetEvent();
}

GameMSG * GameMessageManager::PopMsg()
{
	GameMSG* msg = NULL;


	//	Lock();
	//	SCOPE_EXIT({ UnLock(); });

	while (1)
	{

		if (m_msg.size() > 0)
			break;
		else
		{
			ResetEvent();
			WaitEvent();
		}

	}

	msg = m_msg.front();
	if (msg != NULL)
		m_msg.pop_front();

	return msg;
}

DWORD WINAPI GameMessageManager::GameMsgLoop(LPVOID pArg)
{
	GameMSG* msg = NULL;

	while (true)
	{
		if ((msg = GameMessageManager::Instnace()->GetGameMessage()) != NULL)
		{

			if (msg->msg == GM_QUIT)
			{
				GameMessageManager::Instnace()->TempMsgPush(msg);
				break;
			}

			GameMessageProcedure(msg->msg, msg->wParam, msg->lParam, msg->packet);

			GameMessageManager::Instnace()->TempMsgPush(msg);
		}
	}

	ExitThread(0);

	return 0;
}

/////////////////////////////////////////////////////////////////////////

bool InitDataBase()
{
	return 0;
}

void GameMessageProcedure(DWORD msg, DWORD wParam, DWORD lParam, const char * pPacket)
{
	//BTZPacket* packet = (BTZPacket*)pPacket;

	switch (msg)
	{
	case GM_ACCEPTUSER:
		printf("Accept User \n");
		break;
	case GM_ERROR:
		printf("Error, Look Log! \n");
		break;
	case GM_DISCONNECTUSER:
		IOCP::GetInstance()->PostCompletionStatus(wParam, 0, (OVERLAPPED*)lParam);
		printf("DisConnect User \n");
		break;
	case GM_PKTRECEIVE:
		printf("Recv Packet \n");
		break;
	default:
		break;
	}
}

