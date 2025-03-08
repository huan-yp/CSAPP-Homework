/* Testing Code */

#include <limits.h>
#include <math.h>

/* Routines used by floation point test code */

/* Convert from bit level representation to floating point number */
float u2f(unsigned u) {
  union {
    unsigned u;
    float f;
  } a;
  a.u = u;
  return a.f;
}

/* Convert from floating point number to bit-level representation */
unsigned f2u(float f) {
  union {
    unsigned u;
    float f;
  } a;
  a.f = f;
  return a.u;
}

//1
int test_bitXor(int x, int y)
{
  return x^y;
}
int test_tmin(void) {
  return 0x80000000;
}


//2
int test_isTmax(int x) {
    return x == 0x7FFFFFFF;
}
int test_allOddBits(int x) {
  int i;
  for (i = 1; i < 32; i+=2)
      if ((x & (1<<i)) == 0)
   return 0;
  return 1;
}
int test_addOK(int x, int y)
{
  int sum = x + y;
  // Check for overflow
  if (x > 0 && y > 0 && sum < 0) {
      return 0; // Positive overflow
  } else if (x < 0 && y < 0 && sum >= 0) {
      return 0; // Negative overflow
  }
  return 1; // No overflow
}
int test_isNonZero(int x){
  if (x == 0) {
        return 0;
  } else {
        return 1;
  }
}

int test_negate(int x) {
  return -x;
}




//3


int test_isAsciiDigit(int x) {
  return (0x30 <= x) && (x <= 0x39);
}
int test_isLessOrEqual(int x, int y)
{
  return x <= y;
}
int test_rotateRight(int x, int n){
  return ((unsigned int)x >> n) | ((unsigned int)x << (32 - n));
}





//4
int test_bitReverse(int x){
    unsigned int u = (unsigned int)x;
    u = ((u >> 1) & 0x55555555) | ((u & 0x55555555) << 1);
    u = ((u >> 2) & 0x33333333) | ((u & 0x33333333) << 2);
    u = ((u >> 4) & 0x0f0f0f0f) | ((u & 0x0f0f0f0f) << 4);
    u = ((u >> 8) & 0x00ff00ff) | ((u & 0x00ff00ff) << 8);
    u = ((u >> 16) & 0x0000ffff) | ((u & 0x0000ffff) << 16);
    return u;
}

//float

unsigned test_floatScale2(unsigned uf) {
  float f = u2f(uf);
  float tf = 2*f;
  if (isnan(f))
    return uf;
  else
    return f2u(tf);
}

int test_floatFloat2Int(unsigned uf) {
  float f = u2f(uf);
  int x = (int) f;
  return x;
}

