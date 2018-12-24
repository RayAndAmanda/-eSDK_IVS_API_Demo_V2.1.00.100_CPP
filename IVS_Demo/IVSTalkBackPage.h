#pragma once
#include "IVS_SDK.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <list>


// CIVSTalkBackPage dialog
class CIVSSDKSheet;
class CIVSTalkBackPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CIVSTalkBackPage)

public:
	CIVSTalkBackPage();
	virtual ~CIVSTalkBackPage();

// Dialog Data
	enum { IDD = IDD_DLG_TALKBACK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CIVSSDKSheet* m_pSheet;

	//Э����������ѡ��
	CComboBox m_ComboProtocol;
	//�Ƿ�ѭ������ѡ��
	CComboBox m_ComboCycle;

	//�Խ��豸���Խ����
	CString m_strTalkbackDevice;
	IVS_ULONG m_ulTalkbackHandle;

	//��ӡ�ɾ���㲥�豸�ı���
	CString m_strAddDevCode;
	CString m_strDeleteDevCode;
	//�㲥�豸�б�
	CListCtrl m_listBrodcast;
	std::list<CString> m_BrodcastDevList;
	//���ع㲥�ļ�
	CString m_strBrodcastFile;

	/*GroupBox & StaticLabel & Button*/
	CStatic m_GrpTalkback;
	CStatic m_GrpBrodcastDev;
	CStatic m_GrpBrodcast;
	CStatic m_LabDevCode1;
	CStatic m_LabDevCode2;
	CStatic m_LabDevCode3;
	CStatic m_LabBrodcastFile;
	CButton m_CheckDirect;
	CButton m_BtnStartTalkback;
	CButton m_BtnStopTalkback;
	CButton m_BtnAddDevice;
	CButton m_BtnDeleteDevice;
	CButton m_BtnStartFileBrodcast;
	CButton m_BtnStopFileBrodcast;
	CButton m_BtnStartRealBrodcast;
	CButton m_BtnStopRealBrodcast;

public:
	virtual BOOL OnInitDialog();

	//�������ԶԻ���
	void SetSheet(CIVSSDKSheet* pSheet);

	//�����Խ�
	afx_msg void OnBnClickedButtonStartTalkBack();
	afx_msg void OnBnClickedButtonStopTalkBack();

	//�㲥�豸����
	afx_msg void OnBnClickedButtonAddBrodcastDevice();
	afx_msg void OnBnClickedButtonDeleteBrodcastDevice();
	afx_msg LRESULT OnUpdateBrodcastDevList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNMClickListBrodcastDev(NMHDR *pNMHDR, LRESULT *pResult);

	//�ļ��㲥
	afx_msg void OnBnClickedButtonStopFileBrodcast();
	afx_msg void OnBnClickedButtonStartFileBrodcast();

	//�����㲥
	afx_msg void OnBnClickedButtonStartRealBrodcast();
	afx_msg void OnBnClickedButtonStopRealBrodcast();
};
