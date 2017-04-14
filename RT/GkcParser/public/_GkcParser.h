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

//------------------------------------------------------------------------------
// Won Parser

#pragma pack(push, 1)

// _IWonUtility

class NOVTABLE _IWonUtility
{
public:
	virtual GKC::CallResult Initialize() throw() = 0;
};

DECLARE_GUID(GUID__IWonUtility)

// token types for WON

BEGIN_ENUM(_WonTokenTypes)
	ENUM_VALUE_ENTRY(Error, -1)
	ENUM_VALUE_ENTRY(Unknown, 0)
	ENUM_VALUE_ENTRY(Unexpected, 1)
	ENUM_VALUE_ENTRY(ValueString, 2)
	ENUM_VALUE_ENTRY(ValueNumberInteger, 3)
	ENUM_VALUE_ENTRY(ValueNumberFloat, 4)
	ENUM_VALUE_ENTRY(ValueNumberHexadecimal, 5)
	ENUM_VALUE_ENTRY(ValueBooleanTrue, 6)
	ENUM_VALUE_ENTRY(ValueBooleanFalse, 7)
	ENUM_VALUE_ENTRY(ValueNull, 8)
	ENUM_VALUE_ENTRY(BeginObject, 9)
	ENUM_VALUE_ENTRY(EndObject, 10)
	ENUM_VALUE_ENTRY(Key, 11)
	ENUM_VALUE_ENTRY(BeginArray, 12)
	ENUM_VALUE_ENTRY(EndArray, 13)
	ENUM_VALUE_ENTRY(Max, EndArray)
	ENUM_VALUE_ENTRY(ValueMin, ValueString)
	ENUM_VALUE_ENTRY(ValueMax, ValueNull)
END_ENUM()

// _IWonParser

class NOVTABLE _IWonParser
{
public:
	virtual GKC::CallResult Initialize(const GKC::ShareCom<_IWonUtility>& sp) throw() = 0;
	virtual void SetInput(const GKC::ShareCom<GKC::ITextStream>& sp) throw() = 0;
	virtual GKC::CallResult Start() throw() = 0;
	// return value : SystemCallResults::OK, the call is successful.
	//                SystemCallResults::S_False, it reaches the end of stream.
	//                otherwise, this call is failed.
	virtual GKC::CallResult GetNextToken(int& iType, GKC::StringA& str) throw() = 0;
};

DECLARE_GUID(GUID__IWonParser)

#pragma pack(pop)

//functions

SA_FUNCTION void _WonUtility_Create(GKC::ShareCom<_IWonUtility>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _WonParser_Create(GKC::ShareCom<_IWonParser>& sp, GKC::CallResult& cr) throw();

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_PARSER_H__
////////////////////////////////////////////////////////////////////////////////
