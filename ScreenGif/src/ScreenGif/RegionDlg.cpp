// RegionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "RegionDlg.h"
#include "afxdialogex.h"


// CRegionDlg �Ի���

IMPLEMENT_DYNAMIC(CRegionDlg, CDialogEx)

CRegionDlg::CRegionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegionDlg::IDD, pParent)
{
	m_brush.CreateSolidBrush(RGB(255, 0, 0));
}

CRegionDlg::~CRegionDlg()
{
}

void CRegionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRegionDlg, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CRegionDlg ��Ϣ�������


BOOL CRegionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	CRgn rgn, tmprgn;
	rgn.CreateRectRgn(0, 0, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top);
	tmprgn.CreateRectRgn(2, 2, m_rect.right - m_rect.left - 2, m_rect.bottom - m_rect.top - 2);
	rgn.CombineRgn(&rgn, &tmprgn, RGN_XOR);
	SetWindowRgn(rgn, TRUE);
	SetWindowPos(&wndTopMost, m_rect.left, m_rect.top,m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, SWP_SHOWWINDOW);


	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CRegionDlg::PostNcDestroy()
{
	// TODO:  �ڴ����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


HBRUSH CRegionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	return m_brush;
}
