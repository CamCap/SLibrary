#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbccp32.lib")

#define MAX_COLUMN 100
#define MAX_COLUMN_NAME 50
#define MAX_COLUMN_SIZE 255

class SMySqlManager
{
public:
	void AllocateHandle(); // 핸들변수 초기화
	bool ConnectDataSource(SQLWCHAR* _dns, SQLWCHAR* _id, SQLWCHAR* _pw); // DBMS에 접속
	
	void ExecuteStatementDirect(SQLWCHAR* sql); // 준비과정없이 바로 실행
	
	void PrepareStatement(SQLWCHAR* sql); // 쿼리 실행을 위한 준비
	void ExecuteStatement(); // 실행준비된 쿼리를 실행
	
	bool RetrieveResult(); // 결과 리턴
	
	void DisconnectDataSource(); // 할당했던 핸들을 해제

public:
	SMySqlManager();
	~SMySqlManager();

private:
	SQLHENV      m_hEnv; // 환경 핸들
	SQLHDBC      m_hDbc;
	SQLRETURN    m_ret;

	SQLHSTMT      m_hStmt;
	SQLLEN		  m_numRow;
	SQLSMALLINT   m_numCol;
	SQLWCHAR       m_colName[MAX_COLUMN][MAX_COLUMN_NAME];
	SQLLEN    m_stateCol[MAX_COLUMN];

	SQLINTEGER	  rec;
	SQLINTEGER	  native;
	SQLWCHAR      m_state[10];
	SQLWCHAR	  m_message[255];
	SQLSMALLINT	  m_length;

	char         m_col[MAX_COLUMN][MAX_COLUMN_SIZE];
	BOOL         m_IsConnect;
};

