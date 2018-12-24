#pragma once
#include "IVS_SDK.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <string>
#include <list>
#include <map>


// CIVSPlayBackPage dialog
class CIVSSDKSheet;
class CIVSPlayBackPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CIVSPlayBackPage)

public:
	CIVSPlayBackPage();
	virtual ~CIVSPlayBackPage();

// Dialog Data
	enum { IDD = IDD_DLG_PALYBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CIVSSDKSheet* m_pSheet;

	//��ѯ��ֹʱ��
	CString m_strStartTime;
	CString m_strEndTime;

	//¼��Ŀ¼��
	CTreeCtrl m_treeRecord;
	HTREEITEM m_hItemRecord;

	//¼��ʽ
	CComboBox m_ComboRecordMethod;

	//�طŲ���
	IVS_PLAYBACK_PARAM m_PlaybackParam;
	//�ط����������
	CString m_strCameraCode;
	//�ط���ֹʱ��
	CString m_strPlayDuration;
	//����¼���ļ�
	CString m_strLocalRecPath;

	//ƽ̨¼�����������
	CString m_strPlatRecDevCode;
	//ǰ��¼�����������
	CString m_strPURecDevCode;
	//����·��
	CString m_strSavePath;
	//¼��ʱ��
	IVS_INT32 m_iRecordLength;
	// playback NVR����
	CString m_strPBNVRCode;

	//¼���豸�б�
	std::list<IVS_RECORD_INFO*> m_RecordList;
	std::list<IVS_CAMERA_BRIEF_INFO*> m_DeviceList;
	std::map<HTREEITEM, HTREEITEM> m_RecordMap;//key:¼�����ڵ㣬value:��¼�����ڵ������ĸ��ڵ㣨��������ڵ㣩
	std::map<HTREEITEM, IVS_CAMERA_BRIEF_INFO*> m_DeviceMap;//key:��������ڵ㣬value:��Ӧ���������Ϣ
	std::map<IVS_CAMERA_BRIEF_INFO*, std::list<IVS_RECORD_INFO*>> m_DeviceRecordMap;//key:�������Ϣ��value:���������¼���б�

	/*GroupBox & StaticLabel & Button*/
	CStatic m_GrpSearchRec;
	CStatic m_GrpPlayback;
	CStatic m_GrpRecord;
	CStatic m_LabStartTime;
	CStatic m_LabEndTime;
	CStatic m_LabLocalRecFile;
	CStatic m_LabPlatDevCode;
	CStatic m_LabPUDevCode;
	CStatic m_LabRecLength;
	CStatic m_LabSavePath;
	CButton m_BtnSearchRec;
	CButton m_BtnStartPlatPlay;
	CButton m_BtnStopPlatPlay;
	CButton m_BtnStartPUPlay;
	CButton m_BtnStopPUPlay;
	CButton m_BtnStartLocalPlay;
	CButton m_BtnStopLocalPlay;
	CButton m_BtnPalyPause;
	CButton m_BtnPlayResume;
	CButton m_BtnFastPlay;
	CButton m_BtnRewindPlay;
	CButton m_BtnFrameStep;
	CButton m_BtnStartPlatRec;
	CButton m_BtnStopPlatRec;
	CButton m_BtnStartPuRec;
	CButton m_BtnStopPURec;
	CButton m_BtnStartLocalRec;
	CButton m_BtnStopLocalRec;

public:
	virtual BOOL OnInitDialog();

	//�������ԶԻ���
	void SetSheet(CIVSSDKSheet* pSheet);

	//����б�
	void ClearList();
	//��ʾ¼���б�
	void ShowRecordList();
	//��ȡ�豸�б�
	IVS_INT32 GetDeviceList();
	//��ȡ¼���б�
	afx_msg void OnBnClickedButtonGetRecordList();
	//����¼���б�Ŀ¼���ڵ�
	afx_msg void OnTvnSelchangedTreeRecordList(NMHDR *pNMHDR, LRESULT *pResult);
	//˫��¼���б�Ŀ¼���ڵ�
	afx_msg void OnNMDblclkTreeRecordList(NMHDR *pNMHDR, LRESULT *pResult);
	//����¼���б�
	afx_msg LRESULT OnUpdateRecordList(WPARAM wParam, LPARAM lParam);

	//��ʼԶ�̣�ƽ̨��ǰ�ˣ�¼��ط�
	void StartRemotePlayBack();
	//��ʼƽ̨¼��ط�
	afx_msg void OnBnClickedButtonStartPlatformPlayBack();
	//ֹͣƽ̨¼��ط�
	afx_msg void OnBnClickedButtonStopPlatformPlayBack();
	//��ʼǰ��¼��ط�
	afx_msg void OnBnClickedButtonStartPuPlayBack();
	//ֹͣǰ��¼��ط�
	afx_msg void OnBnClickedButtonStopPuPlayBack();
	//��ʼ����¼��ط�
	afx_msg void OnBnClickedButtonStartLocalPlayBack();
	//ֹͣ����¼��ط�
	afx_msg void OnBnClickedButtonStopLocalPlayBack();

	//��ͣ�ط�
	afx_msg void OnBnClickedButtonPlayBackPause();
	//�ָ��ط�
	afx_msg void OnBnClickedButtonPlayBackResume();
	//���ٲ���
	afx_msg void OnBnClickedButtonFastPlayBack();
	//���˲���
	afx_msg void OnBnClickedButtonRewindPlay();
	//��֡����
	afx_msg void OnBnClickedButtonFramePlay();

	//��ʼ����¼��
	afx_msg void OnBnClickedButtonStartLocalRecord();
	//ֹͣ����¼��
	afx_msg void OnBnClickedButtonStopLocalRecord();
	//��ʼƽ̨¼��
	afx_msg void OnBnClickedButtonStartPlatformRecord();
	//ֹͣƽ̨¼��
	afx_msg void OnBnClickedButtonStopPlatformRecord();
	//��ʼǰ��¼��
	afx_msg void OnBnClickedButtonStartPuRecord();
	//ֹͣǰ��¼��
	afx_msg void OnBnClickedButtonStopPuRecord();

};
