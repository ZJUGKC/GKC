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
This file contains analyzer functions.
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

#include "analyzer/LexerTables.h"
#include "analyzer/LexerAnalyzer.h"
#include "analyzer/GrammarTables.h"
#include "analyzer/GrammarAnalyzer.h"

////////////////////////////////////////////////////////////////////////////////

//functions

namespace GKC {

// LexerTables

inline void _Internal_LexerTables_Create(GKC::ShareCom<_ILexerTables>& sp, GKC::CallResult& cr) throw()
{
	ShareCom<_ILexerTables> spTables;
	_CREATE_COMPONENT_INSTANCE(LexerTables, _ILexerTables, spTables, cr);
	if( cr.IsFailed() )
		return ;
	sp = spTables;
}

// LexerAnalyzer

inline void _Internal_LexerAnalyzer_Create(GKC::ShareCom<_ILexerAnalyzer>& sp, GKC::CallResult& cr) throw()
{
	ShareCom<_ILexerAnalyzer> spAnalyzer;
	_CREATE_COMPONENT_INSTANCE(LexerAnalyzer, _ILexerAnalyzer, spAnalyzer, cr);
	if( cr.IsFailed() )
		return ;
	sp = spAnalyzer;
}

// GrammarTables

inline void _Internal_GrammarTables_Create(GKC::ShareCom<_IGrammarTables>& sp, GKC::CallResult& cr) throw()
{
	ShareCom<_IGrammarTables> spTables;
	_CREATE_COMPONENT_INSTANCE(GrammarTables, _IGrammarTables, spTables, cr);
	if( cr.IsFailed() )
		return ;
	sp = spTables;
}

// GrammarAnalyzer

inline void _Internal_GrammarAnalyzer_Create(GKC::ShareCom<_IGrammarAnalyzer>& sp, GKC::CallResult& cr) throw()
{
	ShareCom<_IGrammarAnalyzer> spAnalyzer;
	_CREATE_COMPONENT_INSTANCE(GrammarAnalyzer, _IGrammarAnalyzer, spAnalyzer, cr);
	if( cr.IsFailed() )
		return ;
	sp = spAnalyzer;
}

} //namespace

// LexerTables

void _LexerTables_Create(GKC::ShareCom<_ILexerTables>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_LexerTables_Create(sp, cr);
}

// LexerAnalyzer

void _LexerAnalyzer_Create(GKC::ShareCom<_ILexerAnalyzer>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_LexerAnalyzer_Create(sp, cr);
}

// GrammarTables

void _GrammarTables_Create(GKC::ShareCom<_IGrammarTables>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_GrammarTables_Create(sp, cr);
}

// GrammarAnalyzer

void _GrammarAnalyzer_Create(GKC::ShareCom<_IGrammarAnalyzer>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_GrammarAnalyzer_Create(sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
