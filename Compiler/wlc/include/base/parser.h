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
#ifndef __PARSER_H__
#define __PARSER_H__
////////////////////////////////////////////////////////////////////////////////

#include "pda.h"
#include "lexer.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// SymbolDataBase

class SymbolDataBase
{
public:
	SymbolDataBase() throw()
	{
	}
	~SymbolDataBase() throw()
	{
	}

	const StringA& GetBuffer() const throw()
	{
		return m_strBuffer;
	}
	StringA& GetBuffer() throw()
	{
		return m_strBuffer;
	}
	const StringA& GetAux() const throw()
	{
		return m_strData;
	}
	StringA& GetAux() throw()
	{
		return m_strData;
	}
	const LEXER_CHAR_INFO& GetCharStart() const throw()
	{
		return m_startInfo;
	}
	LEXER_CHAR_INFO& GetCharStart() throw()
	{
		return m_startInfo;
	}
	const LEXER_CHAR_INFO& GetCharEnd() const throw()
	{
		return m_endInfo;
	}
	LEXER_CHAR_INFO& GetCharEnd() throw()
	{
		return m_endInfo;
	}

protected:
	StringA m_strBuffer;  //original string
	StringA m_strData;    //additional string
	LEXER_CHAR_INFO m_startInfo;
	LEXER_CHAR_INFO m_endInfo;

private:
	//noncopyable
};

// GrammarTable<T>
//  T must be derived from SymbolDataBase

template <class T>
class GrammarTable
{
public:
	GrammarTable() throw()
	{
	}
	~GrammarTable() throw()
	{
	}

	//properties
	void SetPDA(const RefPtr<PushDownAutomata<T>>& pda) throw()
	{
		m_pda = pda;
	}
	PushDownAutomata<T>& GetPDA() throw()
	{
		return m_pda.Deref();
	}
	void SetReductionActionTable(const RefPtr<TokenTable>& ra) throw()
	{
		m_ra_table = ra;
	}
	const TokenTable& GetReductionActionTable() const throw()
	{
		return m_ra_table.Deref();
	}

private:
	RefPtr<PushDownAutomata<T>>  m_pda;
	RefPtr<TokenTable>           m_ra_table;  //reduction action name

private:
	//noncopyable
};

// IGrammarError

class NOVTABLE IGrammarError
{
public:
	//return : true -- changed, false -- unchanged
	virtual bool ModifyInputEvent(INOUT uint& uEvent, INOUT RefPtr<ICharStream>& stream) = 0;
};

// IGrammarAction<T>
//  T must be derived from SymbolDataBase

template <class T>
class NOVTABLE IGrammarAction
{
public:
	virtual void DoAction(INOUT SharedArray<RefPtr<T>>& arr, INOUT SharedArray<StringS>& errorArray) = 0;
};

// GrammarParser<T>
//  T must be derived from SymbolDataBase

template <class T>
class GrammarParser
{
public:
	GrammarParser() throw()
	{
#ifdef DEBUG
		bool b = is_derived_from<T, SymbolDataBase>();
#endif
		assert( b );
	}
	~GrammarParser() throw()
	{
	}

	//settings
	void SetLexerParser(const RefPtr<LexerParser>& lexer) throw()
	{
		m_lexer = lexer;
	}
	void SetGrammarTable(const RefPtr<GrammarTable<T>>& table) throw()
	{
		m_table = table;
	}
	void SetErrorAction(const RefPtr<IGrammarError>& pAction) throw()
	{
		m_actError = pAction;
	}
	void SetAction(const ConstStringA& strAction, const RefPtr<IGrammarAction<T>>& pAction)
	{
		if( SharedArrayHelper::GetBlockPointer(m_arrAction) == NULL )
			m_arrAction = SharedArrayHelper::MakeSharedArray<RefPtr<IGrammarAction<T>>>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//find id
		uint uID = m_table.Deref().GetReductionActionTable().GetTokenID(strAction);
		assert( uID > 0 );
		//fill
		uint uLeastCount = SafeOperators::AddThrow(uID, (uint)1);  //may throw
		if( m_arrAction.GetCount() < (uintptr)uLeastCount )
			m_arrAction.SetCount((uintptr)uLeastCount, 0);  //may throw
		m_arrAction.SetAt(uID, pAction);
	}

	const SharedArray<StringS>& GetErrorArray() const throw()
	{
		return m_arrError;
	}

	//start
	void Start()
	{
		m_lexer.Deref().Start(m_tokenInfo);  //may throw
		m_table.Deref().GetPDA().SetStartState();  //may throw
		if( SharedArrayHelper::GetBlockPointer(m_arrError) == NULL )
			m_arrError = SharedArrayHelper::MakeSharedArray<StringS>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arrError.RemoveAll();
	}

