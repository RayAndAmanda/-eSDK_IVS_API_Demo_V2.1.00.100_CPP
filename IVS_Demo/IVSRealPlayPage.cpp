// IVSRealPlayPage.cpp : implementation file
//

#include "stdafx.h"
#include <algorithm>
#include "afxdialogex.h"
#include "IVS_Demo.h"
#include "IVSRealPlayPage.h"
#include "IVSSDKSheet.h"
#include "Language.h"

//////add by ray
///�������������
#include "iostream"
using namespace std;

#define  PTZ_CONTROL_PARA_LEN 2
const IVS_CHAR PTZ_CONTROL_MODE[PTZ_CONTROL_PARA_LEN]  = "2";//����ģʽ
const IVS_CHAR PTZ_CONTROL_STEP[PTZ_CONTROL_PARA_LEN] = "3";//�ٶ�3 [1-10]
const IVS_UINT32 WIDTH = 14;//����ͼ����
const IVS_UINT32 HEIGHT = 14;//����ͼ��߶�
const IVS_UINT32 IMAGE_SIZE = 6;//����ͼ�����
// CIVSRealPlayPage dialog

IMPLEMENT_DYNAMIC(CIVSRealPlayPage, CPropertyPage)
CIVSRealPlayPage* g_S200_RealPlay=NULL;
CIVSRealPlayPage::CIVSRealPlayPage()
	: CPropertyPage(CIVSRealPlayPage::IDD)
	, m_strCameraCode(_T(""))
	, m_strPresetName(_T(""))
	, m_uiPresetIndex(0)
	,m_pSheet(NULL)
	,m_hMem(NULL)
{
	m_DeviceList.clear();
	m_PresetList.clear();
	g_S200_RealPlay=this;
}

CIVSRealPlayPage::~CIVSRealPlayPage()
{
	m_imageList.Detach();
	ClearDeviceList();//����豸�б�
	ClearPresetList();//���Ԥ��λ�б�
	GlobalFree(m_hMem);
}

void CIVSRealPlayPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_DEVICE, m_treeDevice);
	DDX_Text(pDX, IDC_EDIT_CAMERA_CODE, m_strCameraCode);
	DDX_Control(pDX, IDC_TREE_PRESET, m_treePreset);
	DDX_Text(pDX, IDC_EDIT_PRESETNAME, m_strPresetName);
	DDX_Text(pDX, IDC_EDIT_PRESETINDEX, m_uiPresetIndex);
	DDX_Control(pDX, IDC_COMBO_PROTOCOL_TYPE, m_ComboProtocol);
	DDX_Control(pDX, IDC_CHECK_DIRECT_FIRST, m_checkIsDirect);
	DDX_Control(pDX, IDC_CHECK_MULTI_CAST, m_checkIsMulticast);
	DDX_Control(pDX, IDC_STATIC_LAB_REALPLAY, m_GrpRealplay);
	DDX_Control(pDX, IDC_STATIC_LAB_PTZCTRL, m_GrpPtzCtrl);
	DDX_Control(pDX, IDC_STATIC_LAB_PRESETCTRL, m_GrpPresetCtrl);
	DDX_Control(pDX, IDC_STATIC_CHOSENDEV, m_LabChosenDev);
	DDX_Control(pDX, IDC_STATIC_PRESETNAME, m_LabPresetName);
	DDX_Control(pDX, IDC_STATIC_PRESETINDEX, m_LabPresetIndex);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_BtnStartRealplay);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_BtnStopRealplay);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_IN, m_BtnZoomIn);
	DDX_Control(pDX, IDC_BUTTON_ZOOM_OUT, m_BtnZoomOut);
	DDX_Control(pDX, IDC_BUTTON_PTZ_LOCK, m_BtnPtzLock);
	DDX_Control(pDX, IDC_BUTTON_PTZ_UNLOCK, m_BtnPtzUnlock);
	DDX_Control(pDX, IDC_BUTTON_ADD_PRESET, m_BtnNewPreset);
	DDX_Control(pDX, IDC_BUTTON_DELETE_PRESET, m_BtnDelPreset);
	DDX_Control(pDX, IDC_BUTTON_UP, m_BtnPtzUp);
	DDX_Control(pDX, IDC_BUTTON_DOWN, m_BtnPtzDown);
	DDX_Control(pDX, IDC_BUTTON_LEFT, m_BtnPtzLeft);
	DDX_Control(pDX, IDC_BUTTON_RIGHT, m_BtnPtzRight);
	DDX_Control(pDX, IDC_BUTTON_UP_LEFT, m_BtnPtzUpLeft);
	DDX_Control(pDX, IDC_BUTTON_UP_RIGHT, m_BtnPtzUpRight);
	DDX_Control(pDX, IDC_BUTTON_DOWN_LEFT, m_BtnPtzDownLeft);
	DDX_Control(pDX, IDC_BUTTON_DOWN_RIGHT, m_BtnPtzDownRight);
	DDX_Control(pDX, IDC_BUTTON_PTZ_STOP, m_BtnPtzStop);
	
}


