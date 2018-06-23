#pragma once
#include "CriticalSection.h"
#include <functional>
#include "resource.h"
#include "SMasterServer.h"

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

class SMasterServerBehavair
	:public SDlgBehavair
{
public:
	SMasterServerBehavair() {}
	~SMasterServerBehavair() {}

public:
	bool ConnectMasterServer(const char* ip, int port) {
		return server.ConnectToMasterServer(ip, port);
	}

	void SetTimer(HWND dlgHwnd)
	{
		::SetTimer(dlgHwnd, TIME_MASTER_SERVER_PING, 1000, NULL);
	}

	virtual void Behavair() {
		server.m_server.OnPingCheck(GetTickCount());
	}

private:
	SMasterServer server;
};


class SDlg
{
public:
	struct DlgOption
	{
	public:
		friend class SDlg;


		DlgOption(HWND hWnd, HINSTANCE hi, DWORD dlgId, DLGPROC proc, bool master_server = false)
			:dlgHwnd(hWnd), hInstance(hi), dlgResId(dlgId), use_master_server(master_server)
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

		bool use_master_server;
	};

public:
	void OnInitDlg(HWND hWnd);
	void StartDlg(DlgOption option);
	BOOL OnExit(HWND hWnd);

	void SetTimer(HWND hWndList, HWND hWndEdit, DWORD time);

	void SetMessage(HWND hWnd, const char *s);
	void SetMessage(const char* s);
	INT_PTR CALLBACK BTZ_PROC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	SDlg();
	~SDlg();

protected:
	SCriticalSection m_cs;
	
	DlgOption m_opt;

	TimerBehavair m_Timerbehavair;
	SMasterServerBehavair m_masterBehavair;
};