	// called repeatedly.
	// return : Failed
	//          SystemCallResults::S_False, the end of parsing. The error list may not be empty.
	//          OK, the error list may not be empty.
	CallResult Parse(bool& bEmpty)
	{
		CallResult cr;

		bEmpty = false;
		do {
			bool bSkip;
			uint uEvent;
			//check event
			if( m_table.Deref().GetPDA().IsNoEvent() ) {
				cr = get_event(bSkip, uEvent);  //may throw
				if( cr.IsFailed() )
					break;
				if( bSkip )
					continue;
				m_table.Deref().GetPDA().InputEvent(uEvent);
			}
			//process
			m_table.Deref().GetPDA().ProcessEvent();
			uEvent = m_table.Deref().GetPDA().GetCurrentEvent();
			//actions
			if( m_table.Deref().GetPDA().IsError() ) {
				if( !m_actError.IsNull() ) {
					//modify event
					bool bChanged = m_actError.Deref().ModifyInputEvent(uEvent, m_lexer.Deref().GetStream());  //may throw
					if( bChanged ) {
						m_table.Deref().GetPDA().InputEvent(uEvent);
						continue;
					}
				}
				//only a S0
				if( m_table.Deref().GetPDA().CheckStackStart() ) {
					if( uEvent == PDA_END_OF_EVENT ) {
						cr.SetResult(SystemCallResults::S_False);
						bEmpty = true;
						break;
					}
					add_unexpected_error();  //may throw
					cr.SetResult(SystemCallResults::Fail);
					break;
				}
				add_unexpected_error();  //may throw
				//revert
				bool bOK = m_table.Deref().GetPDA().Revert();
				while( !bOK ) {
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
					m_table.Deref().GetPDA().InputEvent(uEvent);
					bOK = m_table.Deref().GetPDA().NextRevert();
				}
				if( cr.IsFailed() )
					break;
				continue;
			}
			if( m_table.Deref().GetPDA().IsAccepted() ) {
				cr.SetResult(SystemCallResults::S_False);
				break;
			}
			int iRule;
			if( m_table.Deref().GetPDA().IsReduction(iRule) ) {
				//reduce
				m_table.Deref().GetPDA().BeginReduce(iRule);  //may throw
				//action
				RefPtr<IGrammarAction<T>> action = find_action(iRule);
				if( !action.IsNull() ) {
					action.Deref().DoAction(m_table.Deref().GetPDA().GetReduceSymbolArray(), m_arrError);  //may throw
				}
				m_table.Deref().GetPDA().EndReduce();
				break;
			}
			//shift
			T& sData = m_table.Deref().GetPDA().Shift();  //may throw
			sData.GetBuffer() = StringUtilHelper::Clone(m_tokenInfo.GetBuffer());  //may throw
			sData.GetAux()    = StringUtilHelper::Clone(m_tokenInfo.GetData());  //may throw
			sData.GetCharStart() = m_tokenInfo.GetCharStart();
			sData.GetCharEnd()   = m_tokenInfo.GetCharEnd();
		} while( true );  //end while

		return cr;
	}

private:
	//get next event
	CallResult get_event(bool& bSkip, uint& uEvent)
	{
		CallResult cr;
		bSkip = false;
		//token
		cr = m_lexer.Deref().Parse(m_tokenInfo);  //may throw
		if( cr.IsFailed() ) {
			CplErrorBuffer eb;
			result_to_string(cr, FixedArrayHelper::GetInternalPointer(eb), CplErrorBuffer::c_size);
			StringS strError(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			StringUtilHelper::MakeString(eb, strError);  //may throw
			m_arrError.Add(strError);  //may throw
			return cr;
		}
		uint uTokenID;
		if( cr.GetResult() == SystemCallResults::S_False ) {
			cr.SetResult(SystemCallResults::OK);
			uTokenID = TK_EOF;
		}
		else {
			uTokenID = m_tokenInfo.GetID();
		}
		//check value
		if( uTokenID == TK_NULL ) {
			//skip
			bSkip = true;
			return cr;
		}
		if( uTokenID == TK_ERROR ) {
			StringS strError(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
			const CplErrorBuffer& eb = m_tokenInfo.GetErrorString();
			CplErrorBuffer tmp;
			int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), CplErrorBuffer::c_size,
									_S("Error (%u) : (%u) %s"), m_tokenInfo.GetCharStart().uRow + 1, m_tokenInfo.GetCharStart().uCol + 1,
									eb.GetLength() == 0 ?
										ConstHelper::GetInternalPointer(CS_U2S(StringUtilHelper::To_ConstString(m_tokenInfo.GetBuffer())).GetC())
										: FixedArrayHelper::GetInternalPointer(eb)
									);
			if( ret >= 0 )
				tmp.SetLength(ret);
			StringUtilHelper::MakeString(tmp, strError);  //may throw
			//add to error string list
			m_arrError.Add(strError);  //may throw
			cr.SetResult(SystemCallResults::Fail);
			return cr;
		}
		uEvent = uTokenID;
		if( uTokenID == TK_EOF )
			uEvent = PDA_END_OF_EVENT;
		return cr;
	}
	//find action
	RefPtr<IGrammarAction<T>> find_action(uint uID) const throw()
	{
		RefPtr<IGrammarAction<T>> ret;
		if( m_arrAction.IsNull() || uID >= m_arrAction.GetCount() )
			return ret;
		ret = m_arrAction[uID].get_Value();
		return ret;
	}

	//unexpected error
	void add_unexpected_error()
	{
		StringS strError(StringUtilHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		CplErrorBuffer tmp;
		int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), CplErrorBuffer::c_size,
								_S("Error (%u) : (%u) Unexpected."), m_tokenInfo.GetCharStart().uRow + 1, m_tokenInfo.GetCharStart().uCol + 1
								);
		if( ret >= 0 )
			tmp.SetLength(ret);
		StringUtilHelper::MakeString(tmp, strError);  //may throw
		m_arrError.Add(strError);  //may throw
	}

private:
	RefPtr<LexerParser>      m_lexer;
	RefPtr<GrammarTable<T>>  m_table;
	RefPtr<IGrammarError>    m_actError;
	SharedArray<RefPtr<IGrammarAction<T>>>  m_arrAction;
	//error
	SharedArray<StringS>  m_arrError;
	//internal
	LexerTokenInfo  m_tokenInfo;

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__PARSER_H__
////////////////////////////////////////////////////////////////////////////////
