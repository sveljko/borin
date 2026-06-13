#include "borin/raspored.h"

#include <iostream>

#include <cstring>


#define JESTE(u) if (u) { } else return Greska("Nije", #u, __LINE__)
#define NIJE(u) if (!(u)) { } else return Greska("Jeste", #u, __LINE__)
#define DAJE(l,d) if (auto r_ = (l); r_ == (d)) { } else return Greska(#l " != " #d, r_, __LINE__)


static unsigned m_palih;


static int Greska(std::string_view sv, char const *s, int red)
{
    std::cout << sv << " '" << s << "' red " << red << "\n";
    ++m_palih;
    return -1;
}

template<class T> static int Greska(std::string_view s, T const& v, int red)
{
    std::cout << s << " nego: '" << v << "' red " << red << "\n";
    ++m_palih;
    return -1;
}


static int cpp20() {
    char s[] = "Ly_ly,Ny-ny;Sx:sx Dy dy Cx cx Cy cy Zx zx Dx dx";
    std::cout << s << "\n";
    char d[sizeof s * 2];
    auto r = borin_utf8_cyr({s, sizeof s - 1}, d);
    constexpr char ocxek[] = "Љ_љ,Њ-њ;Ш:ш Ђ ђ Ч ч Ћ ћ Ж ж Џ џ";
    DAJE(r.s, 0);
    DAJE(r.d, sizeof d - sizeof ocxek + 1);
    d[sizeof d - r.d] = '\0';
    std::cout << "====\n'" << d << "'\n------\n";
    DAJE(strcmp(ocxek, d), 0);
    char db[sizeof s];
    r = utf8_borin(d, db);
    // Ovde se prosledi duzxina niske u 'd', dakle, bez NUL znaka
    DAJE(memcmp(s, db, sizeof s-1), 0);
    DAJE(r.s, 0);
    DAJE(r.d, 1);

    auto rstr = borin_utf8_cyr({s, sizeof s - 1});
    DAJE(rstr, ocxek);

    char cx[] = "Cxoban tera ovcxice. Lyulya Lyusxke, na Moravi krusxke. Kuq. Cxucxecyi sxcxepan dxak.";
    char cxd[sizeof cx * 2];
    r = borin_utf8_cyr({cx, sizeof cx}, cxd);
    constexpr char cxocxek[] = "Чобан тера овчице. Љуља Љушке, на Морави крушке. Куq. Чучећи шчепан џак.";
    DAJE(r.s, 0);
    DAJE(r.d, sizeof cxd - sizeof cxocxek);
    DAJE (strcmp(cxd, cxocxek), 0);
    char cxdb[sizeof cx];
    r = utf8_borin(cxd, cxdb);
    DAJE(memcmp(cx, cxdb, sizeof s - 1), 0);
    DAJE(r.s, 0);
    DAJE(r.d, 1);

    constexpr char klinizvor[] = "Prilagođene Ǌemu, ǋemu i Njemu, uključio odgovarajući Ǆak, ǅak i Džak, Ǉubavi, ǈubavi i Ljubavi";
    char klin[sizeof klinizvor];
    memcpy(klin, klinizvor, sizeof klinizvor);
    r = utf8_borin(klin, klin);
    constexpr char klinocxek[] = "Prilagodyene Nyemu, Nyemu i Njemu, ukljucxio odgovarajucyi Dxak, Dxak i Dzxak, Lyubavi, Lyubavi i Ljubavi";
    DAJE(memcmp(klin, klinocxek, sizeof klinocxek), 0);
    DAJE(r.s, 0);
    DAJE(r.d, 1);
    auto klinstr = utf8_borin({klinizvor, sizeof klinizvor - 1});
    DAJE(klinstr, klinocxek);

    char klin8[sizeof klinizvor];
    r = borin_utf8_lat({klin, sizeof klin - 1}, klin8);
    constexpr char klin8ocxek[] = "Prilagođene Njemu, Njemu i Njemu, uključio odgovarajući ǅak, ǅak i Džak, Ljubavi, Ljubavi i Ljubavi";
    DAJE(memcmp(klin8, klin8ocxek, sizeof klin8ocxek - 1), 0);
    DAJE(r.s, 0);
    DAJE(r.d, 1);

    auto klin8str = borin_utf8_lat({klinocxek, sizeof klinocxek - 1});
    DAJE(klin8str, klin8ocxek);

    return 0;
}


static int cpp17() {
    char s[] = "Ly_ly,Ny-ny;Sx:sx Dy dy Cx cx Cy cy Zx zx Dx dx";
    auto r = borin_utf8_cyr(s);
    constexpr char ocxek[] = "Љ_љ,Њ-њ;Ш:ш Ђ ђ Ч ч Ћ ћ Ж ж Џ џ";
    DAJE(r, ocxek);
    auto t = utf8_borin(r);
    DAJE(t, s);
    auto u = borin_utf8_lat(s);
    constexpr char ocxekla[] = "Lj_lj,Nj-nj;Š:š Đ đ Č č Ć ć Ž ž ǅ ǆ";
    DAJE(u, ocxekla);
    auto v = utf8_borin(r);
    DAJE(v, s);

    return 0;
}


#define TEST(f) std::cout << "[-----------]\n[ TERAM     ] " #f << "\n" << (f() ? "[ POZITIVAN ] " : "[ negativan ] ") << #f "\n[-----------]\n"

#define IZVESTI                                                         \
    if (m_palih) {                                                      \
        std::cout << "\n\n Ukupno palih testova: " << m_palih << "\n";  \
    }                                                                   \
    else {                                                              \
        std::cout << "\n\n Nalaz negativan.\n";                         \
    }                                                                   \
    return m_palih;

int main() {
    TEST(cpp20);
    TEST(cpp17);

    IZVESTI;
}
