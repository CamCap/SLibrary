#pragma once
#include "CriticalSection.h"


struct DlgOption
{
	DWORD MsgHwnd;
	DWORD TimerHwnd;
	HWND dlgHwnd;
	HINSTANCE hInstance;
	int resID;
	DLGPROC dlgProc;
};

class SDlg
{
public:
	void OnInitDlg(DlgOption* option);
	void StartDlg(HINSTANCE hInstance, int resID, HWND parentHwnd);

	BOOL OnExit(HWND hWnd);

	HWND GetHWND() { return m_hWnd; }

	void SetMessage(const char *s);
	void SetRunTime();

public:
	SDlg();
	~SDlg();

protected:
	HWND m_hWnd;
	HWND m_hWndMsg;				// �޼����� ����� ����Ʈ �ڽ� �ڵ�.
	HWND m_hTimer;				//���� ���� �ð� ����Ʈ�ڽ� �ڵ�

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

public:
};

