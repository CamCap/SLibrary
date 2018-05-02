#include "stdafx.h"
#include "SDlg.h"
#include <ctime>
#include <cstringt.h>
#include <atlstr.h>


SDlg::SDlg()
{
	memcpy(&m_opt, 0, sizeof(DlgOption));
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
	
	auto it = m_opt.m_list.begin();

	while (it != m_opt.m_list.end())
	{
		auto _pair = *it;
		_pair.second = GetDlgItem(m_opt.dlgHwnd, _pair.first);
		//m_hWndMsg = GetDlgItem(m_hWnd, IDC_LOG);
		it++;
	}

	ShowWindow(m_opt.dlgHwnd, SW_SHOWNORMAL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	m_RealDay = st.wDay;
	m_RealHour = st.wHour;
	m_RealMinute = st.wMinute;
	m_WriteTime = m_RealMinute % 10;
}

void SDlg::StartDlg(DlgOption option)
{
	memcpy(&m_opt, &option, sizeof(DlgOption));

	DialogBox(m_opt.hInstance, MAKEINTRESOURCE(m_opt.dlgResId), m_opt.dlgHwnd, m_opt.dlgProc);
}

void SDlg::SetMessage(const char *s)
{
	SendMessage(m_opt.dlgHwnd, LB_ADDSTRING, (WPARAM)0, (LPARAM)s);
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
	//	SetWindowText(m_option.dlgHwnd, LPCWSTR(LPCTSTR(str)));

		m_TimeCount = 0;
	}
}

