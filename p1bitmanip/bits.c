/* 
 * CS:APP Data Lab 
 * 
 * <Jose Reyes, reyesj5>
 * 
 * bits.c - Source file with your solutions to the project.
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

You will provide your solution to the project by
editing the collection of functions in this source file.

CODING RULES:
 
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
  3. Has unpredictable behavior when shifting an integer by more
     than the word size.

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

NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operators (! ~ & ^ | + << >>)
     that you are allowed to use for your implementation of the function. 
     The max operator count is checked by dlc. Note that '=' is not 
     counted; you may use as many of these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
4 The maximum number of ops for each function is given in the
     header comment for each function.

/*
 * STEP 2: Modify the following functions according to the coding rules.
 * 
 */


#endif
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
  /* masks the sign of x and applies equation -x = ~x + 1 to negatives */ 
  int mask1 = x>>31;
  int var1 = x^mask1;
  return var1+((~mask1)+1);
}
/* 
 * addOK - Determine if can compute x+y without overflow
 *   Example: addOK(0x80000000,0x80000000) = 0,
 *            addOK(0x80000000,0x70000000) = 1, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int addOK(int x, int y) {
  /* masks the sign of each value through shifts and compares it to the sum */
  int mask1 = x>>31;
  int mask2 = y>>31;
  int sign = mask1^mask2;
  int sum = x+y;
  int mask3 = sum>>31;
  return (!!sign)|(!(mask1^mask3));
}
/* 
 * allEvenBits - return 1 if all even-numbered bits in word set to 1
 *   Examples allEvenBits(0xFFFFFFFE) = 0, allEvenBits(0x55555555) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allEvenBits(int x) {
  /* through a mask checks all even bits against 1's, and then with an xor 
   * makes sure that all even bits were 1's */ 
  int mask1 = (85<<8)|85;
  int mask2 = (mask1<<16)|mask1;
  int var1 = x&mask2;
  return !(var1^mask2);
}
/*
 * bang - Compute !x without using !
 *   Examples: bang(3) = 0, bang(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int bang(int x) {
  /* by masking x with -x, it checks the sign to check that the value !=0 */
  int var1 = ~x+1;
  int mask1 = var1|x;
  int sign1 = mask1>>31;
  return sign1+1;
}
/* 
 * bitAnd - x&y using only ~ and | 
 *   Example: bitAnd(6, 5) = 4
 *   Legal ops: ~ |
 *   Max ops: 8
 *   Rating: 1
 */
int bitAnd(int x, int y) {
  /* or's the opposite of both values and returns the opposite of the result */
  int var1 = ~x;
  int var2 = ~y;
  int var3 = var1|var2;
  return ~var3;
}
/*
 * bitMask - Generate a mask consisting of all 1's 
 *   lowbit and highbit
 *   Examples: bitMask(5,3) = 0x38
 *   Assume 0 <= lowbit <= 31, and 0 <= highbit <= 31
 *   If lowbit > highbit, then mask should be all 0's
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int bitMask(int highbit, int lowbit) {
  /* creates a masks by shifting 1's and then comparing the result with & */
  int var1 = ~0;
  int mask1 = var1<<highbit<<1;
  int var2 = ~mask1;
  int mask2 = var1<<lowbit;
  return var2&mask2;
}
/*
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  /* negates the values with ~ and then compares them with &, and vice versa */
  int var1 = x&y;
  int var2 = ~x;
  int var3 = ~y;
  int var4 = ~(var2&var3);
  int var5 = ~var1;
  return var4&var5;
}
/* 
 * byteSwap - swaps the nth byte and the mth byte
 *  Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *            byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *  You may assume that 0 <= n <= 3, 0 <= m <= 3
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 25
 *  Rating: 2
 */
int byteSwap(int x, int n, int m) {
  /* exploits shifting to multiply by 8, and then swaps by masking and using & */
  int nbits = n<<3;
  int mbits = m<<3;
  int mask1 = 255<<nbits;
  int mask2 = 255<<mbits;
  int mask3 = x&mask1;
  int mask4 = x&mask2;
  int var1 = (mask3>>nbits)&255;
  int var2 = (mask4>>mbits)&255;
  int mask5 = var1<<mbits;
  int mask6 = var2<<nbits;
  int mask7 = x&(~(mask1|mask2));
  return mask5|mask6|mask7;
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
  /* checks x  for t or f and negates it to check y and z */
  int var1 = !!x;
  int var2 = ~var1+1;
  int mask1 = var2&y;
  int mask2 = ~var2&z;
  return mask1|mask2;
}
/* 
 * copyLSB - set all bits of result to least significant bit of x
 *   Example: copyLSB(5) = 0xFFFFFFFF, copyLSB(6) = 0x00000000
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int copyLSB(int x) {
  /* masking the first bit with &1, and then negating the value */
  int var1 = x&1;
  int var2 = ~var1 + 1;
  return var2;
}
/*
 * ezThreeFourths - multiplies by 3/4 rounding toward 0,
 *   Should exactly duplicate effect of C expression (x*3/4),
 *   including overflow behavior.
 *   Examples: ezThreeFourths(11) = 8
 *             ezThreeFourths(-9) = -6
 *             ezThreeFourths(1073741824) = -268435456 (overflow)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 3
 */
