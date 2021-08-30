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
This file contains global variables for Wmark actions.
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
#include "wmark/action/WmarkDoBlockBodyCommentAction.h"
#include "wmark/action/WmarkDoBlockBodyIndentAction.h"
#include "wmark/action/WmarkDoBlockBodyTextAction.h"

#include "wmark/WmarkAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//global variables

_ScopeShareComObject<WmarkReturnAction> g_actionWmarkReturn;
_ScopeShareComObject<WmarkCommentStartAction> g_actionWmarkCommentStart;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
