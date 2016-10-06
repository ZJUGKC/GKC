/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

%%
digit [0-9]
dot   .
num_exp [eE][\+\-]?{digit}+
float_num {digit}+{dot}{digit}*{num_exp}?|{digit}+{num_exp}|{dot}{digit}+{num_exp}?
TK_CONST_INTEGER  {digit}+
TK_CONST_FLOAT    {TK_CONST_INTEGER}f|{float_num}f
TK_CONST_DOUBLE   {float_num}
letter [a-zA-Z_]
TK_KEY_CLASS      class
TK_KEY_NAMESPACE  namespace
TK_IDENTIFIER     {letter}({letter}|{digit})*
TK_COMMENT_START  /\*
TK_LINE_COMMENT_START  //
hex_digit    [0-9A-Fa-f]
hex_byte     {hex_digit}{hex_digit}?
oct_digit    [0-7]
oct_byte     {oct_digit}({oct_digit}{oct_digit}?)?
escape_char  \\(['"\\0abfnrtv]|x{hex_byte}|{oct_byte})
byte_char    [^'"\r\n]|{escape_char}
TK_CONST_CHAR     '{byte_char}'
TK_CONST_WCHAR    L{TK_CONST_CHAR}
TK_CONST_STRING   "{byte_char}*"
TK_CONST_WSTRING  L{TK_CONST_STRING}
TK_COMMA          ,
TK_BANG           !
TK_SEMI           ;
TK_HYPHEN         \-
TK_LCURLY         \{
TK_RCURLY         \}
TK_LPAREN         \(
TK_RPAREN         \)
TK_LBRACKET       \[
TK_RBRACKET       \]
TK_UPARROW        \^
TK_UPWAVE         ~
TK_STAR           \*
TK_PLUS           \+
TK_SLASH          /
TK_PERCENT        %
TK_LEFT_SHIFT     <<
TK_RIGHT_SHIFT    >>
TK_DOT            {dot}
TK_COLON          :
TK_EQUAL          =
TK_ET             &
TK_VERT           \|
TK_LOG_EQ         ==
TK_LOG_NE         !=
TK_LOG_GE         >=
TK_LOG_LE         <=
TK_LOG_GT         >
TK_LOG_LT         <
TK_LOG_AND        &&
TK_LOG_OR         \|\|
TK_SPACE          [\s\t]+
TK_RETURN         \r|\n|\r\n
