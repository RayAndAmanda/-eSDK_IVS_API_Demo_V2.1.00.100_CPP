#pragma once
#include "afxwin.h"
#include "IVS_SDK.h"
#include "afxcmn.h"
#include <list>
#include "IVS_Demo.h"
class CIVSSDKSheet;
//CIVSRealPlayPage* g_S200_RealPlay=NULL;
class CIVSRealPlayPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CIVSRealPlayPage)

public:
	CIVSRealPlayPage();
	virtual ~CIVSRealPlayPage();

// Dialog Data
	enum { IDD = IDD_DLG_REALPLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CIVSSDKSheet* m_pSheet;

	//�豸�б�
	std::list<IVS_CAMERA_BRIEF_INFO*> m_DeviceList;

	//Ԥ��λ�б�
	std::list<IVS_PTZ_PRESET*> m_PresetList;

	//��������롢Ԥ��λ���ơ�Ԥ��λ������
	CString m_strCameraCode;
	CString m_strPresetName;
	UINT m_uiPresetIndex;

	//�豸��Ԥ��λĿ¼��
	CTreeCtrl m_treeDevice;
	CTreeCtrl m_treePreset;
	HTREEITEM m_hItemDevice;
	HTREEITEM m_hItemPreset;
	CImageList m_imageList;

	HGLOBAL m_hMem;
	//Э����������ѡ��
	CComboBox m_ComboProtocol;

	/*GroupBox & StaticLabel & Button*/
	CStatic m_GrpRealplay;
	CStatic m_GrpPtzCtrl;
	CStatic m_GrpPresetCtrl;
	CStatic m_LabChosenDev;
	CStatic m_LabPresetName;
	CStatic m_LabPresetIndex;
	CButton m_BtnStartRealplay;
	CButton m_BtnStopRealplay;
	CButton m_BtnZoomIn;
	CButton m_BtnZoomOut;
	CButton m_BtnPtzLock;
	CButton m_BtnPtzUnlock;
	CButton m_BtnNewPreset;
	CButton m_BtnDelPreset;
	CButton m_checkIsDirect;
	CButton m_checkIsMulticast;

public:
	virtual BOOL OnInitDialog();

	//�������ԶԻ���
	void SetSheet(CIVSSDKSheet* pSheet);

	//��ʼʵ��
	afx_msg void OnBnClickedButtonPlay();
	//ֹͣʵ��
	afx_msg void OnBnClickedButtonStop();

	//����豸�б�
	void ClearDeviceList();
	//��ʾ�豸�б�
	void ShowDeviceList();
	//�����豸�б�
	afx_msg LRESULT OnUpdateDeviceList(WPARAM wParam, LPARAM lParam);
	//�����豸�б�Ŀ¼���ڵ�
	afx_msg void OnTvnSelchangedTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
	//˫���豸�б�Ŀ¼���ڵ�
	afx_msg void OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult);
	//�����豸���뵽���а�
	void CopyDevCodeToClipBoard(IVS_CHAR* pszDevCode, IVS_CHAR* pszDevName);

	//��̨����
	void PtzControl(const IVS_INT32 iCtrlCode,const IVS_CHAR* pcPara1,const IVS_CHAR* pcPara2);
	afx_msg void OnBnClickedButtonPtzStop();//ֹͣ��̨����
	afx_msg void OnBnClickedButtonUpLeft();//����
	afx_msg void OnBnClickedButtonUp();//��
	afx_msg void OnBnClickedButtonUpRight();//����
	afx_msg void OnBnClickedButtonLeft();//��
	afx_msg void OnBnClickedButtonRight();//��
	afx_msg void OnBnClickedButtonDownLeft();//����
	afx_msg void OnBnClickedButtonDown();//��
	afx_msg void OnBnClickedButtonDownRight();//����
	afx_msg void OnBnClickedButtonZoomIn();//�Ŵ�
	afx_msg void OnBnClickedButtonZoomOut();//��С
	afx_msg void OnBnClickedButtonPtzLock();//��̨����
	afx_msg void OnBnClickedButtonPtzUnlock();//��̨����

	//���Ԥ��λ�б�
	void ClearPresetList();
	//��ʾԤ��λ�б�
	void ShowPresetList();
	//����Ԥ��λ�б�
	afx_msg LRESULT OnUpdatePresetList(WPARAM wParam, LPARAM lParam);
	//����Ԥ��λ�б�Ŀ¼���ڵ�
	afx_msg void OnTvnSelchangedTreePreset(NMHDR *pNMHDR, LRESULT *pResult);
	//˫��Ԥ��λ�б�Ŀ¼���ڵ�
	afx_msg void OnNMDblclkTreePreset(NMHDR *pNMHDR, LRESULT *pResult);
	//����Ԥ��λ
	afx_msg void OnBnClickedButtonAddPreset();
	//ɾ��Ԥ��λ
	afx_msg void OnBnClickedButtonDeletePreset();
	CButton m_BtnPtzUp;
	CButton m_BtnPtzDown;
	CButton m_BtnPtzLeft;
	CButton m_BtnPtzRight;
	CButton m_BtnPtzUpLeft;
	CButton m_BtnPtzUpRight;
	CButton m_BtnPtzDownLeft;
	CButton m_BtnPtzDownRight;
	CButton m_BtnPtzStop;
	afx_msg void OnBnClickedButtonGetCamerainfo();
	afx_msg void OnBnClickedButtonOpenWindowTest();
	
	afx_msg void OnBnClickedButtonCloseWindowTest();
	bool GetCameraInfo(int *PageIndex);
	void Get_Device_Group(const IVS_CHAR* CDomainCode);
	afx_msg void OnBnClickedButtonSaveDeviceGroup();
};
