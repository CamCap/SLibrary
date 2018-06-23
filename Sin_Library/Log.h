#pragma once
#include <map>
#include <fstream>
#include <istream>
#include <string>
#include "CriticalSection.h"
#include <iostream>

//Write만 있고 Read 기능은 없음

typedef std::string string;
typedef std::ofstream ofstream;
typedef std::ifstream ifstream;

class File
{
	ofstream out_stream;
	ifstream in_stream;

public:
	File() {}

	File(string filename) { this->operator()(filename); }

	~File()
	{
		if(out_stream.is_open()) out_stream.close();
		if (in_stream.is_open()) in_stream.close();
	}

	bool operator()(std::string filename, DWORD opt = std::ios::_Nocreate) {
		out_stream.open(filename, std::ios::app || opt);
		in_stream.open(filename, std::ios::in || opt);

		return (out_stream.is_open() || in_stream.is_open()) ? true : false;
	}

	template <typename _type>
	bool operator<<(_type str)
	{
		if (!out_stream.is_open())
		{
			return false;
		}

		out_stream << str;

		return true;
	}

	template<class _Elem, class _Traits>
	bool operator<<(std::ostream& os)
	{
		out_stream << std::endl;

		return true;
	}

	template <typename uchar = unsigned char>
	bool operator<<(uchar data[4][4])
	{
		if (!out_stream.is_open())
		{
			return false;
		}

		for(int i = 0 ; i < 4 ; i++)
			for(int j = 0 ; j < 4 ; j++)
				out_stream << (unsigned char)data[i][j];

		return true;
	}

	template <typename _type>
	bool operator>>(_type str)
	{
		if (!in_stream.is_open())
		{
			return false;
		}

		str = in_stream.get();
		return true;
	}


	template <typename _type>
	bool operator>>(std::string str)
	{
		if (!in_stream.is_open())
		{
			return false;
		}

		str = in_stream.getline();
		return true;
	}

	template <typename uchar = unsigned char>
	bool operator>>(uchar data[4][4])
	{
		if (!in_stream.is_open())
		{
			return false;
		}
		
		int i = 0, j = 0;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (in_stream.eof()) break;

				data[i][j] = in_stream.get();
			}
		}

		return true;
	}
};


class Log
{
public:
	typedef std::map<string, File*> FILE_MAP;

public:
	static Log* Instance();

public:
	void WriteLog(string filename, string log);
	void WriteLog(string filename, int errorcode);

public:
	Log();
	~Log();

private:

	//	void Lock() { EnterCriticalSection(&m_cs); }
	//	void UnLock() { LeaveCriticalSection(&m_cs); }

private:
	static Log* m_instance;
	FILE_MAP m_logmap;
	//CRITICAL_SECTION m_cs;
	SCriticalSection m_cs;
};

#define SOCKET_ERROR_LOG_CODE Log::Instance()->WriteLog("Log_File", WSAGetLastError())
//#define SOCKET_ERROR_LOG_CODE(FILE) Log::Instance()->WriteLog(FILE, WSAGetLastError());
//#define ERROR_LOG(FILE,STRING) Log::Instance()->WriteLog(FILE, STRING);
#define ERROR_LOG(STRING) Log::Instance()->WriteLog("Log_File", STRING)


