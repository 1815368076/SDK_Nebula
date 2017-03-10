#pragma once
#include <windows.h>
#include <vector>

#if defined(DLL_EXPORT)
#define DECLDIR __declspec(dllexport) 
#else 
#define DECLDIR __declspec(dllimport) 
#include "common.h"
#endif

enum cmdId
{
	VoteStart = 0,
	VoteEnd,
	WriteStart,
	WriteEnd,
	SyncStart,
	SyncEnd,
	UpdateStop,
	GetConfig,
};

//�ص�����
typedef void (CALLBACK *UsbDataCallback_t)(const unsigned char*,int,void*);

class RobotPenController
{
public:
	//��ʼ���豸����
	virtual void ConnectInitialize(int nDeviceType, UsbDataCallback_t pCallback = NULL, void *pContext = NULL) = 0;
	//�����豸���ӣ��ɹ����Զ��������ݽ���
	virtual int  ConnectOpen() = 0;
	//�ر��豸���ӣ��ɹ����Զ��ر����ݽ���
	virtual void ConnectDispose() = 0;
	//�ж��豸�Ƿ�������״̬
	virtual bool IsConnected() = 0;
	//��������
	virtual void Send(cmdId nCmd) = 0;
	//����
	virtual bool Update(const char *fileNameMcu,const char *fileNameBle) = 0;
	//����
	virtual void SetConfig(int nCostumNum,int nClassNum,int nDeviceNum) = 0;
	// �������п����豸���ϵĵ�ַ���
	virtual DWORD GetAvailableDevice() = 0;
	//����PID��VID���豸
	virtual int Open(int nVid,int nPid,bool bAll = true) = 0;
};



	//��ʼ���豸����
	extern "C" DECLDIR void  ConnectInitialize(int nDeviceType, IN UsbDataCallback_t pCallback, void *pContext);
	//�����豸���ӣ��ɹ����Զ��������ݽ���
	extern "C" DECLDIR int   ConnectOpen();
	//�ر��豸���ӣ��ɹ����Զ��ر����ݽ���
	extern "C" DECLDIR void  ConnectDispose();
	//�ж��豸�Ƿ�������״̬
	extern "C" DECLDIR bool  IsConnected();
	//��������
	extern "C" DECLDIR void  Send(cmdId nCmd);
	//����
	extern "C" DECLDIR void  Update( const char *fileNameMcu,const char *fileNameBle );
	//����
	extern "C" DECLDIR void  SetConfig(int nCostumNum,int nClassNum,int nDeviceNum);
	//����PID��VID���豸
	extern "C" DECLDIR int  Open(int nVid,int nPid,bool bAll = true);

extern "C" 
{
	//��ȡʵ�� 
	DECLDIR RobotPenController* GetInstance();
	//����ʵ��
	DECLDIR void DestroyInstance();
};