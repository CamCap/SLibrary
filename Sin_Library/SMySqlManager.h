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
	void AllocateHandle(); // �ڵ麯�� �ʱ�ȭ
	bool ConnectDataSource(SQLWCHAR* _dns, SQLWCHAR* _id, SQLWCHAR* _pw); // DBMS�� ����
	
	void ExecuteStatementDirect(SQLWCHAR* sql); // �غ�������� �ٷ� ����
	
	void PrepareStatement(SQLWCHAR* sql); // ���� ������ ���� �غ�
	void ExecuteStatement(); // �����غ�� ������ ����
	
	bool RetrieveResult(); // ��� ����
	
	void DisconnectDataSource(); // �Ҵ��ߴ� �ڵ��� ����

public:
	SMySqlManager();
	~SMySqlManager();

private:
	SQLHENV      m_hEnv; // ȯ�� �ڵ�
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

