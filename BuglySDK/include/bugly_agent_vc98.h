/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_agent_vc98.h
* @author:	hodxiang
* @date:	2025年09月18日 16:47:09 星期四
* @brief:	用于C++98的接口定义(vc6)
**********************************************************************************************************************************************************************************************************************************
**/


#ifndef __BUGLY_AGENT_VC98__H__
#define __BUGLY_AGENT_VC98__H__

#include <windows.h>

#include "bugly_config.h"

/* for c++98 */
#if defined(_MSVC_LANG)
    #if _MSVC_LANG < 201103L
        #define nullptr NULL
        typedef unsigned char uint8_t;
        typedef unsigned short uint16_t;
        typedef unsigned int uint32_t;
        typedef unsigned __int64  uint64_t;
    #endif //_MSVC_LANG < 201103L
#elif __cplusplus < 201103L
    #define nullptr NULL
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
    typedef unsigned __int64  uint64_t;
#endif /* _MSVC_LANG */

/* InitBuglyManager返回值 */
#define BUGLY_INIT_NO_ERROR         0   /* 没有错误 */
#define BUGLY_INIT_DATABASE_ERROR   1   /* bugly database 初始化失败，请检查database path路径是否可读写 */
#define BUGLY_INIT_STATHANDER_ERROR 2   /* 拉起crashpad_handler.exe失败，请检查handler路径是否存在，或者pipename已经被使用 */
#define BUGLY_INIT_PIPENAME_INVALID 3   /* 和crashpad_handler进程通信的pipename不可用 */
#define BUGLY_INIT_PIPENAME_EMPTY   4   /* InitBuglyManager2 获取可用的pipename失败,即pipename参数为空，dll共享内存的pipename也为空 */
#define BUGLY_INIT_PARAM_ERROR      5   /* InitBuglyManager2 结构传入的结构体参数不正确 */

#ifdef BUGLY_WIN_EXPORTS
#define BUGLY_WIN_API __declspec(dllexport)
#else
#define BUGLY_WIN_API __declspec(dllimport)
#endif

class IBuglyAgent;
typedef void (*pfBuglyCrashCallback)(IBuglyAgent*, void*);

extern "C" {
__declspec(dllexport) void BuglyCaptureContext(CONTEXT* context);
typedef void (*BuglyCaptureContextT)(CONTEXT*);

/**
 * @description 向系统注册额外的handler，用于监控额外的异常。比如strcpy_s一类的安全桉树抛出的非法参数崩溃和虚函数调用的purecall错误
 * @param modulePath 模块绝对路径 
 * @return 成功返回true，否则返回falst
 */
BUGLY_WIN_API bool RegisterBuglyExtraHandler(const wchar_t* modulePath);
BUGLY_WIN_API bool UnregisterBuglyExtraHandler(const wchar_t* modulePath);
}


#ifndef BUGLY_REPORT_DUMP
/* 主动触发抓取dmp文件并上报到bugly，使用这个宏触发的上报会全部报到Simulated Exception这个异常分类下
 * 调用这个宏以后会阻塞线程，后续可以自己终止进程
 * BUGLY_REPORT_DUMP();    //主动触发上报
 * ::abort();              //完了终止进程           
 */ 
#define BUGLY_REPORT_DUMP()                                                                                     \
do {                                                                                                            \
  HMODULE hMod = ::GetModuleHandleA(BUGLY_AGENT_NAME);                                                          \
  if (NULL == hMod) {                                                                                           \
    break;                                                                                                      \
  }                                                                                                             \
  BuglyCaptureContextT captureContextPtr = (BuglyCaptureContextT)::GetProcAddress(hMod, "BuglyCaptureContext"); \
  if (!captureContextPtr) {                                                                                     \
    break;                                                                                                      \
  }                                                                                                             \
  CONTEXT context;                                                                                              \
  captureContextPtr(&context);                                                                                  \
  IBuglyAgent* bugly = NULL;                                                                                 	\
  if ((kSuccess == CreateBuglyAgent(&bugly)) &&                                               					\
      (bugly != NULL)) {                                                                                     	\
    bugly->ReportDumpWithoutCrash(context);                                                                     \
  }                                                                                                             \
} while (false)                                                                                                 \

