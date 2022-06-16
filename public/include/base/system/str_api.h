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
inline char_a char_lower(char_a c) throw()
{
	return (char_a)::tolower((int)(unsigned char)c);
}
inline char_h char_lower(char_h c) throw()
{
	return (char_h)::towlower((wint_t)c);
}
inline char_l char_lower(char_l c) throw()
{
	return (char_l)::towlower((wint_t)c);
}
// char_upper
inline char_a char_upper(char_a c) throw()
{
	return (char_a)::toupper((int)(unsigned char)c);
}
inline char_h char_upper(char_h c) throw()
{
	return (char_h)::towupper((wint_t)c);
}
inline char_l char_upper(char_l c) throw()
{
	return (char_l)::towupper((wint_t)c);
}

// calc_string_length

inline uintptr calc_string_length(const char_a* s) throw()
{
	return ::strlen(s);
}

// find_string_char

inline char_a* find_string_char(const char_a* s, int c) throw()
{
	return (char_a*)::strchr(s, c);
}

// find_string_last_char

inline char_a* find_string_last_char(const char_a* s, int c) throw()
{
	return (char_a*)::strrchr(s, c);
}

// find_string_charset

inline char_a* find_string_charset(const char_a* s, const char_a* z) throw()
{
	return (char_a*)::strpbrk(s, z);
}

// find_string_string

inline char_a* find_string_string(const char_a* s, const char_a* z) throw()
{
	return (char_a*)::strstr(s, z);
}

// char_is_*
inline bool char_is_alnum(const char_a& ch) throw()
{
	return ::isalnum((int)(unsigned char)ch) != 0;
}
inline bool char_is_alpha(const char_a& ch) throw()
{
	return ::isalpha((int)(unsigned char)ch) != 0;
}
inline bool char_is_cntrl(const char_a& ch) throw()
{
	return ::iscntrl((int)(unsigned char)ch) != 0;
}
inline bool char_is_graph(const char_a& ch) throw()
{
	return ::isgraph((int)(unsigned char)ch) != 0;
}
inline bool char_is_lower(const char_a& ch) throw()
{
	return ::islower((int)(unsigned char)ch) != 0;
}
inline bool char_is_upper(const char_a& ch) throw()
{
	return ::isupper((int)(unsigned char)ch) != 0;
}
inline bool char_is_digit(const char_a& ch) throw()
{
	return ::isdigit((int)(unsigned char)ch) != 0;
}
inline bool char_is_xdigit(const char_a& ch) throw()
{
	return ::isxdigit((int)(unsigned char)ch) != 0;
}
inline bool char_is_blank(const char_a& ch) throw()
{
	return ::isblank((int)(unsigned char)ch) != 0;
}
inline bool char_is_space(const char_a& ch) throw()
{
	return ::isspace((int)(unsigned char)ch) != 0;
}
inline bool char_is_print(const char_a& ch) throw()
{
	return ::isprint((int)(unsigned char)ch) != 0;
}
inline bool char_is_punct(const char_a& ch) throw()
{
	return ::ispunct((int)(unsigned char)ch) != 0;
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
+      Use a sign (+ or -) to prefix the output value if it is of a signed type.
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
h        char_a (c, s)
l        char_w (c, s)

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
c        Character. hc for char_a character, lc for char_w character, c for char_s character.
s        String. hs for char_a string, ls for char_w string, s for char_s string.
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
h        char_a (c, s, [])
l        char_w (c, s, [])

type:
d         (int*)      Decimal integer.
o         (int*)      Octal integer.
u         (uint*)     Unsigned decimal integer.
x         (int*)      Hexadecimal integer.
e, f, g   (float*)    Floating-point value.
s                     String (char_s). Use [width] option without including the terminator.
c                     Character (char_s). Use [width] option without including the terminator.
[ccc]                 Matches a nonempty sequence of characters from the specified set of accepted characters. Use [width] option without including the terminator.
or [c-c]
or [^ccc]             NOT in a set of characters.
%                     "%%", Matches a literal '%'.
*/

// string_to_value
//   return value: the pointer to the next character. If this value is equal to szString, no conversion is performed.
inline char_a* string_to_value(const char_a* szString, float& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = ::strtof(szString, &pN);  // v may be +-HUGE_VALF
	bOK = (errno == 0);  // ERANGE
	return pN;
}
inline char_a* string_to_value(const char_a* szString, double& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = ::strtod(szString, &pN);  // v may be +-HUGE_VAL
	bOK = (errno == 0);  // ERANGE
	return pN;
}

inline char_a* string_to_value(const char_a* szString, int iBase, int& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = (int)::strtol(szString, &pN, iBase);  // v may be LONG_MAX or LONG_MIN (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}
inline char_a* string_to_value(const char_a* szString, int iBase, uint& v, bool& bOK) throw()
{
	char_a* pN = NULL;
	errno = 0;
	v = (uint)::strtoul(szString, &pN, iBase);  // v may be ULONG_MAX (may be 64bits under linux)
	bOK = (errno == 0);  // ERANGE or EINVAL
	return pN;
}

//------------------------------------------------------------------------------
// path types

BEGIN_ENUM(path_types)
	ENUM_VALUE_ENTRY(None, 0)
	ENUM_VALUE_ENTRY(Relative, 1)
	ENUM_VALUE_ENTRY(Absolute, 2)
END_ENUM()

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

inline int compare_string(const char_a* s1, const char_a* s2) throw()
{
	return ::strcmp(s1, s2);
}

//------------------------------------------------------------------------------
// const string

// const_string_helper

class const_string_helper
{
public:
	//To C-style string
	template <typename Tchar>
	static ref_ptr<Tchar> To_C_Style(const const_string_t<Tchar>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetCount() );
		return ref_ptr<Tchar>(const_array_helper::GetInternalPointer(str) + uStart);
	}

	//find (return value : check null)
	template <typename Tchar>
	static typename const_string_t<Tchar>::Iterator Find(const const_string_t<Tchar>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetCount() );
		assert( !str.IsNull() );
		return typename const_string_t<Tchar>::Iterator(ref_ptr<Tchar>(find_string_char(const_array_helper::GetInternalPointer(str) + uStart, ch)));
	}
	//find last (return value : check null)
	template <typename Tchar>
	static typename const_string_t<Tchar>::Iterator FindLast(const const_string_t<Tchar>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetCount() );
		assert( !str.IsNull() );
		return typename const_string_t<Tchar>::Iterator(ref_ptr<Tchar>(find_string_last_char(const_array_helper::GetInternalPointer(str) + uStart, ch)));
	}
};

