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
///增加输入输出流
#include "iostream"
using namespace std;

#define  PTZ_CONTROL_PARA_LEN 2
const IVS_CHAR PTZ_CONTROL_MODE[PTZ_CONTROL_PARA_LEN]  = "2";//连续模式
const IVS_CHAR PTZ_CONTROL_STEP[PTZ_CONTROL_PARA_LEN] = "3";//速度3 [1-10]
const IVS_UINT32 WIDTH = 14;//关联图像宽度
const IVS_UINT32 HEIGHT = 14;//关联图像高度
const IVS_UINT32 IMAGE_SIZE = 6;//关联图像个数
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
	ClearDeviceList();//清空设备列表
	ClearPresetList();//清空预置位列表
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

	// 状态图片列表
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

	//实况协议类型
	m_ComboProtocol.InsertString(0, _T("UDP"));
	m_ComboProtocol.InsertString(1, _T("TCP"));
	m_ComboProtocol.SetCurSel(1);

	m_checkIsDirect.SetCheck(TRUE);//是否直连
	m_checkIsMulticast.SetCheck(FALSE);//是否组播

	//设置标签、按钮显示名称
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

	//设置云台控制按钮图片
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


//设置属性对话框
void CIVSRealPlayPage::SetSheet(CIVSSDKSheet* pSheet)
{
	m_pSheet = pSheet;
}


//开始实况
void CIVSRealPlayPage::OnBnClickedButtonPlay()
{
	 //TODO: Add your control notification handler code here

	IVS_REALPLAY_PARAM RealplayPara = {0};//实况播放参数
	RealplayPara.bDirectFirst = m_checkIsDirect.GetCheck();
	RealplayPara.bMultiCast = m_checkIsMulticast.GetCheck();
	RealplayPara.uiProtocolType = m_ComboProtocol.GetCurSel() + 1;
	RealplayPara.uiStreamType = STREAM_TYPE_MAIN;

	m_pSheet->StopVideoPlay();
	m_pSheet->SetRealPlayPara(m_strCameraCode, &RealplayPara);
	SendMessage(WM_UPDATEPRESETLIST);//更新实况摄像机的预置位
	m_pSheet->StartRealPlay();
}


//停止实况
void CIVSRealPlayPage::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	m_pSheet->StopRealPlay();
}


//清空设备列表
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


//单击设备列表目录树节点
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
		UpdateData(FALSE);//显示被单击设备的编码

		CopyDevCodeToClipBoard(pCameraInfo->cCode, pCameraInfo->cName);
	}
}


//双击设备列表目录树节点
void CIVSRealPlayPage::OnNMDblclkTreeDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treeDevice.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)

	if (m_hItemDevice == hItem)
	{
		PostMessage(WM_UPDATEDEVICELIST);//双击根节点时，更新设备列表
		return;
	}

	IVS_CAMERA_BRIEF_INFO* pCameraInfo = reinterpret_cast<IVS_CAMERA_BRIEF_INFO*>(m_treeDevice.GetItemData(hItem));
	if (NULL != pCameraInfo)
	{
		m_strCameraCode.Format("%s",pCameraInfo->cCode);
		UpdateData(FALSE);

		CopyDevCodeToClipBoard(pCameraInfo->cCode, pCameraInfo->cName);

		OnBnClickedButtonPlay();//播放被双击设备的实况
	}
}


//拷贝设备编码到剪切板
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


