// BuglyDemo.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "BuglyDemo.h"

#include <string>
#include <thread>
#include <locale>
#include <chrono>

/// bugly 相关头文件
#include "bugly_agent.h"

/// 日志上报和捞取相关头文件
#include "logger\bugly_diagnose_c.h"

/// 日志打印相关头文件
#include "logger\bugly_logger_c.h"


/// appid和appkey，专业版去这里创建产品
/// https://bugly.tds.qq.com/v2/index/main
#define BUGLY_APP_ID  L"3b22d90459"			/// 这里为体验demo，链接为https://bugly.tds.qq.com/v2/overview?productId=3b22d90459&pid=10&tab=dashboardTime&startDate=2026-04-07&endDate=2026-05-06
#define BUGLY_APP_KEY L"8496e5ce-066e-478e-82b9-4a97fc8cff3a"
#define BUGLY_APP_BUNDLE_ID L"com.your.app"
#define BUGLY_INIT_USER_ID L"InitUserId"

/// 日志加密的key，如果传空，就是不加密
 #define BUGLY_LOG_PUBKEY "0408ae518b8f48e758ea0b03ff06a9f3690e41d45fb19d5d6fb4e90c78f8a9504fb07854d55051c2eb0d8e8c9f79d21d7984cb6fc4929f33db98f50ee4282b9bae"
//#define BUGLY_LOG_PUBKEY ""

#define MAX_LOADSTRING 100


std::string WstringToString(const std::wstring& wstr) {
    std::string result;
    if (wstr.empty()) {
        return result;
    }
    std::setlocale(LC_ALL, "");
    int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    if (len <= 0) {
        return result;
    }
    result.resize(len);
    len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &result[0], (int)result.size(), NULL, NULL);
    if (len <= 0) {
        result = "";
    }
    return result;
}

std::string WstringToStringACP(const std::wstring& wstr) {
    std::string result;
    if (wstr.empty()) {
        return result;
    }
    std::setlocale(LC_ALL, "");
    int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    if (len <= 0) {
        return result;
    }
    result.resize(len);
    len = ::WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), (int)wstr.size(), &result[0], (int)result.size(), NULL, NULL);
    if (len <= 0) {
        result = "";
    }
    return result;
}

std::wstring GenBuglyLogDirW() {
	wchar_t tmpPathBuffer[1024] = { 0 };
	::GetTempPath(1024, tmpPathBuffer);
	std::wstring result = tmpPathBuffer;
	return result + L"BuglyDemo\\buglyLog";
}

std::string GenBuglyLogDirA() {
	char tmpPathBuffer[1024] = { 0 };
	::GetTempPathA(1024, tmpPathBuffer);
	std::string result = tmpPathBuffer;
	return result + "BuglyDemo\\buglyLog";
}

std::wstring GenBuglyDataDirW() {
	wchar_t tmpPathBuffer[1024] = { 0 };
	::GetTempPath(1024, tmpPathBuffer);
	std::wstring result = tmpPathBuffer;
	return result + L"BuglyDemo";
}

/// <summary>
/// 获取crashpad_handler.exe所在的路径
/// </summary>
std::wstring GenHandlerPathW() {
	std::wstring path = getExecutableDir();
	path += L"\\crashpad_handler.exe";
	return path;
}

constexpr static const char* kLogTag = "BuglyDemoLog";

/// <summary>
/// crash 回调函数，bugly捕获到crash后，会先回调给业务
/// 业务可以在这里添加一些日志，连同crash一起上报到bugly后台
/// </summary>
void CrashCallback(IBuglyAgent* agent, void* context) {
	if (nullptr == agent) {
		return;
	}

	std::wstring log_path = L"C:\\your_apps_log_path\\your_log_file.log";
	agent->SetLogFilePath(GetCurrentProcessId(), L"Log1", 4, log_path.c_str(), log_path.size());
}

extern "C" {
typedef bool (*RegisterBuglyExtraHandlerT)(const wchar_t*);
}
bool RegisterBuglyExtraHandlerImpl(const wchar_t* path) {
	HMODULE hMod = ::LoadLibraryW(L"client_extension.dll");
	if (NULL == hMod) {
		return false;
	}

	RegisterBuglyExtraHandlerT fun_ptr = (RegisterBuglyExtraHandlerT)::GetProcAddress(hMod, "RegisterBuglyExtraHandler");
	if (nullptr == fun_ptr) {
		return false;
	}

	return fun_ptr(path);
}

