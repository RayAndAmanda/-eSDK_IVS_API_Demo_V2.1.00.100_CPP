#ifndef _DEMO_DEFINE_H_
#define _DEMO_DEFINE_H_

#define SDK_SESSIONID_INIT			(-1)//SessionID��ʼֵ
#define WM_UPDATEDEVICELIST		(WM_APP + 1000)
#define WM_UPDATEPRESETLIST		(WM_APP + 1001)
#define WM_UPDATERECORDLIST		(WM_APP + 1002)
#define WM_DOWNLOADSUCCESS	(WM_APP + 1003)
#define WM_UPDATEALARMLIST		(WM_APP + 1004)
#define WM_UPDATEBRODCASTLIST		(WM_APP + 1005)

// �ж�ָ���Ƿ�Ϊ�գ��޷���ֵ 
#define CHECK_POINTER_VOID(pPointer)	\
	if (NULL == (pPointer))	\
	{									\
		return;						\
	}									\

// �ж�ָ���Ƿ�Ϊ�գ��з���ֵ
#define CHECK_POINTER(pPointer, retValue)	\
	if (NULL == (pPointer))	\
	{									\
		return (retValue);					\
	}									\

//������������
typedef enum
{
	NOT_SET,							//δ��
	REAL_PLAY,						//ʵ��
	LOCAL_PLAY_BACK,			//���ػط�
	PLATFORM_PLAY_BACK,	//ƽ̨�ط�
	PU_PLAY_BACK 				//ǰ�˻ط�
}IVS_PLAY_TYPE;

#endif
