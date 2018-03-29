#include "stdafx.h"
#include "IOCP.h"
#include "SSocket.h"
#include "Log.h"
#include <process.h>
#include "GameMessage.h"


unsigned WINAPI Accept(LPVOID pAcceptOL);
unsigned WINAPI WorkThread(LPVOID pOL);
void AcceptRoutinue(SOCKET, SOCKADDR_IN);
void WorkRoutinue(SPeer*, IO_OVERLAPPED*, int);

IOCP* IOCP::m_instance = NULL;
DWORD IOCP::g_userID = USER_ID_INDEX;

IOCP* IOCP::GetInstance()
{
	if (m_instance == NULL)
	{
		m_instance = new IOCP();
	}
	return m_instance;
}



IOCP::IOCP()
	:m_cs(), m_ThreadAccept(AcceptRoutinue), m_ThreadWork(WorkRoutinue)
{
}


IOCP::~IOCP()
{
	CleanUp();
}


BOOL IOCP::CreateIOCP(IOCPAccept _Accept, IOCPWork _WorkThread)
{
	WSADATA             wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		//Log::Instance()->WriteLog("Project-socket", "WASStartup error");
		ERROR_LOG("WASStartup error");
		return FALSE;
	}

	//	InitializeCriticalSection(&m_criticalsection);

	m_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (m_handle == NULL)
	{
		ERROR_LOG("WASStartup error");
		//Log::Instance()->WriteLog("Project-socket", "create IOCP handle error");
		CleanUp();
		return FALSE;
	}

	m_ThreadAccept = _Accept;
	m_ThreadWork = _WorkThread;

	if (CreateIOCPThread() == false)
	{
		ERROR_LOG("CreateIOCPThread Error!");
		//Log::Instance()->WriteLog("Project-socket", "create IOCP handle error");
		CleanUp();
		return FALSE;
	}
	return true;
}


void IOCP::CleanUp()
{
	if (m_handle)
	{
		for (BYTE i = 0; i < m_threadcount; i++)
		{
			//1을 넣으면 스레드 종료
			BOOL ret = PostCompletionStatus(1);
		}
	}


	if (WAIT_FAILED == WaitForMultipleObjects(m_threadcount, m_threads, FALSE, 10000))
	{
		printf("WaitForMultipleObjects failed");
	}

	for (BYTE i = 0; i < m_threadcount; i++)
	{
		if (m_threads[i] != INVALID_HANDLE_VALUE)
			CloseHandle(m_threads[i]);

		m_threads[i] = INVALID_HANDLE_VALUE;
	}

	if (m_acceptthread != INVALID_HANDLE_VALUE)
		CloseHandle(m_acceptthread);
	m_acceptthread = INVALID_HANDLE_VALUE;

	if (m_handle)
	{
		CloseHandle(m_handle);
		m_handle = NULL;
	}

	WSACleanup();
}

BOOL IOCP::CreateIOCPThread()
{
//	if (m_ThreadAccept == NULL || m_ThreadWork == NULL) return false;

	GetSystemInfo(&m_system);

	m_threadcount = (BYTE)m_system.dwNumberOfProcessors * 2 - 1;
	//*2인 이유는 프로세서의 2배까지 스레드를 생성해야함.
	//m_threadcount = threadcount;

	unsigned int threadid = 0;


	for (unsigned int i = 0; i < m_threadcount; i++)
	{

		//m_threads[i] = CreateThread(NULL, 0, WorkThread, this, 0, &threadid);
		m_threads[i] = (HANDLE)_beginthreadex(NULL, 0, WorkThread, m_ThreadWork, 0, &threadid);
		//WorkThread만들자.
		if (m_threads[i] == NULL)
		{
			Log::Instance()->WriteLog("Project-socket", "Thread Create Fail");
			CleanUp();
			return false;
		}
	}

	m_acceptthread = (HANDLE)_beginthreadex(NULL, 0, Accept, m_ThreadAccept, 0, &threadid);
	SetThreadPriority(m_acceptthread, THREAD_PRIORITY_HIGHEST);

	return true;
}

