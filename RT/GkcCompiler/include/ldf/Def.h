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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __DEF_H__
#define __DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _I_IdTokenMacroAction_Utility

class NOVTABLE _I_IdTokenMacroAction_Utility
{
public:
	// uID : the input value is considered as the minimum value.
	virtual void SetOutput(const RefPtr<TokenTable>& table, const ShareArray<StringA>& arr, const RefPtr<uint>& uID) throw() = 0;
};

DECLARE_GUID(GUID__I_IdTokenMacroAction_Utility)

// _I_RegexCharSymbolData_Utility

class NOVTABLE _I_RegexCharSymbolData_Utility
{
public:
	virtual void GetCharRange(uint& uLow, uint& uHigh) throw() = 0;
	virtual void SetCharRange(uint uLow, uint uHigh) throw() = 0;
};

DECLARE_GUID(GUID__I_RegexCharSymbolData_Utility)

//regex operators

enum {
	REGEX_OP_LINK,
	REGEX_OP_OR,
	REGEX_OP_QUESTION,
	REGEX_OP_STAR,
	REGEX_OP_PLUS,
	REGEX_OP_MAX
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__DEF_H__
////////////////////////////////////////////////////////////////////////////////
