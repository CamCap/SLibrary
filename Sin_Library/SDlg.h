#pragma once
#include "CriticalSection.h"
#include <functional>
#include "resource.h"

class SDlgBehavair
{
public:
	virtual void Behavair() = 0;
};

class TimerBehavair : public SDlgBehavair
{
	HWND m_hWndList;
	HWND m_hWndEdit;

	DWORD m_Day;
	DWORD m_Hour;
	DWORD m_Minute;
	DWORD m_RealDay;
	DWORD m_RealHour;
	DWORD m_RealMinute;
	DWORD m_WriteTime;
	DWORD m_TimeCount;
	char m_RunTime[15];

public:
	virtual void Behavair();

	void SetTimer(HWND hWndList, HWND hWndEdit, DWORD time);

	TimerBehavair() {}
	~TimerBehavair() {}
};


class SDlg
{
public:
	struct DlgOption
	{
	public:
		friend class SDlg;


		DlgOption(HWND hWnd, HINSTANCE hi, DWORD dlgId, DLGPROC proc)
			:dlgHwnd(hWnd), hInstance(hi), dlgResId(dlgId)
		{
			dlgProc = proc;
		}

		~DlgOption() {
			KillTimer(dlgHwnd, m_Timer);
		}

	private:
		DlgOption() :dlgHwnd(NULL), hInstance(NULL), dlgResId(NULL) { dlgProc = NULL; }
		
		DWORD m_Timer;
		DLGPROC dlgProc;
		HWND dlgHwnd;
		HINSTANCE hInstance;
		DWORD dlgResId; // DLG ID
	};

public:
	void OnInitDlg(HWND hWnd);
	void StartDlg(DlgOption option);
	BOOL OnExit(HWND hWnd);

	void SetTimer(HWND hWndList, HWND hWndEdit, DWORD time);
	void SetRunTime();

	void SetMessage(HWND hWnd, const char *s);
	
	INT_PTR CALLBACK BTZ_PROC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	SDlg();
	~SDlg();

protected:
	SCriticalSection m_cs;
	
	DlgOption m_opt;

	TimerBehavair m_behavair;
};


