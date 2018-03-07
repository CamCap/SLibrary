#include "stdafx.h"
#include "ssocket.h"
#include <cstdio>
#include "Log.h"
#include "GameMessage.h"

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
	SetAddr(addr);
}

void SSocket::SetAddr(int family, int port, u_short addr)
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

void SSession::CloseSocket()
{
	GameMessageManager::Instnace()->SendGameMessage(GM_DISCONNECTUSER, (DWORD)this, (DWORD)&m_recvOL, NULL);

	m_socket.CloseSocket();

	memset(&m_recvOL, 0, sizeof(IO_OVERLAPPED));
	memset(&m_sendOL, 0, sizeof(IO_OVERLAPPED));
}

void SSession::SetSessionInfo()
{

}

BOOL SSession::Recv()
{
	SOCKET_INVALID_CHECK(m_socket.socket);
	if (m_recvOL.io_type == IO_NONE) return false;


	DWORD dwRecvNumBytes = 0;
	DWORD dwFlags = 0;

	memset(&m_recvOL, 0, sizeof(WSAOVERLAPPED));
	m_recvOL.io_type = IO_RECV;

	int result = WSARecv(m_socket.socket, &m_recvOL.io_buff, 1, &dwRecvNumBytes, &dwFlags, (WSAOVERLAPPED*)&m_recvOL, NULL);

	if (result == SOCKET_ERROR)
		return FALSE;
	else
		return TRUE;
}

BOOL SSession::Send(char* buffer, int len, int& errcode)
{
	SOCKET_INVALID_CHECK(m_socket.socket);

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

	int result = WSASend(m_socket.socket, &m_sendOL.io_buff, 1, &dwsendbyte, dwflag, (WSAOVERLAPPED*)&m_sendOL, NULL);
	//LPWSAOVERLAPPED_COMPLETION_ROUTINE
	if (result == SOCKET_ERROR)
	{
		return FALSE;
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////


SPeer::SPeer()
	:m_vecSendPacket(0), m_vecStandPacket(), m_session()
{
}

SPeer::~SPeer()
{
	m_session.CloseSocket();
}


void SPeer::Send(BTZPacket* packet)
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

void SPeer::Recv()
{
	bool result = m_session.Recv();

	if ((result == FALSE) && (ERROR_IO_PENDING != WSAGetLastError()))
	{
		//에러제어
		ErrorHandle(__FUNCTION__);
	}
}


void SPeer::CheckSendPacket()
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
	else
	{
		m_session.SendComplete();
		SSession::SESSIONSTATEToString(SSession::SEND_COMPLETE);
	}
}

void SPeer::ReleaseSocket()
{
	m_session.CloseSocket();
}

void SPeer::InitPeer(SOCKET socket, SOCKADDR_IN addr, int userid)
{
	m_session.InitSession(socket, addr, recv_buffer, USER_BUFFER_SIZE);
	m_id = userid;
	Recv();
}

BOOL SPeer::RecvPacket(int size)
{
	BTZPacket* packet = NULL;

	CSLOCK(m_cs)
	{

		if (m_queue.Push(recv_buffer, size) == FALSE)
		{
			//UnLock();
			return FALSE;
		}

		while (true)
		{
			m_queue.Pop(packet);

			if (packet == NULL) break;

			GameMessageManager::Instnace()->SendGameMessage(GM_PKTRECEIVE, (DWORD)this, 0, (char*)packet);
			PacketProcess(packet);
		}
	}

	this->Recv();

	return TRUE;
}

void SPeer::PacketProcess(BTZPacket * packet)
{
	//	SE SESSIONSTATEToString(SEND_COMPLETE);
	SSession::SESSIONSTATEToString(SSession::RECV_COMPLETE);
	switch (packet->packet_id)
	{

	}
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
