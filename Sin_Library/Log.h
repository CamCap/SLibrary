#pragma once
#include <map>
#include <fstream>
#include <string>
#include <ctime>
#include "CriticalSection.h"

//Write만 있고 Read 기능은 없음

class File;


typedef std::string string;
typedef std::ofstream ofstream;
typedef std::map<string, File*> FILE_MAP;

class File
{
public:
	bool FileOpen(string filename);
	void FileClose();
	bool FileWrite(string write);
	bool FileRead(string read); // 이건 만들 생각 없음

public:
	File();
	File(string filename);
	~File();

private:

	bool IsFileOpen();

private:
	ofstream m_file;
};


//싱글톤 패턴 사용
//텍스트에 로그를 저장함.
//아 잠시만 분류별로 파일을 만들어서 로그를 남기고 싶은데 어떻게 하지??
//클래스 별로 나누는 것말고 다른 방법이 없을까?
//싱글톤이 이게 맞던가? 대충 맞는 것같긴한데 에러가 뜰지도...?

class Log
{
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

////////////////////////////////////////////////////


File::File()
{

}

File::File(string filename)
{
	FileOpen(filename);
}

File::~File()
{
	FileClose();
}

bool File::FileOpen(string filename)
{
	filename += ".txt";
	m_file.open(filename, std::ios::app);

	if (!IsFileOpen()) {
		return false;
	}

	return true;
}

void File::FileClose()
{
	if (!IsFileOpen())
		return;

	m_file.close();
}

bool File::FileWrite(string write)
{
	if (!IsFileOpen())
	{
		return false;
	}

	m_file << write << std::endl;

	return true;
}

//미구현
bool File::FileRead(string read)
{
	//std::cout << "�̱����Դϴ�. �����ڰ� ���� �ʿ䰡 ������, �׳� �־ �޼����Դϴ�." << std::endl;

	return true;
}

bool File::IsFileOpen()
{
	return (m_file.fail() || m_file.bad()) ? false : true;
}

///////////////////////////////

Log* Log::m_instance = NULL;

const std::string currentDateTime() {
	time_t     now = time(NULL);
	struct tm  tstruct;
	char       buf[80];
	localtime_s(&tstruct, &now);

	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
	//	return std::string(logtime);
}

////////////////////////


Log::Log()
{
	//InitializeCriticalSection(&m_cs);
}


Log::~Log()
{
	//DeleteCriticalSection(&m_cs);

	//	std::map<string, File*>::iterator it = m_logmap.begin();

	FILE_MAP::iterator it = m_logmap.begin();

	for (it; it != m_logmap.end(); it)
	{
		it->second->FileClose();
		delete it->second;
		m_logmap.erase(it);
	}
}

Log* Log::Instance()
{
	if (m_instance == NULL)
		m_instance = new Log();

	return m_instance;
}

void Log::WriteLog(string filename, string log)
{
	//Lock();
	if (m_logmap.find(filename) == m_logmap.end())
	{
		CSLOCK(m_cs)
		{
			File* file = new File(filename);
			m_logmap.insert(FILE_MAP::value_type(filename, file));
		}
	}
	string str(currentDateTime());
	str += "      ";
	str += log;
	//UnLock();

	m_logmap[filename]->FileWrite(str);
}

void Log::WriteLog(string filename, int errorcdoe)
{
	char errorlog[100];

	sprintf_s(errorlog, "Socket Error Code : %d", errorcdoe);

	WriteLog(filename, errorlog);
}

//////////////////////////////////////////////////////////////


#define SOCKET_ERROR_LOG_CODE Log::Instance()->WriteLog("Log_File", WSAGetLastError())
//#define SOCKET_ERROR_LOG_CODE(FILE) Log::Instance()->WriteLog(FILE, WSAGetLastError());
//#define ERROR_LOG(FILE,STRING) Log::Instance()->WriteLog(FILE, STRING);
#define ERROR_LOG(STRING) Log::Instance()->WriteLog("Log_File", STRING)
