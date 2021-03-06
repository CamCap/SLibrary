#pragma once
#include <functional>
#include "header.h"
#include <WS2tcpip.h>
#include <WinSock2.h>
#include "Packet.h"
#include "CriticalSection.h"

typedef void(*PacketProcess)(BTZPacket*);

class SSocket
{
public:
	//   friend class Session;
	SSocket();
	~SSocket();

	BOOL CreateWSASocket(int af = AF_INET, int type = SOCK_STREAM, int protocol = 0, LPWSAPROTOCOL_INFOW lpPorotocolInfo = NULL, GROUP g = 0, DWORD dwFlags = WSA_FLAG_OVERLAPPED);

	void InitSocket(SOCKET socket, SOCKADDR_IN addr);



	//SOCKET GetSocket() { return m_socket; }
	//SOCKADDR_IN GetAddr() { return m_addr; }

	//void SetSocket(SOCKET socket) { m_socket = socket; }
	void SetAddr(SOCKADDR_IN sockaddr);
	void SetAddr(int family, int port, const char* ip);

	void ReleaseSocket() { m_socket = INVALID_SOCKET; }

	void CloseSocket() { closesocket(m_socket); m_socket = INVALID_SOCKET; }

public:
	SOCKET m_socket;
	SOCKADDR_IN m_addr;
};

/*

비동기 세션 클래스
overlapped랑 wsabuff를 추가적으로 가지자.
SSession을 멤버변수로 가지는 user 클래스를 만들어서  유저 정보도 가지자.

*/

#define IO_RECV 1
#define IO_SEND_STAND 2
#define IO_SENDING 3
//#define IO_COLSE 4
#define IO_NONE 0


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
		ERROR_SOCKET_INVALID,
		RECV_COMPLETE,
		SEND_COMPLETE
		)

		friend class SPeer;

public:
	SSession();				//OL 할당
	~SSession();

	void InitSession(SOCKET socket, SOCKADDR_IN addr, char* recvbuffer, int recvlen);
	void ReleaseSession();

	BOOL Recv();

	BOOL Send(char* buffer, int len, int& errorcode);
	void SendComplete() { m_recvOL.io_type = IO_SEND_STAND; }

private:
	void SetSessionInfo(); // ip, port, cntError초기화


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

class SPeer
{
public:
	DECLARE_ENUM(
	SUSERSTATE,
		ERROR_SENDPACKET_NULL
		);

public:
	explicit SPeer();
	~SPeer();

public:
	void Send(BTZPacket* packet);
	void Send(char* data);
	void CheckSendPacket(); // GQCS에서 Send 완료 되면 호출할 것
	void ReleaseSocket();
	bool InitPeer(SOCKET socket, SOCKADDR_IN addr, int userid);

	BOOL RecvPacket(int size);

	int GetID() { return m_id; }
	DWORD GetOLType() { return m_session.m_recvOL.io_type; }

	int Recv();
#ifdef _DLLWraper 
	void InitPeer(SOCKET socket, SOCKADDR_IN addr) {
		m_session.InitSession(socket, addr, recv_buffer, USER_BUFFER_SIZE);
	}

#endif

protected:
	void ErrorHandle(const char* function);
	//	virtual void PacketProcess(BTZPacket* packet);


public:

	//int GetID() { return m_id; }

#ifdef _DLLWraper
	typedef void(*process)(BTZPacket*);
	//process m_packetProcess;

	std::function<void(BTZPacket*)> m_packetProcess;
#else
	std::function<void(BTZPacket*)> m_packetProcess;
#endif
protected:
	SSession m_session;
	SCircleQueue m_queue;
	char recv_buffer[USER_BUFFER_SIZE];

	SPacketContainer m_vecSendPacket; // 보내는 패킷 벡터

	SCriticalSection m_cs;

	int m_id;
};

//////////////////////////////////////////////////////////////

class SServer
	:public SPeer
{
public:

public:
	void SetName(std::string name);

	std::string GetName() { return m_name; }
	void OnPingCheck(DWORD tick);
//	void SetPingCheckTime(DWORD tick) { m_pingCheckTime = tick; }

public:
	SServer();
	virtual ~SServer();

private:
	std::string m_name;

	DWORD m_tickPing;
};


////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////


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
