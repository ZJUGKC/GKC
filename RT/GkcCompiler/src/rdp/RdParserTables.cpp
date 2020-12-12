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
This file contains global variables for RD parser tables component.
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

#include "rdp/ldf/TokenAction.h"
#include "rdp/ldf/ParserAction.h"

#include "rdp/RdDef.h"
#include "rdp/RdParserTables.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdParserTables

BEGIN_COM_TYPECAST(RdParserTables)
	COM_TYPECAST_ENTRY(_IRdParserTables, _IRdParserTables)
	COM_TYPECAST_ENTRY(_IRdParserTablesAccess, _IRdParserTablesAccess)
END_COM_TYPECAST()

CallResult RdParserTables::generate_tables(const ShareCom<ITextStreamRoot>& spStream, const ShareCom<_IRdScannerTables>& spTables)
{
	CallResult cr;

	//scanner
	_RScanner rs;
	_Token_SetActions(rs);  //may throw
	rs.SetStream(spStream);
	rs.SetStartAction(ACT_TK);

	//table
	_RTdTable rt;
	if ( !_Parser_InitTable(rt) ) { //may throw
		cr.SetResult(SystemCallResults::Fail);
		return cr;
	}

	//prepare
	if ( m_arrRule.IsBlockNull() )
		m_arrRule = ShareArrayHelper::MakeShareArray<RULEELEMENT>(MemoryHelper::GetCrtMemoryManager());  //may throw
	m_arrRule.RemoveAll();
	uint uMaxTokenID = ShareCom<_IRdScannerTables>(spTables).Deref().GetMaxTokenID();
	_RIdTable nt_table;
	nt_table.Initialize(uMaxTokenID + 10);  //may throw

	//parser
	_RParser rp;
	_Parser_ActionSet pas;
	_Parser_ActionData data;
	data.spTables = spTables;
	data.arr = m_arrRule;
	data.nt_table = RefPtr<_RIdTable>(nt_table);
	data.act_table = RefPtr<_RIdTable>(m_action_table);
	m_action_table.Initialize(LA_FIRST);  //may throw
	_Parser_InitActions(rp, pas, data);  //may throw
	rp.SetScanner(RefPtr<_RScanner>(rs));
	rp.SetTable(RefPtr<_RTdTable>(rt));
	rp.Start();  //may throw
	while ( true ) { //may throw
		int res = rp.Parse();  //may throw
		if ( res == 0 )
			break;
		if ( res < 0 ) {
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
	}

	//generate
	if ( !m_parser_table.Generate(ShareArrayHelper::GetInternalPointer(m_arrRule), uMaxTokenID) )  //may throw
		cr.SetResult(SystemCallResults::Fail);

	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
