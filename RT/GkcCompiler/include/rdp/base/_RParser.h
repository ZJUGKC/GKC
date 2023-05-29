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

////////////////////////////////////////////////////////////////////////////////
#ifndef __R_PARSER_H__
#define __R_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//definitions
#define TK_NO_EVENT      ((uint)-3)

// push-down stack
typedef ShareArray<RULEELEMENT>  _RdPushDownStack;

// _RParser

class _RParser
{
public:
	_RParser() noexcept : m_map(MemoryHelper::GetCrtMemoryManager()),
						m_uMaxTerminalID(CPL_TK_NULL)
	{
	}
	_RParser(const _RParser&) = delete;
	_RParser& operator=(const _RParser&) = delete;
	~_RParser() noexcept
	{
	}

	void SetScanner(const RefPtr<_RScanner>& p) noexcept
	{
		assert( !p.IsNull() );
		m_refScanner = p;
	}
	void SetTable(const RefPtr<_RTdTable>& p) noexcept
	{
		assert( !p.IsNull() );
		m_refTable = p;
	}
	void ClearActions() noexcept
	{
		m_map.RemoveAll();
	}
	void AddAction(uint uActionID, const ShareCom<_IRdParserAction>& rpa)
	{
		m_map.Insert(uActionID, rpa);  //may throw
	}
	void SetAcceptedAction(const ShareCom<_IRdParserAction>& rpa) noexcept
	{
		m_rpaAccepted = rpa;
	}
	void SetErrorAction(const GKC::ShareCom<_IRdParserError>& sp) noexcept
	{
		m_errorAction = sp;
	}

