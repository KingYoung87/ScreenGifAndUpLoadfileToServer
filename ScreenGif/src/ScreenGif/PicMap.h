#pragma once


// CPicMap �Ի���
#include "PicProcess.h"
class CPicMap : public CDialogEx
{
	DECLARE_DYNAMIC(CPicMap)

public:
	CPicMap(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicMap();

// �Ի�������
	enum { IDD = IDD_DIALOG_MAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

public:
	list<CRect> m_vecRect;		//�����ͼ����
	CRect m_curRect;			//��ǰһ�λ�ͼ
	CDC m_SrcMemDC;				//��ͼ�����ڴ�dc
	CPicProcess* m_pProcess;	//ǰ���ָ��
	
};
