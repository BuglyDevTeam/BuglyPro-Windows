/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_diagnose.h
* @author:	hodxiang
* @date:	2024年08月26日 21:50:10 星期一
* @brief:	bugly 诊断系统
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_DIAGNOSE__H__
#define __BUGLY_DIAGNOSE__H__

#include <cstdint>
#include "diagnose/log_common_def.h"

namespace Bugly {
namespace Diagnose {

class __declspec(dllexport) BuglyDiagnose {
 public:
  /// @brief 初始化诊断系统
  /// @param appId appId
  /// @param appKey appKey
  /// @param appBundleId bundleId
  /// @param appVersion app version
  /// @param buglyDiagnoseSdkVersion diagnose sdk version
  /// @param logDirs 日志文件路径，在收到捞取日志的指令时会去这些目录下寻找
  /// @param logDirsNum, 日志文件路径个数
  /// @param diagnoseConfigDir 诊断系统配置文件路径，用于保存相关配置，不能和上面的logDir放在一起，以免清理日志时把配置文件清理掉
  /// @param logPubKey 日志加密的pubkey
  static void initDiagnose(const wchar_t* appId,
                      const wchar_t* appKey,
                      const wchar_t* appBundleId,
                      const wchar_t* appVersion,
                      const wchar_t* buglyDiagnoseSdkVersion,
                      const wchar_t* logDirs[],
                      int             logDirsNum,
                      const wchar_t* diagnoseConfigDir,
                      const wchar_t* logPubKey);

  /// @brief 初始化诊断系统
  /// @param appId appId
  /// @param appKey appKey
  /// @param appBundleId bundleId
  /// @param appVersion app version
  /// @param buglyDiagnoseSdkVersion diagnose sdk version
  /// @param logDirs 日志文件路径，在收到捞取日志的指令时会去这些目录下寻找
  /// @param logDirsNum, 日志文件路径个数
  /// @param diagnoseConfigDir
  /// 诊断系统配置文件路径，用于保存相关配置，不能和上面的logDir放在一起，以免清理日志时把配置文件清理掉
  /// @param logPubKey 日志加密的pubkey
  static void initDiagnose(const char* appId,
                      const char* appKey,
                      const char* appBundleId,
                      const char* appVersion,
                      const char* buglyDiagnoseSdkVersion,
                      const char* logDirs[],
                      int         logDirsNum,
                      const char* diagnoseConfigDir,
                      const char* logPubKey);

	/// @brief 设置诊断系统日志上报的域名，该方法必须在initDiagnose之前调用才能生效，默认为kLogServerHostTypeDefalut(woa正式环境)
	/// @param type 诊断系统上报域名类型
	static void setHostType(Bugly::Diagnose::ELogServerHostType type);

  /// @brief 设置userId，这个填写在捞log的"设备ID"处
  /// @param userId 也就时捞log时填的"设备ID"
  static void setUserId(const wchar_t* userId);

  /// @brief 设置userId，这个填写在捞log的"设备ID"处
  /// @param userId 也就时捞log时填的"设备ID"
  static void setUserId(const char* userId);

  /// @brief 设置主动上报的标签白名单。白名单中的自动上报将不计算频率/流量消耗等资源管控，避免造成上报失败。
  //  推荐返回如"用户主动反馈"场景下的自动上报标签
  static void setWhitelistForAutoUploadTags(const wchar_t* tags[], int num);

  /// @brief 设置主动上报的标签白名单。白名单中的自动上报将不计算频率/流量消耗等资源管控，避免造成上报失败。
  //  推荐返回如"用户主动反馈"场景下的自动上报标签
  static void setWhitelistForAutoUploadTags(const char* tags[], int num);

  /// @brief 设置当前的网络类型
  /// @param netType 
  static void setNetType(EBuglyLogNetType netType);

  /// @brief 设置捞取日志时是否递归搜索日志目录，如果不设置默认为true
  /// @param recursive 是否递归搜索addLogPath传入的目录，如果不设置默认为true
  static void setRecursiveSearchLogs(bool recursive);

  /// @brief 添加日志搜索路径，用于捞取日志时搜索符合条件的日志
  /// @param logPath 
  static void addLogPath(const wchar_t* logPath);
  static void addLogPath(const char* logPath);

  /// <summary>
  /// 拉取日志系统配置，包含捞日志信息和染色信息,用于推送接口调用
  /// </summary>
  static void RequestLogConfigOnPush();

  /// @brief 业务附带的上报信息，SDK进程捞日志上报时携带
  ///static void setExtraParamsForLogReport(const std::map<std::string, std::string>& extraParamsForLogReport);

  /// @brief 自定义附件路径，SDK进程捞取日志上报时将打包上传,注意：文件总大小建议不超过300MB
  ///static void setExtraFilesForLogReport(const std::vector<std::string>& xtraFilesForLogReport);

  /// @brief 主动上报日志接口
  /// @param files 待上报的log文件路径
  /// @param count 待上报的log文件数量
  /// @param tag 标签，必填，转换成char*后的长度不超过64 bytes
  /// @param summary 摘要信息，选填，转换成char*后的长度不超过256 bytes
  /// @param queryKey 检索Key，选填，用于平台互通检索上报信息
  /// @param sourcePlatform 来源平台，选填，用于平台互通记录上报来源
  static void uploadLogFiles(const wchar_t** files,
                             int count,
                             const wchar_t* tag,
                             const wchar_t* summary,
                             const wchar_t* queryKey,
                             const wchar_t* sourcePlatform);

  /// @brief 主动上报日志接口
  /// @param files 待上报的log文件路径
  /// @param count 待上报的log文件数量
  /// @param tag 标签，必填，长度不超过64 bytes
  /// @param summary 摘要信息，选填，长度不超过256 bytes
  /// @param queryKey 检索Key，选填，用于平台互通检索上报信息
  /// @param sourcePlatform 来源平台，选填，用于平台互通记录上报来源
  static void uploadLogFiles(const char** files,
                             int count,
                             const char* tag,
                             const char* summary,
                             const char* queryKey,
                             const char* sourcePlatform);
};

} // namespace Diagnose
} // namespace Bugly

#endif  //!__BUGLY_DIAGNOSE__H__