BOOL IOCP::RegisterCompletionPort(SOCKET socket, SPeer* context)
{
	if ((context == NULL) || (socket == INVALID_SOCKET))
		return false;

	HANDLE handle = (HANDLE)socket;

	CSLOCK(m_cs)
	{
		// 할당된 구조체와 소켓을 IOCP와 연결한다. 
		if (!CreateIoCompletionPort(handle, m_handle, (DWORD)context, 0))
		{
#ifdef _DEBUG
			char buff[100];
			int ret = WSAGetLastError();
			wsprintfA(buff, "[ERROR : %d] 에러 발생[%s] \n", ret, __FUNCTION__);
			OutputDebugStringA(buff);
#else
			SOCKET_ERROR_LOG_CODE
#endif

				//UnLock();
				return false;
		}
	}
	//UnLock();

	return true;
}


BOOL IOCP::GetCompletionStatus(LPDWORD pdwOutBytesTransferred, ULONG_PTR * pOutCompletionKey, WSAOVERLAPPED ** pOutOverlapped, int * pErrCode, DWORD dwWaitingTime)
{
	BOOL result = GetQueuedCompletionStatus(m_handle, pdwOutBytesTransferred, pOutCompletionKey, pOutOverlapped, dwWaitingTime);

	return result;
}

BOOL IOCP::PostCompletionStatus(DWORD CompleitonKey, DWORD dwBytesTransferred, WSAOVERLAPPED * pOverlapped)
{
	BOOL result = PostQueuedCompletionStatus(m_handle, CompleitonKey, dwBytesTransferred, pOverlapped);

	if (!result)
	{
#ifdef _DEBUG
		char buff[100];
		int ret = WSAGetLastError();
		wsprintfA(buff, "[ERROR : %d] 에러 발생[%s] \n", ret, __FUNCTION__);
		OutputDebugStringA(buff);
#else
		SOCKET_ERROR_LOG_CODE
#endif
	}

	return TRUE;
}




unsigned WINAPI Accept(LPVOID pAcceptOL)
{
	SSocket accept_socket;
	SOCKET client_socket;
	SOCKADDR_IN client_addr;

	if (accept_socket.CreateWSASocket() == FALSE)
	{
		//Log::Instance()->WriteLog("Project-socket", "Accept Socket Create Fail");
		SOCKET_ERROR_LOG_CODE;
		return 0;
	}


	accept_socket.SetAddr(AF_INET, SERVER_PORT, INADDR_ANY);


	if (SocketTool::Bind(accept_socket.socket, accept_socket.addr) == INVALID_SOCKET)
	{

#ifdef _DEBUG
		char buff[100];
		int ret = WSAGetLastError();
		wsprintfA(buff, "[ERROR : %d] 에러 발생[%s] \n", ret, __FUNCTION__);
		OutputDebugStringA(buff);
#else
		SOCKET_ERROR_LOG_CODE
#endif

			accept_socket.CloseSocket();
		return 0;
		//	Log::Instance()->WriteLog("Project-socket", "Socket Bind Error");
	}

	int send_size;
	int send_len = sizeof(send_size);


	//listen_socket의 Send buffer의 옵션을 확인하기위한 함수
	if (getsockopt(accept_socket.socket, SOL_SOCKET, SO_SNDBUF, (char *)&send_size, &send_len) != SOCKET_ERROR)
	{
		send_size *= 100; //send buffer 크기를 왜 100배로?
		if (setsockopt(accept_socket.socket, SOL_SOCKET, SO_SNDBUF, (char*)&send_size, sizeof(send_len)) == SOCKET_ERROR)
		{
			//setscokopt에 실패했을 때

			ERROR_LOG("connect fail1");
		}
	}
	else // getsockopt 실패시
	{
		ERROR_LOG("connect faile2");
	}

	if (SocketTool::Listen(accept_socket.socket, ListenQueue) == INVALID_SOCKET)
	{
		accept_socket.CloseSocket();
		SOCKET_ERROR_LOG_CODE;
		return 0;
	}
	else
		printf("Listen Sucess! IP : %s / Port : %d \n", SERVER_IP, SERVER_PORT);


	//accept 반복
	while (1)
	{
		client_socket = SocketTool::Accept(accept_socket.socket, client_addr);

		if (client_socket == INVALID_SOCKET)
		{
			accept_socket.CloseSocket();
			continue;
		}

		//Aceept를 성공한 후에...
//		SPeer* puser = UserContainer::GetInstance()->Pop_EmptyUser();

		IOCPAccept routinue = (IOCPAccept)pAcceptOL;
		routinue(client_socket, client_addr);
	}
}


