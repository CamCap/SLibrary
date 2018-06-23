#pragma once
#include "ssocket.h"
#include "Log.h"
//� ������ ������ ������ �����ؾ߸� �۵��ϰ� ����� �ʹ�.
//������ ������ connect���� ���ϸ� ������ ������ ���� �ϰ� ���μ����� ���߸��� �ʹ�.
//sdlg�� �ɼ�
////sdlg�� smasterserver�� ����ϸ�, �˻��� sdlg�� ����

// Ư���� ����� ������, �ܼ��� ������ ���� Ŭ����

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

