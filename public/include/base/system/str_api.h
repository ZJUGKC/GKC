/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//tools

// char_lower
inline CharA char_lower(CharA c) throw()
{
	return (CharA)::tolower((int)c);
}
inline CharH char_lower(CharH c) throw()
{
	return (CharH)::towlower((wint_t)c);
}
inline CharL char_lower(CharL c) throw()
{
	return (CharL)::towlower((wint_t)c);
}
// char_upper
inline CharA char_upper(CharA c) throw()
{
	return (CharA)::toupper((int)c);
}
inline CharH char_upper(CharH c) throw()
{
	return (CharH)::towupper((wint_t)c);
}
inline CharL char_upper(CharL c) throw()
{
	return (CharL)::towupper((wint_t)c);
}

// calc_string_length

inline uintptr calc_string_length(const CharA* s) throw()
{
	return ::strlen(s);
}

// find_string_char

inline CharA* find_string_char(const CharA* s, int c) throw()
{
	return (CharA*)::strchr(s, c);
}

// find_string_last_char

inline CharA* find_string_last_char(const CharA* s, int c) throw()
{
	return (CharA*)::strrchr(s, c);
}

// find_string_string

inline CharA* find_string_string(const CharA* s, const CharA* z) throw()
{
	return (CharA*)::strstr(s, z);
}

//------------------------------------------------------------------------------
// Unified Format String for output
/*
%[flags][width][.precision][modifier]type

flags:
#      The flag uses 0 to prefix nonzero output value with o format.
       The flag uses 0x or 0X to to prefix nonzero output value with x or X format.
       The flag forces the output value to contain a decimal point with e, E, f, a or A format.
       The flag forces the output value to contain a decimal point and prevents the truncation of trailing zeros with g or G format.
       Ignored when used with c, d, u, or s.
0      The leading zeros are added until the minimum width is reached. If both 0 and – appear, or 0 is specified as an integer format (u, x, X, o, d) and a precision specification is also present (for example, %04.d), the 0 is ignored.
-      Left align the result within the given field width.
+      Use a sign (+ or –) to prefix the output value if it is of a signed type.
blank  ' ', use a blank to prefix the output value if it is signed and positive. The blank is ignored if both the blank and + flags appear.

width:
Specify a non-negative decimal integer that controls the minimum number of characters that are output without truncation.
Specify an asterisk (*), an int argument from the argument list supplies the value before the value that's being formatted.

precision:
It consists of a period (.) followed by a non-negative decimal integer that, depending on the conversion type, specifies the number of string characters, the number of decimal places, or the number of significant digits to be output.
The precision specification can cause either truncation of the output value or rounding of a floating-point value. If precision is specified as 0 and the value to be converted is 0, the result is no characters output.
Specify an asterisk (*), an int argument from the argument list supplies the value before the value that's being formatted.
a, A        The precision specifies the number of digits after the point, default is 6. If precision is 0, no decimal point is printed unless the # flag is used.
d, u, o,    The precision specifies the minimum number of digits to be printed without truncation.
x, X
e, E        The precision specifies the number of digits to be printed after the decimal point. The last printed digit is rounded.
            Default precision is 6. If precision is 0 or the period (.) appears without a number following it, no decimal point is printed.
f           The precision value specifies the number of digits after the decimal point. If a decimal point appears, at least one digit appears before it. The value is rounded to the appropriate number of digits.
            Default precision is 6. If precision is 0, or if the period (.) appears without a number following it, no decimal point is printed.
g, G        The precision specifies the maximum number of significant digits printed.
            Default is that six significant digits are printed, and any trailing zeros are truncated.
s           The precision specifies the maximum number of characters to be printed. Characters in excess of precision are not printed.
c           No effect.

modifier:
h        short (d, o, x, X) or ushort (o, u, x, X)
I64      int64 (d, o, x, X) or uint64 (o, u, x, X)
I        intptr (d, o, x, X) or uintptr (o, u, x, X)
h        CharA (c, s)
l        CharW (c, s)

types:
d        Signed decimal integer.
o        Unsigned octal integer.
u        Unsigned decimal integer.
x        Unsigned hexadecimal integer; uses "abcdef."
X        Unsigned hexadecimal integer; uses "ABCDEF."
a        Signed hexadecimal double-precision floating-point value that has the form [−]0xh.hhhh p±dd.
A        Signed hexadecimal double-precision floating-point value that has the form [−]0XH.HHHH P±dd.
e        Signed value that has the form [–]d.dddd e [sign]dd[d].
E        Identical to the e format except that E rather than e introduces the exponent.
f        Signed value that has the form [–]dddd.dddd
g        Signed values are displayed in f or e format, whichever is more compact for the given value and precision.
         The e format is used only when the exponent of the value is less than –4 or greater than or equal to the precision argument.
         Trailing zeros are truncated, and the decimal point appears only if one or more digits follow it.
G        Identical to the g format, except that E, rather than e, introduces the exponent.
p        Displays the argument as an address in hexadecimal digits.
c        Character. hc for CharA character, lc for CharW character, c for CharS character.
s        String. hs for CharA string, ls for CharW string, s for CharS string.
%        "%%", print a '%'.
*/

// Unified Format String for input
/*
%[*][width][modifier]type

An asterisk (*) following the percent sign suppresses assignment of the next input field, which is interpreted as a field of the specified type.
The field is scanned but not stored.

width:
A positive decimal integer controlling the maximum number of characters to be read for that field.

modifier:
h        short (d, o, x) or ushort (u)
l        double (e, f, g)
I64      int64 (d, o, u, x)
h        CharA (c, s, [])
l        CharW (c, s, [])

type:
d         (int*)      Decimal integer.
o         (int*)      Octal integer.
u         (uint*)     Unsigned decimal integer.
x         (int*)      Hexadecimal integer.
e, f, g   (float*)    Floating-point value.
s                     String (CharS). Use [width] option without including the terminator.
c                     Character (CharS). Use [width] option without including the terminator.
[ccc]                 Matches a nonempty sequence of characters from the specified set of accepted characters. Use [width] option without including the terminator.
or [c-c]
or [^ccc]             NOT in a set of characters.
%                     "%%", Matches a literal '%'.
*/

// string_to_value
//   return value: the pointer to the next character. If this value is equal to szString, no conversion is performed.
inline CharA* string_to_value(const CharA* szString, float& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = ::strtof(szString, &pN);  // v may be +-HUGE_VALF
	bOK = (errno == 0);  // ERANGE
	return pN;
}
inline CharA* string_to_value(const CharA* szString, double& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = ::strtod(szString, &pN);  // v may be +-HUGE_VAL
	bOK = (errno == 0);  // ERANGE
	return pN;
}

inline CharA* string_to_value(const CharA* szString, int iBase, int& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = (int)::strtol(szString, &pN, iBase);  // v may be LONG_MAX or LONG_MIN (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline CharA* string_to_value(const CharA* szString, int iBase, uint& v, bool& bOK) throw()
{
	CharA* pN = NULL;
	errno = 0;
	v = (uint)::strtoul(szString, &pN, iBase);  // v may be ULONG_MAX (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/str_api.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/str_api.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
//tools

// compare_string

inline int compare_string(const CharA* s1, const CharA* s2) throw()
{
	return ::strcmp(s1, s2);
}

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
