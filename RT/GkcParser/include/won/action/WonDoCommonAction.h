/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains component class of Do-Common action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WON_DO_COMMON_ACTION_H__
#define __WON_DO_COMMON_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WonDoCommonAction

class WonDoCommonAction : public IGrammarAction,
						public _WonActionBase
{
public:
	WonDoCommonAction() throw()
	{
	}
	~WonDoCommonAction() throw()
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
		StringA str(arrSymbol[1].get_Value().Deref().get_Buffer().Deref().ToUTF8());  //may throw
		m_data.Deref().set_String(str);
		m_data.Deref().set_Type(m_iTokenType);
		return CallResult();
	}

private:
	//noncopyable
	WonDoCommonAction(const WonDoCommonAction&) throw();
	WonDoCommonAction& operator=(const WonDoCommonAction&) throw();
};

DECLARE_COM_TYPECAST(WonDoCommonAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