BEGIN_MESSAGE_MAP(CIVSRealPlayPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CIVSRealPlayPage::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CIVSRealPlayPage::OnBnClickedButtonStop)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DEVICE, &CIVSRealPlayPage::OnTvnSelchangedTreeDevice)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_DEVICE, &CIVSRealPlayPage::OnNMDblclkTreeDevice)
	ON_BN_CLICKED(IDC_BUTTON_PTZ_STOP, &CIVSRealPlayPage::OnBnClickedButtonPtzStop)
	ON_BN_CLICKED(IDC_BUTTON_UP_LEFT, &CIVSRealPlayPage::OnBnClickedButtonUpLeft)
	ON_BN_CLICKED(IDC_BUTTON_UP, &CIVSRealPlayPage::OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_UP_RIGHT, &CIVSRealPlayPage::OnBnClickedButtonUpRight)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, &CIVSRealPlayPage::OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, &CIVSRealPlayPage::OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_LEFT, &CIVSRealPlayPage::OnBnClickedButtonDownLeft)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, &CIVSRealPlayPage::OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_DOWN_RIGHT, &CIVSRealPlayPage::OnBnClickedButtonDownRight)
	ON_MESSAGE(WM_UPDATEDEVICELIST, &CIVSRealPlayPage::OnUpdateDeviceList)
	ON_MESSAGE(WM_UPDATEPRESETLIST, &CIVSRealPlayPage::OnUpdatePresetList)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_PRESET, &CIVSRealPlayPage::OnNMDblclkTreePreset)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_IN, &CIVSRealPlayPage::OnBnClickedButtonZoomIn)
	ON_BN_CLICKED(IDC_BUTTON_ZOOM_OUT, &CIVSRealPlayPage::OnBnClickedButtonZoomOut)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PRESET, &CIVSRealPlayPage::OnBnClickedButtonAddPreset)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_PRESET, &CIVSRealPlayPage::OnBnClickedButtonDeletePreset)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PRESET, &CIVSRealPlayPage::OnTvnSelchangedTreePreset)
	ON_BN_CLICKED(IDC_BUTTON_PTZ_LOCK, &CIVSRealPlayPage::OnBnClickedButtonPtzLock)
	ON_BN_CLICKED(IDC_BUTTON_PTZ_UNLOCK, &CIVSRealPlayPage::OnBnClickedButtonPtzUnlock)
	ON_BN_CLICKED(IDC_BUTTON_GET_CAMERAINFO, &CIVSRealPlayPage::OnBnClickedButtonGetCamerainfo)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_WINDOW_TEST, &CIVSRealPlayPage::OnBnClickedButtonOpenWindowTest)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_WINDOW_TEST, &CIVSRealPlayPage::OnBnClickedButtonCloseWindowTest)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DEVICE_GROUP, &CIVSRealPlayPage::OnBnClickedButtonSaveDeviceGroup)
END_MESSAGE_MAP()


// CIVSRealPlayPage message handlers


static void AddImage(CImageList& imageList, IVS_UINT32 uID)
{
	CBitmap bm;
	bm.LoadBitmap(uID);
	imageList.Add(&bm, RGB(0,0,0));
	bm.DeleteObject();
}


BOOL CIVSRealPlayPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	CHECK_POINTER(m_pSheet, FALSE);
	eSDK::CLanguage* pLang = m_pSheet->m_pLang;
	CHECK_POINTER(pLang, FALSE);

	// ״̬ͼƬ�б�
	m_imageList.Create(WIDTH, HEIGHT, ILC_COLORDDB, IMAGE_SIZE, 1);
	AddImage(m_imageList, IDB_BITMAP_FOLD);
	AddImage(m_imageList, IDB_BITMAP_UNFOLD);
	AddImage(m_imageList, IDB_BITMAP_BALLCAM_ON);
	AddImage(m_imageList, IDB_BITMAP_BALLCAM_OFF);
	AddImage(m_imageList, IDB_BITMAP_FIXEDCAM_ON);
	AddImage(m_imageList, IDB_BITMAP_FIXEDCAM_OFF);

	m_treeDevice.ModifyStyle(0, /*TVS_LINESATROOT|TVS_HASBUTTONS|*/TVS_HASLINES);
	m_treeDevice.SetImageList(&m_imageList, LVSIL_NORMAL);
	m_hItemDevice = m_treeDevice.InsertItem(pLang->GetString(GET_STRING_PARAM(IDS_CAMERA_LIST)));

	m_treePreset.ModifyStyle(0, TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	m_hItemPreset = m_treePreset.InsertItem(pLang->GetString(GET_STRING_PARAM(IDS_PRESET_LIST)));

	//ʵ��Э������
	m_ComboProtocol.InsertString(0, _T("UDP"));
	m_ComboProtocol.InsertString(1, _T("TCP"));
	m_ComboProtocol.SetCurSel(1);

	m_checkIsDirect.SetCheck(TRUE);//�Ƿ�ֱ��
	m_checkIsMulticast.SetCheck(FALSE);//�Ƿ��鲥

	//���ñ�ǩ����ť��ʾ����
	m_GrpRealplay.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_REALPLAY)));
	m_GrpPtzCtrl.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_PTZCTRL)));
	m_GrpPresetCtrl.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_PRESETCTRL)));
	m_LabChosenDev.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_CHOSENDEV)));
	m_LabPresetName.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_PRESETNAME)));
	m_LabPresetIndex.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_PRESETINDEX)));
	m_BtnStartRealplay.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_STARTREALPLAY)));
	m_BtnStopRealplay.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_STOPREALPLAY)));
	m_BtnZoomIn.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_ZOOMIN)));
	m_BtnZoomOut.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_ZOOMOUT)));
	m_BtnPtzLock.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_PTZLOCK)));
	m_BtnPtzUnlock.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_PTZUNLOCK)));
	m_BtnNewPreset.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_NEWPRESET)));
	m_BtnDelPreset.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_DELPRESET)));
	m_checkIsDirect.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_DIRECTFIRSE)));
	m_checkIsMulticast.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_MULTICAST)));

	//������̨���ư�ťͼƬ
	m_BtnPtzStop.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_STOP)));
	m_BtnPtzUp.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_UP)));
	m_BtnPtzDown.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DOWN)));
	m_BtnPtzLeft.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_LEFT)));
	m_BtnPtzRight.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_RIGHT)));
	m_BtnPtzUpLeft.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_UPLEFT)));
	m_BtnPtzUpRight.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_UPRIGHT)));
	m_BtnPtzDownLeft.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DOWNLEFT)));
	m_BtnPtzDownRight.SetBitmap(LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BITMAP_DOWNRIGHT)));
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//�������ԶԻ���
void CIVSRealPlayPage::SetSheet(CIVSSDKSheet* pSheet)
{
	m_pSheet = pSheet;
}


//��ʼʵ��
void CIVSRealPlayPage::OnBnClickedButtonPlay()
{
	 //TODO: Add your control notification handler code here

	IVS_REALPLAY_PARAM RealplayPara = {0};//ʵ�����Ų���
	RealplayPara.bDirectFirst = m_checkIsDirect.GetCheck();
	RealplayPara.bMultiCast = m_checkIsMulticast.GetCheck();
	RealplayPara.uiProtocolType = m_ComboProtocol.GetCurSel() + 1;
	RealplayPara.uiStreamType = STREAM_TYPE_MAIN;

	m_pSheet->StopVideoPlay();
	m_pSheet->SetRealPlayPara(m_strCameraCode, &RealplayPara);
	SendMessage(WM_UPDATEPRESETLIST);//����ʵ���������Ԥ��λ
	m_pSheet->StartRealPlay();
}


