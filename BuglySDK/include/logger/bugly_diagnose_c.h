/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_diagnose_c.h
* @author:	hodxiang
* @date:	2025年10月24日 15:40:06 Friday
* @brief:	diagnose C interface
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_DIAGNOSE_C__H__
#define __BUGLY_DIAGNOSE_C__H__

#ifndef BUGLY_WIN_API
  #ifdef BUGLY_WIN_EXPORTS
    #define BUGLY_WIN_API __declspec(dllexport)
  #else
    #define BUGLY_WIN_API __declspec(dllimport)
  #endif
#endif // #ifndef BUGLY_WIN_API

#ifdef  __cplusplus
extern "C" {
#endif  // __cplusplus

/// 上报域名
#define  kBuglyLogServerHostTypeDefalut      0 /// woa正式环境
#define  kBuglyLogServerHostTypeTest         1 /// woa测试环境
#define  kBuglyLogServerHostTypeOversea      2 /// 海外正式环境
#define  kBuglyLogServerHostTypeBuglyPro     3 /// 专业版环境
#define  kBuglyLogServerHostTypeTencentCloud 4 /// 腾讯云环境
#define  kBuglyLogServerHostTypeTencentCloudSgp 5 /// 腾讯云国际站
#define  kBuglyLogServerHostTypeDBugly       6

/**
 * @description 初始化bugly_logger.dll中的诊断模块，后续业务可以使用它下发命令捞取log
 * @param appId, appId
 * @param appKey, appKey
 * @param bundleId, 应用bundleId
 * @param appVersion, 应用版本号
 * @param buglyDiagnoseSdkVersion, 诊断系统sdk的版本号，也就是bugly_logger.dll的版本号
 * @param logDirs, 下发指令捞取日志时扫描的目录，后续还可以使用BuglyAddLogSearchPath接口添加目录,注意：字符串必须要有'\0'结束符
 * @param logDirsNum, 目录数量
 * @param diagnoseConfigDir, 存放诊断系统配置文件的路径，该路径必须可写
 * @param logPubKey, 加密日志的key，如果传NULL则不加密，如果传default则用默认key
 * @param serverHostType, 上报域名
 * @return: 1 为成功，0 为失败
 */
BUGLY_WIN_API int BuglyInitDiagnose(const char* appId, int appId_len,
    const char* appKey, int appKey_len,
    const char* bundleId, int bundleId_len,
    const char* appVersion, int appVersion_len,
    const char* buglyDiagnoseSdkVersion, int buglyDiagnoseSdkVersion_len,
    const char* logDirs[], 
    int logDirsNum, 
    const char* diagnoseConfigDir, int diagnoseConfigDir_len,
    const char* logPubKey, int logPubKey_len,
    int serverHostType);

/** 
* @brief 设置userId，这个填写在捞log的"设备ID"处
* @param userId, 也就时捞log时填的"设备ID"
*/
BUGLY_WIN_API void BuglySetUserId(const char* userId, int userId_len); 

/** 
* @brief 添加日志搜索路径，用于捞取日志时搜索符合条件的日志
* @param logSearchPath, 日志搜索路径
*/
BUGLY_WIN_API void BuglyAddLogSearchPath(const char* logSearchPath, int logSearchPath_len);

/**
 * @prief 设置捞取日志时是否递归搜索日志目录，如果不设置默认为true
 * @param recursive 是否递归搜索addLogPath传入的目录，如果不设置默认为true
 */
BUGLY_WIN_API void BuglySetRecursiveSearchLogs(int recursive);

/** 
 * @brief 主动上报日志接口
 * @param files, 待上报的log文件路径，注意：路径必须带'\0'结束符
 * @param count, 待上报的log文件数量
 * @param tag, 标签，必填，转换成char*后的长度不超过64 bytes
 * @param summary, 摘要信息，选填，转换成char*后的长度不超过256 bytes
 * @param queryKey, 检索Key，选填，用于平台互通检索上报信息
 * @param sourcePlatform, 来源平台，选填，用于平台互通记录上报来源
*/
BUGLY_WIN_API void BuglyUploadLogFiles(const char** files, 
    int count,
    const char* tag, int tag_len,
    const char* summary, int summary_len,
    const char* queryKey, int queryKey_len,
    const char* sourcePlatform, int sourcePlatform_len);

#ifdef  __cplusplus
}
#endif  // __cplusplus

#endif  //!__BUGLY_DIAGNOSE_C__H__