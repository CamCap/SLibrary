#include "stdafx.h"
#include "SDBManager.h"


SDBManager::SDBManager()
	:m_vecDBMsg(0), m_vecDBMsgContainer(MAX_SQL_LIST), m_vecDBMsgEcho(0)
{
	this->m_sqlManager.AllocateHandle();
	m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}


SDBManager::~SDBManager()
{
	CloseHandle(m_hEvent); m_hEvent = NULL;

	m_threadHandle.detach();
	this->m_sqlManager.DisconnectDataSource();


}

bool SDBManager::ConnectDBMS(SQLWCHAR* ip, SQLWCHAR* id, SQLWCHAR* pw)
{
	if (!m_sqlManager.ConnectDataSource((SQLWCHAR*)ip, (SQLWCHAR*)id, (SQLWCHAR*)pw))
	{
		return false;
	}

	m_threadHandle = std::thread([&]() {this->DBProcedure(this); });

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
BTZ_SQL* SDBManager::PopWaitSql()
{
	BTZ_SQL* sql = NULL;

	while (1)
	{
		if (m_vecDBMsg.size() > 0)
			break;
		else
		{
			ResetEvent();
			WaitEvent();
		}

	}

	sql = m_vecDBMsg.pop();

	return sql;
}

void SDBManager::DBProcedure(SDBManager* db)
{
//	SDBManager* db = (SDBManager*)pArg;

	ResSql res_sql;
	BTZ_SQL* sql;
	int result;

	while (true)
	{
		sql = db->PopWaitSql();

		if (sql == NULL) continue;
		if (db->m_dbProcess == NULL) {
			db->PushWaitSql(sql);
			continue;
		}

//		SQL_RES
		result = db->Exec(sql, res_sql);

		db->m_dbProcess(sql, res_sql, result);
		
		db->PushBtzSql(sql);
	}
}


void SDBManager::WaitEvent()
{
	if (m_hEvent != NULL)
		WaitForSingleObject(m_hEvent, 100); // 1/1000초만큼 대기
}

void SDBManager::SetEvent()
{
	if (m_hEvent != NULL)
		::SetEvent(m_hEvent); // signal 상태로 변경
}

void SDBManager::ResetEvent()
{
	if (m_hEvent != NULL)
		::ResetEvent(m_hEvent); // non-signal 상태로 변경
}