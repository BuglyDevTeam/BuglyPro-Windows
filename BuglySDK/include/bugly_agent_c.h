/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_agent_c.h
* @author:	hodxiang
* @date:	2025年09月18日 23:23:52 星期四
* @brief:	bugly agent c接口, 用于c#或者其他语言调用
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_AGENT_C__H__
#define __BUGLY_AGENT_C__H__

#include <windows.h>

// for c++98
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64  uint64_t;


/// InitBuglyManager返回值
#define BUGLY_INIT_NO_ERROR         0   /// 没有错误
#define BUGLY_INIT_DATABASE_ERROR   1   /// bugly database 初始化失败，请检查database path路径是否可读写
#define BUGLY_INIT_STATHANDER_ERROR 2   /// 拉起crashpad_handler.exe失败，请检查handler路径是否存在，或者pipename已经被使用
#define BUGLY_INIT_PIPENAME_INVALID 3   /// 和crashpad_handler进程通信的pipename不可用
#define BUGLY_INIT_PIPENAME_EMPTY   4   /// InitBuglyManager2 获取可用的pipename失败,即pipename参数为空，dll共享内存的pipename也为空
#define BUGLY_INIT_PARAM_ERROR      5   /// InitBuglyManager2 结构传入的结构体参数不正确

#define BuglyServerHostType                 unsigned int

typedef void (*pfBuglyCrashCB)();

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

/// @brief 这里重新定义一份，字符串采用char* 方便C#和其他语言的类型转换
#pragma pack(1)
struct BuglyConfig_ {
    // 上报进程路径，绝对路径
    const char*        handler_;
    int                handle_len_;

    // crash 数据路径，绝对路径
    const char*        database_;
    int                database_len_;

    // crash 数据路径，绝对路径
    const char*        metrics_dir_;
    int                metrics_dir_len_;

    // app id, 请到bugly官网申请和查看
    const char*        app_id_;
    int                app_id_len_;

    // app key, 请到bugly官网申请和查看
    const char*        app_key_;
    int                app_key_len_;

    // 业务程序版本号
    const char*        app_version_;
    int                app_version_len_;

    // bundle id
    const char*        bundle_id_;
    int                bundle_id_len_;

    // 业务程序名称
    const char*        app_name_;
    int                app_name_len_;

    // 业务程序显示名称，如果crash了，会在bugly弹框上显示
    const char*        app_disp_name_;
    int                app_disp_name_len_;

    // 业务程序构件号
    const char*        build_id_;
    int                build_id_len_;

    // 初始用户id，供dau上报使用，当账号更新后可以调用SetKey通知Bugly更新
    const char*        account_id_;
    int                account_id_len_;

    // 和crashpad_handler.exe连接的管道名称，如果为nullptr,会随机生成一个，
    // 随机生成的可以用GetHandlerIPCPipeName接口获取
    const char*        pipename_;
    int                pipename_len_;

    // 业务传入的deviceId, 如果业务不传，bugly会随机生成一个
    const char*        deviceId_;
    int                deviceId_len_;

    // 最大等待时间，单位 ms
    unsigned int    timeout_;

    // 是否复用已有的crashpad_handler.exe进程，如果为false，就直接拉起新的crashpad_hander.exe
    BOOL            is_reuse_bugly_server_;

    // 是否弹窗提示用户 crash 信息
    BOOL            is_pop_dialog_;

    // 是否发送 crash 进程窗口及系统磁盘附加信息
    BOOL            is_need_attach_info_;

    // 是否上传, 如果为 false, 则只记录保存在本地
    BOOL            is_need_upload_;

    // 是否监控Bugly自身的crash
    BOOL            is_monitor_self_;

    // 是否由系统服务进程调用
    BOOL            is_server_process_;

    // 是否在业务进程抓取crash栈，如果业务程序需要加固，就需要传yes
    // 普通业务不需要，一般来说游戏业务需要
    BOOL            is_in_dll_mode_;

    // bugly上报域名，默认为WOA，也就是腾讯内部产品使用的类型
    // 腾讯外部产品需要设置为 kBuglyServerHostTypeBuglyPro
    // 海外版本需要使用 kBuglyServerHostTypeOversea
    // 腾讯云用户使用 kBuglyServerHostTypeTencentCloud
    BuglyServerHostType server_host_type_;
};
#pragma pack()

BUGLY_WIN_API int InitBuglyManager(struct BuglyConfig_ config);
   /**
   * @description: 获取 ipc 名称，需要在 InitBuglyManager 成功之后调用
   * @param pipe_name, 返回 pipe name
   * @param pipe_name_len pipe_name 缓冲区长度，建议用 MAX_PATH
   * @return: 0 为成功
   */
BUGLY_WIN_API int GetHandlerIPCPipeName(char* pipe_name, unsigned int pipe_name_len);

  /**
   * @description: 其它进程的初始化
   * @param pipe_name, 返回 pipe name
   * @return: 0 为成功
   * */
