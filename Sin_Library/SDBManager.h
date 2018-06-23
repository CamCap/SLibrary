#pragma once

#include "SMySqlManager.h"
#include "Container.h"
#include <functional>
#include <thread>
#include <atlstr.h>
#define MAX_SQL_LIST 10

#define MAX_QUERY_BUFF_SIZE 1500 

struct BTZ_SQL
{
	DWORD sql_id;
	char query[MAX_QUERY_BUFF_SIZE];
};

class SDBManager
{
public:
	SDBManager();
	~SDBManager();

	bool ConnectDBMS(SQLWCHAR* ip, SQLWCHAR* id, SQLWCHAR* pw);
	//void DBProcess(BTZ_SQL* sql, DWORD wParam, DWORD lParam);

	static void DBProcedure(SDBManager* db);

	void PushBtzSql(BTZ_SQL* sql) { m_vecDBMsgContainer.push(sql); }
	BTZ_SQL* PopBtzSql() { return m_vecDBMsgContainer.pop(); }
	
	void PushWaitSql(BTZ_SQL* sql) { m_vecDBMsg.push(sql); SetEvent(); }
	BTZ_SQL* PopWaitSql();

	//BTZ_SQL* PopEchoSql() { return m_vecDBMsgEcho.pop(); }

public:
	int Exec(BTZ_SQL* sql, ResSql& res_sql) {
		CString w_char(sql->query);
		int result = m_sqlManager.ExecuteStatementDirect(w_char.GetBuffer(), res_sql);

		return result;
	}

private:
	void WaitEvent();
	void SetEvent();
	void ResetEvent();

public:
	std::function<void(BTZ_SQL*, ResSql&, DWORD) > m_dbProcess;
	
private:
	VecContainer<BTZ_SQL> m_vecDBMsgContainer;	// 전체 컨테이너
	VecContainer<BTZ_SQL> m_vecDBMsg;			// 대기중인 컨테이너
	VecContainer<BTZ_SQL> m_vecDBMsgEcho;		// 결과 컨테이너 // 안씀

	HANDLE m_hEvent;

	SMySqlManager m_sqlManager;

	std::thread m_threadHandle;
};

