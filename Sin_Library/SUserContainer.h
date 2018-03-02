#pragma once
#include "ssocket.h"
#include "IOCP.h"

#define MAX_USER_COUNT 500

typedef std::vector<SUser*> VECTOR_USER;
typedef std::map<int, SUser*> MAP_USER;

class UserContainer
{
public:
	static UserContainer* GetInstance();

public:
	//	BOOL ReigsterUser(SUser user)

	SUser* Pop_EmptyUser();
	void Push_EmptyUser(SUser* puser);

	void Add_CurUser(int userid, SUser* puser);
	void Remove_CurUser(SOCKET sock);

	void DisConnect(SOCKET_CONTEXT* pUser);
	//	SUser* FindUser()

public:
	UserContainer();
	~UserContainer();

private:

private:
	VECTOR_USER m_vecEmptyUser;
	MAP_USER m_mapConnectUser;

	SCriticalSection m_cs;

	SUser m_user[MAX_USER_COUNT];

private:
	static UserContainer* m_instance;
};

UserContainer* UserContainer::m_instance = NULL;

UserContainer* UserContainer::GetInstance()
{
	if (m_instance == NULL) m_instance = new UserContainer();

	return m_instance;
}

UserContainer::UserContainer()
	:m_cs()
{
	SUser* pUser;
	for (int i = 0; i < MAX_USER_COUNT; i++)
	{
		pUser = &m_user[i];
		//		m_vecEmptyUser[i] = pUser;
		m_vecEmptyUser.push_back(pUser);
	}

}


UserContainer::~UserContainer()
{
}


void UserContainer::Push_EmptyUser(SUser* puser)
{
	if (puser == NULL)
		return;

	CSLOCK(m_cs)
	{
		VECTOR_USER::iterator it = std::find(m_vecEmptyUser.begin(), m_vecEmptyUser.end(), puser);

		if (it != m_vecEmptyUser.end())
			return;

		m_vecEmptyUser.push_back(puser);
	}
	//UnLock();

	return;
}


void UserContainer::Add_CurUser(int userid, SUser* puser)
{
	if (puser == NULL)
		return;\

	CSLOCK(m_cs)
	{
		m_mapConnectUser.insert(std::pair<int, SUser*>(userid, puser));
	}

	return;
}

SUser* UserContainer::Pop_EmptyUser()
{
	//SUser* pUser = NULL;

	if (m_vecEmptyUser.size() == 0)
	{
		//UnLock();
		return NULL;
	}

	//	this->m_vecEmptyUser.erase

	SUser* puser = NULL;

	CSLOCK(m_cs)
	{
		VECTOR_USER::iterator ituser = m_vecEmptyUser.begin();

		puser = *ituser;
		m_vecEmptyUser.erase(ituser);
	}
	//UnLock();
	//SUser* puser = (SUser*)ituser._Ptr;
	return puser;
}

void UserContainer::Remove_CurUser(SOCKET sock)
{
	MAP_USER::iterator it = m_mapConnectUser.find(sock);
	if (it != m_mapConnectUser.end())
	{
		m_mapConnectUser.erase(it);
	}
}

void UserContainer::DisConnect(SOCKET_CONTEXT* pUser)
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
		MAP_USER::iterator it = m_mapConnectUser.find(pUser->m_socket);

		if (it == m_mapConnectUser.end())
			return;

		m_mapConnectUser.erase(it);
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
