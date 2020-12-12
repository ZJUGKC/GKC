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

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RIdTable.h"
#include "rdp/base/_RScanner.h"
#include "rdp/base/_RTdTable.h"
#include "rdp/base/_RParser.h"

#include "rdp/ldf/token/TokenDef.h"

#include "rdp/ldf/parser/ParserDef.h"
#include "rdp/ldf/parser/DoRuleItemEndAction.h"
#include "rdp/ldf/parser/DoNtAction.h"
#include "rdp/ldf/parser/DoVertAction.h"
#include "rdp/ldf/parser/DoActAction.h"
#include "rdp/ldf/parser/DoEpsAction.h"
#include "rdp/ldf/parser/DoTkAction.h"

#include "rdp/ldf/ParserAction.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// global variables

const RULEELEMENT g_Rules[] = {
//==============================================================================
//program : TK_SEP rule_item
{ NT_program, LA_NULL }, { TK_SEP, LA_NULL }, { NT_rule_item, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//rule_item : TK_NT TK_COLON rule_right rule_right_tail TK_SEMI rule_item
{ NT_rule_item, LA_NULL }, { TK_NT, ACT_P_NT }, { TK_COLON, LA_NULL }, { NT_rule_right, LA_NULL }, { NT_rule_right_tail, LA_NULL }, { TK_SEMI, ACT_rule_item_end }, { NT_rule_item, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//rule_item : TK_EPSILON
{ NT_rule_item, ACT_rule_item_end }, { TK_EPSILON, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//rule_right_tail : TK_VERT rule_right rule_right_tail
{ NT_rule_right_tail, LA_NULL }, { TK_VERT, ACT_P_VERT }, { NT_rule_right, LA_NULL }, { NT_rule_right_tail, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//rule_right_tail : TK_EPSILON
{ NT_rule_right_tail, LA_NULL }, { TK_EPSILON, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//rule_right : act_item item_list
{ NT_rule_right, LA_NULL }, { NT_act_item, LA_NULL }, { NT_item_list, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//rule_right : item_list
{ NT_rule_right, LA_NULL }, { NT_item_list, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//item_list : item_one item_list_tail
{ NT_item_list, LA_NULL }, { NT_item_one, LA_NULL }, { NT_item_list_tail, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//item_list : TK_EPS
{ NT_item_list, LA_NULL }, { TK_EPS, ACT_P_EPS }, { CPL_TK_NULL, LA_NULL },
//item_list_tail : item_one item_list_tail
{ NT_item_list_tail, LA_NULL }, { NT_item_one, LA_NULL }, { NT_item_list_tail, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//item_list_tail : TK_EPSILON
{ NT_item_list_tail, LA_NULL }, { TK_EPSILON, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//item_one : TK_NT
{ NT_item_one, LA_NULL }, { TK_NT, ACT_P_NT }, { CPL_TK_NULL, LA_NULL },
//item_one : TK_TK item_one_tail
{ NT_item_one, LA_NULL }, { TK_TK, ACT_P_TK }, { NT_item_one_tail, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//item_one_tail : act_item
{ NT_item_one_tail, LA_NULL }, { NT_act_item, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//item_one_tail : TK_EPSILON
{ NT_item_one_tail, LA_NULL }, { TK_EPSILON, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//act_item : TK_LCURLY TK_ACT TK_RCURLY
{ NT_act_item, LA_NULL }, { TK_LCURLY, LA_NULL }, { TK_ACT, ACT_P_ACT }, { TK_RCURLY, LA_NULL }, { CPL_TK_NULL, LA_NULL },
//==============================================================================
//end
{ CPL_TK_NULL, LA_NULL }
};

// functions

bool _Parser_InitTable(_RTdTable& rt)
{
	return rt.Generate(g_Rules, TK_MAX);  //may throw
}

void _Parser_InitActions(_RParser& rp, _Parser_ActionSet& pas, _Parser_ActionData& data)
{
	CallResult cr;

	ShareCom<_IRdParserAction> sp;
	ShareCom<_I_Parser_DataAction_Utility> spU;

	rp.ClearActions();
	//Do-Rule-Item-End
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoRuleItemEndAction, _IRdParserAction, pas.actionDoRuleItemEnd, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Parser_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetData(RefPtr<_Parser_ActionData>(data));
	rp.AddAction(ACT_rule_item_end, sp);  //may throw
	//Do-Nt
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoNtAction, _IRdParserAction, pas.actionDoNt, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Parser_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetData(RefPtr<_Parser_ActionData>(data));
	rp.AddAction(ACT_P_NT, sp);  //may throw
	//Do-Vert
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoVertAction, _IRdParserAction, pas.actionDoVert, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Parser_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetData(RefPtr<_Parser_ActionData>(data));
	rp.AddAction(ACT_P_VERT, sp);  //may throw
	//Do-Act
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoActAction, _IRdParserAction, pas.actionDoAct, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Parser_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetData(RefPtr<_Parser_ActionData>(data));
	rp.AddAction(ACT_P_ACT, sp);  //may throw
	//Do-Eps
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoEpsAction, _IRdParserAction, pas.actionDoEps, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Parser_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetData(RefPtr<_Parser_ActionData>(data));
	rp.AddAction(ACT_P_EPS, sp);  //may throw
	//Do-Tk
	_CREATE_SCOPE_COMPONENT_INSTANCE(DoTkAction, _IRdParserAction, pas.actionDoTk, sp, cr);
	assert( cr.IsSucceeded() );
	_COMPONENT_INSTANCE_INTERFACE(_IRdParserAction, _I_Parser_DataAction_Utility, sp, spU, cr);
	assert( cr.IsSucceeded() );
	spU.Deref().SetData(RefPtr<_Parser_ActionData>(data));
	rp.AddAction(ACT_P_TK, sp);  //may throw
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
