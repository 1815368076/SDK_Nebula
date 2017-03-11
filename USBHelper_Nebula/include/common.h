#include <stdint.h>

#define VID		0x0483

enum
{
	GATEWAY_PID	=	0x6001,
	NODE_PID	=	0x6002,
};
////////////////////////////////////////NEBULA///////////////////////////////////////
#pragma pack(1)
//״̬
typedef struct robot_status
{
	uint8_t	 device_status;
	uint8_t  battery_level;
	uint8_t  note_num;
}ROBOT_STATUS;
//����
typedef struct robot_report
{
	uint8_t cmd_id;
	uint8_t reserved;
	uint8_t payload[60];
}ROBOT_REPORT;
//�汾
typedef struct st_version
{
	uint8_t version;
	uint8_t version2;
	uint8_t version3;
	uint8_t version4;
}ST_VERSION;
//�豸��Ϣ
typedef struct st_device_info
{
	ST_VERSION version;
	uint8_t  custom_num;
	uint8_t  class_num;
	uint8_t  device_num;
}ST_DEVICE_INFO;

#pragma pack()

enum NEBULA_ERROR
{
	ERROR_NONE,
	ERROR_FLOW_NUM,
	ERROR_FW_LEN,
	ERROR_FW_CHECKSUM,
	ERROR_STATUS,
	ERROR_VERSION,
	ERROR_NAME_CONTENT,
	ERROR_NO_NOTE,
};

enum GATEWAY_STATUS
{
	NEBULA_STATUS_OFFLINE = 0,
	NEBULA_STATUS_STANDBY,         
	NEBULA_STATUS_VOTE,             
	NEBULA_STATUS_MASSDATA,         
	NEBULA_STATUS_END,           
};

enum NODE_STATUS
{
	DEVICE_POWER_OFF = 0,
	DEVICE_STANDBY,
	DEVICE_INIT_BTN,
	DEVICE_OFFLINE,
	DEVICE_ACTIVE,
	DEVICE_LOW_POWER_ACTIVE,
	DEVICE_OTA_MODE,//06
	DEVICE_OTA_WAIT_SWITCH,
	DEVICE_DFU_MODE,
	DEVICE_TRYING_POWER_OFF,
	DEVICE_FINISHED_PRODUCT_TEST,
	DEVICE_SYNC_MODE,
	DEVICE_SEMI_FINISHED_PRODUCT_TEST,
};

enum ROBOT_NEBULA_TYPE
{
	ROBOT_GATEWAY_STATUS			= 0x00,		//��ȡ״̬
	ROBOT_ENTER_VOTE				= 0x01,		//����ͶƱģʽ
	ROBOT_EXIT_VOTE					= 0x02,		//�˳�ͶƱģʽ
	ROBOT_ENTER_BIG_DATA			= 0x03,		//���������ģʽ
	ROBOT_BIG_DATA_REPORT			= 0x04,		//�������ϱ�
	ROBOT_EXIT_BIG_DATA				= 0x05,		//�˳�������ģʽ
	ROBOT_GATEWAY_ERROR				= 0x06,		//����
	ROBOT_NODE_MODE					= 0x07,		//�豸����ģʽ
	ROBOT_SET_DEVICE_NUM			= 0x10,		//�����豸�����
	ROBOT_ENTER_DFU					= 0x11,		//����dfuģʽ
	ROBOT_FIRMWARE_LEN				= 0x12,		//��ȡ�̼�����
	ROBOT_FIRMWARE_DATA				= 0x13,		//��ȡ�̼���Ϣ
	ROBOT_FIRMWARE_CHECK_SUM		= 0x14,		//��ȡУ���
	ROBOT_RAW_RESULT				= 0x15,		//У����
	ROBOT_GATEWAY_REBOOT			= 0x16,		//�豸����
	ROBOT_EXIT_DFU					= 0x17,		//�˳�dfuģʽ
	ROBOT_GATEWAY_VERSION			= 0x18,		//�豸�汾��
	ROBOT_ONLINE_STATUS				= 0x19,		//����״̬
	ROBOT_DEVICE_CHANGE				= 0x20,		//�豸�ı�
	ROBOT_NODE_INFO					= 0x30,		//�豸��Ϣ
	ROBOT_NODE_ERROR				= 0x31,		//node����
	ROBOT_USB_PACKET				= 0x32,		//�ϴ�����
	ROBOT_SET_RTC					= 0x33,		//����RTC
	ROBOT_KEY_PRESS					= 0x34,		//��������
	ROBOT_SHOW_PAGE					= 0x36,		//��ʾҳ��		
	ROBOT_ENTER_SYNC				= 0x40,		//����syncģʽ
	ROBOT_EXIT_SYNC					= 0x41,		//�˳�syncģʽ
	ROBOT_GET_SYNC_HEAD				= 0x42,		//��ȡ�洢�ʼǰ�ͷ
	ROBOT_SYNC_TRANS_BEGIN			= 0x43,		//�ʼǴ������ʼ
	ROBOT_ORIGINAL_PACKET			= 0x44,		//ԭʼ�ʼ����ݰ�
	ROBOT_SYNC_TRANS_END			= 0x45,		//�ʼǴ����������
};
// ��������Ϣ
typedef struct sPenInfo
{
	uint8_t nStatus;		// ��״̬
	uint16_t nX;			// ��x������
	uint16_t nY;			// ��y������
	uint16_t nPress;		// ��ѹ��
}PEN_INFO;  
//�豸��Ϣ
typedef struct usb_info
{
	char szDevName[260];
	unsigned short nVendorNum;    
	unsigned short nProductNum;         
}USB_INFO;

