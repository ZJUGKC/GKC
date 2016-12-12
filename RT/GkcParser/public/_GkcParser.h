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
This file contains GkcParser functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SA_GKC_PARSER_H__
#define __SA_GKC_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Wlang Parser

#pragma pack(push, 1)

// _IWlangUtility

class NOVTABLE _IWlangUtility
{
public:
	virtual GKC::CallResult Initialize() throw() = 0;
};

DECLARE_GUID(GUID__IWlangUtility)

// _IWlangParser

class NOVTABLE _IWlangParser
{
public:
	virtual GKC::CallResult Initialize(const uint& uMaxErrorNumber, const GKC::ShareCom<_IWlangUtility>& sp) throw() = 0;
	virtual void SetInput(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
	virtual void SetOutput(const GKC::ShareCom<GKC::ICplMetaData>& sp) throw() = 0;
	virtual GKC::CallResult Start() throw() = 0;
	// return value : SystemCallResults::OK, the call is successful.
	//                SystemCallResults::S_False, it reaches the end of stream.
	//                otherwise, this call is failed.
	virtual GKC::CallResult Parse() throw() = 0;
	virtual const GKC::ShareArray<GKC::StringS> get_ErrorArray() throw() = 0;
};

DECLARE_GUID(GUID__IWlangParser)

#pragma pack(pop)

//functions

SA_FUNCTION void _WlangUtility_Create(GKC::ShareCom<_IWlangUtility>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _WlangParser_Create(GKC::ShareCom<_IWlangParser>& sp, GKC::CallResult& cr) throw();

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_PARSER_H__
////////////////////////////////////////////////////////////////////////////////
