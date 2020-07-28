/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GRA_DEF_H__
#define __GRA_DEF_H__
////////////////////////////////////////////////////////////////////////////////

// _GraToken

class _GraToken
{
public:
	_GraToken() throw() : m_uID(0)
	{
	}
	_GraToken(const _GraToken& src) throw() : m_uID(src.m_uID)
	{
	}
	~_GraToken() throw()
	{
	}

	_GraToken& operator=(const _GraToken& src) throw()
	{
		m_uID = src.m_uID;
		return *this;
	}

	uint GetID() const throw()
	{
		return m_uID;
	}
	void SetID(uint uID) throw()
	{
		m_uID = uID;
	}

	//compare
	bool operator==(const _GraToken& right) const throw()
	{
		return m_uID == right.m_uID;
	}
	bool operator!=(const _GraToken& right) const throw()
	{
		return m_uID != right.m_uID;
	}
	bool operator<(const _GraToken& right) const throw()
	{
		return m_uID < right.m_uID;
	}
	bool operator>(const _GraToken& right) const throw()
	{
		return right < *this;
	}
	bool operator<=(const _GraToken& right) const throw()
	{
		return !(right < *this);
	}
	bool operator>=(const _GraToken& right) const throw()
	{
		return !(*this < right);
	}

private:
	uint m_uID;
};

// _GraTokenSet

class _GraTokenSet
{
public:
	_GraTokenSet() throw()
	{
	}
	_GraTokenSet(const _GraTokenSet& src) throw() : m_arr(src.m_arr)
	{
	}
	_GraTokenSet(_GraTokenSet&& src) throw() : m_arr(rv_forward(src.m_arr))
	{
	}
	~_GraTokenSet() throw()
	{
	}

	_GraTokenSet& operator=(const _GraTokenSet& src) throw()
	{
		m_arr = src.m_arr;
		return *this;
	}
	_GraTokenSet& operator=(_GraTokenSet&& src) throw()
	{
		m_arr = rv_forward(src.m_arr);
		return *this;
	}

	uintptr GetCount() const throw()
	{
		return m_arr.GetCount();
	}
	const _GraToken& GetToken(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return m_arr[index].get_Value();
	}
	_GraToken& GetToken(uintptr index) throw()
	{
		assert( index < GetCount() );
		return m_arr[index].get_Value();
	}

	uintptr Find(const _GraToken& tk) const throw()
	{
		return GKC::ArrayUtilHelper::Find<GKC::ShareArray<_GraToken>>(m_arr, tk).GetIndex();
	}

	void Add(const _GraToken& tk)
	{
		if( m_arr.IsBlockNull() )
			m_arr = GKC::ShareArrayHelper::MakeShareArray<_GraToken>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arr.Add(tk);  //may throw
	}

	bool AddUniqueToken(const _GraToken& tk)
	{
		if( Find(tk) == INVALID_ARRAY_INDEX ) {
			Add(tk);  //may throw
			return true;
		}
		return false;
	}

	void RemoveAt(uintptr index) throw()
	{
		m_arr.RemoveAt(index);
	}

private:
	GKC::ShareArray<_GraToken> m_arr;
};

// _GraRule

class _GraRule
{
public:
	_GraRule() throw()
	{
	}
	~_GraRule() throw()
	{
	}

	void Add(const _GraToken& tk)
	{
		if( m_arrToken.IsBlockNull() ) {
			m_arrToken = GKC::ShareArrayHelper::MakeShareArray<_GraToken>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
			//left part
			m_arrToken.Add();  //may throw
		}
		m_arrToken.Add(tk);  //may throw
	}

	uintptr GetCount() const throw()
	{
		return m_arrToken.GetCount();
	}
	const _GraToken& GetToken(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return m_arrToken[index].get_Value();
	}
	_GraToken& GetToken(uintptr index) throw()
	{
		assert( index < GetCount() );
		return m_arrToken[index].get_Value();
	}

private:
	GKC::ShareArray<_GraToken> m_arrToken;

private:
	//noncopyable
	_GraRule(const _GraRule&) throw();
	_GraRule& operator=(const _GraRule&) throw();
};

// _GraRuleSet

class _GraRuleSet
{
public:
	_GraRuleSet() throw()
	{
	}
	~_GraRuleSet() throw()
	{
	}

	void Add()
	{
		if( m_arr.IsBlockNull() ) {
			m_arr = GKC::ShareArrayHelper::MakeShareArray<_GraRule>(GKC::MemoryHelper::GetCrtMemoryManager());  //may throw
			//extended rule, S -> start_nonterminal
			m_arr.Add();  //may throw
		}
		m_arr.Add();  //may throw
	}

