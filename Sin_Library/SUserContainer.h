#pragma once

#include "ssocket.h"
#include "Container.h"

#define MAX_USER_COUNT 500

struct SOCKET_CONTEXT;

//template<typename _peer = SPeer>
class UserContainer
	:public Container<SPeer>
{
private:

	typedef void (Container<SPeer>::*remove_curUser)(int);
	typedef SPeer* (Container<SPeer>::*pop_emptyUser)();
	typedef void (Container<SPeer>::*add_curUser)(int, SPeer*);
	typedef void (Container<SPeer>::*push_emptyUser)(SPeer*);

public:

	remove_curUser Remove_curUser = MapPopBack;
	pop_emptyUser Pop_EmptyUser = VecPopBack;
	add_curUser Add_CurUser = MapPushBack;
	push_emptyUser Push_EmptyUser = VecPushBack;

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