#endif //BUGLY_REPORT_DUMP

class IBuglyAgent {
 public:
  /* 所有接口都不是线程安全的，请务必在一个线程操作 */

  /**
   * @description: first 进程的初始化
   * @param handler 上报进程路径，绝对路径
   * @param database crash 数据路径，绝对路径
   * @param metrics_dir crash 数据路径，绝对路径
   * @param app_id app id
   * @param app_key app key
   * @param app_version 版本号
   * @param bundle_id qci id
   * @param timeout timeout 最大等待时间，单位 ms
   * @param is_pop_dialog 是否弹窗提示用户 crash 信息
   * @param is_need_attach_info 是否发送 crash 进程窗口及系统磁盘附加信息
   * @param is_need_upload 是否上传, 如果为 false, 则只记录保存在本地.
   * @param is_monitor_sel
   * @param is_server_process 是否由系统服务进程调用
   * @param account_id 初始用户id，供dau上报使用，当账号更新后可以调用SetKey通知bugly更新
   * @param pipename 和crashpad_hander.exe连接的管道名称，如果为null,就像以前一样随机生成一个，随机生成的可用GetHandlerIPCPipeName接口获取
	 * @param deviceId 业务传入的deviceId
   * @return: 0 为成功
   */
  virtual int InitBuglyManager(const wchar_t* handler,
                               const wchar_t* database,
                               const wchar_t* metrics_dir,
                               const wchar_t* app_id,
                               const wchar_t* app_key,
                               const wchar_t* app_version,
                               const wchar_t* bundle_id,
                               const wchar_t* app_name,
                               const wchar_t* app_disp_name,
                               const char* build_id,
                               unsigned int timeout,
                               bool is_pop_dialog,
                               bool is_need_attach_info,
                               bool is_need_upload = true,
                               bool is_monitor_sel = false,
                               bool is_server_process = false,
                               const wchar_t* account_id = nullptr,
                               const wchar_t* pipename = nullptr,
															 const wchar_t* deviceId = nullptr) = 0;

  
  /**
   * @description: first 进程的初始化，和InitBuglyManager的区别是：InitBuglyManager2接口合并了InitBuglyManager和SetHandlerIPCPipe
   *               如果is_reuse_bugly_server为true，InitBuglyManager2会先判断当前指定的pipename的crashpad_handler进程是否已经存在，如果
   *               存在就直接复用，不存在就拉起新的。如果is_reuse_bugly_server为false，那么和InitBuglyManager一样直接新拉起一个crashpad handler进程
   * @param handler 上报进程路径，绝对路径
   * @param database crash 数据路径，绝对路径
   * @param metrics_dir crash 数据路径，绝对路径
   * @param app_id app id
   * @param app_key app key
   * @param app_version 版本号
   * @param bundle_id qci id
   * @param is_reuse_bugly_server 是否复用已有的crashpad_handler.exe进程，如果为false，就直接拉起新的crashpad_hander.exe
   * @param timeout timeout 最大等待时间，单位 ms
   * @param is_pop_dialog 是否弹窗提示用户 crash 信息
   * @param is_need_attach_info 是否发送 crash 进程窗口及系统磁盘附加信息
   * @param is_need_upload 是否上传, 如果为 false, 则只记录保存在本地.
   * @param is_monitor_sel
   * @param is_server_process 是否由系统服务进程调用
   * @param account_id 初始用户id，供dau上报使用，当账号更新后可以调用SetKey通知bugly更新
   * @param pipename 和crashpad_hander.exe连接的管道名称，如果为null,就像以前一样随机生成一个,随机生成的可以用GetHandlerIPCPipeName接口获取
   * @param is_in_dll_mode 是否在业务进程抓取crash栈，如果业务程序需要加固，就需要传yes
	 * @param deviceId 业务传入的deviceId
   * @return: 0 为成功
   */
  virtual int InitBuglyManager2(const wchar_t* handler,
                               const wchar_t* database,
                               const wchar_t* metrics_dir,
                               const wchar_t* app_id,
                               const wchar_t* app_key,
                               const wchar_t* app_version,
                               const wchar_t* bundle_id,
                               const wchar_t* app_name,
                               const wchar_t* app_disp_name,
                               const char* build_id,
                               bool is_reuse_bugly_server,
                               unsigned int timeout,
                               bool is_pop_dialog,
                               bool is_need_attach_info,
                               bool is_need_upload = true,
                               bool is_monitor_sel = false,
                               bool is_server_process = false,
                               const wchar_t* account_id = nullptr,
                               const wchar_t* pipename = nullptr,
                               bool is_in_dll_mode = false,
															 const wchar_t* deviceId = nullptr) = 0;


