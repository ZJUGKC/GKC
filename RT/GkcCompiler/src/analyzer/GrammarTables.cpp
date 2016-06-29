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
This file contains global variables for grammar tables component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "base/DataColl.h"
#include "base/Fsa.h"
#include "base/Lexer.h"
#include "base/Pda.h"
#include "base/PdaTable.h"
#include "base/Grammar.h"

#include "analyzer/GrammarTables.h"

////////////////////////////////////////////////////////////////////////////////

#include "base/Grammar.cpp"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GrammarTables

BEGIN_COM_TYPECAST(GrammarTables)
	COM_TYPECAST_ENTRY(_IGrammarTables, _IGrammarTables)
	COM_TYPECAST_ENTRY(_IGrammarTablesAccess, _IGrammarTablesAccess)
END_COM_TYPECAST()

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
