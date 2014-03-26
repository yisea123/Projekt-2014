/*
 * University of Southern Denmark
 * Robot Systems Engineering (RB-EBS4-F14)
 * Embedded Programming (EMP)
 *
 * Project:			EMP
 * Author(s):		K.Andersen
 * Module:			binary.h
 * Description:	Contains macro's for bit manipulation and bit definements.
 *
 * ----------------------------------------------------------- *
 * Change log
 * Date YYMMDD		ID			Change
 * 050128			KA			Module Created
 *
 */


#ifndef _BINARY_H
#define _BINARY_H

/***************************** Include files *******************************/
#include "emp_type.h"

/*****************************    Defines    *******************************/

/*****************************************************************************
*   Macroes for setting, clearing and testing a bit in a variable
*   It is the users responsibility to use the macro inside the range
*   of the variable.
*
*   NOTE !!!
*   When using the test macro's be aware of that the return value is the
*   bit numbers value.
*   e.g. if bit 5 is set then the return value is 32.
*****************************************************************************/

#define SET_BIT(word, x) word |= (0x01 << x)
#define CLEAR_BIT(word, x) word &= ~(0x01 << x)


/* Definements for unsigned 8, 16 bit variables   */
#define SET_BIT_LOW(word,bit_number)    word &= ~(0x01 << bit_number)
#define SET_BIT_HIGH(word,bit_number)   word |=  (1<< bit_number)

#define TEST_BIT_HIGH(word,bit_number)  (word & (0x01 << bit_number))
#define TEST_BIT_LOW(word,bit_number)   (!(TEST_BIT_HIGH(word,bit_number)))

#define TOGGLE_BIT(word,bit_number)	word ^= (0x01 << bit_number)


/* Definements for unsigned 32 bit variable */
#define SET_BIT_LOW_UL(word,bit_number)      word &= ~((INT32U) 0x01 << bit_number)
#define SET_BIT_HIGH_UL(word,bit_number)     word |=  ((INT32U) 1<< bit_number)

#define TEST_BIT_HIGH_UL(word,bit_number)    (word & ((INT32U) 0x01 << bit_number))
#define TEST_BIT_LOW_UL(word,bit_number)     (!(TEST_BIT_HIGH(word,bit_number)))

#define TOGGLE_BIT_UL(word,bit_number)	      word ^= ((INT32U) 0x01 << bit_number)

/* Example
*
*  INT16U test;
*
*  SET_BIT_HIGH(test,5);            // Set bit 5 high.
*  if(TEST_BIT_HIGH(test,15))       // bit 15 high ?
*     SET_BIT_LOW(test,2);          // bit 15 high => Set bit 2 low
*
*  TOGGLE_BIT(test,13);             // Toggle bit number 13 in test.
*
*/

