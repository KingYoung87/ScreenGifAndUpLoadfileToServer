
// ScreenGifDlg.h : ͷ�ļ�
//

#pragma once
#include "GlobalData.h"
#include "RegionDlg.h"
#include "AllScreenDlg.h"
#include "afxcmn.h"
#include "Gif.h"
#include "afxwin.h"
class CPicProcess;
class CPicMap;
class CScreenGifDlg : public CDialogEx
{
// ����
public:
	CScreenGifDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CScreenGifDlg();

// �Ի�������
	enum { IDD = IDD_SCREENGIF_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// ���ι���
	afx_msg void OnBnRectangle();
	// ¼��GifͼƬ��ť
	afx_msg void OnBnClickedStart();
	// ����ť
	afx_msg void OnBnClickedShare();
	// ��ɰ�ť
	afx_msg void OnBnClickedGetPic();
	// ���水ť
	afx_msg void OnBnClickedSave();
	// ��ťtootip
	afx_msg BOOL OnToolTipText(UINT NID, NMHDR* pNMHDR, LRESULT* pResult);
	// ȫ�ֿ�ݼ�
	afx_msg long OnHotKey(WPARAM wParam, LPARAM lParam);
	// ����
	afx_msg void OnHelp();

public:
	// �������
	afx_msg LRESULT OnBeginPoint(WPARAM wParam, LPARAM lParam);
	// �����յ�
	afx_msg LRESULT OnEndPoint(WPARAM wParam, LPARAM lParam);
	// ����ƶ�ʱ������
	afx_msg LRESULT OnMovePoint(WPARAM wParam, LPARAM lParam);
	// ��ͼ��ݼ���Ϣ
	afx_msg LRESULT OnScreenPic(WPARAM wParam, LPARAM lParam);
	// ��ʱ��
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// �ı�Ի��򱳾���ɫ
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ����ͼ��֪ͨ��Ϣ
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam, LPARAM lparam);
	// ȡ����ť
	afx_msg void OnBnClickedCancel();
	// ��Բ����
	afx_msg void OnBnClickedButtonCircle();
	// ��ͷ����
	afx_msg void OnBnClickedButtonArrow();
	// ���ֹ���
	afx_msg void OnBnClickedButtonA();
	// ����
	afx_msg void OnBnClickedButtonBack();


public:
	// ��ͼ
	bool GetPic();
	// ����bitmapͼƬ
	void SaveBitmapToFile(CBitmap* pBitmap, CString fileName);
	// ����gifͼƬ
	void MakeGif();
	// ��ӹ�����
	void AddToolbar();
	// ָ������
	void GetRect();
	//�ϴ�������
	void UploadFile();
	//����ˮӡ
	void MakeWatermark(string &_path, string &_fileName);

	afx_msg LRESULT ProgressAdd(WPARAM wParam, LPARAM lParam);

public:
	RECT m_rc;						//��Ļ��ͼ����
	RECT m_AllScreen;				//ȫ���ߴ�
	CRegionDlg* m_pRegionDlg;		//ѡȡ����
	CAllScreenDlg* m_pAllScreenDlg; //��Ļ�Ի���
	CDC m_srcDc;					//��ĻDC
	CDC m_dcCompatible;				//����DC
	CBitmap m_mapCompatible;		//����λͼ
	int m_index;					//bmpͼ���
	bool m_bIsReadyGif;				//�Ƿ����¼��Gif
	WORD m_wGifbeginSize;			//Gif�ļ���ʼ��ͼ�ο�����չ��Ĵ�С
	WORD m_wGifDataSize;			//Gif�ļ�ͼ�����ݲ��ֵĴ�С
	CString m_strPath;				//pic�ļ�·��
	CString m_strCurentGif;			//��ǰ������gif
	CBrush m_brush;					//�Ի��򱳾���ɫ
	CToolBar m_toolbar;				//������
	CImageList m_toolbarlist;		//ͼ������
	NOTIFYICONDATA m_notify;		//�������ݽṹ��
	CToolTipCtrl m_tooltip;			//������tooltip
	CProgressCtrl m_Progress;		//������
	CPicProcess* m_process;			//��ͼ������
	CPicMap* m_map;					//����
	PEN m_pen;						//��������
	CBrush m_rcbrush;				//��ˢ�����ƾ���ʱ�ã�
	CEdit m_edtGifAddress;

	int m_iFrameNum;				//��ȡ֡��
	bool m_blConformSize;			//�Ƿ�̶���С
	BOOL m_blEnableStart;			//�Ƿ���Կ�ʼ
	int m_iProgressCount;
	int m_iDstWidth;				//�ü���Ŀ��
	int m_iDstHeight;				//�ü���ĸ߶�
	double m_fSizeNumber;				//����ֵ
	CComboBox m_cboSize;
	afx_msg void OnCbnSelchangeCombo1();
	CStatic m_stcFrame;
	CButton m_chkConformSize;
	afx_msg void OnBnClickedCheck1();
	CStatic m_stcArea;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
