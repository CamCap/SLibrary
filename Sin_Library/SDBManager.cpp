#include "stdafx.h"
#include "SDBManager.h"


SDBManager::SDBManager()
	:m_vecDBMsg(MAX_SQL_LIST), m_vecDBMsgContainer(MAX_SQL_LIST), m_vecDBMsgEcho(MAX_SQL_LIST)
{
	this->m_sqlManager.AllocateHandle();
}


SDBManager::~SDBManager()
{
	this->m_sqlManager.DisconnectDataSource();
	m_threadHandle.detach();
}

bool SDBManager::ConnectDBMS(const char* ip, const char* id, const char* pw)
{
	if (!m_sqlManager.ConnectDataSource((SQLWCHAR*)ip, (SQLWCHAR*)id, (SQLWCHAR*)pw))
	{
		return false;
	}

	m_threadHandle = std::thread([&]() {this->DBProcedure(); });

	return true;
}

//void DBManager::DBProcess(BTZ_SQL* sql, DWORD wParam, DWORD lparam)
//{
//	switch (sql->sql_id)
//	{
//	default:
//		break;
//	}
//}

void SDBManager::DBProcedure()
{
//	SDBManager* db = (SDBManager*)pArg;

	while (true)
	{
		BTZ_SQL* sql = PopWaitSql();

		if (sql == NULL) continue;
		if (m_dbProcess == NULL) {
			PushWaitSql(sql);
			continue;
		}
		Exec(sql);
		m_dbProcess(sql, NULL, NULL);
		PushBtzSql(sql);
	}
}