#pragma once
#include "CriticalSection.h"
#include <functional>
#include "resource.h"

class SDlgBehavair
{
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

	virtual void Behavair();

	void SetTimer(DWORD time);
	void SetRunTime();
};

여기에 다시 중간 다리를 놓는 클래스를 추가해..? 아니 시발 왜..?
...아 시발 진짜 어렵네
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

	void SetTimer(DWORD time);
	//void

	void SetMessage(DWORD resid, const char *s);
	void SetRunTime(DWORD EditID);

	INT_PTR CALLBACK BTZ_PROC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	SDlg();
	~SDlg();

protected:
	SCriticalSection m_cs;
	
	DlgOption m_opt;

	TimerBehavair m_behavair;
};


