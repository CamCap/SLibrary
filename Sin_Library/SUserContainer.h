#pragma once

#include "ssocket.h"
#include "Container.h"

#define MAX_USER_COUNT 500

struct SOCKET_CONTEXT;

//template<typename _peer = SPeer>
class UserContainer
	:public Container<SPeer>
{
public:
	void Remove_CurUser(int key) { Container<SPeer>::MapPopBack(key); }
	SPeer* Pop_EmptyUser() { return Container<SPeer>::VecPopBack(); }
	void Add_CurUser(int key, SPeer* value) { Container<SPeer>::MapPushBack(key, value); }
	void Push_EmptyUser(SPeer* value) { Container<SPeer>::VecPushBack(value); }
		
public:
	static UserContainer* GetInstance();

public:
	UserContainer();
	virtual ~UserContainer();

	void DisConnect(SOCKET_CONTEXT* pUser);

private:
	SPeer m_user[MAX_USER_COUNT];

	static UserContainer* m_instance;
};

