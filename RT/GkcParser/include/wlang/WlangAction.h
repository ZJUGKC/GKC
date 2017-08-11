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
This file contains WLang action functions.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//functions

// Do-Ns-Body action

inline CallResult _Create_WlangDoNsBodyAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WlangDoNsBodyAction, IGrammarAction, sp, cr);
	return cr;
}

// Do-Body-Semi action

inline CallResult _Create_WlangDoBodySemiAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WlangDoBodySemiAction, IGrammarAction, sp, cr);
	return cr;
}

// grammar error

inline CallResult _Create_WlangGrammarError(ShareCom<IGrammarError>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WlangGrammarError, IGrammarError, sp, cr);
	return cr;
}

// grammar accepted

inline CallResult _Create_WlangGrammarAccepted(ShareCom<IGrammarAccepted>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WlangGrammarAccepted, IGrammarAccepted, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
