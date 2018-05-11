#pragma once
#include "SPeerContainer.h"

#define MAX_WORKER_THREAD 50
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
	bool CreateIOCP(const char* ip, int port); //IOCP를 생성하자
	void CleanUp();


	bool RegisterCompletionPort(SOCKET socket, SPeer* context);
	bool GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR* pOutCompletionKey, WSAOVERLAPPED** pOutOverlapped, \
		int* pErrCode = NULL, DWORD dwWaitingTime = INFINITE); //INFINITE를 설정하면 무한대로 대기한다. 즉 스레드를 대기상태로 만듬
	bool PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred = 0, WSAOVERLAPPED* pOverlapped = NULL);
	
	constexpr SSocket& GetAcceptSocket() { return accept_socket; }

public:
	explicit SIOCP();
	~SIOCP();

private:
	bool CreateIOCPThread();

public:
	static DWORD g_userID;

	IOCPAccept m_RoutinueAccept;
	IOCPWork m_RoutinueWork;
	IOCPDisconnect m_RoutinueDisconnect;

protected:
	SCriticalSection m_cs;

private:
	HANDLE m_handle; // iocp 핸들
	SYSTEM_INFO m_system;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	unsigned short m_threadcount;

	SSocket accept_socket;
};

