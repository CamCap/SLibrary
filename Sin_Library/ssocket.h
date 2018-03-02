#pragma once

#include <cstdio>
#include "header.h"
#include <WinSock2.h>
#include "Packet.h"
#include "Log.h"

//class SSession;


class SSocket
{
public:

	//   friend class Session;

	SSocket();
	~SSocket();

	BOOL CreateWSASocket(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0, LPWSAPROTOCOL_INFOW lpPorotocolInfo = NULL, GROUP g = 0, DWORD dwFlags = WSA_FLAG_OVERLAPPED);
	
	BOOL InitSocket(SOCKET socket, SOCKADDR_IN addr);

	G_PROPERTY(GetSocket) SOCKET socket;
	G_PROPERTY(GetAddr) SOCKADDR_IN addr;

	SOCKET GetSocket() { return m_socket; }
	SOCKADDR_IN GetAddr() { return m_addr; }

	void SetAddr(int family, int addr, u_short port);
	void SetAddr(SOCKADDR_IN sockaddr);

	void ReleaseSocket() { m_socket = INVALID_SOCKET; }

	void CloseSocket() { closesocket(m_socket); m_socket = INVALID_SOCKET; }

private:

private:

	SOCKET m_socket;
	SOCKADDR_IN m_addr;
};

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

BOOL SSocket::InitSocket(SOCKET socket, SOCKADDR_IN addr)
{
	m_socket = socket;
	SetAddr(addr);
}

void SSocket::SetAddr(int family, int addr, u_short port)
{
	memset(&m_addr, 0, sizeof(m_addr));

	m_addr.sin_family = family;
	m_addr.sin_port = htons(port);
	m_addr.sin_addr.s_addr = htonl(addr);
}

void SSocket::SetAddr(SOCKADDR_IN sockaddr)
{
	memcpy(&m_addr, &sockaddr, sizeof(SOCKADDR_IN));
}

////////////////////////////////////////////////////////////////////////////

/*

비동기 세션 클래스
overlapped랑 wsabuff를 추가적으로 가지자.
SSession을 멤버변수로 가지는 user 클래스를 만들어서  유저 정보도 가지자.

*/

#define IO_RECV 1
#define IO_SEND_STAND 2
#define IO_SENDING 3
#define IO_NONE 0

#define MAX_BUFF_SIZE 1024

struct IO_OVERLAPPED :public WSAOVERLAPPED
{
	DWORD io_type;
	WSABUF io_buff;
	//    char io_buff[MAX_BUFF_SIZE];
};

//세션 컨테이너도 만들어서 분리시키자.

class SSession
{
public:
	DECLARE_ENUM(SESSIONSTATE,
	ERROR_SENDING,
		ERROR_SOCKET_INVALID
		)

public:
	SSession();				//OL 할당
	~SSession();

	void InitSession(SOCKET socket, SOCKADDR_IN addr, char* recvbuffer, int recvlen);
	void CloseSocket();

	BOOL Recv();
	BOOL Send(char* buffer, int len, int& errorcode);


private:
	void SetSessionInfo(); // ip, port, cntError초기화

private:
	SSocket m_socket;
	IO_OVERLAPPED* m_recvOL;
	IO_OVERLAPPED* m_sendOL;

	// SCriticalSection m_cs;

	char m_ip[16];
	unsigned short m_port;
	int m_cntError;
};

SSession::SSession()
{
	m_recvOL = new IO_OVERLAPPED();
	m_sendOL = new IO_OVERLAPPED();

	m_recvOL->io_type = IO_RECV;
	m_recvOL->io_type = IO_SEND_STAND;
}

SSession::~SSession()
{
	SAFE_DELETE(m_recvOL);
	SAFE_DELETE(m_sendOL);
}


void SSession::InitSession(SOCKET socket, SOCKADDR_IN addr, char* recvbuffer, int recvlen)
{
	
	m_recvOL->io_buff.buf = recvbuffer;
	m_recvOL->io_buff.len = recvlen;

	m_socket.InitSocket(socket, addr);
}

void SSession::CloseSocket()
{
	m_socket.CloseSocket();

	memset(m_recvOL, 0, sizeof(IO_OVERLAPPED));
	memset(m_sendOL, 0, sizeof(IO_OVERLAPPED));
}

void SSession::SetSessionInfo()
{

}

