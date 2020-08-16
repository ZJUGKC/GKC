/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
This file contains component class of return action for WMARK.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WMARK_RETURN_ACTION_H__
#define __WMARK_RETURN_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkReturnAction

class WmarkReturnAction : public ILexerAction
{
public:
	WmarkReturnAction() throw()
	{
	}
	~WmarkReturnAction() throw()
	{
	}

// ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStreamRoot>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		CallResult cr;
		cr = info.AddCharEndRow(1);
		if( cr.IsFailed() )
			return cr;
		info.ResetCharEndCol();
		return cr;
	}

private:
	//noncopyable
	WmarkReturnAction(const WmarkReturnAction&) throw();
	WmarkReturnAction& operator=(const WmarkReturnAction&) throw();
};

DECLARE_COM_TYPECAST(WmarkReturnAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
