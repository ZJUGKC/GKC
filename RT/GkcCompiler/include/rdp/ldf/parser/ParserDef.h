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
#ifndef __PARSER_DEF_H__
#define __PARSER_DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// non-terminal IDs

enum {
NT_program = 100,
NT_rule_item,
NT_rule_right,
NT_rule_right_tail,
NT_act_item,
NT_item_list,
NT_item_list_tail,
NT_item_one,
NT_item_one_tail
};

// action IDs

enum {
ACT_rule_item_end = LA_FIRST,
ACT_P_NT,
ACT_P_VERT,
ACT_P_ACT,
ACT_P_EPS,
ACT_P_TK
};

// _Parser_ActionData

struct _Parser_ActionData
{
	ShareCom<_IRdScannerTables> spTables;
	ShareArray<RULEELEMENT> arr;
	RefPtr<_RIdTable> nt_table;
	RefPtr<_RIdTable> act_table;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

// _I_Parser_DataAction_Utility

class NOVTABLE _I_Parser_DataAction_Utility
{
public:
	virtual void SetData(const GKC::RefPtr<GKC::_Parser_ActionData>& refData) noexcept = 0;
};

DECLARE_GUID(GUID__I_Parser_DataAction_Utility)

// _ParserDataActionBase

class _ParserDataActionBase : public _I_Parser_DataAction_Utility
{
public:
	_ParserDataActionBase() noexcept
	{
	}

// _I_Parser_DataAction_Utility methods
	virtual void SetData(const GKC::RefPtr<GKC::_Parser_ActionData>& refData) noexcept
	{
		m_refData = refData;
	}

protected:
	GKC::RefPtr<GKC::_Parser_ActionData> m_refData;
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
