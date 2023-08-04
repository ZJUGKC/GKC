/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
This file contains component class of unique text stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TEXT_STREAM_U_H__
#define __TEXT_STREAM_U_H__
////////////////////////////////////////////////////////////////////////////////

#include "_TxtStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// TextStreamU

class TextStreamU : public _ITextStreamRootImpl,
					public _ITextStreamStringU,
					public _ITextUtilityU
{
public:
	TextStreamU() noexcept
	{
	}
	TextStreamU(const TextStreamU&) = delete;
	TextStreamU& operator=(const TextStreamU&) = delete;
	~TextStreamU() noexcept
	{
	}

// _ITextStreamStringU methods
	virtual GKC::CallResult GetAllStringA(_UniqueStringA& str) noexcept
	{
		return get_string<_UniqueStringA, _UniqueArrayHelper>(MODE_ALL, m_carrSeparatorSetA, str);
	}
	virtual GKC::CallResult GetAllStringH(_UniqueStringH& str) noexcept
	{
		return get_string<_UniqueStringH, _UniqueArrayHelper>(MODE_ALL, m_carrSeparatorSetH, str);
	}
	virtual GKC::CallResult GetAllStringL(_UniqueStringL& str) noexcept
	{
		return get_string<_UniqueStringL, _UniqueArrayHelper>(MODE_ALL, m_carrSeparatorSetL, str);
	}
	virtual GKC::CallResult GetStringA(_UniqueStringA& str) noexcept
	{
		return get_string<_UniqueStringA, _UniqueArrayHelper>(MODE_SEPARATOR, m_carrSeparatorSetA, str);
	}
	virtual GKC::CallResult GetStringH(_UniqueStringH& str) noexcept
	{
		return get_string<_UniqueStringH, _UniqueArrayHelper>(MODE_SEPARATOR, m_carrSeparatorSetH, str);
	}
	virtual GKC::CallResult GetStringL(_UniqueStringL& str) noexcept
	{
		return get_string<_UniqueStringL, _UniqueArrayHelper>(MODE_SEPARATOR, m_carrSeparatorSetL, str);
	}
	virtual GKC::CallResult GetLineA(_UniqueStringA& str) noexcept
	{
		return get_string<_UniqueStringA, _UniqueArrayHelper>(MODE_CRLF, m_carrSeparatorSetA, str);
	}
	virtual GKC::CallResult GetLineH(_UniqueStringH& str) noexcept
	{
		return get_string<_UniqueStringH, _UniqueArrayHelper>(MODE_CRLF, m_carrSeparatorSetH, str);
	}
	virtual GKC::CallResult GetLineL(_UniqueStringL& str) noexcept
	{
		return get_string<_UniqueStringL, _UniqueArrayHelper>(MODE_CRLF, m_carrSeparatorSetL, str);
	}

// _ITextUtilityU methods
	virtual void AttachStream(const GKC::RefPtr<_IByteStream>& rp) noexcept
	{
		assert( !rp.IsNull() );
		if( m_refStream != rp ) {
			//assignment
			m_refStream = rp;
		}
	}
	virtual GKC::RefPtr<_IByteStream> DetachStream() noexcept
	{
		do_outstanding_work();
		return GKC::RefPtr<_IByteStream>(rv_forward(m_refStream));
	}
	virtual void SetSeparatorSetA(_UniqueArray<GKC::CharA>&& arr) noexcept
	{
		m_separator_set_a = rv_forward(arr);
		set_separator_set_a(_UniqueArrayHelper::To_ConstArray(m_separator_set_a));
	}
	virtual void SetSeparatorSetH(_UniqueArray<GKC::CharH>&& arr) noexcept
	{
		m_separator_set_h = rv_forward(arr);
		set_separator_set_h(_UniqueArrayHelper::To_ConstArray(m_separator_set_h));
	}
	virtual void SetSeparatorSetL(_UniqueArray<GKC::CharL>&& arr) noexcept
	{
		m_separator_set_l = rv_forward(arr);
		set_separator_set_l(_UniqueArrayHelper::To_ConstArray(m_separator_set_l));
	}

private:
	//separator set
	_UniqueArray<CharA>  m_separator_set_a;
	_UniqueArray<CharH>  m_separator_set_h;
	_UniqueArray<CharL>  m_separator_set_l;
};

DECLARE_COM_TYPECAST(TextStreamU)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
