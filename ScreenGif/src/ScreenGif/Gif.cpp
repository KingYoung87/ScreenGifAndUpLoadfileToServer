#include "stdafx.h"
#include "Gif.h"
#include <gdiplus.h>  
#include <stdio.h>  
using namespace Gdiplus;
#pragma comment(lib,"gdiplus")  

CGif::CGif()
{
}


CGif::~CGif()
{
}

void CGif::CombineGif(CFile* pFile, CString& strgifName, WORD beginSize)
{
	pFile->Seek(-1, CFile::current);
	CFile gif;
	gif.Open(strgifName, CFile::modeRead | CFile::typeBinary);
	BYTE byte = 0x00;
	BYTE* pbuf = new BYTE[beginSize];
	BYTE Databuf[10240];

	gif.Read(pbuf, beginSize);
	byte = 0x21;
	pFile->Write(&byte, 1);
	GIFCONTROL gifcontrol;
	gif.Read(&gifcontrol, sizeof(GIFCONTROL));
	gifcontrol.m_wDelayTime = DELAYTIME;
	pFile->Write(&gifcontrol, sizeof(GIFCONTROL));
	WORD wsize = 0;
	while ((wsize = gif.Read(Databuf, sizeof(Databuf))) == sizeof(Databuf))
	{
		pFile->Write(Databuf, wsize);
	}
	pFile->Write(Databuf, wsize);
	delete[] pbuf;
	gif.Close();
}

int CGif::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders  
	UINT  size = 0;         // size of the image encoder array in bytes  

	ImageCodecInfo* pImageCodecInfo = NULL;

	//1.��ȡGDI+֧�ֵ�ͼ���ʽ�������������Լ�ImageCodecInfo����Ĵ�Ŵ�С  
	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure  

	//2.ΪImageCodecInfo����������ռ�  
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure  

	//3.��ȡ���е�ͼ���������Ϣ  
	GetImageEncoders(num, size, pImageCodecInfo);

	//4.���ҷ��ϵ�ͼ���������Clsid  
	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success  
		}
	}

	//5.�ͷŲ���3������ڴ�  
	free(pImageCodecInfo);
	return -1;  // Failure  
}

bool CGif::GetGifPic(CString strBmpPathName, CString strGifPathName)
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	//1.��ʼ��GDI+���Ա������GDI+�������Գɹ�����  
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	CLSID   encoderClsid;
	Status  stat;
	//2.����Image���󲢼���ͼƬ  
	Image*   image = new Image(strBmpPathName);
	//3. Get the CLSID of the gif encoder.  
	GetEncoderClsid(L"image/gif", &encoderClsid);
	//4.����Image.Save��������ͼƬ��ʽת�������Ѳ���3)�õ���ͼ�������Clsid���ݸ���  
	stat = image->Save(strGifPathName, &encoderClsid, NULL);
	if (stat == Ok)
		//5.�ͷ�Image����  
		delete image;
	//6.��������GDI+��Դ  
	GdiplusShutdown(gdiplusToken);
	if (stat == Ok)
	{
		return true;
	}
	else
	{
		return false;
	}
}