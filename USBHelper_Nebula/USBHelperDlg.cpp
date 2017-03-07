
// USBHelperDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "USBHelper.h"
#include "USBHelperDlg.h"
#include "afxdialogex.h"
#include <GdiplusGraphics.h>
#include "SettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

PCHAR WideStrToMultiStr (PWCHAR WideStr)
{
	ULONG nBytes;
	PCHAR MultiStr;

	// Get the length of the converted string
	//
	nBytes = WideCharToMultiByte(
		CP_ACP,
		0,
		WideStr,
		-1,
		NULL,
		0,
		NULL,
		NULL);

	if (nBytes == 0)
	{
		return NULL;
	}

	// Allocate space to hold the converted string
	//
	MultiStr = (PCHAR)ALLOC(nBytes);

	if (MultiStr == NULL)
	{
		return NULL;
	}

	// Convert the string
	//
	nBytes = WideCharToMultiByte(
		CP_ACP,
		0,
		WideStr,
		-1,
		MultiStr,
		nBytes,
		NULL,
		NULL);

	if (nBytes == 0)
	{
		FREE(MultiStr);
		return NULL;
	}

	return MultiStr;
}

/*static std::wstring MultiCharToWideChar( std::string str )
{
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	std::wstring return_value;
	return_value.append(buffer);
	delete [] buffer;
	return return_value;
}//*/

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CUSBHelperDlg 对话框




CUSBHelperDlg::CUSBHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUSBHelperDlg::IDD, pParent)
	, m_pDlg(NULL)
	, m_bNode(FALSE)
	, m_bRun(TRUE)
	, m_nLastStatus(-1)
	, m_nLastMode(-1)
	, m_nNoteNum(0)
{
	for (int i=0;i<2;i++)
	{
		m_hEvent[i] = CreateEvent(NULL,TRUE,FALSE,NULL);
	}
	// 初始化
	::InitializeCriticalSectionAndSpinCount(&m_sectionLock, 4000);

	memset(&m_lastInfo,0,sizeof(m_lastInfo));

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUSBHelperDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CUSBHelperDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON3_OPEN, &CUSBHelperDlg::OnBnClickedButton3Open)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_DEVICECHANGE()					//--by zlp 2016/9/27
	ON_MESSAGE(WM_CLICK, OnClick)
	ON_MESSAGE(WM_UPDATE, OnUpdate)
	ON_MESSAGE(WM_UPDATE_WINDOW, OnUpdateWindow)
	ON_BN_CLICKED(IDC_BUTTON_VOTE, &CUSBHelperDlg::OnBnClickedButtonVote)
	ON_BN_CLICKED(IDC_BUTTON_VOTE_OFF, &CUSBHelperDlg::OnBnClickedButtonVoteOff)
	ON_BN_CLICKED(IDC_BUTTON_VOTE_CLEAR, &CUSBHelperDlg::OnBnClickedButtonVoteClear)
	ON_BN_CLICKED(IDC_BUTTON3_MS, &CUSBHelperDlg::OnBnClickedButton3Ms)
	ON_BN_CLICKED(IDC_BUTTON3_MS_OFF, &CUSBHelperDlg::OnBnClickedButton3MsOff)
	ON_BN_CLICKED(IDC_BUTTON_STATUS, &CUSBHelperDlg::OnBnClickedButtonStatus)
	ON_BN_CLICKED(IDC_BUTTON_MS_CLEAR, &CUSBHelperDlg::OnBnClickedButtonMsClear)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON3_SET, &CUSBHelperDlg::OnBnClickedButton3Set)
	ON_BN_CLICKED(IDC_BUTTON3_UPDATE, &CUSBHelperDlg::OnBnClickedButton3Update)
	ON_BN_CLICKED(IDC_BUTTON3_SHOW, &CUSBHelperDlg::OnBnClickedButton3Show)
END_MESSAGE_MAP()


// CUSBHelperDlg 消息处理程序

