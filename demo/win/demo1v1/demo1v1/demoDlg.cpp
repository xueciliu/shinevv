
// demoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"
#include "shinevv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// Receive Video Resolution
CdemoDlg* g_this = NULL;
void OnRecvVideoResolution(int width, int height) {
	CString strRes;
	g_this->m_strReslution.Format(L"接收分辨率：%d x %d", width, height);
	g_this->PostMessage(WM_UPDATEUI, FALSE);
}


CdemoDlg::CdemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_strIP = _T("sdk.sl.shinevv.com");

	m_iPort = 3443;

	m_iFrames = 15;

	m_strRoomId = "50414";
	
	m_token = _T("b775bd9f1f1c0c89b9f9f949919cc431");

	m_strNickName = "shine";

	// 本地音频
	m_bEnableLocalAudio=TRUE;
	// 本地视频
	m_bEnableLocalVideo = TRUE;

	// 远端音频
	m_bEnableRemoteAudio = TRUE;
	// 远端视频
	m_bEnableRemoteVideo = TRUE;
}

void CdemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, m_strIP);
	DDX_Text(pDX, IDC_PORT, m_iPort);
	DDX_Text(pDX, IDC_FRAMES, m_iFrames);
	DDX_Text(pDX, IDC_ROOMID, m_strRoomId);
	DDX_Text(pDX, IDC_NICKNAME, m_strNickName);
	DDX_Control(pDX, IDC_CAMERA_LIST, m_ctlCameraList);
	DDX_Check(pDX, IDC_ENABLEAUDIO1, m_bEnableLocalAudio);
	DDX_Check(pDX, IDC_ENABLEVIDEO1, m_bEnableLocalVideo);
	DDX_Check(pDX, IDC_ENABLEAUDIO2, m_bEnableRemoteAudio);
	DDX_Check(pDX, IDC_ENABLEVIDEO2, m_bEnableRemoteVideo);
	DDX_Text(pDX, IDC_TOKEN, m_token);
	DDV_MaxChars(pDX, m_token, 64);
	DDX_Control(pDX, IDC_ENABLEAUDIO1, m_btnEnableLocalAudio);
	DDX_Control(pDX, IDC_ENABLEVIDEO1, m_btnEnableLocalVideo);
	DDX_Control(pDX, IDC_ENABLEAUDIO2, m_btnEnableRemoteAudio);
	DDX_Control(pDX, IDC_ENABLEVIDEO2, m_btnEnableRemoteVideo);
	DDX_Control(pDX, IDC_VIDEOSIZE_LIST, m_ctlVideoSizeList);
	DDX_Text(pDX, IDC_RECVRESOLUTION, m_strReslution);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_JOINROOM, &CdemoDlg::OnBnClickedJoinroom)
	ON_BN_CLICKED(IDC_LEAVEROOM, &CdemoDlg::OnBnClickedLeaveroom)
	ON_MESSAGE(WM_JOIN_SUCCESS, &CdemoDlg::OnMessageJoinSuccess)
	ON_MESSAGE(WM_JOIN_FAIL, &CdemoDlg::OnMessageJoinFail)
	ON_MESSAGE(WM_MEMBER_ENABLE_VIDEO, &CdemoDlg::OnMessageMemberEnableVideo)
	ON_MESSAGE(WM_FULLSCREEN_REFRESH, &CdemoDlg::OnMessageRefreshMemberVideo)
	ON_BN_CLICKED(IDC_ENABLEAUDIO1, &CdemoDlg::OnBnClickedEnableLocalAudio)
	ON_BN_CLICKED(IDC_ENABLEVIDEO1, &CdemoDlg::OnBnClickedEnableLocalVideo)
	ON_BN_CLICKED(IDC_ENABLEAUDIO2, &CdemoDlg::OnBnClickedEnableRemoteAudio)
	ON_BN_CLICKED(IDC_ENABLEVIDEO2, &CdemoDlg::OnBnClickedEnableRemoteVideo)
	ON_CBN_SELCHANGE(IDC_CAMERA_LIST, &CdemoDlg::OnCbnSelchangeCameraList)
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_MEMBER_DISBLE_VIDEO, &CdemoDlg::OnMemberDisbleVideo)
	ON_WM_CLOSE()
	ON_MESSAGE(WM_UPDATEUI, OnUIUpdate)
	ON_BN_CLICKED(IDC_BTN_INTERNET, &CdemoDlg::OnBnClickedBtnInternet)
	ON_BN_CLICKED(IDC_BTN_PRIVATE, &CdemoDlg::OnBnClickedBtnPrivate)
