
// demoDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "shinevv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include "afxcolorbutton.h"
#include "afxcmn.h"
#include "CFullSceenStatic.h"


typedef std::map<std::string, HWND> mapMemberWindow;
typedef std::pair<std::string, HWND> pairMemberWindow;

typedef std::map<CString, int> mapCameraIndex;
typedef std::pair<CString, int> pairCameraIndex;

#define WM_JOIN_SUCCESS				WM_USER+0x110
#define WM_JOIN_FAIL				WM_USER+0x111
#define WM_MEMBER_ENABLE_VIDEO		WM_USER+0x112
#define WM_MEMBER_DISBLE_VIDEO      WM_USER+0x113
#define WM_FULLSCREEN_REFRESH       WM_USER+0x114 
#define WM_UPDATEUI                 WM_USER+0x115  

// CdemoDlg dialog
class CdemoDlg : public CDialogEx
{
// Construction
public:
	CdemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnUIUpdate(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	// ip��ַ
	CString m_strIP;
	// �������˿�
	int m_iPort;
	// �����
	CString m_strRoomId;
	// ���� token
	CString m_token;
	// �ǳ�
	CString m_strNickName;
	// ����ͷ�б�
	CComboBox m_ctlCameraList;
	// ��Ƶ�ߴ��б�
	CComboBox m_ctlVideoSizeList;
	// ֡��
	int m_iFrames;

	// ������Ƶ
	CFullSceenStatic m_ctlLocalVideo;
	// Զ����Ƶ
	CFullSceenStatic m_ctlRemoteVideo;
	
	// ���ؽ���
	BOOL m_bEnableLocalAudio;
	// ���ؽ���
	BOOL m_bEnableLocalVideo;

	// Զ�˽���
	BOOL m_bEnableRemoteAudio;
	// Զ�˽���
	BOOL m_bEnableRemoteVideo;

	void* m_shinevv = NULL;

	std::string m_strLocalMemberId;
	std::string m_strRemoteMemberId;

	mapMemberWindow m_memberWindowMap;
	mapCameraIndex m_cameraIndexMap;

	afx_msg void OnBnClickedJoinroom();
	afx_msg void OnBnClickedLeaveroom();

	afx_msg LRESULT OnMessageJoinSuccess(WPARAM, LPARAM);
	afx_msg LRESULT OnMessageJoinFail(WPARAM, LPARAM);
	afx_msg LRESULT OnMessageMemberEnableVideo(WPARAM, LPARAM);
	afx_msg LRESULT OnMessageRefreshMemberVideo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedEnableLocalAudio();
	afx_msg void OnBnClickedEnableLocalVideo();
	afx_msg void OnBnClickedEnableRemoteAudio();
	afx_msg void OnBnClickedEnableRemoteVideo();	
	afx_msg void OnCbnSelchangeCameraList();
	
	afx_msg void OnDestroy();
	
	CButton m_btnEnableLocalAudio;
	CButton m_btnEnableLocalVideo;
	CButton m_btnEnableRemoteAudio;
	CButton m_btnEnableRemoteVideo;

private:
	void ModifyVideoStatus(std::string memberid, bool bOpen);
	void ModifyAudioStatus(std::string memberid, bool bOpen);
	void ShowMemberVideoWnd(std::string memberid, bool bShow);
	void Reset();
	
public:
	afx_msg LRESULT OnMemberDisbleVideo(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClose();	
	afx_msg void OnCbnSelchangeVideoSizeList();
	// ???
	CString m_strReslution;
	afx_msg void OnBnClickedBtnInternet();
	afx_msg void OnBnClickedBtnPrivate();
};

//// callback function

void OnJoined(void* userData, ErrorCode eCode);

void OnDisConnected(void* userData);

void OnNewMemberJoined(void* userData, const char* pMemberId, const char* pDisplayName);

void OnMemberLeft(void* userData, const char* pMemberId, const char* pDisplayName);

void OnModifyLocalAudioStatus(void* userData, bool bOpen);

void OnModifyLocalVideoStatus(void* userData, bool bOpen);

void OnMemberEnableVideo(void* userData, const char* pMemberId, const char* pDisplayName);

void OnMemberDisableVideo(void* userData, const char* pMemberId, const char* pDisplayName/*, void* pRenderWin*/);

void OnMemberUnMuteAudio(void* userData, const char* pMemberId, const char* pDisplayName);

void OnMemberMuteAudio(void* userData, const char* pMemberId, const char* pDisplayName);

void OnSessionError(void* userData, const char* reason);

void OnRecvUserMessage(void* userData, const char* pHistoryMessage);

void GetVideoDevicesResult(void* userData, const DeviceInfo sDeviceInfos[], int nDeviceNum);

void GetAudioDevicesResultCallBack(void* userData, const DeviceInfo sDeviceInfos[], int nDeviceNum);

void SetCameraIndexRes(void* userData, bool succ);

void OnMemberEnableScreenShare(void* userData, const char* pMemberId, const char* pDisplayName);

void OnMemberDisableScreenShare(void* userData, const char* pMemberId, const char* pDisplayName);

void onEnableMemberVideoResult(void* userData, bool bSucc, const char* pMemberId);

void onDisableMemberVideoResult(void* userData, bool bSucc, const char* pMemberId);

