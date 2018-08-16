/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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

//Linux

//------------------------------------------------------------------------------
//strings

// g_chm_def_body
DECLARE_STATIC_CONST_ARRAY(g_chm_def_body, DECLARE_CONST_STRING_STRUCT_MEMBER_TYPE(char_a))

//------------------------------------------------------------------------------
//functions

inline const_string_a get_crlf_string() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "\n");
}

inline const_string_a get_def_file_ext() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, ".conf");
}

inline void get_lcid_string(uint uLCID, lcid_array& arr, uintptr& uSize) throw()
{
	uSize = 0;
	char_a* p = fixed_array_helper::GetInternalPointer(arr);
	p[0] = 0;
	int ret = value_to_string(p, lcid_array::c_size,
							"%u", uLCID);
	if( ret >= 0 ) {
		uSize = ret;
		p[ret] = 0;
	}
}

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
