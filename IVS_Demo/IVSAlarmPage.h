#pragma once
#include "IVS_SDK.h"
#include "afxcmn.h"
#include <list>
#include <map>
#include "afxwin.h"


// CIVSAlarmPage dialog
class CIVSSDKSheet;
class CIVSAlarmPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CIVSAlarmPage)

public:
	CIVSAlarmPage();
	virtual ~CIVSAlarmPage();

// Dialog Data
	enum { IDD = IDD_DLG_ALARMPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CIVSSDKSheet* m_pSheet;

	//�澯��Ŀ¼�͸澯�豸��Ŀ¼
	CListCtrl m_listAlarm;
	CTreeCtrl m_treeAlarmDevice;
	HTREEITEM m_hItemRoot;

	//�豸�б�
	std::list<IVS_CAMERA_BRIEF_INFO*> m_DeviceList;
	std::map<HTREEITEM, IVS_CAMERA_BRIEF_INFO*> m_DeviceItemMap;//key:���ڵ� value:�豸��Ϣ

	//�澯���𻬸�
	CSliderCtrl m_SliderMin;
	CSliderCtrl m_SliderMax;
	CString m_strMinLevel;
	CString m_strMaxLevel;

	/*GroupBox & StaticLabel & Button*/
	CButton m_CheckMoveDection;
	CButton m_CheckShield;
	CButton m_CheckNoSignal;
	CButton m_CheckShake;
	CButton m_CheckRecognition;
	CButton m_CheckIntrusion;
	CButton m_CheckByAlarmType;
	CButton m_CheckByAlarmLevel;
	CButton m_CheckSubscribeAll;
	CStatic m_GrpSubscribe;
	CStatic m_GrpAlarmReport;
	CStatic m_LabMinLevel;
	CStatic m_LabMaxLevel;
	CButton m_BtnSubscribe;

public:
	virtual BOOL OnInitDialog();

	//�������ԶԻ���
	void SetSheet(CIVSSDKSheet* pSheet);

	//ˢ���豸�б�
	afx_msg LRESULT OnUpdateDeviceList(WPARAM wParam, LPARAM lParam);
	//����豸�б�
	void ClearDeviceList();
	//��ʾ�豸�б�
	void ShowDeviceList();
	afx_msg void OnNMDblclkTreeAlarmDevice(NMHDR *pNMHDR, LRESULT *pResult);

	//���ͻ�����ʵʱ��ʾ�澯����
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	//�澯�������Ϳ���
	afx_msg void OnBnClickedCheckByAlarmType();
	afx_msg void OnBnClickedCheckByAlarmLevel();
	afx_msg void OnBnClickedCheckSubscribeAll();
	void EnableAlarmTypeCheckbox(BOOL bCheck);

	//��ȡ������Ϣ
	CString GetSubscribeInfo();
	//�澯����
	afx_msg void OnBnClickedButtonSubscribeAlarm();
	//ˢ�¸澯�б�
	afx_msg LRESULT OnUpdateAlarmList(WPARAM wParam, LPARAM lParam);
};
