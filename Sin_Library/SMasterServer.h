#pragma once
#include "ssocket.h"
#include "Log.h"
//어떤 서버가 마스터 서버에 접속해야만 작동하게 만들고 싶다.
//마스터 서버에 connect하지 못하면 서버가 에러를 띄우게 하고 프로세스를 꺼뜨리고 싶다.
//sdlg의 옵션
////sdlg가 smasterserver를 사용하면, 검사후 sdlg를 종료

// 특별한 기능은 없지만, 단순히 구분을 위한 클래스

class SMasterServer
{
public:
	SMasterServer();
	~SMasterServer();

	friend class SMasterServerBehavair;

private:
	SServer m_server;

	bool ConnectToMasterServer(const char* ip, int port)
	{
		SOCKET server_socket;
		SOCKADDR_IN sockaddr_in;

		server_socket = socket(AF_INET, SOCK_STREAM, 0);

		if (server_socket == INVALID_SOCKET)
		{
			SOCKET_ERROR_LOG_CODE;
			return WSAGetLastError();
		}

		ZeroMemory(&sockaddr_in, sizeof(sockaddr));
		sockaddr_in.sin_family = AF_INET;
		sockaddr_in.sin_port = htons(port);
		//sockaddr.sin_addr.s_addr = inet_pton("127.0.0.1");
		inet_pton(AF_INET, ip, &sockaddr_in.sin_addr);

		int result = SocketTool::Connect(server_socket, sockaddr_in);

		if (result == true) m_server.InitPeer(server_socket, sockaddr_in, 154321);

		return result;
	}
};