END_MESSAGE_MAP()

// CdemoDlg message handlers

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	g_this = this;

	srand((unsigned)time(NULL));
	m_strLocalMemberId = std::to_string(rand() % 10000000);

	m_shinevv = CreateShinevvInstance();

	m_ctlLocalVideo.SubclassDlgItem(IDC_VIDEO1, this);
	m_ctlLocalVideo.SetParentDlg(this);
	m_ctlLocalVideo.SetMemberId(m_strLocalMemberId);
	m_ctlRemoteVideo.SubclassDlgItem(IDC_VIDEO2, this);
	m_ctlRemoteVideo.SetParentDlg(this);
	
	m_ctlVideoSizeList.AddString(_T("1280x720"));
	m_ctlVideoSizeList.AddString(_T("1920x1080"));
	m_ctlVideoSizeList.AddString(_T("2560x1440"));
	m_ctlVideoSizeList.AddString(_T("4096x2160"));
	m_ctlVideoSizeList.SetCurSel(3);

	InitSDK(m_shinevv, (void*)this, OnJoined, OnDisConnected,
		OnNewMemberJoined, OnMemberLeft, OnMemberEnableVideo,
		OnMemberDisableVideo, OnMemberEnableScreenShare,
		OnMemberDisableScreenShare, OnMemberUnMuteAudio,
		OnMemberMuteAudio, OnRecvUserMessage, OnSessionError);
	GetVideoDevices(m_shinevv, GetVideoDevicesResult);

	//GetVideoDevices(m_shinevv, GetVideoDevicesResult);

	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CdemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CdemoDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
	if (!m_shinevv)
		return;

	//DestoryPaletteInstance(m_shinevv);

	//LeaveRoom(m_shinevv);

	//ReleaseSdk(m_shinevv);

	DestoryShinevvInstance();

	ClearShinevv();
}


void CdemoDlg::OnBnClickedJoinroom()
{
	UpdateData();

	if (!m_shinevv)
		return;

	std::string pRoomId(CW2A(m_strRoomId.GetString()));

	
	const char* pMemberId = m_strLocalMemberId.c_str();

	std::string pDisplayName(CW2A(m_strNickName.GetString()));

	std::string pServerIp(CW2A(m_strIP.GetString()));
	
	std::string pToken(CW2A(m_token.GetString()));

	int w = 4096, h = 2160;
	int nIndex = m_ctlVideoSizeList.GetCurSel();
	switch (nIndex) {
	case 0: {
		// 720P
		w = 1280;
		h = 720;
		break;
	}
	case 1: {
		// 1080P
		w = 1920;
		h = 1080;
		break;
	}
	case 2: {
		// 2K
		w = 2560;
		h = 1440;
		break;
	}
	case 3:
	default: {
		// 4K
		w = 4096;
		h = 2160;
		break;
	}
	}

	SetInputFormat(m_shinevv, w, h, m_iFrames, nullptr);
	JoinRoom(m_shinevv, pRoomId.c_str(), MediaType::Video, pMemberId, pDisplayName.c_str(), pServerIp.c_str(), m_iPort, pToken.c_str());

	m_bEnableLocalAudio = false;
	m_bEnableLocalVideo = false;
	UpdateData(FALSE);
}

void CdemoDlg::OnBnClickedLeaveroom()
{
	// TODO: Add your control notification handler code here
	if (!m_shinevv)
		return;
	LeaveRoom(m_shinevv);
	ReleaseSdk(m_shinevv);
	Reset();
}

void CdemoDlg::Reset() {
	CButton * pJoin = (CButton*)GetDlgItem(IDC_JOINROOM);
	pJoin->ShowWindow(TRUE);
	CButton * pLeva = (CButton*)GetDlgItem(IDC_LEAVEROOM);
	pLeva->ShowWindow(FALSE);
	m_btnEnableLocalVideo.ShowWindow(FALSE);
	m_btnEnableLocalAudio.ShowWindow(FALSE);
	m_strRemoteMemberId.clear();

	m_btnEnableRemoteVideo.ShowWindow(FALSE);
	m_btnEnableRemoteAudio.ShowWindow(FALSE);
}

