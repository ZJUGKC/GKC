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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PARSER_DEF_H__
#define __PARSER_DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _ParserInputHelper

class _ParserInputHelper
{
public:
	//load text
	static CallResult LoadTextFromStream(const ShareCom<IByteStream>& spStream, bool bCheckUTF8, ShareCom<ITextStream>& sp) throw()
	{
		CallResult cr;
		//text
		ShareCom<ITextStream> spText;
		cr = StreamHelper::CreateTextStream(spText);
		if( cr.IsFailed() )
			return cr;
		spText.Deref().SetStream(spStream);
		//BOM
		if( bCheckUTF8 ) {
			//UTF8
			int iBOMType;
			cr = spText.Deref().CheckBOM(iBOMType);
			if( cr.IsFailed() )
				return cr;
			if( iBOMType != BOMTypes::UTF8 ) {
				cr.SetResult(SystemCallResults::Fail);
				return cr;
			}
		}
		sp = spText;
		return cr;
	}
	static CallResult LoadTextFromFile(const ConstStringS& str, bool bCheckUTF8, ShareCom<ITextStream>& sp) throw()
	{
		CallResult cr;
		//stream
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateFileStream(str, FileOpenTypes::Read, 0, spStream);
		if( cr.IsFailed() )
			return cr;
		//text
		cr = LoadTextFromStream(spStream, bCheckUTF8, sp);
		return cr;
	}
	static CallResult LoadTextFromBuffer(const ConstStringA& str, bool bCheckUTF8, ShareCom<ITextStream>& sp) throw()
	{
		CallResult cr;
		//stream
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateBufferStream((uintptr)ConstArrayHelper::GetInternalPointer(str), str.GetCount() * sizeof(CharA), spStream);
		if( cr.IsFailed() )
			return cr;
		//text
		cr = LoadTextFromStream(spStream, bCheckUTF8, sp);
		return cr;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