BOOL CUSBHelperDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//MoveWindow(0,0,1600,900);
	this->ShowWindow(SW_MAXIMIZE);

	InitListCtrl();

	resetUI();
	//AfxBeginThread(ThreadProc,this);
	m_pDlg = new CUpdateDlg;
	m_pDlg->Create(IDD_UPDATEDLG);

	GetDlgItem(IDC_STATIC_SV)->SetWindowText(_T("版本号：0302"));

	AddList();

	AfxBeginThread(ThreadProc,this);

	GetInstance()->ConnectInitialize(GATEWAY,getUsbData,this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CUSBHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if (nID == SC_MINIMIZE)
		{
			if (m_pDlg->IsWindowVisible())
				return;
		}
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUSBHelperDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUSBHelperDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUSBHelperDlg::InitListCtrl()
{
	CListCtrl* pListView = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_USB_DEVICE));
	if (NULL == pListView)
		return;

	pListView->InsertColumn(0, _T("设备名称"), LVCFMT_LEFT, 180);
	pListView->InsertColumn(1, _T("VID"), LVCFMT_LEFT, 80);
	pListView->InsertColumn(2, _T("PID"), LVCFMT_LEFT, 80);
	pListView->SetExtendedStyle (LVS_EX_FULLROWSELECT |LVS_EX_GRIDLINES );//设置扩展
}

void CUSBHelperDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();//*/
}

void CUSBHelperDlg::AddList()
{
	CListCtrl* pListView = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_USB_DEVICE));
	if (NULL == pListView)
		return;

	pListView->DeleteAllItems();

	std::vector<USB_INFO> vecUsbInfo;
	int nCount = GetInstance()->GetAvailableDevice(vecUsbInfo);

	for (int i=0;i<vecUsbInfo.size();i++)
	{
		int nIndex = pListView->GetItemCount();
		pListView->InsertItem(i,MultiCharToWideChar(vecUsbInfo[i].szDevName).c_str());
		CString str;
		str.Format(_T("%d"),vecUsbInfo[i].nVendorNum);
		pListView->SetItemText(nIndex,1,str);
		str.Format(_T("%d"),vecUsbInfo[i].nProductNum);
		pListView->SetItemText(nIndex,2,str);
	}
	if (pListView->GetItemCount() > 0)
	{
		pListView->SetItemState(0,LVNI_FOCUSED | LVIS_SELECTED, LVNI_FOCUSED | LVIS_SELECTED);
	}
}
// 打开设备
void CUSBHelperDlg::OnBnClickedButton3Open()
{
	// TODO: 在此添加控件通知处理程序代码

	CString csBtnTitle;
	GetDlgItemText(IDC_BUTTON3_OPEN,csBtnTitle);
	if (csBtnTitle.Compare(_T("关闭设备")) == 0)
	{
		GetInstance()->ConnectDispose();
		resetDevice();
		return;
	}

	POSITION pos = ((CListCtrl*)GetDlgItem(IDC_LIST_USB_DEVICE))->GetFirstSelectedItemPosition();
	if (pos == nullptr)
	{
		//int nRes = GetInstance()->Open(0x0ED1,0x7806);
		int nRes = GetInstance()->Open(0x0ED1,0x781E);
		ASSERT(nRes == 0);
		if(nRes == 0)
			GetDlgItem(IDC_BUTTON3_OPEN)->SetWindowText(_T("关闭设备"));
		return;
		MessageBox(_T("请先选中设备!"), _T("提示"), IDOK);
		return;
	}

	int nItem = ((CListCtrl*)GetDlgItem(IDC_LIST_USB_DEVICE))->GetNextSelectedItem(pos);
	CString strVid = ((CListCtrl*)GetDlgItem(IDC_LIST_USB_DEVICE))->GetItemText(nItem, 1);
	CString strPid = ((CListCtrl*)GetDlgItem(IDC_LIST_USB_DEVICE))->GetItemText(nItem, 2);

	int nVid = atoi(WideStrToMultiStr(strVid.GetBuffer()));
	int nPid = atoi(WideStrToMultiStr(strPid.GetBuffer()));

	if (nPid == GATEWAY_PID)
		GetInstance()->ConnectInitialize(GATEWAY,getUsbData,this);
	else
		GetInstance()->ConnectInitialize(NODE,getUsbData,this);

	int nRes = GetInstance()->ConnectOpen();
	ASSERT(nRes == 0);
	if(nRes == 0)
		GetDlgItem(IDC_BUTTON3_OPEN)->SetWindowText(_T("关闭设备"));

	m_bNode = ((nPid == NODE_PID) ? TRUE :FALSE);


	/*GetDlgItem(IDC_BUTTON_VOTE)->EnableWindow(!m_bNode);
	GetDlgItem(IDC_BUTTON_VOTE_OFF)->EnableWindow(!m_bNode);//*/
	if(!m_bNode)
	{
		GetDlgItem(IDC_BUTTON_VOTE)->SetWindowText(_T("发起投票"));
		GetDlgItem(IDC_BUTTON_VOTE_OFF)->SetWindowText(_T("结束投票"));
	}
	else
	{
		GetDlgItem(IDC_BUTTON_VOTE)->SetWindowText(_T("开始同步"));
		GetDlgItem(IDC_BUTTON_VOTE_OFF)->SetWindowText(_T("结束同步"));
	}
	GetDlgItem(IDC_BUTTON_VOTE_CLEAR)->EnableWindow(!m_bNode);
	GetDlgItem(IDC_BUTTON3_MS)->EnableWindow(!m_bNode);
	GetDlgItem(IDC_BUTTON3_MS_OFF)->EnableWindow(!m_bNode);
	GetDlgItem(IDC_BUTTON_MS_CLEAR)->EnableWindow(!m_bNode);

	GetDlgItem(IDC_STATIC_DEV)->ShowWindow(m_bNode);
	GetDlgItem(IDC_EDIT_DEV)->ShowWindow(m_bNode);

	GetDlgItem(IDC_STATIC_MODE_NAME)->ShowWindow(m_bNode);
	GetDlgItem(IDC_STATIC_MODE)->ShowWindow(m_bNode);

	if (m_bNode)
		OnBnClickedButtonStatus();
}

