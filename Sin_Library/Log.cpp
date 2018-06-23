#include "stdafx.h"
#include "Log.h"
#include <ctime>
//
//File::File()
//{
//
//}
//
//File::File(string filename)
//{
//	FileOpen(filename);
//}
//
//File::~File()
//{
//	FileClose();
//}
//
//bool File::FileOpen(string filename)
//{
//	filename += ".txt";
//	m_file(filename);
//
//	return true;
//}
//
//void File::FileClose()
//{
////	m_file.close();
//}
//
//bool File::FileWrite(string write)
//{
//	return m_file << write;
//}
////
//////미구현
////bool File::FileRead(string read)
////{
////	return m_file >> read;
////}
//
/////////////////////////////////

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
	//	it->second->FileClose();
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

	*m_logmap[filename] << str;
}

void Log::WriteLog(string filename, int errorcdoe)
{
	char errorlog[100];

	sprintf_s(errorlog, "Socket Error Code : %d", errorcdoe);

	WriteLog(filename, errorlog);
}

//////////////////////////////////////////////////////////////


