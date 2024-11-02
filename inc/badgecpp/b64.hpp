#ifndef BADGECPP_B64_H
#define BADGECPP_B64_H

#include <cstdint>
#include <string>
#include <vector>

namespace badge::b64 {

    /// @brief 将数据编码为b64
    /// @param in 数据输入流
    /// @param out b64输出流
    void b64encode(std::istream &in, std::ostream &out);

    /// @brief 将数据编码为b64
    /// @param data 数据头指针
    /// @param dlen 数据长度
    /// @return b64字符串
    std::string b64encode(const uint8_t *data, uint32_t dlen);

    /// @brief 将b64解码为数据
    /// @param data b64字符串
    /// @return 数据
    std::vector<uint8_t> b64decode(const std::string &data);

    /// @brief 判断是否是b64字符串
    bool isbase64(const std::string &data);

}// namespace badge::b64
#endif
