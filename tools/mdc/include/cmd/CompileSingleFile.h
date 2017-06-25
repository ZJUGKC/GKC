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
This file contains functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __COMPILE_SINGLE_FILE_H__
#define __COMPILE_SINGLE_FILE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// compile single file

inline bool _Compile_Single_File(const StringS& strSrc, const StringS& strDest) throw()
{
	CallResult cr;

	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Initialize...")));

	//init
	ShareCom<IWmarkParser> spParser;
	cr = _Create_WmarkParser(spParser);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Initialization Failed!")));
		return false;
	}

	//compile
	uintptr uCount = _Compile_One_File(spParser, strSrc, strDest);

	//result
	_Print_Compiled_Result(uCount);

	return uCount == 0;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
