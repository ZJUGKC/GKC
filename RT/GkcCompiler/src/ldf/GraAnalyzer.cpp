/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
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
This file contains the functions for gra file.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/FsaTable.h"
#include "base/Lexer.h"
#include "base/Pda.h"
#include "base/PdaTable.h"
#include "base/Grammar.h"

#include "ldf/base/LdfFsa.h"
#include "ldf/base/LdfLexer.h"
#include "ldf/base/LdfGrammar.h"

#include "ldf/gra/LdfGraPda.h"
#include "ldf/gra/GraDef.h"
#include "ldf/gra/GraTokenSymbolData.h"
#include "ldf/gra/GraTokenSymbolData_Factory.h"
#include "ldf/gra/GraPositionSymbolData.h"
#include "ldf/gra/GraPositionSymbolData_Factory.h"
#include "ldf/gra/GraDoRuleRightBlockAction.h"
#include "ldf/gra/GraDoRightBlockAction.h"
#include "ldf/gra/GraDoRightAction.h"
#include "ldf/gra/GraDoRightIdBlockAction.h"
#include "ldf/gra/GraDoIdBlockAction.h"
#include "ldf/gra/GraDoIdAction.h"
#include "ldf/gra/GraDoIdTokenAction.h"
#include "ldf/gra/GraDoIdMacroAction.h"

#include "ldf/GraAction.h"
#include "ldf/GraDotAlg.h"
#include "ldf/GraAnalyzer.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//for gra file

CallResult _Generate_Grammar_Tables(const ShareCom<ITextStreamRoot>& sp, const TokenTable& terminalTable,
									TokenTable& nonterminalTable, TokenTable& reductionTable, PdaTable& table)
{
	CallResult cr;

	//lexer
	_LdfLexerParser lexer;
	cr = lexer.Initialize();  //may throw
	if( cr.IsFailed() )
		return cr;
	//stream
	int iCharType;
	lexer.SetStream(sp, iCharType);

	//grammar
	_LdfGrammarParser grammar;
	grammar.SetLexerParser(lexer.GetLexerParser());
	//nonterminal
	TokenTable nt_table;
	uint uID = 101;
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "gra_def"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_block"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_right_block"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_right"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "id_block"), uID ++);  //may throw
	nt_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "id"), uID ++);  //may throw
	nt_table.Finish();  //may throw
	grammar.SetNonterminalTable(RefPtr<TokenTable>(nt_table));
	//reduction action table for gra file
	TokenTable ra_table;
	uID = 1;
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_ref"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_block_rule"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_right_block"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_right_block"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_right"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_right_id_block"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_block"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"), uID ++);  //may throw
	ra_table.InsertToken(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"), uID ++);  //may throw
	ra_table.Finish();  //may throw
	grammar.SetReductionActionTable(RefPtr<TokenTable>(ra_table));
	//pda table
	grammar.SetPdaTable(LdfGraPdaTraits::GetTable());
	//factory
	{
		ShareCom<IComFactory> spCF;
		_BasicSymbolDataFactory_Create(spCF, cr);
		if( cr.IsFailed() )
			return cr;
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEP"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_MACRO"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_COLON"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_SEMI"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_VERT"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_LCURLY"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_RCURLY"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_ACTION"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "TK_TOKEN"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "id_block"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_right"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_block"), spCF);  //may throw
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "gra_def"), spCF);  //may throw
		cr = _Create_GraTokenSymbolDataFactory(spCF);
		if( cr.IsFailed() )
			return cr;
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "id"), spCF);  //may throw
		cr = _Create_GraPositionSymbolDataFactory(spCF);
		if( cr.IsFailed() )
			return cr;
		grammar.SetFactory(DECLARE_TEMP_CONST_STRING(ConstStringA, "rule_right_block"), spCF);  //may throw
	} //end block
	//data
	_Gra_Data data(terminalTable, nonterminalTable, reductionTable);
	data.Init();  //may throw
	_Gra_ActionSet gra_action_set;  //action set
	//actions
	{
		ShareCom<_IGrammarAction> spAction;
		ShareCom<_I_GraDataAction_Utility> spU;
		//DoRuleRightBlock
		cr = _Create_GraDoRuleRightBlockAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_rule_right_block"), spAction);  //may throw
		//DoRightBlock
		cr = _Create_GraDoRightBlockAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_right_block"), spAction);  //may throw
		//DoRight
		cr = _Create_GraDoRightAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_right"), spAction);  //may throw
		//DoRightIdBlock
		cr = _Create_GraDoRightIdBlockAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_right_id_block"), spAction);  //may throw
		//DoIdBlock
		cr = _Create_GraDoIdBlockAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_block"), spAction);  //may throw
		//DoId
		cr = _Create_GraDoIdAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id"), spAction);  //may throw
		//DoIdToken
		cr = _Create_GraDoIdTokenAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_token"), spAction);  //may throw
		//DoIdMacro
		cr = _Create_GraDoIdMacroAction(gra_action_set, spAction);
		if( cr.IsFailed() )
			return cr;
		_COMPONENT_INSTANCE_INTERFACE(_IGrammarAction, _I_GraDataAction_Utility, spAction, spU, cr);
		assert( cr.IsSucceeded() );
		spU.Deref().SetOutput(RefPtr<_Gra_Data>(data));
		grammar.SetAction(DECLARE_TEMP_CONST_STRING(ConstStringA, "do_id_macro"), spAction);  //may throw
	} //end block

	//execute
	cr = grammar.Execute();  //may throw
	if( cr.IsFailed() )
		return cr;

	//finish
	data.Finish();  //may throw

	//check overflow
	{
		assert( nonterminalTable.GetMinID() > terminalTable.GetMaxID()
				&& reductionTable.GetMinID() == CPL_TK_FIRST
				&& reductionTable.GetMaxID() < (uint)(Limits<int>::Max - 1) );  //with last NULL item
		_GraRuleSet& rs = data.GetRuleSet();
		assert( (uintptr)(reductionTable.GetMaxID()) == rs.GetCount() - 1 );
		for( uintptr i = 0; i < rs.GetCount(); i ++ )
			assert( rs.GetRule(i).GetCount() <= (uintptr)(Limits<uint>::Max) );
	} //end block

	//generate
	_Gra_Generate_Tables(terminalTable, nonterminalTable, data.GetRuleSet(), table);  //may throw

	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
