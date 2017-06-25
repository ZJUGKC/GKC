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

// IWonUtility
typedef _IWonUtility  IWonUtility;

#define GUID_IWonUtility  GUID__IWonUtility

// WonTokenTypes
typedef _WonTokenTypes  WonTokenTypes;

// IWonParser
typedef _IWonParser  IWonParser;

#define GUID_IWonParser  GUID__IWonParser

// IWmarkUtility
typedef _IWmarkUtility  IWmarkUtility;

#define GUID_IWmarkUtility  GUID__IWmarkUtility

// IWmarkParser
typedef _IWmarkParser  IWmarkParser;

#define GUID_IWmarkParser  GUID__IWmarkParser

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
	static CallResult CreateWonUtility(ShareCom<IWonUtility>& sp) throw()
	{
		CallResult cr;
		::_WonUtility_Create(sp, cr);
		return cr;
	}
	static CallResult CreateWonParser(ShareCom<IWonParser>& sp) throw()
	{
		CallResult cr;
		::_WonParser_Create(sp, cr);
		return cr;
	}
	static CallResult CreateWmarkUtility(ShareCom<IWmarkUtility>& sp) throw()
	{
		CallResult cr;
		::_WmarkUtility_Create(sp, cr);
		return cr;
	}
	static CallResult CreateWmarkParser(ShareCom<IWmarkParser>& sp) throw()
	{
		CallResult cr;
		::_WmarkParser_Create(sp, cr);
		return cr;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PARSER_H__
////////////////////////////////////////////////////////////////////////////////
