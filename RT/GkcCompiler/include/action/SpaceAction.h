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
This file contains component class of space action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SPACE_ACTION_H__
#define __SPACE_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// SpaceAction

class SpaceAction : public _ILexerAction
{
public:
	SpaceAction() throw()
	{
	}
	~SpaceAction() throw()
	{
	}

// _ILexerAction methods
	virtual GKC::CallResult DoAction(GKC::ShareCom<GKC::ITextStreamRoot>& stream, _LexerTokenInfo& info, GKC::ConstStringA& strToken, bool& bTokenChanged) throw()
	{
		info.set_ID(CPL_TK_NULL);
		return CallResult();
	}

private:
	//noncopyable
	SpaceAction(const SpaceAction&) throw();
	SpaceAction& operator=(const SpaceAction&) throw();
};

DECLARE_COM_TYPECAST(SpaceAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