//更新设备列表
afx_msg LRESULT CIVSRealPlayPage::OnUpdateDeviceList(WPARAM, LPARAM)
{
	IVS_UINT32 uiReqNum = 1000;//查询数量
	IVS_UINT32 uiReqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiReqNum-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//所需内存大小
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiReqBufSize]);//分配内存
	CHECK_POINTER(pCameraInfoList, 0);
	memset(pCameraInfoList,0,uiReqBufSize);
	IVS_INDEX_RANGE IndexRange = {1, uiReqNum};//索引范围

	IVS_INT32 iRet = IVS_SDK_GetDeviceList(m_pSheet->GetSessionID(), DEVICE_TYPE_CAMERA, &IndexRange, pCameraInfoList, uiReqBufSize);//获取设备列表
	
	if (IVS_SUCCEED == iRet)
	{
		//获取设备列表成功，保存
		IVS_UINT32 uiNum = (pCameraInfoList->stIndexRange.uiToIndex - pCameraInfoList->stIndexRange.uiFromIndex) + 1;
		uiNum = (uiNum > pCameraInfoList->uiTotal) ? pCameraInfoList->uiTotal : uiNum;//实际返回的设备个数
		cout<<"pCameraInfoList->uiTotal:"<< pCameraInfoList->uiTotal<<endl;
		ClearDeviceList();
		for (IVS_UINT32 i=0; i<uiNum; i++)
		{
			IVS_CAMERA_BRIEF_INFO *pDevInfo = new IVS_CAMERA_BRIEF_INFO;
			CHECK_POINTER(pDevInfo, 0);
			memset(pDevInfo,0,sizeof(IVS_CAMERA_BRIEF_INFO));
			memcpy(pDevInfo, &(pCameraInfoList->stCameraBriefInfo[i]), sizeof(IVS_CAMERA_BRIEF_INFO));
			m_DeviceList.push_back(pDevInfo);
			/*cout<<"序列号:"<<i<<"==================================="<<endl;
			cout<<"cCode// 设备编码"<<pCameraInfoList->stCameraBriefInfo[i].cCode    <<endl;
			cout<<"cName// 摄像机名称"<<pCameraInfoList->stCameraBriefInfo[i].cName   <<endl;
			cout<<"cDevGroupCode// 所属设备组编码"<<pCameraInfoList->stCameraBriefInfo[i].cDevGroupCode    <<endl;
			cout<<"cParentCode// 父设备编码"<<pCameraInfoList->stCameraBriefInfo[i].cParentCode    <<endl;
			cout<<"cDomainCode// 域编码"<<pCameraInfoList->stCameraBriefInfo[i].cDomainCode    <<endl;
			cout<<"cDevModelType// 主设备型号"<<pCameraInfoList->stCameraBriefInfo[i].cDevModelType    <<endl;
			cout<<"cVendorType// 主设备型号"<<pCameraInfoList->stCameraBriefInfo[i].cVendorType   <<endl;
			cout<<"uiDevFormType// 主设备型号"<<pCameraInfoList->stCameraBriefInfo[i].uiDevFormType    <<endl;

			cout<<"uiType// 摄像机类型 IVS_CAMERA_TYPE"<<pCameraInfoList->stCameraBriefInfo[i].uiType    <<endl;
			cout<<"cCameraLocation// 摄像机安装位置描述"<<pCameraInfoList->stCameraBriefInfo[i].cCameraLocation    <<endl;

			cout<<"uiCameraStatus// 摄像机扩展状态"<<pCameraInfoList->stCameraBriefInfo[i].uiCameraStatus    <<endl;
			cout<<"uiStatus// 设备状态"<<pCameraInfoList->stCameraBriefInfo[i].uiStatus    <<endl;
			cout<<"uiNetType// 网络类型"<<pCameraInfoList->stCameraBriefInfo[i].uiNetType   <<endl;
			cout<<"bSupportIntelligent// 是否支持智能分析"<<pCameraInfoList->stCameraBriefInfo[i].bSupportIntelligent   <<endl;
			cout<<"bEnableVoice// 随路音频是否启用"<<pCameraInfoList->stCameraBriefInfo[i].bEnableVoice    <<endl;
			cout<<"cNvrCode// 设备所属NVR编码"<<pCameraInfoList->stCameraBriefInfo[i].cNvrCode    <<endl;
			cout<<"cDevCreateTime// 设备创建时间"<<pCameraInfoList->stCameraBriefInfo[i].cDevCreateTime    <<endl;
			cout<<"bIsExDomain// 是否为外部域"<<pCameraInfoList->stCameraBriefInfo[i].bIsExDomain   <<endl;
			cout<<"cDevIp[IVS_IP_LEN] // 前端IP"<<pCameraInfoList->stCameraBriefInfo[i].cDevIp    <<endl;
			cout<<"cReserve;"<<pCameraInfoList->stCameraBriefInfo[i].cReserve    <<endl;*/
		}
	}

	delete[] pCameraInfoList;//释放内存
	pCameraInfoList = NULL;

	ShowDeviceList();//显示设备列表
	CHECK_POINTER(m_pSheet, 0);
	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_GETDEVLIST_OK), GET_STRING_PARAM(IDS_GETDEVLISR_FAIL));
	return 0;
}


