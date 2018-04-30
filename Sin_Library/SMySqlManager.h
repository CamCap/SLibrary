#pragma once
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include "header.h"

#pragma comment(lib,"libmysql.lib")
#pragma comment(lib,"odbc32.lib")
#pragma comment(lib,"odbccp32.lib")

#define MAX_COLUMN 100
#define MAX_COLUMN_NAME 50
#define MAX_COLUMN_SIZE 255

struct ResSql
{
	int m_state;
	char m_col[MAX_COLUMN][MAX_COLUMN_SIZE];
};

class SMySqlManager
{
public:
	DECLARE_ENUM(RES_ENUM,
		SUCESS, //정상적으로 실행됨
		NON_VALUE, // select했으나 값이 없음
		MAX_OVER, // 최대 칼럼값을 초과
		OVERLAP_VALUE
	);

public:
	void AllocateHandle(); // 핸들변수 초기화
	bool ConnectDataSource(SQLWCHAR* _dns, SQLWCHAR* _id, SQLWCHAR* _pw); // DBMS에 접속
	int ExecuteStatementDirect(SQLWCHAR* sql, ResSql& res_sql); // 준비과정없이 바로 실행
	void DisconnectDataSource(); // 할당했던 핸들을 해제


private:
	void Clear();
	bool Fetch();

public:
	SMySqlManager();
	~SMySqlManager();

private:
	SQLHENV      m_hEnv; // 환경 핸들
	SQLHDBC      m_hDbc;

	SQLHSTMT      m_hStmt;
	SQLLEN		  m_numRow;
	SQLSMALLINT   m_numCol;
	SQLWCHAR       m_colName[MAX_COLUMN][MAX_COLUMN_NAME];
	SQLLEN		 m_stateCol[MAX_COLUMN];

	SQLINTEGER	  rec;
	SQLINTEGER	  native;
	SQLWCHAR      m_state[10];
	SQLWCHAR	  m_message[255];
	SQLSMALLINT	  m_length;

	char         m_col[MAX_COLUMN][MAX_COLUMN_SIZE];
	//BOOL         m_IsConnect;
};
