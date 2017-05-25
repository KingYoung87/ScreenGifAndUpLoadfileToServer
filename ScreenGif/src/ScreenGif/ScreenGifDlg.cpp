
// ScreenGifDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ScreenGif.h"
#include "ScreenGifDlg.h"
#include "afxdialogex.h"
#include "SgifAbout.h"
#include "PicProcess.h"
#include "PicMap.h"
#include "GSKCreateGIF.h"
#include "../curl/curl.h"
#include "../json/json.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include<time.h>
#include <atlconv.h>
#include <TlHelp32.h>
using namespace std;

#define  WM_PROGRESS_ADD	WM_USER+200 //������
#define   WIDTHBYTES(bits) (((bits)+31)/32*4)//����ʹͼ������ռ�ֽ���Ϊ4byte�ı��� 
typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;
typedef long LONG;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

string& replace_all(string& str, const string& old_value, const string& new_value)
{
	while (true)
	{
		int pos = 0;
		if ((pos = str.find(old_value, 0)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else break;
	}
	return str;
}

void CloseProgram(CString strProgram)
{
	HANDLE handle; //����CreateToolhelp32Snapshotϵͳ���վ��   
	HANDLE handle1; //����Ҫ�������̾��   
	handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);//���ϵͳ���վ��   
	PROCESSENTRY32 *info; //����PROCESSENTRY32�ṹ��ָ   
						  //PROCESSENTRY32 �ṹ�� dwSize ��Ա���ó� sizeof(PROCESSENTRY32)   

	info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);
	//����һ�� Process32First �������ӿ����л�ȡ�����б�   
	Process32First(handle, info);
	//�ظ����� Process32Next��ֱ���������� FALSE Ϊֹ   
	while (Process32Next(handle, info) != FALSE)
	{
		CString strTmp = info->szExeFile;     //ָ���������     
											  //strcmp�ַ����ȽϺ���ͬҪ������ͬ     
											  //if(strcmp(c, info->szExeFile) == 0 )     
		if (strProgram.CompareNoCase(info->szExeFile) == 0)
		{
			//PROCESS_TERMINATE��ʾΪ����������,FALSE=�ɼ̳�,info->th32ProcessID=����ID      
			handle1 = OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID);
			//��������      
			TerminateProcess(handle1, 0);
		}
	}
	delete info;

	CloseHandle(handle);
}

void ResizeBmpFile(CString cstrSrcFile, int iSrcWidth, int iSrcHeight, CString cstrDstFile, int iDstWidth, int iDstHeight)
{
	//BITMAPFILEHEADER bitHead, writebitHead;
	//BITMAPINFOHEADER bitInfoHead, writebitInfoHead;
	//FILE* pfile;//�����ļ�    
	//FILE* wfile;//����ļ�
	USES_CONVERSION;
	char *strFile = T2A(cstrSrcFile);
	char *strFilesave = T2A(cstrDstFile);

	BITMAPFILEHEADER bmfHeader;
	BITMAPINFOHEADER bmiHeader;

	FILE *pFile;
	if ((pFile = fopen(strFile, "rb")) == NULL){
		return;
	}
	//��ȡ�ļ���Bitmapͷ��Ϣ
	fseek(pFile, 0, SEEK_SET);
	fread(&bmfHeader, sizeof(BITMAPFILEHEADER), 1, pFile);
	fread(&bmiHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
	//�Ȳ�֧��С��16λ��λͼ
	int bitCount = bmiHeader.biBitCount;
	if (bitCount < 16){
		return;
	}
	int srcW = bmiHeader.biWidth;
	int srcH = bmiHeader.biHeight;

	int lineSize = bitCount * srcW / 8;
	//int l_width = WIDTHBYTES(srcW*bitCount);
	//ƫ������windowsϵͳҪ��ÿ��ɨ���а����ֽڶ���
	int alignBytes = ((bmiHeader.biWidth * bitCount + 31) & ~31) / 8L
	- bmiHeader.biWidth * bitCount / 8L;
	//ԭͼ�񻺴�
	//int srcBufSize = lineSize * srcH;
	int srcBufSize = ((iSrcWidth * bitCount + 31) / 32) * 4 * iSrcHeight;
	BYTE* srcBuf = new BYTE[srcBufSize];
	//��ȡ�ļ�������
	for (int i = 0; i < srcH; i++)
	{
		fread(&srcBuf[lineSize * i], lineSize, 1, pFile);
		fseek(pFile, alignBytes, SEEK_CUR);
	}

	//Ŀ��ͼ�񻺴�
	BYTE *pSrc, *pDest;
	int desBufSize = ((iDstWidth * bitCount + 31) / 32) * 4 * iDstHeight;
	int desLineSize = ((iDstWidth * bitCount + 31) / 32) * 4;
	BYTE *desBuf = new BYTE[desBufSize];
	float rateH = (float)iSrcHeight / iDstHeight;
	float rateW = (float)iSrcWidth / iDstWidth;
	//���ٽ���ֵ�㷨
	//for (int i = 0; i < iDstHeight; i++)
	//{
	//	//ѡȡ���ڽ��ĵ�
	//	int tSrcH = (int)(rateH * i + 0.5);
	//	for (int j = 0; j < iDstWidth; j++)
	//	{
	//		int tSrcW = (int)(rateW * j + 0.5);
	//		memcpy(&desBuf[i * desLineSize] + j * bmiHeader.biBitCount / 8, &srcBuf[tSrcH * lineSize] + tSrcW * bmiHeader.biBitCount / 8, bmiHeader.biBitCount / 8);
	//	}
	//}

	//˫�����ڲ�ֵ�㷨
	for (int i = 0; i < iDstHeight; i++)
	{
		int tH = (int)(rateH * i);
		int tH1 = min(tH + 1, srcH - 1);
		float u = (float)(rateH * i - tH);
		for (int j = 0; j < iDstWidth; j++)
		{
			int tW = (int)(rateW * j);
			int tW1 = min(tW + 1, srcW - 1);
			float v = (float)(rateW * j - tW);

			//f(i+u,j+v) = (1-u)(1-v)f(i,j) + (1-u)vf(i,j+1) + u(1-v)f(i+1,j) + uvf(i+1,j+1) 
			for (int k = 0; k < 3; k++)
			{
				desBuf[i * desLineSize + j * bitCount / 8 + k] =
					(1 - u)*(1 - v) * srcBuf[tH * lineSize + tW * bitCount / 8 + k] +
					(1 - u)*v*srcBuf[tH1 * lineSize + tW * bitCount / 8 + k] +
					u * (1 - v) * srcBuf[tH * lineSize + tW1 * bitCount / 8 + k] +
					u * v * srcBuf[tH1 * lineSize + tW1 * bitCount / 8 + k];
			}
		}
	}

	//����Ŀ���ļ�
	HFILE hfile = _lcreat(strFilesave, 0);
	//�ļ�ͷ��Ϣ
	BITMAPFILEHEADER nbmfHeader;
	nbmfHeader.bfType = 0x4D42;
	nbmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)
	+ iDstWidth * iDstHeight * bitCount / 8;
	nbmfHeader.bfReserved1 = 0;
	nbmfHeader.bfReserved2 = 0;
	nbmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	//Bitmapͷ��Ϣ
	BITMAPINFOHEADER   bmi;
	bmi.biSize = sizeof(BITMAPINFOHEADER);
	bmi.biWidth = iDstWidth;
	bmi.biHeight = iDstHeight;
	bmi.biPlanes = 1;
	bmi.biBitCount = bitCount;
	bmi.biCompression = BI_RGB;
	bmi.biSizeImage = 0;
	bmi.biXPelsPerMeter = 0;
	bmi.biYPelsPerMeter = 0;
	bmi.biClrUsed = 0;
	bmi.biClrImportant = 0;

	//д���ļ�ͷ��Ϣ
	_lwrite(hfile, (LPCSTR)&nbmfHeader, sizeof(BITMAPFILEHEADER));
	//д��Bitmapͷ��Ϣ
	_lwrite(hfile, (LPCSTR)&bmi, sizeof(BITMAPINFOHEADER));
	//д��ͼ������
	_lwrite(hfile, (LPCSTR)desBuf, desBufSize);
	_lclose(hfile);
	fclose(pFile);
}