/* Bit definements */
#define b00000000     0
#define b00000001     1
#define b00000010     2
#define b00000011     3
#define b00000100     4
#define b00000101     5
#define b00000110     6
#define b00000111     7
#define b00001000     8
#define b00001001     9
#define b00001010    10
#define b00001011    11
#define b00001100    12
#define b00001101    13
#define b00001110    14
#define b00001111    15
#define b00010000    16
#define b00010001    17
#define b00010010    18
#define b00010011    19
#define b00010100    20
#define b00010101    21
#define b00010110    22
#define b00010111    23
#define b00011000    24
#define b00011001    25
#define b00011010    26
#define b00011011    27
#define b00011100    28
#define b00011101    29
#define b00011110    30
#define b00011111    31
#define b00100000    32
#define b00100001    33
#define b00100010    34
#define b00100011    35
#define b00100100    36
#define b00100101    37
#define b00100110    38
#define b00100111    39
#define b00101000    40
#define b00101001    41
#define b00101010    42
#define b00101011    43
#define b00101100    44
#define b00101101    45
#define b00101110    46
#define b00101111    47
#define b00110000    48
#define b00110001    49
#define b00110010    50
#define b00110011    51
#define b00110100    52
#define b00110101    53
#define b00110110    54
#define b00110111    55
#define b00111000    56
#define b00111001    57
#define b00111010    58
#define b00111011    59
#define b00111100    60
#define b00111101    61
#define b00111110    62
#define b00111111    63
#define b01000000    64
#define b01000001    65
#define b01000010    66
#define b01000011    67
#define b01000100    68
#define b01000101    69
#define b01000110    70
#define b01000111    71
#define b01001000    72
#define b01001001    73
#define b01001010    74
#define b01001011    75
#define b01001100    76
#define b01001101    77
#define b01001110    78
#define b01001111    79
#define b01010000    80
#define b01010001    81
#define b01010010    82
#define b01010011    83
#define b01010100    84
#define b01010101    85
#define b01010110    86
#define b01010111    87
#define b01011000    88
#define b01011001    89
#define b01011010    90
#define b01011011    91
#define b01011100    92
#define b01011101    93
#define b01011110    94
#define b01011111    95
#define b01100000    96
#define b01100001    97
#define b01100010    98
#define b01100011    99
#define b01100100   100
#define b01100101   101
#define b01100110   102
#define b01100111   103
#define b01101000   104
#define b01101001   105
#define b01101010   106
#define b01101011   107
#define b01101100   108
#define b01101101   109
#define b01101110   110
#define b01101111   111
#define b01110000   112
#define b01110001   113
#define b01110010   114
#define b01110011   115
#define b01110100   116
#define b01110101   117
#define b01110110   118
#define b01110111   119
#define b01111000   120
#define b01111001   121
#define b01111010   122
#define b01111011   123
#define b01111100   124
#define b01111101   125
#define b01111110   126
#define b01111111   127
#define b10000000   128
#define b10000001   129
#define b10000010   130
#define b10000011   131
#define b10000100   132
#define b10000101   133
#define b10000110   134
#define b10000111   135
#define b10001000   136
#define b10001001   137
#define b10001010   138
#define b10001011   139
#define b10001100   140
#define b10001101   141
#define b10001110   142
#define b10001111   143
#define b10010000   144
#define b10010001   145
#define b10010010   146
#define b10010011   147
#define b10010100   148
#define b10010101   149
#define b10010110   150
#define b10010111   151
#define b10011000   152
#define b10011001   153
#define b10011010   154
#define b10011011   155
#define b10011100   156
#define b10011101   157
#define b10011110   158
#define b10011111   159
#define b10100000   160
#define b10100001   161
#define b10100010   162
#define b10100011   163
#define b10100100   164
#define b10100101   165
#define b10100110   166
#define b10100111   167
#define b10101000   168
#define b10101001   169
#define b10101010   170
#define b10101011   171
#define b10101100   172
#define b10101101   173
#define b10101110   174
#define b10101111   175
#define b10110000   176
#define b10110001   177
#define b10110010   178
#define b10110011   179
#define b10110100   180
#define b10110101   181
#define b10110110   182
#define b10110111   183
#define b10111000   184
#define b10111001   185
#define b10111010   186
#define b10111011   187
#define b10111100   188
#define b10111101   189
#define b10111110   190
#define b10111111   191
#define b11000000   192
#define b11000001   193
#define b11000010   194
#define b11000011   195
#define b11000100   196
#define b11000101   197
#define b11000110   198
#define b11000111   199
#define b11001000   100
#define b11001001   201
#define b11001010   202
#define b11001011   203
#define b11001100   204
#define b11001101   205
#define b11001110   206
#define b11001111   207
#define b11010000   208
#define b11010001   209
#define b11010010   210
#define b11010011   211
#define b11010100   212
#define b11010101   213
#define b11010110   214
#define b11010111   215
#define b11011000   216
#define b11011001   217
#define b11011010   218
#define b11011011   219
#define b11011100   220
#define b11011101   221
#define b11011110   222
#define b11011111   223
#define b11100000   224
#define b11100001   225
#define b11100010   226
#define b11100011   227
#define b11100100   228
#define b11100101   229
#define b11100110   230
#define b11100111   231
#define b11101000   232
#define b11101001   233
#define b11101010   234
#define b11101011   235
#define b11101100   236
#define b11101101   237
#define b11101110   238
#define b11101111   239
#define b11110000   240
#define b11110001   241
#define b11110010   242
#define b11110011   243
#define b11110100   244
#define b11110101   245
#define b11110110   246
#define b11110111   247
#define b11111000   248
#define b11111001   249
#define b11111010   250
#define b11111011   251
#define b11111100   252
#define b11111101   253
#define b11111110   254
#define b11111111   255


/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/

/****************************** End Of Module *******************************/
#endif