void CUSBHelperDlg::OnDestroy()		//--by zlp 2016/9/26
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	resetDevice();

	for(int i=0;i<m_list.size();i++)
	{
		CDrawDlg *pDlg = m_list[i];
		pDlg = NULL;
	}//*/

	DestroyInstance();

	Sleep(100);
}

void CUSBHelperDlg::resetDevice()
{
	GetDlgItem(IDC_BUTTON3_OPEN)->SetWindowTextW(_T("打开设备"));

	for (int i=0;i<m_list.size();i++)
	{
		CDrawDlg *pDlg = m_list[i];
		if (NULL != pDlg)
		{
			pDlg->SetOnLine();
			pDlg->ResetUI();
		}
	}

	GetDlgItem(IDC_EDIT_CUSTOM)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_CLASS)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_DEV)->SetWindowText(_T(""));

	GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowTextW(_T(""));

	GetDlgItem(IDC_STATIC_MODE_NAME)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MODE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_MODE)->SetWindowTextW(_T(""));

	m_nLastStatus = -1;
	m_nLastMode = -1;
	memset(&m_lastInfo,0,sizeof(m_lastInfo));

	AddList();
}

UINT CUSBHelperDlg::ThreadProc(LPVOID lpParam)
{
	CUSBHelperDlg *pDlg = (CUSBHelperDlg*)lpParam;
	pDlg->ProcessMassData();

	return 0;
}

void CUSBHelperDlg::ProcessMassData()
{
	while(m_bRun)
	{
		/*if(WaitForSingleObject(m_hEvent[1],0) == WAIT_OBJECT_0 )
		break;//*/
		DWORD dwResult = WaitForMultipleObjects(2,m_hEvent,FALSE,INFINITE);

		switch(dwResult - WAIT_OBJECT_0)
		{
		case 0:
			{
				std::queue<ROBOT_REPORT> tmpQueue;
				::EnterCriticalSection(&m_sectionLock);
				while(m_queueData.size() > 0)
				{
					tmpQueue.push(m_queueData.front());
					m_queueData.pop();
				}
				::LeaveCriticalSection(&m_sectionLock);

				while(tmpQueue.size() > 0)
				{
					ROBOT_REPORT report = tmpQueue.front();
					tmpQueue.pop();

					this->parseRobotReport(report);
				}
			}
			break;
		case 1:
			m_bRun = FALSE;
			break;
		default:
			break;
		}
	}
	TRACE("==========================ProcessMassData Exit=========================\n");
}

