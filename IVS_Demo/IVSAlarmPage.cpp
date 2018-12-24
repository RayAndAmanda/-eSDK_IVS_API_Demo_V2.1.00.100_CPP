// IVSAlarmPage.cpp : implementation file
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "IVS_Demo.h"
#include "IVSAlarmPage.h"
#include "IVSSDKSheet.h"


// CIVSAlarmPage dialog

IMPLEMENT_DYNAMIC(CIVSAlarmPage, CPropertyPage)

CIVSAlarmPage::CIVSAlarmPage()
	: CPropertyPage(CIVSAlarmPage::IDD)
	, m_strMaxLevel(_T("100"))
	, m_strMinLevel(_T("1"))
{

}

CIVSAlarmPage::~CIVSAlarmPage()
{
	ClearDeviceList();
}

void CIVSAlarmPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALARM, m_listAlarm);
	DDX_Control(pDX, IDC_TREE_ALARM_DEVICE, m_treeAlarmDevice);
	DDX_Control(pDX, IDC_SLIDER_MIN_LEVEL, m_SliderMin);
	DDX_Control(pDX, IDC_SLIDER_MAX_LEVEL, m_SliderMax);
	DDX_Text(pDX, IDC_STATIC_MAX_VALUE, m_strMaxLevel);
	DDX_Text(pDX, IDC_STATIC_MIN_VALUE, m_strMinLevel);
	DDX_Control(pDX, IDC_CHECK__MOVE_DECTION, m_CheckMoveDection);
	DDX_Control(pDX, IDC_CHECK_SHIELD, m_CheckShield);
	DDX_Control(pDX, IDC_CHECK_NO_SIGNAL, m_CheckNoSignal);
	DDX_Control(pDX, IDC_CHECK_SHAKE, m_CheckShake);
	DDX_Control(pDX, IDC_CHECK_FACE_RECOGNITION, m_CheckRecognition);
	DDX_Control(pDX, IDC_CHECK_BY_ALARM_TYPE, m_CheckByAlarmType);
	DDX_Control(pDX, IDC_CHECK_BY_ALARM_LEVEL, m_CheckByAlarmLevel);
	DDX_Control(pDX, IDC_CHECK_SUBSCRIBE_ALL, m_CheckSubscribeAll);
	DDX_Control(pDX, IDC_CHECK_INTRUSION, m_CheckIntrusion);
	DDX_Control(pDX, IDC_STATIC_SUBSCRIBE_ALARM, m_GrpSubscribe);
	DDX_Control(pDX, IDC_STATIC_ALARM_REPORT, m_GrpAlarmReport);
	DDX_Control(pDX, IDC_STATIC_MIN_LEVEL, m_LabMinLevel);
	DDX_Control(pDX, IDC_STATIC_MAX_LEVEL, m_LabMaxLevel);
	DDX_Control(pDX, IDC_BUTTON_SUBSCRIBE_ALARM, m_BtnSubscribe);
}


BEGIN_MESSAGE_MAP(CIVSAlarmPage, CPropertyPage)
	ON_MESSAGE(WM_UPDATEALARMLIST, &CIVSAlarmPage::OnUpdateAlarmList)
	ON_BN_CLICKED(IDC_BUTTON_SUBSCRIBE_ALARM, &CIVSAlarmPage::OnBnClickedButtonSubscribeAlarm)
	ON_MESSAGE(WM_UPDATEDEVICELIST, &CIVSAlarmPage::OnUpdateDeviceList)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_ALARM_DEVICE, &CIVSAlarmPage::OnNMDblclkTreeAlarmDevice)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK_BY_ALARM_TYPE, &CIVSAlarmPage::OnBnClickedCheckByAlarmType)
	ON_BN_CLICKED(IDC_CHECK_BY_ALARM_LEVEL, &CIVSAlarmPage::OnBnClickedCheckByAlarmLevel)
	ON_BN_CLICKED(IDC_CHECK_SUBSCRIBE_ALL, &CIVSAlarmPage::OnBnClickedCheckSubscribeAll)
END_MESSAGE_MAP()


// CIVSAlarmPage message handlers


