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
const IVS_UINT32 uiGetStep = 1000;	// 分布查询的步阶数
IVS_INT32 g_iSessionID=NULL;
float g_t1=0;
float g_t2=0;
float g_t=0;
 
 typedef struct
{
	unsigned long seq;
}OWN_PARA;

  void __SDK_CALL fRealPlayCallBackFrame(IVS_ULONG ulHandle, IVS_UINT32 uiStreamType, IVS_VOID* pFrameInfo, IVS_VOID* pUserData)
  { //回调码流处理
	  /******************************************************************
 function   : RealPlayCallBackFrame
 description: 获取解码后码流回调函数(实况)
 input      : NA
 output     : ulHandle      播放句柄;
              uiStreamType  帧类型(音频或视频，参考IVS_MEDIA_STREAM);
              pFrameInfo    帧类型参数(视频类型参考IVS_VIDEO_FRAME，音频类型参考IVS_AUDIO_FRAME);
              pUserData     用户数据;
 return     : IVS_VOID
*******************************************************************/
	  //typedef struct
	  //{
		 // IVS_UCHAR*  pY;
		 // IVS_UCHAR*  pU;
		 // IVS_UCHAR*  pV;
		 // IVS_UINT32  uiPitch;            // 平面的内存占用长度(也叫做跨度Stride)
		 // IVS_UINT32  uiWidth;            // 画面宽
		 // IVS_UINT32  uiHeight;           // 画面高
		 // IVS_UINT32  uiPictureFormat;    // 画面格式,参考IVS_PICTURE_FORMAT
		 // IVS_UINT32  uiTimeStamp;        // RTP时间戳
		 // IVS_UINT64  ullTimeTick;        // 时标信息，单位毫秒
		 // IVS_UINT32	uiGopSequence;		// GOP序列
		 // IVS_CHAR	cReserve[32];
		 // IVS_UINT32  uiFrameType;        //帧类型(I P)
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
//	SdSrcInfo2Cli(3, _T("乌鲁木齐,-1"));
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
///获取所有路由信息
// 获取域路由关系列表
IVS_INT32 GetDomainRoute()
{
	// 计算需要开辟的内存空间大小
	IVS_UINT32 uiBufSize = sizeof(IVS_DOMAIN_ROUTE_LIST) + (IVS_MAX_DOMAIN_ROUTE_NUM - 1) * sizeof(IVS_DOMAIN_ROUTE);// 最多128个域
	// 开辟内存空间，存放获取到的域路由信息
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
		// 异常
		return IVS_ALLOC_MEMORY_ERROR;
	}
	memset(pDomainRouteList, 0, uiBufSize);

	// 获取域路由
	
	if (IVS_SUCCEED !=IVS_SDK_GetDomainRoute(g_iSessionID, pDomainRouteList, uiBufSize))
	{
		// 释放内存空间
		delete []pDomainRouteList;
		return -1;
	}
	
	// 将获取的域路由信息插入list中
	// 先清空list
	// 逐个插入本域的域路由信息
	
	IVS_UINT32 uiTotal = pDomainRouteList->uiTotal;
	GqyInsertLog(GQY_LOG_INFO,"设备域列表如下:");
	GqyInsertLog(GQY_LOG_INFO,"pDomainRouteList->uiTotal=%d", uiTotal);
	for(IVS_UINT32 ui = 0; ui < uiTotal; ui++)
	{
		IVS_DOMAIN_ROUTE stSampleDomainRoute;
		IVS_CHAR cTmp[IVS_NAME_LEN] = {0};
		// 拷贝域编码
		memcpy(stSampleDomainRoute.cDomainCode, pDomainRouteList->stDomainRoute[ui].cDomainCode, IVS_DOMAIN_CODE_LEN);
	

		// 拷贝域名

		memcpy(stSampleDomainRoute.cDomainName, pDomainRouteList->stDomainRoute[ui].cDomainName, IVS_NAME_LEN);
		
	
		// 拷贝上层域编码
		memcpy(stSampleDomainRoute.cSuperDomain, pDomainRouteList->stDomainRoute[ui].cSuperDomain, IVS_DOMAIN_CODE_LEN);
		GqyInsertLog(GQY_LOG_INFO,"index=%d,cDomainName=%s",ui,stSampleDomainRoute.cDomainName);
		GqyInsertLog(GQY_LOG_INFO,"index=%d,cDomainCode=%s",ui,stSampleDomainRoute.cDomainCode);
		GqyInsertLog(GQY_LOG_INFO,"index=%d,cSuperDomain=%s",ui,stSampleDomainRoute.cSuperDomain);

		//strncpy_s(cTmp,"", IVS_DOMAIN_CODE_LEN);
		if (strcmp( stSampleDomainRoute.cDomainCode,"")==0)
		{//获取域编码为空停止获取
			break;
		}
		if (strcmp(stSampleDomainRoute.cSuperDomain,"")==0)
		{//没有父域，表示为根节点，发送根节点

			string msg1= stSampleDomainRoute.cDomainName;

			msg1 += ",0";
			//CString str;
			//str.Format(_T("%s"), msg1.c_str());
			SdSrcInfo2Cli(3, (char*)msg1.c_str()); // 发3 表示发送根节点
			printf("SdSrcInfo2Cli3=%s\n",	msg1);
			GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli3=%s\n",msg1);
		}
		else
		{//不是根节点发送分组
			char msg[1024];
			sprintf(msg, "%s,%d,%s,%s", "0",1,stSampleDomainRoute.cDomainName,"1");
			SdSrcInfo2Cli(1, msg);					//1 发送分组信息
			printf("SdSrcInfo2Cli1=%s\n",	msg);
			GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli1=%s\n",msg);
		}
	    //根据域，获取域下面所有的分组情况
	    Get_Device_Group(stSampleDomainRoute.cDomainCode);
	}

	// 释放内存空间
	delete []pDomainRouteList;

	return IVS_SUCCEED;
}
/////分域获取设备组的情况
void Get_Device_Group(const IVS_CHAR* CDomainCode)
{
	//cpp code

	IVS_UINT32 uiBufSize = sizeof(IVS_DEVICE_GROUP_LIST) + 4999*sizeof(IVS_DEVICE_GROUP);//5000个 
	IVS_DEVICE_GROUP_LIST* pDeviceGroupList = (IVS_DEVICE_GROUP_LIST*)(new IVS_CHAR[uiBufSize]); 
	memset(pDeviceGroupList, 0, uiBufSize); 
	//CString Group_Info_path("D:\\exe2014\\IVS_Demo\\Group_Info.ini");
	///2018-02-26
	IVS_CHAR	Test_cDomainCode[128];
	memset(Test_cDomainCode,0, sizeof(Test_cDomainCode));
	memcpy(Test_cDomainCode,CDomainCode,IVS_DOMAIN_CODE_LEN);

	
	///2018-02-26
	IVS_INT32 iRet = IVS_SDK_GetDeviceGroup(g_iSessionID,Test_cDomainCode, "0", pDeviceGroupList, uiBufSize);// 域编码通过IVS_SDK_GetDomainRoute接口获取；系统默认的根设备组ID是“0”，传入“0”可获取根设备组下的所有子设备组 
	if (IVS_SUCCEED == iRet)  
	{  
		//成功 打印设备组信息
		GqyInsertLog(GQY_LOG_INFO,"域:%s,设备分组总数为=%d,信息如下：\n",CDomainCode,pDeviceGroupList->uiTotal);
		CString DeviceGroupIndex;
		for (IVS_UINT32 i=0; i<pDeviceGroupList->uiTotal; i++)
		{
			IVS_DEVICE_GROUP DeviceGroup;
			// 拷贝设备组所属域编码
			memcpy(DeviceGroup.cDomainCode, pDeviceGroupList->stDeviceGroup[i].cDomainCode, IVS_DOMAIN_CODE_LEN);

			// 拷贝设备组编码
			memcpy(DeviceGroup.cGroupCode, pDeviceGroupList->stDeviceGroup[i].cGroupCode, IVS_DEVICE_GROUP_LEN);
			
			// 拷贝父设备组编码
			memcpy(DeviceGroup.cParentGroupCode, pDeviceGroupList->stDeviceGroup[i].cParentGroupCode, IVS_DEVICE_GROUP_LEN);

			// 拷贝设备组名称
			memcpy(DeviceGroup.cGroupName, pDeviceGroupList->stDeviceGroup[i].cGroupName, IVS_NAME_LEN);
			
			//是否属于外域
			DeviceGroup.bIsExDomain=pDeviceGroupList->stDeviceGroup->bIsExDomain;
			GqyInsertLog(GQY_LOG_INFO,"cGroupName=%s,cDomainCode=%s,cGroupCode=%s,cParentGroupCode=%s",DeviceGroup.cGroupName,DeviceGroup.cDomainCode,DeviceGroup.cGroupCode,DeviceGroup.cParentGroupCode);

			//if (0==DeviceGroup.bIsExDomain &&strcmp(DeviceGroup.cParentGroupCode,"0")==0)
			//{//分组父节点为  分组父ID 为1
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
			{//发GQY_Group.bIsExDomin==1,分组父ID 为0

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
/////分页获取所有的设备信息
IVS_INT32 GetStepDeviceList(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceType,
	const IVS_INDEX_RANGE* pIndexRange,
	IVS_VOID* pDeviceList,
	IVS_UINT32 uiBufferSize)
{
	if (NULL == pIndexRange || NULL == pDeviceList)
	{
		return IVS_PARA_INVALID;
	}
	// 初始化开辟的内存空间
	memset(pDeviceList, 0, uiBufferSize);

	// 分页获取指定域下的设备列表
	if (IVS_SUCCEED !=IVS_SDK_GetDeviceList(g_iSessionID, uiDeviceType, pIndexRange, pDeviceList, uiBufferSize))
	{
		// 失败
		printf("IVS_SDK_GetDeviceList fail!");
		
	}

	// 这里只判断了摄像机设备，是因为只需要获取摄像机设备。如果需要获取其他类型设备，则需要根据uiDeviceType转换成对应的结构体指针
	IVS_CAMERA_BRIEF_INFO_LIST *pCamDeviceList = NULL;
	if (DEVICE_TYPE_CAMERA == uiDeviceType)
	{
		pCamDeviceList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST *>(pDeviceList);
	}
	// 将获取的设备组信息插入list中
	// 实际获取到的摄像机设备个数。如果总数大于uiGetStep，则RealNum为uiGetStep，否则为总数
	IVS_UINT32 uiRealNum = 0;
	if (0 == pCamDeviceList->stIndexRange.uiToIndex)
	{
		uiRealNum = 0;
	}
	else
	{
		uiRealNum = pCamDeviceList->stIndexRange.uiToIndex - pCamDeviceList->stIndexRange.uiFromIndex + 1;
	}
	GqyInsertLog(GQY_LOG_INFO,"摄像总数为：%d",uiRealNum);
	// 循环插入list中
	for(IVS_UINT32 ui = 0; ui < uiRealNum; ui++)
	{
		IVS_CAMERA_BRIEF_INFO stCameraInfo;
		IVS_CHAR cTmp[IVS_CAMERA_NAME_LEN] = {0};
		//// 拷贝设备所属域编码
		//memcpy(cTmp, pCamDeviceList->stCameraBriefInfo[ui].cDomainCode, IVS_DOMAIN_CODE_LEN);
		//stCameraInfo.cDomainCode = cTmp;
		//memset(cTmp, 0, IVS_NAME_LEN);

		// 拷贝设备所属设备组编码
		memcpy(stCameraInfo.cDevGroupCode, pCamDeviceList->stCameraBriefInfo[ui].cDevGroupCode, IVS_DEVICE_GROUP_LEN);
		

		// 拷贝摄像机设备编码
		memcpy(stCameraInfo.cCode, pCamDeviceList->stCameraBriefInfo[ui].cCode, IVS_DEV_CODE_LEN);
		
		// 拷贝摄像机设备名称
		memcpy(stCameraInfo.cName, pCamDeviceList->stCameraBriefInfo[ui].cName, IVS_CAMERA_NAME_LEN);
		
		//// 拷贝设备类型
		//stCameraInfo.uiType = pCamDeviceList->stCameraBriefInfo[ui].uiType;

		// 拷贝摄像机状态
		stCameraInfo.uiStatus = pCamDeviceList->stCameraBriefInfo[ui].uiStatus;
		//// 是否为外部域 0-否  1-是
		//stCameraInfo.bIsExDomain=pCamDeviceList->stCameraBriefInfo[ui].bIsExDomain;
		//发送设备信息

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
	// 判断是否还需要继续获取
	// 摄像机设备总数
	IVS_UINT32 uiTotal = pCamDeviceList->uiTotal;
	// 如果没获取完，再递归获取
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
	//IVS获取域和相应的分组信息
	IVS_INT32 iRet=GetDomainRoute();
	if (IVS_SUCCEED!=iRet)
	{
		GqyInsertLog(GQY_LOG_INFO,"GetDomainRoute error!\n");
	}

	//IVS获取分组列表

	IVS_INDEX_RANGE IndexRange = {1, uiGetStep};//索引范围
	//这里之分配一次内存，递归调用的时候不知道，会不会出错。
	IVS_UINT32 uiReqBufSize = sizeof(IVS_CAMERA_BRIEF_INFO_LIST) + (uiGetStep-1) * sizeof(IVS_CAMERA_BRIEF_INFO);//所需内存大小           
	IVS_CAMERA_BRIEF_INFO_LIST* pCameraInfoList = reinterpret_cast<IVS_CAMERA_BRIEF_INFO_LIST*>(new IVS_CHAR[uiReqBufSize]);//分配内存
	memset(pCameraInfoList,0,uiReqBufSize);
	GetStepDeviceList(g_iSessionID,DEVICE_TYPE_CAMERA,
		&IndexRange,
		pCameraInfoList,
		uiReqBufSize);
	delete[] pCameraInfoList;//释放内存
	pCameraInfoList = NULL;
	///
//////////////////////////////////////////////////////////////////////////2018-2-11

//	//本地测试代码2-11
///////////////获取路由信息
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
//		{//获取域编码为空停止获取
//			break;
//		}
//		if (strcmp(GQY_Route.cSuperDomain,"")==0)
//		{//为根节点，发送根节点
//
//			string msg1= GQY_Route.cDomainName;
//			
//			msg1 += ",0";
//			//CString str;
//			//str.Format(_T("%s"), msg1.c_str());
//			SdSrcInfo2Cli(3, (char*)msg1.c_str()); // 发3 表示发送根节点
//			printf("SdSrcInfo2Cli3=%s\n",	msg1);
//		}
//		else
//		{//不是根节点发送分组
//			char msg[254];
//			sprintf(msg, "%s,%d,%s,%s", "0",1,GQY_Route.cDomainName,"1");
//			SdSrcInfo2Cli(1, msg);
//			printf("SdSrcInfo2Cli1=%s\n",	msg);
//		}
//	}
//////获取分组信息
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
//		{//获取组编码为空停止获取
//			break;
//
//		}
//		if (0==GQY_Group.bIsExDomain &&strcmp(GQY_Group.cParentGroupCode,"0")==0)
//		{//分组父节点为  分组父ID 为1
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
//		{//发GQY_Group.bIsExDomin==1,分组父ID 为0
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
///////获取设备信息
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
//		/*int n=tempcode.Find("#");//摄像机编码不需要去掉#
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
//	//本地测试代码2-11
//			//string msg="华为平台根节点1";
			//string msg1 = replaceAllComma(msg);
			//msg1 += ",0";
			////CString str;
			////str.Format(_T("%s"), msg1.c_str());
			//SdSrcInfo2Cli(3, (char*)msg1.c_str()); // 发3 表示发送根节点
			//printf("SdSrcInfo2Cli3=%s",	msg1);
	//////////////////////////////////////////////////////////////////////////////////////////////		
			//1,分组ID
			//2,当前命令下摄像头的数量
			//3,设备名称
			//4,设备ID							//发0   表示发送设备信息
			//5，在线状态
			//sprintf(smsg, "%s,%d,%s,%s,%d", cid.c_str(),1,title.c_str(),szid.c_str(),1);
			//SdSrcInfo2Cli(0, smsg);
///////////////////////////////////////////////////////////////////////////////////////////////////
			//1.pid 父节点ID
			//2.当前命令下的分组数量
			//3.当前分组的名称				  //发1   表示发送分组信息
			//4.当前分组的ID
			//sprintf(smsg, "%s,%d,%s,%s", pid.c_str(),1,path.c_str(),cid.c_str());
			//SdSrcInfo2Cli(1, smsg);
//////////////////////////////////////////////////////////////////////////////////////////////////////
	SdSrcInfo2Cli(2, "0");             ///发2 表示发送结束 
	GqyInsertLog(GQY_LOG_INFO,"SdSrcInfo2Cli2");
	GqyInsertLog(GQY_LOG_INFO,"传输完毕\r\n");
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
	IVS_REALPLAY_PARAM RealplayPara = {0};//实况播放参数
	RealplayPara.bDirectFirst = 0;
	RealplayPara.bMultiCast = 0;
	RealplayPara.uiProtocolType = 2;
	RealplayPara.uiStreamType = STREAM_TYPE_MAIN;
	IVS_ULONG pHandle=0;

	
	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayCBFrame
    * description	: 开始实时浏览，以回调方式输出YUV流
    * input			: iSessionID               登录成功后的会话ID
						  pRealplayPara            实况参数，详见实况参数结构体
						  pCamerCode               摄像头编码
						  fRealPlayCallBackFrame   实时浏览回调函数
						  pUserData                用户数据，自定义，相应回调函数被调用时返回该参数
    * output		: pHandle                  播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	
	IVS_INT32 iRet = IVS_SDK_StartRealPlayCBFrame(g_iSessionID, &RealplayPara, CCameraCode, fRealPlayCallBackFrame,netid, &pHandle);
	//printf("g_iSessionID=%d,   RealplayPara.uiProtocolType=%d\r\n",g_iSessionID,RealplayPara.uiProtocolType);
	//printf("iRet=%d,g_Handle=%d\r\n",iRet,pHandle);
	if(iRet==IVS_SUCCEED)
	{  ///开窗成功后需要保存这个窗口句柄
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
		//printf("摄像机编号为：%s\n",Cameracode.c_str());
		GqyInsertLog(GQY_LOG_INFO,"开窗摄像机编号为：%s\n",Cameracode.c_str());
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
	{//删除关闭窗口的句柄
		SAFE_DELETE(op) 
	}

}
