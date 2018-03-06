#pragma once
#include "SUserContainer.h"

#define MAX_WORKER_THREAD 100
#define ListenQueue 10
#define USER_ID_INDEX 100

class SPeer;

struct SOCKET_CONTEXT
{
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
	SPeer* m_puser;
};

typedef unsigned int(__stdcall *IOCPThread)(void*);

unsigned WINAPI Accept(LPVOID pAcceptOL);
unsigned WINAPI WorkThread(void* pOL);

//라이브러리니까 싱글톤 패턴으로 구현할 필요가 없지 않을까??
//상속해서 쓰는게 좋을 것같긴한데..


class IOCP
{
public:
	
	BOOL CreateIOCP(IOCPThread _Accept = Accept, IOCPThread _WorkThread = WorkThread); //IOCP를 생성하자
	void CleanUp();


	BOOL RegisterCompletionPort(SOCKET_CONTEXT* lpPerSocketContext);
	BOOL GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR* pOutCompletionKey, WSAOVERLAPPED** pOutOverlapped, \
		int* pErrCode = NULL, DWORD dwWaitingTime = INFINITE); //INFINITE를 설정하면 무한대로 대기한다. 즉 스레드를 대기상태로 만듬
	BOOL PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred = 0, WSAOVERLAPPED* pOverlapped = NULL);

	static IOCP* GetInstance();

public:
	explicit IOCP();
	~IOCP();
	
private:
	BOOL CreateIOCPThread();

public:
	static DWORD g_userID;

private:

	HANDLE m_handle; // iocp 핸들
	SYSTEM_INFO m_system;

	SCriticalSection m_cs;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	BYTE m_threadcount;

	static IOCP* m_instance;

	IOCPThread m_ThreadAccept;
	IOCPThread m_ThreadWork;
};

