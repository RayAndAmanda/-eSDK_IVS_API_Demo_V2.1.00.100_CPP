#include "stdafx.h"
#include<stdio.h>
#include "DspLib.h"
#include <string>
#include <vector>
#include "log.h"
#include "IVSRealPlayPage.h"
#include "IVSSDKSheet.h"
#include "S200Interface.h"
using namespace std; 

#ifdef _DEBUG
#pragma   comment   (lib,   "DspLibd.lib")  

#else
#pragma   comment   (lib,   "DspLib.lib")  
#endif

#define  S200INTERFACE
#define SAFE_DELETE(obj)		if(obj){delete obj; obj = NULL;}
#define SAFE_M_DELETE(obj)		if(obj){delete []obj; obj = NULL;}
const IVS_UINT32 uiGetStep = 1000;	// �ֲ���ѯ�Ĳ�����
IVS_INT32 g_iSessionID=NULL;
float g_t1=0;
float g_t2=0;
float g_t=0;
 
 typedef struct
{
	unsigned long seq;
}OWN_PARA;

  void __SDK_CALL fRealPlayCallBackFrame(IVS_ULONG ulHandle, IVS_UINT32 uiStreamType, IVS_VOID* pFrameInfo, IVS_VOID* pUserData)
  { //�ص���������
	  /******************************************************************
 function   : RealPlayCallBackFrame
 description: ��ȡ����������ص�����(ʵ��)
 input      : NA
 output     : ulHandle      ���ž��;
              uiStreamType  ֡����(��Ƶ����Ƶ���ο�IVS_MEDIA_STREAM);
              pFrameInfo    ֡���Ͳ���(��Ƶ���Ͳο�IVS_VIDEO_FRAME����Ƶ���Ͳο�IVS_AUDIO_FRAME);
              pUserData     �û�����;
 return     : IVS_VOID
*******************************************************************/
	  //typedef struct
	  //{
		 // IVS_UCHAR*  pY;
		 // IVS_UCHAR*  pU;
		 // IVS_UCHAR*  pV;
		 // IVS_UINT32  uiPitch;            // ƽ����ڴ�ռ�ó���(Ҳ�������Stride)
		 // IVS_UINT32  uiWidth;            // �����
		 // IVS_UINT32  uiHeight;           // �����
		 // IVS_UINT32  uiPictureFormat;    // �����ʽ,�ο�IVS_PICTURE_FORMAT
		 // IVS_UINT32  uiTimeStamp;        // RTPʱ���
		 // IVS_UINT64  ullTimeTick;        // ʱ����Ϣ����λ����
		 // IVS_UINT32	uiGopSequence;		// GOP����
		 // IVS_CHAR	cReserve[32];
		 // IVS_UINT32  uiFrameType;        //֡����(I P)
	  //}IVS_VIDEO_FRAME;
	 // printf("enter fRealPlayCallBackFrame!");
	  //printf("uiStreamType=%d",uiStreamType);
	  if (MEDIA_STREAM_VIDEO==uiStreamType)
	  {
		NET_VIDEO_DATA *sinChan = (NET_VIDEO_DATA *)(pUserData);
		ULONG pYuvBuf[4];
		//printf("uiStreamType=%d",uiStreamType);
		IVS_VIDEO_FRAME* pTemp=NULL;
		pTemp=(IVS_VIDEO_FRAME*)pFrameInfo;
		pYuvBuf[0]=(ULONG)pTemp->pY;
		pYuvBuf[1]=(ULONG)pTemp->pU;
		pYuvBuf[2]=(ULONG)pTemp->pV;
		
		g_t1=::GetTickCount();
		GqyInsertLog(GQY_LOG_INFO,"%f",g_t1);
		senddata(sinChan,pYuvBuf,pTemp->uiWidth,pTemp->uiHeight,NULL);
		
	  }
	 
	 // printf("leave fRealPlayCallBackFrame!");
  }

S200INTERFACE bool DisConnect()
{
	return true;
}
S200INTERFACE void PlayBackSig(NET_VIDEO_DATA *netvid, int act, SYSTEMTIME &sts, SYSTEMTIME &ste)
{
//	GetIpsIndex("TEST");
}
S200INTERFACE void ChangeYuntai(NET_VIDEO_DATA *netvid, int yuntaipara, int StartStop)
{
//	SdSrcInfo2Cli(3, _T("��³ľ��,-1"));
}
S200INTERFACE char GetSigVic(void)
{
	return 0;
}