BOOL CIVSAlarmPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  Add extra initialization here
	eSDK::CLanguage* pLang = m_pSheet->m_pLang;

	//���ø澯�б�����
	m_listAlarm.ModifyStyle(0, LVS_REPORT);
	m_listAlarm.SetExtendedStyle(m_listAlarm.GetExtendedStyle() | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listAlarm.InsertColumn(0, pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMLEVEL)));//�澯����
	m_listAlarm.InsertColumn(1, pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMSOURCE)));//�澯Դ
	m_listAlarm.InsertColumn(2, pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMEVENTID)));//�澯�¼�ID
	m_listAlarm.InsertColumn(3, pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMSTATUS)));//�澯״̬
	m_listAlarm.InsertColumn(4, pLang->GetString(GET_STRING_PARAM(IDS_LAB_OCCURTIME)));//����ʱ��
	m_listAlarm.InsertColumn(5, pLang->GetString(GET_STRING_PARAM(IDS_LAB_DESCRIPTION)));//�澯����
	m_listAlarm.SetColumnWidth(0, 65);
	m_listAlarm.SetColumnWidth(1, 55);
	m_listAlarm.SetColumnWidth(2, 75);
	m_listAlarm.SetColumnWidth(3, 65);
	m_listAlarm.SetColumnWidth(4, 130);
	m_listAlarm.SetColumnWidth(5, 150);

	m_treeAlarmDevice.ModifyStyle(0, TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES|TVS_CHECKBOXES);
	m_hItemRoot = m_treeAlarmDevice.InsertItem(pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMDEVTREE)));

	//���û�������
	m_SliderMin.SetRange(1, 100);
	m_SliderMin.SetTicFreq(10);
	m_SliderMin.SetLineSize(10);
	m_SliderMin.SetPageSize(10);
	m_SliderMin.SetPos(1);
	m_SliderMax.SetRange(1, 100);
	m_SliderMax.SetTicFreq(10);
	m_SliderMax.SetLineSize(10);
	m_SliderMax.SetPageSize(10);
	m_SliderMax.SetPos(100);
	
	m_CheckByAlarmType.SetCheck(FALSE);
	m_CheckByAlarmLevel.SetCheck(FALSE);
	m_CheckSubscribeAll.SetCheck(TRUE);//Ĭ�϶��ķ�ʽ��Ϊ��������

	m_SliderMin.EnableWindow(FALSE);
	m_SliderMax.EnableWindow(FALSE);
	EnableAlarmTypeCheckbox(FALSE);

	//���ñ�ǩ����ť��ʾ����
	m_CheckMoveDection.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_MOVEDETECTION)));
	m_CheckShield.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_SHIELD)));
	m_CheckNoSignal.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_NOSIGNAL)));
	m_CheckShake.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_SHAKE)));
	m_CheckRecognition.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_FACERECOGNITION)));
	m_CheckIntrusion.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_INTRUSION)));
	m_CheckByAlarmType.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_BYALARMTYPE)));
	m_CheckByAlarmLevel.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_BYALARMLEVEL)));
	m_CheckSubscribeAll.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_BYALL)));
	m_GrpSubscribe.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_SUBSCRIBE)));
	m_GrpAlarmReport.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMREPORT)));
	m_LabMinLevel.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_MIN)));
	m_LabMaxLevel.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_LAB_MAX)));
	m_BtnSubscribe.SetWindowText(pLang->GetString(GET_STRING_PARAM(IDS_BTN_SUBSCRIBE)));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//�������ԶԻ���
void CIVSAlarmPage::SetSheet(CIVSSDKSheet* pSheet)
{
	m_pSheet = pSheet;
}


void CIVSAlarmPage::EnableAlarmTypeCheckbox(BOOL bCheck)
{
	m_CheckMoveDection.EnableWindow(bCheck);
	m_CheckShield.EnableWindow(bCheck);
	m_CheckNoSignal.EnableWindow(bCheck);
	m_CheckShake.EnableWindow(bCheck);
	m_CheckRecognition.EnableWindow(bCheck);
	m_CheckIntrusion.EnableWindow(bCheck);
}


void CIVSAlarmPage::OnBnClickedCheckByAlarmType()
{
	// TODO: Add your control notification handler code here
	if (TRUE == m_CheckByAlarmType.GetCheck())
	{
		//�����澯���͡�����ѡ
		EnableAlarmTypeCheckbox(TRUE);

		m_CheckByAlarmLevel.SetCheck(FALSE);
		m_SliderMin.EnableWindow(FALSE);
		m_SliderMax.EnableWindow(FALSE);

		m_CheckSubscribeAll.SetCheck(FALSE);
	}
	else
	{
		EnableAlarmTypeCheckbox(FALSE);
	}
}


