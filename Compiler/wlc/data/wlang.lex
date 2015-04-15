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

digit [0-9]
dot   .
num_exp [eE][\+\-]?{digit}+
float_num {digit}+{dot}{digit}*{num_exp}?|{digit}+{num_exp}|{dot}{digit}+{num_exp}?
TK_CONST_INTEGER  {digit}+
TK_CONST_FLOAT    {TK_CONST_INTEGER}f|{float_num}f
TK_CONST_DOUBLE   {float_num}
letter [a-zA-Z_]
TK_IDENTIFIER     {letter}({letter}|{digit})*
TK_KEY_CLASS      class
TK_KEY_NAMESPACE  namespace
TK_COMMENT_START  /\*|//
hex_digit    [0-9A-Fa-f]
hex_byte     {hex_digit}{hex_digit}?
oct_digit    [0-7]
oct_byte     {oct_digit}({oct_digit}{oct_digit}?)?
escape_char  \\([′″\\0abfnrtv]|x{hex_byte}|{oct_byte})
byte_char    [^′″\r\n]|{escape_char}
TK_CONST_CHAR     ′{byte_char}′
TK_CONST_WCHAR    L{TK_CONST_CHAR}
TK_CONST_STRING   ″{byte_char}*″
TK_CONST_WSTRING  L{TK_CONST_STRING}
TK_COMMA       ,
TK_BANG        !
TK_SEMI        ;
TK_HYPHEN      \-
TK_LCURLY      \{
TK_RCURLY      \}
TK_LPAREN      \(
TK_RPAREN      \)
TK_LBRACKET    \[
TK_RBRACKET    \]
TK_UPARROW     \^
TK_UPWAVE      ~
TK_STAR        \*
TK_PLUS        \+
TK_SLASH       /
TK_PERCENT     %
TK_DOT         {dot}
TK_COLON       :
TK_EQUAL       =
TK_ET          &
TK_VERT        \|
TK_LOG_EQ      ==
TK_LOG_NE      !=
TK_LOG_GE      >=
TK_LOG_LE      <=
TK_LOG_GT      >
TK_LOG_LT      <
TK_LOG_AND     &&
TK_LOG_OR      \|\|
TK_SPACE       [\s\t]+
TK_RETURN      \r|\n|\r\n
%%
TK_CONST_INTEGER { do_const_integer }
TK_CONST_FLOAT   { do_const_float }
TK_CONST_DOUBLE  { do_const_double }
TK_IDENTIFIER    { do_identifier }
TK_KEY_CLASS     { do_class }
TK_KEY_NAMESPACE { do_namespace }
TK_COMMENT_START { do_comment_start }
TK_CONST_CHAR    { do_const_char }
TK_CONST_WCHAR   { do_const_wchar }
TK_CONST_STRING  { do_const_string }
TK_CONST_WSTRING { do_const_wstring }
TK_COMMA         { do_comma }
TK_BANG          { do_bang }
TK_SEMI          { do_semi }
TK_HYPHEN        { do_hyphen }
TK_LCURLY        { do_lcurly }
TK_RCURLY        { do_rcurly }
TK_LPAREN        { do_lparen }
TK_RPAREN        { do_rparen }
TK_LBRACKET      { do_lbracket }
TK_RBRACKET      { do_rbracket }
TK_UPARROW       { do_uparrow }
TK_UPWAVE        { do_upwave }
TK_STAR          { do_star }
TK_PLUS          { do_plus }
TK_SLASH         { do_slash }
TK_PERCENT       { do_percent }
TK_DOT           { do_dot }
TK_COLON         { do_colon }
TK_EQUAL         { do_euqal }
TK_ET            { do_et }
TK_VERT          { do_vert }
TK_LOG_EQ        { do_log_eq }
TK_LOG_NE        { do_log_ne }
TK_LOG_GE        { do_log_ge }
TK_LOG_LE        { do_log_le }
TK_LOG_GT        { do_log_gt }
TK_LOG_LT        { do_log_lt }
TK_LOG_AND       { do_log_and }
TK_LOG_OR        { do_log_or }
TK_SPACE         { do_space }
TK_RETURN        { do_return }
