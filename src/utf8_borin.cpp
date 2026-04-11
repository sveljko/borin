#include "borin/raspored.h"

#include <algorithm>

#include <cstdint>
#include <cstring>
#include <cassert>


borin_rlt utf8_borin(std::string_view s, std::span<char> d)
{
    if (s.empty()) {
        return { 0, d.size() };
    }
    assert((s.data() >= d.data()) ||  (s.data() < d.data() - s.size()));

    struct X { 
        X(char const*s, uint8_t const (&u)[2])
            : utf((u[0] << 8) + u[1]) {
            memcpy(b,s,2);
        }
        uint16_t utf; 
        uint8_t b[2];
        auto operator<=>(uint16_t x) const {
            return utf - x;
        }
    };
    const static X map[] = {
        // latinica
        {"Cy", {0xC4, 0x86}},
        {"cy", {0xC4, 0x87}},
        {"Cx", {0xC4, 0x8C}},
        {"cx", {0xC4, 0x8D}},
        {"Dy", {0xC4, 0x90}},
        {"dy", {0xC4, 0x91}},
        {"Sx", {0xC5, 0xA0}},
        {"sx", {0xC5, 0xA1}},
        {"Zx", {0xC5, 0xBD}},
        {"zx", {0xC5, 0xBE}},
        // specijalni latinicxni dijakriticxki znaci, retko se koriste
        {"Dx", {0xC7, 0x84}}, // Ǆ - Borin raspored nema DX
        {"Dx", {0xC7, 0x85}}, // ǅ        
        {"dx", {0xC7, 0x86}}, // ǆ        
        {"Ly", {0xC7, 0x87}}, // Ǉ - Borin raspored nema LY
        {"Ly", {0xC7, 0x88}}, // ǈ
        {"ly", {0xC7, 0x89}}, // ǉ
        {"Ny", {0xC7, 0x8A}}, // Ǌ - Borin raspored nema NY
        {"Ny", {0xC7, 0x8B}}, // ǋ
        {"ny", {0xC7, 0x8C}}, // ǌ
        // cyrilica
        {"Dy", {0xD0, 0x82}},
        {"J",  {0xd0, 0x88}},
        {"Ly", {0xD0, 0x89}},
        {"Ny", {0xD0, 0x8A}},
        {"Cy", {0xD0, 0x8B}},
        {"Dx", {0xD0, 0x8F}},
        {"A",  {0xd0, 0x90}}, 
        {"B",  {0xd0, 0x91}},
        {"V",  {0xd0, 0x92}},
        {"G",  {0xd0, 0x93}},
        {"D",  {0xd0, 0x94}},
        {"E",  {0xd0, 0x95}},
        {"Zx", {0xD0, 0x96}},
        {"Z",  {0xd0, 0x97}},
        {"I",  {0xd0, 0x98}},
        {"K",  {0xd0, 0x9A}},
        {"L",  {0xd0, 0x9B}},
        {"M",  {0xd0, 0x9C}},
        {"N",  {0xd0, 0x9D}},
        {"O",  {0xd0, 0x9E}},
        {"P",  {0xd0, 0x9F}},
        {"R",  {0xd0, 0xA0}},
        {"S",  {0xd0, 0xA1}},
        {"T",  {0xd0, 0xA2}},
        {"U",  {0xd0, 0xA3}},
        {"F",  {0xd0, 0xA4}},
        {"H",  {0xd0, 0xA5}},
        {"C",  {0xd0, 0xA6}},
        {"Cx", {0xD0, 0xA7}},
        {"Sx", {0xD0, 0xA8}},
        {"a",  {0xd0, 0xb0}},
        {"b",  {0xd0, 0xb1}},
        {"v",  {0xd0, 0xb2}},
        {"g",  {0xd0, 0xb3}},
        {"d",  {0xd0, 0xb4}},
        {"e",  {0xd0, 0xb5}},
        {"zx", {0xD0, 0xb6}},
        {"z",  {0xd0, 0xb7}},
        {"i",  {0xd0, 0xb8}},
        {"k",  {0xd0, 0xba}},
        {"l",  {0xd0, 0xbb}},
        {"m",  {0xd0, 0xbc}},
        {"n",  {0xd0, 0xbd}},
        {"o",  {0xd0, 0xbe}},
        {"p",  {0xd0, 0xbf}},
        {"r",  {0xd1, 0x80}},
        {"s",  {0xd1, 0x81}},
        {"t",  {0xd1, 0x82}},
        {"u",  {0xd1, 0x83}},
        {"f",  {0xd1, 0x84}},
        {"h",  {0xd1, 0x85}},
        {"c",  {0xd1, 0x86}},
        {"cx", {0xD1, 0x87}},
        {"sx", {0xD1, 0x88}},
        {"dy", {0xD1, 0x92}},
        {"j",  {0xd1, 0x98}},
        {"ly", {0xD1, 0x99}},
        {"ny", {0xD1, 0x9A}},
        {"cy", {0xD1, 0x9B}},
        {"dx", {0xD1, 0x9F}},
    };
    auto mapend = map + sizeof map / sizeof map[0];
    while (!s.empty() && !d.empty()) {
        if (s.front() & 0x80) {
            if ((s.size() == 1) || (d.size() < 2)) {
                break;
            }
            uint16_t c = (uint8_t(s[0]) << 8) + uint8_t(s[1]);
            auto y = std::lower_bound(map, mapend, c);
            if ((y != mapend) && (y->utf == c)) {
                if (y->b[1]) {
                    memcpy(d.data(), y->b, 2);
                    d = d.last(d.size() - 2);
                }
                else {
                    d.front() = y->b[0];
                    d = d.last(d.size() - 1);
                }
            }
            else {
                memcpy(d.data(), s.data(), 2);
                d = d.last(d.size() - 2);
            }
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


extern "C" struct borin_rlt utf8_borin(char const* s, size_t n, char* d, size_t m)
{
    return utf8_borin({s,n}, {d,m});
}