// CScreenGifDlg �Ի���
CScreenGifDlg::CScreenGifDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenGifDlg::IDD, pParent)
{
	m_index = 0;
	m_pAllScreenDlg = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pRegionDlg = NULL;
	//�����ʾ����Ļ�ߴ�
	m_AllScreen.left = 0;
	m_AllScreen.top = 0;
	m_AllScreen.right = GetSystemMetrics(SM_CXSCREEN);
	m_AllScreen.bottom = GetSystemMetrics(SM_CYSCREEN);
	// ������ĻDC
	m_srcDc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	m_bIsReadyGif = false;
	m_wGifbeginSize = 0;
	m_strCurentGif = _T("");
	m_brush.CreateSolidBrush(RGB(37, 37, 38));
	//�ڳ���Ŀ¼�´���picĿ¼
	m_strPath = CGlobalData::GetCurrentFilePath();
	m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));
	m_strPath += _T("\\gif");
	if (!PathFileExists(m_strPath))
	{
		CreateDirectory(m_strPath, NULL);
	}
	//��ʼ������
	m_pen.m_style = PS_SOLID;
	m_pen.m_width = 2;
	m_pen.m_color = RGB(255, 0, 0);
	m_rcbrush.CreateSolidBrush(NULL_BRUSH);

	m_blEnableStart = FALSE;
	m_iProgressCount = 0;
	m_iDstWidth = 0;
	m_iDstHeight = 0;
	m_fSizeNumber = 1.0;
	m_iFrameNum = 0;
	m_blConformSize = true;
}

CScreenGifDlg::~CScreenGifDlg()
{
	NOTIFYICONDATA nd;
	nd.cbSize = sizeof (NOTIFYICONDATA);
	nd.hWnd = m_hWnd;
	nd.uID = NIF_ICON;
	Shell_NotifyIcon(NIM_DELETE, &nd);
	UnregisterHotKey(m_hWnd, HOTKEYID);
	if (m_process != NULL)
	{
		delete m_process;
	}
}
void CScreenGifDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_Progress);
	DDX_Control(pDX, IDC_EDIT1, m_edtGifAddress);
	DDX_Control(pDX, IDC_COMBO1, m_cboSize);
	DDX_Control(pDX, IDC_STC_FRAME, m_stcFrame);
	DDX_Control(pDX, IDC_CHECK1, m_chkConformSize);
	DDX_Control(pDX, IDC_STC_AREA, m_stcArea);
}

BEGIN_MESSAGE_MAP(CScreenGifDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_GETAREA, &CScreenGifDlg::OnBnRectangle)
	ON_BN_CLICKED(ID_START, &CScreenGifDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_SHARE, &CScreenGifDlg::OnBnClickedShare)
	ON_BN_CLICKED(ID_GETPIC, &CScreenGifDlg::OnBnClickedGetPic)
	ON_BN_CLICKED(ID_SAVE, &CScreenGifDlg::OnBnClickedSave)
	ON_MESSAGE(UM_BEGINPOINT, OnBeginPoint)
	ON_MESSAGE(UM_ENDPOINT, OnEndPoint)
	ON_MESSAGE(UM_MOUSEMOVE, OnMovePoint)
	ON_MESSAGE(UK_REGION, OnScreenPic)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipText)
	ON_MESSAGE(WM_USER_NOTIFYICON, OnNotifyMsg)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_MESSAGE(WM_PROGRESS_ADD, ProgressAdd)
	ON_COMMAND(WM_USEHELP,	OnHelp)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_CANCEL, &CScreenGifDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CIRCLE, &CScreenGifDlg::OnBnClickedButtonCircle)
	ON_BN_CLICKED(IDC_BUTTON_ARROW, &CScreenGifDlg::OnBnClickedButtonArrow)
	ON_BN_CLICKED(IDC_BUTTON_A, &CScreenGifDlg::OnBnClickedButtonA)
	ON_BN_CLICKED(IDC_BUTTON_BACK, &CScreenGifDlg::OnBnClickedButtonBack)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CScreenGifDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &CScreenGifDlg::OnBnClickedCheck1)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CScreenGifDlg ��Ϣ�������
