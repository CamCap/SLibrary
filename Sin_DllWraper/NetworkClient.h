#pragma once

#include "ssocket.h"
using namespace System;
using namespace System::Collections::Generic;
using namespace System::Text;
//using namespace System::Threading::Tasks;
using namespace System::Runtime::InteropServices;
//using namespace System::Globalization;



#include <vcclr.h>  
#include <vector>

#define GET_VARIABLE_NAME(Variable) (#Variable)
//#define _DLLWraper

namespace SLibrary_Warper
{
	public  ref class NetworkDefine
	{
	public:
		literal short pkt_size = PKT_BASIC_SIZE;
	};

	public ref class IResultLevel
	{
	public:
		literal short IMsg = 0,
		IErr = 1,
		ICri = 2,
		INull = 3;
	};

	public ref class ResultInfo
	{
	public:
		short m_Level;
		String^ msg;

		ResultInfo(char* message) {
			msg = gcnew String(message);
		}


		ResultInfo(short lev, char* message) :m_Level(lev), msg(gcnew String(message)) {
		}

		ResultInfo(String^ message) : msg(message) {
		}

		!ResultInfo() {
		}	

		~ResultInfo() {
		}
	};


	public ref class Peer
	{
	public:
		delegate void PacketProcess(List<TCHAR>^, int);
		delegate void ErrorHandle(ResultInfo^);
		delegate void WarperProcess(BTZPacket*);

	public:
							Peer();
							!Peer();
		void				 CreatePeer();
		void				Disconnect();
		void				Connect(char* ip, int port);
		void				WraperProcess(BTZPacket* packet);
		int					Recv();
		void				Send(System::IO::MemoryStream^ ms);

	private:
		ResultInfo^			ResultInfoMessageSet(std::string function, short level, void* errcode);
	private:
		SPeer*				m_peer;
	class DelegateProcess*	process;
		ErrorHandle^		m_ErrorHandle;
		PacketProcess^		m_PacketProcess;
	};


	class DelegateProcess
	{
	public:
		DelegateProcess();
		~DelegateProcess();

		gcroot<Peer^> m_Out;

		void Process(BTZPacket* packet);
	};




	//template<typename T>
	//inline void Peer::Send(T data)
	//{
	//	System::IO::MemoryStream^ ms = gcnew System::IO::MemoryStream();

	//	IntPtr ptr = Marshal.AllocHGlobal(Marshal.SizeOf(data));

	//	try
	//	{
	//		Marshal.StructureToPtr(data, ptr, false);

	//		T obj = (T)Marshal.PtrToStructure(ptr, typeof(T));

	//		ms.Write(obj, 0, Marshal.SizeOf(data));

	//		m_peer->Send(static_cast<char*>(obj.ToPointer()));
	//	}
	//	finally
	//	{
	//		Marshal.FreeHGlobal(ptr);
	//		ms.Close();
	//	}
	//}
};

