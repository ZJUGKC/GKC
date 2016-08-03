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

/*
This file contains grammar parser.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRAMMAR_H__
#define __GRAMMAR_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// GrammarParser

class GrammarParser
{
public:
	GrammarParser() throw() : m_symbolList(MemoryHelper::GetCrtMemoryManager())
	{
	}
	~GrammarParser() throw()
	{
	}

	//settings
	void SetLexerParser(const RefPtr<LexerParser>& lexer) throw()
	{
		m_lexer = lexer;
	}
	void SetNonterminalTable(const RefPtr<TokenTable>& nt) throw()
	{
		m_nonterminal_table = nt;
	}
	void SetReductionActionTable(const RefPtr<TokenTable>& ra) throw()
	{
		m_ra_table = ra;
	}
	void SetPdaTable(const PDA_TABLE& table) throw()
	{
		m_pda.SetTable(table);
	}
	void SetErrorAction(const WeakCom<_IGrammarError>& sp) throw()
	{
		m_errorAction = sp;
	}
	void SetAction(const ConstStringA& strAction, const WeakCom<_IGrammarAction>& spAction)
	{
		if( m_arrAction.IsBlockNull() )
			m_arrAction = ShareArrayHelper::MakeShareArray<WeakCom<_IGrammarAction>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//find id
		uint uID = m_ra_table.Deref().get_ID(strAction);
		assert( uID > 0 );
		uID -= m_ra_table.Deref().GetMinID();
		//fill
		uint uLeastCount = SafeOperators::AddThrow(uID, (uint)1);  //may throw
		if( m_arrAction.GetCount() < (uintptr)uLeastCount )
			m_arrAction.SetCount((uintptr)uLeastCount, 0);  //may throw
		m_arrAction.SetAt(uID, spAction);
	}
	void SetFactory(const ConstStringA& strEvent, const ShareCom<IComFactory>& sp)
	{
		if( m_arrTerminalFactory.IsBlockNull() )
			m_arrTerminalFactory = ShareArrayHelper::MakeShareArray<ShareCom<IComFactory>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		if( m_arrNonterminalFactory.IsBlockNull() )
			m_arrNonterminalFactory = ShareArrayHelper::MakeShareArray<ShareCom<IComFactory>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//find
		uint uID;
		ShareArray<ShareCom<IComFactory>>& arr = find_factory_array(strEvent, uID);
		//fill
		uint uLeastCount = SafeOperators::AddThrow(uID, (uint)1);  //may throw
		if( arr.GetCount() < (uintptr)uLeastCount )
			arr.SetCount((uintptr)uLeastCount, 0);  //may throw
		arr.SetAt(uID, sp);
	}

	const ShareArray<StringS>& GetErrorArray() const throw()
	{
		return m_arrError;
	}

	//start
	void Start(bool bUnexpectedEoeAsError)
	{
		m_lexer.Deref().Start();
		m_pda.SetStartState();  //may throw
		if( m_arrError.IsBlockNull() )
			m_arrError = ShareArrayHelper::MakeShareArray<StringS>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arrError.RemoveAll();
		m_bUnexpectedEoeAsError = bUnexpectedEoeAsError;
		m_symbolList.RemoveAll();
	}

	// called repeatedly.
	// return : SystemCallResults::OK, uLastEventNo indicates the error state.
	//                                 PDA_NO_EVENT : correct.
	//                                 otherwise, the event No. causes an error. User can revert stack or stop parsing.
	//                                 The error list may not be empty.
	//          SystemCallResults::S_False, the end of parsing. uLastEventNo indicates the state of source stream.
	//                                      PDA_LAST_EVENT_NO : the source stream is empty, user can also wait for next parsing.
	//                                      otherwise, the source stream is non-empty.
	//                                      The error list may not be empty.
	//                                      The current event has been cleared.
	//          SystemCallResults::S_EOF, user can wait for next parsing.
	//                                    The error list may not be empty.
	//                                    The current event has been cleared.
	//                                    This value is returned after calling Start() with the parameter value false.
	//          otherwise, this call is failed.
	CallResult Parse(uint& uLastEventNo)
	{
		CallResult cr;

		uLastEventNo = PDA_NO_EVENT;
		do {
			bool bSkip;
			uint uEvent;
			//check event
			if( m_pda.IsNoEvent() ) {
				cr = get_event(bSkip, uEvent);  //may throw
				if( cr.IsFailed() )
					break;
				if( bSkip )
					continue;
				m_pda.InputEvent(uEvent);
			}
			//process
			m_pda.ProcessEvent();
			uEvent = m_pda.GetCurrentEvent();
			//actions
			if( m_pda.IsError() ) {
				//modify event
				ShareCom<_IGrammarError> spErrorAction(ShareComHelper::ToShareCom(m_errorAction));
				if( !spErrorAction.IsBlockNull() ) {
					bool bChanged = false;
					//uEvent must be a terminal symbol
					const TokenTable& tt = m_lexer.Deref().GetTokenTable();
					ConstStringA strEvent(tt.get_Token(uEvent));
					cr = spErrorAction.Deref().DoModifyEvent(strEvent, m_lexer.Deref().GetStream(), bChanged);
					if( cr.IsFailed() )
						break;
					if( bChanged ) {
						uEvent = tt.get_ID(strEvent);
						assert( uEvent >= CPL_TK_FIRST );
						m_pda.InputEvent(uEvent);
						continue;
					}
				}
				//only a S0
				if( m_pda.CheckStackStart() ) {
					if( uEvent == PDA_END_OF_EVENT ) {
						cr.SetResult(SystemCallResults::S_False);
						uLastEventNo = PDA_LAST_EVENT_NO;
						m_pda.ClearEvent();
						break;
					}
					add_unexpected_error();  //may throw
					cr.SetResult(SystemCallResults::Fail);
					break;
				}
				if( uEvent == PDA_END_OF_EVENT ) {
					if( m_bUnexpectedEoeAsError ) {
						add_unexpected_error();  //may throw
						cr.SetResult(SystemCallResults::Fail);
						break;
					}
					m_pda.ClearEvent();
					cr.SetResult(SystemCallResults::S_EOF);
					break;
				}
				add_unexpected_error();  //may throw
				uLastEventNo = uEvent;
				break;
			}
			if( m_pda.IsAccepted() ) {
				cr.SetResult(SystemCallResults::S_False);
				m_pda.ClearEvent();
				break;
			}
			int iRule;
			if( m_pda.IsReduction(iRule) ) {
				//reduce
				uint uLeftEventNo;
				uint uRightSymbolNumber = m_pda.BeginReduce(iRule, uLeftEventNo);  //may throw
				auto iter(m_symbolList.AddTail());  //may throw
				cr = create_symbol_data(uLeftEventNo, iter.get_Value());
				if( cr.IsFailed() )
					break;
				//symbol array for a production rule
				ShareArray<ShareCom<_IGrammarSymbolData>> arrSymbol(ShareArrayHelper::MakeShareArray<ShareCom<_IGrammarSymbolData>>(MemoryHelper::GetCrtMemoryManager()));  //may throw
				//rule symbols
				//0 --- left symbol, 1, 2, 3, ... --- right symbols
				add_rule_symbols(uRightSymbolNumber, arrSymbol);  //may throw
				//action
				ShareCom<_IGrammarAction> action = find_action(iRule);
				if( !action.IsBlockNull() ) {
					cr = action.Deref().DoAction(arrSymbol, m_arrError);
					if( cr.IsFailed() )
						break;
				}
				m_pda.EndReduce(uRightSymbolNumber);
				remove_rule_symbols(uRightSymbolNumber);
				break;
			}
			//shift
			uEvent = m_pda.Shift();  //may throw
			auto iter(m_symbolList.AddTail());  //may throw
			ShareCom<_IGrammarSymbolData>& spData = iter.get_Value();
			cr = create_symbol_data(uEvent, spData);
			if( cr.IsFailed() )
				break;
			_LexerTokenInfo& tokenInfo = m_lexer.Deref().GetTokenInfo();
			spData.Deref().set_Buffer(StringHelper::Clone(tokenInfo.get_Buffer()));  //may throw
			spData.Deref().set_Data(StringHelper::Clone(tokenInfo.get_Data()));  //may throw
			spData.Deref().set_WordInfo(tokenInfo.get_WordInfo());
		} while( true );  //end while

		return cr;
	}

	//revert
	CallResult Revert()
	{
		CallResult cr;
		bool bOK = m_pda.Revert();
		while( !bOK ) {
			bool bSkip;
			uint uEvent;
			cr = get_event(bSkip, uEvent);  //may throw
			if( cr.IsFailed() )
				break;
			if( bSkip )
				continue;
			if( uEvent == PDA_END_OF_EVENT ) {
				//error
				cr.SetResult(SystemCallResults::Fail);
				break;
			}
			m_pda.InputEvent(uEvent);
			bOK = m_pda.RevertAgain();
		}
		return cr;
	}

private:
	//get next event
	CallResult get_event(bool& bSkip, uint& uEvent)
	{
		CallResult cr;
		bSkip = false;
		//token
		cr = m_lexer.Deref().Parse();  //may throw
		if( cr.IsFailed() ) {
			_CplErrorBuffer eb;
			result_to_string(cr, FixedArrayHelper::GetInternalPointer(eb), _CplErrorBuffer::c_size);
			StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			StringUtilHelper::MakeString(eb, strError);  //may throw
			m_arrError.Add(strError);  //may throw
			return cr;
		}
		if( cr.GetResult() == SystemCallResults::S_False ) {
			cr.SetResult(SystemCallResults::OK);
			//CPL_TK_EOF
			uEvent = PDA_END_OF_EVENT;
			return cr;
		}
		_LexerTokenInfo& tokenInfo = m_lexer.Deref().GetTokenInfo();
		uint uTokenID = tokenInfo.get_ID();
		//check value
		if( uTokenID == CPL_TK_NULL ) {
			//skip
			bSkip = true;
			return cr;
		}
		if( uTokenID == CPL_TK_ERROR ) {
			const _CplErrorBuffer& eb = tokenInfo.get_ErrorString();
			_CplErrorBuffer tmp;
			int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), _CplErrorBuffer::c_size,
									_S("Error (%u) : (%u) %s"), 
									SafeOperators::AddThrow(tokenInfo.get_WordInfo().infoStart.uRow, (uint)1),
									SafeOperators::AddThrow(tokenInfo.get_WordInfo().infoStart.uCol, (uint)1),
									eb.GetLength() == 0 ?
										ConstArrayHelper::GetInternalPointer(CS_U2S(StringUtilHelper::To_ConstString(tokenInfo.get_Buffer())).GetC())
										: FixedArrayHelper::GetInternalPointer(eb)
									);  //may throw
			if( ret >= 0 )
				tmp.SetLength(ret);
			StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			StringUtilHelper::MakeString(tmp, strError);  //may throw
			//add to error string list
			m_arrError.Add(strError);  //may throw
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		uEvent = uTokenID;
		return cr;
	}
	//find action
	ShareCom<_IGrammarAction> find_action(uint uID) const throw()
	{
		uID -= m_ra_table.Deref().GetMinID();
		ShareCom<_IGrammarAction> ret;
		if( m_arrAction.IsBlockNull() || (uintptr)uID >= m_arrAction.GetCount() )
			return ret;
		ret = ShareComHelper::ToShareCom(m_arrAction[uID].get_Value());
		return ret;
	}
	//factories
	ShareArray<ShareCom<IComFactory>>& find_factory_array(const ConstStringA& strEvent, uint& uID) throw()
	{
		const TokenTable& tt = m_lexer.Deref().GetTokenTable();
		uID = tt.get_ID(strEvent);
		if( uID != 0 ) {
			uID -= tt.GetMinID();
			return m_arrTerminalFactory;
		}
		uID = m_nonterminal_table.Deref().get_ID(strEvent);
		assert( uID > 0 );
		uID -= m_nonterminal_table.Deref().GetMinID();
		return m_arrNonterminalFactory;
	}
	//create symbol data
	CallResult create_symbol_data(uint uEventNo, ShareCom<_IGrammarSymbolData>& sp) throw()
	{
		assert( !m_arrTerminalFactory.IsBlockNull() && !m_arrNonterminalFactory.IsBlockNull() );
		assert( uEventNo > 0 );
		//find factory
		ShareCom<IComFactory> spFactory;
		const TokenTable& tt = m_lexer.Deref().GetTokenTable();
		if( uEventNo <= tt.GetMaxID() ) {
			assert( uEventNo >= tt.GetMinID() );
			uEventNo -= tt.GetMinID();
			assert( (uintptr)uEventNo < m_arrTerminalFactory.GetCount() );
			spFactory = m_arrTerminalFactory[uEventNo].get_Value();
		}
		else {
			assert( uEventNo >= m_nonterminal_table.Deref().GetMinID());
			uEventNo -= m_nonterminal_table.Deref().GetMinID();
			assert( (uintptr)uEventNo < m_arrNonterminalFactory.GetCount() );
			spFactory = m_arrNonterminalFactory[uEventNo].get_Value();
		} //end if
		assert( !spFactory.IsBlockNull() );
		//create
		ShareCom<void> spV;
		CallResult cr(spFactory.Deref().CreateInstance(USE_GUID(GUID__IGrammarSymbolData), spV));
		if( cr.IsFailed() )
			return cr;
		sp = ShareComHelper::TypeCast<void, _IGrammarSymbolData>(spV);
		return cr;
	}
	//0 --- left symbol, 1, 2, 3, ... --- right symbols
	void add_rule_symbols(uint uRightSymbolNumber, ShareArray<ShareCom<_IGrammarSymbolData>>& arr)
	{
		uintptr uCount = m_symbolList.GetCount();
		assert( uRightSymbolNumber > 0 );
		assert( uCount > (uintptr)uRightSymbolNumber );
		auto iter(m_symbolList.GetTail());
		arr.Add(iter.get_Value());  //may throw
		uint uR = uRightSymbolNumber;
		while( uR > 0 ) {
			iter.MovePrev();
			uR --;
		}
		for( uint i = 0; i < uRightSymbolNumber; i ++ ) {
			arr.Add(iter.get_Value());  //may throw
			iter.MoveNext();
		}
	}
	void remove_rule_symbols(uint uRightSymbolNumber) throw()
	{
		auto iter(m_symbolList.GetTail());
		iter.MovePrev();
		while( uRightSymbolNumber > 0 ) {
			auto iter1(iter);
			iter.MovePrev();
			m_symbolList.RemoveAt(iter1);
			uRightSymbolNumber --;
		}
	}

	//unexpected error
	void add_unexpected_error()
	{
		_LexerTokenInfo& tokenInfo = m_lexer.Deref().GetTokenInfo();
		_CplErrorBuffer tmp;
		int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), _CplErrorBuffer::c_size,
								_S("Error (%u) : (%u) Unexpected."),
								SafeOperators::AddThrow(tokenInfo.get_WordInfo().infoStart.uRow, (uint)1),
								SafeOperators::AddThrow(tokenInfo.get_WordInfo().infoStart.uCol, (uint)1)
								);  //may throw
		if( ret >= 0 )
			tmp.SetLength(ret);
		StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		StringUtilHelper::MakeString(tmp, strError);  //may throw
		m_arrError.Add(strError);  //may throw
	}

private:
	RefPtr<LexerParser>  m_lexer;
	RefPtr<TokenTable>   m_nonterminal_table;
	RefPtr<TokenTable>   m_ra_table;  //reduction action name
	PushDownAutomata     m_pda;
	WeakCom<_IGrammarError>  m_errorAction;
	ShareArray<WeakCom<_IGrammarAction>>  m_arrAction;
	//error
	ShareArray<StringS>  m_arrError;

	//EOE as error
	bool m_bUnexpectedEoeAsError;

	//symbol list
	List<ShareCom<_IGrammarSymbolData>> m_symbolList;  //for user data
	//factories for user data
	ShareArray<ShareCom<IComFactory>> m_arrTerminalFactory;
	ShareArray<ShareCom<IComFactory>> m_arrNonterminalFactory;

private:
	//noncopyable
	GrammarParser(const GrammarParser&) throw();
	GrammarParser& operator=(const GrammarParser&) throw();
};

// private interfaces

// _IGrammarTablesAccess

class NOVTABLE _IGrammarTablesAccess
{
public:
	virtual RefPtr<TokenTable> GetNonterminalTable() throw() = 0;
	virtual RefPtr<TokenTable> GetReductionActionTable() throw() = 0;
	virtual const PDA_TABLE& GetPdaTable() throw() = 0;
};

DECLARE_GUID(GUID__IGrammarTablesAccess)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GRAMMAR_H__
////////////////////////////////////////////////////////////////////////////////
