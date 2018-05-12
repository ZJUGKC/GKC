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
						m_uLevelHead(0), m_uIDFirstAddr(0), m_uMinID(0), m_uMaxID(0)
	{
	}
	~TokenTable() throw()
	{
	}

	//remove all nodes
	void RemoveAll() throw()
	{
		m_sym_allocator.Clear();
		m_str_allocator.Clear();
		m_symbol_pool.Reset();
		//m_string_pool
		m_uLevelHead = 0;
		m_uIDFirstAddr = 0;
		m_uMinID = 0;
		m_uMaxID = 0;
	}

	//insert token without find
	void InsertToken(const ConstStringA& strToken, uint uID)
	{
		assert( uID >= CPL_TK_FIRST );
		assert( m_symbol_pool.Find(strToken).IsNull() );
		auto iter(m_symbol_pool.CreateNode(strToken, sizeof(uint), 0, 0, true, m_uLevelHead));  //may throw
		iter.GetData<BeType<uint>>().set_Value(uID);
		m_symbol_pool.SetZeroLevelHead(m_uLevelHead);
	}

	void Finish()
	{
		uint uCount = m_symbol_pool.GetTotalCount();
		//may be an empty table
		if( uCount == 0 )
			return ;
		//find minimum & maximum ID
		uint uMinID = Limits<uint>::Max;
		uint uMaxID = 0;
		{
			auto iter(m_symbol_pool.GetZeroLevelHead());
			for( ; !iter.IsNull(); iter.MoveLevelNext() ) {
				uint uID = iter.GetData<BeType<uint>>().get_Value();
				if( uID < uMinID )
					uMinID = uID;
				if( uID > uMaxID )
					uMaxID = uID;
			}
		} //end block
		assert( uMinID != 0 && uMaxID != 0 && uMinID <= uMaxID );
		uCount = uMaxID - uMinID;
		uCount = SafeOperators::AddThrow(uCount, (uint)1);  //may throw
		//allocate
		uint uBytes = SafeOperators::MultiplyThrow(uCount, (uint)sizeof(uint));  //may throw
		uint uAddr = m_str_allocator.Allocate(uBytes);
		if( uAddr == 0 )
			throw OutOfMemoryException();
		m_uIDFirstAddr = uAddr;
		m_uMinID = uMinID;
		m_uMaxID = uMaxID;
		//fill
		auto iter(m_symbol_pool.GetZeroLevelHead());
		for( ; !iter.IsNull(); iter.MoveLevelNext() ) {
			uint uID = iter.GetData<BeType<uint>>().get_Value();
			uAddr = m_uIDFirstAddr + sizeof(uint) * (uID - uMinID);
			_RefAllocatorHelper::ToObject<BeType<uint>>(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_str_allocator)), uAddr).set_Value(iter.GetKeyAddr());
		} //end for
	}

	uint GetMinID() const throw()
	{
		return m_uMinID;
	}
	uint GetMaxID() const throw()
	{
		return m_uMaxID;
	}

	//properties
	uint get_ID(const ConstStringA& strToken) const throw()
	{
		auto iter(m_symbol_pool.Find(strToken));
		if( iter.IsNull() )
			return 0;
		return iter.GetData<BeType<uint>>().get_Value();
	}
	ConstStringA get_Token(uint uID) const throw()
	{
		assert( m_uIDFirstAddr != 0 );
		assert( uID >= m_uMinID && uID <= m_uMaxID );
		uint uP = m_uIDFirstAddr + sizeof(uint) * (uID - m_uMinID);
		uint uAddr = _RefAllocatorHelper::ToObject<BeType<uint>>(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_str_allocator)), uP).get_Value();
		return m_string_pool.GetString(uAddr);
	}

private:
	ArrayPoolAllocator m_sym_allocator;
	ArrayPoolAllocator m_str_allocator;

	StringPool m_string_pool;
	SymbolPool m_symbol_pool;

	uint m_uLevelHead;
	uint m_uIDFirstAddr;
	uint m_uMinID;
	uint m_uMaxID;

