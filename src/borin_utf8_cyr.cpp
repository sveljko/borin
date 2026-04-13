#include "borin/raspored.h"

#include <algorithm>
#include <memory>

#include <cstdint>
#include <cstring>
#include <cassert>


constexpr uint8_t velika[26][2] = {
    {0xd0, 0x90}, // A
    {0xd0, 0x91}, // B
    {0xd0, 0xa6}, // C
    {0xd0, 0x94}, // D
    {0xd0, 0x95}, // E
    {0xd0, 0xa4}, // F
    {0xd0, 0x93}, // G
    {0xd0, 0xa5}, // H
    {0xd0, 0x98}, // I
    {0xd0, 0x88}, // J
    {0xd0, 0x9A}, // K
    {0xd0, 0x9B}, // L
    {0xd0, 0x9C}, // M
    {0xd0, 0x9D}, // N
    {0xd0, 0x9E}, // O
    {0xd0, 0x9F}, // P
    {'Q', 0},
    {0xd0, 0xA0}, // R
    {0xd0, 0xA1}, // S
    {0xd0, 0xA2}, // T
    {0xd0, 0xA3}, // U
    {0xd0, 0x92}, // V
    {'W', 0},
    {'X', 0},
    {'Y', 0},
    {0xd0, 0x97}, // Z
};
constexpr uint8_t mala[26][2] = {
    {0xd0, 0xb0}, // a
    {0xd0, 0xb1}, // b
    {0xd1, 0x86}, // c
    {0xd0, 0xb4}, // d
    {0xd0, 0xb5}, // e
    {0xd1, 0x84}, // f
    {0xd0, 0xb3}, // g
    {0xd1, 0x85}, // h
    {0xd0, 0xb8}, // i
    {0xd1, 0x98}, // j
    {0xd0, 0xba}, // k
    {0xd0, 0xbb}, // l
    {0xd0, 0xbc}, // m
    {0xd0, 0xbd}, // n
    {0xd0, 0xbe}, // o
    {0xd0, 0xbf}, // p
    {'q', 0},
    {0xd1, 0x80}, // r
    {0xd1, 0x81}, // s
    {0xd1, 0x82}, // t
    {0xd1, 0x83}, // u
    {0xd0, 0xb2}, // v
    {'w', 0},
    {'x', 0},
    {'y', 0},
    {0xd0, 0xb7}, // z
};


borin_rlt borin_utf8_cyr(std::string_view s, std::span<char> d)
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
        {"Cx", {0xD0, 0xA7}},
        {"Cy", {0xD0, 0x8B}},
        {"Dx", {0xD0, 0x8F}},
        {"Dy", {0xD0, 0x82}},
        {"Ly", {0xD0, 0x89}},
        {"Ny", {0xD0, 0x8A}},
        {"Sx", {0xD0, 0xA8}},
        {"Zx", {0xD0, 0x96}},
        {"cx", {0xD1, 0x87}},
        {"cy", {0xD1, 0x9B}},
        {"dx", {0xD1, 0x9F}},
        {"dy", {0xD1, 0x92}},
        {"ly", {0xD1, 0x99}},
        {"ny", {0xD1, 0x9A}},
        {"sx", {0xD1, 0x88}},
        {"zx", {0xD0, 0xB6}}
    };
    auto mapend = map + sizeof map / sizeof map[0];

    auto jednoslov = [&d](uint8_t const (&b)[2]) {
        if (auto znak = b[0]; znak & 0x80) {
            if (d.size() < 2) {
                return 0;
            }
            memcpy(d.data(), b, 2);
            return 2;
        }
        else {
            d.front() = znak;
            return 1;
        }
    };
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
            int r = 1;
            if (auto h = s.front(); (h >= 'A') && (h <= 'Z')) {
                r = jednoslov(velika[h-'A']);
            }
            else if ((h >= 'a') && (h <= 'z')) {
                r = jednoslov(mala[h-'a']);
            }
            else {
                d.front() = s.front();
            }
            if (0 == r) {
                break;
            }
            d = d.last(d.size() - r);
            s.remove_prefix(1);
        }
    }
    return { s.size(), d.size() };
}


std::string borin_utf8_cyr(std::string_view s)
{
    if (s.empty()) {
        return {};
    }
    auto v = s.size() * 2;
    auto rzlt = std::make_unique<char[]>(v);
    auto o = borin_utf8_cyr(s, {rzlt.get(), v});
    return {rzlt.get(), v - o.d - 1};
}


extern "C" struct borin_rlt borin_utf8_cyr(char const* s, size_t n, char* d, size_t m)
{
    return borin_utf8_cyr({s,n}, {d,m});
}
