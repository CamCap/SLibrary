#include "stdafx.h"
#include "SDlg.h"
#include <ctime>
#include <cstringt.h>
#include <atlstr.h>


SDlg::SDlg()
{
}


SDlg::~SDlg()
{
}


void SDlg::OnInitDlg(DlgOption* option)
{
	m_hWnd = option->dlgHwnd;

	m_Day = 0;
	m_Hour = 0;
	m_Minute = 0;
	m_TimeCount = 0;
}


void SDlg::StartDlg(HINSTANCE hInstance, int resID, HWND parentHwnd)
{
//	DialogBox(hInstance, MAKEINTRESOURCE(resID), parentHwnd, (DLGPROC)ServerProc);
}

void SDlg::SetMessage(const char *s)
{
	SendMessage(m_hWndMsg, LB_ADDSTRING, (WPARAM)0, (LPARAM)s);
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
		SetWindowText(m_hTimer, LPCWSTR(LPCTSTR(str)));

		m_TimeCount = 0;
	}
}

