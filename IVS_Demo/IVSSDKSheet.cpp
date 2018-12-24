// IVSSDKSheet.cpp : implementation file
//

#include "stdafx.h"
#include "IVS_Demo.h"
#include "IVSSDKSheet.h"
#include "IVS_SDK.h"
///���s200����
#include "DspLib.h"
#include "S200Interface.h"
#include "log.h"

// CIVSSDKSheet

IMPLEMENT_DYNAMIC(CIVSSDKSheet, CPropertySheet)
CIVSSDKSheet* g_SDKSheet=NULL;
CIVSSDKSheet::CIVSSDKSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	g_SDKSheet=this;
	InitSheet();
}

CIVSSDKSheet::CIVSSDKSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//g_SDKSheet=this;
	InitSheet();
}

CIVSSDKSheet::~CIVSSDKSheet()
{
	IVS_SDK_Logout(m_iSessionID);
	IVS_SDK_Cleanup();//�ͷ�SDK
}


void CIVSSDKSheet::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CPropertySheet::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SHOW, m_ShowBox);
}


BEGIN_MESSAGE_MAP(CIVSSDKSheet, CPropertySheet)
	ON_WM_CTLCOLOR()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()


// CIVSSDKSheet message handlers


void CIVSSDKSheet::InitSheet()
{
	m_pLang = eSDK::CLanguage::GetInstance();
	CHECK_POINTER_VOID(m_pLang)
	m_pLang->LoadLanguage(GetUserDefaultUILanguage());//���������ļ�
	m_iSessionID = SDK_SESSIONID_INIT;
	m_PlayingType = NOT_SET;
	m_ulRealPlayHandle = 0;
	m_ulPUPlayHandle = 0;
	m_ulPlatPlayHandle = 0;
	m_ulLocalPlayHandle = 0;
	m_strRealPlayDevCode = _T("");
	m_strPUPlayDevCode = _T("");
	m_strPlatPlayDevCode = _T("");
	m_strLocalFileName = _T("");
	memset(&m_RealPlayPara, 0, sizeof(IVS_REALPLAY_PARAM));
	memset(&m_PlayBackPara, 0, sizeof(IVS_PLAYBACK_PARAM));
	memset(&m_LocalPlayParam, 0, sizeof(IVS_LOCAL_PLAYBACK_PARAM));

	AddPage(&m_UsrMgrPage);//��ǩҳ
	AddPage(&m_RealPlayPage);
	AddPage(&m_TalkBackPage);
	AddPage(&m_PlayBackPage);
	AddPage(&m_DownLoadPage);
	AddPage(&m_AlarmPage);
	SetPageTitle();
	SetSheet();
}


void CIVSSDKSheet::SetPageTitle(void)
{
	//���ʻ�
	static CString strUsrMgrTab = m_pLang->GetString(GET_STRING_PARAM(IDS_USERMGR_PAGE));
	static CString strRealPlayTab = m_pLang->GetString(GET_STRING_PARAM(IDS_REALPLAY_PAGE));
	static CString strTalkBackTab = m_pLang->GetString(GET_STRING_PARAM(IDS_TALKBACK_PAGE));
	static CString strPlayBackTab = m_pLang->GetString(GET_STRING_PARAM(IDS_PLAYBACK_PAGE));
	static CString strDownLoadTab = m_pLang->GetString(GET_STRING_PARAM(IDS_DOWNLOAD_PAGE));
	static CString strAlarmTab = m_pLang->GetString(GET_STRING_PARAM(IDS_ALARM_PAGE));
	m_UsrMgrPage.m_psp.dwFlags |= PSP_USETITLE;
	m_UsrMgrPage.m_psp.pszTitle = strUsrMgrTab;
	m_RealPlayPage.m_psp.dwFlags |= PSP_USETITLE;
	m_RealPlayPage.m_psp.pszTitle = strRealPlayTab;
	m_TalkBackPage.m_psp.dwFlags |= PSP_USETITLE;
	m_TalkBackPage.m_psp.pszTitle = strTalkBackTab;
	m_PlayBackPage.m_psp.dwFlags |= PSP_USETITLE;
	m_PlayBackPage.m_psp.pszTitle = strPlayBackTab;
	m_DownLoadPage.m_psp.dwFlags |= PSP_USETITLE;
	m_DownLoadPage.m_psp.pszTitle = strDownLoadTab;
	m_AlarmPage.m_psp.dwFlags |= PSP_USETITLE;
	m_AlarmPage.m_psp.pszTitle = strAlarmTab;
}


