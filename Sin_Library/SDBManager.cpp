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
	CloseHandle(m_threadHandle);
}

bool SDBManager::ConnectDBMS(const char* ip, const char* id, const char* pw)
{
	if (!m_sqlManager.ConnectDataSource((SQLWCHAR*)ip, (SQLWCHAR*)id, (SQLWCHAR*)pw))
	{
		return false;
	}

	m_threadHandle = CreateThread(NULL, 0, DBProcedure, (void*)this, 0, NULL);

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

DWORD WINAPI SDBManager::DBProcedure(void* pArg)
{
	SDBManager* db = (SDBManager*)pArg;

	while (true)
	{
		BTZ_SQL* sql = db->PopWaitSql();

		if (sql == NULL) continue;
		if (db->m_dbProcess == NULL) {
			db->PushWaitSql(sql);
			continue;
		}
		db->m_dbProcess(sql, NULL, NULL);
		db->PushBtzSql(sql);
	}
}