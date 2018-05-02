#pragma once
#include "CriticalSection.h"
#include <map>
#include <cstdarg>
#include <functional>
#include <string>


class SDlg
{
	struct DlgOption
	{
		typedef std::pair<DWORD, HWND> HWND_PAIR;
		typedef std::map<std::string, HWND_PAIR> MAP_HWND;
		friend class SDlg;

		HWND dlgHwnd;
		HINSTANCE hInstance;
		DLGPROC dlgProc;
		DWORD dlgResId;

		MAP_HWND m_map;

		DlgOption(HWND hWnd, HINSTANCE hi, DWORD dlgId, DLGPROC proc)
			:dlgHwnd(hWnd), hInstance(hi), dlgResId(dlgId), dlgProc(proc)
		{
		}

	private:

		DlgOption() {}
	};

public:
	void OnInitDlg();
	void StartDlg(DlgOption option);
	BOOL OnExit(HWND hWnd);
/*
	HWND GetHWND() { return m_hWnd; }
	*/

	void SetMessage(const char *s);
	void SetRunTime();

public:
	SDlg();
	~SDlg();

protected:
	DWORD m_Day;
	DWORD m_Hour;
	DWORD m_Minute;
	DWORD m_RealDay;
	DWORD m_RealHour;
	DWORD m_RealMinute;
	DWORD m_WriteTime;
	DWORD m_TimeCount;
	char m_RunTime[15];

	SCriticalSection m_cs;
	
	DlgOption m_opt;
};