  /**
   * @description 新的Bugly初始化接口，使用Bugly::BuglyConfig结构体代替原来的巨多参数, 功能继承自InitBuglyManager2
   * 后续InitBuglyManager1和InitBuglyManager2将停止维护。
   * 结构体参数中的数据在接口内部拷贝保存
  */
  virtual int InitBuglyManager3(const Bugly::BuglyConfig* config) = 0;

   /**
   * @description: 获取 ipc 名称，需要在 InitBuglyManager 成功之后调用
   * @param pipe_name, 返回 pipe name
   * @param pipe_name_len pipe_name 缓冲区长度，建议用 MAX_PATH
   * @return: 0 为成功
   */
  virtual int GetHandlerIPCPipeName(wchar_t* pipe_name, unsigned int pipe_name_len) = 0;

  /**
   * @description: 其它进程的初始化
   * @param pipe_name, 返回 pipe name
   * @return: 0 为成功
   * */
  virtual int SetHandlerIPCPipe(const wchar_t* pipe_name) = 0;

  /**
   * @description: 更新本进程的日志路径，方便 crash 的时候打捞 *
   * @param pipe_name pipename,
   * @param process_id 要更新日志文件路径的进程 id,
   * @param tag 日志 tag 名称, 同一个进程如果要打捞多份日志的化需要用 tag 进行区分
   * @param tag_len tag最大缓冲区，字符数，最大 20
   * @param log_file_path 日志文件路径,
   * @param log_file_path_len 日志文件路径最大缓冲区，字符数，最大 MAX_PATH
   * @return: 0 为成功
   *  */
  virtual int SetLogFilePath(const wchar_t* pipe_name,
                 unsigned long process_id,
                 const wchar_t* tag,
                 unsigned int tag_len,
                 const wchar_t* log_file_path,
                 unsigned int log_file_path_len) = 0;


  /**
   * @description: 更新本进程的日志路径，方便 crash 的时候打捞 *
   * @param process_id 要更新日志文件路径的进程 id,
   * @param tag 日志 tag 名称, 同一个进程如果要打捞多份日志的化需要用 tag 进行区分
   * @param tag_len tag最大缓冲区，字符数，最大 20
   * @param log_file_path 日志文件路径,
   * @param log_file_path_len 日志文件路径最大缓冲区，字符数，最大 MAX_PATH
   * @return: 0 为成功
   *  */
  virtual int SetLogFilePath(unsigned long process_id,
                             const wchar_t* tag,
                             unsigned int tag_len,
                             const wchar_t* log_file_path,
                             unsigned int log_file_path_len) = 0;

  /**
   * @description: 更新本进程的其他数据 *
   * @param pipe_name pipename,
   * @param key key,
   * @param key_len key 最大缓冲区，字符数, 最大 100
   * @param value value,
   * @param value_len value 最大缓冲区，字符数，最大 100
   * @return: 0 为成功
   *  */
  virtual int SetKey(const wchar_t* pipe_name,
         const wchar_t* key,
         unsigned int key_len,
         const wchar_t* value,
         unsigned int value_len) = 0;

  virtual int SetKey(const wchar_t* key,
                     const wchar_t* value) = 0;

  virtual int SetKey(const char* key, const char* value) = 0;

  virtual void SetCrashCallback(pfBuglyCrashCallback callback, void* context) = 0;