HRESULT CScreenGifDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == HOTKEYID)
	{
		GetRect();
	}
	return TRUE;
}


// ��ȡ�յ�����
LRESULT CScreenGifDlg::OnEndPoint(WPARAM wParam, LPARAM lParam)
{
	POINT* pt = (POINT*)lParam;
	m_rc.right = pt->x;
	m_rc.bottom = pt->y;
	bool isAllScreen = false;
	if (pt->x -  m_rc.left == 0 || pt->y - m_rc.top == 0)//���������Чȡȫ��
	{
		//��ȡ����ȫ���ߴ�
		m_rc = m_AllScreen;
		isAllScreen = true;
	}
	//��������ʾ��ѡ�Ի���
	if (NULL != m_pRegionDlg) {
		m_pRegionDlg->DestroyWindow();
		m_pRegionDlg = NULL;
	}

	if (m_pRegionDlg == NULL)
	{
		m_pRegionDlg = new CRegionDlg;
		m_pRegionDlg->m_rect = m_rc;
		m_pRegionDlg->Create(IDD_DIALOG_REGION, NULL);
		m_pRegionDlg->ShowWindow(SW_SHOW);
		m_pRegionDlg->Invalidate();
		m_blEnableStart = TRUE;
		Invalidate();
		RedrawWindow();
	}
	RECT rect;
	GetWindowRect(&rect);
	RECT rt;
	rt.left = m_rc.right - (rect.right - rect.left);
	rt.top = m_rc.bottom;
	rt.right = m_rc.right;
	rt.bottom = m_rc.bottom + (rect.bottom - rect.top);
	if (isAllScreen)
	{
		rt.top = 0;
		rt.right = m_rc.right;
		rt.bottom = rect.bottom - rect.top;
	}

	SetWindowPos(&wndTopMost, rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, SWP_SHOWWINDOW);
	MoveWindow(&rt);
	ShowWindow(SW_SHOW);
	//���ر����Ի���
	if (m_pAllScreenDlg->IsWindowVisible())
	{
		m_pAllScreenDlg->ShowWindow(SW_HIDE);
	}
	
	return TRUE;
}

// ��ȡ��ʼ������
LRESULT CScreenGifDlg::OnBeginPoint(WPARAM wParam, LPARAM lParam)
{
	POINT* pt = (POINT*)lParam;
	m_rc.left = pt->x;
	m_rc.top = pt->y;

	return TRUE;
}

// ��ȡѡ������
LRESULT CScreenGifDlg::OnMovePoint(WPARAM wParam, LPARAM lParam)
{
	POINT* pt = (POINT*)lParam;
	int iWidth = pt->x - m_rc.left;
	int iHeight = pt->y - m_rc.top;

	CString cstrWidth;
	CString cstrHeight;
	CString cstrSize;
	cstrWidth.Format(_T("%d*"), iWidth);
	cstrHeight.Format(_T("%d"),iHeight);
	cstrSize = cstrWidth + cstrHeight;
	m_stcArea.SetWindowText(cstrSize);

	return TRUE;
}

// ��ͼ��ݼ���Ϣ
LRESULT CScreenGifDlg::OnScreenPic(WPARAM wParam, LPARAM lParam)
{
	//ָ������
	GetRect();
	return TRUE;
}
// ��ӹ�����
void CScreenGifDlg::AddToolbar()
{
	GetDlgItem(ID_GETAREA)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_SAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_START)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_GETPIC)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_SHARE)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_FULLSCREEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CANCEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CIRCLE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_ARROW)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_A)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_BACK)->ShowWindow(SW_HIDE);

	if (m_toolbar.CreateEx(this, CBRS_FLYBY | TBSTYLE_FLAT | TBSTYLE_TRANSPARENT | WS_CHILD | WS_VISIBLE | CBRS_SIZE_FIXED | CBRS_TOP | CBRS_TOOLTIPS))
	{
		
		static UINT BASED_CODE DockTool[] = { ID_START, IDC_CANCEL, ID_GETPIC };
		CBitmap bitmap;
		bitmap.LoadBitmap(IDB_BITMAP4);
		m_toolbarlist.Create(16, 16, ILC_COLOR24, 0, 0);
		m_toolbarlist.Add(&bitmap, (CBitmap*)NULL);
		//���ù�������ťͼƬ
		m_toolbar.GetToolBarCtrl().SetImageList(&m_toolbarlist);
		//���ù�������ť��С�� �Ͱ�ť��λͼ��С
		SIZE sbutton, sImage;
		sbutton.cx = 33;
		sbutton.cy = 32;
		sImage.cx = 16;
		sImage.cy = 16;
		m_toolbar.SetSizes(sbutton, sImage);
		m_toolbar.SetButtons(DockTool, (UINT)3);
		RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	}
	

}
BOOL CScreenGifDlg::OnToolTipText(UINT NID, NMHDR* pNMHDR, LRESULT* pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	//  UNICODE��Ϣ
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR   szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;
	UINT tempid;
	UINT fid;
	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags&TTF_IDISHWND) || pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags&TTF_IDISHWND))
	{
		//idFromΪ��������HWND
		nID = ::GetDlgCtrlID((HWND)nID);
		//tem = m_toolbar.CommandToIndex(pNMHDR->idFrom);  
		  HWND hWnd = ::GetFocus();
		  tempid = m_toolbar.GetToolBarCtrl().GetDlgCtrlID();// ::GetDlgCtrlID(hWnd);	 
	}
	if (nID != 0)   //��Ϊ�ָ���
	{
		tempid = m_toolbar.GetToolBarCtrl().GetHotItem();
		if (tempid > 15) {
			return FALSE;
		}
		fid = m_toolbar.GetItemID(tempid);
		strTipText.LoadString(fid);
		
		strTipText = strTipText.Mid(strTipText.Find(_T("/n"), 0) + 1);
#ifndef   _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif
		*pResult = 0;
		// ʹ��������ʾ������������
		CRect rcChild;
		GetWindowRect(&rcChild);
		//ScreenToClient(rcChild);
		int x = 0, y = rcChild.top;
		switch (tempid)
		{
			case 0:
				x = rcChild.left - 15;
				break;
			case 1:
				x = rcChild.left + 15;
				break;
			case 2:
				x = rcChild.left + 50;
				break;
			case 3:
				x = rcChild.left + 90;
				break;
			case 4:
				x = rcChild.left + 130;
				break;
			case 5:
				x = rcChild.left + 165;
				break;
			case 6:
				x = rcChild.left + 200;
				break;
			case 7:
				x = rcChild.left + 230;
				break;
			case 8:
				x = rcChild.left + 260;
				break;
			case 9:
				x = rcChild.left + 295;
				break;
			default:
				x = rcChild.left + 330;
				break;
		}
		y = rcChild.top + 40;
		::MoveWindow(pNMHDR->hwndFrom, x, y, rcChild.Width(), rcChild.Height(), TRUE);
		return TRUE;
	}
	return TRUE;

}
	

