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
This file contains component class of text stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TEXT_STREAM_H__
#define __TEXT_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

#include "_TxtStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// TextStream
//   The author of buffer implementation : Lijuan Mei

class TextStream : public _ITextStreamRootImpl,
				public _ITextStreamString,
				public _ITextUtility
{
public:
	TextStream() throw()
	{
	}
	~TextStream() throw()
	{
		do_outstanding_work();
	}

// _ITextStreamString methods
	virtual GKC::CallResult GetAllStringA(_StringA& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringA, _ShareArrayHelper>(MODE_ALL, m_carrSeparatorSetA, str);
	}
	virtual GKC::CallResult GetAllStringH(_StringH& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringH, _ShareArrayHelper>(MODE_ALL, m_carrSeparatorSetH, str);
	}
	virtual GKC::CallResult GetAllStringL(_StringL& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringL, _ShareArrayHelper>(MODE_ALL, m_carrSeparatorSetL, str);
	}
	virtual GKC::CallResult GetAllString(_VariantString& str) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = GetAllStringH(str.GetString<_StringH>());
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = GetAllStringL(str.GetString<_StringL>());
			break;
		default:
			cr = GetAllStringA(str.GetString<_StringA>());
			break;
		}
		return cr;
	}
	virtual GKC::CallResult GetStringA(_StringA& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringA, _ShareArrayHelper>(MODE_SEPARATOR, m_carrSeparatorSetA, str);
	}
	virtual GKC::CallResult GetStringH(_StringH& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringH, _ShareArrayHelper>(MODE_SEPARATOR, m_carrSeparatorSetH, str);
	}
	virtual GKC::CallResult GetStringL(_StringL& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringL, _ShareArrayHelper>(MODE_SEPARATOR, m_carrSeparatorSetL, str);
	}
	virtual GKC::CallResult GetString(_VariantString& str) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = GetStringH(str.GetString<_StringH>());
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = GetStringL(str.GetString<_StringL>());
			break;
		default:
			cr = GetStringA(str.GetString<_StringA>());
			break;
		}
		return cr;
	}
	virtual GKC::CallResult GetLineA(_StringA& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringA, _ShareArrayHelper>(MODE_CRLF, m_carrSeparatorSetA, str);
	}
	virtual GKC::CallResult GetLineH(_StringH& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringH, _ShareArrayHelper>(MODE_CRLF, m_carrSeparatorSetH, str);
	}
	virtual GKC::CallResult GetLineL(_StringL& str) throw()
	{
		assert( !str.IsBlockNull() );
		return get_string<_StringL, _ShareArrayHelper>(MODE_CRLF, m_carrSeparatorSetL, str);
	}
	virtual GKC::CallResult GetLine(_VariantString& str) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = GetLineH(str.GetString<_StringH>());
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = GetLineL(str.GetString<_StringL>());
			break;
		default:
			cr = GetLineA(str.GetString<_StringA>());
			break;
		}
		return cr;
	}
	virtual GKC::CallResult PutString(const _VariantString& str) throw()
	{
		CallResult cr;
		switch( m_iBomType ) {
		case _BOMTypes::UTF16LE:
		case _BOMTypes::UTF16BE:
			cr = PutStringH(_StringUtilHelper::To_ConstString(str.GetString<_StringH>()));
			break;
		case _BOMTypes::UTF32LE:
		case _BOMTypes::UTF32BE:
			cr = PutStringL(_StringUtilHelper::To_ConstString(str.GetString<_StringL>()));
			break;
		default:
			cr = PutStringA(_StringUtilHelper::To_ConstString(str.GetString<_StringA>()));
			break;
		}
		return cr;
	}
	GKC::CallResult PutLine(const _VariantString& str) throw()
	{
		CallResult cr(PutString(str));
		if( cr.IsSucceeded() )
			cr = PutNewLine();
		return cr;
	}

// _ITextUtility methods
	virtual void SetStream(const _ShareCom<_IByteStream>& sp) throw()
	{
		assert( !sp.IsBlockNull() );
		if( m_spStream != sp ) {
			do_outstanding_work();
			//assignment
			m_spStream = sp;
			//base
			set_stream(RefPtr<_IByteStream>(_ShareComHelper::GetInternalPointer(sp)));
		}
	}
	virtual void SetSeparatorSetA(const _ShareArray<GKC::CharA>& arr) throw()
	{
		m_separator_set_a = arr;
		set_separator_set_a(_ShareArrayHelper::To_ConstArray(arr));
	}
	virtual void SetSeparatorSetH(const _ShareArray<GKC::CharH>& arr) throw()
	{
		m_separator_set_h = arr;
		set_separator_set_h(_ShareArrayHelper::To_ConstArray(arr));
	}
	virtual void SetSeparatorSetL(const _ShareArray<GKC::CharL>& arr) throw()
	{
		m_separator_set_l = arr;
		set_separator_set_l(_ShareArrayHelper::To_ConstArray(arr));
	}

private:
	_ShareCom<_IByteStream>  m_spStream;

	//separator set
	_ShareArray<CharA>  m_separator_set_a;
	_ShareArray<CharH>  m_separator_set_h;
	_ShareArray<CharL>  m_separator_set_l;

private:
	//noncopyable
	TextStream(const TextStream&) throw();
	TextStream& operator=(const TextStream&) throw();
};

DECLARE_COM_TYPECAST(TextStream)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