/// <summary>
/// 初始化bugly，用于监控和上报 crash
/// </summary>
void InitBugly() {
    std::wstring handlerPath = GenHandlerPathW();
    std::wstring reportsDir = GenBuglyDataDirW();

	// 如果是监控windows系统服务进程，这个改成true，普通程序为false
	bool bIsServerProcess = false;
	std::wstring metricsDir = reportsDir;
	wchar_t pipe_name[MAX_PATH] = { 0 };
    IBuglyAgent* bugly_agent = nullptr;
    BuglyAgentResult result = CreateBuglyAgent(&bugly_agent);
    if (result != BuglyAgentResult::kSuccess) {
        return;
    }

    //  bugly_agent->InitBuglyManager(handlerPath.c_str(),
    //      reportsDir.c_str(),
    //      metricsDir.c_str(),
	//BUGLY_APP_ID,
	//BUGLY_APP_KEY,
    //      L"1.0.0.1",
	//BUGLY_APP_BUNDLE_ID,
    //      L"BuglyDemo",
    //      L"BuglyDemoApp",
    //      "1.0.1234",
    //      5000,
    //      true,
    //      true,
    //      true,
    //      false,
    //      bIsServerProcess
    //  );

    /// 推荐使用新的接口
	Bugly::BuglyConfig buglyConfig;
    buglyConfig.size_ = sizeof(Bugly::BuglyConfig);
    buglyConfig.handler_ = handlerPath.c_str();
    buglyConfig.database_ = reportsDir.c_str();
    buglyConfig.metrics_dir_ = metricsDir.c_str();
    buglyConfig.app_id_ = BUGLY_APP_ID;
    buglyConfig.app_key_ = BUGLY_APP_KEY;
    buglyConfig.app_version_ = L"3.18.0.2";
    buglyConfig.bundle_id_ = BUGLY_APP_BUNDLE_ID;
    buglyConfig.app_name_ = APP_NAME;
    buglyConfig.app_disp_name_ = APP_DISP_NAME;
    buglyConfig.build_id_ = L"123.456.789";
    buglyConfig.account_id_ = BUGLY_INIT_USER_ID;		/// 这里是初始化时的默认userid，业务注册bugly时可能还没有登录，这里推荐业务自己保存上一次登录的userid填到这里。
    buglyConfig.deviceId_ = nullptr;

    buglyConfig.is_server_process_ = false;	// 如果是普通app进程，传false，如果是系统服务进程，传true
    buglyConfig.server_host_type_ = kBuglyServerHostTypeBuglyPro;  // kBuglyServerHostTypeWOA; // 腾讯内部项目，用WOA版本

    int bugly_init_result = bugly_agent->InitBuglyManager3(&buglyConfig);

    if (bugly_init_result != BUGLY_INIT_NO_ERROR) {
        return ;
    }

    /// 真正登录完成后，设置userid，业务最好自己也保存一份到本地，下次初始化时可以填到上面account_id_中
	bugly_agent->SetKey(L"account_id", L"BuglyDemoUserId");

    const auto buglyLogDir = GenBuglyLogDirW();
    bugly_agent->LoadBuglyLoggerModule(buglyLogDir.c_str(), nullptr);

	// 中间可以通过下面的调用修改进程名
	//bugly_agent->SetKey(L"process_name", L"my_process_name.exeeeeee");

	bugly_agent->SetCrashCallback(CrashCallback, nullptr);
	// minidump收集indirectlyRefMemory
	bugly_agent->SetEnableGatherIndirectlyReferencedMemory(true);

	// 注册extra handler，用于监控strcpy_s一类安全函数抛出的异常以及虚函数调用purecall错误...
	std::wstring extraHandlerModule = getExecutableDir();
	extraHandlerModule += L"\\bugly_extra_handler.dll";
	// EnablExtraHandler要生效需要满足下面两个其中之一:
	// 1, 调用RegisterBuglyExtraHandler，这个方法会在运行时注册bugly_extra_handler(记得找机会调用UnregisterBuglyExtraHandler反注册)
	// 2, 应用程序安装时运行crashpad_handler.exe --bugly-register-extra-handler [user | global | all]进行注册，如果安装时注册了，那运行时就不用调用RegisterBuglyExtraHandler了
	//    注意， 上面的global和all选项需要管理员权限才会生效。具体情参考接入文档
	RegisterBuglyExtraHandlerImpl(extraHandlerModule.c_str());
	bugly_agent->EnablExtraHandler(true, extraHandlerModule.c_str());

	// 开启卡顿监控
	bugly_agent->EnableClientConfigUpdateNotify(true);
	bugly_agent->StartLagMonitor();
}

