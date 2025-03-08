#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define TMin INT_MIN
#define TMax INT_MAX

#include "btest.h"
#include "bits.h"

test_rec test_set[] = {
//1




 {"bitXor", (funct_t) bitXor, (funct_t) test_bitXor, 2, "& ~", 14, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
//2
 
 {"isTmax", (funct_t) isTmax, (funct_t) test_isTmax, 1, "! ~ & ^ | +", 10, 1,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"allOddBits", (funct_t) allOddBits, (funct_t) test_allOddBits, 1,
    "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"addOK", (funct_t)addOK, (funct_t)test_addOK, 1, "! ~ & ^ | + << >>", 12, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"isNonZero", (funct_t)isNonZero, (funct_t)test_isNonZero, 1, "~ & ^ | + << >>", 10, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
  {"negate", (funct_t)negate, (funct_t)test_negate, 1,
    "! ~ & ^ | + << >>", 5, 2,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

//3
 {"isAsciiDigit", (funct_t) isAsciiDigit, (funct_t) test_isAsciiDigit, 1,
    "! ~ & ^ | + << >>", 15, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},

 {"isLessOrEqual", (funct_t) isLessOrEqual, (funct_t) test_isLessOrEqual, 2,
    "! ~ & ^ | + << >>", 24, 3,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
 {"rotateRight", (funct_t) rotateRight, (funct_t) test_rotateRight, 2,
    "~ & ^ | + << >> !", 10, 3,
  {{TMin, TMax},{0,31},{TMin,TMax}}},
//4

//float

 {"floatFloat2Int", (funct_t) floatFloat2Int, (funct_t) test_floatFloat2Int, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},
 {"bitReverse", (funct_t) bitReverse, (funct_t) test_bitReverse, 1, "! ~ & ^ | + << >>", 45, 4,
  {{TMin, TMax},{TMin,TMax},{TMin,TMax}}},
  {"floatScale2", (funct_t) floatScale2, (funct_t) test_floatScale2, 1,
    "$", 30, 4,
     {{1, 1},{1,1},{1,1}}},

};
