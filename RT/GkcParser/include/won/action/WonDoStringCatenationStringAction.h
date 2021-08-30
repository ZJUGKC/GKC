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
This file contains component class of Do-String-Catenation-String action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_DO_STRING_CATENATION_STRING_ACTION_H__
#define __WON_DO_STRING_CATENATION_STRING_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonDoStringCatenationStringAction

class WonDoStringCatenationStringAction : public IGrammarAction,
										public _WonActionBase
{
public:
	WonDoStringCatenationStringAction() throw()
	{
	}
	~WonDoStringCatenationStringAction() throw()
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
		LexerTokenString& lstr(arrSymbol[0].Deref().get_Data().Deref());
		lstr = arrSymbol[1].Deref().get_Data().Deref();
		uintptr uLen = lstr.GetLength();
		lstr.Insert(uLen, arrSymbol[2].Deref().get_Buffer().Deref());  //may throw
		// remove ""
		lstr.Delete(uLen - 1, 2);
		m_data.Deref().set_Type(m_iTokenType);
		return CallResult();
	}

private:
	//noncopyable
	WonDoStringCatenationStringAction(const WonDoStringCatenationStringAction&) throw();
	WonDoStringCatenationStringAction& operator=(const WonDoStringCatenationStringAction&) throw();
};

DECLARE_COM_TYPECAST(WonDoStringCatenationStringAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