int ezThreeFourths(int x) {
  /* adds x 3 times, and then divides by 4 */
  int var1 = x + x + x;
  int sign = var1>>31;
  int shift = var1>>2;
  int var2 = shift&(~sign);
  int var3 = (shift&sign)+(shift&1);
  return var2+var3;
}
/* 
 * fitsBits - return 1 if x can be represented as an 
 *  n-bit, two's complement integer.
 *   1 <= n <= 32
 *   Examples: fitsBits(5,3) = 0, fitsBits(-4,3) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 2
 */
int fitsBits(int x, int n) {
  /* exploits the fact that in two complements the MSB is either 0 or 1 for sign*/
  int nBits = n+(~0);
  int mask1 = x>>nBits;
  return !(~mask1) | !mask1;
}
/*
 * getByte - Extract byte n from word x
 *   Bytes numbered from 0 (LSB) to 3 (MSB)
 *   Examples: getByte(0x12345678,1) = 0x56
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 2
 */
int getByte(int x, int n) {
  /* shifts to position using powers of 2, and masks the byte to be extracted */
  int nbits = n<<3;
  int mask1 = 255<<nbits;
  int var1 = x&mask1;
  return (var1>>nbits)&255;
}
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
  /* subtracts 48 and 57 and checks the sign to see if result is within range */
  int sum1 = x+(~0x2F);
  int sum2 = x+~0x39;
  int var1 = 1<<31;
  int sign1 = (sum1&var1)>>31;
  int sign2 = (sum2&var1)>>31;
  return !!((!sign1)&sign2);
}
/* 
 * isEqual - return 1 if x == y, and 0 otherwise 
 *   Examples: isEqual(5,5) = 1, isEqual(4,5) = 0
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int isEqual(int x, int y) {
  /* negating x value and xor both values to compare if they are equal with ~ */
  int var1 = ~x;
  int var2 = var1^y;
  return !(~var2);
}
/*
 * isLess - if x < y  then return 1, else return 0 
 *   Example: isLess(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLess(int x, int y) {
  /* checks the signs of x and y, then checks the subtraction sign */
  int signX = x>>31;
  int signY = y>>31;
  int checkSigns1 = (signX&1)&(signY^1);
  int checkSigns2 = (signX^1)&(signY&1);
  int subtractY = x+(~y+1);
  int sumSign = subtractY>>31;
  return (checkSigns1|(sumSign&1))&(!checkSigns2);
}
/*
 * isNonNegative - return 1 if x >= 0, return 0 otherwise 
 *   Example: isNonNegative(-1) = 0.  isNonNegative(0) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 6
 *   Rating: 3
 */
int isNonNegative(int x) {
  /* checks the sign of the number */
  int mask1 = x>>31;
  int sign = mask1&1;
  return !sign;
}
/*
 * isPower2 - returns 1 if x is a power of 2, and 0 otherwise
 *   Examples: isPower2(5) = 0, isPower2(8) = 1, isPower2(0) = 0
 *   Note that no negative number is a power of 2.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 4
 */
int isPower2(int x) {
  /* checks that sign, that it's not 0, and exploits that 2^n - 1 gives 1 bits */
  int sign = x>>31;
  int mask = x+(~0);
  return !(mask&x)&(!sign)&(!!x);
}
/*
 * isTmin - returns 1 if x is the minimum, two's complement number,
 *     and 0 otherwise 
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmin(int x) {
  /* masks x with -x and adds 1 to get 0 if x is Tmin */
  int var1 = ~x+1;
  int var2 = ~x^var1;
  int var3 = var2+1+!x;
  return !(var3);
}
/*
 * minusOne - return a value of -1 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 2
 *   Rating: 1
 */
int minusOne(void) {
  /* uses equation -x = ~x + 1 */
  return (~1)+1;
}
/*
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
  /* masks the bits to bi shifted and convines the results */
  int lShift = x<<n;
  int rShift = 32+(~n+1);
  int mask1 = x>>rShift;
  int mask2 = ~((1<<31)>>(rShift+(~0)));
  int mask3 = mask1&mask2;
  return lShift|mask3;
}
/*
 * satMul2 - multiplies by 2, saturating to Tmin or Tmax if overflow
 *   Examples: satMul2(0x30000000) = 0x60000000
 *             satMul2(0x40000000) = 0x7FFFFFFF (saturate to TMax)
 *             satMul2(0x60000000) = 0x80000000 (saturate to TMin)
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int satMul2(int x) {
  /* checks the signs of the sum and x to check overflow and then uses 
   * this to mask Tmin or Tmax */
  int sum = x + x;
  int tMin = 1<<31;
  int signx = x>>31;
  int signSum = sum>>31;
  int maskSum = ~(signx^signSum);
  int signSat = ~maskSum&(signx^(~tMin));
  return signSat|(sum&maskSum);
}
/*
 * subOK - Determine if can compute x-y without overflow
 *   Example: subOK(0x80000000,0x80000000) = 1,
 *            subOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subOK(int x, int y) {
  /* checks if signs are equal, else checks if sum has same sign as x */
  int sign1 = x>>31;
  int sign2 = y>>31;
  int sign3 = sign1^sign2;
  int sum = x+(~y+1);
  int mask3 = sum>>31;
  return !sign3|!(sign1^mask3);
}

