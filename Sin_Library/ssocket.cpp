#include "stdafx.h"
#include "ssocket.h"
#include <cstdio>
#include "Log.h"
//#include "GameMessage.h"
#include "SIOCP.h"
#include <ws2tcpip.h>

SSocket::SSocket()
{
	m_socket = INVALID_SOCKET;
}


SSocket::~SSocket()
{
	if (m_socket != INVALID_SOCKET)
		closesocket(m_socket);
}

BOOL SSocket::CreateWSASocket(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpPorotocolInfo, GROUP g, DWORD dwFlags)
{
	if (m_socket != INVALID_SOCKET)
		return false;

	m_socket = WSASocket(af, type, protocol, lpPorotocolInfo, g, dwFlags);

	if (m_socket == INVALID_SOCKET)
		return false;

	return true;
}

void SSocket::InitSocket(SOCKET socket, SOCKADDR_IN addr)
{
	m_socket = socket;
	m_addr = addr;
}

void SSocket::SetAddr(int family, int port, const char* ip)
{
	memset(&m_addr, 0, sizeof(m_addr));

	//	m_addr.sin_addr.
	m_addr.sin_family = family;
	m_addr.sin_port = htons(port);
	auto ret = inet_pton(family, ip, (void*)&m_addr.sin_addr.s_addr);
}

void SSocket::SetAddr(SOCKADDR_IN sockaddr)
{
	memcpy(&m_addr, &sockaddr, sizeof(SOCKADDR_IN));
}

////////////////////////////////////////////////////////////////////////////

SSession::SSession()
{
	m_recvOL.io_type = IO_RECV;
	m_sendOL.io_type = IO_SEND_STAND;
}

SSession::~SSession()
{
}


void SSession::InitSession(SOCKET socket, SOCKADDR_IN addr, char* recvbuffer, int recvlen)
{

	m_recvOL.io_buff.buf = recvbuffer;
	m_recvOL.io_buff.len = recvlen;

	m_socket.InitSocket(socket, addr);
}

void SSession::ReleaseSession()
{
	//소켓 강제 종료
	///다음의 기능은 http://egloos.zum.com/mirine35/v/5057014 참조
	///우아한 종료로 TIMEOUT이 발생할 수 있음. 시나리오는 위를 참조.
	///이 TIMEOUT이 무한 발생(실제로는 240초까지만)하면, 소켓을 사용할 수 없다.
	///이를 위해 closesocket이 대기하는 시간을 조절

	LINGER LingerStruct;
	LingerStruct.l_onoff = 1;
	LingerStruct.l_linger = 0;

	if (setsockopt(m_socket.m_socket, SOL_SOCKET, SO_LINGER, (char*)&LingerStruct, sizeof(LingerStruct) == SOCKET_ERROR))
	{
		ERROR_LOG("Set Socket LINGER Error");
	}


	m_socket.CloseSocket();

	memset(&m_recvOL, 0, sizeof(IO_OVERLAPPED));
	memset(&m_sendOL, 0, sizeof(IO_OVERLAPPED));
}

void SSession::SetSessionInfo()
{

}

int SSession::Recv()
{
	SOCKET_INVALID_CHECK(m_socket.m_socket);

	if (m_recvOL.io_type == IO_NONE) return false;


	DWORD dwRecvNumBytes = 0;
	DWORD dwFlags = 0;

	memset(&m_recvOL, 0, sizeof(WSAOVERLAPPED));
	m_recvOL.io_type = IO_RECV;

	int result = WSARecv(m_socket.m_socket, &m_recvOL.io_buff, 1, &dwRecvNumBytes, &dwFlags, (WSAOVERLAPPED*)&m_recvOL, NULL);

	return result;
}

