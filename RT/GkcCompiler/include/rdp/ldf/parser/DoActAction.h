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
Act-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __DO_ACT_ACTION_H__
#define __DO_ACT_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DoActAction

class DoActAction : public _IRdParserAction,
					public _ParserDataActionBase
{
public:
	DoActAction() noexcept
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
		//insert
		uint uActionID = m_refData.Deref().act_table.Deref().Insert(str);  //may throw
		//set action
		uintptr uCount = m_refData.Deref().arr.GetCount();
		assert( uCount > 0 );
		m_refData.Deref().arr[uCount - 1].get_Value().uAction = uActionID;
		return CallResult();
	}
};

DECLARE_COM_TYPECAST(DoActAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
