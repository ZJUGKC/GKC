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
This file contains component class of RD parser.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __RD_PARSER_H__
#define __RD_PARSER_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdParser

class RdParser : public _IRdParser
{
public:
	RdParser() noexcept
	{
	}
	RdParser(const RdParser&) = delete;
	RdParser& operator=(const RdParser&) = delete;
	~RdParser() noexcept
	{
	}

// _IRdParser methods
	virtual void SetTables(const GKC::ShareCom<_IRdParserTables>& spTables) noexcept
	{
		m_spTables = spTables;
	}
	virtual void SetScanner(const GKC::ShareCom<_IRdScanner>& spScanner) noexcept
	{
		m_spScanner = spScanner;
	}
	virtual void ClearActions() noexcept
	{
		m_parser.ClearActions();
	}
	virtual GKC::CallResult AddAction(const GKC::ConstStringA& strAction, const GKC::ShareCom<_IRdParserAction>& spAction) noexcept
	{
		CallResult cr;

		//find
		ShareCom<_IRdParserTablesAccess> spA;
		_COMPONENT_INSTANCE_INTERFACE(_IRdParserTables, _IRdParserTablesAccess, m_spTables, spA, cr);
		if ( cr.IsFailed() )
			return cr;
		uint uActionID = spA.Deref().GetActionTable().Deref().Find(strAction);
		if ( uActionID == 0 ) {
			cr.SetResult(SystemCallResults::Invalid);
			return cr;
		}

		//add
		try {
			m_parser.AddAction(uActionID, spAction);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual void SetAcceptedAction(const GKC::ShareCom<_IRdParserAction>& spAction) noexcept
	{
		m_parser.SetAcceptedAction(spAction);
	}
	virtual GKC::CallResult Start() noexcept
	{
		CallResult cr;

		//scanner
		ShareCom<_IRdScannerAccess> spSA;
		_COMPONENT_INSTANCE_INTERFACE(_IRdScanner, _IRdScannerAccess, m_spScanner, spSA, cr);
		if ( cr.IsFailed() )
			return cr;
		m_parser.SetScanner(spSA.Deref().GetScanner());

		//tables
		ShareCom<_IRdParserTablesAccess> spTA;
		_COMPONENT_INSTANCE_INTERFACE(_IRdParserTables, _IRdParserTablesAccess, m_spTables, spTA, cr);
		if ( cr.IsFailed() )
			return cr;
		m_parser.SetTable(spTA.Deref().GetParserTable());

		//start
		try {
			m_parser.Start();
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::CallResult Parse() noexcept
	{
		CallResult cr;
		try {
			int res = m_parser.Parse();
			if ( res == 0 )
				cr.SetResult(SystemCallResults::S_EOF);
			else if ( res < 0 )
				cr.SetResult((res == -1) ? SystemCallResults::S_False : SystemCallResults::Fail);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::CallResult Revert() noexcept
	{
		CallResult cr;
		try {
			if ( !m_parser.Revert() )
				cr.SetResult(SystemCallResults::Fail);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual bool IsEmpty() noexcept
	{
		return m_parser.IsEmpty();
	}
	virtual const GKC::ShareArray<GKC::StringS>& get_ErrorArray() noexcept
	{
		return m_parser.GetErrorArray();
	}

private:
	_RParser m_parser;

	ShareCom<_IRdParserTables> m_spTables;
	ShareCom<_IRdScanner> m_spScanner;
};

DECLARE_COM_TYPECAST(RdParser)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
