#pragma once

// CPicProcess �Ի���

//�ڻ�ͼ����CPicProcess����ϣ�ͼƬ����ʾʵ��CPicMap������ʾ

class CPicMap;
class CCustomEdit;
class CPicProcess : public CDialog
{
	DECLARE_DYNAMIC(CPicProcess)

public:
	CPicProcess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPicProcess();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROCESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void DrawShape(CDC* dc, CPoint pointb, CPoint pointe);	//����ͼ��


public:
	RECT m_rc;						//���ڴ�С
	CRect m_rect;					//����������
	list<CRect> m_vecRect;			//ͼ������
	bool m_bDrawStart;				//�Ƿ�ʼ��ͼ
	CDC m_SrcMemDC;					//ԭʼDC
	CPicMap* m_map;					//����
	CCustomEdit* m_pEdit;			//���ֹ���
	int m_iTool;					//��ͼ���� 1�����ι��ߣ�2����Բ���ߣ�3����ͷ��4�����֣�5������
	CPoint m_pointEnd;				//�յ�
	CPoint m_pointBegin;			//���
	
};
