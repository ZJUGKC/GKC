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

////////////////////////////////////////////////////////////////////////////////
#ifndef __POOL_H__
#define __POOL_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// DataPoolAllocator

class DataPoolAllocator
{
public:
	DataPoolAllocator() throw() : m_bInit(false)
	{
	}

	//return : the index of pool array
	uint Allocate(uint uBytes)  //may throw
	{
		assert( uBytes > 0 );
		const uintptr c_Grow = 4 * 1024;  //4K
		if( !m_bInit ) {
			m_arr = SharedArrayHelper::MakeSharedArray<byte>(MemoryHelper::GetCrtMemoryManager());
			m_bInit = true;
		}
		uintptr uCount = m_arr.GetCount();
		if( uCount > (uintptr)(Limits<uint>::Max) )
			throw OverflowException();
		uint uNew = SafeOperators::AddThrow((uint)uCount, uBytes);
		m_arr.SetCount((uintptr)uNew, c_Grow);
		return (uint)uCount;
	}
	//get the pointer
	void* ToPtr(uint index) const throw()
	{
		return SharedArrayHelper::GetInternalPointer(m_arr) + index;
	}

private:
	bool  m_bInit;
	SharedArray<byte>  m_arr;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__POOL_H__
////////////////////////////////////////////////////////////////////////////////