void CIVSSDKSheet::SetSheet(void)
{
	m_UsrMgrPage.SetSheet(this);
	m_RealPlayPage.SetSheet(this);
	m_TalkBackPage.SetSheet(this);
	m_PlayBackPage.SetSheet(this);
	m_DownLoadPage.SetSheet(this);
	m_AlarmPage.SetSheet(this);
}


BOOL CIVSSDKSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// TODO:  Add your specialized code here

	//����ͼ��
	HICON icon = AfxGetApp()->LoadIcon(IDI_ICON_ESPACE_IVS);
	SetIcon(icon, TRUE);
	SetIcon(icon, FALSE);

	//������ˢ
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP_HUAWEI);
	m_Brush.CreatePatternBrush(&bm);
	bm.DeleteObject();

	//��������ҳĬ�ϰ�ť
	CWnd* pWnd = GetDlgItem(IDOK);
	pWnd->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
	pWnd = GetDlgItem(IDHELP);
	pWnd->ShowWindow(SW_HIDE);
	pWnd = GetDlgItem(ID_APPLY_NOW);
	pWnd->ShowWindow(SW_HIDE);

	//���������С�����Ӳ��Ŵ���
	const IVS_INT32 iVideoWith = 500;
	CRect BtnRect;
	pWnd->GetWindowRect(&BtnRect);
	CRect WndRect; 
	GetWindowRect(&WndRect);  
	::SetWindowPos(this->m_hWnd, HWND_TOP, 0, 0, WndRect.Width()+iVideoWith, WndRect.Height() - BtnRect.Height(), SWP_NOMOVE | SWP_NOZORDER); 


	//������Ƶ���Ŵ���
	CRect rect;
	GetClientRect(&rect);  
	m_ShowBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_DISABLED, CRect(WndRect.Width(), rect.top+26, rect.right-12, rect.bottom-13), this, IDC_EDIT_SHOW);
	////////////////////////////////////////////////////////////////////////////////////test/////////
 
	::ShowWindow(this->m_hWnd,SW_MINIMIZE);
	////////////////////////////////////////////////test

	return bResult;
}


HBRUSH CIVSSDKSheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if (GetDlgItem(IDC_EDIT_SHOW) == pWnd)
	{
		//pDC->SetBkMode(TRANSPARENT);
		return m_Brush;
	}

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}


//�ڱ�������ʾ������Ϣ
void CIVSSDKSheet::ShowTitleMsg(IVS_INT32 iRet, IVS_UINT32 IDS_OK, CString strOK, IVS_UINT32 IDS_FAIL, CString strFAIL)
{


	return;///add by rayֱ�ӷ��أ������ô�������


	CString strTitle(_T("IVS Demo   "));

	if (IVS_SUCCEED == iRet)
	{
		strTitle.Append(m_pLang->GetString(IDS_OK, strOK));
	} 
	else
	{
		CString strCode;
		strCode.Format(_T("  error : %d"), iRet);
		strTitle.Append(m_pLang->GetString(IDS_FAIL, strFAIL));
		strTitle.Append(strCode);
	}

	SetWindowText(strTitle);
}


//����SessionID
void CIVSSDKSheet::SetSessionID(IVS_INT32 iSessionID)
{
	m_iSessionID = iSessionID;
}


//��ȡSessionID
IVS_INT32 CIVSSDKSheet::GetSessionID(void)
{
	return m_iSessionID;
}