BOOL CScreenGifDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	
	EnableToolTips(TRUE);
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	m_Progress.ShowWindow(SW_HIDE);	//Ӱ�ؽ�����
	
	::RegisterHotKey(m_hWnd, HOTKEYID, MOD_CONTROL|MOD_ALT|MOD_WIN, NULL);//ע��ȫ�ֿ�ݼ� CTRL + WIN + ALT
	AddToolbar();	//��ӹ�����
	
	m_notify.cbSize = sizeof NOTIFYICONDATA;
	m_notify.hWnd = this->m_hWnd;
	m_notify.uID = IDR_MAINFRAME;
	m_notify.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	CString str = _T("Gif��ͼ����");
	CStringW strWide = CT2CW(str); // ��T�ַ�ת���ɿ��ַ�
	wcscpy_s(m_notify.szTip, strWide);
	m_notify.uCallbackMessage = WM_USER_NOTIFYICON;
	m_notify.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;    
	Shell_NotifyIcon(NIM_ADD, &m_notify);	//������ͼ�����������
	//����
	m_map = new CPicMap;
	m_map->Create(CPicMap::IDD, NULL);
	m_map->ShowWindow(SW_HIDE);
	//��ͼ����
	m_process = new CPicProcess;
	m_process->m_rc = m_rc;
	m_map->m_pProcess = m_process; //��������ǰͼ���ַ
	m_process->m_map = m_map; //���滭����ַ
	m_process->Create(CPicProcess::IDD, NULL);	
	m_process->ShowWindow(SW_HIDE);
	
	//���ųߴ�
	CString cstrSizeInfo;
	for (int i = 10; i >= 1; i --) {
		cstrSizeInfo.Format(_T("ԭ�ߴ��%d"),10 * i);
		cstrSizeInfo += _T("%");
		m_cboSize.InsertString(10 - i,cstrSizeInfo);
	}

	m_cboSize.SetCurSel(0);
	m_fSizeNumber = 1.0;
	m_cboSize.EnableWindow(FALSE);

	m_chkConformSize.SetCheck(1);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CScreenGifDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CScreenGifDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




// ָ������
void CScreenGifDlg::GetRect()
{
	if (m_pAllScreenDlg != NULL)	//�����ѡ��ťʱ������֮ǰ�ı����Ի��򣬴����µ�
	{
		m_pAllScreenDlg->DestroyWindow();
		m_pAllScreenDlg = NULL;
	}
	m_pAllScreenDlg = new CAllScreenDlg;		//�����µ�ȫ��Ļ�����Ի���
	m_pAllScreenDlg->m_rc = m_AllScreen;
	m_pAllScreenDlg->Create(IDD_DIALOG_ALLSCREEN, NULL);
	m_pAllScreenDlg->ShowWindow(SW_SHOW);
}

// TODO:  ����Gifͼ
void CScreenGifDlg::OnBnClickedStart()
{
	if (!m_blEnableStart) {
		MessageBox(_T("��ʹ�ÿ�ݼ�ѡ������"));
		return;
	}
	m_iFrameNum = 0;

	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	if (m_bIsReadyGif)
	{
		//Gif¼�ƽ���
		//�ָ���ť����
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETAREA, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SHARE, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SAVE, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETPIC, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_CANCEL, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_A, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_BACK, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_ARROW, true);
		(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_CIRCLE, true);

		m_toolbar.SetButtonInfo(m_toolbar.CommandToIndex(ID_START), ID_START, TBBS_BUTTON, 0);
		
		SetDlgItemText(ID_START, _T("¼��GIF"));
		m_bIsReadyGif = false;
		//�رն�ʱ��
		KillTimer(1);
		m_Progress.SetRange(0, m_index);
		m_Progress.SetPos(0);
		m_Progress.SetStep(1);
		m_Progress.ShowWindow(SW_SHOW);
		MakeGif();	//����GifͼƬ
		return;
	}

	//�ı�ͼ��
	m_bIsReadyGif = true;
	CImageList* imagList = (m_toolbar.GetToolBarCtrl()).GetImageList();
	int toolbarCount = imagList->GetImageCount();
	CBitmap cbmp;
	BITMAP bmp;
	cbmp.LoadBitmap(IDB_BITMAP2);
	cbmp.GetBitmap(&bmp);
	imagList->Add(&cbmp, RGB(192, 192, 192));
	(m_toolbar.GetToolBarCtrl()).SetImageList(imagList);
	imagList->Detach();
	imagList = NULL;
	m_toolbar.SetButtonInfo(m_toolbar.CommandToIndex(ID_START), ID_START, TBBS_BUTTON, 3);
	//�ǰ�ťʧȥ����
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SAVE, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_SHARE, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETPIC, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_CANCEL, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(ID_GETAREA, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_A, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_BACK, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_ARROW, false);
	(m_toolbar.GetToolBarCtrl()).EnableButton(IDC_BUTTON_CIRCLE, false);
	SetDlgItemText(ID_START, _T("ֹͣ��"));
	SetTimer(1, 67, NULL);	//���ö�ʱ����ÿ��66ms��һ��ͼ������
	m_index = 0;
	m_wGifbeginSize = 0;
	CString strTime; //��ȡϵͳʱ�� ����
	CTime tm;
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y%m%d%H%M%S");
	CString strDefName = m_strPath + _T("\\gif_") + strTime + _T(".gif");
	m_strCurentGif = strDefName;
}



