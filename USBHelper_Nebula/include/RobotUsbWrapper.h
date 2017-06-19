#pragma once
#include <windows.h>

#if defined(DLL_EXPORT)
#define DECLDIR __declspec(dllexport) 
#else 
#define DECLDIR __declspec(dllimport) 
#include "common.h"
#endif

enum eRbtType
{
	VoteBegin = 0,
	VoteEnd,
	WriteBegin,
	WriteEnd,
	SyncBegin,
	SyncEnd,
	UpdateStop,
	GetConfig,
	DongleScanStart,
	DongleScanStop,
	DongleDisconnect,
	VoteAnswer,
	EnterUsb,
	ExitUsb,
	AdjustMode,
};

//�ص�����
typedef void (CALLBACK *UsbDataCallback_t)(const unsigned char*,int,void*);

class IRobotEventHandler
{
public:
	virtual ~IRobotEventHandler() {}
	//�豸����¼�
	virtual void onDeviceChanged(eDeviceStatus status,int pid) {
		(void)status;
		(void)pid;
	}
	//����״̬�¼�
	virtual void onGatewayStatus(eGatewayStatus status){
		(void)status;
	}
	//node״̬�¼�
	virtual void onNodeStatus(const NODE_STATUS &status){
		(void)status;
	}
	//�汾�¼�
	virtual void onDeviceInfo(const ST_DEVICE_INFO &info) {
		(void)info;
	}
	//����״̬�¼�
	virtual void onOnlineStatus(uint8_t *status) {
		(void)status;
	}
	//��ѡ�����¼�
	virtual void onExitVote(uint8_t *status) {
		(void)status;
	}
	//��ѡ�����¼�
	virtual void onExitVoteMulit(const ST_OPTION_PACKET &packet) {
		(void)packet;
	}
	//���������������¼�
	virtual void onMassData(int index,const PEN_INFO &penInfo) {
		(void)index;
		(void)penInfo;
	}
	//���ش����¼�
	virtual void onGatewayError(eNebulaError error) {
		(void)error;
	}
	//�����豸����¼�
	virtual void onSetDeviceNum(int result,int customid, int classid, int deviceid) {
		(void)result;
		(void)customid;
		(void)classid;
		(void)deviceid;
	}
	//���������¼�
	virtual void onFirmwareData(int progress) {
		(void)progress;
	}
	//��������¼�
	virtual void onRawResult(int result) {
		(void)result;
	}
	//�����¼�
	virtual void onGatewayReboot() {
	}
	//���������¼�
	virtual void onOriginalPacket(const PEN_INFO &penInfo) {
		(void)penInfo;
	}
	//nodeģʽ�¼�
	virtual void onNodeMode(eNodeMode mode) {
		(void)mode;
	}
	//����rtc�¼�
	virtual void onSetRtc(int result) {
		(void)result;
	}
	//���������¼�
	virtual void onKeyPress(int result) {
		(void)result;
	}
	//ҳ����ʾ�¼�
	virtual void onShowPage(int count, int current) {
		(void)count;
		(void)current;
	}
	//���߱ʼ����������¼�
	virtual void onSyncPacket(const PEN_INFO &penInfo) {
		(void)penInfo;
	}
	//���߱ʼǿ�ʼ
	virtual void onSyncBegin(int noteNum,const ST_RTC_INFO &rtcInfo){
		(void)noteNum;
		(void)rtcInfo;
	}
	//���߱ʼǽ���
	virtual void onSyncEnd(int result){
		(void)result;
	}
	//�ϱ�ҳ��
	virtual void onPageNo(int pageNo){
		(void)pageNo;
	}
	//�����¼�
	virtual void onVoteAnswer(int index,int answer){
		(void)index;
		(void)answer;
	}
	//////////////////////////////dongle//////////////////////
	//Dongle״̬�¼�
	virtual void onDongleStatus(eDongleStatus status,int mode) {
		(void)status;
		(void)mode;
	}
	//Dongle�汾�¼�
	virtual void onDongleVersion(const ST_VERSION &version) {
		(void)version;
	}
	//Dongleɨ���¼�
	virtual void onDongleScanRes(const ST_BLE_DEVICE &device) {
		(void)device;
	}
	//slave�汾�¼�
	virtual void onSlaveVersion(eDeviceType type,const ST_VERSION &version) {
		(void)type;
		(void)version;
	}
	//slave״̬�¼�
	virtual void onSlaveStatus(const NODE_STATUS &status) {
		(void)status;
	}
	//���������¼�
	virtual void onSetName(int result) {
		(void)result;
	}
	//slave�����¼�
	virtual void onSlaveError(eSlaveError error) {
		(void)error;
	}
	//ģ��汾�¼�
	virtual void onModuleVersion(const ST_MODULE_VERSION &version) {
		(void)version;
	}
	//����ģ��У׼�¼�
	virtual void onAjdustMode() {
	}
	//ģ��У׼����¼�
	virtual void onAjdustResult(int result) {
		(void)result;
	}


	//�ʼ��Ż����
	virtual void onOut(float x,float y,float width,float speed,int status){
		(void)x;
		(void)y;
		(void)width;
		(void)speed;
		(void)status;
	}
};