LRESULT CUSBHelperDlg::OnClick(WPARAM wParam, LPARAM lParam)
{
	BSTR b = (BSTR)wParam;
	CString s(b);
	//SysFreeString(b);
	AfxMessageBox(s);
	return 0;
}

void CUSBHelperDlg::OnBnClickedButtonVote()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bNode)
		GetInstance()->Send(SyncStart);
	else
		GetInstance()->Send(VoteStart);
}

void CUSBHelperDlg::OnBnClickedButtonVoteOff()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bNode)
		GetInstance()->Send(SyncEnd);
	else
		GetInstance()->Send(VoteEnd);
}

void CUSBHelperDlg::OnBnClickedButtonVoteClear()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i=0;i<m_list.size();i++)
	{
		CDrawDlg *pDlg = m_list[i];
		if (NULL != pDlg)
			pDlg->SetVote();
	}
}

void CUSBHelperDlg::OnBnClickedButton3Ms()
{
	// TODO: 在此添加控件通知处理程序代码
	GetInstance()->Send(WriteStart);
}

void CUSBHelperDlg::OnBnClickedButton3MsOff()
{
	// TODO: 在此添加控件通知处理程序代码
	GetInstance()->Send(WriteEnd);
}

void CUSBHelperDlg::OnBnClickedButtonStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	GetInstance()->Send(GetConfig);
}

void CUSBHelperDlg::resetUI()
{
	int left = 10;
	int top = 10;
	CRect rect;
	CListCtrl* pListView = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_USB_DEVICE));
	pListView->GetClientRect(rect);
	pListView->MoveWindow(left,top,rect.Width(),rect.Height());

	/*top += rect.Height() + left;
	CButton *pButton = static_cast<CButton*>(GetDlgItem(IDC_BUTTON3_OPEN));
	pButton->GetClientRect(rect);
	pButton->MoveWindow(left,top,rect.Width(),rect.Height());//*/

	CreateChart(rect.Width() + 2*left);
}

void CUSBHelperDlg::CreateChart(int nHStart)
{
	// TODO: 在此添加控件通知处理程序代码
	//int nHStart = 450;
	int nVStart = 10;
	int nItemSize = 200;
	int nSpaceMax = nItemSize + 10;
	int nRow = 0;
	int nColumn = 0;
	CRect rect;
	this->GetClientRect(&rect);
	int nMaxNum = GetPrivateProfileInt(_T("General"),_T("MaxNum"),60,GetAppPath() + _T("\\USBHelper.ini"));
	while(true)
	{
		nItemSize -= 5;
		if(nItemSize <= 0)
			break;
		nSpaceMax = nItemSize + 10;
		nRow = (rect.Width() - nHStart)/nSpaceMax;
		nColumn = (rect.Height() - nVStart)/nSpaceMax;
		if (nRow*nColumn > nMaxNum)
			break;
	}

	for (int j=0;j<nColumn;j++)	
	{
		for (int i=0;i<nRow;i++)
		{
			if (m_list.size() == nMaxNum)
				break;
			CDrawDlg *pDlg = new CDrawDlg;
			m_list.push_back(pDlg);
			CString str;
			str.Format(_T("%d"),m_list.size());
			pDlg->SetText(str);
			//pDlg->SetVote(str);
			pDlg->Create(IDD_DRAWDLG,this);
			pDlg->MoveWindow(nHStart + i*nSpaceMax,nVStart + j*nSpaceMax,nItemSize,nItemSize); 
			pDlg->ShowWindow(SW_SHOWNORMAL);
		}
	}
}

void CUSBHelperDlg::OnBnClickedButtonMsClear()
{
	// TODO: 在此添加控件通知处理程序代码

	for(int i=0;i<m_list.size();i++)
	{
		CDrawDlg *pDlg = m_list[i];
		if (NULL != pDlg)
		{
			pDlg->ResetUI(true);
		}
	}
}

void CUSBHelperDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	GetDlgItem(IDC_BUTTON3_OPEN)->GetWindowText(str);
	if (str == _T("关闭设备"))
		OnBnClickedButton3Open();
	m_bRun = FALSE;

	SetEvent(m_hEvent[1]);

	CDialogEx::OnClose();
}

void CUSBHelperDlg::OnBnClickedButton3Set()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strCustom;
	GetDlgItem(IDC_EDIT_CUSTOM)->GetWindowText(strCustom);
	CString strClass;
	GetDlgItem(IDC_EDIT_CLASS)->GetWindowText(strClass);
	CString strDevice;
	GetDlgItem(IDC_EDIT_DEV)->GetWindowText(strDevice);
	CSettingDlg dlg(strCustom,strClass,strDevice,m_bNode);
	if (IDOK == dlg.DoModal())
	{
		dlg.GetSettingInfo(strCustom,strClass,strDevice);

		int nCustrom = atoi(WideStrToMultiStr(strCustom.GetBuffer()));
		int nClass = atoi(WideStrToMultiStr(strClass.GetBuffer()));
		int nDevice = atoi(WideStrToMultiStr(strDevice.GetBuffer()));
		GetInstance()->SetConfig(nCustrom,nClass,nDevice);
	}
}

void CUSBHelperDlg::OnBnClickedButton3Update()
{
	// TODO: 在此添加控件通知处理程序代码
	if (NULL != m_pDlg)
	{
		if(m_pDlg->IsWindowVisible())
			return;
		CString str;
		GetDlgItem(IDC_STATIC_VERSION)->GetWindowText(str);
		m_pDlg->ResetUI();
		m_pDlg->SetVersion(str);
		m_pDlg->SetUpgradeType(m_bNode);
		m_pDlg->CenterWindow(this);
		m_pDlg->ShowWindow(SW_SHOWDEFAULT);
	}
}

LRESULT CUSBHelperDlg::OnUpdate(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case START_UPADTE_GATEWAY:
			GetInstance()->Update(WideStrToMultiStr(m_strFileMcu.GetBuffer()),"");
		break;
	case START_UPADTE_NODE:
			GetInstance()->Update(WideStrToMultiStr(m_strFileMcu.GetBuffer()),WideStrToMultiStr(m_strFileBle.GetBuffer()));
		break;
	case STOP_UPDATE_GATEWAY:
	case STOP_UPDATE_NODE:
		GetInstance()->Send(UpdateStop);
		break;
	case SET_VERSION:
		{
			BSTR b = (BSTR)wParam;
			CString str(b);
			m_version = CString2Version(str);
		}
		break;
	case SET_MCU:
		{
			BSTR b = (BSTR)wParam;
			CString str(b);
			m_strFileMcu = str;
		}
		break;
	case SET_BLE:
		{
			BSTR b = (BSTR)wParam;
			CString str(b);
			m_strFileBle = str;
		}
		break;
	default:
		break;
	}

	return 0;
}

LRESULT CUSBHelperDlg::OnUpdateWindow(WPARAM wParam, LPARAM lParam)
{
	switch(lParam)
	{
	case ROBOT_GATEWAY_REBOOT:
		{
			if (m_pDlg->IsWindowVisible())
			{
				m_pDlg->ShowWindow(SW_HIDE);
				if (wParam == 0)
					AfxMessageBox(_T("升级成功！"));
				else
					AfxMessageBox(_T("升级失败！"));
				resetDevice();
			}
		}
		break;
	case ROBOT_SET_DEVICE_NUM:
		{
			if (wParam == 0)
				AfxMessageBox(_T("设置成功！"));
			else
				AfxMessageBox(_T("设置失败！"));
			resetDevice();
		}
		break;
	case ROBOT_DEVICE_CHANGE:
		{
			if (wParam == 0)
				AddList();
			else
				resetDevice();
		}
		break;
	default:
		break;
	}

	return 0;
}

