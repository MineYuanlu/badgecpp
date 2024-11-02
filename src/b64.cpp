#include "badgecpp/b64.hpp"
#include <cstddef>
#include <cstdint>
#include <istream>
namespace {

    constexpr char alphabet_map[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    constexpr uint8_t reverse_map[] = {
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62, 255,
            255, 255, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255,
            255, 255, 255, 255, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
            10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
            25, 255, 255, 255, 255, 255, 255, 26, 27, 28, 29, 30, 31, 32, 33,
            34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
            49, 50, 51, 255, 255, 255, 255, 255};
}// namespace
namespace badge::b64 {

    void b64encode(std::istream &in, std::ostream &out) {
        constexpr size_t buffer_size = 3;
        uint8_t buffer[buffer_size];

        while (in) {
            in.read(reinterpret_cast<char *>(buffer), buffer_size);
            const auto bytes_read = in.gcount();

            if (bytes_read == 3) {
                out.put(alphabet_map[buffer[0] >> 2]);
                out.put(alphabet_map[((buffer[0] << 4) & 0x30) | (buffer[1] >> 4)]);
                out.put(alphabet_map[((buffer[1] << 2) & 0x3C) | (buffer[2] >> 6)]);
                out.put(alphabet_map[buffer[2] & 0x3F]);
            } else if (bytes_read == 1) {
                out.put(alphabet_map[buffer[0] >> 2]);
                out.put(alphabet_map[(buffer[0] << 4) & 0x30]);
                out.put('=');
                out.put('=');
            } else if (bytes_read == 2) {
                out.put(alphabet_map[buffer[0] >> 2]);
                out.put(alphabet_map[((buffer[0] << 4) & 0x30) | (buffer[1] >> 4)]);
                out.put(alphabet_map[(buffer[1] << 2) & 0x3C]);
                out.put('=');
            } else
                break;
        }
    }
    std::string b64encode(const uint8_t *data, uint32_t dlen) {
        uint32_t olen = dlen / 3 * 4;
        const uint32_t tail = dlen % 3;
        olen += tail > 0 ? 4 : 0;

        std::string enstr;
        enstr.resize(olen);
        auto *endata = enstr.data();
        for (uint32_t i = 0; i + 3 <= dlen; i += 3) {
            *endata++ = alphabet_map[data[i] >> 2];
            *endata++ = alphabet_map[((data[i] << 4) & 0x30) | (data[i + 1] >> 4)];
            *endata++ = alphabet_map[((data[i + 1] << 2) & 0x3c) | (data[i + 2] >> 6)];
            *endata++ = alphabet_map[data[i + 2] & 0x3f];
        }

        if (tail == 1) {
            *endata++ = alphabet_map[data[dlen - 1] >> 2];
            *endata++ = alphabet_map[(data[dlen - 1] << 4) & 0x30];
            *endata++ = '=';
            *endata = '=';
        } else if (tail == 2) {
            *endata++ = alphabet_map[data[dlen - 2] >> 2];
            *endata++ =
                    alphabet_map[((data[dlen - 2] << 4) & 0x30) | (data[dlen - 1] >> 4)];
            *endata++ = alphabet_map[(data[dlen - 1] << 2) & 0x3c];
            *endata = '=';
        }
        return enstr;
    }

    std::vector<uint8_t> b64decode(const std::string &data) {
        if (!isbase64(data)) return {};

        const auto edata = reinterpret_cast<const uint8_t *>(data.data());
        const uint32_t dlen = data.size();

        uint8_t quad[4];
        std::vector<uint8_t> odata(dlen / 4 * 3);
        uint8_t *ddata = odata.data();
        for (uint32_t i = 0; i + 3 < dlen; i += 4) {
            for (uint32_t j = 0; j < 4; j++) quad[j] = reverse_map[edata[i + j]];
            *ddata++ = (quad[0] << 2) | (quad[1] >> 4);
            if (quad[2] >= 64) break;
            if (quad[3] >= 64) {
                *ddata++ = (quad[1] << 4) | (quad[2] >> 2);
            } else {
                *ddata++ = (quad[1] << 4) | (quad[2] >> 2);
                *ddata++ = (quad[2] << 6) | quad[3];
            }
        }
        if (const size_t osize = ddata - odata.data(); odata.size() > osize)
            odata.erase(odata.begin() + osize, odata.end());
        return odata;
    }

    bool isbase64(const std::string &data) {
        const auto data_length = data.length();
        if (data_length % 4 > 0) return false;

        for (size_t i = 0; i < data_length; ++i) {
            if (data.at(i) == '=') {
                if (i != (data_length - 1) && data.at(i + 1) != '=') {
                    return false;
                }
            } else {
                if (const auto idx = static_cast<uint8_t>(data.at(i)); idx >= sizeof(reverse_map) || reverse_map[idx] == 255)
                    return false;
            }
        }
        return true;
    }

}// namespace badge::b64