//ֹͣʵ��
void CIVSRealPlayPage::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	m_pSheet->StopRealPlay();
}


//����豸�б�
void CIVSRealPlayPage::ClearDeviceList()
{
	std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iter = m_DeviceList.begin();
	std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iterEnd = m_DeviceList.end();
	for (; iter != iterEnd; iter++)
	{
		delete[] *iter;
	}

	m_DeviceList.clear();
}


//�����豸�б�Ŀ¼���ڵ�
void CIVSRealPlayPage::OnTvnSelchangedTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treeDevice.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)
	
	IVS_CAMERA_BRIEF_INFO* pCameraInfo = reinterpret_cast<IVS_CAMERA_BRIEF_INFO*>(m_treeDevice.GetItemData(hItem));
	if (NULL != pCameraInfo)
	{
		m_strCameraCode.Format("%s", pCameraInfo->cCode);
		UpdateData(FALSE);//��ʾ�������豸�ı���

		CopyDevCodeToClipBoard(pCameraInfo->cCode, pCameraInfo->cName);
	}
}


//˫���豸�б�Ŀ¼���ڵ�
void CIVSRealPlayPage::OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treeDevice.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)

	if (m_hItemDevice == hItem)
	{
		PostMessage(WM_UPDATEDEVICELIST);//˫�����ڵ�ʱ�������豸�б�
		return;
	}

	IVS_CAMERA_BRIEF_INFO* pCameraInfo = reinterpret_cast<IVS_CAMERA_BRIEF_INFO*>(m_treeDevice.GetItemData(hItem));
	if (NULL != pCameraInfo)
	{
		m_strCameraCode.Format("%s",pCameraInfo->cCode);
		UpdateData(FALSE);

		CopyDevCodeToClipBoard(pCameraInfo->cCode, pCameraInfo->cName);

		OnBnClickedButtonPlay();//���ű�˫���豸��ʵ��
	}
}


//�����豸���뵽���а�
void CIVSRealPlayPage::CopyDevCodeToClipBoard(IVS_CHAR* pszDevCode, IVS_CHAR* pszDevName)
{
	if (OpenClipboard() && EmptyClipboard())
	{
		GlobalFree(m_hMem);
		m_hMem = GlobalAlloc(GHND, IVS_DEV_CODE_LEN * sizeof(IVS_CHAR));
		char* pBuffer = (char*)GlobalLock(m_hMem);
		memcpy(pBuffer, pszDevCode, strlen(pszDevCode));
		GlobalUnlock(m_hMem);
		SetClipboardData(CF_TEXT, m_hMem);
		CloseClipboard();

		CString strTitle(_T("IVS Demo   "));
		strTitle.Append(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_CAMERACODE_COPIED)));
		//m_pSheet->SetWindowText(strTitle);
	}
}


//�����豸�б�
afx_msg LRESULT CIVSRealPlayPage::OnUpdateDeviceList(WPARAM, LPARAM)
{
	IVS_UINT32 uiReqNum = 1000;//��ѯ����
	IVS_UINT32 uiReqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiReqNum-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//�����ڴ��С
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiReqBufSize]);//�����ڴ�
	CHECK_POINTER(pCameraInfoList, 0);
	memset(pCameraInfoList,0,uiReqBufSize);
	IVS_INDEX_RANGE IndexRange = {1, uiReqNum};//������Χ

	IVS_INT32 iRet = IVS_SDK_GetDeviceList(m_pSheet->GetSessionID(), DEVICE_TYPE_CAMERA, &IndexRange, pCameraInfoList, uiReqBufSize);//��ȡ�豸�б�
	
	if (IVS_SUCCEED == iRet)
	{
		//��ȡ�豸�б�ɹ�������
		IVS_UINT32 uiNum = (pCameraInfoList->stIndexRange.uiToIndex - pCameraInfoList->stIndexRange.uiFromIndex) + 1;
		uiNum = (uiNum > pCameraInfoList->uiTotal) ? pCameraInfoList->uiTotal : uiNum;//ʵ�ʷ��ص��豸����
		cout<<"pCameraInfoList->uiTotal:"<< pCameraInfoList->uiTotal<<endl;
		ClearDeviceList();
		for (IVS_UINT32 i=0; i<uiNum; i++)
		{
			IVS_CAMERA_BRIEF_INFO *pDevInfo = new IVS_CAMERA_BRIEF_INFO;
			CHECK_POINTER(pDevInfo, 0);
			memset(pDevInfo,0,sizeof(IVS_CAMERA_BRIEF_INFO));
			memcpy(pDevInfo, &(pCameraInfoList->stCameraBriefInfo[i]), sizeof(IVS_CAMERA_BRIEF_INFO));
			m_DeviceList.push_back(pDevInfo);
			/*cout<<"���к�:"<<i<<"==================================="<<endl;
			cout<<"cCode// �豸����"<<pCameraInfoList->stCameraBriefInfo[i].cCode    <<endl;
			cout<<"cName// ���������"<<pCameraInfoList->stCameraBriefInfo[i].cName   <<endl;
			cout<<"cDevGroupCode// �����豸�����"<<pCameraInfoList->stCameraBriefInfo[i].cDevGroupCode    <<endl;
			cout<<"cParentCode// ���豸����"<<pCameraInfoList->stCameraBriefInfo[i].cParentCode    <<endl;
			cout<<"cDomainCode// �����"<<pCameraInfoList->stCameraBriefInfo[i].cDomainCode    <<endl;
			cout<<"cDevModelType// ���豸�ͺ�"<<pCameraInfoList->stCameraBriefInfo[i].cDevModelType    <<endl;
			cout<<"cVendorType// ���豸�ͺ�"<<pCameraInfoList->stCameraBriefInfo[i].cVendorType   <<endl;
			cout<<"uiDevFormType// ���豸�ͺ�"<<pCameraInfoList->stCameraBriefInfo[i].uiDevFormType    <<endl;

			cout<<"uiType// ��������� IVS_CAMERA_TYPE"<<pCameraInfoList->stCameraBriefInfo[i].uiType    <<endl;
			cout<<"cCameraLocation// �������װλ������"<<pCameraInfoList->stCameraBriefInfo[i].cCameraLocation    <<endl;

			cout<<"uiCameraStatus// �������չ״̬"<<pCameraInfoList->stCameraBriefInfo[i].uiCameraStatus    <<endl;
			cout<<"uiStatus// �豸״̬"<<pCameraInfoList->stCameraBriefInfo[i].uiStatus    <<endl;
			cout<<"uiNetType// ��������"<<pCameraInfoList->stCameraBriefInfo[i].uiNetType   <<endl;
			cout<<"bSupportIntelligent// �Ƿ�֧�����ܷ���"<<pCameraInfoList->stCameraBriefInfo[i].bSupportIntelligent   <<endl;
			cout<<"bEnableVoice// ��·��Ƶ�Ƿ�����"<<pCameraInfoList->stCameraBriefInfo[i].bEnableVoice    <<endl;
			cout<<"cNvrCode// �豸����NVR����"<<pCameraInfoList->stCameraBriefInfo[i].cNvrCode    <<endl;
			cout<<"cDevCreateTime// �豸����ʱ��"<<pCameraInfoList->stCameraBriefInfo[i].cDevCreateTime    <<endl;
			cout<<"bIsExDomain// �Ƿ�Ϊ�ⲿ��"<<pCameraInfoList->stCameraBriefInfo[i].bIsExDomain   <<endl;
			cout<<"cDevIp[IVS_IP_LEN] // ǰ��IP"<<pCameraInfoList->stCameraBriefInfo[i].cDevIp    <<endl;
			cout<<"cReserve;"<<pCameraInfoList->stCameraBriefInfo[i].cReserve    <<endl;*/
		}
	}

	delete[] pCameraInfoList;//�ͷ��ڴ�
	pCameraInfoList = NULL;

	ShowDeviceList();//��ʾ�豸�б�
	CHECK_POINTER(m_pSheet, 0);
	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_GETDEVLIST_OK), GET_STRING_PARAM(IDS_GETDEVLISR_FAIL));
	return 0;
}


