/* Public domain implementation of Bora's encoding by Srdjan Veljkovic
   SPDX-License-Identifier: Unlicense
or Copyright (C) 2026 Srdjan Veljkovic
   SPDX-License-Identifier: MIT
*/
#if !defined(INC_BORIN_RASPORED)
#define INC_BORIN_RASPORED

#include <stddef.h> /* size_t */


/** Rezultat koji vracyaju funkcije za rad sa Borinim rasporedom */
struct borin_rlt {
    /** Koliko je bajtova ostalo neiskorisxcyeno u izvornoj niski */
    size_t s;
    /** Koliko je bajtova ostalo nepopunyeno u prostoru za odredisxnu nisku */
    size_t d;
};

#if defined __cplusplus

#include <string_view>

#if __cplusplus >= 202002L

#include <span>

/** Prevodi sadrzxaj u Borinom (kodnom) rasporedu u UTF-8 cyirilicu.

    Prevodi onoliko znakova koliko mozxe da stane u @p d _u
    celini_. Dakle, ako je u @p d ostalo josx samo jedan
    bajt, onda necye biti mogucy upis cyrilicxnog slova.
    
    Pri tome, ne postoji nacxin da se prepozna da li je
    poslednyi znak u stvari "isecxen" Borin zapis znaka u dva
    ASCII slova. Tako da, ako je neko hteo da napisxe `kosx`
    ali je napisao `kos`, znak `s` cye biti preveden kao takav.
    
    Nema posebnu obradu za NUL znak ('\0'), jednako cye, kao i bilo
    koji znak koji nije ASCII slovo biti prosto prepisan.

    Ne proverava da li je sadrzxaj ASCII, pa vecy cye i svi znaci
    kojima su biti visxi od sedmog (u praksi, to je samo osmi bit, jer
    su prevodioci u kojima je `char` velicxine drukcxije od 8 bita
    gotovo izumrli) biti prepisani. To dalye znacxi da je mogucye ovoj
    funkciji proslediti UTF-8 kodiran @p s, a da cye samo ASCII znaci
    biti prevedeni u Cyriliicu, a drugi necye. Ovo mozxe da bude
    cxudno ako se, recimo, tako prosledyuju latinicxni tekstovi koji
    sadrzxe i ne ASCII znake, ali mozxe da bude korisno ako se koriste
    nelatinicxni znaci (Grcxki, drugi jezici ili specijalni znaci).
    
    @pre Parametri @p s i @p d ne smeju da se preklapaju
    
    @param s Pogled na nisku koja sadrzxaj sadrzxaj za prevod
    @param d Opseg u koji treba upisati prevedeni sadrzxaj.
    @return Par u kome redom pisxe koliko je ostalo bajtova
    neobradyenih iz @p s i neiskorisxcyenih u @p d.
 */
borin_rlt borin_utf8_cyr(std::string_view s, std::span<char> d);

/** Prevodi sadrzxaj u Borinom (kodnom) rasporedu u UTF-8 latinicu.

    Prevodi onoliko znakova koliko mozxe da stane u @p d _u
    celini_. Dakle, ako je u @p d ostalo josx samo jedan
    bajt, onda necye biti mogucy upis cyrilicxnog slova.
    
    Pri tome, ne postoji nacxin da se prepozna da li je
    poslednyi znak u stvari "isecxen" Borin zapis znaka u dva
    ASCII slova. Tako da, ako je neko hteo da napisxe `kosx`
    ali je napisao `kos`, znak `s` cye biti preveden kao takav.
    
    Nema posebnu obradu za NUL znak ('\0'), jednako cye, kao i bilo
    koji znak koji nije ASCII slovo biti prosto prepisan.

    Ne proverava da li je sadrzxaj ASCII, pa vecy cye i svi znaci
    kojima su biti visxi od sedmog (u praksi, to je samo osmi bit, jer
    su prevodioci u kojima je `char` velicxine drukcxije od 8 bita
    gotovo izumrli) biti prepisani.

    Ovaj prevod nije jednoznacxan i ova funkcija bira standardni zapis,
    sem za Dx:
    - Dy se prevodi u jedan znak "D sa crtom". Opcija bi bila u "Dj".
    - Dx se prevodi u jedan znak "D zajedno sa zx", zato sxto bi
      inacxe to bilo jedino slovo koje se kodira sa tri bajta. Opcije
      bi bile da se prevodi u dva znaka "D pa zx", kao i da se prevodi
      u jedan znak "D zajedno sa Zx".
    - Ly i Ny se prevode u standardna dva znaka. Opcija bi bila da se
      prevode u jedan "L/N zajedno sa j", kao i da veliko slovo bude
      "L/N zajedno sa J".

    Ove opcije nisu pravlyene, ali nije ih tesxko dodati, ako zatreba.
    
    @pre Parametri @p s i @p d ne smeju da se preklapaju
    
    @param s Pogled na nisku koja sadrzxaj sadrzxaj za prevod
    @param d Opseg u koji treba upisati prevedeni sadrzxaj.
    @return Par u kome redom pisxe koliko je ostalo bajtova
    neobradyenih iz @p s i neiskorisxcyenih u @p d.
 */
