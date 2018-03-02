#pragma once
#include "CriticalSection.h"

#define MAX_WORKER_THREAD 100
#define ListenQueue 10
#define USER_ID_INDEX 100

class SUser;

struct SOCKET_CONTEXT
{
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	SUser* m_puser;
};


//���̺귯���ϱ� �̱��� �������� ������ �ʿ䰡 ���� ������??
//����ؼ� ���°� ���� �Ͱ����ѵ�..

class IOCP
{
public:

	BOOL CreateIOCP(); //IOCP�� ��������
	void CleanUp();


	BOOL RegisterCompletionPort(SOCKET_CONTEXT* lpPerSocketContext);
	BOOL GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR* pOutCompletionKey, WSAOVERLAPPED** pOutOverlapped, \
		int* pErrCode = NULL, DWORD dwWaitingTime = INFINITE); //INFINITE�� �����ϸ� ���Ѵ�� ����Ѵ�. �� �����带 �����·� ����
	BOOL PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred = 0, WSAOVERLAPPED* pOverlapped = NULL);

	static IOCP* GetInstance();

public:
	explicit IOCP();
	~IOCP();
	
private:
	void CreateIOCPThread();

public:
	static DWORD g_userID;

private:

	HANDLE m_handle; // iocp �ڵ�
	SYSTEM_INFO m_system;

	SCriticalSection m_cs;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	BYTE m_threadcount;

	static IOCP* m_instance;
};
