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
This file contains PDA tables for lex file.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "base/Pda.h"
#include "base/PdaTable.h"

#include "ldf/LdfLexPda.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LdfLexPdaTraits

// state map
PDA_BEGIN_TRAITS_STATE_MAP(_LDF_lex_PdaTraits)
	//transitions
	PDA_BEGIN_STATE_TRANSITION(PDA_STATE_START)
		PDA_STATE_TRANSITION_ENTRY(101, 3)  //lex_def
		PDA_STATE_TRANSITION_ENTRY(4, 4)    //TK_SEP
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(3)
		PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, PDA_STATE_ACCEPTED)  //$
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(4)
		PDA_STATE_TRANSITION_ENTRY(7, 5)    //TK_TOKEN
		PDA_STATE_TRANSITION_ENTRY(6, 6)    //TK_MACRO
		PDA_STATE_TRANSITION_ENTRY(103, 7)  //id
		PDA_STATE_TRANSITION_ENTRY(102, 8)  //rule_block
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(5)
		PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -4)  //$
		PDA_STATE_TRANSITION_ENTRY(7, -4)    //TK_TOKEN
		PDA_STATE_TRANSITION_ENTRY(6, -4)    //TK_MACRO
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(6)
		PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -5)  //$
		PDA_STATE_TRANSITION_ENTRY(7, -5)    //TK_TOKEN
		PDA_STATE_TRANSITION_ENTRY(6, -5)    //TK_MACRO
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(7)
		PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -3)  //$
		PDA_STATE_TRANSITION_ENTRY(7, -3)    //TK_TOKEN
		PDA_STATE_TRANSITION_ENTRY(6, -3)    //TK_MACRO
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(8)
		PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -1)  //$
		PDA_STATE_TRANSITION_ENTRY(7, 5)     //TK_TOKEN
		PDA_STATE_TRANSITION_ENTRY(6, 6)     //TK_MACRO
		PDA_STATE_TRANSITION_ENTRY(103, 9)   //id
	PDA_END_STATE_TRANSITION()
	PDA_BEGIN_STATE_TRANSITION(9)
		PDA_STATE_TRANSITION_ENTRY(PDA_END_OF_EVENT, -2)  //$
		PDA_STATE_TRANSITION_ENTRY(7, -2)    //TK_TOKEN
		PDA_STATE_TRANSITION_ENTRY(6, -2)    //TK_MACRO
	PDA_END_STATE_TRANSITION()
	//state
	PDA_BEGIN_STATE_SET()
		PDA_STATE_SET_ENTRY(PDA_STATE_START)
		PDA_STATE_SET_ENTRY(3)
		PDA_STATE_SET_ENTRY(4)
		PDA_STATE_SET_ENTRY(5)
		PDA_STATE_SET_ENTRY(6)
		PDA_STATE_SET_ENTRY(7)
		PDA_STATE_SET_ENTRY(8)
		PDA_STATE_SET_ENTRY(9)
	PDA_END_STATE_SET()
PDA_END_TRAITS_STATE_MAP()

// rule map
PDA_BEGIN_TRAITS_RULE_MAP(_LDF_lex_PdaTraits)
	PDA_RULE_ENTRY(0, 1)      // S -> lex_def $
	PDA_RULE_ENTRY(101, 2)    // lex_def -> TK_SEP rule_block
	PDA_RULE_ENTRY(102, 2)    // rule_block -> rule_block id
	PDA_RULE_ENTRY(102, 1)    // rule_block -> id
	PDA_RULE_ENTRY(103, 1)    // id -> TK_TOKEN
	PDA_RULE_ENTRY(103, 1)    // id -> TK_MACRO
PDA_END_TRAITS_RULE_MAP()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