//�ص���������ֻ����¼�����غ͸澯�ϱ��¼�
void __stdcall EventCallBackFunc(IVS_INT32 iEventType, IVS_VOID* pEventBuf, IVS_UINT32 uiBufSize, IVS_VOID* pUserData)
{
	CHECK_POINTER_VOID(pEventBuf);
	CHECK_POINTER_VOID(pUserData);

	if (IVS_EVENT_DOWNLOAD_FAILED == iEventType)
	{
		STREAM_EXCEPTION_INFO* pExceptionInfo = reinterpret_cast<STREAM_EXCEPTION_INFO*>(pEventBuf);
		CHECK_POINTER_VOID(pExceptionInfo)
		if (IVS_PLAYER_RET_RTSP_NORMAL_STOP == pExceptionInfo->iExceptionCode)//��RTSP��������������Ϊ������ɵ��ź�
		{
			CIVSSDKSheet* pSheet = reinterpret_cast<CIVSSDKSheet*>(pUserData);
			CHECK_POINTER_VOID(pSheet->m_DownLoadPage.m_hWnd);
			pSheet->m_DownLoadPage.PostMessage(WM_DOWNLOADSUCCESS);//�������أ��첽������
		}
	}

	if (IVS_EVENT_REPORT_ALARM == iEventType)
	{
		CIVSSDKSheet* pSheet = reinterpret_cast<CIVSSDKSheet*>(pUserData);
		CHECK_POINTER_VOID(pSheet->m_AlarmPage.m_hWnd);
		IVS_ALARM_NOTIFY* pAlarmInfo = new IVS_ALARM_NOTIFY();
		CHECK_POINTER_VOID(pAlarmInfo);
		memcpy(pAlarmInfo, pEventBuf, sizeof(IVS_ALARM_NOTIFY));
		pSheet->m_AlarmPage.PostMessage(WM_UPDATEALARMLIST, NULL, (LPARAM)pAlarmInfo);//ˢ�¸澯��Ϣ���첽������
	}
}


//���ûص�����
void CIVSSDKSheet::SetEventCallback()
{
	IVS_SDK_SetEventCallBack(m_iSessionID, (EventCallBack)EventCallBackFunc, this);
}


//��ȡ���ž��
IVS_ULONG CIVSSDKSheet::GetPlayHandle()
{
	if (REAL_PLAY == m_PlayingType)
	{
		return m_ulRealPlayHandle;
	}
	else if (PU_PLAY_BACK == m_PlayingType)
	{
		return m_ulPUPlayHandle;
	}
	else if(PLATFORM_PLAY_BACK == m_PlayingType)
	{
		return m_ulPlatPlayHandle;
	}
	else
	{
		return m_ulLocalPlayHandle;
	}
}


//����ʵ�����Ų���
void CIVSSDKSheet::SetRealPlayPara(const CString strCameraCode, const IVS_REALPLAY_PARAM* pRealplayPara)
{
	m_strRealPlayDevCode = strCameraCode;
	memcpy(&m_RealPlayPara, pRealplayPara, sizeof(IVS_REALPLAY_PARAM));
}


//����ǰ�˻طŲ���
void CIVSSDKSheet::SetPUPlayBackPara(const CString strCameraCode, const IVS_PLAYBACK_PARAM* pPlaybackParam)
{
	m_strPUPlayDevCode = strCameraCode;
	memcpy(&m_PlayBackPara, pPlaybackParam, sizeof(IVS_PLAYBACK_PARAM));
}


//����ƽ̨�طŲ���
void CIVSSDKSheet::SetPlatPlayBackPara(const CString strCameraCode, const IVS_PLAYBACK_PARAM* pPlaybackParam)
{
	m_strPlatPlayDevCode = strCameraCode;
	memcpy(&m_PlayBackPara, pPlaybackParam, sizeof(IVS_PLAYBACK_PARAM));
}


//���ñ��ػطŲ���
void CIVSSDKSheet::SetLocalPlayBackPara(const CString strLocalFileName, const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam)
{
	m_strLocalFileName = strLocalFileName;
	memcpy(&m_LocalPlayParam, pPlaybackParam, sizeof(IVS_LOCAL_PLAYBACK_PARAM));
}


//ֹͣ���ڲ��ŵ���Ƶ
void CIVSSDKSheet::StopVideoPlay()
{
	if (REAL_PLAY == m_PlayingType)
	{
		StopRealPlay();
	}
	else if (PU_PLAY_BACK == m_PlayingType)
	{
		StopPUPlayBack();
	}
	else if(PLATFORM_PLAY_BACK == m_PlayingType)
	{
		StopPlatformPlayBack();
	}
	else if (LOCAL_PLAY_BACK == m_PlayingType)
	{
		StopLocalPlayBack();
	}
}