S200INTERFACE void PlatformLogin(IPvs &ipvs)
{
	GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_PlatformLogin++++++++++++\r\n");
	if (0!=IVS_SDK_Init())
	{
		GqyInsertLog(GQY_LOG_INFO,"IVS_SDK_Init failed!");
	}
	else
	{
		GqyInsertLog(GQY_LOG_INFO,"IVS_SDK_Init success!\r\n");
	}
	
	CString UserInfo_path("D:\\exe2014\\IVS_Demo\\UserInfo.ini");
	IVS_LOGIN_INFO LoginReqInfo = {0};
	LoginReqInfo.stIP.uiIPType = IP_V4;
	/*strncpy_s(LoginReqInfo.stIP.cIP,_T("172.19.100.119"), IVS_IP_LEN);
	LoginReqInfo.uiPort = static_cast<IVS_UINT32>(9900);
	strncpy_s(LoginReqInfo.cUserName, _T("810225030_qq_com"), IVS_NAME_LEN);
	strncpy_s(LoginReqInfo.pPWD, _T("Ah5s4fKb"), IVS_PWD_LEN);*/

	GetPrivateProfileString("UserInfo","IP", "", LoginReqInfo.stIP.cIP, IVS_IP_LEN,UserInfo_path);
	GetPrivateProfileString("UserInfo","UserName", "", LoginReqInfo.cUserName, IVS_NAME_LEN,UserInfo_path);
	GetPrivateProfileString("UserInfo","PassWord", "", LoginReqInfo.pPWD, IVS_PWD_LEN,UserInfo_path);
	LoginReqInfo.uiPort=GetPrivateProfileInt("UserInfo","uiPort",0,UserInfo_path);

	if (IVS_SUCCEED ==IVS_SDK_Login(&LoginReqInfo, &g_iSessionID) )
	{
	
		GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_PlatformPlatformLogin success!\r\n");
	}
	else
	{
		GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_PlatformPlatformLogin failed!\r\n");
	}
	
	GqyInsertLog(GQY_LOG_INFO,"exit IVS_Demo_PlatformLogin++++++++++++\r\n");

}
///��ȡ����·����Ϣ
// ��ȡ��·�ɹ�ϵ�б�
IVS_INT32 GetDomainRoute()
{
	// ������Ҫ���ٵ��ڴ�ռ��С
	IVS_UINT32 uiBufSize = sizeof(IVS_DOMAIN_ROUTE_LIST) + (IVS_MAX_DOMAIN_ROUTE_NUM - 1) * sizeof(IVS_DOMAIN_ROUTE);// ���128����
	// �����ڴ�ռ䣬��Ż�ȡ������·����Ϣ
	IVS_DOMAIN_ROUTE_LIST* pDomainRouteList = NULL;
	try
	{
		pDomainRouteList = (IVS_DOMAIN_ROUTE_LIST*)(new IVS_CHAR[uiBufSize]);
		if (NULL == pDomainRouteList)
		{
			return IVS_ALLOC_MEMORY_ERROR;
		}
	}
	catch (...)
	{
		// �쳣
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pDomainRouteList, 0, uiBufSize);

	// ��ȡ��·��
	
	if (IVS_SUCCEED !=IVS_SDK_GetDomainRoute(g_iSessionID, pDomainRouteList, uiBufSize))
	{
		// �ͷ��ڴ�ռ�
		delete []pDomainRouteList;
		return -1;
	}
	
	// ����ȡ����·����Ϣ����list��
	// �����list
	// ������뱾�����·����Ϣ
	
	IVS_UINT32 uiTotal = pDomainRouteList->uiTotal;
	GqyInsertLog(GQY_LOG_INFO,"�豸���б�����:");
	GqyInsertLog(GQY_LOG_INFO,"pDomainRouteList->uiTotal=%d", uiTotal);
	for(IVS_UINT32 ui = 0; ui < uiTotal; ui++)
	{
		IVS_DOMAIN_ROUTE stSampleDomainRoute;
		IVS_CHAR cTmp[IVS_NAME_LEN] = {0};
		// ���������
		memcpy(stSampleDomainRoute.cDomainCode, pDomainRouteList->stDomainRoute[ui].cDomainCode, IVS_DOMAIN_CODE_LEN);
	

		// ��������

		memcpy(stSampleDomainRoute.cDomainName, pDomainRouteList->stDomainRoute[ui].cDomainName, IVS_NAME_LEN);
		
	
		// �����ϲ������
		memcpy(stSampleDomainRoute.cSuperDomain, pDomainRouteList->stDomainRoute[ui].cSuperDomain, IVS_DOMAIN_CODE_LEN);
		GqyInsertLog(GQY_LOG_INFO,"index=%d,cDomainName=%s",ui,stSampleDomainRoute.cDomainName);
		GqyInsertLog(GQY_LOG_INFO,"index=%d,cDomainCode=%s",ui,stSampleDomainRoute.cDomainCode);
		GqyInsertLog(GQY_LOG_INFO,"index=%d,cSuperDomain=%s",ui,stSampleDomainRoute.cSuperDomain);

		//strncpy_s(cTmp,"", IVS_DOMAIN_CODE_LEN);
		if (strcmp( stSampleDomainRoute.cDomainCode,"")==0)
		{//��ȡ�����Ϊ��ֹͣ��ȡ
			break;
		}
		if (strcmp(stSampleDomainRoute.cSuperDomain,"")==0)
		{//û�и��򣬱�ʾΪ���ڵ㣬���͸��ڵ�

			string msg1= stSampleDomainRoute.cDomainName;

			msg1 += ",0";
			//CString str;
			//str.Format(_T("%s"), msg1.c_str());
			SdSrcInfo2Cli(3, (char*)msg1.c_str()); // ��3 ��ʾ���͸��ڵ�
			printf("SdSrcInfo2Cli3=%s\n",	msg1);
			GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli3=%s\n",msg1);
		}
		else
		{//���Ǹ��ڵ㷢�ͷ���
			char msg[1024];
			sprintf(msg, "%s,%d,%s,%s", "0",1,stSampleDomainRoute.cDomainName,"1");
			SdSrcInfo2Cli(1, msg);					//1 ���ͷ�����Ϣ
			printf("SdSrcInfo2Cli1=%s\n",	msg);
			GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli1=%s\n",msg);
		}
	    //�����򣬻�ȡ���������еķ������
	    Get_Device_Group(stSampleDomainRoute.cDomainCode);
	}

	// �ͷ��ڴ�ռ�
	delete []pDomainRouteList;

	return IVS_SUCCEED;
}
/////�����ȡ�豸������
void Get_Device_Group(const IVS_CHAR* CDomainCode)
{
	//cpp code

	IVS_UINT32 uiBufSize = sizeof(IVS_DEVICE_GROUP_LIST) + 4999*sizeof(IVS_DEVICE_GROUP);//5000�� 
	IVS_DEVICE_GROUP_LIST* pDeviceGroupList = (IVS_DEVICE_GROUP_LIST*)(new IVS_CHAR[uiBufSize]); 
	memset(pDeviceGroupList, 0, uiBufSize); 
	//CString Group_Info_path("D:\\exe2014\\IVS_Demo\\Group_Info.ini");
	///2018-02-26
	IVS_CHAR	Test_cDomainCode[128];
	memset(Test_cDomainCode,0, sizeof(Test_cDomainCode));
	memcpy(Test_cDomainCode,CDomainCode,IVS_DOMAIN_CODE_LEN);

	
	///2018-02-26
	IVS_INT32 iRet = IVS_SDK_GetDeviceGroup(g_iSessionID,Test_cDomainCode, "0", pDeviceGroupList, uiBufSize);// �����ͨ��IVS_SDK_GetDomainRoute�ӿڻ�ȡ��ϵͳĬ�ϵĸ��豸��ID�ǡ�0�������롰0���ɻ�ȡ���豸���µ��������豸�� 
	if (IVS_SUCCEED == iRet)  
	{  
		//�ɹ� ��ӡ�豸����Ϣ
		GqyInsertLog(GQY_LOG_INFO,"��:%s,�豸��������Ϊ=%d,��Ϣ���£�\n",CDomainCode,pDeviceGroupList->uiTotal);
		CString DeviceGroupIndex;
		for (IVS_UINT32 i=0; i<pDeviceGroupList->uiTotal; i++)
		{
			IVS_DEVICE_GROUP DeviceGroup;
			// �����豸�����������
			memcpy(DeviceGroup.cDomainCode, pDeviceGroupList->stDeviceGroup[i].cDomainCode, IVS_DOMAIN_CODE_LEN);

			// �����豸�����
			memcpy(DeviceGroup.cGroupCode, pDeviceGroupList->stDeviceGroup[i].cGroupCode, IVS_DEVICE_GROUP_LEN);
			
			// �������豸�����
			memcpy(DeviceGroup.cParentGroupCode, pDeviceGroupList->stDeviceGroup[i].cParentGroupCode, IVS_DEVICE_GROUP_LEN);

			// �����豸������
			memcpy(DeviceGroup.cGroupName, pDeviceGroupList->stDeviceGroup[i].cGroupName, IVS_NAME_LEN);
			
			//�Ƿ���������
			DeviceGroup.bIsExDomain=pDeviceGroupList->stDeviceGroup->bIsExDomain;
			GqyInsertLog(GQY_LOG_INFO,"cGroupName=%s,cDomainCode=%s,cGroupCode=%s,cParentGroupCode=%s",DeviceGroup.cGroupName,DeviceGroup.cDomainCode,DeviceGroup.cGroupCode,DeviceGroup.cParentGroupCode);

			//if (0==DeviceGroup.bIsExDomain &&strcmp(DeviceGroup.cParentGroupCode,"0")==0)
			//{//���鸸�ڵ�Ϊ  ���鸸ID Ϊ1
			//	CString tempcGroupCode=DeviceGroup.cGroupCode;
			//	int n=tempcGroupCode.Find("#");
			//	if (-1!=n)
			//	{
			//		tempcGroupCode=tempcGroupCode.Left(n);
			//	}
			//	char msg[1024];
			//	sprintf(msg, "%s,%d,%s,%s", "1",1,DeviceGroup.cGroupName,tempcGroupCode);
			//	GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli1=%s\n",	msg);
			//	SdSrcInfo2Cli(1, msg);

			//}
			//if(1==DeviceGroup.bIsExDomain&&strcmp(DeviceGroup.cParentGroupCode,"0")==0)
			if(strcmp(DeviceGroup.cParentGroupCode,"0")==0)
			{//��GQY_Group.bIsExDomin==1,���鸸ID Ϊ0

				CString tempcGroupCode=DeviceGroup.cGroupCode;
				int n=tempcGroupCode.Find("#");
				if (-1!=n)
				{
					tempcGroupCode=tempcGroupCode.Left(n);
				}

				char msg[1024];
				sprintf(msg, "%s,%d,%s,%s", "0",1,DeviceGroup.cGroupName,tempcGroupCode);
				GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli1=%s\n",msg);
				SdSrcInfo2Cli(1, msg);
			}
			if (strcmp(DeviceGroup.cParentGroupCode,"0")!=0)
		    {
				CString tempccGroupCode=DeviceGroup.cGroupCode;
				int n=tempccGroupCode.Find("#");
				if (-1!=n)
				{
					tempccGroupCode=tempccGroupCode.Left(n);
				}
				CString tempcParentGroupCode=DeviceGroup.cParentGroupCode;
				int m=tempcParentGroupCode.Find("#");
				if (-1!=m)
				{
					tempcParentGroupCode=tempcParentGroupCode.Left(m);
				}
				char msg[1024];
				sprintf(msg, "%s,%d,%s,%s",tempcParentGroupCode ,1,DeviceGroup.cGroupName,tempccGroupCode);
				GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli1=%s\n",msg);
				SdSrcInfo2Cli(1, msg);

			
			}

		
		} 
	}
	else
	{
		GqyInsertLog(GQY_LOG_INFO,"IVS_SDK_GetDeviceGroup failed!\n");
		GqyInsertLog(GQY_LOG_INFO,"iRet=%d,test-CDomainCode=%s\n",iRet,Test_cDomainCode);
		
	}
	delete[] pDeviceGroupList; 
	pDeviceGroupList = NULL;
}
/////��ҳ��ȡ���е��豸��Ϣ
IVS_INT32 GetStepDeviceList(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceType,
	const IVS_INDEX_RANGE* pIndexRange,
	IVS_VOID* pDeviceList,
	IVS_UINT32 uiBufferSize)
{
	if (NULL == pIndexRange || NULL == pDeviceList)
	{
		return IVS_PARA_INVALID;
	}
	// ��ʼ�����ٵ��ڴ�ռ�
	memset(pDeviceList, 0, uiBufferSize);

	// ��ҳ��ȡָ�����µ��豸�б�
	if (IVS_SUCCEED !=IVS_SDK_GetDeviceList(g_iSessionID, uiDeviceType, pIndexRange, pDeviceList, uiBufferSize))
	{
		// ʧ��
		printf("IVS_SDK_GetDeviceList fail!");
		
	}

	// ����ֻ�ж���������豸������Ϊֻ��Ҫ��ȡ������豸�������Ҫ��ȡ���������豸������Ҫ����uiDeviceTypeת���ɶ�Ӧ�Ľṹ��ָ��
	IVS_CAMERA_BRIEF_INFO_LIST *pCamDeviceList = NULL;
	if (DEVICE_TYPE_CAMERA == uiDeviceType)
	{
		pCamDeviceList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST *>(pDeviceList);
	}
	// ����ȡ���豸����Ϣ����list��
	// ʵ�ʻ�ȡ����������豸�����������������uiGetStep����RealNumΪuiGetStep������Ϊ����
	IVS_UINT32 uiRealNum = 0;
	if (0 == pCamDeviceList->stIndexRange.uiToIndex)
	{
		uiRealNum = 0;
	}
	else
	{
		uiRealNum = pCamDeviceList->stIndexRange.uiToIndex - pCamDeviceList->stIndexRange.uiFromIndex + 1;
	}
	GqyInsertLog(GQY_LOG_INFO,"��������Ϊ��%d",uiRealNum);
	// ѭ������list��
	for(IVS_UINT32 ui = 0; ui < uiRealNum; ui++)
	{
		IVS_CAMERA_BRIEF_INFO stCameraInfo;
		IVS_CHAR cTmp[IVS_CAMERA_NAME_LEN] = {0};
		//// �����豸���������
		//memcpy(cTmp, pCamDeviceList->stCameraBriefInfo[ui].cDomainCode, IVS_DOMAIN_CODE_LEN);
		//stCameraInfo.cDomainCode = cTmp;
		//memset(cTmp, 0, IVS_NAME_LEN);

		// �����豸�����豸�����
		memcpy(stCameraInfo.cDevGroupCode, pCamDeviceList->stCameraBriefInfo[ui].cDevGroupCode, IVS_DEVICE_GROUP_LEN);
		

		// ����������豸����
		memcpy(stCameraInfo.cCode, pCamDeviceList->stCameraBriefInfo[ui].cCode, IVS_DEV_CODE_LEN);
		
		// ����������豸����
		memcpy(stCameraInfo.cName, pCamDeviceList->stCameraBriefInfo[ui].cName, IVS_CAMERA_NAME_LEN);
		
		//// �����豸����
		//stCameraInfo.uiType = pCamDeviceList->stCameraBriefInfo[ui].uiType;

		// ���������״̬
		stCameraInfo.uiStatus = pCamDeviceList->stCameraBriefInfo[ui].uiStatus;
		//// �Ƿ�Ϊ�ⲿ�� 0-��  1-��
		//stCameraInfo.bIsExDomain=pCamDeviceList->stCameraBriefInfo[ui].bIsExDomain;
		//�����豸��Ϣ

		char msg[1024];
		CString tempcDevGroupCode=stCameraInfo.cDevGroupCode;
		int m=tempcDevGroupCode.Find("#");
		if (-1!=m)
		{
			tempcDevGroupCode=tempcDevGroupCode.Left(m);
		}
		sprintf(msg, "%s,%d,%s,%s,%d", tempcDevGroupCode,1,stCameraInfo.cName,stCameraInfo.cCode,stCameraInfo.uiStatus);
		printf("SdSrcInfo2Cli0=%s\n",msg);
		GqyInsertLog(GQY_LOG_INFO,"cDevGroupCode=%s,%d,cName=%s,cCode=%s,uiStatus=%d", tempcDevGroupCode,1,stCameraInfo.cName,stCameraInfo.cCode,stCameraInfo.uiStatus);
		GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli0=%s\n",msg);
		SdSrcInfo2Cli(0, msg);
	}
	// �ж��Ƿ���Ҫ������ȡ
	// ������豸����
	IVS_UINT32 uiTotal = pCamDeviceList->uiTotal;
	// ���û��ȡ�꣬�ٵݹ��ȡ
	if (uiTotal > pCamDeviceList->stIndexRange.uiToIndex)
	{
		IVS_INDEX_RANGE IndexRange;
		IndexRange.uiFromIndex = pCamDeviceList->stIndexRange.uiToIndex + 1;
		IndexRange.uiToIndex = IndexRange.uiFromIndex + uiGetStep - 1;

		(void)GetStepDeviceList(g_iSessionID,uiDeviceType, &IndexRange, pDeviceList, uiBufferSize);
		
	}
	return IVS_SUCCEED;
}

 void GetSrcS200()
{
	printf("enter GetSrcS200\n");
	GqyInsertLog(GQY_LOG_INFO,"enter GetSrcS200\n");
	/////////////////////////////////////////////////////////////////////2018-2-11
	//IVS��ȡ�����Ӧ�ķ�����Ϣ
	IVS_INT32 iRet=GetDomainRoute();
	if (IVS_SUCCEED!=iRet)
	{
		GqyInsertLog(GQY_LOG_INFO,"GetDomainRoute error!\n");
	}

	//IVS��ȡ�����б�

	IVS_INDEX_RANGE IndexRange = {1, uiGetStep};//������Χ
	//����֮����һ���ڴ棬�ݹ���õ�ʱ��֪�����᲻�����
	IVS_UINT32 uiReqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiGetStep-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//�����ڴ��С           
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiReqBufSize]);//�����ڴ�
	memset(pCameraInfoList,0,uiReqBufSize);
	GetStepDeviceList(g_iSessionID,DEVICE_TYPE_CAMERA,
		&IndexRange,
		pCameraInfoList,
		uiReqBufSize);
	delete[] pCameraInfoList;//�ͷ��ڴ�
	pCameraInfoList = NULL;
	///
