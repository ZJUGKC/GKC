/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains the string of file called won.lex,
i.e., the lexical definition of WON.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_won_lex

BEGIN_STATIC_CONST_STRING(g_won_lex)
	STATIC_CONST_STRING_ENTRY(
"\xEF\xBB\xBF"
"%%" "\r\n"
"digit_nonzero [1-9]" "\r\n"
"digit_zero 0" "\r\n"
"digit {digit_zero}|{digit_nonzero}" "\r\n"
"num_exp [eE][\\+\\-]?{digit}+" "\r\n"
"dot ." "\r\n"
"hex_digit {digit}|[A-Fa-f]" "\r\n"
"escape_char \\\\([\"\\\\/bfnrt]|u{hex_digit}{hex_digit}({hex_digit}{hex_digit})?)" "\r\n"
"byte_char [^\\\\\"\\x00-\\x1F]|{escape_char}" "\r\n"
"TK_STRING \"{byte_char}*\"" "\r\n"
"TK_NUMBER_INTEGER \\-?({digit_zero}|{digit_nonzero}{digit}*)" "\r\n"
"TK_NUMBER_FLOAT {TK_NUMBER_INTEGER}({dot}{digit}+{num_exp}?|{num_exp})" "\r\n"
"hex_byte  {hex_digit}{hex_digit}" "\r\n"
"hex_byte2 {hex_byte}?{hex_byte}" "\r\n"
"hex_byte3 {hex_byte2}?{hex_byte}" "\r\n"
"hex_byte4 {hex_byte3}?{hex_byte}" "\r\n"
"hex_byte5 {hex_byte4}?{hex_byte}" "\r\n"
"hex_byte6 {hex_byte5}?{hex_byte}" "\r\n"
"hex_byte7 {hex_byte6}?{hex_byte}" "\r\n"
"hex_byte8 {hex_byte7}?{hex_byte}" "\r\n"
"TK_NUMBER_HEXADECIMAL {digit_zero}x{hex_byte8}" "\r\n"
"TK_LCURLY \\{" "\r\n"
"TK_RCURLY \\}" "\r\n"
"TK_COLON :" "\r\n"
"TK_COMMA ," "\r\n"
"TK_LBRACKET \\[" "\r\n"
"TK_RBRACKET \\]" "\r\n"
"TK_BOOLEAN_TRUE true" "\r\n"
"TK_BOOLEAN_FALSE false" "\r\n"
"TK_KEY_NULL null" "\r\n"
"TK_COMMENT_START /\\*" "\r\n"
"TK_SPACE [\\s\\t]+" "\r\n"
"TK_RETURN \\n|\\r\\n?" "\r\n"
	)
END_STATIC_CONST_STRING(g_won_lex)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