borin_rlt borin_utf8_lat(std::string_view s, std::span<char> d);

/** Prevodi sadrzxaj iz UTF-8 u Borin raspored. Podrzxava i cyirilicu
    i latinicu. Ne prevodi `Lj` u `Ly` i slicxno. Dakle, nikoji
    standardni ili uobicxjenih zapisi koji koriste ASCII slova koja
    postoje i u Cyrilici se ne prevode, cxak i kada ne znamo za
    slucxaj gde bi to bilo pogresxno.

    Prevodi onoliko znakova koliko mozxe da stane u @p d _u
    celini_. Ako je poslednyi UTF-8 znak nezavrsxen, bicye odbacxen.
    Nema posebnu obradu za NUL znak ('\0'), jednako cye, kao i bilo
    koji znak koji nije ASCII slovo biti prosto prepisan.

    @pre Parametri @p s i @p d smeju da se preklapaju samo ako je
    @p s na visxoj (ili istoj) memorijskoj adresi od @p d
    
    @param s Pogled na nisku koja sadrzxaj sadrzxaj za prevod
    @param d Opseg u koji treba upisati prevedeni sadrzxaj.
    @return Par u kome redom pisxe koliko je ostalo bajtova
    neobradyenih iz @p s i neiskorisxcyenih u @p d.
 */
borin_rlt utf8_borin(std::string_view s, std::span<char> d);

#endif // __cplusplus >= 202002L

/** Varijanta koja vracya sadrzxaj iz @p s u Borinom rasproedu
    preveden u UTF-8 cyirilicu. Mozxe da se koristi u C++17. */
std::string borin_utf8_cyr(std::string_view s);

/** Varijanta koja vracya sadrzxaj iz @p s u Borinom rasproedu
    preveden u UTF-8 latinicu. Mozxe da se koristi u C++17. */
std::string borin_utf8_lat(std::string_view s);

/** Varijanta koja vracya UTF-8 sadrzxaj iz @p s preveden u
    Borin raspored. Mozxe da se koristi u C++17. */
std::string utf8_borin(std::string_view s);

extern "C"
#endif /* __cplusplus */
/** Ovo je samo C sprega za odgovarajyucyu C++20 funkciju.

    @param s Pocxetak niske za prevod
    @param n Duzxina niske za prevod
    @param d Pocxetak niske za preveden sadrzxaj
    @param m Duzxina prostora za nisku za upis prevedenog sadrzxaja
    @return Duzxina neiskorisxcyenog u @p s odnosno @p d
 */
struct borin_rlt borin_utf8_cyr(char const* s, size_t n, char* d, size_t m);

#if defined __cplusplus
extern "C"
#endif
/** Ovo je samo C sprega za odgovarajyucyu C++20 funkciju.

    @param s Pocxetak niske za prevod
    @param n Duzxina niske za prevod
    @param d Pocxetak niske za preveden sadrzxaj
    @param m Duzxina prostora za nisku za upis prevedenog sadrzxaja
    @return Duzxina neiskorisxcyenog u @p s odnosno @p d
 */
struct borin_rlt borin_utf8_lat(char const* s, size_t n, char* d, size_t m);


#if defined __cplusplus
extern "C"
#endif
/** Ovo je samo C sprega za odgovarajyucyu C++20 funkciju.

    @param s Pocxetak niske za prevod
    @param n Duzxina niske za prevod
    @param d Pocxetak niske za preveden sadrzxaj
    @param m Duzxina prostora za nisku za upis prevedenog sadrzxaja
    @return Duzxina neiskorisxcyenog u @p s odnosno @p d
 */
struct borin_rlt utf8_borin(char const* s, size_t n, char* d, size_t m);


#endif /* !defined(INC_BORIN_RASPORED) */
