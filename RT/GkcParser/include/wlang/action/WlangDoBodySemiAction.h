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
This file contains component class of Do-Body-Semi action.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __WLANG_DO_BODY_SEMI_ACTION_H__
#define __WLANG_DO_BODY_SEMI_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WlangDoBodySemiAction

class WlangDoBodySemiAction : public IGrammarAction
{
public:
	WlangDoBodySemiAction() throw()
	{
	}
	~WlangDoBodySemiAction() throw()
	{
	}

// IGrammarAction methods
	virtual GKC::CallResult DoAction(INOUT GKC::ShareArray<GKC::ShareCom<_IGrammarSymbolData>>& arrSymbol, INOUT GKC::ShareArray<GKC::StringS>& errorArray) throw()
	{
		CallResult cr;
		return cr;
	}

private:
	//noncopyable
	WlangDoBodySemiAction(const WlangDoBodySemiAction&) throw();
	WlangDoBodySemiAction& operator=(const WlangDoBodySemiAction&) throw();
};

DECLARE_COM_TYPECAST(WlangDoBodySemiAction)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
