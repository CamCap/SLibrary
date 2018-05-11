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
	bool CreateIOCP(const char* ip, int port); //IOCP�� ��������
	void CleanUp();


	bool RegisterCompletionPort(SOCKET socket, SPeer* context);
	bool GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR* pOutCompletionKey, WSAOVERLAPPED** pOutOverlapped, \
		int* pErrCode = NULL, DWORD dwWaitingTime = INFINITE); //INFINITE�� �����ϸ� ���Ѵ�� ����Ѵ�. �� �����带 �����·� ����
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
	HANDLE m_handle; // iocp �ڵ�
	SYSTEM_INFO m_system;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	unsigned short m_threadcount;

	SSocket accept_socket;
};

