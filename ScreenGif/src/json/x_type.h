/**
    @file		x_type.h
    @author  	jinglei
    @version 	0.1
    @brief 		x1����ͨ��ͷ�ļ�
 */

#ifndef X_TYPEH
#define X_TYPEH

#include <string.h>
#include <stdlib.h>

#if (defined(_WIN32) || defined(_WIN64)) && !defined(__WIN__)
#define __WIN__
#endif
#if !defined(__WIN__)
#define STDCALL		/**<  */
#define X_API		/**<  */
#define CALLBACK	/**<  */
#ifndef FALSE		
#define FALSE 0		/**<  */
#endif
#ifndef TRUE		
#define TRUE 1		/**<  */
#endif
#ifndef HWND		
#define HWND void*	/**<  */
#endif
#ifndef HDC
#define HDC void*	/**<  */
#endif
#else
#include <windows.h>
#define STDCALL __stdcall	/**<  */
#define X_API __declspec(dllexport)	/**<  */
#define CALLBACK __stdcall	/**<  */
#define _CRT_SECURE_NO_WARNINGS 1 /**<  */
#if defined(_MSC_VER)
#define snprintf _snprintf	/**<  */
#endif

#endif

#ifndef OUT
#define OUT		/**<  */
#endif

#ifndef IN
#define IN		/**<  */
#endif

#ifndef NULL
#define NULL 0		/**<  */
#endif

/**    
    @name	ͨ�û�����������
    @{
*/
//typedef int BOOL;		/**<  */
#define BOOL int
typedef char int8;		/**<  */
typedef unsigned char uint8;		/**<  */
typedef short int16;		/**<  */
typedef unsigned short uint16;		/**<  */
typedef long int32;		/**<  */
typedef unsigned long uint32;		/**<  */
typedef long long int int64;		/**<  */
typedef unsigned long long uint64;		/**<  */
/**    
    @}
*/

/**
    @name ģ��ID
    @{
*/
#define MODULE_MS			0x80	/**< msģ��ID */
#define MODULE_VS			0x01	/**< vsģ��ID */
#define MODULE_CLIENT_SDK	0x82	/**< clientsdkģ��ID */
#define MODULE_VOD			0x83	/**< vodģ��ID */
#define MODULE_REC			0x84	/**< �洢ģ��ID (¼���ѯ)*/
#define MODULE_DECODER      	0x85	/**< ���������ģ��ID */
#define MODULE_TV       		0x86    	/**< ����ǽ������ģ��ID */
#define MODULE_CONFIG		0x87	/**< ���÷�����ģ��ID */
#define MODULE_NVR			0x88	/**< ����NVRģ��ID (¼�����) */
#define MODULE_GPS			0x02	/**< gps ģ��ID */
#define MODULE_IAS          0x89    /**< ���ܷ���ģ��ID */
#define MODULE_CLOUD         0x90    /**< ���������Ƶ���ģ��ID*/
#define MODULE_UPLOAD        0x91    /**< UPLOADģ��ID */

#define MS_NORMAL_SERVER 0   /**< ��ͨ�м�� */
#define MS_CLOUD_DEV 1        /**< �ƽڵ� */
#define MS_CLOUD_SERVER   2   /**< �Ʒ������� */
#define MS_DECODER_CONTROL   3   /**< ƴ�ط����� */
#define MS_DECODER_UNIT   4   /**< ƴ�ؽ��뵥Ԫ */

/**
    @}
*/

/**
    @name ��Դ����
    @{
*/
#define VIDEO_RES_TYPE			0		/**< ��Ƶ���ӵ���Դ���� */
#define LOGIN_RES_TYPE			1		/**< ������¼����Դ���� */
#define PLAY_RES_TYPE			0x8001	/**< �طŵ���Դ���� */
#define DOWN_RES_TYPE			0x8002	/**< ���ص���Դ���� */
#define STATE_RES_TYPE			0x8003	/**< ��ѯϵͳ��Դ */
#define TALK_RES_TYPE			0x8004	/**< ����Խ���Դ */
#define UP_RES_TYPE				0x8005	/**< �ϴ�����Դ���� */  //add by zsm 2013-07-10
#define UPLOAD2_RES_TYPE		0x8006  /**< UPLOAD �ϴ���Դ */
#define LOGIN_MS_RES_TYPE		0x8002	/**< MS��¼��Դ */
#define LOGIN_VS_RES_TYPE		0x8003	/**< VS��¼��Դ */
#define LOGIN_SDK_RES_TYPE		0x8004	/**< SDK��¼��Դ */
#define LOGIN_VOD_RES_TYPE		0x8005	/**< VOD��¼��Դ */
#define LOGIN_REC_RES_TYPE		0x8006	/**< REC��¼��Դ */
#define LOGIN_DECODER_RES_TYPE	0x8007	/**< DECODER��¼��Դ */
#define LOGIN_TV_RES_TYPE		0x8008	/**< TV��¼��Դ */
#define LOGIN_CONFIG_RES_TYPE	0x8009	/**< CONFIG��¼��Դ */
#define LOGIN_NVR_RES_TYPE		0x800A	/**< NVR��¼��Դ */
#define LOGIN_GPS_RES_TYPE		0x800B	/**< GPS��¼��Դ */
#define LOGIN_IAS_RES_TYPE      0x800C   /**< IAS ��½��Դ */
#define LOGIN_CLOUD_RES_TYPE	0x800D  /**< ClOUD��¼��Դ*/
#define LOGIN_CLOUD_CFG_RES_TYPE	0x800E  /**< ClOUD��¼cfg��Դ*/
#define PUSH_STREAM_RES_TYPE	0x800F  /**< Push�� ��Դ*/

#define VIDEO_TALK_RES_TYPE		6
#define NET_TYPE_VIDEO_TALK		7

/**
    @}
*/

#define TRY_AGIAN_NUM	30	/**< �����ط����� */


#define MAX_IP_LEN 16	/**< ���IP��ַ���� */
//add by shiweiqi 2011-1-28
#define MAX_MAC_LEN 60	/**< ���MAC��ַ���� */
//end add
#define MAX_USERNAME_LEN 60	/**< ����û������� */
#define MAX_PASSWORD_LEN 60	/**< ������볤�� */

#define MAX_REC_TEMPLATE_TIME_NUM 10	/**< ���¼��ģ��ʱ����� */
#define MAX_REC_TEMPLATE_NUM 64	/**< ���¼��ģ���� */
#define MAX_REC_TEMPLATE_NAME_LEN	100	/**< ���¼��ģ�����Ƴ��� */
#define MAX_MONTH 12	/**< ����·ݳ��� */
#define MAX_WEEKDAY 7	/**< ������ڳ��� */
#define MAX_TIME_STRING_LEN 8	/**< ���ʱ�䴮���� */

#define ATM_DVR_REC  3	/**< ATM_DVR¼���ѯ���� */

#ifndef MAX_PATH
#define MAX_PATH 260	/**< ���·������ */


#endif

#endif
