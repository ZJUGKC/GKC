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
This file contains component class of RD scanner tables.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __RD_SCANNER_TABLES_H__
#define __RD_SCANNER_TABLES_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// RdScannerTables

class RdScannerTables : public _IRdScannerTables
{
public:
	RdScannerTables() noexcept
	{
	}
	RdScannerTables(const RdScannerTables&) = delete;
	RdScannerTables& operator=(const RdScannerTables&) = delete;
	~RdScannerTables() noexcept
	{
	}

// _IRdScannerTables methods
	virtual GKC::CallResult Generate(const GKC::ShareCom<GKC::ITextStreamRoot>& spStream) noexcept
	{
		CallResult cr;
		try {
			cr = generate_tables(spStream);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::ConstStringA GetTokenName(uint uID) noexcept
	{
		return m_token_table.GetName(uID);
	}
	virtual bool GetTokenID(const GKC::ConstStringA& strName, uint& uID) noexcept
	{
		uID = m_token_table.Find(strName);
		return uID != 0;
	}
	virtual bool GetActionID(const GKC::ConstStringA& strName, uint& uAct) noexcept
	{
		uAct = m_action_table.Find(strName);
		return uAct != 0;
	}
	virtual uint GetMaxTokenID() noexcept
	{
		return m_token_table.GetMaxID();
	}

private:
	CallResult generate_tables(const ShareCom<ITextStreamRoot>& spStream);

private:
	_RIdTable m_token_table;
	_RIdTable m_action_table;
};

DECLARE_COM_TYPECAST(RdScannerTables)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
