#pragma once
#include "SServerContainer.h"

#define MAX_WORKER_THREAD 100
#define ListenQueue 10
#define USER_ID_INDEX 1


unsigned WINAPI Accept(LPVOID pAcceptOL);
unsigned WINAPI WorkThread(LPVOID pOL);

typedef void(*IOCPAccept)(SOCKET, SOCKADDR_IN);
typedef void(*IOCPWork)(SPeer*, IO_OVERLAPPED*, int);
typedef void(*IOCPDisconnect)(SPeer*);


class SIOCP
{
public:
	BOOL CreateIOCP(); //IOCP�� ��������
	void CleanUp();


	BOOL RegisterCompletionPort(SOCKET socket, SPeer* context);
	BOOL GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR* pOutCompletionKey, WSAOVERLAPPED** pOutOverlapped, \
		int* pErrCode = NULL, DWORD dwWaitingTime = INFINITE); //INFINITE�� �����ϸ� ���Ѵ�� ����Ѵ�. �� �����带 �����·� ����
	BOOL PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred = 0, WSAOVERLAPPED* pOverlapped = NULL);
		
public:
	explicit SIOCP();
	~SIOCP();

private:
	BOOL CreateIOCPThread();

public:
	static DWORD g_userID;

	IOCPAccept m_ThreadAccept;
	IOCPWork m_ThreadWork;
	IOCPDisconnect m_ThreadDisconnect;

protected:
	SCriticalSection m_cs;

private:
	HANDLE m_handle; // iocp �ڵ�
	SYSTEM_INFO m_system;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	short m_threadcount;
	
};

