#pragma once

#include <string>

// CFullSceenStatic

class CFullSceenStatic : public CStatic
{
	DECLARE_DYNAMIC(CFullSceenStatic)

public:
	CFullSceenStatic();
	virtual ~CFullSceenStatic();

	RECT m_preWinRect;
	BOOL m_bFullScreen;
	HWND m_hParent;

	CString m_strMemberId;
	void* m_pParentDlg;

	void SetParentDlg(void* pParentDlg);
	void SetMemberId(std::string strMemberId);

	void ChangeWindowSize();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


