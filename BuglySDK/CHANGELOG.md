## [1.0.25.6] - 2026-05-26
### Features
- 制品库签名

## [1.0.25.5] - 2026-05-15
### Features
- 增加单独的诊断和日志C接口
- 确保Windows7系统的兼容性

## [1.0.25.beta.3] - 2025-10-10
### Features
- 升级协议加密到V3。
- 合入VC6的支持
### Fix
- 修复在某些情况下Bugly进程会永久挂起已经crash的业务进程的bug
- 修复bugly data目录路径有空格导致上报无堆栈的bug

## [1.0.25.beta.2] - 2025-09-03
### Features
- 支持Windows arm64架构CPU。

### Fix
- 修复了一个卡顿上报成anr的bug。
- 修复了xlog中zstd_static.lib为多线程dll编译的问题

## [1.0.25.beta.1] - 2025-08-19
### Features
- 新增InitBuglyManager3接口和BuglyConfig类，将bugly初始化参数收敛到结构体中
- 添加设置上报域名的接口，统一woa和专业版sdk
- crash上报时附加命令行参数
- Bugly初始化和卡顿监控初始化解耦
- 新增windows相关信息上报
- bugly弹框界面App名称和crash进程一致
- crash附加日志大小阈值调整，压缩日志最大5MB，未压缩日志最大60MB
- 增加页面跟踪能力
- bugly初始化接口增加参数让业务可以传入devicid，本地创建deviceid优化
- 更新诊断系统版本，捞取日志文件名完善，防止批量捞取的日志文件重名的问题
- 完善卡顿监控返回值触发配置拉取逻辑
- 自定义错误上报增加内存和存储空间信息
- 卡顿监控上报协议支持压缩和加密
- crash dmp文件支持zip压缩

### Fix
- 修复诊断系统处理中文路径日志的bug
- 修复诊断系统可能会遗漏废弃zip文件的bug
- 修复crash上报被关闭以后还会弹bugly框的bug
- 修复欠费后配置更新不及时的bug
- 修复读写注册表接口的bug


## [1.0.24] - 2025-03-26
### Features
- 增加游戏模式，兼容ACE加固。
- crash上报增加内存容量信息和硬盘大小信息

## [1.0.23.3] - 2025-07-31
### Features
- 紧急适配VC6

## [1.0.23.2] - 2025-05-27
### Fix
- 捞取日志的日志zip包文件名修改，基本和移动端对齐
- 修复染色命令下发后生效有延迟的bug
- 诊断系统增加清理废弃zip文件的逻辑

## [1.0.23.1] - 2025-04-01
### Fix
- 修复多个crashpad_handler.exe进程可能会同时读写配置缓存的问题

## [1.0.23] - 2025-03-18
### Features
- 支持云控配置系统
- 支持fulldump
- 支持卡顿和ANR监控
- 支持分进程统计crash率

## [1.0.22.1] - 2025-03-03
### Fix
- 修复诊断系统偶先的捞取不了日志的bug
- 修复诊断系统内部的bug
- 建议所有业务都升级到这个版本

## [1.0.22] - 2025-01-10
### Features
- 完善Fastfail异常的捕获

## [1.0.21] - 2025-01-03
### Features
- 支持业务自己指定pipename和bugly服务进程连接，并提供相关接口支持
- 添加InitBuglyManager2接口，简化bugly初始化工作

## [1.0.20] - 2024-12-27
### Features
- 修复日志模块会错误创建一个临时文件的bug
- 完善诊断系统捞取日志的功能

## [1.0.19] - 2024-12-18
### Features
- 自定义错误上报支持app_channel字段
- InitBuglyManager初始化函数增加一个account_id的参数，用于dau上报，
  后续可以也可以通过SetKey("account_id", "account_id")来更新account_id

## [1.0.18] - 2024-12-17
### Features
- crash上报目前可以上报所有线程的调用栈了
- crash上报目前支持app_channel字段了
- crash上报目前支持启动次数和次数崩溃率了
### Fix
- 修复可能的漏报bug
- 修复bugly守护进程可能的crash
- 完善bugly成功率上报数据
- 修复一些遗留的bug

## [1.0.17] - 2024-11-16
### Features
- 修复某些情况下捞日志不够及时的bug

## [1.0.16] - 2024-11-15
### Features
- Bugly-Windows SDK支持专业版

## [1.0.15] - 2024-10-30
### Features
- 日志组件增加设置是否递归搜索日志目录的接口:setRecursiveSearchLogs

## [1.0.14] - 2024-10-28
### Features
- sdk 上报协议加密

## [1.0.13] - 2024-10-24
### Features
- 增加crash上报的成功率上报功能

## [1.0.12] - 2024-10-15
### Features
- 修复win32下crashpad_handler.exe进程加载bugly_logger.dll函数失效的bug
- 修复主动上报日志的tag白名单接口不生效的bug

## [1.0.11] - 2024-10-10
### Features
- 新增ReportDumpAndCrash接口，用于上报业务自己捕获的异常，并在采集结束后自动结束该崩溃进程

## [1.0.10] - 2024-09-26
### BugFix
- 修复初始化时遍历所有模块可能会死循环的bug。
- 修复dau上报可能不会及时上报的bug。

## [1.0.9] - 2024-09-14

### Features
- Bugly-Windows 删除third_party/boringssl和third_part/xnu两个没有使用的库，解决开源合规问题。

## [1.0.8] - 2024-09-12

### Features
- Bugly-Windows 捞取日志的 `附加文件路径` 增加特殊字符 `{diagnose_configs}` ，用于捞取诊断系统的相关配置文件。
- 更新diagnose submoule，收拢httpPost,httpPut和Zip这三个异步接口调用，将callback切换到workerthread，保证后续处理的线程安全。

## [1.0.7] - 2024-09-10

### Features
- Bugly-Windows支持日志捞取和主动上报



## [1.0.6] - 2024-08-12

### Features
- Bugly-Windows上报的"系统版本"字段支持windows 11



## [1.0.5] - 2024-08-06

### Features
- 搭建Bugly-Windows构建流水线和制品发布仓库
- 添加制品文件版本号更新机制