/// <summary>
///  上报自定义错误,这种错误不是crash，而是业务定义的一些错误，比如业务集成了lua捕获的lua错误，js错误等等
/// </summary>
void CustomErrorTest() {
	std::string addr =
		"#0      _MyHomePageState._incrementCounter.<anonymous closure> (package:flutter_application_1/main.dart:81:33)";
	std::string message = "type 'double' is not a subtype of type 'int' in type cast";
	std::string name = "_CastError(Serious)";
	std::string thread_name = "main";
	std::string call_stack =
		"#0      _MyHomePageState._incrementCounter.<anonymous closure> "
		"(package:flutter_application_1/main.dart:81:33)\n#1      State.setState "
		"(package:flutter/src/widgets/framework.dart:1088:30)\n#2      _MyHomePageState._incrementCounter "
		"(package:flutter_application_1/main.dart:73:5)\n#3      _InkResponseState._handleTap "
		"(package:flutter/src/material/ink_well.dart:989:21)\n#4      GestureRecognizer.invokeCallback "
		"(package:flutter/src/gestures/recognizer.dart:193:24)\n#5      TapGestureRecognizer.handleTapUp "
		"(package:flutter/src/gestures/tap.dart:608:11)\n#6      BaseTapGestureRecognizer._checkUp "
		"(package:flutter/src/gestures/tap.dart:296:5)\n#7      BaseTapGestureRecognizer.acceptGesture "
		"(package:flutter/src/gestures/tap.dart:267:7)\n#8      GestureArenaManager.sweep "
		"(package:flutter/src/gestures/arena.dart:157:27)\n#9      GestureBinding.handleEvent "
		"(package:flutter/src/gestures/binding.dart:444:20)\n#10     GestureBinding.dispatchEvent "
		"(package:flutter/src/gestures/binding.dart:420:22)\n#11     RendererBinding.dispatchEvent "
		"(package:flutter/src/rendering/binding.dart:278:11)\n#12     GestureBinding._handlePointerEventImmediately "
		"(package:flutter/src/gestures/binding.dart:374:7)\n#13     GestureBinding.handlePointerEvent "
		"(package:flutter/src/gestures/binding.dart:338:5)\n#14     GestureBinding._flushPointerEventQueue "
		"(package:flutter/src/gestures/binding.dart:296:7)\n#15     GestureBinding._handlePointerDataPacket "
		"(package:flutter/src/gestures/binding.dart:279:7)\n#16     _rootRunUnary (dart:async/zone.dart:1444:13)\n#17    "
		" _CustomZone.runUnary (dart:async/zone.dart:1335:19)\n#18     _CustomZone.runUnaryGuarded "
		"(dart:async/zone.dart:1244:7)\n#19     _invoke1 (dart:ui/hooks.dart:185:10)\n#20     "
		"PlatformDispatcher._dispatchPointerDataPacket (dart:ui/platform_dispatcher.dart:293:7)\n#21     "
		"_dispatchPointerDataPacket (dart:ui/hooks.dart:98:31)\n";

    IBuglyAgent* pBugly = nullptr;
    CreateBuglyAgent(&pBugly);
    if (nullptr == pBugly) {
        return;
    }

    /// 设置自定义字段
	pBugly->SetCustomField("custom field1", "1234");
	pBugly->SetCustomField("custom field1", "");
	pBugly->SetCustomField("custom field2", "hodxiang");
	pBugly->SetCustomField("custom field3", nullptr);
	pBugly->SetCustomField("", nullptr);

    /// 上报自定义错误
	pBugly->ReportCommonCustomError(addr.c_str(), message.c_str(), name.c_str(), call_stack.c_str(), thread_name.c_str());
}

