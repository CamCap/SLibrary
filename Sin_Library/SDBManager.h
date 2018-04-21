#pragma once

#include "SMySqlManager.h"
#include "Container.h"
#include <functional>
#include <thread>

#define MAX_SQL_LIST 10

#define MAX_QUERY_BUFF_SIZE 1500 

struct BTZ_SQL
{
	DWORD sql_id;
//	short sql_size;
	char query[MAX_QUERY_BUFF_SIZE];
};

class SDBManager
{
//	using namespace std::thread;

public:
	SDBManager();
	~SDBManager();

	bool ConnectDBMS(const char* ip, const char* id, const char* pw);
	//void DBProcess(BTZ_SQL* sql, DWORD wParam, DWORD lParam);
	std::function<void(BTZ_SQL*, DWORD, DWORD) > m_dbProcess;

	void DBProcedure();

	void PushBtzSql(BTZ_SQL* sql) { m_vecDBMsgContainer.push(sql); }
	BTZ_SQL* PopBtzSql() { return m_vecDBMsgContainer.pop(); }
	
	void PushWaitSql(BTZ_SQL* sql) { m_vecDBMsg.push(sql); }
	BTZ_SQL* PopWaitSql() { return m_vecDBMsg.pop(); }

	BTZ_SQL* PopEchoSql() { return m_vecDBMsgEcho.pop(); }

	char* GetResultStr() {}

public:
	template <class... args>
	void Exec(BTZ_SQL* sql, args... Arg) { m_sqlManager.ExecuteStatementDirect((SQLWCHAR*)sql->query);
		m_sqlManager.RetrieveResult(Arg...);
	}


private:
	VecContainer<BTZ_SQL> m_vecDBMsgContainer; // 전체 컨테이너
	VecContainer<BTZ_SQL> m_vecDBMsg; // 대기중인 컨테이너
	VecContainer<BTZ_SQL> m_vecDBMsgEcho; // 결과 컨테이너

	SMySqlManager m_sqlManager;

	std::thread m_threadHandle;
};

