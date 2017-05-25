#include "stdafx.h"
#include "HttpFileEx.h"


CHttpFileEx::CHttpFileEx()
{
	m_lpUTF8 = NULL;		//UTF8���뻺��ָ��
	m_lpMultiByte = NULL;		//
	m_wpWideChar = NULL;		//Widechar���뻺��ָ��
	//������Ĭ��ֵ
	m_strServerName = "";
	m_strObjectName = "";
	m_nPort = INTERNET_DEFAULT_HTTP_PORT;
	m_strBoundary = _T("-------------2389jhrwfjsjd9823");
}

CHttpFileEx::~CHttpFileEx()
{
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpUTF8)
	{
		delete [] m_lpUTF8;
		m_lpUTF8 = NULL;
	}
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpMultiByte)
	{
		delete [] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}
	//���ֽڴ���ָ��
	if (m_wpWideChar)
	{
		delete []m_wpWideChar;
		m_wpWideChar = NULL;
	}
}

//���÷���������
void CHttpFileEx::SetServerParam(CString strServerName, CString strObjectName, WORD Port)
{
	m_strServerName = strServerName;
	m_strObjectName = strObjectName;
	m_nPort = Port;
}

//��������
//���������1��ʾGB2312��2��ʾUTF8
//���ش�������б�
//100�������ɹ�
//101���������޷�����
//102���ύҳ���޷���
//103�����ݷ���ʧ��
//104������������ʧ��
//500���쳣����
DWORD CHttpFileEx::PostDataMethod(LPCTSTR strDataSend, int DataSize, LPTSTR *pRecvData, DWORD &RecvSize,int SendCode,int RecvCode)	
{
	CInternetSession	InternetSession;
	CHttpConnection		*pHttp = NULL;
	CHttpFile			*pFile = NULL;
	DWORD				nRetCode	= 100;	//����ֵ
	TCHAR				tzErrMsg[MAX_PATH] = {0};	//	��׽�쳣
	CString				strResponse;	//���յ�����

	try
	{
		pHttp = InternetSession.GetHttpConnection(m_strServerName,m_nPort);
		if (pHttp == NULL)
		{
			return 101;
		}
		pFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_POST,m_strObjectName,NULL);
		if (pFile == NULL)
		{
			return 102;
		}
		CString	strHeader = _T("Content-Type: application/x-www-form-urlencoded");
		//////////////////////////////////////////////////////////////////////////
		//����ʱ����GB2312
		if (SendCode == 1)
		{
#ifdef UNICODE
			WideToMulti((LPCWSTR)(LPCTSTR)strDataSend,DataSize);
#else
			MultiToMulti((LPCSTR)(LPCTSTR)strDataSend,DataSize);				
#endif
			//����
			if (pFile->SendRequest(strHeader,m_lpMultiByte,m_nMultiByteLen) == FALSE)
			{
				return 103;
			}
		}
		//����ʱѡ��UTF8
		else if (SendCode == 2)
		{
#ifdef UNICODE
			WideCharToUTF8((LPCWSTR)(LPCTSTR)strDataSend,DataSize);
#else
			MultiByteToUTF8((LPCSTR)(LPCTSTR)strDataSend,DataSize);
#endif
			//����
			if (pFile->SendRequest(strHeader,m_lpUTF8,m_nUTF8Len) == FALSE)
			{
				return 103;
			}
		}

		//��ѯ״̬
		DWORD	dwStatusCode = 0;
		pFile->QueryInfoStatusCode(dwStatusCode);
		if (dwStatusCode != 200)
		{
			return 104;
		}

		//////////////////////////////////////////////////////////////////////////
		//��������
		char	szRead[1024] = {0};
		while((pFile->Read(szRead,1023)) > 0)
		{
			strResponse += szRead;
		}
		//ת�����յ�����
		//���ղ���GB2312����
		if (RecvCode == 1)
		{
#ifdef UNICODE
			MultiToWide((LPCSTR)(LPCTSTR)strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			MultiToMulti(strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//���ղ���UTF8����
		else if (RecvCode == 2)
		{
#ifdef UNICODE
			UTF8ToWideChar((LPCSTR)(LPCTSTR)strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			UTF8ToMultiByte(strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//����ֵ
		nRetCode = 100;
	}
	//�����쳣
	catch (CInternetException* e)
	{
		e->GetErrorMessage(tzErrMsg,sizeof(tzErrMsg));
		e->Delete();
		nRetCode = 500;
	}

	//////////////////////////////////////////////////////////////////////////
	//�ر�
	pFile->Close();
	delete	pFile;
	pHttp->Close();
	delete pHttp;
	InternetSession.Close();

	return nRetCode;
}
//�ύͼƬ���Լ��������ݰ�
//99��ͼƬ������
//100�������ɹ�
//101���������޷�����
//102���ύҳ���޷���
//103�����ݷ���ʧ��
//104������������ʧ��
//500���쳣����
DWORD CHttpFileEx::PostPicDataMethod(CString strLocalPath,LPCTSTR strFormName,LPTSTR *pRecvData, DWORD &RecvSize,int SendCode ,int RecvCode)
{
	DWORD	dwRetCode = 100;
	DWORD	dwPostSize = 0;		//Ҫ���͵����ݴ�С
	DWORD	dwPicSize	= 0;	//ͼƬ��С
	DWORD	dwPostEndSize = 0;	//Э��β�����ݳ���

	CInternetSession	InternetSession;
	CHttpConnection		*pHttp = NULL;
	CHttpFile			*pFile = NULL;
	TCHAR				tzErrMsg[MAX_PATH] = {0};	//	��׽�쳣
	CString				strResponse;	//���յ�����

	//��ȡͼƬ��С
	dwPicSize = GetExistPic(strLocalPath);

	//�������ݰ�
	MakeHeaderData();
	MakePostPicData(strFormName,strLocalPath);	//��ԭ�����ݰ��������ͼƬǰ׺��
	MakeEndData();
	//���ݰ��������

	try
	{
		pHttp = InternetSession.GetHttpConnection(m_strServerName,m_nPort);
		if (pHttp == NULL)
		{
			return 101;
		}
		pFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_POST,m_strObjectName,NULL);
		if (pFile == NULL)
		{
			return 102;
		}
		pFile->AddRequestHeaders(m_strHeaderData);
		//////////////////////////////////////////////////////////////////////////
		//����ʱ����GB2312
		if (SendCode == 1)
		{
#ifdef UNICODE
			WideToMulti((LPCWSTR)(LPCTSTR)m_strPostData,m_strPostData.GetLength());
			//����β�����ݳ���
			dwPostEndSize = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength(),NULL,0,NULL,NULL);
#else
			MultiToMulti((LPCSTR)(LPCTSTR)m_strPostData,m_strPostData.GetLength());		
			dwPostEndSize = m_strEndData.GetLength();
#endif
			//����Э��β��ֻ��Ӣ�����ݣ���unicode��UTF8��ASCII�����£�
			//Ӣ������ת���󳤶Ȳ��䣬��˿�ֱ�Ӽ��ϳ��ȡ�
			//��ȡβ������תΪUTF8��ĳ���
			dwPostSize = dwPicSize + m_nMultiByteLen + dwPostEndSize;
			//���ͣ���֪�����ܳ���
			pFile->SendRequestEx(dwPostSize);
			pFile->Write(m_lpMultiByte,m_nMultiByteLen);
		}
		//����ʱѡ��UTF8
		else if (SendCode == 2)
		{
#ifdef UNICODE
			WideCharToUTF8((LPCWSTR)(LPCTSTR)m_strPostData,m_strPostData.GetLength());
			dwPostEndSize = WideCharToMultiByte(CP_UTF8,0,(LPCWSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength(),NULL,0,NULL,NULL);
#else
			//����鷳��Ҫת������ֱ���ú���ת���ˡ�
			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength());
			dwPostEndSize = m_nUTF8Len;

			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strPostData,m_strPostData.GetLength());
#endif
			dwPostSize = dwPicSize + m_nUTF8Len + dwPostEndSize;
			//����
			pFile->SendRequestEx(dwPostSize);
			pFile->Write(m_lpUTF8,m_nUTF8Len);
		}
		//////////////////////////////////////////////////////////////////////////
		//����ͼƬ
		//�����С
		int	BuffSize = 8 * 1024;
		if (dwPicSize != 0)
		{
			char *pbuff = new char[BuffSize];
			CFile	cFile;
			UINT	ReadLength = -1;

			if (pbuff != NULL)
			{
				if (cFile.Open(strLocalPath,CFile::modeRead|CFile::shareDenyWrite,NULL))
				{
					while(ReadLength !=0)
					{
						ReadLength = cFile.Read(pbuff,BuffSize);
						if (ReadLength != 0)
						{
							pFile->Write(pbuff,ReadLength);
						}
					}

					cFile.Close();
				}
				//ɾ����
				delete []pbuff;
				pbuff = NULL;
			}

		}
		//////////////////////////////////////////////////////////////////////////
		//����Э��β��
		//����ʱ����GB2312
		if (SendCode == 1)
		{
#ifdef UNICODE
			WideToMulti((LPCWSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength());
#else
			MultiToMulti((LPCSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength());				
#endif
			pFile->Write(m_lpMultiByte,m_nMultiByteLen);
		}
		//����ʱѡ��UTF8
		else if (SendCode == 2)
		{
#ifdef UNICODE
			WideCharToUTF8((LPCWSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength());
#else
			MultiByteToUTF8((LPCSTR)(LPCTSTR)m_strEndData,m_strEndData.GetLength());
#endif
			pFile->Write(m_lpUTF8,m_nUTF8Len);
		}


		//�������
		pFile->EndRequest(HSR_SYNC);
		//��ѯ״̬
		DWORD	dwStatusCode = 0;
		pFile->QueryInfoStatusCode(dwStatusCode);
		if (dwStatusCode != 200)
		{
			return 104;
		}

		//////////////////////////////////////////////////////////////////////////
		//��������
		char	szRead[1024] = {0};
		while((pFile->Read(szRead,1023)) > 0)
		{
			strResponse += szRead;
		}
		//ת�����յ�����
		//���ղ���GB2312����
		if (RecvCode == 1)
		{
#ifdef UNICODE
			MultiToWide((LPCSTR)(LPCTSTR)strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			MultiToMulti(strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//���ղ���UTF8����
		else if (RecvCode == 2)
		{
#ifdef UNICODE
			UTF8ToWideChar((LPCSTR)(LPCTSTR)strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_wpWideChar;
			RecvSize = m_nWideCharLen;
#else
			UTF8ToMultiByte(strResponse,strResponse.GetLength());
			*pRecvData = (LPTSTR)m_lpMultiByte;
			RecvSize = m_nMultiByteLen;
#endif
		}
		//����ֵ
		dwRetCode = 100;
	}
	//�����쳣
	catch (CInternetException* e)
	{
		e->GetErrorMessage(tzErrMsg,sizeof(tzErrMsg));
		e->Delete();
		dwRetCode = 500;
	}


	//////////////////////////////////////////////////////////////////////////
	//�ر�
	pFile->Close();
	delete	pFile;
	pHttp->Close();
	delete pHttp;
	InternetSession.Close();
	return dwRetCode;
}
//����ֵΪͼƬ��С������ֵΪ0��ʾ�����ڡ�
DWORD CHttpFileEx::GetExistPic(CString strPicPath)
{
	DWORD	dwRetCode = 0;
	WIN32_FIND_DATA	FindData;
	HANDLE	hFile = INVALID_HANDLE_VALUE;

	hFile = FindFirstFile(strPicPath,&FindData);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		dwRetCode = 0;
	}
	else
	{
		dwRetCode = FindData.nFileSizeLow;
		FindClose(hFile);
		hFile = INVALID_HANDLE_VALUE;
	}
	return dwRetCode;
}
//UTF-8תΪ���ַ�
void CHttpFileEx::UTF8ToWideChar(LPCSTR pUTF8,int UTF8Len)
{
	if (m_wpWideChar)
	{
		delete [] m_wpWideChar;
		m_wpWideChar = NULL;
	}
	
	m_nWideCharLen = MultiByteToWideChar(CP_UTF8,0,pUTF8,UTF8Len,NULL,0);
	
	m_wpWideChar = new WCHAR[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;
	
	MultiByteToWideChar(CP_UTF8,0,pUTF8,UTF8Len,m_wpWideChar,m_nWideCharLen);
	
}
//��UTF8תΪ���ֽ�
void CHttpFileEx::UTF8ToMultiByte(LPCSTR pUTF8, int UTF8Len)
{
	//���ת��ҲҪ���������Ƚ�UTF8תΪ���ַ���
	//�ٽ����ַ�תΪ���ֽڡ�
	if (m_lpMultiByte)
	{
		delete [] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}
	//UTF8ת���ַ�
	UTF8ToWideChar(pUTF8,UTF8Len);
	//���ַ�תΪ���ֽ�
	m_nMultiByteLen = WideCharToMultiByte(CP_ACP,0,m_wpWideChar,m_nWideCharLen,NULL,0,NULL,NULL);
	m_lpMultiByte = new char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = 0;
	
	WideCharToMultiByte(CP_UTF8,0,m_wpWideChar,m_nWideCharLen,m_lpMultiByte,m_nMultiByteLen,NULL,NULL);
}
//�����ַ�תΪUTF-8
void CHttpFileEx::WideCharToUTF8(LPCWSTR wpWideData, int WideLen)
{
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpUTF8)
	{
		delete [] m_lpUTF8;
		m_lpUTF8 = NULL;
	}
	
	m_nUTF8Len = WideCharToMultiByte(CP_UTF8,0,wpWideData,WideLen,NULL,0,NULL,NULL);
	
	m_lpUTF8 = new char[m_nUTF8Len + 1];
	m_lpUTF8[m_nUTF8Len] = 0;
	
	WideCharToMultiByte(CP_UTF8,0,wpWideData,WideLen,m_lpUTF8,m_nUTF8Len,NULL,NULL);
}


//�����ֽ�תΪUTF8
void CHttpFileEx::MultiByteToUTF8(LPCSTR lpMultiData, int MultiLen)
{
	//���ת��Ҫ�鷳�㣬Ҫ�Ƚ����ֽ�תΪ���ַ����ٽ����ַ�תΪUTF8
	//��ʵ���ȫ����Ӣ�ĵĻ�����û�б�Ҫ����ת���ˣ���������ģ��ͱ���ת��
	//���ԭ�ȵ�����û��ɾ��
	if (m_lpUTF8)
	{
		delete [] m_lpUTF8;
		m_lpUTF8 = NULL;
	}
	if (m_wpWideChar)
	{
		delete [] m_wpWideChar;
		m_wpWideChar = NULL;
	}
	//��תΪ���ַ�	
	m_nWideCharLen = MultiByteToWideChar(CP_ACP,0,lpMultiData,MultiLen,NULL,0);
	
	m_wpWideChar = new WCHAR[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;
	
	MultiByteToWideChar(CP_ACP,0,lpMultiData,MultiLen,m_wpWideChar,m_nWideCharLen);
	//�ٽ����ַ�תΪUTF8
	WideCharToUTF8(m_wpWideChar,m_nWideCharLen);
}

void CHttpFileEx::WideToMulti(LPCWSTR wpWideData, int WideLen)
{
	if (m_lpMultiByte)
	{
		delete [] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}

	m_nMultiByteLen = WideCharToMultiByte(CP_ACP,0,wpWideData,WideLen,NULL,0,NULL,NULL);
	m_lpMultiByte = new char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = '\0';

	WideCharToMultiByte(CP_ACP,0,wpWideData,WideLen,m_lpMultiByte,m_nMultiByteLen,NULL,NULL);
}

void CHttpFileEx::MultiToMulti(LPCSTR strDataSend, int DataSize)
{
	if (m_lpMultiByte)
	{
		delete [] m_lpMultiByte;
		m_lpMultiByte = NULL;
	}
	
	m_nMultiByteLen = DataSize;
	m_lpMultiByte = new	char[m_nMultiByteLen + 1];
	m_lpMultiByte[m_nMultiByteLen] = '\0';

	memcpy(m_lpMultiByte,(LPCSTR)strDataSend,m_nMultiByteLen);
}

void CHttpFileEx::MultiToWide(LPCSTR strDataSend, int DataSize)
{
	if (m_wpWideChar)
	{
		delete [] m_wpWideChar;
		m_wpWideChar = NULL;
	}

	m_nWideCharLen = MultiByteToWideChar(CP_ACP,0,strDataSend,DataSize,NULL,0);

	m_wpWideChar = new wchar_t[m_nWideCharLen + 1];
	m_wpWideChar[m_nWideCharLen] = 0;

	MultiByteToWideChar(CP_ACP,0,strDataSend,DataSize,m_wpWideChar,m_nWideCharLen);
}

//����ͷ����
void CHttpFileEx::MakeHeaderData()
{
	 CString strTemp = /*_T("POST /adminlilu/Include/UpFileSave.asp HTTP/1.1\r\n")*/
		_T("Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/xaml+xml,*/*\r\n")
		_T("Accept-Encoding: gzip, deflate\r\n")
		_T("Accept-Language: zh-cn\r\n")
		_T("Content-Type: multipart/form-data; boundary=%s\r\n")
		_T("Cache-Control: no-cache\r\n\r\n");

		/*_T("Host: %s:%d\r\n")*/
	 m_strHeaderData.Format(strTemp,m_strBoundary);
}
//���췢�͵����ݰ�
BOOL CHttpFileEx::MakePostData(LPCTSTR strFormName, LPCTSTR pPostValue)
{
	if (strFormName == NULL)
	{
		return FALSE;
	}

	CString	strTemp;
	strTemp.Format(_T("--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n%s\r\n"),
		m_strBoundary,strFormName,pPostValue);
	//����һ���µ����ݰ�
	m_strPostData += strTemp;
	
	return TRUE;
}
//����Э����������
BOOL CHttpFileEx::MakePostData(LPCTSTR strFormName, int iPostValue)
{
	if (strFormName == NULL)
	{
		return FALSE;
	}

	CString	strTemp;
	strTemp.Format(_T("--%s\r\nContent-Disposition: form-data; name=\"%s\"\r\n\r\n%d\r\n"),
		m_strBoundary,strFormName,iPostValue);
	//����һ���µ����ݰ�
	m_strPostData += strTemp;

	return TRUE;
}
//����ͼƬ����ǰ׺��
void CHttpFileEx::MakePostPicData(LPCTSTR strFormName, LPCTSTR pPostValue)
{
	if (strFormName == NULL)
	{
		return ;
	}

	CString	strTemp;
	CString	strPicPath(pPostValue);	//
	CString	strExtName;	//��չ��
	strTemp.Format(_T("--%s\r\nContent-Disposition: form-data; name=\"%s\"; filename=\"%s\"\r\n"),
		m_strBoundary,strFormName,pPostValue);
	//������չ���趨content-type
	strPicPath.MakeUpper();
	strExtName = strPicPath.Right(strPicPath.GetLength() - strPicPath.ReverseFind(_T('.')) - 1);

	if (strExtName == _T("JPG") || strExtName == _T("JPEG"))
	{
		strTemp += _T("Content-Type: image/pjpeg\r\n\r\n");
	}
	else if (strExtName == _T("GIF"))
	{
		strTemp += _T("Content-Type: image/gif\r\n\r\n");
	}
	else if (strExtName == _T("PNG"))
	{
		strTemp += _T("Content-Type: image/x-png\r\n\r\n");
	}
	//������������
	else
	{
		strTemp += _T("Content-Type: application/octet-stream\r\n\r\n");
	}
	//����һ���µ����ݰ�
	m_strPostData += strTemp;
	
	return;
}
//�����������
void CHttpFileEx::MakeEndData()
{
	CString strFormat;

	strFormat += _T("\r\n--%s\r\nContent-Disposition: form-data; name=\"Submit\"\r\n\r\nsubmit\r\n--%s--");
	
	m_strEndData.Format(strFormat,m_strBoundary, m_strBoundary);
}

//����HTTP�ļ���������ɱ����ļ�
BOOL CHttpFileEx::DownLoadFile(CString strServerPath,CString strLocalFilePath)
{
	if(strServerPath.IsEmpty())return FALSE;

	CString FilePath = strServerPath;
	DWORD dwSize = NULL;

	CFile* httpFile = NULL;
	char * pData = NULL;
	UINT   uCurrent = 0,uTotal=0;
	CFile  writeFile;
	CInternetSession session(_T("HttpClient"));
	try
	{
		//���������ļ�
		if(!writeFile.Open(strLocalFilePath,CFile::modeCreate | CFile::modeReadWrite))
			return FALSE;

		httpFile = (CHttpFile*)session.OpenURL(strServerPath);
		//���ʧ�ܣ��򷵻�FALSE
		if(httpFile == INVALID_HANDLE_VALUE)
		{
			writeFile.Close();
			return FALSE;
		}
        //��ȡ�ļ��ֽ���
		dwSize = (DWORD)httpFile->GetLength();

		pData = (char *)malloc(8 * 1024);
		memset(pData,0,8 * 1024);
		//������
		while(1)
		{
			//��ȥ����
			uCurrent = httpFile->Read(pData,8 * 1024);
			//д�����ļ�
			writeFile.Write(pData,uCurrent);
			//�����Ѿ�д������ݴ�С
			uTotal = uTotal + uCurrent;
			if(uTotal >= dwSize) break;
		}
		free(pData);pData = NULL;
		writeFile.Close();
		//ɾ���ļ�
		httpFile->Close();
		delete httpFile;

		return TRUE;
	}
	catch(CInternetException *e)
	{
		if(httpFile!=NULL)
		{
			httpFile->Close();
			delete httpFile;
		}
		e->Delete();
		return FALSE;
	}
}




