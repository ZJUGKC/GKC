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
This file contains global variables for WMARK comment start action component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "wmark/action/WmarkCommentStartAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkCommentStartAction

BEGIN_COM_TYPECAST(WmarkCommentStartAction)
	COM_TYPECAST_ENTRY(ILexerAction, ILexerAction)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
