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
This file contains global variables for RD scanner tables component.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcCompiler.h"

#include "rdp/base/_RIdTable.h"
#include "rdp/base/_RScanner.h"
#include "rdp/base/_RTdTable.h"
#include "rdp/base/_RParser.h"

#include "rdp/ldf/token/TokenDef.h"

#include "rdp/ldf/scanner/ScannerDef.h"
#include "rdp/ldf/scanner/DoIdAction.h"

#include "rdp/ldf/TokenAction.h"
#include "rdp/ldf/ScannerAction.h"

#include "rdp/RdScannerTables.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdScannerTables

BEGIN_COM_TYPECAST(RdScannerTables)
	COM_TYPECAST_ENTRY(_IRdScannerTables, _IRdScannerTables)
END_COM_TYPECAST()

CallResult RdScannerTables::generate_tables(const ShareCom<ITextStreamRoot>& spStream)
{
	CallResult cr;

	//scanner
	_RScanner rs;
	_Token_SetActions(rs);  //may throw
	rs.SetStream(spStream);
	rs.SetStartAction(ACT_TK);

	//table
	_RTdTable rt;
	if ( !_Scanner_InitTable(rt) ) { //may throw
		cr.SetResult(SystemCallResults::Fail);
		return cr;
	}

	//parser
	_RParser rp;
	_Scanner_ActionSet sas;
	m_token_table.Initialize(CPL_TK_FIRST);  //may throw
	m_action_table.Initialize(LA_FIRST);  //may throw
	_Scanner_InitActions(rp, sas, m_token_table, m_action_table);  //may throw
	rp.SetScanner(RefPtr<_RScanner>(rs));
	rp.SetTable(RefPtr<_RTdTable>(rt));
	rp.Start();  //may throw
	int res;
	while ( (res = rp.Parse()) != 0 ) { //may throw
		if ( res < 0 ) {
			cr.SetResult(SystemCallResults::Fail);
			break;
		}
	}

	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
