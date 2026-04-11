#include "borin/raspored.h"

#include <iostream>

#include <cstring>
#include <cassert>


int main() {
    char s[] = "Ly_ly,Ny-ny;Sx:sx Dy dy Cx cx Cy cy Zx zx Dx dx";
    std::cout << s << "\n";
    char d[sizeof s * 2];
    auto r = borin_utf8_cyr({s, sizeof s}, d);
    constexpr char ocxek[] = "Љ_љ,Њ-њ;Ш:ш Ђ ђ Ч ч Ћ ћ Ж ж Џ џ";
    assert(strcmp(d, ocxek) == 0);
    std::cout << "====\n'" << d << "'\n------\n'";
    assert(0 == r.s);
    assert(sizeof d - sizeof ocxek == r.d);
    std::cout << r.s << ',' << r.d << '\n';
    char db[sizeof s];
    r = utf8_borin(d, db);
    
    // Ovde se prosledi duzxina niske u 'd', dakle, bez NUL znaka
    assert(memcmp(s, db, sizeof s-1) == 0);
    assert(0 == r.s);
    assert(1 == r.d);
  
    char cx[] = "Cxoban tera ovcxice. Lyulya Lyusxke, na Moravi krusxke. Kuq. Cxucxecyi sxcxepan dxak.";
    char cxd[sizeof cx * 2];
    r = borin_utf8_cyr(cx, cxd);
    constexpr char cxocxek[] = "Чобан тера овчице. Љуља Љушке, на Морави крушке. Куq. Чучећи шчепан џак.";
    assert(0 == r.s);
    assert(sizeof cxd - sizeof cxocxek + 1 == r.d);
    assert(strcmp(cxd, cxocxek) == 0);
    char cxdb[sizeof cx];
    r = utf8_borin(cxd, cxdb);
    assert(memcmp(cx, cxdb, sizeof s) == 0);
    assert(0 == r.s);
    assert(1 == r.d);

    constexpr char klinizvor[] = "Prilagođene Ǌemu, ǋemu i Njemu, uključio odgovarajući Ǆak, ǅak i Džak, Ǉubavi, ǈubavi i Ljubavi";
    char klin[sizeof klinizvor];
    memcpy(klin, klinizvor, sizeof klinizvor);
    r = utf8_borin(klin, klin);
    constexpr char klinocxek[] = "Prilagodyene Nyemu, Nyemu i Njemu, ukljucxio odgovarajucyi Dxak, Dxak i Dzxak, Lyubavi, Lyubavi i Ljubavi";
    assert(memcmp(klin, klinocxek, sizeof klinocxek) == 0);
    assert(0 == r.s);
    assert(1 == r.d);

    char klin8[sizeof klinizvor];
    r = borin_utf8_lat(klin, klin8);
    constexpr char klin8ocxek[] = "Prilagođene Njemu, Njemu i Njemu, uključio odgovarajući ǅak, ǅak i Džak, Ljubavi, Ljubavi i Ljubavi";
    assert(memcmp(klin8, klin8ocxek, sizeof klin8ocxek) == 0);
    assert(0 == r.s);
    assert(1 == r.d);
    
    return 0;
}