//��ʾ�豸�б�
void CIVSRealPlayPage::ShowDeviceList()
{
	m_treeDevice.DeleteAllItems();//ɾ�����нڵ�
	m_hItemDevice = m_treeDevice.InsertItem(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_CAMERA_LIST)));//��Ӹ��ڵ�

	std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iter = m_DeviceList.begin();
	std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iterEnd = m_DeviceList.end();
	for (; iter != iterEnd; iter++)
	{
		TVINSERTSTRUCT tvStruct;
		tvStruct.hParent = m_hItemDevice;
		tvStruct.hInsertAfter = NULL;
		tvStruct.item.mask = TVIF_TEXT|TVIF_PARAM;

		CString strTmp;
		strTmp.Format("%-6s", (*iter)->cName);
		
		switch ((*iter)->uiStatus)
		{
		case DEV_STATUS_ONLINE:
			strTmp += m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_ONLINE));
			break;
		case DEV_STATUS_OFFLINE:
			strTmp += m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_OFFLINE));
			break;
		case DEV_STATUS_SLEEP:
			strTmp += m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_SLEEP));
			break;
		default:
			break;
		}

		IVS_UINT32 uImageID = -1;
		if ((CAMERA_DOME == (*iter)->uiType) || (CAMERA_FIX_DOME == (*iter)->uiType))
		{
			uImageID = (DEV_STATUS_ONLINE == (*iter)->uiStatus) ? 2 : 3;//���
		} 
		else
		{
			uImageID = (DEV_STATUS_ONLINE == (*iter)->uiStatus) ? 4 : 5;//ǹ��
		}

		tvStruct.item.pszText = strTmp.GetBuffer();
		strTmp.ReleaseBuffer();
		HTREEITEM hItemSub = m_treeDevice.InsertItem(&tvStruct);//����ӽڵ�
		m_treeDevice.SetItemData(hItemSub, reinterpret_cast<DWORD_PTR>(*iter));//�����ݵ��ӽڵ�
		m_treeDevice.SetItemImage(hItemSub, uImageID, uImageID);
	}

	m_treeDevice.SetItemImage(m_hItemDevice, 1, 1);//UNFOLD
	m_treeDevice.Expand(m_hItemDevice, TVE_EXPAND);//չ��Ŀ¼��
}


//���Ԥ��λ�б�
void CIVSRealPlayPage::ClearPresetList()
{
	std::list<IVS_PTZ_PRESET*>::iterator iter = m_PresetList.begin();
	std::list<IVS_PTZ_PRESET*>::iterator iterEnd = m_PresetList.end();
	for (; iter != iterEnd; iter++)
	{
		delete[] (*iter);
		*iter = NULL;
	}

	m_PresetList.clear();
}


//����Ԥ��λ�б�Ŀ¼���ڵ�
void CIVSRealPlayPage::OnTvnSelchangedTreePreset(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treePreset.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)

	IVS_PTZ_PRESET* pPresetInfo = reinterpret_cast<IVS_PTZ_PRESET*>(m_treePreset.GetItemData(hItem));
	if (NULL != pPresetInfo)
	{
		m_uiPresetIndex = pPresetInfo->uiPresetIndex;
		UpdateData(FALSE);//��ʾ��ѡ��Ԥ��λ��������
	}
}


//˫��Ԥ��λ�б�Ŀ¼���ڵ�
void CIVSRealPlayPage::OnNMDblclkTreePreset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treePreset.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)

	if (m_hItemPreset == hItem)
	{
		PostMessage(WM_UPDATEPRESETLIST);//˫�����ڵ�ʱ������Ԥ��λ�б�
		return;
	}

	IVS_PTZ_PRESET* pPresetInfo =reinterpret_cast<IVS_PTZ_PRESET*>(m_treePreset.GetItemData(hItem));
	if (NULL != pPresetInfo)
	{
		CString strPresteIndex;
		strPresteIndex.Format("%d", pPresetInfo->uiPresetIndex);

		PtzControl(PTZ_PREFAB_BIT_RUN, strPresteIndex, "");//˫���ӽڵ�ʱ���øýڵ��Ԥ��λ
	}
}


