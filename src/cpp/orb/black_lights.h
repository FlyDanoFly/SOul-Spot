#ifndef _BLACK_LIGHT_H_
#define _BLACK_LIGHT_H_


enum BLACK_LIGHT {
    BLACK_LIGHT_UNDEFINED = -1,
    BLACK_LIGHT_00 = 2,
    BLACK_LIGHT_01 = 3,
    BLACK_LIGHT_02 = 4,
    BLACK_LIGHT_03 = 5,
    BLACK_LIGHT_04 = 6,
    BLACK_LIGHT_05 = 7,
    BLACK_LIGHT_06 = 8,
    BLACK_LIGHT_07 = 9,
    BLACK_LIGHT_08 = 10,
    BLACK_LIGHT_09 = 11,
    BLACK_LIGHT_10 = 12,
    BLACK_LIGHT_11 = 13,
    BLACK_LIGHT_12 = 42,
    BLACK_LIGHT_13 = 43,
    BLACK_LIGHT_14 = 44,
    BLACK_LIGHT_COUNT = 15
};

inline void set_blacklight(BLACK_LIGHT black_light, int intensity) {
    analogWrite(black_light, intensity);
}

inline void set_all_blacklights(int intensity) {
    set_blacklight(BLACK_LIGHT_00, intensity);
    set_blacklight(BLACK_LIGHT_01, intensity);
    set_blacklight(BLACK_LIGHT_02, intensity);
    set_blacklight(BLACK_LIGHT_03, intensity);
    set_blacklight(BLACK_LIGHT_04, intensity);
    set_blacklight(BLACK_LIGHT_05, intensity);
    set_blacklight(BLACK_LIGHT_06, intensity);
    set_blacklight(BLACK_LIGHT_07, intensity);
    set_blacklight(BLACK_LIGHT_08, intensity);
    set_blacklight(BLACK_LIGHT_09, intensity);
    set_blacklight(BLACK_LIGHT_10, intensity);
    set_blacklight(BLACK_LIGHT_11, intensity);
    set_blacklight(BLACK_LIGHT_12, intensity);
    set_blacklight(BLACK_LIGHT_13, intensity);
    set_blacklight(BLACK_LIGHT_14, intensity);
}


#endif // _BLACK_LIGHT_H_
