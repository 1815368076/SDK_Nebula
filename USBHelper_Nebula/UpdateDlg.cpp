// UpdateDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "USBHelper.h"
#include "UpdateDlg.h"
#include "afxdialogex.h"


// CUpdateDlg 对话框

IMPLEMENT_DYNAMIC(CUpdateDlg, CDialog)

CUpdateDlg::CUpdateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUpdateDlg::IDD, pParent)
	, m_bNode(FALSE)
{

}

CUpdateDlg::~CUpdateDlg()
{
}

void CUpdateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON4_UPDATE, &CUpdateDlg::OnBnClickedButton4Update)
	ON_BN_CLICKED(IDC_BUTTON_BROWER, &CUpdateDlg::OnBnClickedButtonBrower)
	ON_BN_CLICKED(IDC_BUTTON_BROWER2, &CUpdateDlg::OnBnClickedButtonBrower2)
	ON_WM_NCDESTROY()
	ON_MESSAGE(WM_PROCESS, OnProcess)
	ON_BN_CLICKED(IDC_BUTTON4_STOP, &CUpdateDlg::OnBnClickedButton4Stop)
END_MESSAGE_MAP()


// CUpdateDlg 消息处理程序


void CUpdateDlg::OnBnClickedButton4Update()
{
	// TODO: 在此添加控件通知处理程序代码

	ST_VERSION versionDev,versionFile;
	CString str;
	GetDlgItem(IDC_EDIT_VERSION)->GetWindowText(str);
	versionDev = CString2Version(str);
	GetDlgItem(IDC_EDIT_VERSION2)->GetWindowText(str);
	versionFile = CString2Version(str);
	this->GetParent()->SendMessage(WM_UPDATE,(WPARAM)str.GetBuffer(),SET_VERSION);
	if (IsNeedUpdate(versionFile,versionDev))
	{
		((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(0);
		GetDlgItem(IDC_EDIT_MCU)->GetWindowText(str);
		this->GetParent()->SendMessage(WM_UPDATE,(WPARAM)str.GetBuffer(),SET_MCU);
		GetDlgItem(IDC_EDIT_BT)->GetWindowText(str);
		this->GetParent()->SendMessage(WM_UPDATE,(WPARAM)str.GetBuffer(),SET_BLE);

		if(m_bNode)
			this->GetParent()->SendMessage(WM_UPDATE,NULL,START_UPADTE_NODE);
		else
			this->GetParent()->SendMessage(WM_UPDATE,NULL,START_UPADTE_GATEWAY);
	}
}

void CUpdateDlg::OnBnClickedButtonBrower()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = _T("Bin Files(*.bin)|*.bin|All Files(*.*)|*.*||");  
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter,NULL);  

	if(dlg.DoModal() == IDOK)  
	{
		GetDlgItem(IDC_EDIT_MCU)->SetWindowText(dlg.GetPathName());

		CStringArray sArray;
		CString strFileName = dlg.GetFileName();
		SplitFields(strFileName.Left(strFileName.GetLength()-4),sArray,_T("_"));

		if(sArray.GetCount() == 4)
		{
			m_version.version = atoi(WideCharToMultichar(sArray[2].GetBuffer()).c_str());
			m_version.version2 = atoi(WideCharToMultichar(sArray[3].GetBuffer()).c_str());

			CString str;
			str.Format(_T("%d.%d.%d.%d"),m_version.version,m_version.version2,m_version.version3,m_version.version4);
			GetDlgItem(IDC_EDIT_VERSION2)->SetWindowText(str);
		}
		else if(sArray.GetCount() == 6)
		{
			CString str;
			str.Format(_T("%s.%s.%s.%s"),sArray[2],sArray[3],sArray[4],sArray[5]);
			GetDlgItem(IDC_EDIT_VERSION2)->SetWindowText(str);
		}
		else
		{
			AfxMessageBox(_T("文件格式不匹配！"));
		}
	}  
}

