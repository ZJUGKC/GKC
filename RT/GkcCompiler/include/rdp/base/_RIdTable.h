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
Identifier table
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __R_ID_TABLE_H__
#define __R_ID_TABLE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _RIdTable

class _RIdTable
{
public:
	_RIdTable() noexcept : m_map(MemoryHelper::GetCrtMemoryManager()), m_uMinID(1), m_uMaxID(1)
	{
	}
	_RIdTable(const _RIdTable&) = delete;
	_RIdTable& operator=(const _RIdTable&) = delete;
	~_RIdTable() noexcept
	{
	}

	void Initialize(uint uFirst = 1)
	{
		if ( m_arrId.IsBlockNull() )
			m_arrId = ShareArrayHelper::MakeShareArray<StringA>(MemoryHelper::GetCrtMemoryManager());  //may throw
		m_arrId.RemoveAll();
		m_map.RemoveAll();
		m_uMinID = uFirst;
		m_uMaxID = uFirst;
	}

	ConstStringA GetName(uint uID) const noexcept
	{
		assert( uID >= m_uMinID && uID < m_uMaxID );
		return StringUtilHelper::To_ConstString(m_arrId[uID - m_uMinID]);
	}

	//find
	uint Find(const ConstStringA& str) const noexcept
	{
		auto iter(m_map.Find(str));
		if ( iter == m_map.GetEnd() )
			return 0;
		return iter.get_Value().get_Second();
	}

	//insert
	uint Insert(const StringA& str)
	{
		ConstStringA c_str(StringUtilHelper::To_ConstString(str));
		auto iter(m_map.Find(c_str));
		if ( iter != m_map.GetEnd() )
			return iter.get_Value().get_Second();
		if ( CheckOverflow(m_uMaxID) )
			throw OverflowException();
		m_arrId.Add(str);  //may throw
		iter = m_map.InsertWithoutFind(c_str);  //may throw
		iter.get_Value().get_Second() = m_uMaxID ++;
		return m_uMaxID - 1;
	}

	//maximum ID
	uint GetMaxID() const noexcept
	{
		return m_uMaxID;
	}

	//tools
	static bool CheckOverflow(uint uID) noexcept
	{
		return uID >= Limits<uint>::Max / 10 * 9;
	}

private:
	HashMap<ConstStringA, uint, ConstStringHashTrait<ConstStringA>, ConstStringCompareTrait<ConstStringA>> m_map;
	ShareArray<StringA> m_arrId;  //string pool

	uint m_uMinID;
	uint m_uMaxID;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