void CdemoDlg::OnCbnSelchangeCameraList()
{
	// TODO: Add your control notification handler code here
	int nIndex = m_ctlCameraList.GetCurSel();

	CString cstrCamera;

	m_ctlCameraList.GetLBText(nIndex, cstrCamera);
	DisableMemberVideo(m_shinevv, m_strLocalMemberId.c_str(), NULL);
	SetVideoDevice(m_shinevv, m_cameraIndexMap[cstrCamera], NULL);
	EnableMemberVideo(m_shinevv, m_strLocalMemberId.c_str(), NULL);
}

void CdemoDlg::ModifyVideoStatus(std::string memberid, bool bOpen) {

	if (memberid.empty()) return;

	if (!bOpen) {
		EnableMemberVideo(m_shinevv, memberid.c_str(), onEnableMemberVideoResult);
	}
	else
	{
		DisableMemberVideo(m_shinevv, memberid.c_str(), onDisableMemberVideoResult);
	}
}
void CdemoDlg::ModifyAudioStatus(std::string memberid, bool bOpen) {
	if (memberid.empty()) return;
	if (!bOpen) {
		EnableMemberAudio(m_shinevv, memberid.c_str(), NULL);
	}
	else
	{
		DisableMemberAudio(m_shinevv, memberid.c_str(), NULL);
	}
}

void CdemoDlg::ShowMemberVideoWnd(std::string memberid, bool bShow) {
	if (memberid.compare(m_strLocalMemberId) == 0) {
		m_ctlLocalVideo.ShowWindow(bShow);
	}else 	if (memberid.compare(m_strRemoteMemberId) == 0) {
		m_ctlRemoteVideo.ShowWindow(bShow);
	}
}


void CdemoDlg::OnBnClickedEnableLocalAudio()
{
	// TODO: Add your control notification handler code here
	if (!m_shinevv)
		return;
	UpdateData(TRUE);
	ModifyAudioStatus(m_strLocalMemberId, m_bEnableLocalAudio);
}

void CdemoDlg::OnBnClickedEnableLocalVideo()
{
	// TODO: Add your control notification handler code here
	if (!m_shinevv)
		return;
	UpdateData(TRUE);
	ModifyVideoStatus(m_strLocalMemberId, m_bEnableLocalVideo);
}

void CdemoDlg::OnBnClickedEnableRemoteAudio()
{
	// TODO: Add your control notification handler code here
	if (!m_shinevv)
		return;
	UpdateData(TRUE);
	ModifyAudioStatus(m_strRemoteMemberId, m_bEnableRemoteAudio);
}


void CdemoDlg::OnBnClickedEnableRemoteVideo()
{
	// TODO: Add your control notification handler code here
	if (!m_shinevv)
		return;
	UpdateData(TRUE);
	ModifyVideoStatus(m_strRemoteMemberId, m_bEnableRemoteVideo);
}

//process message 
LRESULT CdemoDlg::OnMessageJoinSuccess(WPARAM, LPARAM) {
	if (!m_shinevv)
		return 0;
	CButton * pJoin = (CButton*)GetDlgItem(IDC_JOINROOM);
	pJoin->ShowWindow(FALSE);
	CButton * pLeva = (CButton*)GetDlgItem(IDC_LEAVEROOM);
	pLeva->ShowWindow(TRUE);

	m_ctlLocalVideo.ShowWindow(TRUE);
	m_btnEnableLocalAudio.ShowWindow(TRUE);
	m_btnEnableLocalVideo.ShowWindow(TRUE);
	SetRenderWindow(m_shinevv, m_strLocalMemberId.c_str(), (void*)m_ctlLocalVideo.GetSafeHwnd(), true);

	return 0;
}

LRESULT CdemoDlg::OnMessageJoinFail(WPARAM, LPARAM) {
	if (!m_shinevv)
		return 0;

	LeaveRoom(m_shinevv);

	return 0;
}

