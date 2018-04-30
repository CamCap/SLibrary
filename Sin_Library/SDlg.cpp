#include "stdafx.h"
#include "SDlg.h"
#include <ctime>
#include <cstringt.h>
#include <atlstr.h>


SDlg::SDlg()
{
	memcpy(&m_option, 0, sizeof(DlgOption));
}


SDlg::~SDlg()
{
}


void SDlg::OnInitDlg()
{
	m_Day = 0;
	m_Hour = 0;
	m_Minute = 0;
	m_TimeCount = 0;


	CString str;
	str.Format(_T("=================초기화 종료================="));
	SetMessage(LPSTR(LPCTSTR(str)));

	ShowWindow(m_option.dlgHwnd, SW_SHOWNORMAL);
	SetTimer(m_option.dlgHwnd, m_option.resID, 10000, NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

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

	sprintf_s(m_RunTime, "%d일%2d시%2d분", m_Day, m_Hour, m_Minute);
	SetWindowTextA(m_option.TimerHwnd, m_RunTime);

	m_RealDay = st.wDay;
	m_RealHour = st.wHour;
	m_RealMinute = st.wMinute;
	m_WriteTime = m_RealMinute % 10;

	
	str.Format(_T("%d일%2d시%2d분 서버 가동 시작"), st.wDay, st.wHour, st.wMinute);
	SetMessage(LPSTR(LPCTSTR(str)));
	
}


void SDlg::StartDlg(DlgOption* option)
{
	memcpy(&m_option, option, sizeof(DlgOption));

	DialogBox(m_option.hInstance, MAKEINTRESOURCE(m_option.resID), m_option.dlgHwnd, m_option.dlgProc);
}

void SDlg::SetMessage(const char *s)
{
	if(m_option.dlgHwnd != 0)
		SendMessage(m_option.dlgHwnd, LB_ADDSTRING, (WPARAM)0, (LPARAM)s);
}


void SDlg::SetRunTime()
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
		//wsprintfA(m_RunTime, "%d일%2d시%2d분", m_Day, m_Hour, m_Minute);
		CString str;
		str.Format(_T("%d일%2d시%2d분"), m_Day, m_Hour, m_Minute);
		SetWindowText(m_option.dlgHwnd, LPCWSTR(LPCTSTR(str)));

		m_TimeCount = 0;
	}
}

