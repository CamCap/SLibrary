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
		SUCESS, //���������� �����
		NON_VALUE, // select������ ���� ����
		MAX_OVER, // �ִ� Į������ �ʰ�
		OVERLAP_VALUE
	);

public:
	void AllocateHandle(); // �ڵ麯�� �ʱ�ȭ
	bool ConnectDataSource(SQLWCHAR* _dns, SQLWCHAR* _id, SQLWCHAR* _pw); // DBMS�� ����
	int ExecuteStatementDirect(SQLWCHAR* sql, ResSql& res_sql); // �غ�������� �ٷ� ����
	void DisconnectDataSource(); // �Ҵ��ߴ� �ڵ��� ����


private:
	void Clear();
	bool Fetch();

public:
	SMySqlManager();
	~SMySqlManager();

private:
	SQLHENV      m_hEnv; // ȯ�� �ڵ�
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
