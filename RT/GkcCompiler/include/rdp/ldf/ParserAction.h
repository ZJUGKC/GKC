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
Parser Actions
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PARSER_ACTION_H__
#define __PARSER_ACTION_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

struct _Parser_ActionSet
{
	_ScopeShareComObject<DoRuleItemEndAction> actionDoRuleItemEnd;
	_ScopeShareComObject<DoNtAction> actionDoNt;
	_ScopeShareComObject<DoVertAction> actionDoVert;
	_ScopeShareComObject<DoActAction> actionDoAct;
	_ScopeShareComObject<DoEpsAction> actionDoEps;
	_ScopeShareComObject<DoTkAction> actionDoTk;
};

//functions

bool _Parser_InitTable(_RTdTable& rt);

void _Parser_InitActions(_RParser& rp, _Parser_ActionSet& pas, _Parser_ActionData& data);

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