//显示设备列表
void CIVSRealPlayPage::ShowDeviceList()
{
	m_treeDevice.DeleteAllItems();//删除所有节点
	m_hItemDevice = m_treeDevice.InsertItem(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_CAMERA_LIST)));//添加根节点

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
			uImageID = (DEV_STATUS_ONLINE == (*iter)->uiStatus) ? 2 : 3;//球机
		} 
		else
		{
			uImageID = (DEV_STATUS_ONLINE == (*iter)->uiStatus) ? 4 : 5;//枪机
		}

		tvStruct.item.pszText = strTmp.GetBuffer();
		strTmp.ReleaseBuffer();
		HTREEITEM hItemSub = m_treeDevice.InsertItem(&tvStruct);//添加子节点
		m_treeDevice.SetItemData(hItemSub, reinterpret_cast<DWORD_PTR>(*iter));//绑定数据到子节点
		m_treeDevice.SetItemImage(hItemSub, uImageID, uImageID);
	}

	m_treeDevice.SetItemImage(m_hItemDevice, 1, 1);//UNFOLD
	m_treeDevice.Expand(m_hItemDevice, TVE_EXPAND);//展开目录树
}


//清空预置位列表
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


//单击预置位列表目录树节点
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
		UpdateData(FALSE);//显示被选中预置位的索引号
	}
}


//双击预置位列表目录树节点
void CIVSRealPlayPage::OnNMDblclkTreePreset(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treePreset.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)

	if (m_hItemPreset == hItem)
	{
		PostMessage(WM_UPDATEPRESETLIST);//双击根节点时，更新预置位列表
		return;
	}

	IVS_PTZ_PRESET* pPresetInfo =reinterpret_cast<IVS_PTZ_PRESET*>(m_treePreset.GetItemData(hItem));
	if (NULL != pPresetInfo)
	{
		CString strPresteIndex;
		strPresteIndex.Format("%d", pPresetInfo->uiPresetIndex);

		PtzControl(PTZ_PREFAB_BIT_RUN, strPresteIndex, "");//双击子节点时调用该节点的预置位
	}
}


//更新预置位列表
afx_msg LRESULT CIVSRealPlayPage::OnUpdatePresetList(WPARAM wParam, LPARAM lParam)
{
	IVS_UINT32 uiPresetNum;
	IVS_PTZ_PRESET PresetList[IVS_MAX_PRESET_NUM] = {0};
	IVS_INT32 iRet = IVS_SDK_GetPTZPresetList(m_pSheet->GetSessionID(), m_strCameraCode, PresetList, sizeof(PresetList), &uiPresetNum);//获取预置位列表
	
	if (IVS_SUCCEED == iRet)
	{
		//获取预置位列表成功，保存
		ClearPresetList();
		for (IVS_UINT32 i=0; i<uiPresetNum; i++)
		{
			IVS_PTZ_PRESET *pPresetInfo = new IVS_PTZ_PRESET;
			CHECK_POINTER(pPresetInfo, 0);
			memcpy(pPresetInfo, &PresetList[i], sizeof(IVS_PTZ_PRESET));
			m_PresetList.push_back(pPresetInfo);
		}
	}

	ShowPresetList();//显示预置位列表
	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_GETPRESET_OK), GET_STRING_PARAM(IDS_GETPRESET_FAIL));
	return 0;
}


//预置位排序规则，按索引号升序
static bool AscendOrder(const IVS_PTZ_PRESET* pPresetA, const IVS_PTZ_PRESET* pPresetB)
{
	return pPresetA->uiPresetIndex < pPresetB->uiPresetIndex;
}


//显示预置位列表
void CIVSRealPlayPage::ShowPresetList()
{
	m_treePreset.DeleteAllItems();//删除所有节点
	m_hItemPreset = m_treePreset.InsertItem(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_PRESET_LIST)));//添加根节点
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

	stable_sort(m_PresetList.begin(), m_PresetList.end(), AscendOrder);//按预置位索引号升序排序

	std::list<IVS_PTZ_PRESET*>::iterator iterPreset = m_PresetList.begin();
	std::list<IVS_PTZ_PRESET*>::iterator iterEndPreset = m_PresetList.end();
	for (; iterPreset != iterEndPreset; iterPreset++)
	{
		CString strTmp;
		strTmp.Format("%3d %s", (*iterPreset)->uiPresetIndex, (*iterPreset)->cPresetName);

		tvStruct.hParent = hItemParent;
		tvStruct.item.pszText = strTmp.GetBuffer();
		strTmp.ReleaseBuffer();
		HTREEITEM hItemSub = m_treePreset.InsertItem(&tvStruct);//添加子节点
		m_treeDevice.SetItemData(hItemSub, (DWORD_PTR)(*iterPreset));//绑定数据
	}

	//展开目录树
	m_treePreset.Expand(hItemParent, TVE_EXPAND);
	m_treePreset.Expand(m_hItemPreset, TVE_EXPAND);
}


