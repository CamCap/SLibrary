#include "stdafx.h"
#include "SDlg.h"
#include <ctime>
#include <cstringt.h>
#include <atlstr.h>

void TimerBehavair::SetRunTime()
{
	m_TimeCount++;
	if (m_TimeCount == 6)
	{
		//가동 시간
		m_Minute++;
		if (m_Minute >= 60)
		{
			m_Hour++;
			m_Minute = 0;
			if (m_Hour >= 24)
			{
				m_Day++;
				m_Hour = 0;
			}
		}

		CString str;
		str.Format(_T("%d일%2d시%2d분"), m_Day, m_Hour, m_Minute);

		m_TimeCount = 0;
	}
}

void TimerBehavair::SetTimer(DWORD time)
{
	SYSTEMTIME st;
	CString str;
	GetLocalTime(&st);

	m_RealDay = st.wDay;
	m_RealHour = st.wHour;
	m_RealMinute = st.wMinute;
	m_WriteTime = m_RealMinute % 10;

	char year[5];
	wsprintfA(year, "%d", st.wYear);
	char month[3];
	if (st.wMonth < 10)
		wsprintfA(month, "0%d", st.wMonth);
	else
		wsprintfA(month, "%d", st.wMonth);
	char day[3];
	if (st.wDay < 10)
		wsprintfA(day, "0%d", st.wDay);
	else
		wsprintfA(day, "%d", st.wDay);

}

//////////////////////////////////////////////////////////////////

SDlg::SDlg()
{
//	memcpy(&m_opt, 0, sizeof(DlgOption));
}


SDlg::~SDlg()
{
}

void SDlg::OnInitDlg(HWND hWnd)
{	
	ShowWindow(m_opt.dlgHwnd, SW_SHOWNORMAL);
	
	m_opt.dlgHwnd = hWnd;
}

void SDlg::StartDlg(DlgOption option)
{
	memcpy(&m_opt, &option, sizeof(DlgOption));

	DialogBox(m_opt.hInstance, MAKEINTRESOURCE(m_opt.dlgResId), m_opt.dlgHwnd, m_opt.dlgProc);
}

void SDlg::SetTimer(DWORD time)
{
	::SetTimer(m_opt.dlgHwnd, TIME_ID, time, NULL);

	//str.Format(_T("%d일%2d시%2d분 서버 가동 시작"), st.wDay, st.wHour, st.wMinute);
	//SetMessage(this->m_opt.ListID, LPSTR(LPCTSTR(str)));
	//m_timerFunction = process;
}

void SDlg::SetMessage(DWORD resid, const char *s)
{
	HWND hWnd = GetDlgItem(m_opt.dlgHwnd, resid);

	//if (hWnd == NULL) return;

	SendMessage(hWnd, LB_ADDSTRING, (WPARAM)0, (LPARAM)s);
}


void SDlg::SetRunTime(DWORD EditID)
{
	
}

INT_PTR SDlg::BTZ_PROC(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
	case WM_TIMER:
	{
		switch (wParam)
		{
		case TIME_ID:
			SetRunTime();
			break;
		}
	}break;
	default:
		break;
	}

	return FALSE;
}


