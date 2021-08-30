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
This file contains the string of file called won.gra,
i.e., the grammar definition of WON.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// g_won_gra

BEGIN_STATIC_CONST_STRING(g_won_gra)
	STATIC_CONST_STRING_ENTRY(
"\xEF\xBB\xBF"
"%%" "\r\n"
"value : string_catenation { do_value_string_catenation }" "\r\n"
"	| TK_NUMBER_INTEGER { do_value_number_integer }" "\r\n"
"	| TK_NUMBER_FLOAT { do_value_number_float }" "\r\n"
"	| TK_NUMBER_HEXADECIMAL { do_value_number_hexadecimal }" "\r\n"
"	| object { do_value_object }" "\r\n"
"	| array { do_value_array }" "\r\n"
"	| TK_BOOLEAN_TRUE { do_value_boolean_true }" "\r\n"
"	| TK_BOOLEAN_FALSE { do_value_boolean_false }" "\r\n"
"	| TK_KEY_NULL { do_value_null }" "\r\n"
"	;" "\r\n"
"string_catenation : string_catenation TK_STRING { do_string_catenation_string }" "\r\n"
"	| TK_STRING { do_string_catenation }" "\r\n"
"	;" "\r\n"
"object : begin_object end_object { do_object_null }" "\r\n"
"	| begin_object pair_list end_object { do_object_pair_list }" "\r\n"
"	;" "\r\n"
"begin_object : TK_LCURLY { do_begin_object }" "\r\n"
"	;" "\r\n"
"end_object : TK_RCURLY { do_end_object }" "\r\n"
"	;" "\r\n"
"pair_list : pair_list TK_COMMA pair { do_pair_list_list_pair }" "\r\n"
"	| pair { do_pair_list_pair }" "\r\n"
"	;" "\r\n"
"pair : key TK_COLON value { do_pair_key_value }" "\r\n"
"	;" "\r\n"
"key : TK_STRING { do_key_string }" "\r\n"
"	;" "\r\n"
"array : begin_array end_array { do_array_null }" "\r\n"
"	| begin_array value_list end_array { do_array_value_list }" "\r\n"
"	;" "\r\n"
"begin_array : TK_LBRACKET { do_begin_array }" "\r\n"
"	;" "\r\n"
"end_array : TK_RBRACKET { do_end_array }" "\r\n"
"	;" "\r\n"
"value_list : value_list TK_COMMA value { do_value_list_list_value }" "\r\n"
"	| value { do_value_list_value }" "\r\n"
"	;" "\r\n"
	)
END_STATIC_CONST_STRING(g_won_gra)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