/// <summary>
/// 初始化诊断系统，用于捞取和上报日志
/// </summary>
void InitDiagnose() {
	// 日志根目录
	const std::string logRootDir = WstringToString(GenBuglyLogDirW());
	// 日志文件目录，注意：日志文件目录下不要放其他东西，避免自动清理日志时误删
	const std::string logPath = logRootDir + "\\logs";
	// 日志配置文件目录，配置文件放在日志根目录就可以了
	const std::string logConfigPath = logRootDir;

	std::string pubKey = BUGLY_LOG_PUBKEY;

    // 日志目录，捞取日志时诊断系统会在以下目录中匹配日志文件并打包上传
	char* logDirs[1];
	logDirs[0] = const_cast<char*>(logPath.c_str());

  	const std::string appid_ = WstringToString(BUGLY_APP_ID);
  	const std::string appkey_ = WstringToString(BUGLY_APP_KEY);
  	const std::string appbundleId_ = WstringToString(BUGLY_APP_BUNDLE_ID);
  	const std::string appVersion_ = "3.16.0.1";
  	const std::string buglyDiagnoseSdkVersoin_ = "1.0.15";
	BuglyInitDiagnose(appid_.c_str(), appid_.size(), 
                   appkey_.c_str(), appkey_.size(), 
                   appbundleId_.c_str(), appbundleId_.size(), 
                   appVersion_.c_str(), appVersion_.size(),
                   buglyDiagnoseSdkVersoin_.c_str(), buglyDiagnoseSdkVersoin_.size(),
                   (const char**)logDirs,
                   1,
                   logRootDir.c_str(), logRootDir.size(),
                   pubKey.c_str(), pubKey.size(), 
                   kBuglyLogServerHostTypeDBugly);

    /// 注意，这里一定要设置UserId，这里的UserId就是Bugly捞取日志页面上的“设备ID”
    const std::string userid = "BuglyDemoUserId";
    BuglySetUserId(userid.c_str(), userid.size());
}

/// <summary>
/// 递归遍历目录，收集所有日志文件
/// </summary>
std::vector<std::string> GetAllLogFiles(const std::string& logPath) {
	std::vector<std::string> result;

	struct stat tmpInfo;
	if (stat(logPath.c_str(), &tmpInfo) != 0) {
		return result;
	}

	bool isDir = tmpInfo.st_mode & S_IFDIR;
	bool isFile = tmpInfo.st_mode & S_IFREG;

	if (isFile) {
		result.push_back(logPath);
	}
	else if (isDir) {
		WIN32_FIND_DATAA findFileData;
		std::string p_dir = logPath;
		std::string find_path = p_dir + "\\*.*";

		HANDLE hFind = ::FindFirstFileA(find_path.c_str(), &findFileData);
		if (hFind == INVALID_HANDLE_VALUE) {
			return result;
		}
		while (true) {
			if ((strcmp(findFileData.cFileName, ".") != 0) && (strcmp(findFileData.cFileName, "..") != 0)) {
				std::string sub_p = p_dir + "\\" + findFileData.cFileName;
				if (stat(sub_p.c_str(), &tmpInfo) == 0) {
					isDir = tmpInfo.st_mode & S_IFDIR;
					isFile = tmpInfo.st_mode & S_IFREG;

					if (isFile) {
						result.push_back(sub_p);
					}
					else if (isDir) {
						auto s_r = GetAllLogFiles(sub_p);
						result.insert(result.end(), s_r.begin(), s_r.end());
					}
				}
			}
			if (!::FindNextFileA(hFind, &findFileData)) {
				break;
			}
		}
	}
	return result;
}

/// <summary>
/// 主动上报log
/// </summary>
void HandleUploadLogs() {
	std::string logPath = WstringToStringACP(GenBuglyLogDirW());
	while (logPath[logPath.size() - 1] == '\\') {
		logPath.erase(logPath.size() - 1);
	}

	auto logFileList = GetAllLogFiles(logPath);
	if (logFileList.empty()) {
		return;
	}

	char** logFiles = (char**)malloc(logFileList.size() * sizeof(char*));
	int idx = 0;
	for (const auto& lf : logFileList) {
		logFiles[idx++] = const_cast<char*>(lf.c_str());
	}

	/// 这里的日志文件路径既可以传文件路径，就像前面用GetAllLogFiles获取所有文件列表一样
	/// 也可以直接传目录路径，BuglyDiagnose内部会自动递归遍历所有遇到的目录

    const std::string tag("bugly_logger.dll c interface");
    const std::string summary("some test log for upload log interface!");

    BuglyUploadLogFiles(const_cast<const char**>(logFiles), idx, 
                        tag.c_str(), tag.size(), 
                        summary.c_str(), summary.size(),
                        NULL, 0, NULL, 0);
}

