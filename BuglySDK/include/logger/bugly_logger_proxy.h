/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_logger_proxy.h
* @author:	hodxiang
* @date:	2024年07月02日 17:30:21 星期二
* @brief:	
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_LOGGER_PROXY__H__
#define __BUGLY_LOGGER_PROXY__H__

#include "ibugly_logging_interface.h"

namespace Bugly {


#define BuglyLog(level, tag_name, format, ...) \
do { \
  Bugly::BuglyLoggerProxy::defaultProxy().log(level, tag_name, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
} while (0)

#define BuglyLogVerbos(tag, format, ...) BuglyLog(Bugly::EBuglyLogLevel::kLevelVerbos, tag, format, ##__VA_ARGS__)
#define BuglyLogDebug(tag, format, ...) BuglyLog(Bugly::EBuglyLogLevel::kLevelDebug, tag, format, ##__VA_ARGS__)
#define BuglyLogInfo(tag, format, ...) BuglyLog(Bugly::EBuglyLogLevel::kLevelInfo, tag, format, ##__VA_ARGS__)
#define BuglyLogWarn(tag, format, ...) BuglyLog(Bugly::EBuglyLogLevel::kLevelWarn, tag, format, ##__VA_ARGS__)
#define BuglyLogError(tag, format, ...) BuglyLog(Bugly::EBuglyLogLevel::kLevelError, tag, format, ##__VA_ARGS__)
#define BuglyLogFatal(tag, format, ...) BuglyLog(Bugly::EBuglyLogLevel::kLevelfatal, tag, format, ##__VA_ARGS__)

struct  BuglyLoggerConfig;
class BuglySpinLock;

class __declspec(dllexport) BuglyLoggerProxy {
 public:
    static BuglyLoggerProxy& defaultProxy();

    BuglyLoggerProxy();
    ~BuglyLoggerProxy() = delete;

    /// <summary>
    /// 设置外部logger,BuglyLoggerProxy不会析构logger,类似弱引用
    /// </summary>
    void setLogger(IBuglyLoggingIMP* logger);

    /// <summary>
    /// 设置logger为buglyLogger(底层为xlog),proxy自己会管理buglyLogger_的生命周期
    /// </summary>
    /// <param name="config">初始化配置</param>
    void setBuglyLogger(const BuglyLoggerConfig& config);
    /// <summary>
    /// 清理buglyLogger_,这个接口不会修改用户log输出的logger_
    /// </summary>
    void cleanBuglyLogger();

    /// @brief 将log写入文件
    /// @param sync 是否同步写入。
    void flushLog(bool sync);

    /// <summary>
    /// 写入log，并关闭log模块，在程序退出时调用
    /// </summary>
    void endLogger();

    void log(EBuglyLogLevel level,
             const char* tag,
             const char* file,
             const char* func,
             const int line,
             const char* format,
             ...);
    void logv(EBuglyLogLevel level,
             const char* tag,
             const char* file,
             const char* func,
             const int line,
             const char* format,
             va_list args);
    void log(EBuglyLogLevel level, const char* msg);

    /// @brief 设置log等级
    /// @param level 
    void setLogLevel(EBuglyLogLevel level);
    void setLogLevel(EBuglyLogLevel level, uint64_t endtime);

    /// @brief 重置log等级
    void resetColorLevel();

    /// @brief 获取当前log输出路径
    bool getLogDir(char* out, int len);
    /// @brief 获取当前log输出路径
    bool getLogDir(wchar_t* out, int len);
    
    bool getLogConfigFolderPath(char* out_buffer, int out_buffer_len);

    bool getLogConfigPubKey(char* out_buffer, int out_buffer_len);

   private:
    IBuglyLoggingIMP *logger_ = nullptr;
    IBuglyLoggingIMP *buglyLogger_ = nullptr;
    BuglySpinLock *lock_ = nullptr;
};

}

#endif  //!__BUGLY_LOGGER_PROXY__H__