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
This file contains component class of lexer tables.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LEXER_TABLES_H__
#define __LEXER_TABLES_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// LexerTables

class LexerTables : public _ILexerTables,
					public _ILexerTablesAccess
{
public:
	LexerTables() throw() : m_fsa_table(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_fsa_allocator)))
	{
	}
	~LexerTables() throw()
	{
	}

// _ILexerTables methods
	virtual GKC::CallResult GenerateTables(const GKC::ShareCom<GKC::ITextStreamRoot>& sp) throw()
	{
		CallResult cr;
		clear_all();
		try {
			cr = _Generate_Lexer_Tables(sp, m_token_table, m_fsa_table);  //may throw
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		if( cr.IsFailed() ) {
			clear_all();
		}
		return cr;
	}

// _ILexerTablesAccess methods
	virtual GKC::RefPtr<GKC::TokenTable> GetTokenTable() throw()
	{
		return RefPtr<TokenTable>(m_token_table);
	}
	virtual const GKC::FSA_TABLE& GetFsaTable() throw()
	{
		return m_fsa_table.GetTable();
	}

private:
	void clear_all() throw()
	{
		m_token_table.RemoveAll();
		m_fsa_table.Reset();
		m_fsa_allocator.Clear();
	}

private:
	TokenTable  m_token_table;
	FsaTable    m_fsa_table;

	ArrayPoolAllocator m_fsa_allocator;

private:
	//noncopyable
	LexerTables(const LexerTables&) throw();
	LexerTables& operator=(const LexerTables&) throw();
};

DECLARE_COM_TYPECAST(LexerTables)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
