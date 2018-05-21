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

/*
This file contains global variables for text stream component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "stream/TextStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// array

// windows

BEGIN_STATIC_CONST_ARRAY(g_windows_crlf_a)
	STATIC_CONST_ARRAY_ENTRY('\r')
	STATIC_CONST_ARRAY_ENTRY('\n')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_windows_crlf_a)

BEGIN_STATIC_CONST_ARRAY(g_windows_crlf_h)
	STATIC_CONST_ARRAY_ENTRY('\r')
	STATIC_CONST_ARRAY_ENTRY('\n')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_windows_crlf_h)

BEGIN_STATIC_CONST_ARRAY(g_windows_crlf_l)
	STATIC_CONST_ARRAY_ENTRY('\r')
	STATIC_CONST_ARRAY_ENTRY('\n')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_windows_crlf_l)

// unix

BEGIN_STATIC_CONST_ARRAY(g_unix_crlf_a)
	STATIC_CONST_ARRAY_ENTRY('\n')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_unix_crlf_a)

BEGIN_STATIC_CONST_ARRAY(g_unix_crlf_h)
	STATIC_CONST_ARRAY_ENTRY('\n')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_unix_crlf_h)

BEGIN_STATIC_CONST_ARRAY(g_unix_crlf_l)
	STATIC_CONST_ARRAY_ENTRY('\n')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_unix_crlf_l)

// mac

BEGIN_STATIC_CONST_ARRAY(g_mac_crlf_a)
	STATIC_CONST_ARRAY_ENTRY('\r')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_mac_crlf_a)

BEGIN_STATIC_CONST_ARRAY(g_mac_crlf_h)
	STATIC_CONST_ARRAY_ENTRY('\r')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_mac_crlf_h)

BEGIN_STATIC_CONST_ARRAY(g_mac_crlf_l)
	STATIC_CONST_ARRAY_ENTRY('\r')
	STATIC_CONST_ARRAY_ENTRY_LAST('\0')
END_STATIC_CONST_ARRAY(g_mac_crlf_l)

// text stream

BEGIN_COM_TYPECAST(TextStream)
	COM_TYPECAST_ENTRY(_ITextStream, _ITextStream)
END_COM_TYPECAST()

CallResult TextStream::write_crlf(uint uBytesPerChar) throw()
{
	const byte* p = NULL;
	uintptr uChars = 0;
	if( m_iCRLFStyle == _CRLFStyles::Unix ) {
		if( uBytesPerChar == sizeof(CharA) ) {
			p = (const byte*)g_unix_crlf_a::GetAddress();
			uChars = g_unix_crlf_a::GetCount();
		}
		else if( uBytesPerChar == sizeof(CharH) ) {
			p = (const byte*)g_unix_crlf_h::GetAddress();
			uChars = g_unix_crlf_h::GetCount();
		}
		else if( uBytesPerChar == sizeof(CharL) ) {
			p = (const byte*)g_unix_crlf_l::GetAddress();
			uChars = g_unix_crlf_l::GetCount();
		}
		else {
			assert( false );
		}
	}
	else if( m_iCRLFStyle == _CRLFStyles::Mac ) {
		if( uBytesPerChar == sizeof(CharA) ) {
			p = (const byte*)g_mac_crlf_a::GetAddress();
			uChars = g_mac_crlf_a::GetCount();
		}
		else if( uBytesPerChar == sizeof(CharH) ) {
			p = (const byte*)g_mac_crlf_h::GetAddress();
			uChars = g_mac_crlf_h::GetCount();
		}
		else if( uBytesPerChar == sizeof(CharL) ) {
			p = (const byte*)g_mac_crlf_l::GetAddress();
			uChars = g_mac_crlf_l::GetCount();
		}
		else {
			assert( false );
		}
	}
	else {
		//windows
		if( uBytesPerChar == sizeof(CharA) ) {
			p = (const byte*)g_windows_crlf_a::GetAddress();
			uChars = g_windows_crlf_a::GetCount();
		}
		else if( uBytesPerChar == sizeof(CharH) ) {
			p = (const byte*)g_windows_crlf_h::GetAddress();
			uChars = g_windows_crlf_h::GetCount();
		}
		else if( uBytesPerChar == sizeof(CharL) ) {
			p = (const byte*)g_windows_crlf_l::GetAddress();
			uChars = g_windows_crlf_l::GetCount();
		}
		else {
			assert( false );
		}
	} //end if
	return write_characters(p, uChars, uBytesPerChar);
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
