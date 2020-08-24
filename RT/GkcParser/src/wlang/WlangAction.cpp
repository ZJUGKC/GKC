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
This file contains global variables for WLang actions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcParser.h"

#include "wlang/base/WlangDef.h"

#include "wlang/action/WlangGrammarError.h"
#include "wlang/action/WlangGrammarAccepted.h"
#include "wlang/action/WlangDoNsBodyAction.h"
#include "wlang/action/WlangDoBodySemiAction.h"

#include "wlang/WlangAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//global variables

_ScopeShareComObject<WlangGrammarError> g_actionWlangGrammarError;

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
