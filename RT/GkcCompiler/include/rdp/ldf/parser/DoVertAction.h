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
Vert-Action
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __DO_VERT_ACTION_H__
#define __DO_VERT_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DoVertAction

class DoVertAction : public _IRdParserAction,
					public _ParserDataActionBase
{
public:
	DoVertAction() noexcept
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
		ShareArray<RULEELEMENT>& arr = m_refData.Deref().arr;
		uintptr uCount = arr.GetCount();
		assert( uCount > 0 );
		//find left part
		uintptr idx;
		for ( idx = uCount; idx > 0; idx -- ) {
			if ( arr[idx - 1].get_Value().uToken == CPL_TK_NULL )
				break;
		}
		assert( idx < uCount );
		uint uLeftPart = arr[idx].get_Value().uToken;
		//add
		RULEELEMENT re = { CPL_TK_NULL, LA_NULL };
		arr.Add(re);  //may throw
		re.uToken = uLeftPart;
		re.uAction = LA_NULL;
		arr.Add(re);  //may throw
		return CallResult();
	}
};

DECLARE_COM_TYPECAST(DoVertAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