//////////////////////////////////////////////////////////////////////////2018-2-11

//	//���ز��Դ���2-11
///////////////��ȡ·����Ϣ
//
//	
//	CString Route_Info_path("D:\\exe2014\\IVS_Demo\\Domain_route.ini");
//	
//	CString RouteIndex;
//	for (int i=0;; i++)
//	{
//		IVS_DOMAIN_ROUTE GQY_Route={0};
//		RouteIndex.Format("DomainRouteIndex_%d",i);
//		GetPrivateProfileString(RouteIndex,"cDomainCode", "", GQY_Route.cDomainCode, IVS_DOMAIN_CODE_LEN,Route_Info_path);
//		GetPrivateProfileString(RouteIndex,"cDomainName", "", GQY_Route.cDomainName, IVS_NAME_LEN, Route_Info_path);
//		GetPrivateProfileString(RouteIndex,"cSuperDomain", "", GQY_Route.cSuperDomain, IVS_DOMAIN_CODE_LEN, Route_Info_path);
//		if (strcmp( GQY_Route.cDomainCode,"")==0)
//		{//��ȡ�����Ϊ��ֹͣ��ȡ
//			break;
//		}
//		if (strcmp(GQY_Route.cSuperDomain,"")==0)
//		{//Ϊ���ڵ㣬���͸��ڵ�
//
//			string msg1= GQY_Route.cDomainName;
//			
//			msg1 += ",0";
//			//CString str;
//			//str.Format(_T("%s"), msg1.c_str());
//			SdSrcInfo2Cli(3, (char*)msg1.c_str()); // ��3 ��ʾ���͸��ڵ�
//			printf("SdSrcInfo2Cli3=%s\n",	msg1);
//		}
//		else
//		{//���Ǹ��ڵ㷢�ͷ���
//			char msg[254];
//			sprintf(msg, "%s,%d,%s,%s", "0",1,GQY_Route.cDomainName,"1");
//			SdSrcInfo2Cli(1, msg);
//			printf("SdSrcInfo2Cli1=%s\n",	msg);
//		}
//	}
//////��ȡ������Ϣ
//	CString Group_Info_path("D:\\exe2014\\IVS_Demo\\DeviceGroup.ini");
//	CString GroupIndex;
//	for(int i=0;;i++)
//	{
//		IVS_DEVICE_GROUP GQY_Group={0};
//		GroupIndex.Format("DeviceGroupIndex_%d",i);
//		GetPrivateProfileString(GroupIndex,"cGroupCode", "", GQY_Group.cGroupCode, IVS_DEVICE_GROUP_LEN,Group_Info_path);
//		GetPrivateProfileString(GroupIndex,"cGroupName", "", GQY_Group.cGroupName, IVS_NAME_LEN,Group_Info_path);
//		GetPrivateProfileString(GroupIndex,"cParentGroupCode", "", GQY_Group.cParentGroupCode, IVS_DEVICE_GROUP_LEN,Group_Info_path);
//		//GetPrivateProfileString(GroupIndex,"bIsExDomin", "", GQY_Group.bIsExDomin, IVS_DEVICE_GROUP_LEN,Group_Info_path);
//		GQY_Group.bIsExDomain=GetPrivateProfileInt(GroupIndex,"bIsExDomain",-1,Group_Info_path);
//		if (strcmp(GQY_Group.cGroupCode,"")==0)
//		{//��ȡ�����Ϊ��ֹͣ��ȡ
//			break;
//
//		}
//		if (0==GQY_Group.bIsExDomain &&strcmp(GQY_Group.cParentGroupCode,"0")==0)
//		{//���鸸�ڵ�Ϊ  ���鸸ID Ϊ1
//			CString tempcGroupCode=GQY_Group.cGroupCode;
//			int n=tempcGroupCode.Find("#");
//			if (-1!=n)
//			{
//				tempcGroupCode=tempcGroupCode.Left(n);
//			}
//			char msg[254];
//			sprintf(msg, "%s,%d,%s,%s", "1",1,GQY_Group.cGroupName,tempcGroupCode);
//			printf("SdSrcInfo2Cli1=%s\n",	msg);
//			SdSrcInfo2Cli(1, msg);
//			
//		}
//		if(1==GQY_Group.bIsExDomain&&strcmp(GQY_Group.cParentGroupCode,"0")==0)
//		{//��GQY_Group.bIsExDomin==1,���鸸ID Ϊ0
//		
//			CString tempcGroupCode=GQY_Group.cGroupCode;
//			int n=tempcGroupCode.Find("#");
//			if (-1!=n)
//			{
//				tempcGroupCode=tempcGroupCode.Left(n);
//			}
//			
//			char msg[254];
//			sprintf(msg, "%s,%d,%s,%s", "0",1,GQY_Group.cGroupName,tempcGroupCode);
//			printf("SdSrcInfo2Cli1=%s\n",	msg);
//			SdSrcInfo2Cli(1, msg);
//		}
//		if (strcmp(GQY_Group.cParentGroupCode,"0")!=0)
//		{
//			CString tempccGroupCode=GQY_Group.cGroupCode;
//			int n=tempccGroupCode.Find("#");
//			if (-1!=n)
//			{
//				tempccGroupCode=tempccGroupCode.Left(n);
//			}
//			CString tempcParentGroupCode=GQY_Group.cParentGroupCode;
//			int m=tempcParentGroupCode.Find("#");
//			if (-1!=m)
//			{
//				tempcParentGroupCode=tempcParentGroupCode.Left(m);
//			}
//			char msg[254];
//			sprintf(msg, "%s,%d,%s,%s",tempcParentGroupCode ,1,GQY_Group.cGroupName,tempccGroupCode);
//			printf("SdSrcInfo2Cli1=%s\n",	msg);
//			SdSrcInfo2Cli(1, msg);
//
//		}
//	
//	}
///////��ȡ�豸��Ϣ
//	CString Camer_Info_path("D:\\exe2014\\IVS_Demo\\CameraInfo.ini");
//	CString CameraIndex;
//	for(int i=0;;i++)
//	{
//		IVS_CAMERA_BRIEF_INFO GQY_Camera={0};
//		CameraIndex.Format("CameraIndex_%d",i);
//		GetPrivateProfileString(CameraIndex,"cCode", "", GQY_Camera.cCode,IVS_DEV_CODE_LEN ,Camer_Info_path);
//		GetPrivateProfileString(CameraIndex,"cName", "", GQY_Camera.cName, IVS_CAMERA_NAME_LEN,Camer_Info_path);
//		GetPrivateProfileString(CameraIndex,"cDevGroupCode", "", GQY_Camera.cDevGroupCode, IVS_DEVICE_GROUP_LEN,Camer_Info_path);
//		GQY_Camera.uiStatus=GetPrivateProfileInt(CameraIndex,"uiStatus",-1,Camer_Info_path);
//		if (strcmp(GQY_Camera.cCode,"")==0)
//		{
//			break;
//		}
//		CString tempcode=GQY_Camera.cCode;
//		/*int n=tempcode.Find("#");//��������벻��Ҫȥ��#
//		if (-1!=n)
//		{
//			tempcode=tempcode.Left(n);
//		}*/
//		CString tempcDevGroupCode=GQY_Camera.cDevGroupCode;
//		int m=tempcDevGroupCode.Find("#");
//		if (-1!=m)
//		{
//			tempcDevGroupCode=tempcDevGroupCode.Left(m);
//		}
//		char msg[254];
//		//sprintf(msg, "%s,%d,%s,%s",tempcParentGroupCode ,1,GQY_Group.cGroupName,tempccGroupCode);
//		sprintf(msg, "%s,%d,%s,%s,%d", tempcDevGroupCode,1,GQY_Camera.cName,tempcode,GQY_Camera.uiStatus);
//		printf("SdSrcInfo2Cli0=%s\n",	msg);
//		GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli0=%s\n",msg);
//		SdSrcInfo2Cli(0, msg);
//
//
//	}
//	//���ز��Դ���2-11
//			//string msg="��Ϊƽ̨���ڵ�1";
			//string msg1 = replaceAllComma(msg);
			//msg1 += ",0";
			////CString str;
			////str.Format(_T("%s"), msg1.c_str());
			//SdSrcInfo2Cli(3, (char*)msg1.c_str()); // ��3 ��ʾ���͸��ڵ�
			//printf("SdSrcInfo2Cli3=%s",	msg1);
	//////////////////////////////////////////////////////////////////////////////////////////////		
			//1,����ID
			//2,��ǰ����������ͷ������
			//3,�豸����
			//4,�豸ID							//��0   ��ʾ�����豸��Ϣ
			//5������״̬
			//sprintf(smsg, "%s,%d,%s,%s,%d", cid.c_str(),1,title.c_str(),szid.c_str(),1);
			//SdSrcInfo2Cli(0, smsg);
