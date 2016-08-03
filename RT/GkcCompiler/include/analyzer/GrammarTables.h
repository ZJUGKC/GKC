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
This file contains component class of grammar tables.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRAMMAR_TABLES_H__
#define __GRAMMAR_TABLES_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GrammarTables

class GrammarTables : public _IGrammarTables,
					public _IGrammarTablesAccess
{
public:
	GrammarTables() throw() : m_pda_table(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_pda_allocator)))
	{
	}
	~GrammarTables() throw()
	{
	}

// _IGrammarTables methods
	virtual GKC::CallResult GenerateTables(const GKC::ShareCom<GKC::ITextStream>& sp) throw()
	{
		CallResult cr;
		return cr;
	}

// _IGrammarTablesAccess methods
	virtual RefPtr<TokenTable> GetNonterminalTable() throw()
	{
		return RefPtr<TokenTable>(m_nonterminal_table);
	}
	virtual RefPtr<TokenTable> GetReductionActionTable() throw()
	{
		return RefPtr<TokenTable>(m_ra_table);
	}
	virtual const PDA_TABLE& GetPdaTable() throw()
	{
		return m_pda_table.GetTable();
	}

private:
	void clear_all() throw()
	{
		m_nonterminal_table.RemoveAll();
		m_ra_table.RemoveAll();
		m_pda_table.Reset();
		m_pda_allocator.Clear();
	}

private:
	TokenTable  m_nonterminal_table;
	TokenTable  m_ra_table;
	PdaTable    m_pda_table;

	ArrayPoolAllocator m_pda_allocator;

private:
	//noncopyable
	GrammarTables(const GrammarTables&) throw();
	GrammarTables& operator=(const GrammarTables&) throw();
};

DECLARE_COM_TYPECAST(GrammarTables)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
