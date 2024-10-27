#include "badgecpp/util.hpp"
#include <algorithm>
namespace badge {
    std::string dtos(double num, int p) {
        // 处理负数
        bool negative = false;
        if (num < 0) {
            negative = true;
            num = -num;
        }

        // 分离整数部分
        long long integer_part = static_cast<long long>(num);

        // 分离小数部分
        double decimal_part_f = num - static_cast<double>(integer_part);

        // 将小数部分转换为整数，避免浮点精度问题
        long long decimal_part = 0;
        if (p > 0) {
            double multiplier = 1.0;
            for (int i = 0; i < p; ++i) multiplier *= 10.0;
            decimal_part = static_cast<long long>(decimal_part_f * multiplier);
        }

        // 去除小数部分的末尾0
        int actual_decimal_digits = p;
        while (actual_decimal_digits > 0 && (decimal_part % 10) == 0) {
            decimal_part /= 10;
            actual_decimal_digits--;
        }

        // 特殊情况处理：如果整数部分和小数部分都为0
        if (integer_part == 0 && decimal_part == 0) return "0";

        // 构建字符串
        std::string result;
        if (negative) result.push_back('-');

        char buffer[32];

        // 处理整数部分
        if (integer_part == 0) {
            if (actual_decimal_digits == 0) result.push_back('0');
        } else {
            int int_len = 0;
            while (integer_part > 0) {
                buffer[int_len++] = '0' + (integer_part % 10);
                integer_part /= 10;
            }
            std::reverse(buffer, buffer + int_len);
            result.append(buffer, int_len);
        }

        // 处理小数部分
        if (actual_decimal_digits > 0) {
            result.push_back('.');
            int dec_len = 0;
            // 从最低位到最高位处理小数部分的每一位
            for (int i = 0; i < actual_decimal_digits; ++i) {
                buffer[dec_len++] = '0' + (decimal_part % 10);
                decimal_part /= 10;
            }
            // 反转小数部分
            std::reverse(buffer, buffer + dec_len);
            result.append(buffer, dec_len);
        }

        return result;
    }
}// namespace badge