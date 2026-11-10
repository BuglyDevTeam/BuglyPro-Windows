/**
**********************************************************************************************************************************************************************************************************************************
* @file:	ibugly_logging_interface.h
* @author:	hodxiang
* @date:	2024年07月02日 17:02:52 星期二
* @brief:	bugly log实现接口，业务也可以时间这个接口给bugly使用
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __IBUGLY_LOGGING_IMP__H__
#define __IBUGLY_LOGGING_IMP__H__

#include "bugly_logging_def.h"
#include <ctime>
#include <cstdint>
#include <cstdarg>

namespace Bugly {
class IBuglyLoggingIMP {
 public:
    /// <summary>
    /// 输出log接口(带格式化字符串)
    /// </summary>
    virtual void log(EBuglyLogLevel level, const char *tag, const char *file, const char *func, const int line, const char *format, ...) = 0;

    /// <summary>
    /// 输出log接口(带格式化字符串)
    /// </summary>
    virtual void logv(EBuglyLogLevel level, const char *tag, const char *file, const char *func, const int line, const char *format, va_list args) = 0;

    /// <summary>
    /// 输出log接口(直接输出)
    /// </summary>
    virtual void log(EBuglyLogLevel level, const char* msg) = 0;

    /// <summary>
    /// 是否处于染色状态
    /// </summary>
    virtual bool isColorState() = 0;

    /// <summary>
    /// 重置染色级别
    /// </summary>
    virtual void resetColorLevel() = 0;

    /// <summary>
    /// 设置log打印级别
    /// </summary>
    virtual void setLogLevel(const EBuglyLogLevel level) = 0;

    /// <summary>
    /// 设置打印log的级别，以及过期时间
    /// </summary>
    virtual void setLogLevel(const EBuglyLogLevel level, const time_t endtime) = 0;

    /// <summary>
    /// 获取log文件夹路径,注意这个路径下不要放其他文件，以免清理log时错误清除掉
    /// </summary>
    virtual void getLogFolderPath(char *outPath, const int maxPath) = 0;

    /// @brief log相关本地配置文件存放路径，用于存放log相关的配置,注意不要和上面的getLogFolderPath
    /// @brief 相同，以免清理log时配置文件被清除
    virtual void getLogConfigFolderPath(char *outPath, const int maxPath) = 0;

    /// <summary>
    /// 强制写入log到文件
    /// </summary>
    virtual void flushLog(const bool isSync) = 0;

    /// <summary>
    /// 设置log最大存留时间, 单位秒，默认10(10 * 24 * 60 * 60)天
    /// </summary>
    virtual void SetMaxLogAliveTime(const uint64_t time) = 0;

    /// <summary>
    /// 设置所有log文件的大小限额，超过即清理(底层限制最小50MB)
    /// </summary>
    /// <param name="maxSize">总文件大小，默认为0代表不限制</param>
    virtual void SetMaxTotalLogFileSize(const uint64_t maxSize) = 0;

    /// <summary>
    /// 设置控制台log输出
    /// </summary>
    virtual void SetConsoleLogEnable(const bool enabled) = 0;

    /// <summary>
    /// 获取日志加密的公钥
    /// </summary>
    virtual bool GetLogEncryptPublicKey(char* out, const int out_len) = 0;
};
}
#endif  //!__IBUGLY_LOGGING_IMP__H__
