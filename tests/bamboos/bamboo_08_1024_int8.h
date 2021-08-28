#ifndef BAMBOO_08_1024_H_
#define BAMBOO_08_1024_H_

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "mozzi_pgmspace.h"

#define BAMBOO_08_1024_NUM_CELLS 2048
#define BAMBOO_08_1024_SAMPLERATE 16384

CONSTTABLE_STORAGE(int8_t) BAMBOO_08_1024_DATA []  = {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
                0, 1, 0, 0, 0, -6, -33, -86, -100, -11, 111, 125, 49, -15, -60, -49, -7, -7, -1,
                9, -27, -38, 21, 77, 80, 50, 0, -42, -46, -32, 3, -13, -91, -62, 44, 80, 36, -5,
                15, 47, 27, 5, -8, -60, -78, -34, -7, 12, 12, -7, 15, 51, 55, 43, 20, -24, -56,
                -42, 0, 3, -52, -58, 5, 49, 53, 44, 24, 4, 2, 8, -7, -42, -61, -44, -17, -2, 12,
                28, 42, 35, 15, 18, 31, 7, -41, -61, -41, -19, -15, -3, 15, 18, 28, 43, 41, 19,
                -3, -18, -29, -37, -34, -23, -14, -14, -9, 23, 56, 48, 11, -8, -1, 4, -12, -34,
                -46, -44, -19, 16, 29, 17, 16, 27, 32, 23, 5, -13, -31, -43, -36, -13, 2, 1, -1,
                13, 39, 45, 24, 1, -13, -19, -17, -19, -28, -35, -21, 10, 29, 24, 22, 24, 18, 5,
                -5, -12, -23, -34, -34, -17, 1, 10, 16, 21, 27, 24, 20, 12, -6, -26, -30, -21,
                -21, -18, -7, 6, 15, 26, 37, 35, 16, -7, -23, -26, -22, -23, -24, -17, -7, 9,
                31, 39, 26, 7, 0, -6, -18, -25, -21, -20, -20, -8, 14, 27, 23, 17, 15, 16, 10,
                -4, -22, -27, -23, -16, -6, 1, 2, 8, 22, 26, 17, 4, -2, -11, -22, -24, -17, -17,
                -14, -3, 12, 26, 26, 15, 8, 5, -1, -9, -18, -23, -22, -14, 0, 9, 10, 17, 26, 22,
                9, 0, -2, -13, -22, -24, -18, -10, 0, 7, 13, 18, 21, 17, 9, 1, -8, -17, -18,
                -18, -18, -12, 0, 8, 13, 22, 25, 18, 6, -4, -12, -16, -20, -21, -17, -7, 5, 14,
                18, 18, 14, 11, 6, -4, -15, -23, -21, -14, -12, -7, 4, 12, 15, 17, 18, 11, 0,
                -10, -15, -18, -17, -16, -11, -4, 5, 11, 17, 20, 15, 7, 0, -8, -16, -21, -17,
                -11, -8, 0, 10, 15, 18, 18, 12, 4, -3, -11, -15, -17, -15, -10, -7, 2, 11, 14,
                15, 15, 10, 4, -4, -10, -15, -17, -15, -9, -2, 4, 12, 15, 17, 13, 7, 0, -6, -13,
                -15, -16, -13, -8, 0, 8, 12, 13, 13, 11, 7, -1, -9, -13, -16, -15, -10, -4, 1,
                6, 12, 14, 13, 8, 4, -3, -12, -14, -14, -13, -9, -2, 4, 8, 10, 12, 11, 6, 1, -4,
                -10, -12, -12, -12, -7, -1, 3, 7, 11, 13, 11, 6, -1, -7, -11, -14, -14, -11, -6,
                2, 7, 11, 11, 11, 9, 4, -3, -8, -11, -12, -12, -9, -4, 3, 7, 10, 11, 10, 7, 3,
                -4, -8, -11, -13, -12, -7, -3, 3, 9, 13, 11, 10, 6, 0, -7, -11, -12, -12, -10,
                -5, -1, 4, 10, 14, 12, 7, 3, -1, -5, -10, -13, -13, -9, -3, 3, 7, 9, 11, 9, 8,
                6, -2, -9, -12, -11, -10, -7, -2, 2, 6, 10, 12, 10, 6, 0, -5, -7, -8, -11, -11,
                -6, -3, 3, 8, 10, 8, 5, 5, 2, -3, -9, -12, -12, -10, -6, 0, 5, 7, 7, 8, 9, 5, 0,
                -6, -9, -12, -12, -9, -5, -1, 3, 8, 10, 9, 8, 3, -2, -7, -9, -11, -11, -9, -4,
                1, 5, 9, 9, 8, 6, 2, -2, -6, -9, -13, -11, -6, -2, 2, 6, 8, 9, 9, 6, 2, -3, -6,
                -9, -10, -8, -4, -3, 1, 7, 9, 9, 8, 4, 0, -3, -5, -8, -10, -8, -5, -2, 3, 8, 9,
                8, 7, 4, 1, -3, -7, -9, -10, -7, -3, 0, 4, 7, 7, 7, 5, 3, -1, -5, -7, -10, -10,
                -7, -2, 2, 4, 6, 8, 7, 4, 1, -3, -7, -9, -9, -7, -5, -3, 2, 4, 6, 6, 5, 3, 1,
                -2, -6, -8, -9, -8, -4, 0, 3, 4, 6, 7, 5, 4, 0, -5, -8, -8, -7, -4, -3, 0, 3, 5,
                7, 7, 5, 2, -2, -4, -6, -7, -7, -5, -3, 1, 4, 5, 6, 6, 4, 1, -1, -4, -7, -8, -6,
                -4, -2, 1, 5, 6, 6, 5, 4, -1, -4, -6, -6, -6, -6, -4, -1, 2, 4, 6, 5, 4, 2, 0,
                -4, -6, -6, -6, -4, -3, -1, 3, 5, 5, 5, 3, 1, -1, -4, -6, -7, -6, -4, -1, 2, 3,
                4, 5, 5, 2, 1, -1, -4, -7, -6, -5, -2, -1, 1, 4, 5, 5, 5, 1, 0, -2, -4, -5, -6,
                -4, -2, -1, 1, 4, 6, 5, 4, 2, -2, -3, -4, -6, -6, -4, -2, 0, 3, 4, 5, 4, 3, 1,
                -2, -4, -6, -7, -5, -4, -1, 1, 3, 4, 6, 5, 3, 0, -3, -4, -5, -6, -5, -3, -2, 1,
                3, 5, 5, 4, 1, 0, -2, -4, -5, -5, -5, -2, 0, 3, 4, 4, 4, 4, 1, -1, -3, -5, -5,
                -5, -4, -1, 1, 2, 4, 4, 4, 2, 0, -2, -3, -5, -5, -4, -3, -2, 1, 2, 4, 4, 3, 2,
                -1, -2, -3, -4, -5, -5, -3, -1, 1, 3, 4, 4, 3, 2, 0, -2, -3, -5, -5, -4, -3, -1,
                1, 4, 3, 4, 3, 1, -1, -3, -3, -5, -4, -3, -2, 0, 1, 3, 3, 3, 2, 1, -2, -2, -4,
                -4, -4, -3, -2, 0, 2, 3, 3, 3, 2, 0, -2, -3, -4, -5, -4, -3, -1, 1, 2, 3, 4, 3,
                1, -1, -2, -3, -4, -4, -3, -2, 0, 2, 2, 3, 3, 2, 1, 0, -2, -4, -4, -4, -3, -2,
                1, 1, 2, 3, 3, 2, 1, 0, -3, -4, -4, -4, -3, -2, 1, 2, 3, 3, 3, 1, 0, -1, -3, -3,
                -4, -3, -2, -2, 1, 2, 3, 2, 2, 1, -1, -2, -3, -3, -3, -3, -2, 0, 1, 2, 3, 3, 2,
                1, -1, -2, -3, -4, -3, -3, -2, 0, 1, 2, 2, 2, 1, 0, -1, -2, -3, -3, -3, -3, -2,
                0, 2, 2, 2, 2, 1, 0, -1, -3, -4, -4, -3, -2, 0, 1};

#endif /* BAMBOO_08_1024_H_ */