LRESULT CdemoDlg::OnMessageMemberEnableVideo(WPARAM wParam, LPARAM lParam) {
	if (!m_shinevv)
		return 0;

	CString *csMemberId = (CString*)lParam;

	std::string strMemberId(CW2A(csMemberId->GetString()));

	mapMemberWindow::iterator it = m_memberWindowMap.find(std::string(strMemberId));
	if (it != m_memberWindowMap.end()) {
		SetRenderWindow(m_shinevv, strMemberId.c_str(), (void*)it->second, false, OnRecvVideoResolution);
	}
	if (it == m_memberWindowMap.end()) {
		if (strMemberId.compare(m_strLocalMemberId)==0)
		{
			SetRenderWindow(m_shinevv, strMemberId.c_str(), (void*)m_ctlLocalVideo.GetSafeHwnd(), true);
		}
	}
	ShowMemberVideoWnd(strMemberId, TRUE);

	if (csMemberId) {
		delete csMemberId;
		csMemberId = NULL;
	}

	return 0;
}

LRESULT CdemoDlg::OnMessageRefreshMemberVideo(WPARAM wParam, LPARAM lParam) {
	if (!m_shinevv)
		return 0;

	CString* csMemberId = (CString *)lParam;

	std::string strMemberId(CW2A(csMemberId->GetString()));

	if (strMemberId.compare(m_strLocalMemberId) == 0) {
		SetRenderWindow(m_shinevv, strMemberId.c_str(), (void*)m_ctlLocalVideo.GetSafeHwnd(), true);
	}
	else if (strMemberId.compare(m_strRemoteMemberId) == 0){
		SetRenderWindow(m_shinevv, strMemberId.c_str(), (void*)m_ctlRemoteVideo.GetSafeHwnd(), false, OnRecvVideoResolution);
	}

	return 0;
}

//call back 
void OnJoined(void* userData, ErrorCode eCode) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;

	if (eCode == ErrorCode::VVSuccess) {
		::PostMessage(demo_instance->GetSafeHwnd(), WM_JOIN_SUCCESS, NULL, NULL);
	}
	else {
		::PostMessage(demo_instance->GetSafeHwnd(), WM_JOIN_FAIL, NULL, NULL);
	}
}

void OnDisConnected(void* userData) {

}

void OnNewMemberJoined(void* userData, const char* pMemberId, const char* pDisplayName) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;

	HWND remoteWindow = NULL;
	if (demo_instance->m_strRemoteMemberId.empty()) {
		demo_instance->m_strRemoteMemberId = pMemberId;
		remoteWindow = demo_instance->m_ctlRemoteVideo.GetSafeHwnd();
		demo_instance->m_ctlRemoteVideo.ShowWindow(TRUE);
		demo_instance->m_btnEnableRemoteAudio.ShowWindow(TRUE);
		demo_instance->m_btnEnableRemoteVideo.ShowWindow(TRUE);
		demo_instance->m_bEnableRemoteAudio = false;
		demo_instance->m_bEnableRemoteVideo = false;
		//demo_instance->UpdateData(FALSE);

		demo_instance->PostMessage(WM_UPDATEUI, FALSE);
		demo_instance->m_ctlRemoteVideo.SetMemberId(demo_instance->m_strRemoteMemberId);
	}

	mapMemberWindow::iterator it = demo_instance->m_memberWindowMap.find(std::string(pMemberId));
	if (it != demo_instance->m_memberWindowMap.end()) {
		demo_instance->m_memberWindowMap.erase(std::string(pMemberId));
	}

	demo_instance->m_memberWindowMap.insert(pairMemberWindow(std::string(pMemberId), remoteWindow));
}

void OnMemberLeft(void* userData, const char* pMemberId, const char* pDisplayName) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;

	mapMemberWindow::iterator it = demo_instance->m_memberWindowMap.find(std::string(pMemberId));
	if (it != demo_instance->m_memberWindowMap.end()) {
		demo_instance->m_memberWindowMap.erase(std::string(pMemberId));
	}

	if (demo_instance->m_strRemoteMemberId == pMemberId) {
		demo_instance->m_strRemoteMemberId.clear();
		demo_instance->m_ctlRemoteVideo.ShowWindow(FALSE);
		demo_instance->m_btnEnableRemoteAudio.ShowWindow(FALSE);
		demo_instance->m_btnEnableRemoteVideo.ShowWindow(FALSE);
	}
}

void OnModifyLocalAudioStatus(void* userData, bool bOpen) {

}

