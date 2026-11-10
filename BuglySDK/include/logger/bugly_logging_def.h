/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_logging_def.h
* @author:	hodxiang
* @date:	2024年07月02日 17:14:16 星期二
* @brief:	log 相关定义
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_LOGGING_DEF__H__
#define __BUGLY_LOGGING_DEF__H__

namespace Bugly {

constexpr const char* kBuglyLoggerTagInternal = "Bugly";

enum EBuglyLogLevel {
    kLevelAll = 0,
    kLevelVerbos = 0,
    kLevelDebug,
    kLevelInfo,
    kLevelWarn,
    kLevelError,
    kLevelfatal,
    kLevelNone,
};


}
#endif  //!__BUGLY_LOGGING_DEF__H__