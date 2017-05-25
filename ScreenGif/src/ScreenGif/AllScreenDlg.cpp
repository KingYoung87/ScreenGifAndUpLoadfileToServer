// AllScreenDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "AllScreenDlg.h"
#include "afxdialogex.h"



// CAllScreenDlg �Ի���

IMPLEMENT_DYNAMIC(CAllScreenDlg, CDialogEx)

CAllScreenDlg::CAllScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAllScreenDlg::IDD, pParent)
	, m_bstart(false)
{
	m_rcSelect.left = 0;
	m_rcSelect.top = 0;
	m_rcSelect.right = 0;
	m_rcSelect.bottom = 0;
}

CAllScreenDlg::~CAllScreenDlg()
{
}

void CAllScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAllScreenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BOOL CAllScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 100, LWA_ALPHA);
	SetWindowPos(&wndTop, m_rc.left, m_rc.top,m_rc.right, m_rc.bottom, SWP_SHOWWINDOW);
	//�޸��������״
	SetClassLong(GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


void CAllScreenDlg::PostNcDestroy()
{
	// TODO:  �ڴ����ר�ô����/����û���
	delete this;
	CDialogEx::PostNcDestroy();
}


void CAllScreenDlg::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);//��ӡ����Ļ��  
	CPen pen;
	pen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	CPen* pOldPen = dc.SelectObject(&pen);
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* pOlBrush = dc.SelectObject(pBrush);
	if (m_rcSelect.right > m_rcSelect.left && m_rcSelect.bottom > m_rcSelect.top)
	{
		dc.Rectangle(&m_rcSelect);

	}
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOlBrush);
	dc.DeleteDC();
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CAllScreenDlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	//SetClassLong(m_hWnd, GCLP_HCURSOR, (LONG)LoadCursor(NULL, IDC_CROSS));
	m_bstart = true;
	m_rcSelect.left = 0;
	m_rcSelect.top = 0;
	m_rcSelect.right = 0;
	m_rcSelect.bottom = 0;
	HWND hpWnd = GetParent()->m_hWnd;
	m_beginpoint = point;
	::SendMessage(hpWnd, UM_BEGINPOINT, 0, (LPARAM)(&m_beginpoint));
	m_rcSelect.left = point.x;
	m_rcSelect.top = point.y;

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CAllScreenDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bstart = false;
	HWND hpWnd = GetParent()->m_hWnd;
	point.y = m_rcSelect.top + (m_rcSelect.right - m_rcSelect.left) * 0.625;
	m_endpoint = point;
	::SendMessage(hpWnd, UM_ENDPOINT, 0, (LPARAM)(&m_endpoint));
	//m_rcSelect.right = point.x;
	//m_rcSelect.bottom = point.y;
	Invalidate();

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CAllScreenDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bstart)
	{
		m_rcSelect.right = point.x;
		point.y = m_rcSelect.top + (m_rcSelect.right - m_rcSelect.left) * 0.625;
		m_rcSelect.bottom = point.y;
		HWND hpWnd = GetParent()->m_hWnd;
		::SendMessage(hpWnd, UM_MOUSEMOVE, 0, (LPARAM)(&point));
		Invalidate();
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CAllScreenDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDialogEx::OnTimer(nIDEvent);
}
