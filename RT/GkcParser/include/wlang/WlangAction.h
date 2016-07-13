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

inline CallResult _Create_WlangGrammarError(ShareCom<IGrammarError>& sp) throw()
{
	CallResult cr;
	ShareCom<IGrammarError> spError;
	_CREATE_COMPONENT_INSTANCE(WlangGrammarError, IGrammarError, spError, cr);
	if( cr.IsFailed() )
		return cr;
	sp = spError;
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
