#include "stdafx.h"
#include "SUserContainer.h"
#include "ssocket.h"
#include "IOCP.h"
#include "Log.h"

UserContainer* UserContainer::m_instance = NULL;

UserContainer* UserContainer::GetInstance()
{
	if (m_instance == NULL)
		m_instance = new UserContainer();

	return m_instance;
}

UserContainer::UserContainer()
{
	SPeer* pUser;
	for (int i = 0; i < MAX_USER_COUNT; i++)
	{
		pUser = &m_user[i];
		//		m_vecEmptyUser[i] = pUser;
		m_vecCon.push_back(pUser);
	}
}

UserContainer::~UserContainer()
{

}

void UserContainer::DisConnect(SOCKET_CONTEXT * pUser)
{
	if (pUser == NULL)
	{
#ifdef _DEBUG
		OutputDebugStringA("[ERROR] NULL인 유저를 지운다. [UserContainer::DisConnect]\n");
#endif
		return;
	}

#ifdef _DEBUG
	OutputDebugStringA("[ERROR] NULL인 유저를 지운다. [UserContainer::DisConnect]\n");
#endif

	CSLOCK(m_cs)
	{
		//현재 연결된 유저 목록에서 지우고...
		MAP_CONTANINER::iterator it = m_mapCon.find(pUser->m_socket);

		if (it == m_mapCon.end())
			return;

		m_mapCon.erase(it);
	}
	//소켓 강제 종료
	///다음의 기능은 http://egloos.zum.com/mirine35/v/5057014 참조
	///우아한 종료로 TIMEOUT이 발생할 수 있음. 시나리오는 위를 참조.
	///이 TIMEOUT이 무한 발생(실제로는 240초까지만)하면, 소켓을 사용할 수 없다.
	///이를 위해 closesocket이 대기하는 시간을 조절

	LINGER LingerStruct;
	LingerStruct.l_onoff = 1;
	LingerStruct.l_linger = 0;

	if (setsockopt(pUser->m_socket, SOL_SOCKET, SO_LINGER, (char*)&LingerStruct, sizeof(LingerStruct) == SOCKET_ERROR))
	{
		//return;
		ERROR_LOG("Set Socket LINGER Error ");
	}

	//closesocket((SOCKET)*pUser);
	//(SOCKET)*pUser = INVALID_SOCKET;
	pUser->m_puser->ReleaseSocket();
}