///////////////////////////////////////////////////////////////////////////////////////////////////
			//1.pid ���ڵ�ID
			//2.��ǰ�����µķ�������
			//3.��ǰ���������				  //��1   ��ʾ���ͷ�����Ϣ
			//4.��ǰ�����ID
			//sprintf(smsg, "%s,%d,%s,%s", pid.c_str(),1,path.c_str(),cid.c_str());
			//SdSrcInfo2Cli(1, smsg);
//////////////////////////////////////////////////////////////////////////////////////////////////////
	SdSrcInfo2Cli(2, "0");             ///��2 ��ʾ���ͽ��� 
	GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli2");
	GqyInsertLog(GQY_LOG_INFO,"�������\r\n");
	GqyInsertLog(GQY_LOG_INFO,"leave GetSrcS200()\r\n");
}

S200INTERFACE void GetSrcInfo(IPvs &ipvs)
{
 	GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_PlatformS200INTERFACE GetSrcInfo++++++++++++++\r\n");
 	PlatformLogin(ipvs);
	
 	GetSrcS200();
 
 	GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_PlatformGetSrcInfo-----------\r\n");

}

  BOOL StartRealPlayS200(const char* path, void* netid)
 {

	IPvs ips={0};
	PlatformLogin(ips);
	CString CCameraCode;
	//CCameraCode.Format(_T("%s"),_T("03345720000000000101#571fce2fd1b544769f2ba21f6d2d34c4"));
	
	//CCameraCode.Format(_T("%s"),_T("12345670000000000101#571fce2fd1b544769f2ba21f6d2d34c4"));
	CCameraCode.Format(path);
	IVS_REALPLAY_PARAM RealplayPara = {0};//ʵ�����Ų���
	RealplayPara.bDirectFirst = 0;
	RealplayPara.bMultiCast = 0;
	RealplayPara.uiProtocolType = 2;
	RealplayPara.uiStreamType = STREAM_TYPE_MAIN;
	IVS_ULONG pHandle=0;

	
	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayCBFrame
    * description	: ��ʼʵʱ������Իص���ʽ���YUV��
    * input			: iSessionID               ��¼�ɹ���ĻỰID
						  pRealplayPara            ʵ�����������ʵ�������ṹ��
						  pCamerCode               ����ͷ����
						  fRealPlayCallBackFrame   ʵʱ����ص�����
						  pUserData                �û����ݣ��Զ��壬��Ӧ�ص�����������ʱ���ظò���
    * output		: pHandle                  ���ž��
    * return		: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
	* remark		: NA
    **************************************************************************/
	
	IVS_INT32 iRet = IVS_SDK_StartRealPlayCBFrame(g_iSessionID, &RealplayPara, CCameraCode, fRealPlayCallBackFrame,netid, &pHandle);
	//printf("g_iSessionID=%d,   RealplayPara.uiProtocolType=%d\r\n",g_iSessionID,RealplayPara.uiProtocolType);
	//printf("iRet=%d,g_Handle=%d\r\n",iRet,pHandle);
	if(iRet==IVS_SUCCEED)
	{  ///�����ɹ�����Ҫ����������ھ��
		NET_VIDEO_DATA *pnet = (NET_VIDEO_DATA *)(netid);

		if (pnet)
		{
			OWN_PARA* ownp = (OWN_PARA*)pnet->ownpara;
			if (ownp)
			{
				ownp=(OWN_PARA*)malloc(sizeof(OWN_PARA));

				ownp->seq = pHandle;
				pnet->ownpara=ownp;

			}

		}
	}

	return TRUE;
 }


