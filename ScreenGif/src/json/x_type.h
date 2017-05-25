/**
    @file		x_type.h
    @author  	jinglei
    @version 	0.1
    @brief 		x1工程通用头文件
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
    @name	通用基本数据类型
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
    @name 模块ID
    @{
*/
#define MODULE_MS			0x80	/**< ms模块ID */
#define MODULE_VS			0x01	/**< vs模块ID */
#define MODULE_CLIENT_SDK	0x82	/**< clientsdk模块ID */
#define MODULE_VOD			0x83	/**< vod模块ID */
#define MODULE_REC			0x84	/**< 存储模块ID (录像查询)*/
#define MODULE_DECODER      	0x85	/**< 解码服务器模块ID */
#define MODULE_TV       		0x86    	/**< 电视墙服务器模块ID */
#define MODULE_CONFIG		0x87	/**< 设置服务器模块ID */
#define MODULE_NVR			0x88	/**< 设置NVR模块ID (录像控制) */
#define MODULE_GPS			0x02	/**< gps 模块ID */
#define MODULE_IAS          0x89    /**< 智能分析模块ID */
#define MODULE_CLOUD         0x90    /**< 基础服务云调度模块ID*/
#define MODULE_UPLOAD        0x91    /**< UPLOAD模块ID */

#define MS_NORMAL_SERVER 0   /**< 普通中间件 */
#define MS_CLOUD_DEV 1        /**< 云节点 */
#define MS_CLOUD_SERVER   2   /**< 云服务器器 */
#define MS_DECODER_CONTROL   3   /**< 拼控服务器 */
#define MS_DECODER_UNIT   4   /**< 拼控解码单元 */

/**
    @}
*/

/**
    @name 资源类型
    @{
*/
#define VIDEO_RES_TYPE			0		/**< 视频连接的资源类型 */
#define LOGIN_RES_TYPE			1		/**< 主机登录的资源类型 */
#define PLAY_RES_TYPE			0x8001	/**< 回放的资源类型 */
#define DOWN_RES_TYPE			0x8002	/**< 下载的资源类型 */
#define STATE_RES_TYPE			0x8003	/**< 查询系统资源 */
#define TALK_RES_TYPE			0x8004	/**< 网络对讲资源 */
#define UP_RES_TYPE				0x8005	/**< 上传的资源类型 */  //add by zsm 2013-07-10
#define UPLOAD2_RES_TYPE		0x8006  /**< UPLOAD 上传资源 */
#define LOGIN_MS_RES_TYPE		0x8002	/**< MS登录资源 */
#define LOGIN_VS_RES_TYPE		0x8003	/**< VS登录资源 */
#define LOGIN_SDK_RES_TYPE		0x8004	/**< SDK登录资源 */
#define LOGIN_VOD_RES_TYPE		0x8005	/**< VOD登录资源 */
#define LOGIN_REC_RES_TYPE		0x8006	/**< REC登录资源 */
#define LOGIN_DECODER_RES_TYPE	0x8007	/**< DECODER登录资源 */
#define LOGIN_TV_RES_TYPE		0x8008	/**< TV登录资源 */
#define LOGIN_CONFIG_RES_TYPE	0x8009	/**< CONFIG登录资源 */
#define LOGIN_NVR_RES_TYPE		0x800A	/**< NVR登录资源 */
#define LOGIN_GPS_RES_TYPE		0x800B	/**< GPS登录资源 */
#define LOGIN_IAS_RES_TYPE      0x800C   /**< IAS 登陆资源 */
#define LOGIN_CLOUD_RES_TYPE	0x800D  /**< ClOUD登录资源*/
#define LOGIN_CLOUD_CFG_RES_TYPE	0x800E  /**< ClOUD登录cfg资源*/
#define PUSH_STREAM_RES_TYPE	0x800F  /**< Push流 资源*/

#define VIDEO_TALK_RES_TYPE		6
#define NET_TYPE_VIDEO_TALK		7

/**
    @}
*/

#define TRY_AGIAN_NUM	30	/**< 发送重发次数 */


#define MAX_IP_LEN 16	/**< 最大IP地址长度 */
//add by shiweiqi 2011-1-28
#define MAX_MAC_LEN 60	/**< 最大MAC地址长度 */
//end add
#define MAX_USERNAME_LEN 60	/**< 最大用户名长度 */
#define MAX_PASSWORD_LEN 60	/**< 最大密码长度 */

#define MAX_REC_TEMPLATE_TIME_NUM 10	/**< 最大录像模板时间段数 */
#define MAX_REC_TEMPLATE_NUM 64	/**< 最大录像模板数 */
#define MAX_REC_TEMPLATE_NAME_LEN	100	/**< 最大录像模板名称长度 */
#define MAX_MONTH 12	/**< 最大月份长度 */
#define MAX_WEEKDAY 7	/**< 最大星期长度 */
#define MAX_TIME_STRING_LEN 8	/**< 最大时间串长度 */

#define ATM_DVR_REC  3	/**< ATM_DVR录像查询类型 */

#ifndef MAX_PATH
#define MAX_PATH 260	/**< 最大路径长度 */


#endif

#endif
