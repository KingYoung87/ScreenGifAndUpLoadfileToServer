#pragma once

// CSgifAbout �Ի���

class CSgifAbout : public CDialogEx
{
	DECLARE_DYNAMIC(CSgifAbout)

public:
	CSgifAbout(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSgifAbout();

// �Ի�������
	enum { IDD = IDD_DIALOG_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnNmClickgithub(NMHDR* pNMHDR, LRESULT* pResult);
	void OnNmClickoschina(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();

};