//����Ԥ��λ�б�
afx_msg LRESULT CIVSRealPlayPage::OnUpdatePresetList(WPARAM wParam, LPARAM lParam)
{
	IVS_UINT32 uiPresetNum;
	IVS_PTZ_PRESET PresetList[IVS_MAX_PRESET_NUM] = {0};
	IVS_INT32 iRet = IVS_SDK_GetPTZPresetList(m_pSheet->GetSessionID(), m_strCameraCode, PresetList, sizeof(PresetList), &uiPresetNum);//��ȡԤ��λ�б�
	
	if (IVS_SUCCEED == iRet)
	{
		//��ȡԤ��λ�б�ɹ�������
		ClearPresetList();
		for (IVS_UINT32 i=0; i<uiPresetNum; i++)
		{
			IVS_PTZ_PRESET *pPresetInfo = new IVS_PTZ_PRESET;
			CHECK_POINTER(pPresetInfo, 0);
			memcpy(pPresetInfo, &PresetList[i], sizeof(IVS_PTZ_PRESET));
			m_PresetList.push_back(pPresetInfo);
		}
	}

	ShowPresetList();//��ʾԤ��λ�б�
	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_GETPRESET_OK), GET_STRING_PARAM(IDS_GETPRESET_FAIL));
	return 0;
}


//Ԥ��λ������򣬰�����������
static bool AscendOrder(const IVS_PTZ_PRESET* pPresetA, const IVS_PTZ_PRESET* pPresetB)
{
	return pPresetA->uiPresetIndex < pPresetB->uiPresetIndex;
}


//��ʾԤ��λ�б�
void CIVSRealPlayPage::ShowPresetList()
{
	m_treePreset.DeleteAllItems();//ɾ�����нڵ�
	m_hItemPreset = m_treePreset.InsertItem(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_PRESET_LIST)));//��Ӹ��ڵ�
	HTREEITEM hItemParent = m_hItemPreset;

	TVINSERTSTRUCT tvStruct;
	tvStruct.hParent = m_hItemPreset;
	tvStruct.hInsertAfter = NULL;
	tvStruct.item.mask = TVIF_TEXT|TVIF_PARAM;

	std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iter = m_DeviceList.begin();
	std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iterEnd = m_DeviceList.end();
	for (; iter != iterEnd; iter++)
	{
		if (0 == m_strCameraCode.Compare((*iter)->cCode))
		{
			tvStruct.item.pszText = (*iter)->cName;

			hItemParent = m_treePreset.InsertItem(&tvStruct);
			m_treeDevice.SetItemData(hItemParent, NULL);
			break;
		}
	}

	stable_sort(m_PresetList.begin(), m_PresetList.end(), AscendOrder);//��Ԥ��λ��������������

	std::list<IVS_PTZ_PRESET*>::iterator iterPreset = m_PresetList.begin();
	std::list<IVS_PTZ_PRESET*>::iterator iterEndPreset = m_PresetList.end();
	for (; iterPreset != iterEndPreset; iterPreset++)
	{
		CString strTmp;
		strTmp.Format("%3d %s", (*iterPreset)->uiPresetIndex, (*iterPreset)->cPresetName);

		tvStruct.hParent = hItemParent;
		tvStruct.item.pszText = strTmp.GetBuffer();
		strTmp.ReleaseBuffer();
		HTREEITEM hItemSub = m_treePreset.InsertItem(&tvStruct);//����ӽڵ�
		m_treeDevice.SetItemData(hItemSub, (DWORD_PTR)(*iterPreset));//������
	}

	//չ��Ŀ¼��
	m_treePreset.Expand(hItemParent, TVE_EXPAND);
	m_treePreset.Expand(m_hItemPreset, TVE_EXPAND);
}


//��̨����
void CIVSRealPlayPage::PtzControl(const IVS_INT32 iCtrlCode,const IVS_CHAR* pcPara1, const IVS_CHAR* pcPara2)
{
	IVS_UINT32 uiLockStatus = 0;
	IVS_INT32 iRet = IVS_SDK_PtzControl(m_pSheet->GetSessionID(), m_strCameraCode, iCtrlCode, pcPara1, pcPara2, &uiLockStatus);

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_PTZCTRL_OK), GET_STRING_PARAM(IDS_PTZCTRL_FAIL));
}


void CIVSRealPlayPage::OnBnClickedButtonPtzStop()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_STOP, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//ֹͣ��̨����
}


void CIVSRealPlayPage::OnBnClickedButtonUpLeft()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UP_LEFT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//����
}


void CIVSRealPlayPage::OnBnClickedButtonUp()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UP, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//��
}


void CIVSRealPlayPage::OnBnClickedButtonUpRight()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UP_RIGHT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//����
}


void CIVSRealPlayPage::OnBnClickedButtonLeft()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LEFT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//��
}


void CIVSRealPlayPage::OnBnClickedButtonRight()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_RIGHT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//��
}


void CIVSRealPlayPage::OnBnClickedButtonDownLeft()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_DOWN_LEFT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//����
}


void CIVSRealPlayPage::OnBnClickedButtonDown()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_DOWN, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//��
}


void CIVSRealPlayPage::OnBnClickedButtonDownRight()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_DOWN_RIGHT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//����
}


void CIVSRealPlayPage::OnBnClickedButtonZoomIn()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LENS_ZOOM_IN, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//�Ŵ�
}


void CIVSRealPlayPage::OnBnClickedButtonZoomOut()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LENS_ZOOM_OUT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//��С
}


void CIVSRealPlayPage::OnBnClickedButtonPtzLock()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LOCK, "", "");//��̨����
}


void CIVSRealPlayPage::OnBnClickedButtonPtzUnlock()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UNLOCK, "", "");//��̨����
}