void CUSBHelperDlg::OnBnClickedButton3Show()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCount = GetPrivateProfileInt(_T("General"),_T("ShowNum"),60,GetAppPath() + _T("\\USBHelper.ini"));
	if(nCount > m_list.size())
		return;
	for(int i=0;i<nCount;i++)
	{
		CDrawDlg *pDlg = m_list[i];
		if (NULL != pDlg)
		{
			pDlg->ResetWindow();
		}
	}
}

ST_VERSION CUSBHelperDlg::CString2Version(CString strVersion)
{
	ST_VERSION version = {0};
	CStringArray sArray;
	SplitFields(strVersion,sArray,_T("."));

	if(sArray.GetCount() == 4)
	{
		version.version = atoi(WideStrToMultiStr(sArray[0].GetBuffer()));
		version.version2 = atoi(WideStrToMultiStr(sArray[1].GetBuffer()));
		version.version3 = atoi(WideStrToMultiStr(sArray[2].GetBuffer()));
		version.version4 = atoi(WideStrToMultiStr(sArray[3].GetBuffer()));
	}
	return version;
}

///////////////////////////////////////////////
BOOL CUSBHelperDlg::IsItemExist(CString strName)
{
	CListCtrl* pListView = static_cast<CListCtrl*>(GetDlgItem(IDC_LIST_USB_DEVICE));
	for(int i=0;i<pListView->GetItemCount();i++)
	{
		if (pListView->GetItemText(i,0) == strName)
			return TRUE;
	}
	return FALSE;
}

void CUSBHelperDlg::getUsbData(const unsigned char *pData,int len,void *pContext)
{
	CUSBHelperDlg *pDlg = (CUSBHelperDlg*)pContext;

	pDlg->setUsbData(pData);
}

void CUSBHelperDlg::setUsbData(const unsigned char *pData)
{
	ROBOT_REPORT report = {0};
	memcpy(&report,pData,sizeof(ROBOT_REPORT));

	::EnterCriticalSection(&m_sectionLock);
	m_queueData.push(report);
	::LeaveCriticalSection(&m_sectionLock);
	SetEvent(m_hEvent[0]);
}

