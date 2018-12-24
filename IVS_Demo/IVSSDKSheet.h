#pragma once
#include "IVSRealPlayPage.h"
#include "IVSPlayBackPage.h"
#include "IVSUsrMgrPage.h"
#include "IVSDownLoadPage.h"
#include "IVSAlarmPage.h"
#include "IVSTalkBackPage.h"
#include "Language.h"
#include <list>


// CIVSSDKSheet
//CIVSSDKSheet* g_SDKSheet=NULL;
class CIVSSDKSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CIVSSDKSheet)

public:
	CIVSSDKSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CIVSSDKSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CIVSSDKSheet();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
	CEdit m_ShowBox;
	IVS_INT32 m_iSessionID;
	CBrush m_Brush;

	//��ǰ��Ƶ��������
	IVS_PLAY_TYPE m_PlayingType;

	//��Ƶ���ž��
	IVS_ULONG m_ulRealPlayHandle;
	IVS_ULONG m_ulPUPlayHandle;
	IVS_ULONG m_ulPlatPlayHandle;
	IVS_ULONG m_ulLocalPlayHandle;

	//���������
	CString m_strRealPlayDevCode;
	CString m_strPUPlayDevCode;
	CString m_strPlatPlayDevCode;

	//����¼���ļ���
	CString m_strLocalFileName;

	//ʵ����¼�񲥷Ų���
	IVS_REALPLAY_PARAM  m_RealPlayPara;
	IVS_PLAYBACK_PARAM m_PlayBackPara;
	IVS_LOCAL_PLAYBACK_PARAM m_LocalPlayParam;

public:
	eSDK::CLanguage* m_pLang;

	//��¼��ʵ�����طš����ء��澯��ǩҳ
	CIVSUsrMgrPage m_UsrMgrPage;
	CIVSRealPlayPage m_RealPlayPage;
	CIVSPlayBackPage m_PlayBackPage;
	CIVSDownLoadPage m_DownLoadPage;
	CIVSAlarmPage m_AlarmPage;
	CIVSTalkBackPage m_TalkBackPage;

public:
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void InitSheet();

	//�ڱ�������ʾ������Ϣ
	void ShowTitleMsg(IVS_INT32 iRet, IVS_UINT32 IDS_OK, CString strOK, IVS_UINT32 IDS_FAIL, CString strFAIL);

	//�������ԶԻ���
	void SetSheet(void);
	//���ñ�ǩҳ����
	void SetPageTitle(void);

	//����SessionID
	void SetSessionID(IVS_INT32 iSessionID);
	//��ȡSessionID
	IVS_INT32 GetSessionID(void);

	//��ȡ���ž��
	IVS_ULONG GetPlayHandle();

	//���ûص�����
	void SetEventCallback();

	//ֹͣ���ڲ��ŵ���Ƶ
	void StopVideoPlay();
	//��ʼʵ��
	void StartRealPlay();
	//��ʼǰ��¼��ط�
	void StartPUPlayBack();
	//��ʼƽ̨¼��ط�
	void StartPlatformPlayBack();
	//��ʼ����¼��ط�
	void StartLocalPlayBack();
	//ֹͣʵ��
	void StopRealPlay();
	//ֹͣǰ��¼��ط�
	void StopPUPlayBack();
	//ֹͣƽ̨¼��ط�
	void StopPlatformPlayBack();
	//ֹͣ����¼��ط�
	void StopLocalPlayBack();

	//����ʵ�����Ų���
	void SetRealPlayPara(const CString strCameraCode, const IVS_REALPLAY_PARAM* pRealplayPara);
	//����ǰ�˻طŲ���
	void SetPUPlayBackPara(const CString strCameraCode, const IVS_PLAYBACK_PARAM* pPlaybackParam);
	//����ƽ̨�طŲ���
	void SetPlatPlayBackPara(const CString strCameraCode, const IVS_PLAYBACK_PARAM* pPlaybackParam);
	//���ñ��ػطŲ���
	void SetLocalPlayBackPara(const CString strLocalFileName, const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam);

	//��ͣ�ط�
	void PlayBackPause();
	//�ָ��ط�
	void PlayBackResume();
	//�طŵ�֡����
	void PlayBackFrameStepForward();
	//���ûط��ٶ�
	void SetPlayBackSpeed(IVS_FLOAT fSpeed);
	
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
};


