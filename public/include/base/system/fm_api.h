/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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

// File Management Tools

//------------------------------------------------------------------------------
//OS

#if defined(GKC_OS_WINDOWS)
	#include "Windows/fm_api.h"
#elif defined(GKC_OS_LINUX)
	#include "Linux/fm_api.h"
#else
	#error Error OS type!
#endif

//------------------------------------------------------------------------------
// directory

//Create directory regardless of the missing of intermediate directories.
inline bool force_directory(char_s* szString) throw()
{
	bool bExisting = true;
	bool bChecked = false;
	char_s* sz = szString;
	//find
	while( *sz != 0 ) {
		// `:` + `/\\`
		if( check_drive_separator(*sz) ) {
			sz ++;
			while( check_path_separator(*sz) )
				sz ++;
			continue;
		}
		// `/\\`
		if( check_path_separator(*sz) ) {
			if( sz != szString ) {
				char_s ch = *sz;
				*sz = 0;
				bExisting = check_directory_exists(szString);
				bChecked = true;
				*sz = ch;
				if( !bExisting )
					break;
			}
			sz ++;
			while( check_path_separator(*sz) )
				sz ++;
			if( check_drive_separator(*sz) ) {
				if( bChecked )
					bChecked = false;
			}
			continue;
		}
		if( bChecked )
			bChecked = false;
		sz ++;
	} //end while
	//check
	if( sz == szString )
		return true;
	if( !bChecked )
		bExisting = check_directory_exists(szString);
	if( bExisting )
		return true;
	//create directories recursively
	do {
		char_s ch = *sz;
		*sz = 0;
		if( !create_directory(szString) ) {
			*sz = ch;
			return false;
		}
		*sz = ch;
		while( check_path_separator(*sz) )
			sz ++;
		if( *sz == 0 )
			break;
		do {
			sz ++;
		} while( *sz != 0 && !check_path_separator(*sz) );
	} while( true ); //end while
	return true;
}

////////////////////////////////////////////////////////////////////////////////
