#pragma once 

void GetSrcS200();
 BOOL StartRealPlayS200(const char* path, void* netid);
 IVS_INT32 GetDomainRoute();
 void Get_Device_Group(const IVS_CHAR* CDomainCode);
 IVS_INT32 GetStepDeviceList(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceType,
	 const IVS_INDEX_RANGE* pIndexRange,
	 IVS_VOID* pDeviceList,
	 IVS_UINT32 uiBufferSize);