void CUpdateDlg::OnBnClickedButtonBrower2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strFilter = _T("Bin Files(*.bin)|*.bin|All Files(*.*)|*.*||");  
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strFilter,NULL);  

	if(dlg.DoModal() == IDOK)  
	{  
		GetDlgItem(IDC_EDIT_BT)->SetWindowText(dlg.GetPathName());

		CStringArray sArray;
		CString strFileName = dlg.GetFileName();
		SplitFields(strFileName.Left(strFileName.GetLength()-4),sArray,_T("_"));

		if(sArray.GetCount() == 4)
		{
			m_version.version3 = atoi(WideCharToMultichar(sArray[2].GetBuffer()).c_str());
			m_version.version4 = atoi(WideCharToMultichar(sArray[3].GetBuffer()).c_str());

			CString str;
			str.Format(_T("%d.%d.%d.%d"),m_version.version,m_version.version2,m_version.version3,m_version.version4);
			GetDlgItem(IDC_EDIT_VERSION2)->SetWindowText(str);
		}
		else
		{
			AfxMessageBox(_T("文件格式不匹配！"));
		}
	}  
}

void CUpdateDlg::SetVersion(const CString &strVersion)
{
	GetDlgItem(IDC_EDIT_VERSION)->SetWindowText(strVersion);
	m_version = CString2Version(strVersion);
}

void CUpdateDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: 在此处添加消息处理程序代码
	delete this;
}

LRESULT CUpdateDlg::OnProcess(WPARAM wParam, LPARAM lParam)
{
	CString str(_T(""));

	switch(lParam)
	{
	case ROBOT_FIRMWARE_DATA:
		{
			((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(wParam);
			str.Format(_T("进度：%d%%"),wParam);
		}
		break;
	default:
		break;
	}
	GetDlgItem(IDC_STATIC_UPDATE_INFO)->SetWindowText(str);

	return 0;
}

BOOL CUpdateDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetRange(0,100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CUpdateDlg::OnBnClickedButton4Stop()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bNode)
		this->GetParent()->SendMessage(WM_UPDATE,NULL,STOP_UPDATE_NODE);
	else
		this->GetParent()->SendMessage(WM_UPDATE,NULL,STOP_UPDATE_GATEWAY);
}

PCHAR CUpdateDlg::WideStrToMultiStr (PWCHAR WideStr)
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

ST_VERSION CUpdateDlg::CString2Version(CString strVersion)
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

bool CUpdateDlg::IsNeedUpdate(const ST_VERSION &versionWeb,const ST_VERSION &versionDev)
{
	if( versionWeb.version*10 + versionWeb.version2 > versionDev.version*10 + versionDev.version2)
		return true;
	if( versionWeb.version*10 + versionWeb.version2 == versionDev.version*10 + versionDev.version2)
	{
		if( versionWeb.version3*10 + versionWeb.version4 > versionDev.version3*10 + versionDev.version4)
			return true;
	}

	return false;
}

void CUpdateDlg::SetUpgradeType(BOOL bNode /*=FALSE*/)
{
	m_bNode = bNode;
	GetDlgItem(IDC_STATIC_BLE)->ShowWindow(bNode);
	GetDlgItem(IDC_EDIT_BT)->ShowWindow(bNode);
	GetDlgItem(IDC_BUTTON_BROWER2)->ShowWindow(bNode);
}

void CUpdateDlg::ResetUI()
{
	GetDlgItem(IDC_EDIT_VERSION)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_VERSION2)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_MCU)->SetWindowText(_T(""));
	GetDlgItem(IDC_EDIT_BT)->SetWindowText(_T(""));
	GetDlgItem(IDC_STATIC_UPDATE_INFO)->SetWindowText(_T(""));
	((CProgressCtrl*)GetDlgItem(IDC_PROGRESS1))->SetPos(0);

	memset(&m_version,0,sizeof(m_version));
}