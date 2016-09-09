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
#ifndef __LEX_DEF_H__
#define __LEX_DEF_H__
////////////////////////////////////////////////////////////////////////////////

// _I_IdTokenMacroAction_Utility

class NOVTABLE _I_IdTokenMacroAction_Utility
{
public:
	// uID : the input value is considered as the minimum value.
	virtual void SetOutput(const GKC::RefPtr<GKC::TokenTable>& table, const GKC::ShareArray<GKC::StringA>& arr, const GKC::RefPtr<uint>& uID) throw() = 0;
};

DECLARE_GUID(GUID__I_IdTokenMacroAction_Utility)

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
