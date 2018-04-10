#pragma once
#include "SServerContainer.h"

#define MAX_WORKER_THREAD 100
#define ListenQueue 10
#define USER_ID_INDEX 1


unsigned WINAPI Accept(LPVOID pAcceptOL);
unsigned WINAPI WorkThread(LPVOID pOL);

void AcceptRoutinue(SOCKET, SOCKADDR_IN);
void WorkRoutinue(SPeer*, IO_OVERLAPPED*, int);
void Disconnect(SPeer*);

typedef void(*IOCPAccept)(SOCKET, SOCKADDR_IN);
typedef void(*IOCPWork)(SPeer*, IO_OVERLAPPED*, int);
typedef void(*IOCPDisconnect)(SPeer*);


class IOCP
{
public:
	BOOL CreateIOCP(IOCPAccept _Accept = AcceptRoutinue, \
		IOCPWork _WorkThread = WorkRoutinue, \
		IOCPDisconnect _Disconnect = Disconnect); //IOCP를 생성하자
	void CleanUp();


	BOOL RegisterCompletionPort(SOCKET socket, SPeer* context);
	BOOL GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR* pOutCompletionKey, WSAOVERLAPPED** pOutOverlapped, \
		int* pErrCode = NULL, DWORD dwWaitingTime = INFINITE); //INFINITE를 설정하면 무한대로 대기한다. 즉 스레드를 대기상태로 만듬
	BOOL PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred = 0, WSAOVERLAPPED* pOverlapped = NULL);

	static IOCP* GetInstance();
	
	IOCPAccept* GetAcceptRoutinue() { return &m_ThreadAccept; }
	IOCPWork* GetWorkRoutinue() { return &m_ThreadWork; }
	IOCPDisconnect* GetDisconnectRoutinue() { return &m_ThreadDisconnect; }

public:
	explicit IOCP();
	~IOCP();

private:
	BOOL CreateIOCPThread();

public:
	static DWORD g_userID;

protected:
	SCriticalSection m_cs;

private:

	HANDLE m_handle; // iocp 핸들
	SYSTEM_INFO m_system;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	short m_threadcount;

	static IOCP* m_instance;

	IOCPAccept m_ThreadAccept;
	IOCPWork m_ThreadWork;
	IOCPDisconnect m_ThreadDisconnect;
};

