#pragma once

#include "header.h"
#include <WinSock2.h>
#include "Packet.h"
#include "CriticalSection.h"

class SSocket
{
public:
	//   friend class Session;
	SSocket();
	~SSocket();

	BOOL CreateWSASocket(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0, LPWSAPROTOCOL_INFOW lpPorotocolInfo = NULL, GROUP g = 0, DWORD dwFlags = WSA_FLAG_OVERLAPPED);
	
	void InitSocket(SOCKET socket, SOCKADDR_IN addr);

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

/*

�񵿱� ���� Ŭ����
overlapped�� wsabuff�� �߰������� ������.
SSession�� ��������� ������ user Ŭ������ ����  ���� ������ ������.

*/

#define IO_RECV 1
#define IO_SEND_STAND 2
#define IO_SENDING 3
#define IO_NONE 0


struct IO_OVERLAPPED :public WSAOVERLAPPED
{
	DWORD io_type;
	WSABUF io_buff;
	//    char io_buff[MAX_BUFF_SIZE];
};

//���� �����̳ʵ� ���� �и���Ű��.

class SSession
{
public:
	DECLARE_ENUM(SESSIONSTATE,
		ERROR_SENDING,
		ERROR_SOCKET_INVALID,
		RECV_COMPLETE,
		SEND_COMPLETE
	)

public:
	SSession();				//OL �Ҵ�
	~SSession();

	void InitSession(SOCKET socket, SOCKADDR_IN addr, char* recvbuffer, int recvlen);
	void CloseSocket();

	BOOL Recv();	

	BOOL Send(char* buffer, int len, int& errorcode);
	void SendComplete() { m_recvOL.io_type = IO_SEND_STAND; }

private:
	void SetSessionInfo(); // ip, port, cntError�ʱ�ȭ

private:
	SSocket m_socket;
	IO_OVERLAPPED m_recvOL;
	IO_OVERLAPPED m_sendOL;

	// SCriticalSection m_cs;

	char m_ip[16];
	unsigned short m_port;
	int m_cntError;
};

////////////////////////////////////////////////////////////////////////////

#define USER_BUFFER_SIZE 1024

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
	void Send(BTZPacket* packet);
	void CheckSendPacket(); // GQCS���� Send �Ϸ� �Ǹ� ȣ���� ��
	void ReleaseSocket();
	void InitUser(SOCKET socket, SOCKADDR_IN addr, int userid);

	BOOL RecvPacket(int size);
private:
	void ErrorHandle(const char* function);
	virtual void PacketProcess(BTZPacket* packet);
	void Recv();

private:
	SSession m_session;
	SCircleQueue m_queue;
	char recv_buffer[USER_BUFFER_SIZE];


	SPacketContainer m_vecSendPacket; // ������ ��Ŷ ����
	SPacketContainer m_vecStandPacket; // ��������� ��Ŷ ����

	SCriticalSection m_cs;

	std::string m_id;
	bool m_sex;		//false�� ����, true�� ����
	int m_userid;
};



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

	return ::bind(sock, (SOCKADDR*)&addr, sizeof(addr));
}

int SocketTool::Connect(IN SOCKET socket, IN SOCKADDR_IN addr)
{
	SOCKET_INVALID_CHECK(socket);

	return ::connect(socket, (SOCKADDR*)&addr, sizeof(addr));
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

	return ::listen(socket, len);
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
