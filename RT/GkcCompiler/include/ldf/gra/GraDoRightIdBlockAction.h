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
This file contains component class of Do-Right-Id-Block action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DO_RIGHT_ID_BLOCK_ACTION_H__
#define __GRA_DO_RIGHT_ID_BLOCK_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GraDoRightIdBlockAction

class GraDoRightIdBlockAction : public _IGrammarAction,
								public _GraDataActionBase
{
public:
	GraDoRightIdBlockAction() throw()
	{
	}
	~GraDoRightIdBlockAction() throw()
	{
	}

// _IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		try {
			do_action(arrSymbol);  //may throw
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
	void do_action(ShareArray<ShareCom<_IGrammarSymbolData>>& arrSymbol)
	{
		//action
		StringA str(arrSymbol[3].get_Value().Deref().get_Buffer().Deref().ToUTF8());  //may throw
		uint uID = m_data.Deref().AddToReductionTable(StringUtilHelper::To_ConstString(str));  //may throw
		uID;
		assert( uID == m_data.Deref().GetRuleSet().GetCount() - 1 );
	}

private:
	//noncopyable
	GraDoRightIdBlockAction(const GraDoRightIdBlockAction&) throw();
	GraDoRightIdBlockAction& operator=(const GraDoRightIdBlockAction&) throw();
};

DECLARE_COM_TYPECAST(GraDoRightIdBlockAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