// TODO:  ������
void CScreenGifDlg::OnBnClickedShare()
{
	// TODO:  ������
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	CString strPath = _T("http://service.weibo.com/share/share.php?appkey=583395093&title=");
	CString strWords = _T("΢������");
	CString strContext = strPath + strWords;
	OnBnClickedCancel();
	ShellExecute(NULL, _T("open"), (LPCTSTR)strContext, _T(""), _T(""), SW_SHOW);

}

// TODO:  ��ͼ
void CScreenGifDlg::OnBnClickedGetPic()
{
	if (m_strCurentGif == "") {
		MessageBox(_T("���Ƚ���gif��ȡ��"));
		return;
	}

	m_edtGifAddress.SetWindowText(_T(""));
	m_Progress.SetRange(0,10);
	m_Progress.SetPos(0);
	m_Progress.SetStep(1);
	m_Progress.ShowWindow(SW_SHOW);

	DWORD dwTime = GetTickCount() + 5000;
	while (dwTime > GetTickCount())
	{
		ProgressAdd(0,0);
		Sleep(100);
	}

	m_Progress.ShowWindow(SW_HIDE);
	UploadFile();
}

// TODO:  ���Ϊ
void CScreenGifDlg::OnBnClickedSave()
{
	GetPic();
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	CString filter;
	filter = _T("*.bmp | *.BMP ||");
	CString strTime; //��ȡϵͳʱ�� ����
	CTime tm; 
	tm = CTime::GetCurrentTime();
	strTime = tm.Format("%Y%m%d%H%M%S");
	CString strDefName = _T("SGif��ͼ") + strTime + _T(".bmp");
	CFileDialog dlg(FALSE, NULL, LPCTSTR(strDefName), OFN_HIDEREADONLY, filter);
	if (dlg.DoModal() == IDOK)
	{
		CString str;
		str = dlg.GetPathName();
		if (str.Find(_T(".bmp")) == -1 || str.Find(_T(".BMP")) == -1) // ȷ���û�������ȷ
		{
			str += _T(".bmp");
		}
		CBitmap* pbmp = m_dcCompatible.GetCurrentBitmap();
		if (pbmp != NULL)
		{
			SaveBitmapToFile(pbmp, str);
		}
	}
	m_mapCompatible.DeleteObject();
		
}

bool CScreenGifDlg::GetPic()
{
	// ��������DC
	if (m_dcCompatible.m_hDC == NULL)
	{
		m_dcCompatible.CreateCompatibleDC(&m_srcDc);
	}

	if (m_process->m_SrcMemDC.m_hDC == NULL)
	{
		m_process->m_SrcMemDC.CreateCompatibleDC(&m_srcDc);
	}
	
	if (m_map->m_SrcMemDC.m_hDC == NULL)
	{
		m_map->m_SrcMemDC.CreateCompatibleDC(&m_srcDc);
	}


	// ����λͼ����DC
	if (m_mapCompatible.m_hObject == NULL)
	{
		m_mapCompatible.CreateCompatibleBitmap(&m_srcDc, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4);

	}
	CBitmap bmp1, bmp2;
	bmp1.CreateCompatibleBitmap(&m_srcDc, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4);
	bmp2.CreateCompatibleBitmap(&m_srcDc, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4);


	// ������λͼѡ�����DC��
	m_dcCompatible.SelectObject(&m_mapCompatible);
	m_process->m_SrcMemDC.SelectObject(&bmp1);
	m_map->m_SrcMemDC.SelectObject(&bmp2);
	// ��ԭʼ�豸��ɫ���������ݿ鸴�Ƶ�����DC��
	m_dcCompatible.BitBlt(0, 0, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, &m_srcDc, m_rc.left + 2, m_rc.top + 2, SRCCOPY);
	m_process->m_SrcMemDC.BitBlt(0, 0, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, &m_srcDc, m_rc.left + 2, m_rc.top + 2, SRCCOPY);
	m_map->m_SrcMemDC.BitBlt(0, 0, m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, &m_srcDc, m_rc.left + 2, m_rc.top + 2, SRCCOPY);
	CClientDC dc(this);
	//RECT displayRect;
	// ��λͼ�����ڼ��а���
	if (OpenClipboard())
	{
		//��ռ��а�
		EmptyClipboard();
		//����λͼ
		SetClipboardData(CF_BITMAP, m_dcCompatible.GetCurrentBitmap()->m_hObject);
		CloseClipboard();
	}
	return true;
	
}