void CIVSAlarmPage::OnBnClickedCheckByAlarmLevel()
{
	// TODO: Add your control notification handler code here
	if (TRUE == m_CheckByAlarmLevel.GetCheck())
	{
		//�����澯���𡱱���ѡ
		m_SliderMin.EnableWindow(TRUE);
		m_SliderMax.EnableWindow(TRUE);

		m_CheckByAlarmType.SetCheck(FALSE);
		EnableAlarmTypeCheckbox(FALSE);

		m_CheckSubscribeAll.SetCheck(FALSE);
	}
	else
	{
		m_SliderMin.EnableWindow(FALSE);
		m_SliderMax.EnableWindow(FALSE);
	}
}


void CIVSAlarmPage::OnBnClickedCheckSubscribeAll()
{
	// TODO: Add your control notification handler code here
	if (TRUE == m_CheckSubscribeAll.GetCheck())
	{
		//���������С�����ѡ
		m_CheckByAlarmType.SetCheck(FALSE);
		EnableAlarmTypeCheckbox(FALSE);

		m_CheckByAlarmLevel.SetCheck(FALSE);
		m_SliderMin.EnableWindow(FALSE);
		m_SliderMax.EnableWindow(FALSE);
	}
}


//˫���豸�б�Ŀ¼���ڵ�
void CIVSAlarmPage::OnNMDblclkTreeAlarmDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	HTREEITEM hItem = m_treeAlarmDevice.GetSelectedItem();
	CHECK_POINTER_VOID(hItem)

	if (m_hItemRoot == hItem)
	{
		PostMessage(WM_UPDATEDEVICELIST);//˫�����ڵ�ʱ�������豸�б�
	}
}


//ˢ���豸�б�
afx_msg LRESULT CIVSAlarmPage::OnUpdateDeviceList(WPARAM wParam, LPARAM lParam)
{

	//��ȡ������б�
	const IVS_UINT32 uiReqNum = 1000;//��ѯ����
	IVS_UINT32 uiRsqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiReqNum-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//�����ڴ��С
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiRsqBufSize]);//�����ڴ�
	CHECK_POINTER(pCameraInfoList, 0);
	memset(pCameraInfoList,0,uiRsqBufSize);
	IVS_INDEX_RANGE IndexRange = {1, uiReqNum};//������Χ

	IVS_INT32 iRet = IVS_SDK_GetDeviceList(m_pSheet->GetSessionID(), DEVICE_TYPE_CAMERA, &IndexRange, pCameraInfoList, uiRsqBufSize);//��ȡ�豸�б�

	if (IVS_SUCCEED == iRet)
	{
		//��ȡ�豸�б�ɹ�������
		IVS_UINT32 uiNum = (pCameraInfoList->stIndexRange.uiToIndex - pCameraInfoList->stIndexRange.uiFromIndex) + 1;
		uiNum = (uiNum > pCameraInfoList->uiTotal) ? pCameraInfoList->uiTotal : uiNum;//ʵ�ʷ��ص��豸����

		ClearDeviceList();
		for (IVS_UINT32 i=0; i<uiNum; i++)
		{
			IVS_CAMERA_BRIEF_INFO *pDevInfo = new IVS_CAMERA_BRIEF_INFO;
			CHECK_POINTER(pDevInfo, 0);
			memset(pDevInfo,0,sizeof(IVS_CAMERA_BRIEF_INFO));
			memcpy(pDevInfo, &(pCameraInfoList->stCameraBriefInfo[i]), sizeof(IVS_CAMERA_BRIEF_INFO));
			m_DeviceList.push_back(pDevInfo);
		}
	}

	delete[] pCameraInfoList;//�ͷ��ڴ�
	pCameraInfoList = NULL;

	ShowDeviceList();//��ʾ�豸�б�
	CHECK_POINTER(m_pSheet, 0);
	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_GETDEVLIST_OK), GET_STRING_PARAM(IDS_GETDEVLISR_FAIL));
	return 0;
}