/// <summary>
/// 初始化日志组件,用于打印日志
/// </summary>
void InitLogger() {
    // 日志根目录
    std::string logRootDir = GenBuglyLogDirA();
    // 日志文件目录，注意：日志文件目录下不要放其他东西，避免自动清理日志时误删
	std::string logPath = logRootDir + "\\logs";
    // 日志配置文件目录，配置文件放在日志根目录就可以了
	std::string logConfigPath = logRootDir;

	// 日志加密key，可以去bugly的项目设置里生成，也可以不加密
	std::string pubKey = BUGLY_LOG_PUBKEY;

    // 日志文件前缀，不同的模块可以通过创建不同的日志子实例，写入不同的日志文件
	std::string logPerfix = "test_demo";

	int defualtLogLevel = kBuglyLogLevel_Info;
	bool enableConsolLog = false;
    // 日志等级设置
#ifndef NDEBUG
	defualtLogLevel = kBuglyLogLevel_All;
	enableConsolLog = true;
#endif

  // 初始化日志模块，调用这里以后就可以打印日志了
	BuglyInitLogger(logRootDir.c_str(), logRootDir.size(), 
                 logPerfix.c_str(), logPerfix.size(), 
                 defualtLogLevel, 
                 pubKey.c_str(), pubKey.size(),
                 enableConsolLog);

	// 子日志实例，这里创建一个新的日志实例，使用该日志打印的日志存放在以sublogger开头的日志文件中
  	const std::string subLogPerfix("sublogger");
  	void* subl = BuglyDiagnoseCreateSubLogger(logRootDir.c_str(), logRootDir.size(),
                                             subLogPerfix.c_str(), subLogPerfix.size(),
                                             defualtLogLevel, true);

    /// 测试日志打印
	BuglyLogFmtDebug(kLogTag, "jjxh, on xlog inited:%p", wWinMain);

    using namespace std::chrono_literals;
	std::thread th1([]() {
        static int c = 0;
        while (true) {
            BuglyLogMsgInfo(kLogTag, "on th1 log ...");
            BuglyLogFmtFatal(kLogTag, "on th1 logfmt:%d", c++);
            std::this_thread::sleep_for(30ms);
        }
	});

	std::thread th2([]() {
        static int c = 0;
		while (true) {
			BuglyLogMsgInfo(kLogTag, "on th2 log 中文测试 ...");
            BuglyLogFmtFatal(kLogTag, "on th2 logfmt:%d", c++);
			std::this_thread::sleep_for(30ms);
		}
	});

	std::thread th3([subl]() {
		while (true) {
            BuglySubLogFmtError(subl, kLogTag, "on th3 sub log...");
            BuglySubLogFmtDebug(subl, kLogTag, "on th3 sub log fmt:%s", std::to_string((uintptr_t)subl).c_str());
			std::this_thread::sleep_for(30ms);
		}
	});

	th1.detach();
	th2.detach();
	th3.detach();
}

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ LPWSTR    lpCmdLine,
                      _In_ int       nCmdShow)
{
    std::setlocale(LC_ALL, "");

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。
    /// 设置主线程名字
    //::SetThreadDescription(::GetCurrentThread(), L"MyMainThreadName");

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BUGLYDEMO, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BUGLYDEMO));

    /// 初始化Bugly
    InitBugly();

    /// 初始化日志,如果需要bugly来输出日志
    InitLogger();
	//::SetTimer(hWnd, 10240, 10000, (TIMERPROC)NULL);

    /// 初始化诊断系统
    InitDiagnose();


    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BUGLYDEMO));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BUGLYDEMO);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 将实例句柄存储在全局变量中

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                              CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_CREATE:
            {
                ::SetTimer(hWnd, 10240, 10000, (TIMERPROC)NULL);
            }
            break;
        case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);
                // 分析菜单选择:
                switch (wmId)
                {
                    case IDM_CRASH:
                        {
                            //int a = 0x1234;
                            //int* p = (int*)a;
                            //*p = 2;
                          abort();
                        }
                        break;
                    case IDM_CUSTOMERROR:
                        CustomErrorTest();
                        break;
                    case IDM_UPLOADLOG:
                        HandleUploadLogs();
                        break;
                    case IDM_ABOUT:
                        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                        break;
                    case IDM_EXIT:
                        DestroyWindow(hWnd);
                        break;
					case IDM_LAG: {
                        std::this_thread::sleep_for(std::chrono::seconds(1));
					} break;
					case IDM_ANR: {
						std::this_thread::sleep_for(std::chrono::seconds(6));
					} break;
                    default:
                        return DefWindowProc(hWnd, message, wParam, lParam);
                }
            }
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hWnd, &ps);
                // TODO: 在此处添加使用 hdc 的任何绘图代码...
                EndPaint(hWnd, &ps);
            }
            break;
        case WM_TIMER:
            {
		        auto now = std::chrono::system_clock::now();
		        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
	    	    BuglyLogFmtInfo(kLogTag, "hodxiang's timer, on timer:%lld", duration.count());
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
    }
    return (INT_PTR)FALSE;
}
