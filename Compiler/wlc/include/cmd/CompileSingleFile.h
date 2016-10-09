﻿/*
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

// compile single file

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
	uintptr uCount = _Compile_One_File(spParser, strSrc, strDest);

	//result
	_Print_Compiled_Result(uCount);

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