unsigned WINAPI WorkThread(LPVOID pOL)
{
	DWORD DwNumberBytes = 0;
	SPeer* pCompletionKey = NULL;
	IO_OVERLAPPED* pOverlapped = NULL;
	BOOL result = FALSE;
	//

	while (true)
	{
		result = IOCP::GetInstance()->GetCompletionStatus(&DwNumberBytes, reinterpret_cast<ULONG_PTR*>(&pCompletionKey), reinterpret_cast<WSAOVERLAPPED **>(&pOverlapped));

		if (result) // 정상적으로 실행.
		{
			//SOCKET_ERROR_LOG_CODE;
			//continue;

			//if((int)pCompletionKey->)

			if (pOverlapped == NULL) continue;
		}
		else
		{
			if (pOverlapped != NULL)
			{
				SPeerContainer<>::GetInstance()->DisConnect(pCompletionKey);
				IOCP::GetInstance()->PostCompletionStatus((DWORD)pCompletionKey, 0, (OVERLAPPED*)pOverlapped);
				GameMessageManager::Instnace()->SendGameMessage(GM_DISCONNECTUSER, (ULONG64)pCompletionKey, (ULONG64)pOverlapped, NULL);
			}

			continue;
		}

		//이미 연결이 끊김
		if ((pCompletionKey == NULL)) continue;

		//클라가 연결을 끊음
		if (DwNumberBytes == 0)
		{
			SPeerContainer<>::GetInstance()->DisConnect(pCompletionKey); 
			IOCP::GetInstance()->PostCompletionStatus((DWORD)pCompletionKey, 0, (OVERLAPPED*)pOverlapped);
			GameMessageManager::Instnace()->SendGameMessage(GM_DISCONNECTUSER, (ULONG64)pCompletionKey, (ULONG64)pOverlapped, NULL);
			continue;
		}

		IOCPWork routinue = (IOCPWork)pOL;
		routinue(pCompletionKey, pOverlapped, DwNumberBytes);
	}

	return 0;
}

void AcceptRoutinue(SOCKET client_socket, SOCKADDR_IN client_addr)
{
	SPeerContainer<>* container = SPeerContainer<>::GetInstance();

	SPeer* puser = container->Pop_EmptyPeer();

	if (puser != NULL)
	{
		container->Add_CurPeer(IOCP::g_userID, puser);

		if (IOCP::GetInstance()->RegisterCompletionPort(client_socket, puser) == false)
		{
			printf("[Accept Thread] : RegisterCompletionPort fail \n");
		}
		else
		{
			puser->InitPeer(client_socket, client_addr, IOCP::g_userID++);

			printf("[Accept Thread] : RegisterCompletionPort Sucess \n");
			GameMessageManager::Instnace()->SendGameMessage(GM_ACCEPTUPEER, 0, 0, NULL);
		}
	}

	return;
}

void WorkRoutinue(SPeer* pCompletionKey, IO_OVERLAPPED* pOverlapped, int DwNumberBytes)
{
	if (pOverlapped->io_type == IO_RECV)
	{
//		pCompletionKey->m_puser->RecvPacket(DwNumberBytes);
		pCompletionKey->RecvPacket(DwNumberBytes);
		
	}
	else if (pOverlapped->io_type == IO_SENDING)
	{
		//송신 처리 함수
//		pCompletionKey->m_puser->CheckSendPacket();
		pCompletionKey->CheckSendPacket();
	}
	else // IO_NONE 혹은 에러
	{

	}
}