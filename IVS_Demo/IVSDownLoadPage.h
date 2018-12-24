#pragma once
#include "IVS_SDK.h"
#include "afxcmn.h"
#include "afxwin.h"


// CIVSDownLoadPage dialog
class CIVSSDKSheet;
class CIVSDownLoadPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CIVSDownLoadPage)

public:
	CIVSDownLoadPage();
	virtual ~CIVSDownLoadPage();

// Dialog Data
	enum { IDD = IDD_DLG_DOWNLOAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CIVSSDKSheet* m_pSheet;

	//���������
	CString m_strDevCode;
	//¼����ֹʱ��
	CString m_strStartTime;
	CString m_strEndTime;
	//¼�񱣴�·��
	CString m_strRecSavePath;

	//ץ�ı���·��
	CString m_strSnapSavePath;
	//ץ��ͼƬ��������
	CString m_strPicName;
	//ץ��ͼƬ�����ʽ
	CComboBox m_ComboPicFormat;

	//������Ϣ
	CString m_strDownloadInfo;
	//ƽ̨¼�����ؾ��
	IVS_ULONG m_ulPlatDlHandle;
	//ǰ��¼�����ؾ��
	IVS_ULONG m_ulPUDlHandle;
	// download NVR����
	CString m_strDLNVRCode;

	/*GroupBox & StaticLabel & Button*/
	CStatic m_GrpDownload;
	CStatic m_GrpSanpshot;
	CStatic m_LabDevCode;
	CStatic m_LabStartTime;
	CStatic m_LabEndTime;
	CStatic m_LabDlSavePath;
	CStatic m_LabPicSavePath;
	CStatic m_LabPicName;
	CButton m_BtnStartPlatDL;
	CButton m_BtnStopPlatDL;
	CButton m_BtnStartPUDL;
	CButton m_BtnStopPUDL;
	CButton m_BtnPauseDL;
	CButton m_BtnResumeDL;
	CButton m_BtnGetDLInfo;
	CButton m_BtnSnapshot;

public:
	virtual BOOL OnInitDialog();

	//�������ԶԻ���
	void SetSheet(CIVSSDKSheet* pSheet);

	//��ʼƽ̨¼������
	afx_msg void OnBnClickedButtonStartPlatformDownload();
	//ֹͣƽ̨¼������
	afx_msg void OnBnClickedButtonStopPlatformDownload();
	//��ʼǰ��¼������
	afx_msg void OnBnClickedButtonStartPuDownload();
	//ֹͣǰ��¼������
	afx_msg void OnBnClickedButtonStopPuDownload();
	//��ͣ����
	afx_msg void OnBnClickedButtonPauseDownload();
	//�ָ�����
	afx_msg void OnBnClickedButtonResumeDownload();
	//��ȡ������Ϣ
	afx_msg void OnBnClickedButtonGetDownloadInfo();
	//������ɴ�����
	afx_msg LRESULT OnDownloadSuccess(WPARAM wParam, LPARAM lParam);

	//����ͼƬץ��
	afx_msg void OnBnClickedButtonLocalSnapshot();

};