BUGLY_WIN_API int SetHandlerIPCPipe(const char* pipe_name, int pipe_name_len);

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
BUGLY_WIN_API int SetLogFilePathWithPipeName(const char* pipe_name, int pipe_name_len,
                 unsigned long process_id,
                 const char* tag,
                 unsigned int tag_len,
                 const char* log_file_path,
                 unsigned int log_file_path_len);


  /**
   * @description: 更新本进程的日志路径，方便 crash 的时候打捞 *
   * @param process_id 要更新日志文件路径的进程 id,
   * @param tag 日志 tag 名称, 同一个进程如果要打捞多份日志的化需要用 tag 进行区分
   * @param tag_len tag最大缓冲区，字符数，最大 20
   * @param log_file_path 日志文件路径,
   * @param log_file_path_len 日志文件路径最大缓冲区，字符数，最大 MAX_PATH
   * @return: 0 为成功
   *  */
BUGLY_WIN_API int SetLogFilePathWithPid(unsigned long process_id,
                            const char* tag,
                            unsigned int tag_len,
                            const char* log_file_path,
                            unsigned int log_file_path_len);

  /**
   * @description: 更新本进程的其他数据 *
   * @param pipe_name pipename,
   * @param key key,
   * @param key_len key 最大缓冲区，字符数, 最大 100
   * @param value value,
   * @param value_len value 最大缓冲区，字符数，最大 100
   * @return: 0 为成功
   *  */
BUGLY_WIN_API int SetKeyWithPipeName(const char* pipe_name, int pipe_name_len,
         const char* key,
         unsigned int key_len,
         const char* value,
         unsigned int value_len);


BUGLY_WIN_API int SetKey(const char* key, int key_len, const char* value, int value_len);

BUGLY_WIN_API void SetCrashCallback(pfBuglyCrashCB callback);

BUGLY_WIN_API int ReportCustomError(unsigned long process_id,
                        const char* tag,
                        unsigned int tag_len,
                        const char* err_file_path,
                        unsigned int err_file_path_len);

  /**
  * @description: 通用自定义错误上报接口，使用内存传递错误信息，对于可能触发很频繁的错误推荐使用该接口
  * @param addr "#0      _MyHomePageState._incrementCounter.<anonymous closure> (package:flutter_application_1/main.dart:81:33)"
  * @param message "type 'double' is not a subtype of type 'int' in type cast"
  * @param name "_CastError(Serious)"
  * @param call_stack full call_stack string
  * @param thread_name "main"
  * @param additional_info  后续扩展
  */
BUGLY_WIN_API int ReportCommonCustomError(const char* addr, int addr_len,
                                const char* message, int message_len,
                                const char* name, int name_len,
                                const char* call_stack, int call_stack_len,
                                const char* thread_name, int thread_name_len);

  /**
   * @description: 生成minidump时是否收集更多内存信息,打开这个会增加dmp文件大小
   * @param bEnable: 是否收集更多内存
   * @param maxSize: 收集额外内存大小限额,默认10MB
   */
BUGLY_WIN_API void SetEnableGatherIndirectlyReferencedMemory(BOOL bEnable, uint32_t maxSize = 10485760);
  

  /**
   * @description: 上报非crash的dump。或者业务自己捕获到的异常dump
   * @param context: 当前上下文
  */
BUGLY_WIN_API void ReportDumpWithoutCrash(const CONTEXT& context);

  /**
   * @description: 上报crash的dump。业务自己捕获到的异常dump可通过此接口上报；
   * 与ReportDumpWithoutCrash差异在于采集完成dump后自动关闭该进程
   * @param exception_pointers: 异常的上下文
   */
BUGLY_WIN_API void ReportDumpAndCrash(EXCEPTION_POINTERS* exception_pointers);


  /**
   * @description: 加载bugly日志模块,用于bugly自身的log输出
   * @param log_dir: 存放log文件夹的路径,这里需要业务传入是因为如果是服务进程拉起的，可能获取不到temp目录。最终的log文件会放在log_dir/logs目录下。因为log_dir下还会存一些配置文件。
   * @param dllpath: bugly_logger.dll的路径
  */
BUGLY_WIN_API BOOL LoadBuglyLoggerModule(const char* log_dir, int log_dir_len, const char* dllpath, int dllpath_len);

  /**
   * @description: 设置上报的自定义字段,限制和setkey一样，最大长度都是100
   * @param key: 自定义字段的key
   * @param value: 自定义字段的value，如果value为null或者空字符串，则删除对应的kv
  */
BUGLY_WIN_API void SetCustomField(const char* key, int key_len, const char* value, int value_len);


BUGLY_WIN_API void SetCustomFieldWithPipeName(const char* pipe_name, int pipe_name_len,
                              const char* key,
                              uint32_t key_len,
                              const char* value,
                              uint32_t value_len);

  /**
   * @description: 上报所有线程的调用栈。默认只上报crash线程的调用栈
   * @param bEnable: 是否上报所有线程调用栈
  */
BUGLY_WIN_API void SetEnableDumpAllThreadCallstack(BOOL bEnable);
  
  /**
   * @description: 开始卡顿监控
  */
BUGLY_WIN_API void StartLagMonitor();

  /**
   * @description: 停止卡顿监控
  */
