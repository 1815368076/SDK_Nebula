#include "StdAfx.h"
#include "RbtEventHandler.h"


CRbtEventHandler::CRbtEventHandler(void)
{
}

CRbtEventHandler::~CRbtEventHandler(void)
{
}

void CRbtEventHandler::SetMsgReceiver(HWND hWnd)
{
	m_hMainWnd = hWnd;
}

//�豸����¼�
void CRbtEventHandler::onDeviceChanged(eDeviceStatus status,int pid)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_DEVICE_CHANGE), (WPARAM)status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_DEVICE_CHANGE, (LPARAM)&status);
	}
}
//�豸����¼�
void CRbtEventHandler::onDeviceChanged(eDeviceStatus status,eDeviceType type)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_DEVICE_CHANGE), (WPARAM)status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_DEVICE_CHANGE, (LPARAM)&status);
	}
}

//����״̬�¼�
void CRbtEventHandler::onGatewayStatus(eGatewayStatus status)	
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_GATEWAY_STATUS), (WPARAM)&status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_GATEWAY_STATUS, (LPARAM)&status);
	}
}
//node״̬�¼�
void CRbtEventHandler::onNodeStatus(const NODE_STATUS &status)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_NODE_STATUS), (WPARAM)&status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_NODE_STATUS, (LPARAM)&status);
	}
}
//�汾�¼�
void CRbtEventHandler::onDeviceInfo(const ST_DEVICE_INFO &info)	
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_GATEWAY_VERSION), (WPARAM)&info, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_GATEWAY_VERSION, (LPARAM)&info);
	}
}
//����״̬�¼�
void CRbtEventHandler::onOnlineStatus(uint8_t *status)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_ONLINE_STATUS), (WPARAM)status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_ONLINE_STATUS, (LPARAM)&status);
	}
}
//��ѡ�����¼�
void CRbtEventHandler::onExitVote(uint8_t *status)	
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_EXIT_VOTE), (WPARAM)status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_EXIT_VOTE, (LPARAM)&status);
	}
}
//��ѡ�����¼�
void CRbtEventHandler::onExitVoteMulit(const ST_OPTION_PACKET &packet)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_EXIT_VOTE_MULIT), (WPARAM)&packet, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_EXIT_VOTE_MULIT, (LPARAM)&packet);
	}
}
//���������������¼�
void CRbtEventHandler::onMassData(int index,const PEN_INFO &penInfo)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_MASS_DATA), (WPARAM)&index, (LPARAM)&penInfo);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_MASS_DATA, (LPARAM)&penInfo);
	}
}
//���ش����¼�
void CRbtEventHandler::onGatewayError(eNebulaError error)	
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_GATEWAY_ERROR), (WPARAM)&error, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_GATEWAY_ERROR, (LPARAM)&error);
	}
}
//�����豸����¼�
void CRbtEventHandler::onSetDeviceNum(int result,int customid, int classid, int deviceid)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SET_DEVICE_NUM), (WPARAM)status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SET_DEVICE_NUM, (LPARAM)&result);
	}
}
//���������¼�
void CRbtEventHandler::onFirmwareData(int progress)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_FIRMWARE_DATA), (WPARAM)&progress, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_FIRMWARE_DATA, (LPARAM)&progress);
	}
}
//��������¼�
void CRbtEventHandler::onRawResult(int result)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_RAW_RESULT), (WPARAM)&result, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_RAW_RESULT, (LPARAM)&result);
	}
}
//�����¼�
void CRbtEventHandler::onGatewayReboot()
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_GATEWAY_REBOOT), 0, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_GATEWAY_REBOOT, 0);
	}
}
//���������¼�
void CRbtEventHandler::onOriginalPacket(const PEN_INFO &penInfo)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_ORIGINAL_PACKET), (WPARAM)status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_ORIGINAL_PACKET, 0);
	}
}
//nodeģʽ�¼�
void CRbtEventHandler::onNodeMode(eNodeMode mode)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_NODE_MODE), (WPARAM)&mode, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_ORIGINAL_PACKET, (LPARAM)&mode);
	}
}
//����rtc�¼�
void CRbtEventHandler::onSetRtc(int result)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SET_RTC), (WPARAM)&result, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SET_RTC, (LPARAM)&result);
	}
}
//���������¼�
void CRbtEventHandler::onKeyPress(int result)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_KEY_PRESS), (WPARAM)&result, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_KEY_PRESS, (LPARAM)&result);
	}
}
//ҳ����ʾ�¼�
void CRbtEventHandler::onShowPage(int count, int current)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SHOW_PAGE), (WPARAM)&count,(LPARAM)&current);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SHOW_PAGE, (LPARAM)&count);
	}
}
//���߱ʼ����������¼�
void CRbtEventHandler::onSyncPacket(const PEN_INFO &penInfo)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SYNC_PACKET), (WPARAM)&penInfo, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SYNC_PACKET, (LPARAM)&penInfo);
	}
}
//���߱ʼǿ�ʼ
void CRbtEventHandler::onSyncBegin(int noteNum,const ST_RTC_INFO &rtcInfo)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SYNC_TRANS_BEGIN), 0, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SYNC_TRANS_BEGIN, 0);
	}
}
//���߱ʼǽ���
void CRbtEventHandler::onSyncEnd(int result)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SYNC_TRANS_END), 0, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SYNC_TRANS_END, 0);
	}
}
//�ϱ�ҳ��
void CRbtEventHandler::onMassShowPage(int index,int pageNo)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_PAGE_NO), (WPARAM)&pageNo, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SHOW_PAGE, 0);
	}
}
//�����¼�
void CRbtEventHandler::onVoteAnswer(int index,int answer)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_VOTE_ANSWER), 0, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_VOTE_ANSWER, 0);
	}
}
//////////////////////////////dongle//////////////////////
//Dongle״̬�¼�
void CRbtEventHandler::onDongleStatus(eDongleStatus status,int mode)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_DONGLE_STATUS), (WPARAM)&status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_DONGLE_STATUS, 0);
	}
}
//Dongle�汾�¼�
void CRbtEventHandler::onDongleVersion(const ST_VERSION &version)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_DONGLE_VERSION), (WPARAM)&version, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_DONGLE_VERSION, 0);
	}
}
//Dongleɨ���¼�
void CRbtEventHandler::onDongleScanRes(const ST_BLE_DEVICE &device)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_DONGLE_SCAN_RES), (WPARAM)&device, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_DONGLE_SCAN_RES, 0);
	}
}
//slave�汾�¼�
void CRbtEventHandler::onSlaveVersion(eDeviceType type,const ST_VERSION &version)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SLAVE_VERSION), 0, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SLAVE_VERSION, 0);
	}
}
//slave״̬�¼�
void CRbtEventHandler::onSlaveStatus(const NODE_STATUS &status)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SLAVE_STATUS), (WPARAM)&status, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SLAVE_STATUS, 0);
	}
}
//���������¼�
void CRbtEventHandler::onSetName(int result)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SET_NAME), (WPARAM)&result, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SET_NAME, 0);
	}
}
//slave�����¼�
void CRbtEventHandler::onSlaveError(eSlaveError error)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_SLAVE_ERROR), (WPARAM)&error, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_SLAVE_ERROR, 0);
	}
}

//�ʼ��Ż����
void CRbtEventHandler::onOut(float x,float y,float width,int press,int status)
{
	if(m_hMainWnd != NULL)
	{
		//::PostMessage(m_hMainWnd, WM_MSGID(ROBOT_OPTIMIZE_PACKET), 0, 0);
		::PostMessage(m_hMainWnd, WM_RBTEVENT, ROBOT_OPTIMIZE_PACKET, 0);
	}
}