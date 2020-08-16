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
This file contains component class of return action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __RETURN_ACTION_H__
#define __RETURN_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// ReturnAction

class ReturnAction : public _ILexerAction
{
public:
	ReturnAction() throw()
	{
	}
	~ReturnAction() throw()
	{
	}

// _ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStreamRoot>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		CallResult cr;
		cr = info.AddCharEndRow(1);
		if( cr.IsFailed() )
			return cr;
		info.ResetCharEndCol();
		info.set_ID(CPL_TK_NULL);
		return cr;
	}

private:
	//noncopyable
	ReturnAction(const ReturnAction&) throw();
	ReturnAction& operator=(const ReturnAction&) throw();
};

DECLARE_COM_TYPECAST(ReturnAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
