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
		OutputDebugStringA("[ERROR] NULL�� ������ �����. [UserContainer::DisConnect]\n");
#endif
		return;
	}

#ifdef _DEBUG
	OutputDebugStringA("[ERROR] NULL�� ������ �����. [UserContainer::DisConnect]\n");
#endif

	CSLOCK(m_cs)
	{
		//���� ����� ���� ��Ͽ��� �����...
		MAP_CONTANINER::iterator it = m_mapCon.find(pUser->m_socket);

		if (it == m_mapCon.end())
			return;

		m_mapCon.erase(it);
	}
	//���� ���� ����
	///������ ����� http://egloos.zum.com/mirine35/v/5057014 ����
	///����� ����� TIMEOUT�� �߻��� �� ����. �ó������� ���� ����.
	///�� TIMEOUT�� ���� �߻�(�����δ� 240�ʱ�����)�ϸ�, ������ ����� �� ����.
	///�̸� ���� closesocket�� ����ϴ� �ð��� ����

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
