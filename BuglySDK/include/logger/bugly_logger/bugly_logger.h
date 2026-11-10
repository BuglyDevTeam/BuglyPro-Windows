/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_logger.h
* @author:	hodxiang
* @date:	2024年07月03日 15:16:46 星期三
* @brief:	bugly 内置的logger实现，底层基于xlog
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_LOGGER__H__
#define __BUGLY_LOGGER__H__

#include <atomic>
#include <string>
#include "logger/ibugly_logging_interface.h"
#include <windows.h>

namespace mars {
namespace comm {
class XloggerCategory;
}
}
namespace Bugly {

constexpr int kMaxPath = 260;
constexpr int kMaxLogFilePrefixLength = 64;
struct __declspec(dllexport) BuglyLoggerConfig {
  // 日志目录,默认C:/Users/xxx/AppData/Local/Temp/Tencent/buglyLog/logs
  char logPath_[kMaxPath]{0};

  // 日志相关本地持久化文件目录，用于存放需要本地保存的配置文件,
  // 默认C:/Users/xxx/AppData/Local/Temp/Tencent/buglyLog
  char logConfigDir_[kMaxPath]{0};

  // 默认日志级别,默认EBuglyLogLevel::kLevelError
  EBuglyLogLevel defaultLogLevel_{EBuglyLogLevel::kLevelError};

  // 日志加密key，默认不加密
  char publicKey_[kMaxPath]{0};

  // 控制台是否输出，默认关闭
  bool enableConsoleLog_{false};

  // 日志文件名前缀，可选
  char logFilePrefix_[kMaxLogFilePrefixLength]{0};

  BuglyLoggerConfig() = default;
  BuglyLoggerConfig(const char* logPath,
                    const char* logConfigDir,
                    const char* logPrefix,
                    const EBuglyLogLevel level,
                    const char* pubKey);

  BuglyLoggerConfig(const BuglyLoggerConfig& Other);
};


class BuglyLogger : public IBuglyLoggingIMP {
 public:
  BuglyLogger(BuglyLoggerConfig config);
  virtual ~BuglyLogger();

  // IBuglyLoggingIMP
  virtual void log(EBuglyLogLevel level, const char *tag, const char *file, const char *func, const int line, const char *format, ...) override;
  virtual void logv(EBuglyLogLevel level, const char *tag, const char *file, const char *func, const int line, const char *format, va_list args) override;
  virtual void log(EBuglyLogLevel level, const char* msg) override;

  virtual bool isColorState() override;
  virtual void resetColorLevel() override;
  virtual void setLogLevel(const EBuglyLogLevel level) override;
  virtual void setLogLevel(const EBuglyLogLevel level, const time_t endtime) override;

  virtual void getLogFolderPath(char *outPath, const int maxPath) override;
  virtual void getLogConfigFolderPath(char* outPath, const int maxPath) override;
  virtual void flushLog(const bool isSync) override;
  virtual void SetMaxLogAliveTime(const uint64_t time);
  virtual void SetMaxTotalLogFileSize(const uint64_t maxSize);
  virtual void SetConsoleLogEnable(const bool enabled);
  virtual bool GetLogEncryptPublicKey(char* out, const int out_len) override;

public:
  friend void writeToXlog(BuglyLogger& logger,
                                        const char* file,
                                        const char* func,
                                        const int line,
                                        const EBuglyLogLevel leven,
                                        const char* tag,
                                        const char* msg);

  friend void writeToXlog(BuglyLogger& logger, 
                                        const EBuglyLogLevel level, 
                                        void* xlogInfo, 
                                        const char* msg);


  bool loadLocalColorInfo(EBuglyLogLevel& level, int64_t& endtime);

 private:
  BuglyLoggerConfig config_;
  uintptr_t xlogInstancePtr_ = 0;
  uint64_t logIndex_ = 0;
};

}


extern "C" {
typedef Bugly::BuglyLogger*(__stdcall* CreateBuglyLoggerT)(const Bugly::BuglyLoggerConfig*);
}

__forceinline Bugly::BuglyLogger* CreateBuglyLogger(const Bugly::BuglyLoggerConfig* config,
                                                    const char* buglyLoggerDllPath = "bugly_logger.dll") {
  auto handle =
      ::LoadLibraryExA(buglyLoggerDllPath, nullptr, LOAD_LIBRARY_SEARCH_USER_DIRS | LOAD_LIBRARY_SEARCH_SYSTEM32);
  if (nullptr == handle) {
    handle = ::LoadLibraryA(buglyLoggerDllPath);
  }
  if (nullptr == handle) {
    return nullptr;
  }

  CreateBuglyLoggerT creater = reinterpret_cast<CreateBuglyLoggerT>(::GetProcAddress(handle, "CreateBuglyLoggerImpl"));
  if (nullptr == creater) {
    creater = reinterpret_cast<CreateBuglyLoggerT>(::GetProcAddress(handle, "_CreateBuglyLoggerImpl@4"));
  }
  if (nullptr == creater) {
    return nullptr;
  }
  return creater(config);
}

#endif  //!__BUGLY_LOGGER__H__