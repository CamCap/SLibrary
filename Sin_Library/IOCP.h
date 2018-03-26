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

unsigned WINAPI Accept(LPVOID pAcceptOL);
unsigned WINAPI WorkThread(LPVOID pOL);

void AcceptRoutinue(SOCKET, SOCKADDR_IN);
void WorkRoutinue();

//���̺귯���ϱ� �̱��� �������� ������ �ʿ䰡 ���� ������??
//����ؼ� ���°� ���� �Ͱ����ѵ�..

typedef void(*IOCPAccept)(SOCKET, SOCKADDR_IN);
typedef void(*IOCPWork)(void);


class IOCP
{
public:
	BOOL CreateIOCP(IOCPAccept _Accept = AcceptRoutinue, IOCPWork _WorkThread = WorkRoutinue); //IOCP�� ��������
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
	BOOL CreateIOCPThread();

public:
	static DWORD g_userID;

protected:

	SCriticalSection m_cs;


private:

	HANDLE m_handle; // iocp �ڵ�
	SYSTEM_INFO m_system;

	HANDLE m_threads[MAX_WORKER_THREAD];
	HANDLE m_acceptthread;
	short m_threadcount;

	static IOCP* m_instance;

	IOCPAccept m_ThreadAccept;
	IOCPWork m_ThreadWork;
};