//����Ԥ��λ
void CIVSRealPlayPage::OnBnClickedButtonAddPreset()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	IVS_UINT32 uiIndex = 0;

	IVS_INT32 iRet = IVS_SDK_AddPTZPreset(m_pSheet->GetSessionID(), m_strCameraCode, m_strPresetName, &uiIndex);

	if (IVS_SUCCEED == iRet)
	{
		SendMessage(WM_UPDATEPRESETLIST);//��ӳɹ������Ԥ��λ�б�
	} 

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_ADDPRESET_OK), GET_STRING_PARAM(IDS_ADDPRESET_FAIL));
}


//ɾ��Ԥ��λ
void CIVSRealPlayPage::OnBnClickedButtonDeletePreset()
{
	// TODO: Add your control notification handler code here
	IVS_INT32 iRet = IVS_SDK_DelPTZPreset(m_pSheet->GetSessionID(), m_strCameraCode, m_uiPresetIndex);

	if (IVS_SUCCEED == iRet)
	{
		SendMessage(WM_UPDATEPRESETLIST);//ɾ���ɹ������Ԥ��λ�б�
	} 

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_DELPRESET_OK), GET_STRING_PARAM(IDS_DELPRESET_FAIL));
}


void CIVSRealPlayPage::OnBnClickedButtonGetCamerainfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ȡ·����Ϣadd by ray
	// get domain route/////////////////////////////////////////////////////////////////////
	IVS_UINT32 uiBufSize = sizeof(IVS_DOMAIN_ROUTE_LIST) + (IVS_MAX_DOMAIN_ROUTE_NUM - 1) * sizeof(IVS_DOMAIN_ROUTE);
	IVS_DOMAIN_ROUTE_LIST* pDomainRouteList = NULL;
	try
	{
		pDomainRouteList = (IVS_DOMAIN_ROUTE_LIST*)(new IVS_CHAR[uiBufSize]);
	}
	catch (...)
	{
		return;
	}
	memset(pDomainRouteList, 0, uiBufSize);
	IVS_INT32 iRet = IVS_SDK_GetDomainRoute(m_pSheet->GetSessionID(), pDomainRouteList, uiBufSize);
	if (IVS_SUCCEED == iRet) 
	{ 
		////��ȡ·����Ϣ�ɹ�����
		 //MessageBox(_T("��ȡ·����Ϣ�ɹ���"));
		cout<<"IVS_SDK_GetDomainRoute success��"<<endl;
		char FilePath[256]={0};
		char szFileName[MAX_PATH]={0};
		HMODULE module=GetModuleHandle(0);
		GetModuleFileName(module,szFileName,MAX_PATH);
		for(int i=strlen(szFileName);i>0;i--)
		{
			if (szFileName[i]=='\\')
			{
				szFileName[i+1]='\0';
				break;
			}
		}
		strcpy_s(FilePath,sizeof(FilePath),szFileName);
		strcat_s(FilePath,sizeof(FilePath),"test\\Domain_route.ini");
		CString DomainRouteIndex;
		for (int i=0;i<pDomainRouteList->uiTotal;i++)
		{
			 DomainRouteIndex.Format(" DomainRouteIndex_%d",i);
			 ////������Ҫת��
			 CString temp;

			 temp.Empty();
			 temp.Format("%u",pDomainRouteList->stDomainRoute[i].uiDomainType);
			 WritePrivateProfileString(DomainRouteIndex,"uiDomainType",temp ,FilePath);
			 temp.Empty();
			 temp.Format("%d",pDomainRouteList->stDomainRoute[i].bIsAgent);
			 WritePrivateProfileString(DomainRouteIndex,"bIsAgent",temp ,FilePath);

			 WritePrivateProfileString(DomainRouteIndex,"cDomainCode",pDomainRouteList->stDomainRoute[i].cDomainCode ,FilePath);
			 WritePrivateProfileString(DomainRouteIndex,"cDomainName",pDomainRouteList->stDomainRoute[i].cDomainName ,FilePath);
			 WritePrivateProfileString(DomainRouteIndex,"stIP",pDomainRouteList->stDomainRoute[i].stIP.cIP ,FilePath);
			 temp.Empty();
			 temp.Format("%u",pDomainRouteList->stDomainRoute[i].uiPort);
			 WritePrivateProfileString(DomainRouteIndex,"uiPort",temp ,FilePath);
			 WritePrivateProfileString(DomainRouteIndex,"cSuperDomain",pDomainRouteList->stDomainRoute[i].cSuperDomain ,FilePath);
			 temp.Empty();
			 temp.Format("%u",pDomainRouteList->stDomainRoute[i]. bIsLocalDomain);
			 WritePrivateProfileString(DomainRouteIndex," bIsLocalDomain",temp ,FilePath);
			/* typedef struct
			 {
				 IVS_UINT32    uiDomainType;                       
				 IVS_BOOL      bIsAgent;                          
				 IVS_CHAR      cDomainCode[IVS_DOMAIN_CODE_LEN];   
				 IVS_CHAR      cDomainName[IVS_NAME_LEN];        
				 IVS_IP        stIP;                                
				 IVS_UINT32    uiPort;                           
				 IVS_CHAR      cSuperDomain[IVS_DOMAIN_CODE_LEN];    
				 IVS_BOOL      bIsLocalDomain;                     
				 IVS_CHAR      cReserve[64];
			 }IVS_DOMAIN_ROUTE;*/
			 //�����������ĵ���д���� ����pDomainRouteList->stDomainRoute[i].cDomainCode
			// Get_Device_Group(pDomainRouteList->stDomainRoute[i].cDomainName);
			 cout<<"pDomainRouteList->stDomainRoute[i].cDomainCode:"<<pDomainRouteList->stDomainRoute[i].cDomainCode<<endl;
			 IVS_CHAR	Test_cDomainCode[128];
			 memset(Test_cDomainCode,0,IVS_DOMAIN_CODE_LEN);
			 memcpy(Test_cDomainCode,pDomainRouteList->stDomainRoute[i].cDomainCode,IVS_DOMAIN_CODE_LEN);
				
			 Get_Device_Group(Test_cDomainCode);
			
		}
		// success
	}

	delete[] pDomainRouteList;
	pDomainRouteList = NULL;

	int CameraPageIndex=0;
	bool flag=true;
	while(flag)
	{
		//��ҳ��ȡ�������Ϣ
		flag=GetCameraInfo(&CameraPageIndex);
	}
	
	

}