//����豸�б�
void CIVSAlarmPage::ClearDeviceList()
{
    std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iter = m_DeviceList.begin();
    std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iterEnd = m_DeviceList.end();
	for (; iter != iterEnd; iter++)
	{
		delete[] *iter;
	}

	m_DeviceList.clear();
	m_DeviceItemMap.clear();
}


//��ʾ�豸�б�
void CIVSAlarmPage::ShowDeviceList()
{
	m_treeAlarmDevice.DeleteAllItems();//ɾ�����нڵ�
	m_hItemRoot = m_treeAlarmDevice.InsertItem(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_LAB_ALARMDEVTREE)));//��Ӹ��ڵ�

    std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iter = m_DeviceList.begin();
    std::list<IVS_CAMERA_BRIEF_INFO*>::iterator iterEnd = m_DeviceList.end();       
	for (; iter != iterEnd; iter++)
	{
		TVINSERTSTRUCT tvStruct;
		tvStruct.hParent = m_hItemRoot;
		tvStruct.hInsertAfter = NULL;
		tvStruct.item.mask = TVIF_TEXT|TVIF_PARAM;

		CString strTmp;
		strTmp.Format("%-6s", (*iter)->cName);

		tvStruct.item.pszText = strTmp.GetBuffer();
		strTmp.ReleaseBuffer();
		HTREEITEM hItemSub = m_treeAlarmDevice.InsertItem(&tvStruct);//����ӽڵ�
		m_treeAlarmDevice.SetItemData(hItemSub, (DWORD_PTR)(*iter));//�����ݵ��ӽڵ�
		m_DeviceItemMap[hItemSub] = *iter;
	}

	m_treeAlarmDevice.Expand(m_hItemRoot, TVE_EXPAND);//չ��Ŀ¼��
}


//��ȡ������Ϣ
CString CIVSAlarmPage::GetSubscribeInfo()
{
	CString strSubscribeInfo = "";

    std::map<HTREEITEM, IVS_CAMERA_BRIEF_INFO*>::iterator iter = m_DeviceItemMap.begin();
    std::map<HTREEITEM, IVS_CAMERA_BRIEF_INFO*>::iterator iterEnd = m_DeviceItemMap.end();       
	for (; iter != iterEnd; iter++)
	{
		if (TRUE == m_treeAlarmDevice.GetCheck(iter->first))
		{
			//��ǰ�豸��ѡ��,Ϊ����Ӷ���

			CString strSourceCode = iter->second->cCode;
			IVS_INT32 uiPos = strSourceCode.Find("#");
			if (-1 != uiPos)
			{
				//�澯Դ����
				strSourceCode.Delete(uiPos, strSourceCode.GetLength()-uiPos);
			}

			if (TRUE == m_CheckSubscribeAll.GetCheck())
			{
				//��������
				strSubscribeInfo +=	"<SubscribeInfo>";
				strSubscribeInfo +=		"<AlarmInCode>" + strSourceCode + "</AlarmInCode>";
				strSubscribeInfo +=		"<SubscribeType>1</SubscribeType>";//1-���� 2-���澯���� 3-���澯����
				strSubscribeInfo +=	"</SubscribeInfo>";
			}
			else if (TRUE == m_CheckByAlarmLevel.GetCheck())
			{
				//���澯����
				strSubscribeInfo +=	"<SubscribeInfo>";
				strSubscribeInfo +=		"<AlarmInCode>" + strSourceCode + "</AlarmInCode>";
				strSubscribeInfo +=		"<SubscribeType>3</SubscribeType>";//1-���� 2-���澯���� 3-���澯����
				strSubscribeInfo +=		"<AlarmLevelValueMin>" + m_strMinLevel + "</AlarmLevelValueMin>";
				strSubscribeInfo +=		"<AlarmLevelValueMax>" + m_strMaxLevel + "</AlarmLevelValueMax>";
				strSubscribeInfo +=	"</SubscribeInfo>";
			}
			else if (TRUE == m_CheckByAlarmType.GetCheck())
			{
				//���澯����
				strSubscribeInfo +=																			"<SubscribeInfo>";
				strSubscribeInfo +=																				"<AlarmInCode>" + strSourceCode + "</AlarmInCode>";
				strSubscribeInfo +=																				"<SubscribeType>2</SubscribeType>";//1-���� 2-���澯���� 3-���澯����
				strSubscribeInfo +=																				"<AlarmTypeList>";
				strSubscribeInfo +=	(TRUE == m_CheckMoveDection.GetCheck()) ?		"<AlarmType>ALARM_TYPE_MOVE_DECTION</AlarmType>" : "";//�ƶ����澯
				strSubscribeInfo +=	(TRUE == m_CheckShield.GetCheck()) ?					"<AlarmType>ALARM_SHIELD</AlarmType>" : "";//��Ƶ�ڵ��澯
				strSubscribeInfo +=	(TRUE == m_CheckNoSignal.GetCheck()) ?				"<AlarmType>ALARM_NO_SIGNAL</AlarmType>" : "";//��Ƶ��ʧ�澯
				strSubscribeInfo +=	(TRUE == m_CheckShake.GetCheck()) ?					"<AlarmType>ALARM_MAU_VQD_SHAKE</AlarmType>" : "";//��Ƶ�����澯
				strSubscribeInfo +=	(TRUE == m_CheckRecognition.GetCheck()) ?			"<AlarmType>ALARM_FACE_RECOGNITION</AlarmType>" : "";//����ʶ��澯
				strSubscribeInfo +=	(TRUE == m_CheckIntrusion.GetCheck()) ?				"<AlarmType>ALARM_INTRUSION</AlarmType>" : "";//���ָ澯
				strSubscribeInfo +=																				"</AlarmTypeList>";
				strSubscribeInfo +=																			"</SubscribeInfo>";
			}
			else
			{
				//δѡ�������ͣ���Ĭ�϶�������
				strSubscribeInfo +=	"<SubscribeInfo>";
				strSubscribeInfo +=		"<AlarmInCode>" + strSourceCode + "</AlarmInCode>";
				strSubscribeInfo +=		"<SubscribeType>1</SubscribeType>";//1-���� 2-���澯���� 3-���澯����
				strSubscribeInfo +=	"</SubscribeInfo>";

				m_CheckSubscribeAll.SetCheck(TRUE);
			}
		}
	}

	return strSubscribeInfo;
}


