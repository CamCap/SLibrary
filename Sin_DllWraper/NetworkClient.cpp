#include "stdafx.h"
#include "NetworkClient.h"
#include <array>
#include <stdlib.h>  
#include <string.h>  
//#include "msclr\marshal_cppstd.h"
//#include <msclr\marshal_windows.h>
//#include <msclr\marshal_cppstd.h>
#include <msclr\marshal.h>  

using namespace System;
using namespace System::Configuration;
using namespace msclr::interop;

SLibrary_Warper::Peer::Peer()
{
}

SLibrary_Warper::Peer::!Peer()
{
	if (m_peer != NULL) delete m_peer;
	if (process != NULL) process = NULL;
	WSACleanup();
}

void SLibrary_Warper::Peer::CreatePeer()
{
	if (m_peer == NULL) {
		m_peer = new SPeer();

		process = new DelegateProcess();
		process->m_Out = this;

		//process->m_In = process->Process;
		m_peer->m_packetProcess = std::bind(&SLibrary_Warper::DelegateProcess::Process, std::ref(*process), std::placeholders::_1);
		//m_peer->m_packetProcess = std::bind(&interior_ptr<SLibrary_Warper::DelegateProcess::Process>)
	}

	WSADATA wsaData;

	try {
		//if((m_ErrorHandle = ErrorHandle)->Equals(NULL))
		//	throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::INull, (void*)GET_VARIABLE_NAME(ErrorHandle));

		//if((m_PacketProcess = process)->Equals(NULL))
		//	throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::INull, (void*)GET_VARIABLE_NAME(process));

		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
			throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::IErr, (void*)WSAGetLastError());
	}
	catch (ResultInfo^ exception)
	{
		m_ErrorHandle(exception);
	}
	
	//throw gcnew System::NotImplementedException();
}

void SLibrary_Warper::Peer::Disconnect()
{
	if (m_peer != NULL)
	{
		m_peer->ReleaseSocket();
	}
}

void SLibrary_Warper::Peer::Connect(char * ip, int port)
{
	try
	{
		if (m_peer == NULL) throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::INull, (void*)"Not Calling CreatePeer");

		SOCKET socket;
		SOCKADDR_IN addr;

		if ((socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED)) == INVALID_SOCKET)
			throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::IErr, (void*)WSAGetLastError());

		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		auto ret = inet_pton(AF_INET, ip, (void*)&addr.sin_addr.s_addr);

		if (m_peer->InitPeer(socket, addr, 0) == false)
			throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::IErr, (void*)WSAGetLastError());
	}
	catch (ResultInfo^ exception)
	{
		m_ErrorHandle(exception);
	}
}

void SLibrary_Warper::Peer::WraperProcess(BTZPacket * packet)
{
	char* bytes = new char[packet->packet_size];	
	memcpy(bytes, packet, packet->packet_size);

	String^ str = marshal_as<String^>(bytes);	
	List<TCHAR>^ data = gcnew List<TCHAR>(packet->packet_size);

	data->CopyTo(str->ToCharArray());
	
	m_PacketProcess(data, packet->packet_size);

	delete[] bytes;
}

int SLibrary_Warper::Peer::Recv()
{
	int result = 0;
	try {
		if ((result = m_peer->Recv()) < 0) throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::IErr, (void*)WSAGetLastError());

		if (m_peer->RecvPacket(result) == false) throw ResultInfoMessageSet(__FUNCTION__, IResultLevel::ICri, (void*)"Peer CircleQueue is Wrong");
	}
	catch (ResultInfo^ exception)
	{
		m_ErrorHandle(exception);
	}

	return result;
}

void SLibrary_Warper::Peer::Send(System::IO::MemoryStream^ ms)
{
	String^ str = ms->ToString();
	IntPtr ptr = Marshal::StringToHGlobalAnsi(str);
	try
	{
		m_peer->Send(static_cast<char*>(ptr.ToPointer()));
	}
	finally{
		Marshal::FreeHGlobal(ptr);
	}
	//m_peer->Send(marshal_as<char*>(str));
}

SLibrary_Warper::ResultInfo ^ SLibrary_Warper::Peer::ResultInfoMessageSet(std::string function, short level, void * errcode)
{
	char message[1024];

	switch (level)
	{
	case IResultLevel::IMsg:
		wsprintfA(message, "[%s]:: %s \n", function, (*(char*)errcode));
		break;
	case IResultLevel::ICri:
		wsprintfA(message, "[%s]:: \n");
		break;
	case IResultLevel::INull:
		wsprintfA(message, "[%s]:: %s is NULL \n", function, (*(char*)errcode));
		break;
	case IResultLevel::IErr:
		wsprintfA(message, "[%s]::GetLastErrorCode : %d \n", function, (*(int*)errcode));
		break;
	}

	return gcnew ResultInfo(level, message);
}

SLibrary_Warper::DelegateProcess::DelegateProcess()
{

}

SLibrary_Warper::DelegateProcess::~DelegateProcess()
{
	m_Out = nullptr;

}

void SLibrary_Warper::DelegateProcess::Process(BTZPacket * packet)
{
	((SLibrary_Warper::Peer ^)m_Out)->WraperProcess(packet);
}