enum eDeviceType
{
	GATEWAY = 0,
	NODE,
	DONGLE,
};

enum
{
	CLICK = 1,
	DBCLICK,
	PAGEUP,
	PAGEDOWN,
	CREATEPAGE,
};

////////////////////////////DONGLE////////////////////////////////
#pragma pack(1)
typedef struct st_ble_device
{
	uint8_t num;
	uint8_t rssi;
	uint8_t match;
	uint8_t addr[6];
	uint8_t device_name[18];
	uint8_t device_type;
}ST_BLE_DEVICE;
#pragma pack()

//����״̬
enum BLE_STATUS
{
	BLE_STANDBY				= 0,
	BLE_SCANNING			= 1,	//����ɨ��
	BLE_CONNECTING			= 2,	//������
	BLE_CONNECTED			= 3,	//���ӳɹ�
	BLE_ACTIVE_DISCONNECT	= 4,	//���ڶϿ�����
	BLE_RECONNECTING		= 5,	//��������
	BLE_LINK_BREAKOUT		= 6,	//��������������
	BLE_DFU_START			= 7,	//����dfuģʽ
};

enum DONGLE_DEVICE
{
	P7			= 1,
	ELITE		= 2,
	ELITE_PLUS	= 3,
	J0			= 8,
};

enum ROBOT_DONGLE_TYPE
{
	ROBOT_DONGLE_STATUS			= 0x00,		//dongele״̬
	ROBOT_DONGLE_VERSION,					//dongle�汾
	ROBOT_DONGLE_SCAN_RES,					//ɨ����
	ROBOT_SET_NAME,							//��������
	ROBOT_SLAVE_ERROR,						//������Ϣ
	ROBOT_DONGLE_FIRMWARE_DATA,				//����
	ROBOT_DONGLE_RAW_RESULT,				//�������
	ROBOT_SLAVE_STATUS,						//slave״̬
	ROBOT_SLAVE_VERSION,					//slave�汾
};

enum UPDATE_TYPE
{
	DONGLE_BLE = 0,
	DONGLE_MCU,
	SLAVE_MCU,
};

enum SALVE_ERROR
{
	ERROR_SLAVE_NONE = 0,
	ERROR_OTA_FLOW_NUM,
	ERROR_OTA_LEN,
	ERROR_OTA_CHECKSUM,
	ERROR_OTA_STATUS,
	ERROR_OTA_VERSION,
};

enum SALVE_ERROR
{
	ERROR_SLAVE_NONE = 0,
	ERROR_OTA_FLOW_NUM,
	ERROR_OTA_LEN,
	ERROR_OTA_CHECKSUM,
	ERROR_OTA_STATUS,
	ERROR_OTA_VERSION,
};