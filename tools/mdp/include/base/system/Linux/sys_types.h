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

// g_script_body

DECLARE_STATIC_CONST_STRING(g_script_body, char_a)

// g_script_command_item

DECLARE_STATIC_CONST_STRING(g_script_command_item, char_a)

// g_script_chm_encoding

DECLARE_STATIC_CONST_STRING(g_script_chm_encoding, char_a)

// g_script_chm_cmd

DECLARE_STATIC_CONST_STRING(g_script_chm_cmd, char_a)

// g_script_epub_cmd

DECLARE_STATIC_CONST_STRING(g_script_epub_cmd, char_a)

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

inline const_string_a get_chm_generator() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "chmcpl");
}

inline const_string_a get_script_filename() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "build.sh");
}

inline const_string_a get_array_delimiter() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "\n");
}

inline const_string_a get_command_src() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "\\\"$s.md\\\"");
}
inline const_string_a get_command_dest() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "\\\"$s.html\\\"");
}
inline const_string_a get_command_del() throw()
{
	return DECLARE_TEMP_CONST_STRING(const_string_a, "rm");
}

inline const_string_a get_xhtml_options_template(bool bLatest) throw()
{
	return bLatest
		? DECLARE_TEMP_CONST_STRING(const_string_a, "-x -l $$LANG$$ \\\"$2stylesheet.css\\\"")
		: DECLARE_TEMP_CONST_STRING(const_string_a, "-x    $$LANG$$ \\\"$2stylesheet.css\\\"")
		;
}

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