void CIVSRealPlayPage::OnBnClickedButtonOpenWindowTest()
{

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString CCameraCode;
	CCameraCode.Format(_T("%s"),_T("03345720000000000101#571fce2fd1b544769f2ba21f6d2d34c4"));
	IVS_REALPLAY_PARAM RealplayPara = {0};//ʵ�����Ų���
	//RealplayPara.bDirectFirst = 1;
	//RealplayPara.bMultiCast = 0;
	//RealplayPara.uiProtocolType = 0;
	RealplayPara.bDirectFirst = m_checkIsDirect.GetCheck();
	RealplayPara.bMultiCast = m_checkIsMulticast.GetCheck();
	RealplayPara.uiProtocolType = m_ComboProtocol.GetCurSel() + 1;
	RealplayPara.uiStreamType = STREAM_TYPE_MAIN;

	m_pSheet->StopVideoPlay();
	m_pSheet->SetRealPlayPara(CCameraCode, &RealplayPara);
	SendMessage(WM_UPDATEPRESETLIST);//����ʵ���������Ԥ��λ
	m_pSheet->StartRealPlay();
	
	

		

}


void CIVSRealPlayPage::OnBnClickedButtonCloseWindowTest()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_pSheet->StopRealPlay();
}


bool CIVSRealPlayPage::GetCameraInfo(int *PageIndex)
{
	//add by ray ��ӻ�ȡ����ͷ��Ϣ��ť�������Ϣ���浽�ĵ��д���
	IVS_UINT32 uiReqNum = 1000;//��ѯ����
	IVS_UINT32 uiReqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiReqNum-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//�����ڴ��С
	IVS_UINT32 Camera_uiTotal=0;             
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiReqBufSize]);//�����ڴ�
	CHECK_POINTER(pCameraInfoList, 0);
	bool flag=false;
	int Camera_PageIndex=0;
	Camera_PageIndex=*PageIndex;
	memset(pCameraInfoList,0,uiReqBufSize);
	IVS_INDEX_RANGE IndexRange = {1+Camera_PageIndex*uiReqNum, uiReqNum*(1+Camera_PageIndex)};//������Χ


	IVS_INT32 iRet = IVS_SDK_GetDeviceList(m_pSheet->GetSessionID(), DEVICE_TYPE_CAMERA, &IndexRange, pCameraInfoList, uiReqBufSize);//��ȡ�豸�б�

	if (IVS_SUCCEED == iRet)
	{	cout<<" IVS_SDK_GetDeviceList success!"<<endl;
		//��ȡ�豸�б�ɹ�������
		char FilePath[256]={0};
		char szFileName[MAX_PATH]={0};
		HMODULE module=GetModuleHandle(0);
		GetModuleFileName(module,szFileName,MAX_PATH);
		for(int i=strlen(szFileName);i>0;i--)
		{
			if (szFileName[i]=='\\')
			{
				szFileName[i+1]='\0';
				break;
			}
		}
		strcpy_s(FilePath,sizeof(FilePath),szFileName);
		strcat_s(FilePath,sizeof(FilePath),"CameraInfo.ini");


		IVS_UINT32 uiNum = (pCameraInfoList->stIndexRange.uiToIndex - pCameraInfoList->stIndexRange.uiFromIndex) + 1;
		uiNum = (uiNum > pCameraInfoList->uiTotal) ? pCameraInfoList->uiTotal : uiNum;//ʵ�ʷ��ص��豸����
		cout<<"uiNumʵ�ʷ��ص��豸����:"<<uiNum<<endl;
		cout<<"pCameraInfoList->uiTotal:"<<pCameraInfoList->uiTotal<<endl;
		if(1000==uiNum)
		{//�ź�Դ��������1000����Ҫ�ٴλ�ȡ

			flag=true;
			*PageIndex++;
		} 
		else
		{
			flag=false;
		}
		cout<<"*PageIndex="<<*PageIndex<<endl;
		ClearDeviceList();
		CString CameraIndex;
		for (IVS_UINT32 i=Camera_PageIndex*uiReqNum; i<uiNum+Camera_PageIndex*uiReqNum; i++)
		{
			CameraIndex.Format("CameraIndex_%d",i);
			WritePrivateProfileString(CameraIndex,"cCode",pCameraInfoList->stCameraBriefInfo[i].cCode ,FilePath);
			WritePrivateProfileString(CameraIndex,"cName",pCameraInfoList->stCameraBriefInfo[i].cName ,FilePath);
			WritePrivateProfileString(CameraIndex,"cDevGroupCode",pCameraInfoList->stCameraBriefInfo[i].cDevGroupCode ,FilePath);
			WritePrivateProfileString(CameraIndex,"cParentCode",pCameraInfoList->stCameraBriefInfo[i].cParentCode ,FilePath);
			WritePrivateProfileString(CameraIndex,"cDomainCode",pCameraInfoList->stCameraBriefInfo[i].cDomainCode ,FilePath);

			WritePrivateProfileString(CameraIndex,"cDevModelType",pCameraInfoList->stCameraBriefInfo[i].cDevModelType ,FilePath);
			WritePrivateProfileString(CameraIndex,"cVendorType",pCameraInfoList->stCameraBriefInfo[i].cVendorType ,FilePath);
	
			////������Ҫת��
			CString temp;

			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].uiDevFormType);
			WritePrivateProfileString(CameraIndex,"uiDevFormType",temp ,FilePath);

			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].uiType );
			WritePrivateProfileString(CameraIndex,"uiType",temp,FilePath);
			
			
			WritePrivateProfileString(CameraIndex,"cCameraLocation",pCameraInfoList->stCameraBriefInfo[i].cCameraLocation ,FilePath);

			////������Ҫת��
			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].uiCameraStatus );
			WritePrivateProfileString(CameraIndex,"uiCameraStatus",temp ,FilePath);

			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].uiStatus );
			WritePrivateProfileString(CameraIndex,"uiStatus",temp ,FilePath);
			
			temp.Empty();
			temp.Format("%d",pCameraInfoList->stCameraBriefInfo[i].uiNetType );
			WritePrivateProfileString(CameraIndex,"uiNetType",temp ,FilePath);	
			
			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].bSupportIntelligent);
			WritePrivateProfileString(CameraIndex,"bSupportIntelligent",temp ,FilePath);
			
			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].bEnableVoice );
			WritePrivateProfileString(CameraIndex,"bEnableVoice",temp ,FilePath);


			WritePrivateProfileString(CameraIndex,"cNvrCode",pCameraInfoList->stCameraBriefInfo[i].cNvrCode ,FilePath);
			WritePrivateProfileString(CameraIndex,"cDevCreateTime",pCameraInfoList->stCameraBriefInfo[i].cDevCreateTime ,FilePath);
			
			////������Ҫת��
			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].bIsExDomain );
			WritePrivateProfileString(CameraIndex,"bIsExDomain",temp,FilePath);

			WritePrivateProfileString(CameraIndex,"cDevIp",pCameraInfoList->stCameraBriefInfo[i].cDevIp ,FilePath);
			WritePrivateProfileString(CameraIndex,"cReserve",pCameraInfoList->stCameraBriefInfo[i].cReserve ,FilePath);
		
			
		}
		
	}
	
	delete[] pCameraInfoList;//�ͷ��ڴ�
	pCameraInfoList = NULL;
	return flag;
}


