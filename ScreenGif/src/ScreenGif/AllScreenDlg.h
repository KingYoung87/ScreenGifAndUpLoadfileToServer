#pragma once
#include "CustomEdit.h"

// CAllScreenDlg �Ի���

class CAllScreenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAllScreenDlg)

public:
	CAllScreenDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAllScreenDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ALLSCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����ƶ�ʱ������
	//afx_msg LRESULT OmMouseMove(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	RECT m_rc;			//��Ļ�ߴ�
	RECT m_rcSelect;	//��ѡ�ߴ�
	POINT m_beginpoint;	//��ʼ��
	POINT m_endpoint;	//������
	bool m_bstart;		//���ڻ�����	
	CCustomEdit* m_cEdit;
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
