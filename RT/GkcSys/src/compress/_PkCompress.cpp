/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

/*
internal
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "compress/_PkCompress.h"

////////////////////////////////////////////////////////////////////////////////

/*
matching length:
<2 : non-encoded string
2 : only encode string when pos < 100H
non-encoded: output 0 to flag buffer, output this non-encoded byte to data buffer
encoded: output 1 to flag buffer, output length bits to flag buffer,
         output length byte to data buffer if length > 9,
         output high-order position byte bits to flag buffer,
         output low-order position byte to data buffer
*/

// encoded length table
/*
length     2    3   4    5    6     7     8     9     >9
encoded    110  00  101  001  1011  0011  1111  0111  010
reverse    011  00  101  100  1101  1100  1111  1110  010  instant code
*/
byte g_EncodeLenLenTable[] = {
3, 2, 3, 3, 4, 4, 4, 4, 3
};

ushort g_EncodeLenBitsTable[] = {
6, 0, 5, 1, 0xB, 3, 0xF, 7, 2
};

// encoded position table (high byte)
/*
32 items:
          0        1        2        3        4        5        6
string    1        0000     1000     00100    10100    01100    11100
reverse   1        0000     0001     00100    00101    00110    00111
          7        8        9        0A       0B       0C       0D
string    000010   100010   010010   110010   001010   101010   011010
reverse   010000   010001   010010   010011   010100   010101   010110
          0E       0F       10       11       12       13       14
string    0111010  1111010  0000110  1000110  0100110  1100110  0010110
reverse   0101110  0101111  0110000  0110001  0110010  0110011  0110100
          15       16       17       18       19       1A       1B
string    1010110  0110110  1110110  0001110  1001110  0101110  1101110
reverse   0110101  0110110  0110111  0111000  0111001  0111010  0111011
          1C       1D       1E       1F
string    0011110  1011110  0111110  1111110
reverse   0111100  0111101  0111110  0111111
*/
byte g_EncodePosLenTable[] = {
1, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6,
7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

byte g_EncodePosBitsTable[] = {
0x01, 0x00, 0x08, 0x04, 0x14, 0x0C,
0x1C, 0x02, 0x22, 0x12, 0x32, 0x0A,
0x2A, 0x1A, 0x3A, 0x7A, 0x06, 0x46,
0x26, 0x66, 0x16, 0x56, 0x36, 0x76,
0x0E, 0x4E, 0x2E, 0x6E, 0x1E, 0x5E,
0x3E, 0x7E
};

//decode
byte g_DecodeLenTable[] = {
	3, 0, 0xA, 2, 5, 4,
	0, 0, 0, 0, 0, 0,
	7, 6, 9, 8
};
byte g_DecodePosTable[] = {
	1, 2, //Decode to pages(each page 100H)
	0, 0, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0,
	7, 8, 9, 0xA, 0xB, 0xC, 0xD
};

////////////////////////////////////////////////////////////////////////////////
