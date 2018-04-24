#include "stdafx.h"
#include "GameMessage.h"
#include "SIOCP.h"

template<> GameMessageManager* SSingleton<GameMessageManager>::ms_singleton = 0;


GameMessageManager::GameMessageManager()
{
	m_event = CreateEvent(NULL, TRUE, FALSE, NULL);
}

GameMessageManager::~GameMessageManager()
{
	CloseHandle(m_event); m_event = NULL;

	DestoryMsg();
}

void GameMessageManager::DestoryMsg()
{
	GameMSG* msg = NULL;
	for (unsigned int i = 0; i < m_iocpmsg.size(); i++)
	{
		msg = m_iocpmsg.front();

		if (msg != NULL)
		{
			m_iocpmsg.pop_front();
			delete msg;
		}
	}

	for (unsigned int i = 0; i < m_msg.size(); i++)
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
		WaitForSingleObject(m_event, 100); // 1/1000초만큼 대기
}

void GameMessageManager::SetEvent()
{
	if (m_event != NULL)
		::SetEvent(m_event); // signal 상태로 변경
}

void GameMessageManager::ResetEvent()
{
	if (m_event != NULL)
		::ResetEvent(m_event); // non-signal 상태로 변경
}

void GameMessageManager::PushIocpMsg(GameMSG * msg)
{
	if (msg == NULL)
		return;

	CSLOCK(m_cs)
	{
//		this->m_
		this->m_iocpmsg.push_back(msg);
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
		if ((msg = GameMessageManager::GetInstance()->GetGameMessage()) != NULL)
		{

			if (msg->msg == GM_QUIT)
			{
				GameMessageManager::GetInstance()->PushIocpMsg(msg);
				break;
			}

			GameMessageProcedure(msg->msg, msg->wParam, msg->lParam, msg->packet);

			GameMessageManager::GetInstance()->PushIocpMsg(msg);
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
