// SgifAbout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "SgifAbout.h"
#include "afxdialogex.h"


// CSgifAbout �Ի���

IMPLEMENT_DYNAMIC(CSgifAbout, CDialogEx)

CSgifAbout::CSgifAbout(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSgifAbout::IDD, pParent)
{

}

CSgifAbout::~CSgifAbout()
{
}

void CSgifAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSgifAbout, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_GITHUB, OnNmClickgithub)
	ON_NOTIFY(NM_CLICK, IDC_SYSLINK_OSCHINA, OnNmClickoschina)
END_MESSAGE_MAP()


BOOL CSgifAbout::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//((CLinkCtrl*)(GetDlgItem(IDC_SYSLINK_GITHUB)))->SetItemUrl(0, _T("https://github.com/wenii/ScreenGif.git"));
	//((CLinkCtrl*)(GetDlgItem(IDC_SYSLINK_OSCHINA)))->SetItemUrl(0, _T("https://git.oschina.net/wentx/ScreenGif.git"));
	SetDlgItemText(IDC_STATIC_VERSION1, _T("�汾 2.2.0.1"));
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

// CSgifAbout ��Ϣ�������
void CSgifAbout::OnNmClickgithub(NMHDR* pNMHDR, LRESULT* pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	//ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
}
void CSgifAbout::OnNmClickoschina(NMHDR* pNMHDR, LRESULT* pResult)
{
	PNMLINK pNMLink = (PNMLINK)pNMHDR;
	//ShellExecute(NULL, _T("open"), pNMLink->item.szUrl, NULL, NULL, SW_SHOWNORMAL);
}


