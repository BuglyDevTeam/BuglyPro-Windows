/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_logger_c.h
* @author:	hodxiang
* @date:	2026年05月12日 11:26:42 星期二
* @brief:	bugly logger c interface
**********************************************************************************************************************************************************************************************************************************
**/


#ifndef __BUGLY_LOGGER_C__H__
#define __BUGLY_LOGGER_C__H__

#ifndef BUGLY_WIN_API
  #ifdef BUGLY_WIN_EXPORTS
    #define BUGLY_WIN_API __declspec(dllexport)
  #else
    #define BUGLY_WIN_API __declspec(dllimport)
  #endif
#endif // #ifndef BUGLY_WIN_API

#define kBuglyLogLevel_All                  0
#define kBuglyLogLevel_Verbos               0
#define kBuglyLogLevel_Debug                1
#define kBuglyLogLevel_Info                 2
#define kBuglyLogLevel_Warn                 3
#define kBuglyLogLevel_Error                4
#define kBuglyLogLevel_Fatal                5
#define kBuglyLogLevel_None                 6

#define BUGLYLOGF(level, tag_name, format, ...) \
do{ \
    BuglyDiagnoseLogFmt(level, tag_name, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
} while (0)

#define BUGLYSUBLOGF(sub_logger, level, tag_name, format, ...) \
do{ \
    BuglyDiagnoseSubLogFmt(sub_logger, level, tag_name, __FILE__, __func__, __LINE__, format, ##__VA_ARGS__); \
} while (0)

#define BUGLYLOGMSG(level, tag_name, msg) \
do { \
    BuglyDiagnoseLogMsg(level, tag_name, __FILE__, __func__, __LINE__, msg); \
} while(0)

#define BUGLYSUBLOGMSG(sub_logger, level, tag_name, msg) \
do { \
    BuglyDiagnoseSubLogMsg(sub_logger, level, tag_name, __FILE__, __func__, __LINE__, msg); \
} while(0)

#define BuglyLogFmtVerbos(tag, format, ...) BUGLYLOGF(kBuglyLogLevel_Verbos, tag, format, ##__VA_ARGS__)
#define BuglyLogFmtDebug(tag, format, ...) BUGLYLOGF(kBuglyLogLevel_Debug, tag, format, ##__VA_ARGS__)
#define BuglyLogFmtInfo(tag, format, ...) BUGLYLOGF(kBuglyLogLevel_Info, tag, format, ##__VA_ARGS__)
#define BuglyLogFmtWarn(tag, format, ...) BUGLYLOGF(kBuglyLogLevel_Warn, tag, format, ##__VA_ARGS__)
#define BuglyLogFmtError(tag, format, ...) BUGLYLOGF(kBuglyLogLevel_Error, tag, format, ##__VA_ARGS__)
#define BuglyLogFmtFatal(tag, format, ...) BUGLYLOGF(kBuglyLogLevel_Fatal, tag, format, ##__VA_ARGS__)

#define BuglySubLogFmtVerbos(sub_logger, tag, format, ...) BUGLYSUBLOGF(sub_logger, kBuglyLogLevel_Verbos, tag, format, ##__VA_ARGS__)
#define BuglySubLogFmtDebug(sub_logger, tag, format, ...) BUGLYSUBLOGF(sub_logger, kBuglyLogLevel_Debug, tag, format, ##__VA_ARGS__)
#define BuglySubLogFmtInfo(sub_logger, tag, format, ...) BUGLYSUBLOGF(sub_logger, kBuglyLogLevel_Info, tag, format, ##__VA_ARGS__)
#define BuglySubLogFmtWarn(sub_logger, tag, format, ...) BUGLYSUBLOGF(sub_logger, kBuglyLogLevel_Warn, tag, format, ##__VA_ARGS__)
#define BuglySubLogFmtError(sub_logger, tag, format, ...) BUGLYSUBLOGF(sub_logger, kBuglyLogLevel_Error, tag, format, ##__VA_ARGS__)
#define BuglySubLogFmtFatal(sub_logger, tag, format, ...) BUGLYSUBLOGF(sub_logger, kBuglyLogLevel_Fatal, tag, format, ##__VA_ARGS__)

#define BuglyLogMsgVerbos(tag, msg) BUGLYLOGMSG(kBuglyLogLevel_Verbos, tag, msg)
#define BuglyLogMsgDebug(tag, msg) BUGLYLOGMSG(kBuglyLogLevel_Debug, tag, msg)
#define BuglyLogMsgInfo(tag, msg) BUGLYLOGMSG(kBuglyLogLevel_Info, tag, msg)
#define BuglyLogMsgWarn(tag, msg) BUGLYLOGMSG(kBuglyLogLevel_Warn, tag, msg)
#define BuglyLogMsgError(tag, msg) BUGLYLOGMSG(kBuglyLogLevel_Error, tag, msg)
#define BuglyLogMsgFatal(tag, msg) BUGLYLOGMSG(kBuglyLogLevel_Fatal, tag, msg)

#define BuglySubLogMsgVerbos(sub_logger, tag, msg) BUGLYSUBLOGMSG(sub_logger, kBuglyLogLevel_Verbos, tag, msg)
#define BuglySubLogMsgDebug(sub_logger, tag, msg) BUGLYSUBLOGMSG(sub_logger, kBuglyLogLevel_Debug, tag, msg)
#define BuglySubLogMsgInfo(sub_logger, tag, msg) BUGLYSUBLOGMSG(sub_logger, kBuglyLogLevel_Info, tag, msg)
#define BuglySubLogMsgWarn(sub_logger, tag, msg) BUGLYSUBLOGMSG(sub_logger, kBuglyLogLevel_Warn, tag, msg)
#define BuglySubLogMsgError(sub_logger, tag, msg) BUGLYSUBLOGMSG(sub_logger, kBuglyLogLevel_Error, tag, msg)
#define BuglySubLogMsgFatal(sub_logger, tag, msg) BUGLYSUBLOGMSG(sub_logger, kBuglyLogLevel_Fatal, tag, msg)


#ifdef  __cplusplus
extern "C" {
#endif  // __cplusplus

/**
 * @description 初始化bugly_logger.dll中自带的xlog，后续业务可以使用它记录log
 * @param logDir, 存储log的路径，注意要是可写入的路径。
 * @param logPrefix, 日志文件名的前缀，可以用来区分不同的log
 * @param logLevel, 日志等级
 * @param pubkey, 日志加密Key，如果传null表示不加密，传default表示用默认key加密
 * @param enableConsoleLog 是否将log输出到控制台,1 为 输出，0为不输出
 * @return: 1 为成功，0 为失败
 */
BUGLY_WIN_API int BuglyInitLogger(const char* logDir, int logDir_len,
    const char* logPrefix, int logPrefix_len,
    int logLevel, 
    const char* pubkey, int pubkey_len,
    int enableConsoleLog);

/**
 * @description 创建一个子日志实例
 * @param logDir, 存储log的路径，注意要是可写入的路径。
 * @param logPrefix, 日志文件名的前缀，可以用来区分不同的log
 * @param logLevel, 日志等级
 * @param enableConsoleLog 是否将log输出到控制台,1 为 输出，0为不输出
 * @return: sublogger对象，在不需要时需要调用BuglyDiagnoseFreeSubLogger销毁
 */
BUGLY_WIN_API void* BuglyDiagnoseCreateSubLogger(const char* logDir, int logDir_len,
    const char* logPrefix, int logPrefix_len, int logLevel, int enableConsoleLog);

/**
 * @description 释放一个子日志实例
 */
BUGLY_WIN_API void BuglyDiagnoseFreeSubLogger(void* sub_logger);


/**
 * @description 写入日志
 */
BUGLY_WIN_API void BuglyDiagnoseLogFmt(int level, 
    const char* tag_name,
    const char* file,
    const char* func,
    int line, 
    const char* format,
    ...);

/**
 * @description 写入日志
 */
BUGLY_WIN_API void BuglyDiagnoseSubLogFmt(void* sub_logger, int level, 
    const char* tag_name, 
    const char* file, 
    const char* func, 
    int line, 
    const char* format, 
    ...);

/**
 * @description 写入日志
 */
BUGLY_WIN_API void BuglyDiagnoseLogMsg(int level, 
    const char* tag_name, 
    const char* file, 
    const char* func, 
    int line,
    const char* msg);

/**
 * @description 写入日志
 */
BUGLY_WIN_API void BuglyDiagnoseSubLogMsg(void* sub_logger, 
    int level, 
    const char* tag_name, 
    const char* file, 
    const char* func, 
    int line,
    const char* msg);

/**
 * @description 日志落盘
 * @param bsync, 是否同步写入,0为false，1为true
 */
BUGLY_WIN_API void BuglyDiagnoseFlushLog(int bsync);

/**
 * @description 日志落盘
 * @param sub_logger, 子日志实例
 * @param bsync, 是否同步写入，0为false，1为true
 */
BUGLY_WIN_API void BuglyDiagnoseFlushSubLog(void* sub_logger, int bsync);

#ifdef  __cplusplus
}
#endif  // __cplusplus

#endif  //!__BUGLY_LOGGER_C__H__