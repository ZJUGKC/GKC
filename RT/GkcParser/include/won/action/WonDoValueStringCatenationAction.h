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
This file contains component class of Do-Value-String-Catenation action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_DO_VALUE_STRING_CATENATION_ACTION_H__
#define __WON_DO_VALUE_STRING_CATENATION_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////
 
// WonDoValueStringCatenationAction

class WonDoValueStringCatenationAction : public IGrammarAction,
										public _WonActionBase
{
public:
	WonDoValueStringCatenationAction() throw()
	{
	}
	~WonDoValueStringCatenationAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		try {
			cr = do_action(arrSymbol, errorArray);  //may throw
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

private:
	//do action
	CallResult do_action(ShareArray<ShareCom<IGrammarSymbolData>>& arrSymbol, ShareArray<StringS>& errorArray)
	{
		StringA str(arrSymbol[1].Deref().get_Data().Deref().ToUTF8());  //may throw
		m_data.Deref().set_String(str);
		m_data.Deref().set_Type(m_iTokenType);
		return CallResult();
	}

private:
	//noncopyable
	WonDoValueStringCatenationAction(const WonDoValueStringCatenationAction&) throw();
	WonDoValueStringCatenationAction& operator=(const WonDoValueStringCatenationAction&) throw();
};

DECLARE_COM_TYPECAST(WonDoValueStringCatenationAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
