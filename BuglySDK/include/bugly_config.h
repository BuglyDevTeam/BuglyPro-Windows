/**
**********************************************************************************************************************************************************************************************************************************
* @file:	bugly_config.h
* @author:	hodxiang
* @date:	2025年07月21日 15:33:04 星期一
* @brief:	bugly 初始化参数
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __BUGLY_CONFIG__H__
#define __BUGLY_CONFIG__H__


namespace Bugly {

// Bugly上报后台类型
#define BuglyServerHostType                 unsigned int

// 腾讯内部版本使用WOA
#define kBuglyServerHostTypeWOA             0

// 不属于腾讯的业务，使用BuglyPro
#define kBuglyServerHostTypeBuglyPro        1

// 海外业务使用Oversea
#define kBuglyServerHostTypeOversea         2

// 腾讯云
#define kBuglyServerHostTypeTencentCloud    3

// 腾讯云国际站
#define kBuglyServerHostTypeTencentCloudSgp 4

#define kBuglyServerHostTypeDBugly          5 


struct BuglyConfig {
    // BuglyConfig 结构体的大小
    int             size_{sizeof(BuglyConfig)};

    // 上报进程路径，绝对路径
    const wchar_t*        handler_{nullptr};

    // crash 数据路径，绝对路径
    const wchar_t*        database_{nullptr};

    // crash 数据路径，绝对路径
    const wchar_t*        metrics_dir_{nullptr};

    // app id, 请到bugly官网申请和查看
    const wchar_t*        app_id_{nullptr};

    // app key, 请到bugly官网申请和查看
    const wchar_t*        app_key_{nullptr};

    // 业务程序版本号
    const wchar_t*        app_version_{nullptr};

    // bundle id
    const wchar_t*        bundle_id_{nullptr};

    // 业务程序名称
    const wchar_t*        app_name_{nullptr};

    // 业务程序显示名称，如果crash了，会在bugly弹框上显示
    const wchar_t*        app_disp_name_{nullptr};

    // 业务程序构件号
    const wchar_t*        build_id_{nullptr};

    // 初始用户id，供dau上报使用，当账号更新后可以调用SetKey通知Bugly更新
    const wchar_t*        account_id_{nullptr};

    // 和crashpad_handler.exe连接的管道名称，如果为nullptr,会随机生成一个，
    // 随机生成的可以用GetHandlerIPCPipeName接口获取
    const wchar_t*        pipename_{nullptr};

    // 业务传入的deviceId, 如果业务不传，bugly会随机生成一个
    const wchar_t*        deviceId_{nullptr};

    // 最大等待时间，单位 ms
    unsigned int    timeout_{5000};

    // 是否复用已有的crashpad_handler.exe进程，如果为false，就直接拉起新的crashpad_hander.exe
    bool            is_reuse_bugly_server_{true};

    // 是否弹窗提示用户 crash 信息
    bool            is_pop_dialog_{true};

    // 是否发送 crash 进程窗口及系统磁盘附加信息
    bool            is_need_attach_info_{true};

    // 是否上传, 如果为 false, 则只记录保存在本地
    bool            is_need_upload_{true};

    // 是否监控Bugly自身的crash
    bool            is_monitor_self_{false};

    // 是否由系统服务进程调用
    bool            is_server_process_{false};

    // 是否在业务进程抓取crash栈，如果业务程序需要加固，就需要传yes
    // 普通业务不需要，一般来说游戏业务需要
    bool            is_in_dll_mode_{false};

    // bugly上报域名，默认为WOA，也就是腾讯内部产品使用的类型
    // 腾讯外部产品需要设置为 kBuglyServerHostTypeBuglyPro
    // 海外版本需要使用 kBuglyServerHostTypeOversea
    // 腾讯云用户使用 kBuglyServerHostTypeTencentCloud
    BuglyServerHostType server_host_type_{kBuglyServerHostTypeWOA};
};

}

#endif  //!__BUGLY_CONFIG__H__