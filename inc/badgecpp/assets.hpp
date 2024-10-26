/// Copyright (c) 2024 MineYuanlu
/// Author: MineYuanlu
/// Github: https://github.com/MineYuanlu/badgecpp
/// Licence: MIT
#ifndef BADGECPP_ASSETS__HPP_GUARD
#define BADGECPP_ASSETS__HPP_GUARD
namespace badge {


    struct version final {
        [[nodiscard]] static int major();          ///< 主版本号, 未知时为0
        [[nodiscard]] static int minor();          ///< 次版本号, 未知时为0
        [[nodiscard]] static int patch();          ///< 修订号, 未知时为0
        [[nodiscard]] static const char *special();///< 特殊版本号, 未知时为nullptr
        [[nodiscard]] static const char *string(); ///< 版本号字符串, 未知时为nullptr
    };

}// namespace badge
#endif// BADGECPP_ASSETS__HPP_GUARD
