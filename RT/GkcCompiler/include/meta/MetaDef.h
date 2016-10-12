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
#ifndef __META_DEF_H__
#define __META_DEF_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CplLevelStack

class CplLevelStack
{
public:
	CplLevelStack() throw()
	{
	}
	~CplLevelStack() throw()
	{
	}

// methods
	uint& Add()
	{
		if( m_arr.IsBlockNull() )
			m_arr = ShareArrayHelper::MakeShareArray<uint>(MemoryHelper::GetCrtMemoryManager());  //may throw
		//overflow
		uintptr uCount = m_arr.GetCount();
		if( uCount > (uintptr)SymbolPool::MAX_LEVEL )
			throw OverflowException();
		//add
		return m_arr.Add().get_Value();  //may throw
	}
	void Pop() throw()
	{
		m_arr.RemoveAt(m_arr.GetCount() - 1);
	}

	uint GetTop() const throw()
	{
		return m_arr[m_arr.GetCount() - 1].get_Value();
	}
	uint GetTopLevel() const throw()
	{
		return (uint)(m_arr.GetCount() - 1);
	}

	bool IsEmpty() const throw()
	{
		return m_arr.GetCount() == 0;
	}

private:
	ShareArray<uint> m_arr;

private:
	//noncopyable
	CplLevelStack(const CplLevelStack&) throw();
	CplLevelStack& operator=(const CplLevelStack&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
