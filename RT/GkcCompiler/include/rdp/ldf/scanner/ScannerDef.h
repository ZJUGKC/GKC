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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SCANNER_DEF_H__
#define __SCANNER_DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// non-terminal IDs

enum {
NT_program = 100,
NT_sec_tkid,
NT_sec_actid
};

// scanner actions

enum {
ACT_S_TK = LA_FIRST,
ACT_S_ACT
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

// _I_Scanner_DataAction_Utility

class NOVTABLE _I_Scanner_DataAction_Utility
{
public:
	virtual void SetTable(const GKC::RefPtr<GKC::_RIdTable>& refTable) noexcept = 0;
};

DECLARE_GUID(GUID__I_Scanner_DataAction_Utility)

// _ScannerDataActionBase

class NOVTABLE _ScannerDataActionBase : public _I_Scanner_DataAction_Utility
{
public:
	_ScannerDataActionBase() noexcept
	{
	}

// _I_Scanner_DataAction_Utility methods
	virtual void SetTable(const GKC::RefPtr<GKC::_RIdTable>& refTable) noexcept
	{
		m_refTable = refTable;
	}

protected:
	GKC::RefPtr<GKC::_RIdTable>  m_refTable;
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
