
// ScreenGifDlg.h : 头文件
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
// 构造
public:
	CScreenGifDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CScreenGifDlg();

// 对话框数据
	enum { IDD = IDD_SCREENGIF_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	// 矩形工具
	afx_msg void OnBnRectangle();
	// 录制Gif图片按钮
	afx_msg void OnBnClickedStart();
	// 分享按钮
	afx_msg void OnBnClickedShare();
	// 完成按钮
	afx_msg void OnBnClickedGetPic();
	// 保存按钮
	afx_msg void OnBnClickedSave();
	// 按钮tootip
	afx_msg BOOL OnToolTipText(UINT NID, NMHDR* pNMHDR, LRESULT* pResult);
	// 全局快捷键
	afx_msg long OnHotKey(WPARAM wParam, LPARAM lParam);
	// 帮助
	afx_msg void OnHelp();

public:
	// 截屏起点
	afx_msg LRESULT OnBeginPoint(WPARAM wParam, LPARAM lParam);
	// 截屏终点
	afx_msg LRESULT OnEndPoint(WPARAM wParam, LPARAM lParam);
	// 鼠标移动时的坐标
	afx_msg LRESULT OnMovePoint(WPARAM wParam, LPARAM lParam);
	// 截图快捷键消息
	afx_msg LRESULT OnScreenPic(WPARAM wParam, LPARAM lParam);
	// 定时器
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 改变对话框背景颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 托盘图标通知消息
	afx_msg LRESULT OnNotifyMsg(WPARAM wparam, LPARAM lparam);
	// 取消按钮
	afx_msg void OnBnClickedCancel();
	// 画圆工具
	afx_msg void OnBnClickedButtonCircle();
	// 箭头工具
	afx_msg void OnBnClickedButtonArrow();
	// 文字工具
	afx_msg void OnBnClickedButtonA();
	// 撤销
	afx_msg void OnBnClickedButtonBack();


public:
	// 截图
	bool GetPic();
	// 保存bitmap图片
	void SaveBitmapToFile(CBitmap* pBitmap, CString fileName);
	// 制作gif图片
	void MakeGif();
	// 添加工具栏
	void AddToolbar();
	// 指定区域
	void GetRect();
	//上传服务器
	void UploadFile();
	//叠加水印
	void MakeWatermark(string &_path, string &_fileName);

	afx_msg LRESULT ProgressAdd(WPARAM wParam, LPARAM lParam);

public:
	RECT m_rc;						//屏幕截图坐标
	RECT m_AllScreen;				//全屏尺寸
	CRegionDlg* m_pRegionDlg;		//选取框类
	CAllScreenDlg* m_pAllScreenDlg; //屏幕对话框
	CDC m_srcDc;					//屏幕DC
	CDC m_dcCompatible;				//兼容DC
	CBitmap m_mapCompatible;		//兼容位图
	int m_index;					//bmp图编号
	bool m_bIsReadyGif;				//是否可以录制Gif
	WORD m_wGifbeginSize;			//Gif文件开始到图形控制扩展块的大小
	WORD m_wGifDataSize;			//Gif文件图像数据部分的大小
	CString m_strPath;				//pic文件路径
	CString m_strCurentGif;			//当前制作的gif
	CBrush m_brush;					//对话框背景颜色
	CToolBar m_toolbar;				//工具栏
	CImageList m_toolbarlist;		//图标链表
	NOTIFYICONDATA m_notify;		//托盘数据结构体
	CToolTipCtrl m_tooltip;			//工具栏tooltip
	CProgressCtrl m_Progress;		//进度条
	CPicProcess* m_process;			//画图区窗口
	CPicMap* m_map;					//画布
	PEN m_pen;						//画笔属性
	CBrush m_rcbrush;				//画刷（绘制矩形时用）
	CEdit m_edtGifAddress;

	int m_iFrameNum;				//截取帧数
	bool m_blConformSize;			//是否固定大小
	BOOL m_blEnableStart;			//是否可以开始
	int m_iProgressCount;
	int m_iDstWidth;				//裁剪后的宽度
	int m_iDstHeight;				//裁剪后的高度
	double m_fSizeNumber;				//缩放值
	CComboBox m_cboSize;
	afx_msg void OnCbnSelchangeCombo1();
	CStatic m_stcFrame;
	CButton m_chkConformSize;
	afx_msg void OnBnClickedCheck1();
	CStatic m_stcArea;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
