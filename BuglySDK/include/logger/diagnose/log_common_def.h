/**
**********************************************************************************************************************************************************************************************************************************
* @file:	log_common_def.h
* @author:	hodxiang
* @date:	2024年11月11日 20:33:56 星期一
* @brief:	一些公共的数据定义
**********************************************************************************************************************************************************************************************************************************
**/

#ifndef __LOG_COMMON_DEF__H__
#define __LOG_COMMON_DEF__H__



namespace Bugly {
namespace Diagnose {

/// <summary>
/// 当前网络类型，用于log主动上报限频
/// </summary>
enum EBuglyLogNetType {
  kBuglyLogNetType_Unknown,
  kBuglyLogNetType_WiredNet,    /// 有线网络,pc
  kBuglyLogNetType_Wifi,        
  kBuglyLogNetType_XG,
};

enum ELogServerHostType {
  kLogServerHostTypeDefalut = 0,  /// woa正式环境
  kLogServerHostTypeTest,         /// woa测试环境
  kLogServerHostTypeOversea,      /// 海外正式环境
  kLogServerHostTypeBuglyPro,     /// 专业版环境
  kLogServerHostTypeTencentCloud, /// 腾讯云环境
  kLogServerHostTypeTencentCloudSgp, /// 腾讯云国际站
  kLogServerHostTypeDBugly,       ///
};

}
}

#endif  //!__LOG_COMMON_DEF__H__