//���ͻ�����ʵʱ��ʾ�澯����
void CIVSAlarmPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	m_strMinLevel.Format(_T("%d"), m_SliderMin.GetPos());
	m_strMaxLevel.Format(_T("%d"), m_SliderMax.GetPos());
	UpdateData(FALSE);

	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
}


//�澯����
void CIVSAlarmPage::OnBnClickedButtonSubscribeAlarm()
{
	// TODO: Add your control notification handler code here

	//��ȡ�û�ID
	IVS_UINT32 uiUserID = 0;
	CString strUserId = "";
	IVS_INT32 iRet = IVS_SDK_GetUserID(m_pSheet->GetSessionID(), &uiUserID);
	if (IVS_SUCCEED == iRet) 
	{ 
		strUserId.Format("%d", uiUserID); 
	}
	else
	{
		CString strTitle(_T("IVS Demo   "));
		strTitle.Append(m_pSheet->m_pLang->GetString(GET_STRING_PARAM(IDS_GETUSERIDFAIL)));
		//m_pSheet->SetWindowText(strTitle);//��������ʾ��ȡ�û�IDʧ��
		return;
	}

	CString strDomainCode = m_DeviceList.empty() ? "" : m_DeviceList.front()->cCode;
	IVS_INT32 uiPos = strDomainCode.Find("#");
	if (-1 != uiPos)
	{
		//�����
		strDomainCode.Delete(0, uiPos+1);
	}

	uiPos = -1;
	CString strAlarmInCode = m_DeviceList.empty() ? "" : m_DeviceList.front()->cCode;	//�澯Դ����
	uiPos = strAlarmInCode.Find("#");
	if (-1 != uiPos)
	{
		strAlarmInCode.Delete(uiPos, strAlarmInCode.GetLength());
	}

	//xml��ʽ��������
	CString strReqXml = "";
	strReqXml += "<Content>";
	strReqXml +=	"<DomainCode>" + strDomainCode + "</DomainCode>";
	strReqXml +=	"<Subscribe>";
	strReqXml +=		"<SubscriberInfo>";
	strReqXml +=			"<Subscriber>1</Subscriber>";//1-�����û� 2-�����û�
	strReqXml +=			"<SubscriberID>"+strUserId+"</SubscriberID>";
	strReqXml +=		"</SubscriberInfo>";
	strReqXml +=		"<AddSubscribeList>";
	strReqXml +=			"<SubscribeInfo>";
	strReqXml +=				"<AlarmInCode>" + strAlarmInCode + "</AlarmInCode>";
	strReqXml +=				"<SubscribeType>1</SubscribeType>";
	strReqXml +=				"<AlarmLevelValueMin>1</AlarmLevelValueMin>";
	strReqXml +=				"<AlarmLevelValueMax>100</AlarmLevelValueMax>";
	strReqXml +=				"<AlarmTypeList/>";
	strReqXml +=				"<DevDomainCode>" + strDomainCode + "</DevDomainCode>";
	strReqXml +=			"</SubscribeInfo>";
	strReqXml +=		"</AddSubscribeList>";
	strReqXml +=		"<DelSubscribeList/>";
	strReqXml +=	"</Subscribe>";
	strReqXml += "</Content>";

	iRet = IVS_SDK_SubscribeAlarm(m_pSheet->GetSessionID(), strReqXml);

	m_pSheet->ShowTitleMsg(iRet, GET_STRING_PARAM(IDS_SUBSCRIBE_OK), GET_STRING_PARAM(IDS_SUBSCRIBE_FAIL));
}

