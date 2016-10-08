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
This file contains classes for source analyzer.
Author: Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SOURCE_ANALYZER_H__
#define __SOURCE_ANALYZER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Create_WlangParser

inline CallResult _Create_WlangParser(ShareCom<IWlangParser>& sp) throw()
{
	const uint c_uMaxErrorNumber = 400;
	CallResult cr;
	//utility
	ShareCom<IWlangUtility> spU;
	cr = ParserHelper::CreateWlangUtility(spU);
	if( cr.IsFailed() )
		return cr;
	cr = spU.Deref().Initialize();
	if( cr.IsFailed() )
		return cr;
	//parser
	ShareCom<IWlangParser> spS;
	cr = ParserHelper::CreateWlangParser(spS);
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