//------------------------------------------------------------------------------
// path

inline void get_path_extension_start(char_a& ch) throw()
{
	ch = '.';
}
inline bool check_path_extension_start(const char_a& ch) throw()
{
	return ch == '.';
}

//check last separator of path
template <typename t_char>
inline bool check_path_last_separator(const t_char* szString, uintptr uLength) throw()
{
	return uLength > 0 && (check_path_separator(szString[uLength - 1]) || check_drive_separator(szString[uLength - 1]));
}
template <typename t_char>
inline bool check_path_deletable_last_separator(const t_char* szString, uintptr uLength) throw()
{
	return uLength > 0 && check_path_separator(szString[uLength - 1])
		&& (uLength > 1) && !check_drive_separator(szString[uLength - 2]);
}

//find extension start of path
template <typename t_char>
inline t_char* find_path_extension_start(const t_char* szString, uintptr uLength) throw()
{
	if( uLength == 0 )
		return NULL;
	t_char* sz = (t_char*)szString;
	sz += uLength;
	do {
		sz --;
		t_char& ch = *sz;
		if( check_path_separator(ch) || check_drive_separator(ch) )
			return NULL;
		if( check_path_extension_start(ch) )
			return sz;
	} while( sz != szString );
	return NULL;
}

//find file part start of path
template <typename t_char>
inline t_char* find_path_file_part_start(const t_char* szString, uintptr uLength) throw()
{
	if( uLength == 0 )
		return (t_char*)szString;
	t_char* sz = (t_char*)szString;
	sz += uLength;
	do {
		sz --;
		t_char& ch = *sz;
		if( check_path_separator(ch) || check_drive_separator(ch) )
			return sz + 1;
	} while( sz != szString );
	return (t_char*)szString;
}

////////////////////////////////////////////////////////////////////////////////
