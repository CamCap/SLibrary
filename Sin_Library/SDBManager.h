#pragma once

#include "SMySqlManager.h"
#include "Container.h"
#include <functional>

#define MAX_SQL_LIST 10

struct BTZ_SQL
{
	DWORD sql_id;
	short sql_size;
};

class SDBManager
{
public:
	SDBManager();
	~SDBManager();

	bool ConnectDBMS(const char* ip, const char* id, const char* pw);
	//void DBProcess(BTZ_SQL* sql, DWORD wParam, DWORD lParam);
	std::function<void(BTZ_SQL*, DWORD, DWORD) > m_dbProcess;

	static DWORD WINAPI DBProcedure(void* pArg);

	void PushBtzSql(BTZ_SQL* sql) { m_vecDBMsgContainer.push(sql); }
	BTZ_SQL* PopBtzSql() { return m_vecDBMsgContainer.pop(); }
	
	void PushWaitSql(BTZ_SQL* sql) { m_vecDBMsg.push(sql); }
	BTZ_SQL* PopWaitSql() { return m_vecDBMsg.pop(); }


	BTZ_SQL* PopEchoSql() { return m_vecDBMsgEcho.pop(); }
private:
	VecContainer<BTZ_SQL> m_vecDBMsgContainer; // 전체 컨테이너
	VecContainer<BTZ_SQL> m_vecDBMsg; // 대기중인 컨테이너
	VecContainer<BTZ_SQL> m_vecDBMsgEcho; // 결과 컨테이너

	SMySqlManager m_sqlManager;

	HANDLE m_threadHandle;
};