void CIVSRealPlayPage::Get_Device_Group(const IVS_CHAR* CDomainCode)
{
	//cpp code
	// get device group list��ȡ�豸����Ϣ

	IVS_UINT32 uiBufSize = sizeof(IVS_DEVICE_GROUP_LIST) + 4999*sizeof(IVS_DEVICE_GROUP);//5000�� 
	IVS_DEVICE_GROUP_LIST* pDeviceGroupList = (IVS_DEVICE_GROUP_LIST*)(new IVS_CHAR[uiBufSize]); 
	// CHECK_POINTER_VOID(pDeviceGroupList)//ָ���п� 
	memset(pDeviceGroupList, 0, uiBufSize); 
	cout<<"CDomainCode:"<<CDomainCode<<"session:"<<m_pSheet->GetSessionID()<<endl;
	CString a(CDomainCode);

	//IVS_CHAR Test_CDomainCode[33]={0};
	//memset(Test_CDomainCode,0,33);
	//for (int i=0;i<32;i++)
	//{
	//	Test_CDomainCode[i]=CDomainCode[i];
	//}
	//
	IVS_INT32 iRet = IVS_SDK_GetDeviceGroup(m_pSheet->GetSessionID(),CDomainCode, "0", pDeviceGroupList, uiBufSize);// �����ͨ��IVS_SDK_GetDomainRoute�ӿڻ�ȡ��ϵͳĬ�ϵĸ��豸��ID�ǡ�0�������롰0���ɻ�ȡ���豸���µ��������豸�� 
	if (IVS_SUCCEED == iRet)  
	{  
		//�ɹ� ��ӡ�豸����Ϣ
		char FilePath[256]={0};
		char szFileName[MAX_PATH]={0};
		HMODULE module=GetModuleHandle(0);
		GetModuleFileName(module,szFileName,MAX_PATH);
		for(int i=strlen(szFileName);i>0;i--)
		{
			if (szFileName[i]=='\\')
			{
				szFileName[i+1]='\0';
				break;
			}
		}
		strcpy_s(FilePath,sizeof(FilePath),szFileName);
		strcat_s(FilePath,sizeof(FilePath),"test\\DeviceGroup.ini");
		//�����ǻ�ȡ��������������з�����Ϣ
		//Ϊ�˷�ֹ���ݱ���������Ҫ��֮ǰ��¼��ĩβ�������\
		/////
		int FindEnd=0;
		CString RouteIndex;
		for (FindEnd=0;;FindEnd++)
		{
			CString temp;
			IVS_DEVICE_GROUP Find_end_group;
			RouteIndex.Format("DeviceGroupIndex_%d",FindEnd);
			GetPrivateProfileString(RouteIndex,"cGroupCode", "", Find_end_group.cGroupCode, IVS_DOMAIN_CODE_LEN,FilePath);
			if (strcmp(Find_end_group.cGroupCode,"")==0)
			{
				break;
			}
		}
		//////////////////////
		CString DeviceGroupIndex;
		for (IVS_UINT32 i=0; i<pDeviceGroupList->uiTotal; i++)
		{
			DeviceGroupIndex.Format("DeviceGroupIndex_%d",i+FindEnd);
			
			WritePrivateProfileString(DeviceGroupIndex,"cGroupCode",pDeviceGroupList->stDeviceGroup[i].cGroupCode ,FilePath);
			WritePrivateProfileString(DeviceGroupIndex,"cGroupName",pDeviceGroupList->stDeviceGroup[i].cGroupName ,FilePath);
			WritePrivateProfileString(DeviceGroupIndex,"cParentGroupCode",pDeviceGroupList->stDeviceGroup[i].cParentGroupCode ,FilePath);
			WritePrivateProfileString(DeviceGroupIndex,"cDomainCode",pDeviceGroupList->stDeviceGroup[i].cDomainCode ,FilePath);
		
			////������Ҫת��
			CString temp;
			temp.Empty();
			temp.Format("%u",pDeviceGroupList->stDeviceGroup[i].bIsExDomain );
			WritePrivateProfileString(DeviceGroupIndex,"bIsExDomain",temp,FilePath);
			WritePrivateProfileString(DeviceGroupIndex,"cReserve",pDeviceGroupList->stDeviceGroup[i].cReserve ,FilePath);
		}
	
		
		//cout<<"Test_CDomainCode:"<<Test_CDomainCode<<" success!"<<endl;

	 } 
	else
	{
		
		cout<<"IVS_SDK_GetDeviceGroup failed!"<<endl;
	}
	delete[] pDeviceGroupList; 
	pDeviceGroupList = NULL;
}


void CIVSRealPlayPage::OnBnClickedButtonSaveDeviceGroup()
{
	
	Get_Device_Group(_T("571fce2fd1b544769f2ba21f6d2d34c4"));
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