	uintptr GetCount() const throw()
	{
		return m_arr.GetCount();
	}
	const _GraRule& GetRule(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return m_arr[index].get_Value();
	}
	_GraRule& GetRule(uintptr index) throw()
	{
		assert( index < GetCount() );
		return m_arr[index].get_Value();
	}

private:
	GKC::ShareArray<_GraRule> m_arr;

private:
	//noncopyable
	_GraRuleSet(const _GraRuleSet&) throw();
	_GraRuleSet& operator=(const _GraRuleSet&) throw();
};

// _I_GraTokenSymbolData_Utility

class NOVTABLE _I_GraTokenSymbolData_Utility
{
public:
	virtual void GetToken(_GraToken& tk) throw() = 0;
	virtual void SetToken(const _GraToken& tk) throw() = 0;
};

DECLARE_GUID(GUID__I_GraTokenSymbolData_Utility)

// _I_GraPositionSymbolData_Utility

class NOVTABLE _I_GraPositionSymbolData_Utility
{
public:
	virtual uintptr GetPosition() throw() = 0;
	virtual void SetPosition(uintptr uPos) throw() = 0;
};

DECLARE_GUID(GUID__I_GraPositionSymbolData_Utility)

// _Gra_Data

class _Gra_Data
{
public:
	_Gra_Data(const GKC::TokenTable& tt, GKC::TokenTable& nt, GKC::TokenTable& rt) throw() : m_tt(tt), m_nt(nt), m_rt(rt)
	{
	}
	~_Gra_Data() throw()
	{
	}

	void Init()
	{
		m_nt_id = GKC::SafeOperators::AddThrow(m_tt.GetMaxID(), (uint)20);  //may throw
		m_rt_id = CPL_TK_FIRST;
	}
	void Finish()
	{
		m_nt.Finish();  //may throw
		m_rt.Finish();  //may throw
		//start nonterminal
		m_rs.GetRule(0).Add(m_rs.GetRule(1).GetToken(0));  //may throw
	}

	//terminal
	const GKC::TokenTable& GetTerminalTable() const throw()
	{
		return m_tt;
	}

	//nonterminal
	uint AddToNonterminalTable(const GKC::ConstStringA& str)
	{
		uint uID = m_nt.get_ID(str);
		if( uID == 0 ) {
			m_nt.InsertToken(str, m_nt_id);  //may throw
			uID = m_nt_id;
			m_nt_id = GKC::SafeOperators::AddThrow(m_nt_id, (uint)1);  //may throw
		}
		return uID;
	}
	//reduction
	uint AddToReductionTable(const GKC::ConstStringA& str)
	{
		m_rt.InsertToken(str, m_rt_id);  //may throw
		uint uID = m_rt_id;
		m_rt_id = GKC::SafeOperators::AddThrow(m_rt_id, (uint)1);  //may throw
		return uID;
	}

	const _GraRuleSet& GetRuleSet() const throw()
	{
		return m_rs;
	}
	_GraRuleSet& GetRuleSet() throw()
	{
		return m_rs;
	}

private:
	const GKC::TokenTable& m_tt;  //terminal
	GKC::TokenTable& m_nt;  //nonterminal
	GKC::TokenTable& m_rt;  //reduction
	//IDs
	uint m_nt_id;
	uint m_rt_id;

	_GraRuleSet m_rs;

private:
	//noncopyable
	_Gra_Data(const _Gra_Data&) throw();
	_Gra_Data& operator=(const _Gra_Data&) throw();
};

// _I_GraDataAction_Utility

class NOVTABLE _I_GraDataAction_Utility
{
public:
	virtual void SetOutput(const GKC::RefPtr<_Gra_Data>& data) throw() = 0;
};

DECLARE_GUID(GUID__I_GraDataAction_Utility)

// _GraDataActionBase

class NOVTABLE _GraDataActionBase : public _I_GraDataAction_Utility
{
public:
	_GraDataActionBase() throw()
	{
	}
	~_GraDataActionBase() throw()
	{
	}

// _I_GraDataAction_Utility methods
	virtual void SetOutput(const GKC::RefPtr<_Gra_Data>& data) throw()
	{
		m_data = data;
	}

protected:
	GKC::RefPtr<_Gra_Data> m_data;

private:
	//noncopyable
	_GraDataActionBase(const _GraDataActionBase&) throw();
	_GraDataActionBase& operator=(const _GraDataActionBase&) throw();
};

////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
