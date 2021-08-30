/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
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
This file contains component class of Do-String-Catenation action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_DO_STRING_CATENATION_ACTION_H__
#define __WON_DO_STRING_CATENATION_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonDoStringCatenationAction

class WonDoStringCatenationAction : public IGrammarAction,
									public _WonActionBase
{
public:
	WonDoStringCatenationAction() throw()
	{
	}
	~WonDoStringCatenationAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		arrSymbol[0].Deref().set_Data(arrSymbol[1].Deref().get_Buffer().Deref());
		m_data.Deref().set_Type(m_iTokenType);
		return CallResult();
	}

private:
	//noncopyable
	WonDoStringCatenationAction(const WonDoStringCatenationAction&) throw();
	WonDoStringCatenationAction& operator=(const WonDoStringCatenationAction&) throw();
};

DECLARE_COM_TYPECAST(WonDoStringCatenationAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