private:
	//noncopyable
	TokenTable(const TokenTable&) throw();
	TokenTable& operator=(const TokenTable&) throw();
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
	void SetTokenTable(const RefPtr<TokenTable>& tk) throw()
	{
		m_token_table = tk;
	}
	const TokenTable& GetTokenTable() const throw()
	{
		return m_token_table.Deref();
	}
	void SetFsaTable(const FSA_TABLE& tb) throw()
	{
		m_fsa.SetTable(tb);
	}
	void SetAction(const ConstStringA& strToken, const ShareCom<_ILexerAction>& spAction)
	{
		if( m_arrAction.IsBlockNull() )
			m_arrAction = ShareArrayHelper::MakeShareArray<ShareCom<_ILexerAction>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//find id
		uint uID = m_token_table.Deref().get_ID(strToken);
		assert( uID > 0 );
		uID -= m_token_table.Deref().GetMinID();
		//fill
		uint uLeastCount = SafeOperators::AddThrow(uID, (uint)1);  //may throw
		if( m_arrAction.GetCount() < (uintptr)uLeastCount )
			m_arrAction.SetCount((uintptr)uLeastCount, 0);  //may throw
		m_arrAction.SetAt(uID, spAction);
	}
	void SetStream(IN const ShareCom<ITextStream>& stream, OUT int& iCharType) throw()
	{
		m_stream = stream;
		//character type of token
		iCharType = _BOMTypeToLexerTokenCharType(m_stream.Deref().GetBOM());
		m_token_info.SetCharType(iCharType);
	}
	ShareCom<ITextStream> GetStream() const throw()
	{
		return m_stream;
	}

	//token info
	_LexerTokenInfo& GetTokenInfo() throw()
	{
		return m_token_info;
	}

	//start parsing
	void Start() throw()
	{
		m_fsa.InitializeAsStopState();  //special
		m_token_info.ResetCharInfo();
	}

	// called repeatedly.
	// return : SystemCallResults::OK, the token id may be CPL_TK_ERROR.
	//          SystemCallResults::S_False, it reaches the end of stream.
	//          otherwise, this call is failed.
	CallResult Parse()
	{
		CallResult cr;

		bool bErrorOrEnd;
		//can restart (last token is error or EOE)
		if( !m_fsa.CanRestart(bErrorOrEnd) ) {
			cr.SetResult(bErrorOrEnd ? SystemCallResults::Fail : SystemCallResults::S_False);
			return cr;
		}

		uint uEvent;
		CharF ch;
		//read a character
		cr = m_stream.Deref().GetChar(ch);
		if( cr.IsFailed() )
			return cr;
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			cr.SetResult(SystemCallResults::S_False);
			return cr;
		}

		//start state
		m_fsa.SetStartState();
		m_token_info.Reset();  //may throw
		//first character
		uEvent = (uint)ch;
		m_token_info.Append(ch);  //may throw

		//loop
		do {
			//state
			m_fsa.ProcessState(uEvent);
			//stopped
			if( m_fsa.IsStopped() ) {
				//match
				uint uBackNum;
				int iMatch = m_fsa.GetMatch(uBackNum);
				assert( iMatch >= 0 );
				if( iMatch == 0 ) {
					//error
					m_token_info.set_ID(CPL_TK_ERROR);
					break;
				}
				//iMatch is the same as token id (from CPL_TK_FIRST)
				m_token_info.set_ID(iMatch);
				//back
				if( uBackNum > 0 ) {
					cr = m_stream.Deref().UngetChar((int64)uBackNum);
					assert( cr.IsOK() );
					m_token_info.BackChar(uBackNum);  //may throw
				}
				uint uID = m_token_info.get_ID();
				//action
				ShareCom<_ILexerAction> action(find_action(uID));
				if( !action.IsBlockNull() ) {
					ConstStringA strToken(m_token_table.Deref().get_Token(uID));
					bool bTokenChanged = false;
					cr = action.Deref().DoAction(m_stream, m_token_info, strToken, bTokenChanged);
					if( cr.IsFailed() ) {
						//process CPL_TK_ERROR
						if( m_token_info.get_ID() == CPL_TK_ERROR )
							cr.SetResult(SystemCallResults::OK);
						break;
					}
					if( bTokenChanged ) {
						uID = m_token_table.Deref().get_ID(strToken);
						assert( uID >= CPL_TK_FIRST );
						m_token_info.set_ID(uID);
					}
				} //end if
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
				uEvent = (uint)ch;
				m_token_info.Append(ch);  //may throw
			}
		} while( true );

		return cr;
	}

private:
	ShareCom<_ILexerAction> find_action(uint uID) const throw()
	{
		uID -= m_token_table.Deref().GetMinID();
		ShareCom<_ILexerAction> ret;
		if( m_arrAction.IsBlockNull() || (uintptr)uID >= m_arrAction.GetCount() )
			return ret;
		ret = m_arrAction[uID].get_Value();
		return ret;
	}

private:
	//settings
	RefPtr<TokenTable>   m_token_table;
	FiniteStateAutomata  m_fsa;
	ShareArray<ShareCom<_ILexerAction>>  m_arrAction;
	ShareCom<ITextStream>  m_stream;

	//token info
	_LexerTokenInfo  m_token_info;

private:
	//noncopyable
	LexerParser(const LexerParser&) throw();
	LexerParser& operator=(const LexerParser&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

// private interfaces

// _ILexerTablesAccess

class NOVTABLE _ILexerTablesAccess
{
public:
	virtual GKC::RefPtr<GKC::TokenTable> GetTokenTable() throw() = 0;
	virtual const GKC::FSA_TABLE& GetFsaTable() throw() = 0;
};

DECLARE_GUID(GUID__ILexerTablesAccess)

// _ILexerAnalyzerAccess

class NOVTABLE _ILexerAnalyzerAccess
{
public:
	virtual GKC::RefPtr<GKC::LexerParser> GetLexerParser() throw() = 0;
};

DECLARE_GUID(GUID__ILexerAnalyzerAccess)

////////////////////////////////////////////////////////////////////////////////
#endif //__LEXER_H__
////////////////////////////////////////////////////////////////////////////////