  virtual int ReportCustomError(unsigned long process_id,
                                const wchar_t* tag,
                                unsigned int tag_len,
                                const wchar_t* err_file_path,
                                unsigned int err_file_path_len) = 0;

  /**
  * @description: 通用自定义错误上报接口，使用内存传递错误信息，对于可能触发很频繁的错误推荐使用该接口
  * @param addr "#0      _MyHomePageState._incrementCounter.<anonymous closure> (package:flutter_application_1/main.dart:81:33)"
  * @param message "type 'double' is not a subtype of type 'int' in type cast"
  * @param name "_CastError(Serious)"
  * @param call_stack full call_stack string
  * @param thread_name "main"
  * @param additional_info  后续扩展
  */
  virtual int ReportCommonCustomError(const char* addr,
                                      const char* message,
                                      const char* name,
                                      const char* call_stack,
                                      const char* thread_name) = 0;

  /**
   * @description: 生成minidump时是否收集更多内存信息,打开这个会增加dmp文件大小
   * @param bEnable: 是否收集更多内存
   * @param maxSize: 收集额外内存大小限额,默认10MB
   */
  virtual void SetEnableGatherIndirectlyReferencedMemory(bool bEnable, uint32_t maxSize = 10485760) = 0;
  

  /**
   * @description: 上报非crash的dump。或者业务自己捕获到的异常dump
   * @param context: 当前上下文
  */
  virtual void ReportDumpWithoutCrash(const CONTEXT& context) = 0;

  /**
   * @description: 上报crash的dump。业务自己捕获到的异常dump可通过此接口上报；
   * 与ReportDumpWithoutCrash差异在于采集完成dump后自动关闭该进程
   * @param exception_pointers: 异常的上下文
   */
  virtual void ReportDumpAndCrash(EXCEPTION_POINTERS* exception_pointers) = 0;


  /**
   * @description: 加载bugly日志模块,用于bugly自身的log输出
   * @param log_dir: 存放log文件夹的路径,这里需要业务传入是因为如果是服务进程拉起的，可能获取不到temp目录。最终的log文件会放在log_dir/logs目录下。因为log_dir下还会存一些配置文件。
   * @param dllpath: bugly_logger.dll的路径
  */
  virtual bool LoadBuglyLoggerModule(const wchar_t* log_dir, const wchar_t* dllpath = nullptr) = 0;

  /**
   * @description: 设置上报的自定义字段,限制和setkey一样，最大长度都是100
   * @param key: 自定义字段的key
   * @param value: 自定义字段的value，如果value为null或者空字符串，则删除对应的kv
  */
  virtual void SetCustomField(const char* key, const char* value) = 0;

  /**
   * @description: 设置上报的自定义字段,限制和setkey一样，最大长度都是100
   * @param key: 自定义字段的key
   * @param value: 自定义字段的value，如果value为null或者空字符串，则删除对应的kv
  */
  virtual void SetCustomField(const wchar_t* key, const wchar_t* value) = 0;

  virtual void SetCustomField(const wchar_t* pipe_name,
                              const wchar_t* key,
                              uint32_t key_len,
                              const wchar_t* value,
                              uint32_t value_len) = 0;

  /**
   * @description: 上报所有线程的调用栈。默认只上报crash线程的调用栈
   * @param bEnable: 是否上报所有线程调用栈
  */
  virtual void SetEnableDumpAllThreadCallstack(bool bEnable) = 0;
  
  /**
   * @description: 开始卡顿监控
  */
  virtual void StartLagMonitor() = 0;

  /**
   * @description: 停止卡顿监控
  */
  virtual void StopLagMonitor() = 0;
  
  /**
   * @description: 暂停卡顿监控， 跟停止监控的区别在于这个函数不销毁监控对象
  */
  virtual void PauseLagMonitor() = 0;
  
  /**
   * @description: 恢复卡顿监控， 调用pause后可以调用此函数来恢复
  */
  virtual void ResumeLagMonitor() = 0;

