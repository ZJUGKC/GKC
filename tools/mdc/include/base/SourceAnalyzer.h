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

/*
This file contains classes for source analyzer.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SOURCE_ANALYZER_H__
#define __SOURCE_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Create_WmarkParser

inline CallResult _Create_WmarkParser(ShareCom<IWmarkParser>& sp) throw()
{
	const uint c_uMaxErrorNumber = 100;
	CallResult cr;
	//utility
	ShareCom<IWmarkUtility> spU;
	cr = ParserHelper::CreateWmarkUtility(spU);
	if( cr.IsFailed() )
		return cr;
	cr = spU.Deref().Initialize();
	if( cr.IsFailed() )
		return cr;
	//parser
	ShareCom<IWmarkParser> spS;
	cr = ParserHelper::CreateWmarkParser(spS);
	if( cr.IsFailed() )
		return cr;
	//init
	cr = spS.Deref().Initialize(c_uMaxErrorNumber, spU);
	if( cr.IsFailed() )
		return cr;
	sp = spS;
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif // __SOURCE_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////
