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
This file contains component class of RD scanner.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __RD_SCANNER_H__
#define __RD_SCANNER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdScanner

class RdScanner : public _IRdScanner,
				public _IRdScannerAccess
{
public:
	RdScanner() noexcept
	{
	}
	RdScanner(const RdScanner&) = delete;
	RdScanner& operator=(const RdScanner&) = delete;
	~RdScanner() noexcept
	{
	}

// _IRdScanner methods
	virtual void SetTables(const GKC::ShareCom<_IRdScannerTables>& spTables) noexcept
	{
		m_s.SetTables(spTables);
	}
	virtual void SetStream(const GKC::ShareCom<GKC::ITextStreamRoot>& spStream) noexcept
	{
		m_s.SetStream(spStream);
	}
	virtual void ClearActions() noexcept
	{
		m_s.ClearActions();
	}
	virtual GKC::CallResult AddAction(const GKC::ConstStringA& strAction, const GKC::ShareCom<_IRdScannerAction>& spAction) noexcept
	{
		CallResult cr;

		//find
		uint uActionID;
		cr = find_action(strAction, uActionID);
		if ( cr.IsFailed() )
			return cr;

		//add
		try {
			m_s.AddAction(uActionID, spAction);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::CallResult SetStartAction(const GKC::ConstStringA& strStartAction) noexcept
	{
		CallResult cr;
		uint uStartAction;
		cr = find_action(strStartAction, uStartAction);
		if ( cr.IsFailed() )
			return cr;
		m_s.SetStartAction(uStartAction);
		return cr;
	}
	virtual void Start(_LexerTokenInfo& info) noexcept
	{
		m_s.Start(info);
	}
	virtual GKC::CallResult GetToken(_LexerTokenInfo& info) noexcept
	{
		CallResult cr;
		try {
			cr = m_s.GetToken(info);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

// _IRdScannerAccess methods
	virtual GKC::RefPtr<GKC::_RScanner> GetScanner() noexcept
	{
		return RefPtr<_RScanner>(m_s);
	}

private:
	CallResult find_action(const ConstStringA& strName, uint& uID) const noexcept
	{
		assert( !m_s.GetTables().IsBlockNull() );
		CallResult cr;
		if ( !m_s.GetTables().Deref().GetActionID(strName, uID) )
			cr.SetResult(SystemCallResults::Invalid);
		return cr;
	}

private:
	_RScanner m_s;
};

DECLARE_COM_TYPECAST(RdScanner)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
