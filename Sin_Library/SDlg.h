#pragma once
#include "CriticalSection.h"


struct DlgOption
{
	HWND MsgHwnd;
	HWND TimerHwnd;
	HWND dlgHwnd;
	HINSTANCE hInstance;
	int resID;
	DLGPROC dlgProc;
};

class SDlg
{
public:
	void OnInitDlg();
	void StartDlg(DlgOption* option);

	BOOL OnExit(HWND hWnd);
/*
	HWND GetHWND() { return m_hWnd; }*/

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
	
	DlgOption m_option;

public:
};

