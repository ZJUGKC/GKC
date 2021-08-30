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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __TEXT_GENERATOR_H__
#define __TEXT_GENERATOR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _TextGenerator

class _TextGenerator : public _IHtmlGenerator
{
public:
// _IHtmlGenerator methods
	virtual CallResult Generate(bool bOpen, ShareCom<ICplMetaData>& spMeta, const CplMetaDataPosition& posData, ShareCom<ITextStreamRoot>& spText)
	{
		CallResult cr;
		cr = spText.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "<p>"));
		if( cr.IsFailed() )
			return cr;
		const CharA* sz = (const CharA*)spMeta.Deref().GetData(posData);
		cr = spText.Deref().PutLineA(ConstStringA(sz, calc_string_length(sz)));
		if( cr.IsFailed() )
			return cr;
		cr = spText.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "</p>"));
		return cr;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