S200INTERFACE int LogAndOpen(NET_VIDEO_DATA *netvid)
{

	//printf("IVS_Demo_Platform LogAndOpen+++++++++++++\r\n");
	GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_Platform LogAndOpen+++++++++++++\r\n");
	if (netvid)
	{
		string Cameracode = netvid->strIP;
		OWN_PARA *op = NULL;
		op=(OWN_PARA*)malloc(sizeof(OWN_PARA));
			
		memset(op, 0, sizeof(OWN_PARA));
		netvid->ownpara = op;
		//printf("��������Ϊ��%s\n",Cameracode.c_str());
		GqyInsertLog(GQY_LOG_INFO,"������������Ϊ��%s\n",Cameracode.c_str());
		StartRealPlayS200(Cameracode.c_str(), netvid);
		
 	}
	
	GqyInsertLog(GQY_LOG_INFO,"IVS_Demo_Platform LogAndOpen-----------\r\n");
	//printf("IVS_Demo_Platform LogAndOpen-----------\r\n");

 	return 1;
}
S200INTERFACE void CloseSig(NET_VIDEO_DATA *netvid)
{

	printf("IVS_Demo_PlatformCloseSig++++++++++++\r\n");
	
	
	OWN_PARA *op = (OWN_PARA *)(netvid->ownpara);
	if (op->seq)
	{
		IVS_INT32 iRet = IVS_SDK_StopRealPlay(g_iSessionID, op->seq);
		if (IVS_SUCCEED==iRet)
		{
			printf("IVS_Demo_Platform CloseSig success!-----------\r\n");
		}
	}
	if (op)
	{//ɾ���رմ��ڵľ��
		SAFE_DELETE(op) 
	}

}
