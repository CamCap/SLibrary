#include "stdafx.h"
#include "SMySqlManager.h"


SMySqlManager::SMySqlManager()
{
	//mysql_get_client_info();
}


SMySqlManager::~SMySqlManager()
{
}

void SMySqlManager::AllocateHandle()
{
	int result = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);

	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{
		result = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);

		if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
		{
			result = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);

			if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
			{
#ifdef _DEBUG
				//¼º°ø
#endif
			}
			else
			{
#ifdef _DEBUG
				SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
			}
		}
		else
		{
#ifdef _DEBUG
			SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
		}
	}
	else
	{
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
	}
}

bool SMySqlManager::ConnectDataSource(SQLWCHAR* _dns, SQLWCHAR* _id, SQLWCHAR* _pw)
{
	int result = SQLConnect(m_hDbc, _dns, SQL_NTS, _id, SQL_NTS, _pw, SQL_NTS);

	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{

	}
	else
	{
		return false;
	}
	return true;
}

void SMySqlManager::ExecuteStatementDirect(SQLWCHAR* sql) 
{
	int result = 0;
	result = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	
	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{
	}
	else {
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
	}


	result = SQLExecDirect(m_hStmt, sql, SQL_NTS);
	
	if (result == SQL_SUCCESS) {

	}
	else {
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
	}

}

void SMySqlManager::PrepareStatement(SQLWCHAR * sql)
{
	int result = 0;
	result = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	
	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{
	}
	else {
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
	}


	result = SQLPrepare(m_hStmt, sql, SQL_NTS);


	if (result == SQL_SUCCESS) {

	}
	else {
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
	}
}

void SMySqlManager::ExecuteStatement() 
{
	int result = SQLExecute(m_hStmt);

	if (result == SQL_SUCCESS) {
	}
	else 
	{
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
#endif
	}

}


bool SMySqlManager::RetrieveResult()
{
	short choco_id;
	char choco_name[16];
	float choco_cal;
	SQLLEN cid, cna, cal;
	int result = 0;

	SQLRowCount(m_hStmt, &m_numRow);
	SQLNumResultCols(m_hStmt, &m_numCol);
	
	if (m_numCol > MAX_COLUMN)
	{
		return false;
	}

	if (m_numCol == 0)
	{
		SQLCloseCursor(m_hStmt);
		SQLFreeStmt(m_hStmt, SQL_UNBIND);
		return TRUE;
	}

	for (int i = 0; i < m_numCol; i++)
	{
		SQLBindCol(m_hStmt, i + 1, SQL_C_CHAR, m_col[i], MAX_COLUMN_SIZE, &m_stateCol[i]);
		SQLDescribeCol(m_hStmt, i + 1, m_colName[i], MAX_COLUMN_NAME, NULL, NULL, NULL, NULL, NULL);
	}

	result = SQLFetch(m_hStmt);

	if (m_ret == SQL_NO_DATA)
	{
		SQLCloseCursor(m_hStmt);
		SQLFreeStmt(m_hStmt, SQL_UNBIND);
		return FALSE;
	}

	return true;
//	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}

void SMySqlManager::DisconnectDataSource()
{
	if (m_hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
		m_hStmt = NULL;
	}

	SQLDisconnect(m_hDbc);

	if (m_hDbc)
	{
		SQLFreeHandle(SQL_HANDLE_DBC, m_hDbc);
		m_hDbc = NULL;
	}

	if (m_hEnv) {
		SQLFreeHandle(SQL_HANDLE_ENV, m_hEnv);
		m_hEnv = NULL;
	}
}