DWORD WINAPI thread_MakeGif(LPVOID lpParam)
{
	CScreenGifDlg *dlgScreenGif = (CScreenGifDlg*)lpParam;

	USES_CONVERSION;
	char * filename = T2A(dlgScreenGif->m_strCurentGif);
	string path = replace_all((string)filename, "\\", "/");
	string strFileName = path.substr(0, path.length() - 4) + "_1.gif";

	//���ļ����µ�bmpͼ�ϳ�һ��gif�ļ�
	int n = dlgScreenGif->m_index;
	dlgScreenGif->m_index = 0;
	bool blFirstBmp = true;
	CFile file;
	while (n--)
	{
		CString strbmpName;
		strbmpName.Format(_T("\\Bitmap_%d.bmp"), dlgScreenGif->m_index);
		strbmpName = dlgScreenGif->m_strPath + strbmpName;
		dlgScreenGif->m_index++;
		::PostMessage(dlgScreenGif->m_hWnd, WM_PROGRESS_ADD, NULL, NULL);
		HBITMAP hBmp = (HBITMAP)LoadImage(NULL, strbmpName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (hBmp == NULL) {
			break;
		}

		BYTE *palette = NULL;
		BYTE *pData = NULL;
		int nWidth, nHeight;
		BYTE bitsPixel = 8;

		//Ϊλͼ���ɵ�ɫ��,�õ��������ݡ�����
		if (GetData(hBmp, &palette, &pData, &bitsPixel, &nWidth, &nHeight) == FALSE) {
			DeleteObject(hBmp);
			break;
		}
		DeleteObject(hBmp);

		if (blFirstBmp) {
			//����GIF�ļ�
			if (file.Open(dlgScreenGif->m_strCurentGif, CFile::typeBinary | CFile::modeCreate | CFile::modeWrite) == FALSE) {
				break;
			}
			//дGIFͷ
			CreateGIFHeard(file, nWidth, nHeight, bitsPixel);
			blFirstBmp = false;
		}
		//����ͼƬ
		AddImageToGIF(file, pData, palette, nWidth, nHeight, bitsPixel, 15, -1);
		delete[]pData;
		delete[]palette;

		::DeleteFile(strbmpName);
	}
	//����GIF
	CloseGIF(file);
	file.Close();

	//����ˮӡ
	dlgScreenGif->MakeWatermark(path, strFileName);
	dlgScreenGif->m_Progress.ShowWindow(SW_HIDE);

	return 0;
}

void CScreenGifDlg::MakeGif()
{
	HANDLE hdlMakeGif = CreateThread(NULL, 0, thread_MakeGif, this, 0, NULL);
	return;
}



//����bitmapͼ
void CScreenGifDlg::SaveBitmapToFile(CBitmap* pBitmap, CString fileName)
{
	//����λͼ�ļ�ÿ��������ռ�ֽ���    
	WORD wBitCount = 0;//ÿ��������ռλ��
	HDC hDC = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	int iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);
	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 16)
		wBitCount = 16;
	else if (iBits <= 24)
		wBitCount = 24;
	else if (iBits <= 32)
		wBitCount = 32;
	//�����ɫ���С    
	DWORD dwPaletteSize = 0;
	if (wBitCount <= 16)
	{
		dwPaletteSize = (1 << wBitCount) * sizeof (RGBQUAD);
	}
	//����λͼ��Ϣͷ�ṹ  
	BITMAPINFOHEADER bi; //λͼ��Ϣͷ�ṹ   
	BITMAP Bitmap;	//λͼ���Խṹ
	GetObject(pBitmap->m_hObject, sizeof (BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof (BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	//λͼ�����ֽڴ�С
	DWORD dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;
	//Ϊλͼ���ݷ����ڴ�  
	LPBITMAPINFOHEADER lpbi; //ָ��λͼ��Ϣͷ�ṹ   
	HANDLE hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof (BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;
	// �����ɫ��         
	HANDLE hPal = GetStockObject(DEFAULT_PALETTE);
	HANDLE hOldPal = NULL;
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	//   ��ȡ�õ�ɫ�����µ�����ֵ    
	GetDIBits(hDC, (HBITMAP)(pBitmap->m_hObject), 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof (BITMAPINFOHEADER)+dwPaletteSize,
		(LPBITMAPINFO)lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ��       
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}    
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ�ṹ   
	bmfHdr.bfType = 0x4D42;     //   "BM"    
	DWORD dwDIBSize = sizeof (BITMAPFILEHEADER)
		+sizeof (BITMAPINFOHEADER)
		+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof (BITMAPFILEHEADER)
		+(DWORD)sizeof (BITMAPINFOHEADER)
		+dwPaletteSize;
	//����λͼ�ļ�
	CFile bmpfile;
	bmpfile.Open(fileName, CFile::modeCreate |CFile::modeWrite | CFile::typeBinary);
	//   д��λͼ�ļ�ͷ    
	bmpfile.Write(&bmfHdr, sizeof(BITMAPFILEHEADER));
	//   д��λͼ�ļ���������    
	bmpfile.Write(lpbi,dwDIBSize);
	//���            
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	bmpfile.Close();

}

// ÿ��50ms��ȡһ��ͼ
void CScreenGifDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�����bmp�ļ�
	if (nIDEvent == 1)
	{
		CString strbmpName;
		strbmpName.Format(_T("\\Bitmap%d.bmp"), m_index);
		strbmpName = m_strPath + strbmpName;
		if (m_bIsReadyGif)	//�Ƿ�����¼��
		{
			GetPic();
			CBitmap* pbmp = m_dcCompatible.GetCurrentBitmap();
			if (pbmp != NULL)
			{
				SaveBitmapToFile(pbmp, strbmpName);
			}
			m_mapCompatible.DeleteObject();
			m_iFrameNum++;
			CString cstrFrame;
			cstrFrame.Format(_T("total  %d frame"), m_iFrameNum);
			m_stcFrame.SetWindowText(cstrFrame);

			//���ļ��������� 
			CString strDstbmpName;
			strDstbmpName.Format(_T("\\Bitmap_%d.bmp"), m_index);
			strDstbmpName = m_strPath + strDstbmpName;

			int iSrcWidth = m_rc.right - m_rc.left;
			int iSrcHeight = m_rc.bottom - m_rc.top;

			if (!m_blConformSize) {
				m_iDstWidth = iSrcWidth * m_fSizeNumber;
				m_iDstHeight = iSrcHeight * m_fSizeNumber;
			}
			else {
				m_iDstWidth = 400;
				m_iDstHeight = 250;
			}
			ResizeBmpFile(strbmpName, iSrcWidth, iSrcHeight, strDstbmpName, m_iDstWidth, m_iDstHeight);

			::DeleteFile(strbmpName);
		}
		m_index++;
	}
	CDialogEx::OnTimer(nIDEvent);
}


HBRUSH CScreenGifDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}



LRESULT  CScreenGifDlg::OnNotifyMsg(WPARAM wparam, LPARAM lparam)
{
	if (wparam != IDR_MAINFRAME)
		return    1;
	switch (lparam)
	{
		case  WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�     
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);//�õ����λ��     
			CMenu menu;
		
			menu.CreatePopupMenu();//����һ������ʽ�˵�     
			menu.AppendMenu(MF_STRING, WM_USEHELP, _T("����"));
			menu.AppendMenu(MF_STRING, WM_DESTROY, _T("�˳�"));
			//ȷ������ʽ�˵���λ��
			SetForegroundWindow();
			menu.TrackPopupMenu(TPM_LEFTALIGN, lpoint->x, lpoint->y, this);
			//��Դ����     
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;
		}
		break;
		case    WM_LBUTTONDBLCLK://˫������Ĵ���     
		{
			 NULL;//�򵥵���ʾ���������¶�     
		}
		break;
	}
	return 0;
}

