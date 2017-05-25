#pragma once
#define DELAYTIME 0x0500				//��ʱʱ��0.05��

//gif�ļ�ͷ
typedef struct gifheader	//6
{
	BYTE m_Signature[3];	//GIF����� ��GIF��
	BYTE m_Version[3];		//GIF�汾�� ��87a����89a��
} GIFHEADER;

//�߼���Ļ������
typedef struct gifscrdesc	//7
{
	WORD m_wWidth;	//�߼���Ļ���
	WORD m_wDepth;	//�߼���Ļ�߶�
	struct globalflag	//��װ��
	{				
		BYTE PalBits : 3;		//ȫ����ɫ���С
		BYTE SortFlag : 1;		//ȫ����ɫ������־�������λ1��ʾȫ����ɫ�б��������
		BYTE ColorRes : 3;		//��ɫ��ȣ�cr+1ȷ����ɫ���
		BYTE GlobalPal : 1;		//ȫ����ɫ�б��־����λ��ʾ��ȫ����ɫ�б�pixelֵ������
	} m_GloabalFlag;
	BYTE m_byBackground;		//������ɫ����ȫ����ɫ�б��е����������û��ȫ����ɫ�б���ֵû�����壩
	BYTE m_byAspect;			//���ؿ�߱�
} GIFSCRDESC;

//ͼ��������
typedef struct gifimage	//10
{
	BYTE m_byBegin; //ͼ���ʶ����ʼ���̶�ֵΪ������
	WORD m_wLeft;	//X�᷽��ƫ����
	WORD m_wTop;	//Y�᷽��ƫ����
	WORD m_wWidth;	//ͼ����
	WORD m_wDepth;	//ͼ��߶�
	struct localflag
	{
		BYTE PalBits : 3;	//�ֲ���ɫ�б��С��pixel+1Ϊ��ɫ�б��λ��
		BYTE Reserved : 2;	//�����������ʼ��Ϊ0
		BYTE SortFlag : 1;	//�����־�������λ��ʾ�����ŵľֲ���ɫ�б��������
		BYTE Interlace : 1;	//��֯��־����λʱͼ������ʹ�ý�֯��ʽ���У�����ʹ��˳������
		BYTE LocalPal : 1;	//�ֲ���ɫ�б��־����λʱ��ʾ��������ͼ���ʶ��������һ���ֲ���ɫ�б�
	} m_LocalFlag;
} GIFIMAGE;
//ͼ�ο�����չ��
typedef struct gifcontrol
{
	BYTE m_byExtensionIntroducer;	//��ʶ����һ����չ�飬�̶�ֵΪ0x21
	BYTE m_byGraphicControlLabel;	//ͼ�ο�����չ��ǩ����ʶ����һ��ͼ�ο�����չ�飬�̶�ֵ0xF9
	BYTE m_byBlaockSize;			//���С���������ս������̶�ֵ4
	struct flag
	{
		//BYTE Transparency : 1;		//͸����ɫ��־����λʱ��ʾʹ��͸����ɫ
		BYTE UserInput : 1;			//�û������־��ָ�Ƿ��ڴ��û�����֮�����������ȥ��λ��ʾ�ڴ��û����룬�÷��ʾ���ڴ��û�����
		BYTE DisposalMethod : 3;
			//0:��ʹ�ô��÷�ʽ
			//1:������ͼ�Σ���ͼ�δӵ�ǰλ����ȥ
			//2:�ظ�������ɫ
			//3:�ظ�����ǰ״̬
			//4-7:�Զ���
		BYTE Reserved : 3;			//����
	} m_Flag;
	WORD m_wDelayTime;				//�ӳ�ʱ�䣬��λ0.01��
	BYTE m_byTransparencyIndex;		//͸����ɫ����
	BYTE m_byTeminator;				//���ս���
} GIFCONTROL;


typedef struct pen
{
	int m_style;		//������ʽ
	int m_width;		//���ʿ��
	COLORREF m_color;	//������ɫ
} PEN;


class CGif
{
public:
	CGif();
	~CGif();
public:
	// �õ�gif����
	static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	// bmpͼƬת��GifͼƬ
	static bool GetGifPic(CString strBmpPathName, CString strGifPathName);
	// �ϲ�����gif���ɶ�̬ͼ
	static void CombineGif(CFile* pFile, CString& strgifName, WORD beginSize);
};