//云台控制
void CIVSRealPlayPage::PtzControl(const IVS_INT32 iCtrlCode,const IVS_CHAR* pcPara1, const IVS_CHAR* pcPara2)
{
	IVS_UINT32 uiLockStatus = 0;
	IVS_INT32 iRet = IVS_SDK_PtzControl(m_pSheet->GetSessionID(), m_strCameraCode, iCtrlCode, pcPara1, pcPara2, &uiLockStatus);

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_PTZCTRL_OK), GET_STRING_PARAM(IDS_PTZCTRL_FAIL));
}


void CIVSRealPlayPage::OnBnClickedButtonPtzStop()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_STOP, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//停止云台动作
}


void CIVSRealPlayPage::OnBnClickedButtonUpLeft()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UP_LEFT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//左上
}


void CIVSRealPlayPage::OnBnClickedButtonUp()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UP, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//上
}


void CIVSRealPlayPage::OnBnClickedButtonUpRight()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UP_RIGHT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//右上
}


void CIVSRealPlayPage::OnBnClickedButtonLeft()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LEFT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//左
}


void CIVSRealPlayPage::OnBnClickedButtonRight()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_RIGHT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//右
}


void CIVSRealPlayPage::OnBnClickedButtonDownLeft()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_DOWN_LEFT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//左下
}


void CIVSRealPlayPage::OnBnClickedButtonDown()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_DOWN, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//下
}


void CIVSRealPlayPage::OnBnClickedButtonDownRight()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_DOWN_RIGHT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//右下
}


void CIVSRealPlayPage::OnBnClickedButtonZoomIn()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LENS_ZOOM_IN, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//放大
}


void CIVSRealPlayPage::OnBnClickedButtonZoomOut()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LENS_ZOOM_OUT, PTZ_CONTROL_MODE, PTZ_CONTROL_STEP);//缩小
}


void CIVSRealPlayPage::OnBnClickedButtonPtzLock()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_LOCK, "", "");//云台加锁
}


void CIVSRealPlayPage::OnBnClickedButtonPtzUnlock()
{
	// TODO: Add your control notification handler code here
	PtzControl(PTZ_UNLOCK, "", "");//云台解锁
}


//增加预置位
void CIVSRealPlayPage::OnBnClickedButtonAddPreset()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	IVS_UINT32 uiIndex = 0;

	IVS_INT32 iRet = IVS_SDK_AddPTZPreset(m_pSheet->GetSessionID(), m_strCameraCode, m_strPresetName, &uiIndex);

	if (IVS_SUCCEED == iRet)
	{
		SendMessage(WM_UPDATEPRESETLIST);//添加成功后更新预置位列表
	} 

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_ADDPRESET_OK), GET_STRING_PARAM(IDS_ADDPRESET_FAIL));
}


//删除预置位
void CIVSRealPlayPage::OnBnClickedButtonDeletePreset()
{
	// TODO: Add your control notification handler code here
	IVS_INT32 iRet = IVS_SDK_DelPTZPreset(m_pSheet->GetSessionID(), m_strCameraCode, m_uiPresetIndex);

	if (IVS_SUCCEED == iRet)
	{
		SendMessage(WM_UPDATEPRESETLIST);//删除成功后更新预置位列表
	} 

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_DELPRESET_OK), GET_STRING_PARAM(IDS_DELPRESET_FAIL));
}


void CIVSRealPlayPage::OnBnClickedButtonGetCamerainfo()
{
	// TODO: 在此添加控件通知处理程序代码
	//获取路由信息add by ray
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
		////获取路由信息成功保存
		 //MessageBox(_T("获取路由信息成功！"));
		cout<<"IVS_SDK_GetDomainRoute success！"<<endl;
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
			 ////整形需要转换
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
			 //这里有问题文档上写的是 调用pDomainRouteList->stDomainRoute[i].cDomainCode
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
		//分页获取摄像机信息
		flag=GetCameraInfo(&CameraPageIndex);
	}
	
	

}

void CIVSRealPlayPage::OnBnClickedButtonOpenWindowTest()
{

	// TODO: 在此添加控件通知处理程序代码
	CString CCameraCode;
	CCameraCode.Format(_T("%s"),_T("03345720000000000101#571fce2fd1b544769f2ba21f6d2d34c4"));
	IVS_REALPLAY_PARAM RealplayPara = {0};//实况播放参数
	//RealplayPara.bDirectFirst = 1;
	//RealplayPara.bMultiCast = 0;
	//RealplayPara.uiProtocolType = 0;
	RealplayPara.bDirectFirst = m_checkIsDirect.GetCheck();
	RealplayPara.bMultiCast = m_checkIsMulticast.GetCheck();
	RealplayPara.uiProtocolType = m_ComboProtocol.GetCurSel() + 1;
	RealplayPara.uiStreamType = STREAM_TYPE_MAIN;

	m_pSheet->StopVideoPlay();
	m_pSheet->SetRealPlayPara(CCameraCode, &RealplayPara);
	SendMessage(WM_UPDATEPRESETLIST);//更新实况摄像机的预置位
	m_pSheet->StartRealPlay();
	
	

		

}


