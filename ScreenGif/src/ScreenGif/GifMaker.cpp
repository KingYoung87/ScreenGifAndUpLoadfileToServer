#include "stdafx.h"  
#include "GifMaker.h"  
#include <iostream>  
#include "..\ximage\ximagif.h"  
#include <io.h>  

using namespace std;

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

void decoding_gif(string strGifName, string strSavePath)
{
	CxImage img;

	//std::wstring stemp = s2ws(strGifName); // Temporary buffer is required  
	//LPCWSTR PicName = stemp.c_str();
	img.Load(strGifName.c_str(), CXIMAGE_FORMAT_GIF);

	int iNumFrames = img.GetNumFrames();
	cout << "frames num = " << iNumFrames << endl;

	CxImage* newImage = new CxImage();

	for (int i = 0; i < iNumFrames; i++) {
		newImage->SetFrame(i);
		newImage->Load(strGifName.c_str(), CXIMAGE_FORMAT_GIF);

		char tmp[64];
		sprintf(tmp, "%d", i);

		string tmp1;
		tmp1 = tmp1.insert(0, tmp);

		tmp1 = strSavePath + tmp1 + ".png";
		//stemp = s2ws(tmp1); // Temporary buffer is required  
		//PicName = stemp.c_str();
		newImage->Save(tmp1.c_str(), CXIMAGE_FORMAT_PNG);
	}

	if (newImage) delete newImage;
}

int TraverseFolder(const string strFilePath, string strImageNameSets[])
{
	int iImageCount = 0;

	_finddata_t fileInfo;

	long handle = _findfirst(strFilePath.c_str(), &fileInfo);

	if (handle == -1L) {
		cerr << "failed to transfer files" << endl;
		return -1;
	}

	do {
		//cout << fileInfo.name <<endl;  
		strImageNameSets[iImageCount] = (string)fileInfo.name;

		iImageCount++;

	} while (_findnext(handle, &fileInfo) == 0);

	return iImageCount;
}

void encoding_gif(string strImgPath, string strGifName)
{
	string strImgSets[100] = {};

	int iImgCount = TraverseFolder(strImgPath, strImgSets);

	string strTmp = strImgPath.substr(0, strImgPath.find_last_of("/") + 1);

	CxImage** img = new CxImage*[iImgCount];
	if (img == NULL) {
		cout << "new Cximage error!" << endl;
		return;
	}
	//std::wstring stemp;
	//LPCWSTR PicName;
	for (int i = 0; i < iImgCount; i++) {
		string tmp1;
		tmp1 = strTmp + strImgSets[i];
		//stemp = s2ws(tmp1); // Temporary buffer is required  
		//PicName = stemp.c_str();
		img[i] = new CxImage;
		img[i]->Load((strTmp + strImgSets[i]).c_str(), CXIMAGE_FORMAT_BMP);
		//bpp = 1;  bpp = 4;             bpp = 8;  
		if (0 == img[i]->GetNumColors())
		{
			img[i]->DecreaseBpp(8, true);
		}
	}

	CxIOFile hFile;
	//stemp = s2ws(strGifName); // Temporary buffer is required  
	//PicName = stemp.c_str();

	string Method = "wb";
	std::wstring  stempmd = s2ws(Method);
	LPCWSTR wMethod = stempmd.c_str();
	bool BFlag = hFile.Open(s2ws(strGifName).c_str(), wMethod);

	CxImageGIF multiimage;

	multiimage.SetLoops(-1);
	multiimage.SetFrameDelay(300);
	multiimage.SetDisposalMethod(2);
	multiimage.Encode(&hFile, img, iImgCount, false, false);

	hFile.Close();

	delete[] img;
}