	void Start()
	{
		m_uMaxTerminalID = m_refTable.Deref().GetMaxTerminalID();
		m_refScanner.Deref().Start(m_tokenInfo);
		if( m_vecError.IsBlockNull() )
			m_vecError = ShareArrayHelper::MakeShareArray<StringS>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_vecError.RemoveAll();
		if( m_stack.IsBlockNull() )
			m_stack = ShareArrayHelper::MakeShareArray<RULEELEMENT>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_stack.RemoveAll();
		RULEELEMENT elem;
		elem.uToken = m_refTable.Deref().GetStartNT();
		elem.uAction = LA_NULL;
		m_stack.Add(elem);  //may throw
		m_uCurrentEvent = TK_NO_EVENT;
		m_bEmpty = true;
	}
	//return value: < 0 (error, -1 means it can be reverted), = 0 (end), > 0 (succeeded)
	int Parse()
	{
		CallResult cr;

		//loop
		while( true ) {
			//input event
			if( m_uCurrentEvent == TK_NO_EVENT ) {
				cr = m_refScanner.Deref().GetToken(m_tokenInfo);  //may throw
				if( cr.IsFailed() ) {
					append_unexpected_error();  //may throw
					return -2;
				}
				uint uID = m_tokenInfo.get_ID();
				if( uID == CPL_TK_ERROR ) {
					append_unexpected_error();  //may throw
					return -2;
				}
				if( uID == CPL_TK_NULL )
					continue;
				m_uCurrentEvent = uID;
			}

			//end
			if( m_stack.IsEmpty() ) {
				if( m_uCurrentEvent != CPL_TK_EOF ) {
					append_unexpected_error();  //may throw
					return -2;
				}
				//accepted
				if( !m_bEmpty && !m_rpaAccepted.IsBlockNull() ) {
					cr = m_rpaAccepted.Deref().DoAction(m_tokenInfo, m_vecError);
					if( cr.IsFailed() )
						return -2;
				}
				return 0;
			}

			//top
			RULEELEMENT elem = m_stack.GetAt(m_stack.GetCount() - 1);

			// terminal
			if( elem.uToken <= m_uMaxTerminalID ) {
				if( elem.uToken != m_uCurrentEvent ) {
					bool bChanged = false;
					cr = do_error(m_uCurrentEvent, bChanged);
					if( cr.IsSucceeded() && bChanged )
						continue;
					append_unexpected_error();  //may throw
					return -1;
				}
				//pop
				m_stack.RemoveAt(m_stack.GetCount() - 1);
				//action
				cr = do_action(elem.uAction);
				if( cr.IsFailed() )
					return -2;
				//reset
				m_uCurrentEvent = TK_NO_EVENT;
				break;
			}

			// nonterminal
			int iAct = m_refTable.Deref().Input(elem.uToken, m_uCurrentEvent);
			if( iAct == 0 ) {
				bool bChanged = false;
				cr = do_error(m_uCurrentEvent, bChanged);
				if( cr.IsSucceeded() && bChanged )
					continue;
				append_unexpected_error();  //may throw
				return -1;
			}
			RULEITEM item;
			if( iAct > 0 ) {
				//rule
				m_refTable.Deref().GetRule(iAct - 1, item);
				//action
				cr = do_action(item.pRule[0].uAction);
				if( cr.IsFailed() )
					return -2;
				//pop
				m_stack.RemoveAt(m_stack.GetCount() - 1);
				//push
				for( uintptr i = item.uNum - 1; i > 0; i -- )
					m_stack.Add(item.pRule[i]);  //may throw
				//flag
				if( m_bEmpty )
					m_bEmpty = false;
			}
			else {
				//action
				if( iAct < -1 ) {
					m_refTable.Deref().GetRule(-(iAct + 2), item);
					cr = do_action(item.pRule[0].uAction);
					if( cr.IsFailed() )
						return -2;
				}
				//pop
				m_stack.RemoveAt(m_stack.GetCount() - 1);
				break;
			}
		} //end while

		return 1;
	}
	bool Revert()
	{
		if( m_stack.IsEmpty() )
			return false;
		_RdPushDownStack skTemp(ShareArrayHelper::MakeShareArray<RULEELEMENT>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		//stack
		RULEELEMENT elem = m_stack.GetAt(m_stack.GetCount() - 1);
		while( true ) {
			m_stack.RemoveAt(m_stack.GetCount() - 1);
			skTemp.Add(elem);  //may throw
			if( m_stack.IsEmpty() )
				break;
			elem = m_stack.GetAt(m_stack.GetCount() - 1);
			//check
			if( elem.uToken <= m_uMaxTerminalID ) {
				if( m_uCurrentEvent == elem.uToken )
					return true;
			}
			else {
				int iAct = m_refTable.Deref().Input(elem.uToken, m_uCurrentEvent);
				if( iAct != 0 )
					return true;
			}
		}
		while( !skTemp.IsEmpty() ) {
			m_stack.Add(skTemp.GetAt(skTemp.GetCount() - 1));  //may throw
			skTemp.RemoveAt(skTemp.GetCount() - 1);
		}
		//input
		elem = m_stack.GetAt(m_stack.GetCount() - 1);
		while( true ) {
			CallResult cr = m_refScanner.Deref().GetToken(m_tokenInfo);  //may throw
			if( cr.IsFailed() )
				return false;
			uint uID = m_tokenInfo.get_ID();
			if( uID == CPL_TK_ERROR )
				return false;
			if( uID == CPL_TK_NULL )
				continue;
			if( uID == CPL_TK_EOF )
				return false;
			m_uCurrentEvent = uID;
			//check
			if( elem.uToken <= m_uMaxTerminalID ) {
				if( m_uCurrentEvent == elem.uToken )
					break;
			}
			else {
				int iAct = m_refTable.Deref().Input(elem.uToken, m_uCurrentEvent);
				if( iAct != 0 )
					break;
			}
		}
		return true;
	}

	//info
	bool IsEmpty() const noexcept
	{
		return m_bEmpty;
	}
	const ShareArray<StringS>& GetErrorArray() const noexcept
	{
		return m_vecError;
	}

private:
	void append_unexpected_error()
	{
		StringS strError(StringHelper::MakeEmptyString<CharS>(MemoryHelper::GetCrtMemoryManager()));  //may throw
		_CplErrorStringHelper::GenerateError(m_tokenInfo.get_WordInfo().infoStart.uRow,
											m_tokenInfo.get_WordInfo().infoStart.uCol,
											DECLARE_TEMP_CONST_STRING(ConstStringS, _S("unexpected.")),
											strError);  //may throw
		m_vecError.Add(strError);  //may throw
	}
	CallResult do_action(uint uActionID) noexcept
	{
		if( uActionID != LA_NULL ) {
			auto iterA(m_map.Find(uActionID));
			if( iterA != m_map.GetEnd() ) {
				if( !iterA.get_Value().get_Second().IsBlockNull() ) {
					CallResult cr(iterA.get_Value().get_Second().Deref().DoAction(m_tokenInfo, m_vecError));
					if( cr.IsFailed() )
						return cr;
				}
			}
		}
		return CallResult();
	}
	CallResult do_error(uint& uEvent, bool& bChanged) noexcept
	{
		CallResult cr;
		if( !m_errorAction.IsBlockNull() ) {
			ShareCom<_IRdScannerTables> spTables = m_refScanner.Deref().GetTables();
			ConstStringA strEvent(
				(uEvent == CPL_TK_EOF) ? DECLARE_TEMP_CONST_STRING(ConstStringA, "CPL_TK_EOF")
					: spTables.Deref().GetTokenName(uEvent)
				);
			ShareCom<ITextStreamRoot> spText(m_refScanner.Deref().GetStream());
			cr = m_errorAction.Deref().DoModifyEvent(strEvent, spText, bChanged);
			if( cr.IsFailed() )
				return cr;
			if( bChanged ) {
				uint uNewEvent;
				if( !spTables.Deref().GetTokenID(strEvent, uNewEvent) ) {
					cr.SetResult(SystemCallResults::Fail);
					return cr;
				}
				uEvent = uNewEvent;
			}
		}
		return cr;
	}

private:
	//<action-id, action>
	RBMap<uint, ShareCom<_IRdParserAction>>  m_map;
	ShareCom<_IRdParserAction>  m_rpaAccepted;
	ShareCom<_IRdParserError>  m_errorAction;
	RefPtr<_RScanner>  m_refScanner;
	RefPtr<_RTdTable>  m_refTable;
	uint  m_uMaxTerminalID;

	ShareArray<StringS>  m_vecError;
	_RdPushDownStack  m_stack;
	_LexerTokenInfo  m_tokenInfo;
	uint  m_uCurrentEvent;
	bool  m_bEmpty;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
