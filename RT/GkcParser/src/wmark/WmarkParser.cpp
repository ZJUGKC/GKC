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
This file contains global variables for Wmark parser component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "wmark/action/WmarkReturnAction.h"
#include "wmark/action/WmarkCommentStartAction.h"
#include "wmark/action/WmarkGrammarAccepted.h"
#include "wmark/action/WmarkDoDocRsBlockListAction.h"
#include "wmark/action/WmarkDoBlockListListBlockAction.h"
#include "wmark/action/WmarkDoBlockBlockBodyAction.h"
#include "wmark/action/WmarkDoBlockBodyIndentAction.h"

#include "wmark/WmarkAction.h"
#include "wmark/WmarkParser.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// WmarkParser

BEGIN_COM_TYPECAST(WmarkParser)
	COM_TYPECAST_ENTRY(_IWmarkParser, _IWmarkParser)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
