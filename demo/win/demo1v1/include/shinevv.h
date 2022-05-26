#ifndef SHINEVV_H
#define SHINEVV_H

#ifdef _WINDOWS
#ifdef SHINEVV_INTERFACE_EXPORTS  
#define SHINEVV_INTERFACE_API __declspec(dllexport)   
#else  
#define SHINEVV_INTERFACE_API __declspec(dllimport)   
#endif
#else
#define SHINEVV_INTERFACE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

	typedef void(*OnVideoResolution)(int width, int height);
	/**
	* 错误码
	* @VVSuccess						加入房间成功
	* @VVConnectFail					连接服务器失败
	* @VVCreateMediaSessionFail			创建媒体会话失败
	* @VVAlreadyJoined					重复加入
	*/
	enum ErrorCode {
		VVSuccess = 0,
		VVConnectFail,
		VVCreateMediaSessionFail,
		VVAlreadyJoined
	};

	/**
	* 媒体会话类型
	* @Audio					语音通话
	* @Video					视频通话
	*/
	enum MediaType {
		Audio = 0,
		Video,
		None
	};

	typedef struct {
		const char* pDeviceName;
		int nDeviceIndex;
	}DeviceInfo;

	/**
	* 画笔类型
	*/
	enum PenType
	{
		LINE_PEN = 1,//画线
		BRUSH_PEN = 2,//画刷 暂不支持
		BEELINE_PEN = 3,//直线  暂不支持
		ARROR_PEN = 4,//箭头 暂不支持
		RECT_PEN = 5,//画矩形
		ELLIPSE_PEN = 6,//画椭圆
		ERASER_PEN = 10,//橡皮擦
	};

	/**
	* 创建shinevv句柄
	* 业务逻辑 跟随主程序一起创建
	* @return shinevv句柄
	*/
	SHINEVV_INTERFACE_API void* CreateShinevvInstance();

	/**
	* 销毁Shinevv句柄
	* 业务逻辑 在主程序要退出时调用
	* @param handle				Shinevv句柄
	*/
	SHINEVV_INTERFACE_API void DestoryShinevvInstance();

	/**
	* 回收内部资源，在程序结束时调用
	* 业务逻辑 在主程序要退出时调用(先调用DestoryShinevvInstance()，再调用ClearShinevv() )
	*/
	SHINEVV_INTERFACE_API void ClearShinevv();

	/**
	* 初始化shinevvSDK.
	* 业务逻辑 在创建房间时调用
	* @param handle							shinevv句柄
	* @param userData						用户自定义数据
	* @param OnJoined						加入房间回调,调用JoinRoom后会触发
	* @param OnDisConnected					断开连接回调,当此回调被触发时，要依次调用LeaveRoom()， ReleaseSdk()来释放资源
	* @param OnNewMemberJoined				新成员加入房间回调，在此回调中调用SetRenderWindow()接口来设置新成员视频渲染窗口句柄
	* @param OnMemberLeft					成员离开房间回调，其它成员调用LeaveRoom后会触发
	* @param OnMemberEnableVideo			成员打开视频回调
	* @param OnMemberDisableVideo			成员关闭视频回调
	* @param OnMemberUnMuteAudio			成员打开音频回调
	* @param OnMemberMuteAudio				成员关闭音频回调
	* @param OnSessionError					会话失败回调，会话失败的原因包括设备被占用，网络堵塞导致消息超时，消息内容错误等等
	*/
	SHINEVV_INTERFACE_API void InitSDK(
		void* handle,
		void* userData,
		void(*OnJoined)(void* userData, ErrorCode eCode),
		void(*OnDisConnected)(void* userData),
		void(*OnNewMemberJoined)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberLeft)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberEnableVideo)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberDisableVideo)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberEnableScreenShare)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberDisableScreenShare)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberUnMuteAudio)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnMemberMuteAudio)(void* userData, const char* pMemberId, const char* pDisplayName),
		void(*OnRecvUserMessage)(void* userData, const char* pHistoryMessage),
		void(*OnSessionError)(void* userData, const char* reason));

	/**
	* 销毁shinevvSDK内部资源.
	* 业务逻辑 在销毁房间时调用
	* @param handle					shinevv句柄
	*/
	SHINEVV_INTERFACE_API void ReleaseSdk(
		void* handle);

	/**
	* 指定视频渲染窗口.
	* 业务逻辑 针对本地视频，在JoinRoom()之前调用，针对远端视频在收到OnMemberEnableVideo()回调时调用
	* @param handle					shinevv句柄
	* @param pMemberId				成员标识符
	* @param pRemoteRenderWin       渲染窗口句柄
	* @param bMirror				是否镜像，一般本地图像为true，远端为false
	*/
	SHINEVV_INTERFACE_API void SetRenderWindow(
		void* handle,
		const char* pMemberId,
		void* pRenderWin,
		bool bMirror, OnVideoResolution onVideoResulution=nullptr);

	/**
	* 指定屏幕共享渲染窗口.
	* 业务逻辑 针对本地视频，在JoinRoom()之前调用，针对远端视频在收到OnMemberEnableScreenShare()回调时调用
	* @param handle					shinevv句柄
	* @param pMemberId				成员标识符
	* @param pRemoteRenderWin       渲染窗口句柄
	* @param bMirror				是否镜像，一般本地图像为true，远端为false
	*/
	SHINEVV_INTERFACE_API void SetScreenShareRenderWindow(
		void* handle,
		const char* pMemberId,
		void* pRenderWin,
		bool bMirror);

	/**
	* 加入房间.
	* 业务逻辑 在创建房间后，主动开启视频会议时调用
	* @param handle					shinevv句柄
	* @param pRoomId						房间号
	* @param eMediaType						媒体会话类型
	* @param pMemberId						成员标识符，必须在当前房间中保持唯一
	* @param pDisplayName					成员昵称
	* @param pServerIp						服务器IP
	* @param nPort							服务器端口
	* @param pToken							认证信息,此为预留接口，目前还没有认证，这里可以给一个任意字符串	
	*/
	SHINEVV_INTERFACE_API void JoinRoom(
		void* handle,
		const char* pRoomId,
		MediaType eMediaType,
		const char* pMemberId,
		const char* pDisplayName,
		const char* pServerIp,
		int nPort,
		const char* pToken);

	/**
	* 离开房间.
	* 业务逻辑 在创建房间后，主动关闭视频会议时调用
	* @param handle					shinevv句柄
	*/
	SHINEVV_INTERFACE_API void LeaveRoom(
		void* handle);


	/*******************************************
	* 以下为流媒体相关接口
	*/

	/**
	* 打开指定成员视频(包括自己)
	* 业务逻辑 默认状态下是接受所有房间成员视频，如果调用过DisableMemberVideo()后，可以通过此接口接收指定成员视频
	* @param handle						shinevv句柄
	* @param pMemberId					成员标识符
	* @param EnableMemberVideoResult	接收指定成员视频回调
	*/
	SHINEVV_INTERFACE_API void EnableMemberVideo(
		void* handle,
		const char* pMemberId,
		void(*EnableMemberVideoResult)(void* userData, bool bSucc, const char* pMemberId));

	/**
	* 关闭指定成员视频(包括自己)
	* 业务逻辑 默认状态下是接受所有房间成员视频，通过此接口可以不接收指定成员视频
	* @param handle						shinevv句柄
	* @param pMemberId					成员标识符
	* @param DisableMemberVideoResult	不接收指定成员视频回调
	*/
	SHINEVV_INTERFACE_API void DisableMemberVideo(
		void* handle,
		const char* pMemberId,
		void(*DisableMemberVideoResult)(void* userData, bool bSucc, const char* pMemberId));

	/**
	* 打开指定成员音频(包括自己)
	* 业务逻辑 默认状态下是接受所有房间成员音频，如果调用过DisableMemberAudio()后，可以通过此接口接收指定成员音频
	* @param handle						shinevv句柄
	* @param pMemberId					成员标识符
	* @param EnableMemberVideoResult	接收指定成员音频回调
	*/
	SHINEVV_INTERFACE_API void EnableMemberAudio(
		void* handle,
		const char* pMemberId,
		void(*EnableMemberAudioResult)(void* userData, bool bSucc, const char* pMemberId));

	/**
	* 关闭指定成员音频(包括自己)
	* 业务逻辑 默认状态下是接受所有房间成员音频，通过此接口可以不接收指定成员音频
	* @param handle						shinevv句柄
	* @param pMemberId					成员标识符
	* @param DisableMemberAudioResult	不接收指定成员视频回调
	*/
	SHINEVV_INTERFACE_API void DisableMemberAudio(
		void* handle,
		const char* pMemberId,
		void(*DisableMemberAudioResult)(void* userData, bool bSucc, const char* pMemberId));

	/**
	* 获取视频设备信息.
	* @param handle						shinevv句柄
	* @param GetVideoDevicesResult	获取视频设备信息回调
	*/
	SHINEVV_INTERFACE_API void GetVideoDevices(
		void* handle, 
		void(*GetVideoDevicesResult)(void* userData, const DeviceInfo sDeviceInfos[], int nDeviceNum));

	/**
	* 获取音频设备信息.
	* @param handle						shinevv句柄
	* @param GetAudioDevicesResult	获取音频设备信息回调
	*/
	SHINEVV_INTERFACE_API void GetAudioDevices(
		void* handle,
		void(*GetAudioDevicesResult)(void* userData, const DeviceInfo sDeviceInfos[], int nDeviceNum));

	/**
	* 指定摄像头.
	* @param handle						shinevv句柄
	* @param nIndex						摄像头序列号
	* @param SetVideoDeviceResult	    指定摄像头回调
	*/
	SHINEVV_INTERFACE_API void SetVideoDevice(
		void* handle,
		int nIndex,
		void(*SetVideoDeviceResult)(void* userData, bool bSucc));

	/**
	* 指定输入分辨率.
	* @param handle						shinevv句柄
	* @param pWidth						宽度
	* @param pHeight					高度
	* @param fps						帧率
	* @param OnGetOutputFormatResult	得到实际输出分辨率回调
	*/
	SHINEVV_INTERFACE_API void SetInputFormat(
		void* handle, 
		int nWidth,
		int nHeight, 
		int nfps,
		void(*OnGetOutputFormatResult)(void* userData, int nWidth, int nHeight, int fps));

	/*******************************************
	* 以下为屏幕共享接口
	*/

	/**
	* 开始屏幕共享.
	* @param handle						shinevv句柄
	*/
	SHINEVV_INTERFACE_API void EnableScreenShare(
		void* handle);

	/**
	* 停止屏幕共享.
	* @param handle						shinevv句柄
	*/
	SHINEVV_INTERFACE_API void DisableScreenShare(
		void* handle);

	/*******************************************
	* 其他接口
	*/

	/**
	* 发送用户自定义数据.
	* @param handle						shinevv句柄
	* @param data						json格式数据内容
	*/
	SHINEVV_INTERFACE_API void SendUserMessage(
		void* handle,
		const char* data);


	/**
	* 发送用户自定义数据.
	* @param handle						shinevv句柄
	* @param OnGetUserMessageHistory	消息内容回调，data为json格式
	*/
	SHINEVV_INTERFACE_API void GetUserMessageHistory(
		void* handle,
		void(*OnGetUserMessageHistory)(void* userData, const char* data));
#ifdef __cplusplus
}
#endif
#endif