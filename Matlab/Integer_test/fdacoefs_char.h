/*
 * Filter Coefficients (C Source) generated by the Filter Design and Analysis Tool
 * Generated by MATLAB(R) 9.3 and Signal Processing Toolbox 7.5.
 * Generated on: 24-May-2019 21:30:15
 */

/*
 * Discrete-Time FIR Filter (real)
 * -------------------------------
 * Filter Structure  : Direct-Form FIR
 * Filter Length     : 101
 * Stable            : Yes
 * Linear Phase      : Yes (Type 1)
 */

/* General type conversion for MATLAB generated C-code  */
#include "tmwtypes.h"
/* 
 * Expected path to tmwtypes.h 
 * C:\Program Files\MATLAB\R2017b\extern\include\tmwtypes.h 
 */
/*
 * Warning - Filter coefficients were truncated to fit specified data type.  
 *   The resulting response may not match generated theoretical response.
 *   Use the Filter Design & Analysis Tool to design accurate
 *   int8 filter coefficients.
 */
const int h_len = 101;
const int8_T h[101] = {
      0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    1,    0,
     -1,   -1,    0,    1,    0,   -1,   -1,    1,    2,    1,   -2,   -2,
      1,    3,    1,   -3,   -3,    1,    4,    1,   -3,   -4,    1,    5,
      2,   -4,   -4,    2,    5,    2,   -5,   -5,    2,    6,    2,   -5,
     -5,    2,    6,    2,   -5,   -5,    2,    6,    2,   -5,   -5,    2,
      5,    2,   -4,   -4,    2,    5,    1,   -4,   -3,    1,    4,    1,
     -3,   -3,    1,    3,    1,   -2,   -2,    1,    2,    1,   -1,   -1,
      0,    1,    0,   -1,   -1,    0,    1,    0,    0,    0,    0,    0,
      0,    0,    0,    0,    0
};
