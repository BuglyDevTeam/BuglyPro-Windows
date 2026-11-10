/**
**********************************************************************************************************************************************************************************************************************************
* @file:	BuglyWindowsInterface.cs
* @author:	hodxiang
* @date:	2025年10月27日 21:50:10 星期一
* @brief:	bugly C# 接口示例文件
**********************************************************************************************************************************************************************************************************************************
**/

using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Bugly
{
    internal static class BuglyWindowsInterface
    {
        #region 示例代码

        /// 获取字符串的字节长度,默认都是UTF8编码
        private static int GetStringBytesLen(string  str) {
            if (str == null || string.IsNullOrEmpty(str)) {
                return 0;
            }
            return Encoding.UTF8.GetByteCount(str);
        }


        /// <summary>
        /// crash 回调函数
        /// </summary>
        static void MyCrashCallback()
        {
            Console.WriteLine("Bugly on MyCrashCallback");
            /// 在crash回调用可以设置需要上传的日志文件路径。上传后可以在附件中查看日志
            /// string logFilePath1 = "C:\\Users\\bf_hodxiang\\AppData\\Local\\Temp\\Tencent\\buglyLog\\logs\\CSharpLog_2025102816.xlog";
            /// string logFilePath2 = "C:\\Users\\bf_hodxiang\\AppData\\Local\\Temp\\Tencent\\buglyLog\\logs\\bugly_handler_log_MIAMXOEGKRIYAFDN_2025102723.xlog";
            /// SetLogFilePathWithPid((ulong)System.Diagnostics.Process.GetCurrentProcess().Id, "tag1", 4, logFilePath1, GetStringBytesLen(logFilePath1));
            /// SetLogFilePathWithPid((ulong)System.Diagnostics.Process.GetCurrentProcess().Id, "tag2", 4, logFilePath2, GetStringBytesLen(logFilePath2));
        }

        [DllImport("kernel32.dll", CharSet = CharSet.Unicode, SetLastError = true)]
        private static extern IntPtr LoadLibrary(string lpLibFileName);
       
        /// <summary>
        /// 定位Bugly插件所在的绝对路径
        /// </summary>
        /// <returns></returns>
        private static string GetBuglyPluginDir()
        {
            string exePath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            string buglyDir = Path.Combine(Path.GetDirectoryName(exePath), "Bugly");
            if (!Directory.Exists(buglyDir))
            {
                return null;
            }
            return buglyDir;
        }

        /// <summary>
        /// 定位bugly数据所在的路径，这里会包含bugly捕获生成的dump文件，日志文件等等
        /// </summary>
        /// <returns></returns>
        private static string GetBuglyDataDir()
        {
            string tempPath = Path.GetTempPath();
            string buglyDataDir = Path.Combine(tempPath, "Tencent", "ycgame");
            return buglyDataDir;
        }

        /// <summary>
        /// 提前加载Bugly插件的dll
        /// </summary>
        /// <returns></returns>
        private static bool LoadBuglyDll()
        {
            bool result = false;
            do {
                string buglyDir = GetBuglyPluginDir();
                if (buglyDir == null) {
                    break;
                }

                string client_ext_path = Path.Combine(buglyDir, "client_extension.dll");
                if (!System.IO.File.Exists(client_ext_path)) {
                    break;
                }

                IntPtr handle = LoadLibrary(client_ext_path);
                if (handle == IntPtr.Zero) {
                    break;
                }

                string logger_dll_path = Path.Combine(buglyDir, "bugly_logger.dll");
                if (!System.IO.File.Exists(logger_dll_path)) {
                    break;
                }
                handle = LoadLibrary(logger_dll_path);
                if (handle == IntPtr.Zero) {
                    break;
                }

                result = true;
            } while (false);
            return result;
        }

        /// <summary>
        /// 示例初始化Bugly的方法，业务可以根据自己的情况修改
        /// </summary>
        public static void InitBugly(string appId, string appKey, string appVersion, string appBundleId)
        {
            /// 如果Bugly插件目录不存在，就直接返回，不要继续初始化bugly了。
            if (!LoadBuglyDll()) return;

            BuglyConfig_ config = new BuglyConfig_();
            /// 获取Bugly模块crashpad_handler.exe的路径
            string buglyDir = GetBuglyPluginDir();
            string crashpad_handler_path = Path.Combine(buglyDir, "crashpad_handler.exe");

            config.handler_ = crashpad_handler_path;
            config.handler_len_ = GetStringBytesLen(crashpad_handler_path);

            /// 获取crash dump文件存放路径,该路径必须可写
            string buglyDataDir = GetBuglyDataDir(); /// "C:\\Users\\bf_hodxiang\\AppData\\Local\\Temp\\Tencent\\ycgame
            config.database_ = buglyDataDir;
            config.database_len_ = GetStringBytesLen(buglyDataDir);

            config.metrics_dir_ = config.database_;
            config.metrics_dir_len_ = GetStringBytesLen(config.database_);
            config.app_id_ = appId;
            config.app_id_len_ = GetStringBytesLen(appId);
            config.app_key_ = appKey;
            config.app_key_len_ = GetStringBytesLen(appKey);
            config.app_version_ = appVersion;
            config.app_version_len_ = GetStringBytesLen(appVersion);
            config.bundle_id_ = appBundleId;
            config.bundle_id_len_ = GetStringBytesLen(appBundleId);
            config.app_name_ = "C#测试程序";
            config.app_name_len_ = GetStringBytesLen(config.app_name_);
            config.app_disp_name_ = "C#测试程序";
            config.app_disp_name_len_ = GetStringBytesLen(config.app_disp_name_);
            config.build_id_ = "999.999.999";
            config.build_id_len_ = GetStringBytesLen(config.build_id_);

            /// 初始账号，业务可以每次登录后将userid保存起来，下次启动时传递给bugly，后续用户真正登录
            /// 后，再调用SetKey("account_id", "userid")更新真正的userid
            config.account_id_ = "init_account_id";
            config.account_id_len_ = GetStringBytesLen(config.account_id_);
            config.server_host_type_ = kBuglyServerHostTypeWOA;

            /// 如果是游戏，且接入了ACE加固，这里必须设置为true
            /// config.is_in_dll_mode_ = true;  

            int ret = InitBuglyManager(config);
            Console.WriteLine("initBugly result:" + ret);

            /// 其他功能项的开启
            /// 可选，除了crsah线程以外，额外将其他所有线程调用栈都报上去。
            SetEnableDumpAllThreadCallstack(true);

            /// minidump收集indirectlyRefMemory, 第二个参数为额外收集内存的大小限额,不填的话默认就是10MB
            SetEnableGatherIndirectlyReferencedMemory(true);

            /// 设置渠道信息
            string app_channel_key = "app_channel";
            string app_channel_value = "MyChannel";
            SetKey(app_channel_key, GetStringBytesLen(app_channel_key), app_channel_value, GetStringBytesLen(app_channel_value));

            /// 更新UerId
            string user_id_key = "account_id";
            string user_id_value = "hodxiang";
            SetKey(user_id_key, GetStringBytesLen(user_id_key), user_id_value, GetStringBytesLen(user_id_value));


            ///开启额外的捕获机制，默认为关闭，开启以后可以上报strcpy_s一类的安全函数抛出的非法参数崩溃和虚函数调用等purecall错误
            string buglyExtraHandlerPath = Path.Combine(buglyDir, "bugly_extra_handler.dll");
            /// 注意，如果使用 crashpad_handler.exe --bugly-register-extra-handler 1 注册过，那这里就不用再次注册了
            RegisterBuglyExtraHandlerA(buglyExtraHandlerPath, GetStringBytesLen(buglyExtraHandlerPath));
            EnablExtraHandler(true, buglyExtraHandlerPath, GetStringBytesLen(buglyExtraHandlerPath));

            /// 可选，设置crash回调函数，当bugly捕获到crash后会回调该函数
            BuglyCrashCallbackDelegate crashCallback = new BuglyCrashCallbackDelegate(MyCrashCallback);
            SetCrashCallback(crashCallback);
        }


        /// <summary>
        /// 示例初始化日志诊断系统的方法，业务可以根据自己的情况修改
        /// </summary>
        public static void InitBuglyDiagnose(string appId, string appKey, string appBundleId, string appVersion)
        {
            string logDir = Path.Combine(GetBuglyDataDir(), "buglyLog"); // Temp\Tencent\ycgame\buglyLog

            string logPerfix = "CSharpLog";
            string[] dirs = new string[1];
            dirs[0] = logDir;
            string pubkey = "default";
            int ret = BuglyInitLogger(logDir, GetStringBytesLen(logDir), 
                logPerfix, GetStringBytesLen(logPerfix), 
                kBuglyLogLevel_Verbos, 
                pubkey, GetStringBytesLen(pubkey), 
                1);
            Console.WriteLine("BuglyInitLogger ret: {0}", ret);

            string buglyDiagnoseSdkVersion = "123.456";
            ret = BuglyInitDiagnose(appId, GetStringBytesLen(appId), 
                appKey, GetStringBytesLen(appKey), 
                appBundleId, GetStringBytesLen(appBundleId), 
                appVersion, GetStringBytesLen(appVersion), 
                buglyDiagnoseSdkVersion, GetStringBytesLen(buglyDiagnoseSdkVersion), 
                dirs, dirs.Length, 
                logDir, GetStringBytesLen(logDir), 
                pubkey, GetStringBytesLen(pubkey), 
                kLogServerHostDefalut);
            Console.WriteLine("BuglyInitDiagnose ret: {0}", ret);
            string userId = "hodxiang";
            BuglySetUserId(userId, GetStringBytesLen(userId));
        }

        /// <summary>
        /// 示例主动上传log
        /// </summary>
        public static void UploadLogFiles()
        {
            /// 这里上传bugly自己的log，业务可以按需修改
            string tempPath = Path.GetTempPath();
            string logDir = Path.Combine(Path.Combine(tempPath, "Tencent"), "buglyLog"); // Temp\Tencent\buglyLog

            string[] logFiles = new string[1];
            logFiles[0] = logDir;
            string tag = "C# 日志上传测试";
            string summary = "C# 日志上传测试文件summary";
            string queryKey = "123";
            string sourcePlatform = "Windows";
            BuglyUploadLogFiles(logFiles, logFiles.Length, 
                tag, GetStringBytesLen(tag), 
                summary, GetStringBytesLen(summary), 
                queryKey, GetStringBytesLen(queryKey), 
                sourcePlatform, GetStringBytesLen(sourcePlatform));
        }

        #endregion //示例代码

        #region 对外接口

        /// <summary>
        /// 处理C# Exception,生成Bugly可识别的堆栈信息
        /// </summary>
        public static void HandlerException(Object sender, Exception e)
        {
            Console.WriteLine("on HandlerException: " + e.ToString());
            string name = e.GetType().FullName;
            string sender_name = sender.ToString();
            string source = e.Source;
            string message = e.Message;


            StringBuilder stackTraceBuilder = new StringBuilder();
            StackTrace stackTrace = new StackTrace(e, true);
            int count = stackTrace.FrameCount;

            for (int i = 0; i < count; i++)
            {
                StackFrame frame = stackTrace.GetFrame(i);
                stackTraceBuilder.AppendFormat("{0}.{1}", frame.GetMethod().DeclaringType.Name, frame.GetMethod().Name);
                ParameterInfo[] parameters = frame.GetMethod().GetParameters();
                if (parameters == null || parameters.Length == 0)
                {
                    stackTraceBuilder.Append(" () ");
                }
                else
                {
                    stackTraceBuilder.Append(" (");
                    int pcount = parameters.Length;
                    ParameterInfo param = null;
                    for (int p = 0; p < pcount; p++)
                    {
                        param = parameters[p];
                        stackTraceBuilder.AppendFormat("{0} {1}", param.ParameterType.Name, param.Name);

                        if (p != pcount - 1)
                        {
                            stackTraceBuilder.Append(", ");
                        }
                    }
                    param = null;
                    stackTraceBuilder.Append(") ");
                }
                string fileName = frame.GetFileName();
                if (!string.IsNullOrEmpty(fileName) && !fileName.ToLower().Equals("unknown"))
                {
                    fileName = fileName.Replace("\\", "/");
                    int loc = fileName.ToLower().IndexOf("/assets/");
                    if (loc < 0)
                    {
                        loc = fileName.ToLower().IndexOf("assets/");
                    }
                    if (loc > 0)
                    {
                        fileName = fileName.Substring(loc);
                    }
                    stackTraceBuilder.AppendFormat("(at {0}:{1})", fileName, frame.GetFileLineNumber());
                }
                stackTraceBuilder.AppendLine();
            }

			string call_stack = stackTraceBuilder.ToString();
			string thread_name = "main";
			
            ReportCommonCustomError(source, GetStringBytesLen(source), 
                message, GetStringBytesLen(message), 
                name, GetStringBytesLen(name), 
                call_stack, GetStringBytesLen(call_stack), 
                thread_name, GetStringBytesLen(thread_name));
        }

        /// <summary>
        /// Bugly crash/error 上报的域名
        /// </summary>
        public static readonly uint kBuglyServerHostTypeWOA = 0;
        public static readonly uint kBuglyServerHostTypeBuglyPro = 1;
        public static readonly uint kBuglyServerHostTypeOversea = 2;
        public static readonly uint kBuglyServerHostTypeTencentCloud = 3;

        /// <summary>
        /// Bugly初始化信息结构体
        /// </summary>
        [StructLayout(LayoutKind.Sequential, Pack = 1)]
        public class BuglyConfig_
        {
            // 上报进程路径，绝对路径
            public string handler_;
            public int 	  handler_len_ = 0;

            // crash 数据路径，绝对路径
            public string database_;
            public int 	  database_len_ = 0;

            // crash 数据路径，绝对路径
            public string metrics_dir_;
            public int metrics_dir_len_ = 0;

            // app id, 请到bugly官网申请和查看
            public string app_id_;
            public int app_id_len_ = 0;

            // app key, 请到bugly官网申请和查看
            public string app_key_;
            public int app_key_len_ = 0;

            // 业务程序版本号
            public string app_version_;
            public int app_version_len_ = 0;

            // bundle id
            public string bundle_id_;
            public int bundle_id_len_ = 0;

            // 业务程序名称
            public string app_name_;
            public int app_name_len_ = 0;

            // 业务程序显示名称，如果crash了，会在bugly弹框上显示
            public string app_disp_name_;
            public int app_disp_name_len_ = 0;

            // 业务程序构件号
            public string build_id_;
            public int build_id_len_ = 0;

            // 初始用户id，供dau上报使用，当账号更新后可以调用SetKey通知Bugly更新
            public string account_id_;
            public int account_id_len_ = 0;

            // 和crashpad_handler.exe连接的管道名称，如果为nullptr,会随机生成一个，
            // 随机生成的可以用GetHandlerIPCPipeName接口获取
            public string pipename_;
            public int pipename_len_ = 0;

            // 业务传入的deviceId, 如果业务不传，bugly会随机生成一个
            public string deviceId_;
            public int deviceId_len_ = 0;

            // 最大等待时间，单位 ms
            public uint timeout_ = 5000;

            // 是否复用已有的crashpad_handler.exe进程，如果为false，就直接拉起新的crashpad_hander.exe
            public bool is_reuse_bugly_server_ = true;

            // 是否弹窗提示用户 crash 信息
            public bool is_pop_dialog_ = true;

            // 是否发送 crash 进程窗口及系统磁盘附加信息
            public bool is_need_attach_info_ = true;

            // 是否上传, 如果为 false, 则只记录保存在本地
            public bool is_need_upload_ = true;

            // 是否监控Bugly自身的crash
            public bool is_monitor_self_ = false;

            // 是否由系统服务进程调用
            public bool is_server_process_ = false;

            // 是否在业务进程抓取crash栈，如果业务程序需要加固，就需要传yes
            // 普通业务不需要，一般来说游戏业务需要
            public bool is_in_dll_mode_ = false;

            // bugly上报域名，默认为kBuglyServerHostTypeWOA，也就是腾讯内部产品使用的类型
            // 腾讯外部产品需要设置为 kBuglyServerHostTypeBuglyPro
            // 海外版本需要使用 kBuglyServerHostTypeOversea
            // 腾讯云用户使用 kBuglyServerHostTypeTencentCloud
            public uint server_host_type_ = kBuglyServerHostTypeWOA;
        };


        #region Bugly C 接口声明
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int InitBuglyManager(BuglyConfig_ config);

        /**
        * @description: 通用自定义错误上报接口，使用内存传递错误信息，对于可能触发很频繁的错误推荐使用该接口
        * @param addr "#0      _MyHomePageState._incrementCounter.<anonymous closure> (package:flutter_application_1/main.dart:81:33)"
        * @param message "type 'double' is not a subtype of type 'int' in type cast"
        * @param name "_CastError(Serious)"
        * @param call_stack full call_stack string
        * @param thread_name "main"
        */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int ReportCommonCustomError(string addr, int addr_len,
                                                string message, int message_len,
                                                string name, int name_len,
                                                string call_stack, int call_stack_len,
                                                string thread_name, int thread_name_len);

        /**
         * @description: 设置额外信息，包括userid，appchannel等等
         * @param key: key, 可选值: account_id, app_channel 等
         * @param value: 值
        */

        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetKey(string key, int key_len, string value, int value_len);

        /**
         * @description: 加载bugly日志模块,用于bugly自身的log输出
         * @param log_dir: 存放log文件夹的路径,这里需要业务传入是因为如果是服务进程拉起的，可能获取不到temp目录。最终的log文件会放在log_dir/logs目录下。因为log_dir下还会存一些配置文件。
         * @param dllpath: bugly_logger.dll的路径
        */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool LoadBuglyLoggerModule(string log_dir, int log_dir_len, string dllpath, int dllpath_len);

        /**
         * @description: 注册bugly_extra_handler.dll，用于EnablExtraHandler
         * @param modulePath: bugly_extra_handler.dll的路径
         */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool RegisterBuglyExtraHandlerA(string modulePath, int modulePath_len);


        /**
         * @description: 反注册bugly_extra_handler.dll，用于EnablExtraHandler
         * @param modulePath: bugly_extra_handler.dll的路径
         */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool UnregisterBuglyExtraHandlerA(string modulePath, int modulePath_len);

        /**
         * @description:
         * 开启额外的捕获机制，默认为关闭，开启以后可以上报strcpy_s一类的安全函数抛出的非法参数崩溃和虚函数调用等purecall错误
         * @param bExtraHandlerEnable: 是否开启额外的捕获机制
         * @param modulePath: bugly_extra_handler.dll的路径
         */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int EnablExtraHandler(bool bExtraHandlerEnable, string modulePath, int modulePath_len);

        /**
         * @description: 设置上报的自定义字段,限制和setkey一样，最大长度都是100
         * @param key: 自定义字段的key
         * @param value: 自定义字段的value，如果value为null或者空字符串，则删除对应的kv
        */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCustomField(string key, int key_len, string value, int value_len);


        /**
         * @description: 更新场景信息，Bugly最多存储256个历史场景信息
         * @param scene: 场景id，最长MAX_PATH。错误，崩溃，卡顿可以根据这个进行聚类。
        */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetScene(string scene, int scene_len);


        /**
         * @description: 生成minidump时是否收集更多内存信息,打开这个会增加dmp文件大小
         * @param bEnable: 是否收集更多内存
         * @param maxSize: 收集额外内存大小限额,默认10MB
         */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetEnableGatherIndirectlyReferencedMemory(bool bEnable, uint maxSize = 10485760);


        /**
         * @description: 上报所有线程的调用栈。默认只上报crash线程的调用栈
         * @param bEnable: 是否上报所有线程调用栈
        */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetEnableDumpAllThreadCallstack(bool bEnable);

        /**
         * @description: 设置上报crash时需要上传的日志文件
         * @param process_id: 该log文件关联的进程id，因为客户端可能有多个进程，不同的进程crash，需要上传不同的log文件
         *                    这里要传当前crash的进程id。
         * @param tag: tag，如果需要上传多个文件，每个tag都要不一样
         * @param tag_len: tag字符串的长度
         * @param log_file_path: 要上传的日志文件路径
         * @param log_file_path_len: log_file_path字符串的长度
        */

        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetLogFilePathWithPid(ulong process_id,
                                                       string tag,
                                                       uint tag_len,
                                                       string log_file_path,
                                                       uint log_file_path_len);

        /**
         * @description: crash时的回调函数，业务可以在回调函数中做一些事情，注意生命周期要是static的
        */
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void BuglyCrashCallbackDelegate();

        /**
         * @description: 设置crash时的回调函数，业务可以在回调函数中做一些事情
         * @param callback: 回调函数，注意生命周期，要static的
        */
        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCrashCallback(BuglyCrashCallbackDelegate callback);

        [DllImport("client_extension.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void BuglyTestCrash(int type);
        #endregion


        #region 日志 C 接口声明
        /// <summary>
        ///  Bugly 日志等级
        /// </summary>
        public static readonly int kBuglyLogLevel_All = 0;
        public static readonly int kBuglyLogLevel_Verbos = 0;
        public static readonly int kBuglyLogLevel_Debug = 1;
        public static readonly int kBuglyLogLevel_Info = 2;
        public static readonly int kBuglyLogLevel_Warn = 3;
        public static readonly int kBuglyLogLevel_Error = 4;
        public static readonly int kBuglyLogLevel_Fatal = 5;
        public static readonly int kBuglyLogLevel_None = 6;

        /**
         * @description 初始化bugly_logger.dll中自带的xlog，后续业务可以使用它记录log
         * @param logDir, 存储log的路径，注意要是可写入的路径。
         * @param logPerfix, 日志文件名的前缀，可以用来区分不同的log
         * @param logLevel, 日志等级
         * @param pubkey, 日志加密Key，如果传null表示不加密，传default表示用默认key加密
         * @param enableConsoleLog 是否将log输出到控制台,1 为 输出，0为不输出
         * @return: 1 为成功，0 为失败
         */
        [DllImport("bugly_logger.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BuglyInitLogger(string logDir, int logDir_len,
                                                  string logPerfix, int logPerfix_len,
                                                  int logLevel,
                                                  string pubkey, int pubkey_len,
                                                  int enableConsoleLog);


        public static readonly int kLogServerHostDefalut = 0;      /// woa正式环境
        public static readonly int kLogServerHostTest = 1;         /// woa测试环境
        public static readonly int kLogServerHostOversea = 2;      /// 海外正式环境
        public static readonly int kLogServerHostBuglyPro = 3;     /// 专业版环境
        public static readonly int kLogServerHostTencentCloud = 4; /// 腾讯云环境
        /**
         * @description 初始化bugly_logger.dll中的诊断模块，后续业务可以使用它下发命令捞取log
         * @param appId, appId。
         * @param appKey, appKey
         * @param bundleId, app bundle id
         * @param appVersion, app 版本号
         * @param buglyDiagnoseSdkVersion bugly_logger.dll的版本号
         * @param logDirs, 上报日志时扫描的路径，业务下发捞日志命令后客户会在这些目录下查找匹配的日志文件
         *                  并打包上传。注意：路径必须以'\0'结尾!!
         * @param logDirsNum, logDirs中路径数量
         * @param diagnoseConfigDir, 诊断系统存放配置的路径，该路径必须可写
         * @param logPubKey, 日志加密Key，如果传null表示不加密，传default表示用默认key加密
         * @param serverHostType, 上报域名
         * @return: 1 为成功，0 为失败
         */
        [DllImport("bugly_logger.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int BuglyInitDiagnose(string appId, int appId_len, 
                                                    string appKey, int appKey_len,
                                                    string bundleId, int bundleId_len,
                                                    string appVersion, int appVersion_len,
                                                    string buglyDiagnoseSdkVersion, int buglyDiagnoseSdkVersion_len,
                                                    string[] logDirs,
                                                    int logDirsNum,
                                                    string diagnoseConfigDir, int diagnoseConfigDir_len,
                                                    string logPubKey, int logPubKey_len,
                                                    int serverHostType);

        /**
         * @description 设置userId，这个填写在捞log的"设备ID"处
         * @param userId, 也就时捞log时填的"设备ID"
         */
        [DllImport("bugly_logger.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void BuglySetUserId(string userId, int userId_len);

        /**
         * @description 添加日志搜索路径，用于捞取日志时搜索符合条件的日志
         * @param logSearchPath, 日志搜索路径
         */
        [DllImport("bugly_logger.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void BuglyAddLogSearchPath(string logSearchPath, int logSearchPath_len);

        /**
         * @description 设置捞取日志时是否递归搜索日志目录，如果不设置默认为true
         * @param recursive 是否递归搜索addLogPath传入的目录，如果不设置默认为true
         */
        [DllImport("bugly_logger.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void BuglySetRecursiveSearchLogs(int recursive);

        /** 
         * @brief 主动上报日志接口
         * @param files, 待上报的log文件路径,注意：路径必须以'\0'结尾!!
         * @param count, 待上报的log文件数量
         * @param tag, 标签，必填，转换成char*后的长度不超过64 bytes
         * @param summary, 摘要信息，选填，转换成char*后的长度不超过256 bytes
         * @param queryKey, 检索Key，选填，用于平台互通检索上报信息
         * @param sourcePlatform, 来源平台，选填，用于平台互通记录上报来源
        */
        [DllImport("bugly_logger.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void BuglyUploadLogFiles(string[] files,
                                                       int count,
                                                       string tag, int tag_len,
                                                       string summary, int summary_len,
                                                       string queryKey, int queryKey_len,
                                                       string sourcePlatform, int sourcePlatform_len);
        #endregion
#endregion
    }
}
