/*
** Copyright (c) 2017, Xin YUAN, courses of Zhejiang University
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
This file contains Won action functions.
*/

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//functions

// Do-Common action

inline CallResult _Create_WonDoCommonAction(ShareCom<IGrammarAction>& sp) throw()
{
	CallResult cr;
	_CREATE_COMPONENT_INSTANCE(WonDoCommonAction, IGrammarAction, sp, cr);
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