//ȡ��
void CScreenGifDlg::OnBnClickedCancel()
{
	if (m_process->IsWindowVisible())
	{
		m_process->ShowWindow(SW_HIDE);
	}
	if (m_map->IsWindowVisible())
	{
		m_map->ShowWindow(SW_HIDE);
	}
	if (m_pRegionDlg != NULL)
	{
		m_pRegionDlg->ShowWindow(SW_HIDE);
	}
	m_blEnableStart = FALSE;
	//ShowWindow(SW_HIDE);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CScreenGifDlg::OnHelp()
{
	CString strHelp = _T("���������ʵ��gifͼƬ����������ˮӡ�����ɷ�������ַ�Ĺ��ܣ������ʹ�÷�ʽΪ��������������ͣ�����������С�");
	strHelp += _T("����Ҫ¼��һ��gifͼ��ʱ��ʹ�ÿ�ݼ� CTRL + WIN + ALT ������ѡȡ��Ļ�����ͼ��");
	strHelp += _T("�̰�ť������ɵ���ˮӡ���ϴ���������������ȡ����ͼ������ʼ¼��gif���ٴΰ��½���gif¼�ơ�");

	::MessageBox(NULL, strHelp, _T("����"), MB_YESNO | MB_ICONINFORMATION);

}

//���ι���
void CScreenGifDlg::OnBnRectangle()
{
	//��ѡ����ι��ߵ�ʱ���ȸ���ͼ���򸲸Ǵ��ڣ�����Ҫ�ڸô��������ͼ
	if (m_process != NULL && m_map != NULL)
	{
		GetPic();	//�Ƚ�ͼ
		m_process->m_iTool = 1;	//����
		m_process->m_rc = m_rc;
		m_map->m_curRect = m_rc;
		m_process->m_rect.SetRectEmpty();
		m_process->m_vecRect.clear();
		m_map->SetWindowPos(&wndBottom, m_rc.left + 2, m_rc.top + 2,
			m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, SWP_SHOWWINDOW);
		m_process->SetWindowPos(&wndTop, m_rc.left + 2, m_rc.top + 2,
			m_rc.right - m_rc.left - 4, m_rc.bottom - m_rc.top - 4, SWP_SHOWWINDOW);

	}
}
//��ԲȦ
void CScreenGifDlg::OnBnClickedButtonCircle()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

//��ͷ
void CScreenGifDlg::OnBnClickedButtonArrow()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

//����
void CScreenGifDlg::OnBnClickedButtonA()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������


}


//����
void CScreenGifDlg::OnBnClickedButtonBack()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
}

struct curl_return_string {
	char *str;
	size_t len;
	size_t size;
}; // ���ڴ�curl���صĽ��

size_t recive_data_from_http_api(void *buffer, size_t size, size_t nmemb, void *user_p)
{
	struct curl_return_string *result_t = (struct curl_return_string *)user_p;

	if (result_t->size < ((size * nmemb) + 1)) {
		result_t->str = (char *)realloc(result_t->str, (size * nmemb) + 1);
		if (result_t->str == NULL) {
			return 0;
		}
		result_t->size = (size * nmemb) + 1;
	}

	result_t->len = size * nmemb;
	memcpy(result_t->str, buffer, result_t->len);
	result_t->str[result_t->len] = '\0';

	return result_t->len;
}