BOOL SSession::Recv()
{
	SOCKET_INVALID_CHECK(m_socket.socket);


	DWORD    dwRecvNumBytes = 0;
	DWORD    dwFlags = 0;

	memset(m_recvOL, 0, sizeof(WSAOVERLAPPED));
	m_recvOL->io_type = IO_RECV;

	int result = WSARecv(m_socket.socket, &m_recvOL->io_buff, 1, &dwRecvNumBytes, &dwFlags, (WSAOVERLAPPED*)&m_recvOL, NULL);

	if (result == SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;
}

BOOL SSession::Send(char* buffer, int len, int& errcode)
{
	SOCKET_INVALID_CHECK(m_socket.socket);

	if (m_sendOL->io_type == IO_SENDING) {
		errcode = SESSIONSTATE::ERROR_SENDING;
		return FALSE;
	}

	DWORD dwflag = 0;
	DWORD dwsendbyte = 0;

	m_sendOL->io_buff.buf = buffer;
	m_sendOL->io_buff.len = len;

	memset(m_sendOL, 0, sizeof(WSAOVERLAPPED));
	m_sendOL->io_type = IO_SENDING;

	int result = WSASend(m_socket.socket, &m_sendOL->io_buff, 1, &dwsendbyte, dwflag, (WSAOVERLAPPED*)&m_sendOL, NULL);
	//LPWSAOVERLAPPED_COMPLETION_ROUTINE
	if (result == SOCKET_ERROR)
	{
		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////

#define USER_BUFFER_SIZE 10000

class SUser
{
public:
	DECLARE_ENUM(
	SUSERSTATE,

		ERROR_SENDPACKET_NULL
		);

public:
	explicit SUser();
	~SUser();

public:
	void Recv();

	void Send(BTZPacket* packet);
	void CheckSendPacket(); // GQCS에서 Send 완료 되면 호출할 것
	void ReleaseSocket() { m_session.CloseSocket(); }
	void InitUser(SOCKET socket, SOCKADDR_IN addr, int userid);

	BOOL RecvPacket(int size);
private:
	void ErrorHandle(const char* function);
	void PacketProcess(BTZPacket* packet);

private:
	SSession m_session;
	SCircleQueue<char> m_queue;
	char recv_buffer[USER_BUFFER_SIZE];


	SPacketContainer m_vecSendPacket; // 보내는 패킷 벡터
	SPacketContainer m_vecStandPacket; // 사용대기중인 패킷 벡터

	std::string m_id;
	bool m_sex;		//false면 남자, true면 여자
	int m_userid;
};


SUser::SUser()
	:m_vecSendPacket(0), m_vecStandPacket(), m_session()
{
}

SUser::~SUser()
{
	m_session.CloseSocket();
}

void SUser::Recv()
{
	BOOL result = m_session.Recv();

	if ((result == FALSE) && (ERROR_IO_PENDING != WSAGetLastError()))
	{
		//에러제어
		ErrorHandle(__FUNCTION__);
	}
}

void SUser::Send(BTZPacket* packet)
{
	BTZPacket* sendpacket = NULL;
	m_vecStandPacket.Pop(sendpacket);

	if (sendpacket == NULL)
	{
		SUSERSTATEToString(ERROR_SENDPACKET_NULL);
		return;
	}
	memcpy(sendpacket, packet, sizeof(*packet));

	m_vecSendPacket.Push(sendpacket);

	//m_session.Send((char*)sendpacket, sendpacket->packet_size);
	CheckSendPacket();
}


void SUser::CheckSendPacket()
{
	BTZPacket* sendpacket = NULL;
	m_vecSendPacket.Pop(sendpacket);

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
}

void SUser::InitUser(SOCKET socket, SOCKADDR_IN addr, int userid)
{
	m_session.InitSession(socket, addr, recv_buffer, USER_BUFFER_SIZE);
	m_userid = userid;
}

BOOL SUser::RecvPacket(int size)
{
	BTZPacket* packet = NULL;

	if (m_queue.Push(recv_buffer, size) == FALSE)
	{
		//UnLock();
		return FALSE;
	}

	while (true)
	{
		m_queue.Pop((char*)packet);

		if (packet == NULL) break;

		GameMessageManager::Instnace()->SendGameMessage(GM_PKTRECEIVE, (DWORD)this, 0, (char*)packet);
		PacketProcess(packet);
	}

	//UnLock();

	Recv();

	return TRUE;
}

void SUser::PacketProcess(BTZPacket * packet)
{
}


void SUser::ErrorHandle(const char* function)
{
#ifdef _DEBUG
	char buff[100];
	int ret = WSAGetLastError();
	wsprintfA(buff, "[ERROR : %d] 에러 발생[%s] \n", ret, function);
	OutputDebugStringA(buff);
#else
	SOCKET_ERROR_LOG_CODE
#endif
}

////////////////////////////////////////////////////////////////////////////


namespace SocketTool
{
	static int Bind(IN SOCKET sock, IN SOCKADDR_IN addr);
	static int Connect(IN SOCKET socket, IN SOCKADDR_IN addr);
	static SOCKET Accept(IN SOCKET socket, IN SOCKADDR_IN& addr);
	static int Listen(IN SOCKET socket, IN short len);
	static int Recv(IN SOCKET socket, OUT int& recv_len, OUT SOCKET_CALLBACK callback = NULL);
	static int Send(IN SOCKET socket, OUT int& send_len, OUT SOCKET_CALLBACK callback = NULL);
};

int SocketTool::Bind(IN SOCKET sock, IN SOCKADDR_IN addr)
{
	SOCKET_INVALID_CHECK(sock);

	return (::bind(sock, (SOCKADDR*)&addr, sizeof(addr)) != INVALID_SOCKET) ? TRUE : WSAGetLastError();
}

int SocketTool::Connect(IN SOCKET socket, IN SOCKADDR_IN addr)
{
	SOCKET_INVALID_CHECK(socket);

	return (::connect(socket, (SOCKADDR*)&addr, sizeof(addr)) != INVALID_SOCKET) ? TRUE : WSAGetLastError();
}

SOCKET SocketTool::Accept(IN SOCKET socket, IN SOCKADDR_IN& addr)
{
	SOCKET_INVALID_CHECK(socket);

	int addr_size = sizeof(addr);

	SOCKET accept_socket = ::accept(socket, (SOCKADDR*)&addr, &addr_size);

	//if(accept_socket == INVALID_SOCKET) return WSAGetLastError();

	return accept_socket;
}

int SocketTool::Listen(IN SOCKET socket, IN short len)
{
	SOCKET_INVALID_CHECK(socket);

	return (::listen(socket, len) != INVALID_SOCKET) ? true : WSAGetLastError();
}

//
//int SocketTool::Recv(IN SOCKET socket, OUT int& recv_len, OUT SOCKET_CALLBACK callback)
//{
//	SOCKET_INVALID_CHECK(socket);
//
//	//WSARecv(socket,, );
//
//	return 0;
//}
//
//int SocketTool::Send(IN SOCKET socket, OUT int& send_len, OUT SOCKET_CALLBACK callback)
//{
//	return 0;
//}