void CUSBHelperDlg::parseRobotReport(const ROBOT_REPORT &report)
{
	switch(report.cmd_id)
	{
	case ROBOT_GATEWAY_STATUS://获取状态
		{
			ROBOT_STATUS status = {0};
			memcpy(&status,report.payload,sizeof(ROBOT_STATUS));

			if (m_nLastStatus == status.device_status)
			{
				break;
			}
			else
				m_nLastStatus = status.device_status;

			if (report.reserved == GATEWAY)
			{
				switch(status.device_status)
				{
				case NEBULA_STATUS_OFFLINE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("OFFLINE"));
					break;
				case NEBULA_STATUS_STANDBY:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("STANDBY"));
					break;
				case NEBULA_STATUS_VOTE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("VOTE"));
					break;
				case NEBULA_STATUS_MASSDATA:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("MASSDATA"));
					break;
				case NEBULA_STATUS_END:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("END"));
					break;
				default:
					{
						CString str;
						str.Format(_T("UNKNOW:%d"),status.device_status);
						GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(str);
					}
					break;
				}
			}
			else
			{
				switch(status.device_status)
				{
				case DEVICE_POWER_OFF:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_POWER_OFF"));
					break;
				case DEVICE_STANDBY:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_STANDBY"));
					break;
				case DEVICE_INIT_BTN:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_INIT_BTN"));
					break;
				case DEVICE_OFFLINE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_OFFLINE"));
					break;
				case DEVICE_ACTIVE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_ACTIVE"));
					break;
				case DEVICE_LOW_POWER_ACTIVE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_LOW_POWER_ACTIVE"));
					break;
				case DEVICE_OTA_MODE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_OTA_MODE"));
					break;
				case DEVICE_OTA_WAIT_SWITCH:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_OTA_WAIT_SWITCH"));
					break;
				case DEVICE_DFU_MODE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_DFU_MODE"));
					break;
				case DEVICE_TRYING_POWER_OFF:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_TRYING_POWER_OFF"));
					break;
				case DEVICE_FINISHED_PRODUCT_TEST:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_FINISHED_PRODUCT_TEST"));
					break;
				case DEVICE_SYNC_MODE:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_SYNC_MODE"));
					break;
				case DEVICE_SEMI_FINISHED_PRODUCT_TEST:
					GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DEVICE_SEMI_FINISHED_PRODUCT_TEST"));
					break;
				default:
					break;
				}
				CString str;
				str.Format(_T("离线笔记:%d条"),status.note_num);
				GetDlgItem(IDC_STATIC_NOTE)->SetWindowText(str);
			}
		
		}
		break;			
	case ROBOT_EXIT_VOTE://退出投票模式
		{
			for (int i=0;i<m_list.size();i++)
			{
				CString str;
				str.Format(_T("%d"),report.payload[i]);
				CDrawDlg *pDlg = m_list[i];
				if (NULL != pDlg)
					pDlg->SetVote(str);
			}
		}
		break;			
	case ROBOT_BIG_DATA_REPORT://大数据上报
		{
			PEN_INFO penInfo = {0};
			memcpy(&penInfo,report.payload,sizeof(PEN_INFO));
			CDrawDlg *pDlg = m_list[report.reserved];
			if (pDlg)
				pDlg->AddData(penInfo);
		}
		break;			
	case ROBOT_GATEWAY_ERROR://错误
		{
			switch(report.payload[0])
			{
			case ERROR_NONE:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_NONE"));
				break;
			case ERROR_FLOW_NUM:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_FLOW_NUM"));
				break;
			case ERROR_FW_LEN:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_FW_LEN"));
				break;
			case ERROR_FW_CHECKSUM:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_FW_CHECKSUM"));
				break;
			case ERROR_STATUS:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_STATUS"));
				break;
			case ERROR_VERSION:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_VERSION"));
				break;
			case ERROR_NAME_CONTENT:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_NAME_CONTENT"));
				break;
			case ERROR_NO_NOTE:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("ERROR_NO_NOTE"));
				break;
			default:
				break;
			}
		}
		break;			
	case ROBOT_SET_DEVICE_NUM://设置设备网络号
		{
			this->PostMessage(WM_UPDATE_WINDOW,report.payload[0],report.cmd_id);
			if (report.payload[0] == 0)
			{
				CString str;
				str.Format(_T("%d"),report.payload[1]);
				GetDlgItem(IDC_EDIT_CUSTOM)->SetWindowText(str);
				str.Format(_T("%d"),report.payload[2]);
				GetDlgItem(IDC_EDIT_CLASS)->SetWindowText(str);
				str.Format(_T("%d"),report.payload[3]);
				GetDlgItem(IDC_EDIT_DEV)->SetWindowText(str);
			}
		}
		break;			
	case ROBOT_FIRMWARE_DATA://进度
			m_pDlg->PostMessage(WM_PROCESS,report.payload[0],report.cmd_id);
		break;			
	case ROBOT_RAW_RESULT://校验结果
			this->PostMessage(WM_UPDATE_WINDOW,report.payload[0],report.cmd_id);
		break;			
	case ROBOT_GATEWAY_REBOOT:	//设备重启	
		this->PostMessage(WM_UPDATE_WINDOW,report.payload[0],report.cmd_id);
		break;			
	case ROBOT_GATEWAY_VERSION://设备版本号
		{
			ST_DEVICE_INFO info = {0};
			memcpy(&info,report.payload,sizeof(ST_DEVICE_INFO));

			if (m_lastInfo.custom_num == info.custom_num
				&& m_lastInfo.class_num == info.class_num
				&& m_lastInfo.device_num == info.device_num
				&& m_lastInfo.version.version == info.version.version
				&& m_lastInfo.version.version2 == info.version.version2
				&& m_lastInfo.version.version3 == info.version.version3
				&& m_lastInfo.version.version4 == info.version.version4)
			{
				break;
			}
			else
				memcpy(&m_lastInfo,&info,sizeof(m_lastInfo));

			CString str;
			str.Format(_T("%d.%d.%d.%d"),info.version.version4,info.version.version3,info.version.version2,info.version.version);
			GetDlgItem(IDC_STATIC_VERSION)->SetWindowText(str);

			str.Format(_T("%d"),info.custom_num);
			GetDlgItem(IDC_EDIT_CUSTOM)->SetWindowText(str);
			str.Format(_T("%d"),info.class_num);
			GetDlgItem(IDC_EDIT_CLASS)->SetWindowText(str);
			str.Format(_T("%d"),info.device_num);
			GetDlgItem(IDC_EDIT_DEV)->SetWindowText(str);
		}
		break;			
	case ROBOT_ONLINE_STATUS://在线状态
		{
			for (int i=0;i<m_list.size();i++)
			{
				CDrawDlg *pDlg = m_list[i];
				if (NULL != pDlg)
					pDlg->SetOnLine(report.payload[i]);
			}
		}
		break;				
	case ROBOT_DEVICE_CHANGE://设备改变
		this->PostMessage(WM_UPDATE_WINDOW,report.payload[0],report.cmd_id);
		break;	
	case ROBOT_NODE_MODE:
		{
			int nStatus = report.payload[0];
			if (m_nLastMode == nStatus)
				break;
			else
				m_nLastMode = nStatus;
			CString str;
			switch(nStatus)
			{
			case 0:
				{
					str = _T("BLE");
					GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_HIDE);
				}
				break;
			case 1:
				{
					str = _T("2.4G");
					GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_HIDE);
				}
				break;
			case 2:
				{
					str = _T("USB");
					GetDlgItem(IDC_STATIC_NOTE)->ShowWindow(SW_SHOW);
				}
				break;
			default:
				str = _T("Unknow");
				break;
			}
			GetDlgItem(IDC_STATIC_MODE)->SetWindowText(str);
		}
		break;
	case ROBOT_USB_PACKET://USB坐标
		{
			PEN_INFO penInfo = {0};
			memcpy(&penInfo,report.payload,sizeof(PEN_INFO));

			penInfo.nPress = (penInfo.nStatus == 0x11) ? 1 : 0;
			
			//TRACE(_T("X:%d-Y:%d-Press:%d\n"),penInfo.nX,penInfo.nY,penInfo.nPress);

			m_list[0]->AddData(penInfo);
		}
		break;
	case ROBOT_KEY_PRESS://按键按下
		{
			int nStatus = report.payload[0];
			switch(nStatus)
			{
			case CLICK:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("CLICK"));
				break;
			case DBCLICK:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("DBCLICK"));
				break;
			case PAGEUP:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("PAGEUP"));
				break;
			case PAGEDOWN:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("PAGEDOWN"));
				break;
			case CREATEPAGE:
				GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(_T("CREATEPAGE"));
				break;
			default:
				break;
			}
		}
		break;
	case ROBOT_SHOW_PAGE://显示页码	
		{
			int nCurrentPage = report.payload[0];
			int nPageCount = report.payload[1];
			CString str;
			str.Format(_T("第%d页,共%d页"),nCurrentPage,nPageCount);
			GetDlgItem(IDC_STATIC_SCANTIP)->SetWindowText(str);
		}
		break;
	case ROBOT_SYNC_TRANS_BEGIN:
		{
			GetDlgItem(IDC_STATIC_NOTE)->SetWindowText(_T("ROBOT_SYNC_TRANS_BEGIN"));
		}
		break;
	case ROBOT_ORIGINAL_PACKET:
		{
			PEN_INFO penInfo = {0};
			memcpy(&penInfo,report.payload,sizeof(PEN_INFO));

			//penInfo.nPress = (penInfo.nStatus == 0x11) ? 1 : 0;

			//TRACE(_T("X:%d-Y:%d-Status:%d\n"),penInfo.nX,penInfo.nY,penInfo.nStatus);

			m_list[0]->AddData(penInfo);
		}
		break;
	case ROBOT_SYNC_TRANS_END:
		{
			GetDlgItem(IDC_STATIC_NOTE)->SetWindowText(_T("ROBOT_SYNC_TRANS_END"));
		}
		break;
	default:						
		break;
	}
}