void OnModifyLocalVideoStatus(void* userData, bool bOpen) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;
	demo_instance->m_ctlLocalVideo.ShowWindow(bOpen);
	if (bOpen) {
		SetRenderWindow(demo_instance->m_shinevv, demo_instance->m_strLocalMemberId.c_str(), (void*)demo_instance->m_ctlLocalVideo.GetSafeHwnd(), true);
	}
}

void OnMemberEnableVideo(void* userData, const char* pMemberId, const char* pDisplayName) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;

	CString *csMemberId = new CString(pMemberId);

	::PostMessage(demo_instance->GetSafeHwnd(), WM_MEMBER_ENABLE_VIDEO, NULL, (LPARAM)(csMemberId));
}

void OnMemberDisableVideo(void* userData, const char* pMemberId, const char* pDisplayName/*, void* pRenderWin*/) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;

	CString *csMemberId = new CString(pMemberId);

	::PostMessage(demo_instance->GetSafeHwnd(), WM_MEMBER_DISBLE_VIDEO, NULL, (LPARAM)(csMemberId));
}

void OnMemberEnableScreenShare(void* userData, const char* pMemberId, const char* pDisplayName) {}
void OnMemberDisableScreenShare(void* userData, const char* pMemberId, const char* pDisplayName) {}

void OnMemberUnMuteAudio(void* userData, const char* pMemberId, const char* pDisplayName) {

}

void OnMemberMuteAudio(void* userData, const char* pMemberId, const char* pDisplayName) {

}

void OnRecvUserMessage(void* userData, const char* pHistoryMessage) {

}

void OnSessionError(void* userData, const char* reason) {

}

void GetVideoDevicesResult(void* userData, const DeviceInfo sDeviceInfos[], int nDeviceNum) {
	CdemoDlg* demo_instance = (CdemoDlg*)userData;

	for (int i = 0; i < nDeviceNum; i++) {
		CString cameraName(sDeviceInfos[i].pDeviceName);
		demo_instance->m_ctlCameraList.AddString(cameraName);

		demo_instance->m_cameraIndexMap[cameraName] = sDeviceInfos[i].nDeviceIndex;
	}
	demo_instance->m_ctlCameraList.SetCurSel(0);
}

void GetAudioDevicesResultCallBack(void* userData, const DeviceInfo sDeviceInfos[], int nDeviceNum) {

}

void SetCameraIndexRes(void* userData, bool succ) {

}

void onEnableMemberVideoResult(void* userData, bool bSucc, const char* pMemberId) {
	if(bSucc){
		OnMemberEnableVideo(userData, pMemberId, NULL);
	}
}
void onDisableMemberVideoResult(void* userData, bool bSucc, const char* pMemberId) {
	if (bSucc) {
		OnMemberDisableVideo(userData, pMemberId, NULL);
	}
}
void onEnableMemberAudioResult(void* userData, bool bSucc, const char* pMemberId) {}
void onDisableMemberAudioResult(void* userData, bool bSucc, const char* pMemberId) {}

afx_msg LRESULT CdemoDlg::OnMemberDisbleVideo(WPARAM wParam, LPARAM lParam)
{

	CString *csMemberId = (CString*)lParam;

	std::string strMemberId(CW2A(csMemberId->GetString()));

	ShowMemberVideoWnd(strMemberId, FALSE);

	if (csMemberId) {
		delete csMemberId;
		csMemberId = NULL;
	}
	return 0;
}

void CdemoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	OnBnClickedLeaveroom();
	CDialogEx::OnClose();
}

void CdemoDlg::OnBnClickedBtnInternet()
{
	m_token = "b775bd9f1f1c0c89b9f9f949919cc431";
	m_strRoomId = "50414";
	m_strNickName = "shine";
	m_strIP = "sdk.sl.shinevv.com";
	UpdateData(FALSE);
}


void CdemoDlg::OnBnClickedBtnPrivate()
{
	m_token = "9136ecd559356e6a74a4c69718b2d4b7";
	m_strRoomId = "50414";
	m_strNickName = "shine";
	m_strIP = "127.0.0.1";
	UpdateData(FALSE);
}

LRESULT CdemoDlg::OnUIUpdate(WPARAM wParam, LPARAM lParam) {
	BOOL bres = wParam;
	this->UpdateData(bres);
	return 0;
}