//UTCʱ��תUTC+8ʱ��
static CString UTCToUTC8(CString strUTCTime)
{
	CString strUTC8Time = "";
    const IVS_UINT32 uiTimeLength = 14;
       
	if (uiTimeLength != strUTCTime.GetLength())//ʱ�䳤��Ϊ14�ֽ�
	{
		strUTC8Time = "Time Format Error";
	} 
	else
	{
		strUTC8Time.Append(strUTCTime.Left(4));//��
		strUTCTime.Delete(0, 4);
		strUTC8Time.Append("-");
		strUTC8Time.Append(strUTCTime.Left(2));//��
		strUTCTime.Delete(0, 2);
		strUTC8Time.Append("-");
		strUTC8Time.Append(strUTCTime.Left(2));//��
		strUTCTime.Delete(0, 2);
		strUTC8Time.Append(" ");

		CString strHour = strUTCTime.Left(2);
		IVS_UINT32 iHour = atoi(strHour) + 8;//��8Сʱ
		strHour.Format(_T("%d"), iHour);

		strUTC8Time.Append(strHour);//ʱ
		strUTCTime.Delete(0, 2);
		strUTC8Time.Append(":");
		strUTC8Time.Append(strUTCTime.Left(2));//��
		strUTCTime.Delete(0, 2);
		strUTC8Time.Append(":");
		strUTC8Time.Append(strUTCTime.Left(2));//��
	}

	return strUTC8Time;
}


//ˢ�¸澯�б�
afx_msg LRESULT CIVSAlarmPage::OnUpdateAlarmList(WPARAM wParam, LPARAM lParam)
{
      
	IVS_ALARM_NOTIFY* pAlarmInfo = reinterpret_cast<IVS_ALARM_NOTIFY*>(lParam);//lParam�ڵ��ô���֤�ǿ�
	static IVS_UINT32 uiCnt = 0;
	CString strAlarmLevel = "";
	CString strAlarmEventID = "";
	CString strAlarmStatus = "";
	strAlarmLevel.Format("%d", pAlarmInfo->uiAlarmLevelValue);
	strAlarmEventID.Format("%d", pAlarmInfo->ullAlarmEventID);
	strAlarmStatus.Format("%d", pAlarmInfo->uiAlarmStatus);
	m_listAlarm.InsertItem(uiCnt, strAlarmLevel);//�澯����
	m_listAlarm.SetItemText(uiCnt, 1, pAlarmInfo->cAlarmInName);//�澯Դ
	m_listAlarm.SetItemText(uiCnt, 2, strAlarmEventID);//�澯�¼�ID
	m_listAlarm.SetItemText(uiCnt, 3, strAlarmStatus);//�澯״̬
	m_listAlarm.SetItemText(uiCnt, 4, UTCToUTC8(pAlarmInfo->cOccurTime));//����ʱ�䣬SDK�ϱ���ʱ����UTC��׼ʱ�䣬����ʱ����UTC+8
	m_listAlarm.SetItemText(uiCnt, 5, pAlarmInfo->cAlarmDesc);//������Ϣ
	uiCnt++;
	delete pAlarmInfo;
	pAlarmInfo = NULL;
	return 0;
}
