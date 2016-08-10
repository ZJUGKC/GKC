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
This file contains global variables for lexer tables component.
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

#include "ldf/Def.h"
#include "ldf/MacroTokenAction.h"
#include "ldf/DoIdTokenMacroAction.h"
#include "ldf/LdfFsa.h"
#include "ldf/RegexCharAction.h"
#include "ldf/RegexCharSymbolData.h"
#include "ldf/RegexCharSymbolData_Factory.h"
#include "ldf/RegexDoCharAction.h"
#include "ldf/Base.h"
#include "ldf/LdfRegexFsa.h"
#include "ldf/LdfRegexPda.h"
#include "ldf/RegexAnalyzer.h"
#include "ldf/LdfLexPda.h"
#include "ldf/LexAnalyzer.h"

#include "analyzer/LexerTables.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LexerTables

BEGIN_COM_TYPECAST(LexerTables)
	COM_TYPECAST_ENTRY(_ILexerTables, _ILexerTables)
	COM_TYPECAST_ENTRY(_ILexerTablesAccess, _ILexerTablesAccess)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
