#include "borin/raspored.h"

#include <algorithm>

#include <cstdint>
#include <cstring>
#include <cassert>


borin_rlt borin_utf8_lat(std::string_view s, std::span<char> d)
{
    if (s.empty()) {
        return { 0, d.size() };
    }
    assert((s.data() - d.size() >= d.data()) || (d.data() - s.size() >= s.data()));

    struct X { 
        X(char const*s, uint8_t const (&u)[2])
            : cy(((uint8_t)s[0] << 8) + (uint8_t)s[1]) {
            memcpy(b,u,2);
        }
        uint16_t cy; 
        uint8_t b[2];
        auto operator<=>(uint16_t x) const {
            return cy - x;
        }
    };
    const static X map[16] = {
        {"Cx", {0xC4, 0x8C}},
        {"Cy", {0xC4, 0x86}},
        {"Dx", {0xC7, 0x85}}, // ǅ - standardno bi bilo tri bajta!
        {"Dy", {0xC4, 0x90}},
        {"Ly", {'L', 'j'}},
        {"Ny", {'N', 'j'}},
        {"Sx", {0xC5, 0xA0}},
        {"Zx", {0xC5, 0xBD}},
        {"cx", {0xC4, 0x8D}},
        {"cy", {0xC4, 0x87}},
        {"dx", {0xC7, 0x86}}, // ǆ - standardno bi bilo tri bajta!        
        {"dy", {0xC4, 0x91}},
        {"ly", {'l', 'j'}},
        {"ny", {'n', 'j'}},
        {"sx", {0xC5, 0xA1}},
        {"zx", {0xC5, 0xBE}},
    };
    auto mapend = map + sizeof map / sizeof map[0];

    while (!s.empty() && !d.empty()) {
        if (s.size() == 1) {
            d.front() = s.front();
            return { 0, d.size()-1 };
        }
        uint16_t c = (uint8_t(s[0]) << 8) + s[1];
        auto y = std::lower_bound(map, mapend, c);
        if ((y != mapend) && (y->cy == c)) {
            if (d.size() < 2) {
                return { s.size(), d.size() };
            }
            memcpy(d.data(), y->b, 2);
            d = d.last(d.size() - 2);
            s.remove_prefix(2);
        }
        else {
            d.front() = s.front();
            d = d.last(d.size() - 1);
            s.remove_prefix(1);
        }
    }
    return { s.size(), d.size() };
}


extern "C" struct borin_rlt borin_utf8_lat(char const* s, size_t n, char* d, size_t m)
{
    return borin_utf8_lat({s,n}, {d,m});
}