//�ϴ�������
void CScreenGifDlg::UploadFile()
{
	CURL *curl_upload;
	CURLcode res;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_version_info_data *vinfo = curl_version_info(CURLVERSION_NOW);
	if (vinfo->features & CURL_VERSION_SSL) {
		MessageBox(_T("SSL Surport!"));
	}
	curl_upload = curl_easy_init();
	if (curl_upload)
	{
		struct curl_httppost* post = NULL;
		struct curl_httppost* last = NULL;
		// �������淵�� http �Ľ��
		struct curl_return_string curl_result_t;
		curl_result_t.len = 0;
		curl_result_t.str = (char *)calloc(1, 1024);
		if (curl_result_t.str == NULL) {
			return;
		}
		curl_result_t.size = 1024;

		USES_CONVERSION;
		char * filename = T2A(m_strCurentGif);
		string strPostFilePath = (string)filename;
		strPostFilePath = strPostFilePath.substr(0, strPostFilePath.length() - 4) + "_1.gif";
		string path = replace_all((string)filename, "\\", "/");
		string strFileName = path.substr(0, path.length() - 4) + "_1.gif";
		filename = (char*)strPostFilePath.c_str();

		//Sleep(1000);

		//�ϴ��ļ���������
		curl_formadd(&post, &last,
			CURLFORM_COPYNAME, "file",
			CURLFORM_FILE, filename,
			CURLFORM_END);

		curl_formadd(&post,
			&last,
			CURLFORM_COPYNAME, "token",
			CURLFORM_COPYCONTENTS, "4KQgBVw2+JE-",
			CURLFORM_END);

		curl_easy_setopt(curl_upload, CURLOPT_URL, "http://admin.gif.com/admin/gif/upload");
		curl_easy_setopt(curl_upload, CURLOPT_HTTPPOST, post);
		curl_easy_setopt(curl_upload, CURLOPT_WRITEFUNCTION, recive_data_from_http_api);
		curl_easy_setopt(curl_upload, CURLOPT_WRITEDATA, &curl_result_t);
		res = curl_easy_perform(curl_upload);
		/* Check for errors */
		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));
			MessageBox(_T("�ӿڷ��ʳ����ϴ�������ʧ�ܣ�%s", curl_easy_strerror(res)));
		}
		else {
			if (curl_result_t.str) {
				struct array_list *tmp_array;
				struct json_object *url;
				struct json_object *err;
				struct json_object *result_json;

				result_json = json_tokener_parse(curl_result_t.str);

				if (json_object_get_type(result_json) != json_type_object) {
					MessageBox(_T("�ϴ�������ʧ�ܣ��������ʹ���"));
					return;
				}

				//�����Ƿ�ʧ��
				err = json_object_object_get(result_json, "err");
				if (err != NULL) {
					string errInfo = json_object_get_string(err);
					MessageBox(_T("�ϴ�������ʧ�ܣ�"));
					return;
				}

				url = json_object_object_get(result_json, "url");
				string filePath = json_object_get_string(url);

				if (filePath != "") {
					//�ڽ����Ͻ���չʾ
					const char* cPath = filePath.c_str();
					CString cstrCmd = A2T(cPath);
					m_edtGifAddress.SetWindowText(cstrCmd);
				}
			}
		}

		if (curl_result_t.str != NULL) {
			free(curl_result_t.str);
			curl_result_t.str = NULL;
			curl_result_t.len = 0;
			curl_result_t.size = 0;
		}
		/* always cleanup */
		curl_easy_cleanup(curl_upload);
		curl_formfree(post);
		curl_global_cleanup();
	}

	//�رս���
	CloseProgram(_T("cmd.exe"));
}

void CScreenGifDlg::MakeWatermark(string &_path, string &_fileName)
{
	USES_CONVERSION;
	//��ǰ�̷�
	char *currentDriver = T2A(m_strPath);
	string strCurDriver = (string)currentDriver;
	string strCurPath = strCurDriver;
	strCurDriver = strCurDriver.substr(0, 2);

	//�����
	//E: & cd Git_Programe/C++/ScreenGif/Debug/vc120/gif/ & ffmpeg -i E:/Git_Programe/C++/ScreenGif/Debug/vc120/gif/SGif20170516150152.gif 
	//-vf "movie = logo.png, scale = 30:15[watermask]; [in][watermask] overlay = 10:10[out]" 
	//-f gif E:/Git_Programe/C++/ScreenGif/Debug/vc120/gif/SGif20170516150152_4.gif
	int iX = 10;
	int iY = m_iDstHeight - 60;
	char x_X[32];
	char y_Y[32];
	sprintf_s(x_X, "%d", iX);
	sprintf_s(y_Y, "%d", iY);
	string x = x_X;
	string y = y_Y;
	string cmd;
	cmd = "/k ";
	cmd += strCurDriver + " && ";
	cmd += "cd " + strCurPath;
	cmd += " && ffmpeg -i " + _path;
	cmd += " -vf ";
	cmd += '"';
	cmd += "movie = logo.png, scale = 117:49[watermask]; [in][watermask] overlay = " + x + ":" + y + "[out]";
	cmd += '"';
	cmd += " -f gif ";
	cmd += _fileName;

	//ת���ַ�����
	const char* cmdInfo = cmd.c_str();
	CString cstrCmd = A2T(cmdInfo);
	SHELLEXECUTEINFO ShExecInfo = { 0 };
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = _T("C:\\Windows\\system32\\cmd.exe");
	ShExecInfo.lpParameters = cstrCmd;// (LPCTSTR)cmd.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_HIDE;
	ShExecInfo.hInstApp = NULL;
	ShellExecuteEx(&ShExecInfo);
}



void CScreenGifDlg::OnCbnSelchangeCombo1()
{
	int index = m_cboSize.GetCurSel();
	switch (index) {
	case 0:
		m_fSizeNumber = 1.0;
		break;
	case 1:
		m_fSizeNumber = 0.9;
		break;
	case 2:
		m_fSizeNumber = 0.8;
		break;
	case 3:
		m_fSizeNumber = 0.7;
		break;
	case 4:
		m_fSizeNumber = 0.6;
		break;
	case 5:
		m_fSizeNumber = 0.5;
		break;
	case 6:
		m_fSizeNumber = 0.4;
		break;
	case 7:
		m_fSizeNumber = 0.3;
		break;
	case 8:
		m_fSizeNumber = 0.2;
		break;
	case 9:
		m_fSizeNumber = 0.1;
		break;
	default:
		m_fSizeNumber = 1.0;
		break;
	}
}

LRESULT CScreenGifDlg::ProgressAdd(WPARAM wParam, LPARAM lParam)
{
	m_Progress.StepIt();
	UpdateData(FALSE);
	m_Progress.UpdateWindow();
	m_Progress.RedrawWindow();
	return S_OK;
}


void CScreenGifDlg::OnBnClickedCheck1()
{
	if (m_chkConformSize.GetCheck() == BST_CHECKED) {
		m_blConformSize = true;
		m_cboSize.EnableWindow(FALSE);
	}
	else {
		m_blConformSize = false;
		m_cboSize.EnableWindow(TRUE);
	}
}


void CScreenGifDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));

	CDialogEx::OnLButtonDown(nFlags, point);
}
