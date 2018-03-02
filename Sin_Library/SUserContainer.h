#pragma once
#include <vector>
#include <map>
#include "CriticalSection.h"
#include "IOCP.h"
#include "ssocket.h"

#define MAX_USER_COUNT 500

typedef std::vector<SUser*> VECTOR_USER;
typedef std::map<int, SUser*> MAP_USER;

struct SOCKET_CONTEXT;

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
