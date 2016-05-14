/*
** Copyright (c) 2015, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __LEXER_H__
#define __LEXER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// TokenTable

class TokenTable
{
public:
	TokenTable() throw() : m_string_pool(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_str_allocator))),
						m_symbol_pool(m_string_pool, RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_sym_allocator))),
						m_uLevelHead(0)
	{
	}
	~TokenTable() throw()
	{
	}

	void InsertToken(const ConstStringA& strToken, uint uID)
	{
		assert( uID >= TK_FIRST );
		assert( m_symbol_pool.Find(strToken).IsNull() );
		auto iter(m_symbol_pool.CreateNode(strToken, sizeof(uint), 0, 0, m_uLevelHead));  //may throw
		iter.GetData<BeType<uint>>().set_Value(uID);
		m_symbol_pool.SetZeroLevelHead(m_uLevelHead);
	}

	//properties
	uint get_ID(const ConstStringA& strToken) const throw()
	{
		auto iter(m_symbol_pool.Find(strToken));
		if( iter.IsNull() )
			return 0;
		return iter.GetData<BeType<uint>>().get_Value();
	}

private:
	ArrayPoolAllocator m_sym_allocator;
	ArrayPoolAllocator m_str_allocator;
	SymbolPool m_symbol_pool;
	StringPool m_string_pool;
	uint m_uLevelHead;  //not used in this class

private:
	//noncopyable
	TokenTable(const TokenTable&) throw();
	TokenTable& operator=(const TokenTable&) throw();
};

// LexerTables

class LexerTables
{
public:
	LexerTables() throw()
	{
	}
	~LexerTables() throw()
	{
	}

	//properties
	void SetFSA(const RefPtr<FiniteStateAutomata>& fsa) throw()
	{
		m_fsa = fsa;
	}
	FiniteStateAutomata& GetFSA() throw()
	{
		return m_fsa.Deref();
	}
	void SetTokenTable(const RefPtr<TokenTable>& tk) throw()
	{
		m_token_table = tk;
	}
	const TokenTable& GetTokenTable() const throw()
	{
		return m_token_table.Deref();
	}

private:
	RefPtr<FiniteStateAutomata>  m_fsa;
	RefPtr<TokenTable>           m_token_table;

private:
	//noncopyable
	LexerTables(const LexerTables&) throw();
	LexerTables& operator=(const LexerTables&) throw();
};

// LexerParser

class LexerParser
{
public:
	LexerParser() throw()
	{
	}
	~LexerParser() throw()
	{
	}

	//settings
	void SetLexerTable(const RefPtr<LexerTable>& table) throw()
	{
		m_table = table;
	}
	void SetAction(const ConstStringA& strToken, const RefPtr<ILexerAction>& pAction)
	{
		if( SharedArrayHelper::GetBlockPointer(m_arrAction) == NULL )
			m_arrAction = SharedArrayHelper::MakeSharedArray<RefPtr<ILexerAction>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//find id
		uint uID = m_table.Deref().GetTokenTable().GetTokenID(strToken);
		assert( uID > 0 );
		//fill
		uint uLeastCount = SafeOperators::AddThrow(uID, (uint)1);  //may throw
		if( m_arrAction.GetCount() < (uintptr)uLeastCount )
			m_arrAction.SetCount((uintptr)uLeastCount, 0);  //may throw
		m_arrAction.SetAt(uID, pAction);
	}
	void SetStream(IN const RefPtr<ICharStream>& stream) throw()
	{
		m_stream = stream;
	}
	RefPtr<ICharStream>& GetStream() throw()
	{
		return m_stream;
	}

	//start parsing
	void Start(OUT LexerTokenInfo& info) throw()
	{
		m_table.Deref().GetFSA().InitializeAsStopState();  //special
		info.ResetCharInfo();
	}

	// called repeatedly.
	// return : Failed
	//          SystemCallResults::S_False, the end of stream.
	//          OK, the token id may be TK_ERROR.
	CallResult Parse(INOUT LexerTokenInfo& info)
	{
		CallResult cr;

		bool bErrorOrEnd;
		//can restart (last token is error or EOE)
		if( !m_table.Deref().GetFSA().CanRestart(bErrorOrEnd) ) {
			cr.SetResult(bErrorOrEnd ? SystemCallResults::Fail : SystemCallResults::S_False);
			return cr;
		}

		uint uEvent;
		byte ch;
		//read a character
		cr = m_stream.Deref().GetChar(ch);
		if( cr.IsFailed() )
			return cr;
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			cr.SetResult(SystemCallResults::S_False);
			return cr;
		}

		//start state
		m_table.Deref().GetFSA().SetStartState();
		info.Reset();  //may throw
		//first character
		uEvent = ch;
		info.Append(ch);  //may throw

		//loop
		do {
			//state
			m_table.Deref().GetFSA().ProcessState(uEvent);
			//stopped
			if( m_table.Deref().GetFSA().IsStopped() ) {
				//match
				uintptr uBackNum;
				int iMatch = m_table.Deref().GetFSA().GetMatch(uBackNum);
				assert( iMatch >= 0 );
				if( iMatch == 0 ) {
					//error
					info.SetID(TK_ERROR);
					break;
				}
				//iMatch is the same as token id (from TK_FIRST)
				info.SetID(iMatch);
				//back
				assert( uBackNum <= (uintptr)Limits<uint>::Max );
				if( uBackNum > 0 ) {
					cr = m_stream.Deref().UngetChar((uint)uBackNum);
					assert( cr.IsOK() );
					info.BackChar((uint)uBackNum);  //may throw
				}
				//action
				RefPtr<ILexerAction> action = find_action(info.GetID());
				if( !action.IsNull() ) {
					action.Deref().DoAction(m_stream, info);  //may throw
				}
				break;
			} //end if
			//get next char
			cr = m_stream.Deref().GetChar(ch);
			if( cr.IsFailed() )
				break;
			if( cr.GetResult() == SystemCallResults::S_EOF ) {
				uEvent = FSA_END_OF_EVENT;
				cr.SetResult(SystemCallResults::OK);
			}
			else {
				uEvent = ch;
				info.Append(ch);  //may throw
			}
		} while( true );

		return cr;
	}

private:
	RefPtr<ILexerAction> find_action(uint uID) const throw()
	{
		RefPtr<ILexerAction> ret;
		if( m_arrAction.IsNull() || uID >= m_arrAction.GetCount() )
			return ret;
		ret = m_arrAction[uID].get_Value();
		return ret;
	}

private:
	//settings
	RefPtr<LexerTable>  m_table;
	SharedArray<RefPtr<ILexerAction>>  m_arrAction;
	RefPtr<ICharStream>  m_stream;

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__LEXER_H__
////////////////////////////////////////////////////////////////////////////////
