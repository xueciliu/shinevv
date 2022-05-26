// CFullSceenStatic.cpp: 实现文件
//

#include "stdafx.h"
#include "CFullSceenStatic.h"
#include "demoDlg.h"


// CFullSceenStatic

IMPLEMENT_DYNAMIC(CFullSceenStatic, CStatic)

CFullSceenStatic::CFullSceenStatic()
{
	m_preWinRect = { 0 };
	m_bFullScreen = FALSE;
	m_hParent = NULL;

	m_pParentDlg = NULL;
	m_strMemberId = _T("");
}

CFullSceenStatic::~CFullSceenStatic()
{
}


BEGIN_MESSAGE_MAP(CFullSceenStatic, CStatic)
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CFullSceenStatic 消息处理程序
afx_msg void CFullSceenStatic::OnLButtonDblClk(UINT nFlags, CPoint point)
{	
	ChangeWindowSize();	
}

void CFullSceenStatic::ChangeWindowSize() {
	if (!m_bFullScreen)
	{
		m_hParent = ::GetParent(GetSafeHwnd());

		LONG style = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
		style &= ~WS_CHILD;
		style |= WS_POPUP;
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);

		int nW = GetSystemMetrics(SM_CXSCREEN);
		int nH = GetSystemMetrics(SM_CYSCREEN);

		::SetParent(GetSafeHwnd(), ::GetDesktopWindow());
		::GetWindowRect(GetSafeHwnd(), &m_preWinRect);
		::MoveWindow(GetSafeHwnd(), 0, 0, nW, nH, TRUE);

		m_bFullScreen = TRUE;
	}
	else
	{
		int nW = m_preWinRect.right - m_preWinRect.left;
		int nH = m_preWinRect.bottom - m_preWinRect.top;
		int nX = m_preWinRect.left;
		int nY = m_preWinRect.top;

		LONG style = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
		style |= WS_CHILD;
		style &= ~WS_POPUP;
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, style);

		::SetParent(GetSafeHwnd(), m_hParent);
		::MoveWindow(GetSafeHwnd(), nX, nY, nW, nH, TRUE);

		m_bFullScreen = FALSE;
	}

	if (m_pParentDlg) {
		::SendMessage(((CdemoDlg*)m_pParentDlg)->GetSafeHwnd(), WM_FULLSCREEN_REFRESH, NULL, (LPARAM)&m_strMemberId);
	}
}

void CFullSceenStatic::SetParentDlg(void* pParentDlg) {
	m_pParentDlg = pParentDlg;
}

void CFullSceenStatic::SetMemberId(std::string strMemberId) {
	m_strMemberId = strMemberId.c_str();
}

BOOL CFullSceenStatic::PreTranslateMessage(MSG* pMsg) {
	if (m_bFullScreen && pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		ChangeWindowSize();
		return TRUE;
	}
	else {
		return CStatic::PreTranslateMessage(pMsg);
	}
}
