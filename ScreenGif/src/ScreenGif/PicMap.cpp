// PicMap.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "PicMap.h"
#include "afxdialogex.h"
#include "Gif.h"
#include "ScreenGifDlg.h"

// CPicMap �Ի���

IMPLEMENT_DYNAMIC(CPicMap, CDialogEx)

CPicMap::CPicMap(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPicMap::IDD, pParent)
{

}

CPicMap::~CPicMap()
{
}

void CPicMap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPicMap, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CPicMap ��Ϣ�������


void CPicMap::OnPaint()
{
	CClientDC dc(this);
	CDC m_CurMemDC;
	
	if (m_CurMemDC.m_hDC == NULL)		//��������DC
	{
		m_CurMemDC.CreateCompatibleDC(&dc);
	}

	PEN penstruct = ((CScreenGifDlg*)GetParent())->m_pen;
	//CBrush brush = ((CScreenGifDlg*)GetParent())->m_rcbrush;	//֧�ֻ�ʵ�ľ���
	CPen pen(penstruct.m_style, penstruct.m_width, penstruct.m_color);
	CPen* pOldPen = m_CurMemDC.SelectObject(&pen);
	CBrush* pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* oldBrush = m_CurMemDC.SelectObject(pBrush);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(&dc, m_curRect.Width(), m_curRect.Height());	//��������λͼ
	m_CurMemDC.SelectObject(&bmp);
	m_CurMemDC.BitBlt(0, 0, m_curRect.Width(), m_curRect.Height(), &m_SrcMemDC, 0, 0, SRCCOPY); //����ԭͼ����ǰ�ڴ�dc

	//��ǰ�ڴ�dc�л�ͼ
	list<CRect>::iterator it;
	for (it = (m_pProcess->m_vecRect).begin(); it != (m_pProcess->m_vecRect).end(); it++)
	{
		if (m_pProcess->m_iTool == 1)		//����
		{
			m_CurMemDC.Rectangle(&(*it));
		}
		else if (m_pProcess->m_iTool == 2)	//��Բ
		{
			m_CurMemDC.Ellipse(&(*it));
		}
	}
	//��ͼ�λ��ڻ�����
	dc.BitBlt(0, 0, m_curRect.Width(), m_curRect.Height(), &m_CurMemDC, 0, 0, SRCCOPY);
	m_CurMemDC.DeleteDC();
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


BOOL CPicMap::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	//return CDialogEx::OnEraseBkgnd(pDC);
}
