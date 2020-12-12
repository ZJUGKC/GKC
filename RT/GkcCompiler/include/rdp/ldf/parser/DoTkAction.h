/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
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
Tk-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __DO_TK_ACTION_H__
#define __DO_TK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DoTkAction

class DoTkAction : public _IRdParserAction,
				public _ParserDataActionBase
{
public:
	DoTkAction() noexcept
	{
	}

// _IRdParserAction methods
	virtual GKC::CallResult DoAction(const _LexerTokenInfo& info, GKC::ShareArray<GKC::StringS>& errorArray) noexcept
	{
		CallResult cr;
		try {
			cr = do_action(info, errorArray);
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
	CallResult do_action(const _LexerTokenInfo& info, ShareArray<StringS>& errorArray)
	{
		_LexerTokenString strTmp;
		info.get_Buffer().CloneTo(strTmp);  //may throw
		StringA str(strTmp.ToUTF8());  //may throw
		//find
		uint uID;
		if ( !m_refData.Deref().spTables.Deref().GetTokenID(StringUtilHelper::To_ConstString(str), uID) )
			return CallResult(SystemCallResults::Unexpected);
		//add
		RULEELEMENT re = { uID, LA_NULL };
		m_refData.Deref().arr.Add(re);  //may throw
		return CallResult();
	}
};

DECLARE_COM_TYPECAST(DoTkAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
