// CustomEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "CustomEdit.h"


// CCustomEdit

IMPLEMENT_DYNAMIC(CCustomEdit, CEdit)

CCustomEdit::CCustomEdit()
{

}

CCustomEdit::~CCustomEdit()
{
}


BEGIN_MESSAGE_MAP(CCustomEdit, CEdit)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CCustomEdit ��Ϣ�������




HBRUSH CCustomEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CEdit::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//�޸ı���͸��
	//pDC->SetBkMode(TRANSPARENT);

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
