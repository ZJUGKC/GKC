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

/*
This file contains functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __COMPILE_SINGLE_FILE_H__
#define __COMPILE_SINGLE_FILE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

inline bool _Compile_One_File(ShareCom<IWlangParser>& spParser, const StringS& strSrc, const StringS& strDest) throw()
{
	CallResult cr;

	//file name
	ConsoleHelper::WriteLine(strSrc);

	//source file
	ShareCom<IByteStream> spStream;
	cr = StreamHelper::CreateFileStream(StringUtilHelper::To_ConstString(strSrc), FileOpenTypes::Read, 0, spStream);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened!")));
		return false;
	}
	ShareCom<ITextStream> spText;
	cr = StreamHelper::CreateTextStream(spText);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened as a text file!")));
		return false;
	}
	spText.Deref().SetStream(spStream);

	//parser
	spParser.Deref().SetStream(spText);

	cr = spParser.Deref().Start();
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Parser cannot start!")));
		return false;
	}

	while( true ) {
		cr = spParser.Deref().Parse();
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Parsing Failed!")));
			break;
		}
		if( cr.GetResult() == SystemCallResults::S_False )
			break;
		thread_sleep(1);
	}

	//errors
	ShareArray<StringS> arrError(spParser.Deref().get_ErrorArray());
	uintptr uCount = arrError.GetCount();
	if( uCount != 0 ) {
		auto iter(arrError.GetBegin());
		for( ; iter != arrError.GetEnd(); iter.MoveNext() ) {
			ConsoleHelper::WriteLine(iter.get_Value());
		}
	}

	return !cr.IsFailed() && uCount == 0;
}

inline int _Compile_Single_File(const StringS& strSrc, const StringS& strDest) throw()
{
	CallResult cr;

	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Initialize...")));

	//init
	ShareCom<IWlangParser> spParser;
	cr = _Create_WlangParser(spParser);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Initialization Failed!")));
		return 0;
	}

	//compile
	if( _Compile_One_File(spParser, strSrc, strDest) ) {
		//save
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