void __SDK_CALL fRealPlayCallBackFrametest(IVS_ULONG ulHandle, IVS_UINT32 uiStreamType, IVS_VOID* pFrameInfo, IVS_VOID* pUserData)
{
	printf("enter fRealPlayCallBackFrame!");
	printf("uiStreamType=%d",uiStreamType);
	if (MEDIA_STREAM_VIDEO==uiStreamType)
	{
		NET_VIDEO_DATA *sinChan = (NET_VIDEO_DATA *)(pUserData);
		ULONG pYuvBuf[4];

		IVS_VIDEO_FRAME* pTemp=NULL;
		pTemp=(IVS_VIDEO_FRAME*)pFrameInfo;
		pYuvBuf[0]=(ULONG)pTemp->pY;
		pYuvBuf[1]=(ULONG)pTemp->pU;
		pYuvBuf[2]=(ULONG)pTemp->pV;
		printf("pYuvBuf[0]=%d,pYuvBuf[1]=%d,pYuvBuf[2]=%d\n",pYuvBuf[0],pYuvBuf[1],pYuvBuf[2]);
		senddata(sinChan,pYuvBuf,(long)pTemp->uiWidth,(long)pTemp->uiHeight,NULL);
		
	}
	printf("leave fRealPlayCallBackFrame!");
}
//��ʼʵ��
void CIVSSDKSheet::StartRealPlay()
{
	IVS_INT32 iRet = IVS_SDK_StartRealPlay(m_iSessionID, &m_RealPlayPara, m_strRealPlayDevCode, m_ShowBox.GetSafeHwnd(), &m_ulRealPlayHandle);
	//IVS_INT32 iRet = IVS_SDK_StartRealPlayCBFrame(m_iSessionID, &m_RealPlayPara, m_strRealPlayDevCode, fRealPlayCallBackFrametest,"test", &m_ulRealPlayHandle);
	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_START_REALPLAY_OK), GET_STRING_PARAM(IDS_START_REALPLAY_FAIL));
	if (0==iRet)
	{
		GqyInsertLog(GQY_LOG_INFO,"IVS_SDK_StartRealPlay success\r\n");
	} 
	else
	{
		GqyInsertLog(GQY_LOG_INFO,"IVS_SDK_StartRealPlay failed!\r\n");
	}
	
	m_PlayingType = (IVS_SUCCEED == iRet) ? REAL_PLAY : NOT_SET;
}


//��ʼǰ��¼��ط�
void CIVSSDKSheet::StartPUPlayBack()
{
	IVS_INT32 iRet = IVS_SDK_StartPUPlayBack(m_iSessionID, m_strPUPlayDevCode, &m_PlayBackPara, m_ShowBox.GetSafeHwnd(), &m_ulPUPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_START_PUPLAYBACK_OK), GET_STRING_PARAM(IDS_START_PUPLAYBACK_FAIL));

	m_PlayingType = (IVS_SUCCEED == iRet) ? PU_PLAY_BACK : NOT_SET;
}


//��ʼƽ̨¼��ط�
void CIVSSDKSheet::StartPlatformPlayBack()
{
	IVS_INT32 iRet = IVS_SDK_StartPlatformPlayBack(m_iSessionID, m_strPlatPlayDevCode, &m_PlayBackPara, m_ShowBox.GetSafeHwnd(), &m_ulPlatPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_START_PLATPLAYBACK_OK), GET_STRING_PARAM(IDS_START_PLATPLAYBACK_FAIL));

	m_PlayingType = (IVS_SUCCEED == iRet) ? PLATFORM_PLAY_BACK : NOT_SET;
}


//��ʼ����¼��ط�
void CIVSSDKSheet::StartLocalPlayBack()
{
	IVS_INT32 iRet = IVS_SDK_StartLocalPlayBack(m_iSessionID, m_strLocalFileName, &m_LocalPlayParam, m_ShowBox.GetSafeHwnd(), &m_ulLocalPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_START_LOCALPLAYBACK_OK), GET_STRING_PARAM(IDS_START_LOCAL_PLAYBACK_FAIL));

	m_PlayingType = (IVS_SUCCEED == iRet) ? LOCAL_PLAY_BACK : NOT_SET;
}


