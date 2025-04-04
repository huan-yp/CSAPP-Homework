/* 
 * CS:APP Data Lab 
 * 
 * <Please put your name and userid here>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
//1
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(x & y) & ~(~x & ~y);
}


/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
  return !(x ^ ~(x + 1)) & !(!(x ^ ~0)); // x = 0 is corner case
}

//2
/* 
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) {
  int k = 0xAA;
  k = k | (k << 8);
  k = k | (k << 16); // build mask
  return !((x & k) ^ k);
}

/*
 * addOK - determine if x+y overflows, return 0 if it overflows, otherwise return 1
 * Example: addOK(0x8000000, 0x8000000) = 0
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 20
 * Rating: 2
*/
int addOK(int x, int y) {
  int flag1 = ((x + y) >> 31) & 1;
  int flag2 = x >> 31 & 1;
  int flag3 = y >> 31 & 1;
  return (flag2 ^ flag3) | !(flag1 ^ flag2);
}

/*
 * isNonZero - return 0 if x is equal to 0, otherwise return 1
 * Example: isNonZero(3) = 1
 * Legal ops: ~ & ^ | + << >>
 * Max ops: 10
 * Rating: 2
*/
int isNonZero(int x) { 
  int px = x;
  int nx = ~x + 1;
  int sp = ~(px >> 31) & 1;
  int sn = ~(nx >> 31) & 1;
  return (sp ^ sn);
}

/* 
 * negate - return -x 
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) {
  return ~x + 1; // trival
}

//3
/* 
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
  int w = x & 0x3f;
  int f1 = !(w ^ x); // not cut
  int f2 = !((x >> 4) ^ 3);  // no low
  int b3 = (x >> 3) & 1; 
  // bit 3 exist
  int nb12 = !((x >> 1) & 3);
  // bit 1 2 not exist
  return f1 & f2 & (!b3 | nb12);
}

/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
  int f = (~x + 1) + y;
  // trival mathmatic tricks
  return !(f >> 31);
}

/*
 * rotateRight - rotate x right by n bits, 0 <= n <= 31
 * Example: rotateRight(0x87654321, 4) = 0x18765432
 * Legal ops: ~ & ^ | + << >> !
 * Max ops: 10
 * Rating: 3
*/
int rotateRight(int x, int n){
  int mask = (((x >> 31) << 31) >> n) << 1;
  int lb = (x >> n) ^ mask;
  int hb = x << (33 + (~n));
  return lb | hb;
}


//4

/*
 * bitCount - reverse the order of 32-bit bit sequence.
 * Example: bitReverse(0x80000002) = 0x40000001
 * Legal ops: ! ~ & ^ | + << >>
 * Max ops: 45
 * Rating: 4
*/
int bitReverse(int x) {
  int x0 = x;
  int mask0 = 0xff << 8 | 0xff;
  int x1 = (x0 >> 16) & mask0 | x0 << 16;
  int mask1 = 0xff << 16 | 0xff;
  int x2 = (x1 >> 8) & mask1 | (x1 & mask1) << 8;
  int midmask2 = 0xf << 8 | 0xf;
  int mask2 = midmask2 << 16 | midmask2;
  int x3 = (x2 >> 4) & mask2 | (x2 & mask2) << 4;
  int midmask3 = 0x33 << 8 | 0x33;
  int mask3 = midmask3 << 16 | midmask3;
  int x4 = (x3 >> 2) & mask3 | (x3 & mask3) << 2;
  int midmask4 = 0x55 << 8 | 0x55;
  int mask4 = midmask4 << 16 | midmask4;
  int x5 = (x4 >> 1) & mask4 | (x4 & mask4) << 1;
  return x5;
}


//float

/* 
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
  unsigned lead = uf & 0x7f800000u; // exp leading bits
  unsigned sign = uf & 0x80000000u;
  unsigned jumpbit = uf >> 22 & 1; // jump point bit
  unsigned jumpmask = 0x007fffffu; // precision mask
  unsigned p = uf & jumpmask;  // precision val
  if(lead == 0x7f800000u)return uf; // NaN
  if(lead == 0){
    return uf << 1 | sign; // abnormal
  }
  else{
    return uf + 0x800000u; // normal
  }
}

/* 
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
  int sign = uf >> 31;
  int exp = (uf >> 23 & 0xff) - 127; // exp num
  int val = uf & 0x7fffffu + 0x800000u; // precision val
  int fval = 0; // final return val;
  if(exp < 0)return 0; // absolute val less than 1
  if(exp > 30)return 0x80000000u; // too large
  if(exp > 23){
    fval = val << (exp - 23);
  }
  else{
    fval = val >> (23 - exp);
  }
  if(sign)return -fval;
  else return fval;
}


