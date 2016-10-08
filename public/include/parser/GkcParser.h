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
This file contains GkcParser helper classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_PARSER_H__
#define __GKC_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_SYS_H__
	#error GkcParser.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////

#include "_GkcParser.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// IWlangUtility
typedef _IWlangUtility  IWlangUtility;

#define GUID_IWlangUtility  GUID__IWlangUtility

// IWlangParser
typedef _IWlangParser  IWlangParser;

#define GUID_IWlangParser  GUID__IWlangParser

// ParserHelper

class ParserHelper
{
public:
	static CallResult CreateWlangUtility(ShareCom<IWlangUtility>& sp) throw()
	{
		CallResult cr;
		::_WlangUtility_Create(sp, cr);
		return cr;
	}
	static CallResult CreateWlangParser(ShareCom<IWlangParser>& sp) throw()
	{
		CallResult cr;
		::_WlangParser_Create(sp, cr);
		return cr;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PARSER_H__
////////////////////////////////////////////////////////////////////////////////
