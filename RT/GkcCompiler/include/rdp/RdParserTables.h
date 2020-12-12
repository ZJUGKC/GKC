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
This file contains component class of RD parser tables.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __RD_PARSER_TABLES_H__
#define __RD_PARSER_TABLES_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdParserTables

class RdParserTables : public _IRdParserTables,
					public _IRdParserTablesAccess
{
public:
	RdParserTables() noexcept
	{
	}
	RdParserTables(const RdParserTables&) = delete;
	RdParserTables& operator=(const RdParserTables&) = delete;
	~RdParserTables() noexcept
	{
	}

// _IRdParserTables methods
	virtual GKC::CallResult Generate(const GKC::ShareCom<GKC::ITextStreamRoot>& spStream, const GKC::ShareCom<_IRdScannerTables>& spTables) noexcept
	{
		CallResult cr;
		try {
			cr = generate_tables(spStream, spTables);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}

// _IParserTablesAccess methods
	virtual GKC::RefPtr<GKC::_RTdTable> GetParserTable() noexcept
	{
		return RefPtr<_RTdTable>(m_parser_table);
	}
	virtual GKC::RefPtr<GKC::_RIdTable> GetActionTable() noexcept
	{
		return RefPtr<_RIdTable>(m_action_table);
	}

private:
	CallResult generate_tables(const ShareCom<ITextStreamRoot>& spStream, const ShareCom<_IRdScannerTables>& spTables);

private:
	_RTdTable m_parser_table;
	_RIdTable m_action_table;

	ShareArray<RULEELEMENT> m_arrRule;
};

DECLARE_COM_TYPECAST(RdParserTables)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