  /**
   * @description: 通过业务指定的pipename生成windows上正式的pipename，将该pipename传递给InitBuglyManater拉起bugly守护进程，
                   后续其他进程也可以用这个pipename和这个bugly进程交互,后续调用SetKey传递pipename也应该使用该返回值
   * @param buzPipename: 业务指定的用于和bugly守护进程交互的pipename，注意该buzPipename要全局唯一，且版本唯一，
                         防止业务升级时新版本客户端进程链接到老版本的bugly, 可以使用类似“版本号_构建号”
   * @param buglyAppid: 业务的bugly appid，和传入InitBuglyManager中的一致，用于区分不同业务的pipename
   * @param outPipeName: 返回值
   * @param pipeNameLen: outPipeName的大小
   * @return 成功返回true，否则返回false
   */
  virtual bool GenCanonicalPipeName(const wchar_t* buzPipename,
                                    const wchar_t* buglyAppid,
                                    wchar_t* outPipeName,
                                    unsigned int pipeNameLen) = 0;
  
  /**
   * @description: 查询当前是否有指定buzPipename和buglyAppid拉起的bugly守护进程
   * @param buzPipename: 用于拉起bugly守护进程的buzPipename
   * @param buglyAppid: 用于拉起bugly守护进程的buglyAppid
   * @return 如果当前已经存在对应的bugly守护进程运行，返回true。否则返回false
   */
  virtual bool BuglyServerExistsAndReady(const wchar_t* buzPipename, const wchar_t* buglyAppid) = 0;

  /**
   * @description: 查询当前是否由指定pipename拉起的bugly守护进程
   * @param canonicalPipename: 正规的windows pipename，由GenCanonicalPipeName生成
   * @return 如果当前已经存在对应的bugly守护进程运行，返回true。否则返回false
   */
  virtual bool BuglyServerExistsAndReady(const wchar_t* canonicalPipename) = 0;

  /**
   * @description:
   * 开启额外的捕获机制，默认为关闭，开启以后可以上报strcpy_s一类的安全函数抛出的非法参数崩溃和虚函数调用等purecall错误
   * @param bExtraHandlerEnable: 是否开启额外的捕获机制
   */
  virtual bool EnablExtraHandler(bool bExtraHandlerEnable, const wchar_t* modulePath) = 0;

  /**
   * @description:
   * 进程启动时可以额外上报DAU，用于统计指定进程的crash率
   * @param buzProcessName: 进程标识，可以时进程名，也可以是进程类型，由业务自行决定。
   * @param processId: 进程ID
   * @return 成功返回true，否则返回false
   */
  virtual bool ReportProcessDau(const wchar_t* buzProcessName, uint64_t processId, const wchar_t* pipename = nullptr) = 0;

  /**
   * @description: 给子进程设置对应的appInfo。比如如果需要在子进程开启卡顿监控，就需要在子进程先设置这个
   */
  virtual void SetAppBaseInfo(const wchar_t* app_id,
                              const wchar_t* app_key,
                              const wchar_t* bundle_id,
                              const wchar_t* app_ver,
                              const wchar_t* process_name,
                              const wchar_t* account_id,
															const wchar_t* device_id) = 0;

  /**
   * @description: 开启业务进程更新配置的通知
   * 开启后才能收到配置更新的通知，某些监控项比如卡顿监控，需要在业务的主线程中启动和关闭，就需要
   * 打开该通知
   * @param bNeedsUpdateConfig: 是否需要马上同步一次配置, 如果该接口在InitBugly前调用，就可以填falst，因为Bugly加载默认配置后也会发通知
   *                            如果是子进程后续启动，要填true
   */
  virtual void EnableClientConfigUpdateNotify(bool bNeedsUpdateConfig = false) = 0;

	/**
	 * @description: 更新场景信息，Bugly最多存储256个历史场景信息
	 * @param scene: 场景id，最长MAX_PATH。错误，崩溃，卡顿可以根据这个进行聚类。
	*/
	virtual void SetScene(const wchar_t* scene) = 0;
	virtual void SetScene(const char* scene) = 0;
	virtual void SetScene(const wchar_t* pipename, const wchar_t* scene) = 0;
};


#endif  //!__BUGLY_AGENT_VC98__H__