//ֹͣʵ��
void CIVSSDKSheet::StopRealPlay()
{
	IVS_INT32 iRet = IVS_SDK_StopRealPlay(m_iSessionID, m_ulRealPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_STOP_REALPLAY_OK), GET_STRING_PARAM(IDS_STOP_REALPLAY_FAIL));

	if (IVS_SUCCEED == iRet)
	{
		m_ulRealPlayHandle = 0;
		m_PlayingType = NOT_SET;
		m_strRealPlayDevCode = _T("");
		memset(&m_RealPlayPara, 0, sizeof(IVS_REALPLAY_PARAM));
	} 
}


//ֹͣǰ��¼��ط�
void CIVSSDKSheet::StopPUPlayBack()
{
	IVS_INT32 iRet = IVS_SDK_StopPUPlayBack(m_iSessionID, m_ulPUPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_STOP_PUPLAYBACK_OK), GET_STRING_PARAM(IDS_STOP_PUPLAYBACK_FAIL));

	if (IVS_SUCCEED == iRet)
	{
		m_ulPUPlayHandle = 0;
		m_PlayingType = NOT_SET;
		m_strPUPlayDevCode = _T("");
		memset(&m_PlayBackPara, 0, sizeof(IVS_PLAYBACK_PARAM));
	} 
}


//ֹͣƽ̨¼��ط�
void CIVSSDKSheet::StopPlatformPlayBack()
{
	IVS_INT32 iRet = IVS_SDK_StopPlatformPlayBack(m_iSessionID, m_ulPlatPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_STOP_PLATPLAYBACK_OK), GET_STRING_PARAM(IDS_STOP_PLATPLAYBACK_FAIL));

	if (IVS_SUCCEED == iRet)
	{
		m_ulPlatPlayHandle = 0;
		m_PlayingType = NOT_SET;
		m_strPlatPlayDevCode = _T("");
		memset(&m_PlayBackPara, 0, sizeof(IVS_PLAYBACK_PARAM));
	} 
}


//ֹͣ����¼��ط�
void CIVSSDKSheet::StopLocalPlayBack()
{
	IVS_INT32 iRet = IVS_SDK_StopLocalPlayBack(m_iSessionID, m_ulLocalPlayHandle);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_STOP_LOCALPLAYBACK_OK), GET_STRING_PARAM(IDS_STOP_LOCAL_PLAYBACK_FAIL));
	if (IVS_SUCCEED == iRet)
	{
		m_ulLocalPlayHandle = 0;
		m_PlayingType = NOT_SET;
		memset(&m_LocalPlayParam, 0, sizeof(IVS_LOCAL_PLAYBACK_PARAM));
	} 
}


//��ͣ�ط�
void CIVSSDKSheet::PlayBackPause()
{
	IVS_INT32 iRet = IVS_SDK_PlayBackPause(m_iSessionID, GetPlayHandle());

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_PLAYBACKPAUSE_OK), GET_STRING_PARAM(IDS_PLAYBACKPAUSE_FAIL));
}


//�ָ��ط�
void CIVSSDKSheet::PlayBackResume()
{
	IVS_SDK_SetPlayBackSpeed(m_iSessionID, GetPlayHandle(), 1.0);
	IVS_INT32 iRet = IVS_SDK_PlayBackResume(m_iSessionID, GetPlayHandle());

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_PLAYBACKRESUME_OK), GET_STRING_PARAM(IDS_PLAYBACKRESUME_FIAL));
}


//�طŵ�֡����
void CIVSSDKSheet::PlayBackFrameStepForward()
{
	IVS_INT32 iRet = IVS_SDK_PlayBackFrameStepForward(m_iSessionID, GetPlayHandle());

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_FRAMESTEP_OK), GET_STRING_PARAM(IDS_FRAMESTEP_FAIL));
}


//���ûط��ٶ�
void CIVSSDKSheet::SetPlayBackSpeed(IVS_FLOAT fSpeed)
{
	IVS_INT32 iRet = IVS_SDK_SetPlayBackSpeed(m_iSessionID, GetPlayHandle(), fSpeed);

	ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_SETSPEED_OK), GET_STRING_PARAM(IDS_SETSPEED_FAIL));
}




BOOL CIVSSDKSheet::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CopyDataCheck(m_hWnd, pCopyDataStruct);
	return CPropertySheet::OnCopyData(pWnd, pCopyDataStruct);
}