BUGLY_WIN_API void StopLagMonitor();
  
  /**
   * @description: 暂停卡顿监控， 跟停止监控的区别在于这个函数不销毁监控对象
  */
BUGLY_WIN_API void PauseLagMonitor();
  
  /**
   * @description: 恢复卡顿监控， 调用pause后可以调用此函数来恢复
  */
BUGLY_WIN_API void ResumeLagMonitor();

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
BUGLY_WIN_API BOOL GenCanonicalPipeName(const char* buzPipename, int buzPipename_len,
                            const char* buglyAppid, int buglyAppid_len,
                            char* outPipeName,
                            unsigned int pipeNameLen);
  
  /**
   * @description: 查询当前是否有指定buzPipename和buglyAppid拉起的bugly守护进程
   * @param buzPipename: 用于拉起bugly守护进程的buzPipename
   * @param buglyAppid: 用于拉起bugly守护进程的buglyAppid
   * @return 如果当前已经存在对应的bugly守护进程运行，返回true。否则返回false
   */
BUGLY_WIN_API BOOL BuglyServerExistsAndReady(const char* buzPipename, int buzPipename_len, const char* buglyAppid, int buglyAppid_len);

  /**
   * @description: 查询当前是否由指定pipename拉起的bugly守护进程
   * @param canonicalPipename: 正规的windows pipename，由GenCanonicalPipeName生成
   * @return 如果当前已经存在对应的bugly守护进程运行，返回true。否则返回false
   */
BUGLY_WIN_API BOOL BuglyServerExistsAndReady2(const char* canonicalPipename, int canonicalPipename_len);

  /**
   * @description: 注册bugly_extra_handler.dll，用于EnablExtraHandler
   * @param modulePath: bugly_extra_handler.dll的路径
   * @return 成功返回TREU，否则返回FALSE
   */
BUGLY_WIN_API BOOL RegisterBuglyExtraHandlerA(const char* modulePath, int modulePath_len);

  /**
   * @description: 反注册bugly_extra_handler.dll，用于EnablExtraHandler
   * @param modulePath: bugly_extra_handler.dll的路径
   * @return 成功返回TREU，否则返回FALSE
   */
BUGLY_WIN_API BOOL UnregisterBuglyExtraHandlerA(const char* modulePath, int modulePath_len);

  /**
   * @description:
   * 开启额外的捕获机制，默认为关闭，开启以后可以上报strcpy_s一类的安全函数抛出的非法参数崩溃和虚函数调用等purecall错误
   * @param bExtraHandlerEnable: 是否开启额外的捕获机制
   */
BUGLY_WIN_API BOOL EnablExtraHandler(BOOL bExtraHandlerEnable, const char* modulePath, int modulePath_len);

  /**
   * @description:
   * 进程启动时可以额外上报DAU，用于统计指定进程的crash率
   * @param buzProcessName: 进程标识，可以时进程名，也可以是进程类型，由业务自行决定。
   * @param processId: 进程ID
   * @return 成功返回true，否则返回false
   */
BUGLY_WIN_API BOOL ReportProcessDau(const char* buzProcessName, int buzProcessName_len, uint64_t processId, const char* pipename, int pipename_len);

  /**
   * @description: 给子进程设置对应的appInfo。比如如果需要在子进程开启卡顿监控，就需要在子进程先设置这个
   */
BUGLY_WIN_API void SetAppBaseInfo(const char* app_id, int app_id_len,
                    const char* app_key, int app_key_len,
                    const char* bundle_id, int bundle_id_len,
                    const char* app_ver, int app_ver_len,
                    const char* process_name, int process_name_len,
                    const char* account_id, int account_id_len,
					          const char* device_id, int device_id_len);

  /**
   * @description: 开启业务进程更新配置的通知
   * 开启后才能收到配置更新的通知，某些监控项比如卡顿监控，需要在业务的主线程中启动和关闭，就需要
   * 打开该通知
   * @param bNeedsUpdateConfig: 是否需要马上同步一次配置, 如果该接口在InitBugly前调用，就可以填falst，因为Bugly加载默认配置后也会发通知
   *                            如果是子进程后续启动，要填true
   */
BUGLY_WIN_API void EnableClientConfigUpdateNotify(BOOL bNeedsUpdateConfig = false);

	/**
	 * @description: 更新场景信息，Bugly最多存储256个历史场景信息
	 * @param scene: 场景id，最长MAX_PATH。错误，崩溃，卡顿可以根据这个进行聚类。
	*/
BUGLY_WIN_API void SetScene(const char* scene, int scene_len);
BUGLY_WIN_API void SetSceneWithPipeName(const char* pipename, int pipename_len, const char* scene, int scene_len);

#ifndef NDEBUG
	/**
	 * @description: 触发测试crash，只有debug版本生效
	 * @param type: crash类型，一般填0
	*/
BUGLY_WIN_API void BuglyTestCrash(int type);
#endif // NDEBUG

#ifdef  __cplusplus
}
#endif  // __cplusplus


#endif  //!__BUGLY_AGENT_C__H__