class RobotPenController
{
public:
	//��ʼ�� �ص�
	virtual void ConnectInitialize(eDeviceType nDeviceType,UsbDataCallback_t pCallback = NULL, void *pContext = NULL) = 0;
	//��ʼ�� �¼�
	virtual void ConnectInitialize(eDeviceType nDeviceType,IRobotEventHandler *pEventHander = NULL) = 0;
	//�����豸���ӣ��ɹ����Զ��������ݽ���
	virtual int  ConnectOpen() = 0;
	//�ر��豸���ӣ��ɹ����Զ��ر����ݽ���
	virtual void ConnectDispose() = 0;
	//�ж��豸�Ƿ�������״̬
	virtual bool IsConnected() = 0;
	//��������
	virtual void Send(int nCmd) = 0;
	//����
	virtual bool Update(const char *fileMcu,const char *fileBle,eDeviceType type = Unknow) = 0;
	//����
	virtual void SetConfig(int nCostumNum,int nClassNum,int nDeviceNum) = 0;
	//��ȡ�����豸����
	virtual int GetDeviceCount() = 0;
	//��ȡ�����豸
	virtual DWORD GetAvailableDevice() = 0;
	virtual bool GetDeviceInfo(int index,USB_INFO &usbInfo) = 0;
	//����PID��VID���豸
	virtual int Open(int nVid,int nPid,bool bAll = true) = 0;
	//���������豸
	virtual void ConnectSlave(int nID) = 0;
	//������������
	virtual void SetSlaveName(const char *name) = 0;
	//�����豸����
	virtual void SetDeviceType(eDeviceType nDeviceType) = 0;
	//��������ƫ��
	virtual void SetOffset(int nOffsetX,int nOffsetY) = 0;
	//��������
	virtual void SetIsHorizontal(bool bHorizontal) = 0;
	//��ȡ�豸��
	virtual int Width() = 0;
	//��ȡ�豸��
	virtual int Height() = 0;
	//��ת�Ƕ�
	virtual void Rotate(int nAngle = 0) = 0;
	//��ʼͶƱ
	virtual void VoteMulit(bool bMulit = true) = 0;
	//���ñʿ��
	virtual void SetPenWidth(float nPenWidth = 0) = 0;
	//���û�����С
	virtual void SetCanvasSize(int nWidth,int nHeight) = 0;
	//�ʼ��Ż�
	virtual void In(const PEN_INFO &penInfo) = 0;
	//�Ƿ���ѹ��
	virtual void SetPressStatus(bool bPress) = 0;
};

//��ʼ�� �ص�
extern "C" DECLDIR void  ConnectInitialize(eDeviceType nDeviceType, IN UsbDataCallback_t pCallback, void *pContext);
//��ʼ�� �¼�
extern "C" DECLDIR void  ConnectInitialize2(eDeviceType nDeviceType, IN IRobotEventHandler *pEventHander);
//�����豸���ӣ��ɹ����Զ��������ݽ���
extern "C" DECLDIR int   ConnectOpen();
//�ر��豸���ӣ��ɹ����Զ��ر����ݽ���
extern "C" DECLDIR void  ConnectDispose();
//�ж��豸�Ƿ�������״̬
extern "C" DECLDIR bool  IsConnected();
//��������
extern "C" DECLDIR void  Send(int nCmd);
//����
extern "C" DECLDIR void  Update(const char *fileMcu,const char *fileBle,eDeviceType type = Unknow);
//����
extern "C" DECLDIR void  SetConfig(int nCostumNum,int nClassNum,int nDeviceNum);
//��ȡ�����豸����
extern "C" DECLDIR int GetDeviceCount();
//��ȡ�����豸
extern "C" DECLDIR bool GetDeviceInfo(int index,USB_INFO &usbInfo);
//����PID��VID���豸
extern "C" DECLDIR int  Open(int nVid,int nPid,bool bAll = true);
//���������豸
extern "C" DECLDIR void ConnectSlave(int nID);
//������������
extern "C" DECLDIR void SetSlaveName(const char *name);
//���û�����С
extern "C" DECLDIR void SetCanvasSize(int nWidth,int nHeight);
//�����豸����
extern "C" DECLDIR void SetDeviceType(eDeviceType nDeviceType);
//��������ƫ��
extern "C" DECLDIR void SetOffset(int nOffsetX,int nOffsetY);
//��������
extern "C" DECLDIR void SetIsHorizontal(bool bHorizontal);
//��ȡ�豸��
extern "C" DECLDIR int Width();
//��ȡ�豸��
extern "C" DECLDIR int Height();
//��ת�Ƕ�
extern "C" DECLDIR void Rotate(int nAngle);
//��������
extern "C" DECLDIR void SetPenWidth(float nPenWidth);
//��ʼͶƱ
extern "C" DECLDIR void VoteMulit(bool bMulit);
//�ʼ��Ż�
extern "C" DECLDIR void In(const PEN_INFO &penInfo);
//�Ƿ���ѹ��
extern "C" DECLDIR void SetPressStatus(bool bPress);

extern "C" 
{
	//��ȡʵ�� 
	DECLDIR RobotPenController* GetInstance();
	//����ʵ��
	DECLDIR void DestroyInstance();
};