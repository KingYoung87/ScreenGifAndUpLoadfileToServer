#pragma once

#include <afxinet.h>
#include <stdio.h>
#include <string>

#pragma comment(lib,"Wininet.lib")

#define HTTP_POST_GB2312 1 //���ͺͽ��յ���ҳ����
#define HTTP_POST_UTF8  2

/************************************************************
 * �����ƣ�CHttpFileEx                                      *
 * ��  �ã��ṩHTTP�ϴ������ع���                           *
 * ��  �ߣ���Դ������                                       *
 * ��  Ȩ������Ȩ����ԭ��������                             *
 * �����ߣ�����(shenyi0106@163.com) QQ:52851771             *
 * ʱ  �䣺2010-12-21                                       *
 *************************************************************/
/*************************************************************
 * ʹ�÷�����                                                *
 * �ϴ��������ݵ�ͼƬ:                                     *
 *  CHttpFileEx  httpPost;                                   *
 *  httpPost.SetServerParam("www.xx.com","/posttest.asp",80);*
 *  //��ӱ�����                                           *
 *  httpPost.MakePostData("FormName","xxxxx");               *
 *  //���ͼƬ����                                           *
 *  TCHAR  *pRecvData = NULL; //�������ݵ�ָ��               *
 *  DWORD  dwRecvSize = 0;  //�������ݵĴ�С                 *
 *  DWORD  dwRetSend = 0;  //���ͺ�������ֵ                  *
 *  int SendCode = HTTP_POST_GB2312;                         *
 *  int RecvCode =HTTP_POST_GB2312; //��������յı���       *
 *  dwRetSend = httpPost.PostPicDataMethod("c:\\����ͼƬ.jpg"*
 *  ,"filename", &pRecvData,dwRecvSize,SendCode,RecvCode);   *
 *************************************************************/

class CHttpFileEx
{
public:
	CHttpFileEx();
	virtual ~CHttpFileEx();
	void SetServerParam(CString strServerName,CString strServerPath,WORD Port);

	//����Э����������
	BOOL MakePostData(LPCTSTR pPostName, LPCTSTR pPostValue);
	BOOL MakePostData(LPCTSTR pPostName,int iPostValue);

	//���ʹ�ͼƬ������
	DWORD PostPicDataMethod(CString strLocalPath,LPCTSTR strFormName,LPTSTR *pRecvData, DWORD &RecvSize,
		int SendCode = HTTP_POST_GB2312,//���ͺͽ��յı���
		int RecvCode = HTTP_POST_GB2312); 

	//�����ı�������
	DWORD PostDataMethod(LPCTSTR strDataSend, int DataSize, LPTSTR *pRecvData, DWORD &RecvSize,
		int SendCode = HTTP_POST_GB2312,int RecvCode = HTTP_POST_GB2312);

	//����HTTP�ļ���������ɱ����ļ�
	static BOOL DownLoadFile(CString strServerPath,CString strLocalFilePath);
private:
	void MakeHeaderData(); //����Э��ͷ������
	void MakeEndData();  //����Э��β������
	void MakePostPicData(LPCTSTR pPostName, LPCTSTR pPostValue); //����ͼƬǰ׺����
	//����ֵΪͼƬ��С������ֵΪ0��ʾ�����ڡ�
	DWORD GetExistPic(CString strPicPath);

	void WideCharToUTF8(LPCWSTR wpWideData, int WideLen);
	void MultiByteToUTF8(LPCSTR lpMultiData, int MultiLen);
	void UTF8ToWideChar(LPCSTR pUTF8,int UTF8Len);
	void UTF8ToMultiByte(LPCSTR pUTF8, int UTF8Len);
	void MultiToWide(LPCSTR strDataSend, int DataSize);
	void MultiToMulti(LPCSTR strDataSend, int DataSize);
	void WideToMulti(LPCWSTR wpWideData, int WideLen);
private:
	CString   m_strBoundary; //���ݷָ���
	CString   m_strHeaderData; //Э��ͷ����
	CString   m_strPostData; //Э����������
	CString   m_strEndData; //Э��β����

	//����������
	INTERNET_PORT m_nPort; 
	CString   m_strObjectName;
	CString   m_strServerName;
	/////����ת��/////////////////////////////////////////////////////////////
	char * m_lpUTF8;   //UTF8���뻺��ָ��
	char * m_lpMultiByte; //Multil���ֽڱ���ָ��
	wchar_t * m_wpWideChar;  //Widechar���뻺��ָ��
	int  m_nUTF8Len; 
	int  m_nMultiByteLen;
	int  m_nWideCharLen;
};