void CIVSRealPlayPage::OnBnClickedButtonCloseWindowTest()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pSheet->StopRealPlay();
}


bool CIVSRealPlayPage::GetCameraInfo(int *PageIndex)
{
	//add by ray 添加获取摄像头信息按钮，获得信息保存到文档中储存
	IVS_UINT32 uiReqNum = 1000;//查询数量
	IVS_UINT32 uiReqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiReqNum-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//所需内存大小
	IVS_UINT32 Camera_uiTotal=0;             
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiReqBufSize]);//分配内存
	CHECK_POINTER(pCameraInfoList, 0);
	bool flag=false;
	int Camera_PageIndex=0;
	Camera_PageIndex=*PageIndex;
	memset(pCameraInfoList,0,uiReqBufSize);
	IVS_INDEX_RANGE IndexRange = {1+Camera_PageIndex*uiReqNum, uiReqNum*(1+Camera_PageIndex)};//索引范围


	IVS_INT32 iRet = IVS_SDK_GetDeviceList(m_pSheet->GetSessionID(), DEVICE_TYPE_CAMERA, &IndexRange, pCameraInfoList, uiReqBufSize);//获取设备列表

	if (IVS_SUCCEED == iRet)
	{	cout<<" IVS_SDK_GetDeviceList success!"<<endl;
		//获取设备列表成功，保存
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
		uiNum = (uiNum > pCameraInfoList->uiTotal) ? pCameraInfoList->uiTotal : uiNum;//实际返回的设备个数
		cout<<"uiNum实际返回的设备个数:"<<uiNum<<endl;
		cout<<"pCameraInfoList->uiTotal:"<<pCameraInfoList->uiTotal<<endl;
		if(1000==uiNum)
		{//信号源数量超过1000，需要再次获取

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
	
			////整形需要转换
			CString temp;

			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].uiDevFormType);
			WritePrivateProfileString(CameraIndex,"uiDevFormType",temp ,FilePath);

			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].uiType );
			WritePrivateProfileString(CameraIndex,"uiType",temp,FilePath);
			
			
			WritePrivateProfileString(CameraIndex,"cCameraLocation",pCameraInfoList->stCameraBriefInfo[i].cCameraLocation ,FilePath);

			////整形需要转换
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
			
			////整形需要转换
			temp.Empty();
			temp.Format("%u",pCameraInfoList->stCameraBriefInfo[i].bIsExDomain );
			WritePrivateProfileString(CameraIndex,"bIsExDomain",temp,FilePath);

			WritePrivateProfileString(CameraIndex,"cDevIp",pCameraInfoList->stCameraBriefInfo[i].cDevIp ,FilePath);
			WritePrivateProfileString(CameraIndex,"cReserve",pCameraInfoList->stCameraBriefInfo[i].cReserve ,FilePath);
		
			
		}
		
	}
	
	delete[] pCameraInfoList;//释放内存
	pCameraInfoList = NULL;
	return flag;
}


void CIVSRealPlayPage::Get_Device_Group(const IVS_CHAR* CDomainCode)
{
	//cpp code
	// get device group list获取设备组信息

	IVS_UINT32 uiBufSize = sizeof(IVS_DEVICE_GROUP_LIST) + 4999*sizeof(IVS_DEVICE_GROUP);//5000个 
	IVS_DEVICE_GROUP_LIST* pDeviceGroupList = (IVS_DEVICE_GROUP_LIST*)(new IVS_CHAR[uiBufSize]); 
	// CHECK_POINTER_VOID(pDeviceGroupList)//指针判空 
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
	IVS_INT32 iRet = IVS_SDK_GetDeviceGroup(m_pSheet->GetSessionID(),CDomainCode, "0", pDeviceGroupList, uiBufSize);// 域编码通过IVS_SDK_GetDomainRoute接口获取；系统默认的根设备组ID是“0”，传入“0”可获取根设备组下的所有子设备组 
	if (IVS_SUCCEED == iRet)  
	{  
		//成功 打印设备组信息
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
		//这里是获取单个域下面的所有分组信息
		//为了防止数据被擦除，需要在之前记录的末尾继续添加\
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
		
			////整形需要转换
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
	// TODO: 在此添加控件通知处理程序代码
}
