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

#include "analyzer/LexerTables.h"

////////////////////////////////////////////////////////////////////////////////

#include "base/Lexer.cpp"

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
