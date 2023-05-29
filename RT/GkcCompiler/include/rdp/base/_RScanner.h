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
#ifndef __R_SCANNER_H__
#define __R_SCANNER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//LL actions
#define LA_NULL     (0)
#define LA_FIRST    (1)

// _RScanner

class _RScanner
{
public:
	_RScanner() noexcept : m_map(MemoryHelper::GetCrtMemoryManager()), m_uStartAction(0)
	{
	}
	_RScanner(const _RScanner&) = delete;
	_RScanner& operator=(const _RScanner&) = delete;
	~_RScanner() noexcept
	{
	}

	ShareCom<_IRdScannerTables> GetTables() const noexcept
	{
		return m_spTables;
	}
	void SetTables(const ShareCom<_IRdScannerTables>& spTables) noexcept
	{
		assert( !spTables.IsBlockNull() );
		m_spTables = spTables;
	}
	ShareCom<_IRdScannerTables> GetTables() noexcept
	{
		return m_spTables;
	}
	void SetStream(const ShareCom<ITextStreamRoot>& spStream) noexcept
	{
		assert( !spStream.IsBlockNull() );
		m_spStream = spStream;
	}
	ShareCom<ITextStreamRoot> GetStream() const noexcept
	{
		return m_spStream;
	}
	void ClearActions() noexcept
	{
		m_map.RemoveAll();
	}
	void AddAction(uint uActionID, const ShareCom<_IRdScannerAction>& spAction)
	{
		m_map.Insert(uActionID, spAction);  //may throw
	}
	void SetStartAction(uint uStartAction) noexcept
	{
		m_uStartAction = uStartAction;
	}
	void Start(_LexerTokenInfo& info) noexcept
	{
		assert( !m_spStream.IsBlockNull() );
		//clear
		info.ResetCharInfo();
	}
	CallResult GetToken(_LexerTokenInfo& info)
	{
		assert( !m_spStream.IsBlockNull() );
		info.Reset();  //may throw
		info.set_ID(CPL_TK_NULL);
		uint uNextAction = m_uStartAction;
		//loop
		while ( uNextAction != LA_NULL ) {
			uint uActionID = uNextAction;
			uNextAction = LA_NULL;
			auto iter(m_map.Find(uActionID));
			assert( iter != m_map.GetEnd() );
			CallResult cr(iter.get_Value().get_Second().Deref().DoAction(m_spStream, m_spTables, uNextAction, info));
			if ( cr.IsFailed() )
				return cr;
		}
		return CallResult();
	}

private:
	//map: <action id, action>
	RBMap<uint, ShareCom<_IRdScannerAction>> m_map;
	//tables
	ShareCom<_IRdScannerTables> m_spTables;
	//stream
	ShareCom<ITextStreamRoot> m_spStream;
	//start action
	uint m_uStartAction;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