int SSession::Send(char* buffer, int len, int& errcode)
{
	SOCKET_INVALID_CHECK(m_socket.m_socket);

	if (m_sendOL.io_type == IO_SENDING) {
		errcode = SESSIONSTATE::ERROR_SENDING;
		return FALSE;
	}

	DWORD dwflag = 0;
	DWORD dwsendbyte = 0;

	m_sendOL.io_buff.buf = buffer;
	m_sendOL.io_buff.len = len;

	memset(&m_sendOL, 0, sizeof(WSAOVERLAPPED));
	m_sendOL.io_type = IO_SENDING;

	int result = WSASend(m_socket.m_socket, &m_sendOL.io_buff, 1, &dwsendbyte, dwflag, (WSAOVERLAPPED*)&m_sendOL, NULL);

	return result;
}
////////////////////////////////////////////////////////////////////////////


SPeer::SPeer()
	:m_vecSendPacket(), m_session(), m_id(-1)
{
}

SPeer::~SPeer()
{
	m_session.ReleaseSession();
}


void SPeer::Send(BTZPacket* packet)
{
	m_vecSendPacket.Push(packet);

	//m_session.Send((char*)sendpacket, sendpacket->packet_size);
	CheckSendPacket();
}

void SPeer::Send(char* data)
{
	m_vecSendPacket.Push((BTZPacket*)data);

	CheckSendPacket();
}

int SPeer::Recv()
{
	int result = m_session.Recv();

#ifdef _DLLWraper 

#else

	if ((result == FALSE) && (ERROR_IO_PENDING != WSAGetLastError()))
	{
		//에러제어
		ErrorHandle(__FUNCTION__);
	}
#endif

	return result;
}


void SPeer::CheckSendPacket()
{
	BTZPacket* sendpacket = NULL;
	sendpacket = m_vecSendPacket.Pop();

	if (sendpacket != NULL)
	{
		int errcode = 0;
		BOOL result = m_session.Send((char*)sendpacket, sendpacket->packet_size, errcode);

		if ((result == FALSE) && (errcode != SSession::SESSIONSTATE::ERROR_SENDING))
		{
			//에러제어
			ErrorHandle(__FUNCTION__);
		}
	}
	else
	{
		m_session.SendComplete();
	}
}

void SPeer::ReleaseSocket()
{
	m_session.ReleaseSession();
}

bool SPeer::InitPeer(SOCKET socket, SOCKADDR_IN addr, int userid)
{
	m_session.InitSession(socket, addr, recv_buffer, USER_BUFFER_SIZE);
	m_id = userid;
	Recv();

	return true;
}

BOOL SPeer::RecvPacket(int recv_size)
{
	BTZPacket* packet = NULL;

#ifdef _DLLWraper
#else
	CSLOCK(m_cs)
#endif
	{
		if ( m_queue.Push(recv_buffer, recv_size) == FALSE )
			return FALSE;

		while (true)
		{
			packet = m_queue.Pop();

			if ( packet == NULL ) break;

		}
	}
#ifdef _DLLWraper
#else
	Recv();
#endif
	return TRUE;
}

void SPeer::ErrorHandle(const char* function)
{
#ifdef _DEBUG
	char buff[100];
	int ret = WSAGetLastError();
	wsprintfA(buff, "[ERROR : %d] 에러 발생[%s] \n", ret, function);
	OutputDebugStringA(buff);
	printf("%s", buff);
#else
	SOCKET_ERROR_LOG_CODE;
#endif
}

////////////////////////////////////////////////////////////////////////////

void SServer::SetName(std::string name)
{
	m_name = name;
}


SServer::SServer()
	: SPeer()
{
}


SServer::~SServer()
{
}

//클라에서 받는 핑 시간으로 체크
void SServer::OnPingCheck(DWORD tick)
{
	if (tick - m_tickPing >= PING_CHECK_TIME) // 시간초과
	{
		m_tickPing = tick;

		if (GetOLType() != IO_NONE)
		{
#ifdef _DEBUG
#endif
			ReleaseSocket();
		}
		else
		{
#ifdef _DEBUG
#endif
		}
	}
}