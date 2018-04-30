#include "stdafx.h"
#include "SMySqlManager.h"
#include <list>
#include "Log.h"
#include <comdef.h> 

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
				//성공
#endif
			}
			else
			{
#ifdef _DEBUG
				SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
				_bstr_t bstr(m_message);
				const char* message = bstr;
				ERROR_LOG(message);
#endif
			}
		}
		else
		{
#ifdef _DEBUG
			SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
			_bstr_t bstr(m_message);
			const char* message = bstr;
			ERROR_LOG(message); 
#endif
		}
	}
	else
	{
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
		_bstr_t bstr(m_message);
		const char* message = bstr;
		ERROR_LOG(message);
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
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
		_bstr_t bstr(m_message);
		const char* message = bstr;
		ERROR_LOG(message);
#endif
		return false;
	}

	result = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);

	if (result == SQL_SUCCESS || result == SQL_SUCCESS_WITH_INFO)
	{
	}
	else {
#ifdef _DEBUG
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
		_bstr_t bstr(m_message);
		const char* message = bstr;
		ERROR_LOG(message);
#endif
		return false;
	}

	return true;
}

int SMySqlManager::ExecuteStatementDirect(SQLWCHAR* sql, ResSql& res_sql)
{
	int result = 0;

	result = SQLExecDirect(m_hStmt, sql, SQL_NTS);
	
	if (result == SQL_SUCCESS) {

	}
	else {
		SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
		//memcpy(res_sql.m_message, m_message, sizeof(m_message));
		res_sql.m_state = OVERLAP_VALUE;
		return result;
	}

	SQLRowCount(m_hStmt, &m_numRow);
	SQLNumResultCols(m_hStmt, &m_numCol);

	if (m_numCol > MAX_COLUMN)
	{
		wcscpy_s(m_message, L"최대 칼럼수를 초과");	
		//memcpy(res_sql.m_message, m_message, sizeof(m_message));
		res_sql.m_state = MAX_OVER;
		return false;
	}

	if (m_numCol == 0 || m_numRow == 0)
	{
		Clear();
		res_sql.m_state = NON_VALUE;
		//SQLGetDiagRec(SQL_HANDLE_DBC, m_hDbc, ++rec, m_state, &native, m_message, sizeof(m_message), &m_length);
		return false;
	}

	for (int i = 0; i < m_numCol; i++)
	{
		SQLBindCol(m_hStmt, i + 1, SQL_C_CHAR, m_col[i], MAX_COLUMN_SIZE, &m_stateCol[i]);
		SQLDescribeCol(m_hStmt, i + 1, m_colName[i], MAX_COLUMN_NAME, NULL, NULL, NULL, NULL, NULL);
	}

	if (!Fetch())
	{
		Clear();
		return false;
	}

	res_sql.m_state = SUCESS;
	memcpy(res_sql.m_col, m_col, sizeof(char[MAX_COLUMN][MAX_COLUMN_SIZE]));

	return true;
}

bool SMySqlManager::Fetch()
{
	int result = SQLFetch(m_hStmt);

//	if (m_ret == SQL_NO_DATA) return FALSE;

	return TRUE;
}

void SMySqlManager::Clear()
{
	SQLCloseCursor(m_hStmt);
	SQLFreeStmt(m_hStmt, SQL_UNBIND);
}

//
//template <class... Args>
//constexpr TemplateClass<Args...> SMySqlManager::RetrieveResult(Args... arg)
//{
//	TemplateClass<Args...> m_temp;	
//	
//
//}

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
