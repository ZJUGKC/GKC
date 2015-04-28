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
		if( !m_bInit ) {
			m_arr = SharedArrayHelper::MakeSharedArray<byte>(MemoryHelper::GetCrtMemoryManager());
			m_bInit = true;
		}
		uintptr uCount = m_arr.GetCount();
		//overflow
		if( uCount > (uintptr)(Limits<uint>::Max) )
			throw OverflowException();
		//add a room for count number
		if( uCount < (uintptr)sizeof(uint) )
			uCount = (uintptr)sizeof(uint);
		uint uNew = SafeOperators::AddThrow((uint)uCount, uBytes);
		m_arr.SetCount((uintptr)uNew, 0);
		//fill the count number
		uint uAct = uNew - ((uint)sizeof(uint));
		if( !ByteOrderHelper::IsBigEndianHost() ) {
			uAct = ByteOrderHelper::Swap(uAct);
		}
		*((uint*)ToPtr(0)) = uAct;
		//return a nonzero value
		return (uint)uCount;
	}
	//get the pointer
	void* ToPtr(uint index) const throw()
	{
		return SharedArrayHelper::GetInternalPointer(m_arr) + index;
	}
	//get the size
	uint GetSize() const throw()
	{
		uintptr uCount = m_arr.GetCount();
		if( uCount == 0 )
			return 0;
		assert( uCount >= (uintptr)sizeof(uint) );
		uint uSize = *((uint*)ToPtr(0));
		if( !ByteOrderHelper::IsBigEndianHost() ) {
			uSize = ByteOrderHelper::Swap(uSize);
		}
		return uSize;
	}

private:
	bool  m_bInit;
	SharedArray<byte>  m_arr;

private:
	//noncopyable
};

// StringPool

class StringPool
{
public:
	StringPool() throw()
	{
	}
	~StringPool() throw()
	{
	}

	const RefPtr<CharA> GetString(uint uIndex) const throw()
	{
		return RefPtr<CharA>((CharA*)m_alloc.ToPtr(uIndex + sizeof(uint)));
	}
	uint GetStringLength(uint uIndex) const throw()
	{
		uint uLen = *((uint*)m_alloc.ToPtr(uIndex));
		if( !ByteOrderHelper::IsBigEndianHost() ) {
			uLen = ByteOrderHelper::Swap(uLen);
		}
		return uLen;
	}

	//nonempty string
	uint PutString(const StringA& str)
	{
		uintptr uLen = str.GetLength();
		assert( uLen > 0 );
		//overflow
		if( uLen >= (uintptr)(Limits<uint>::Max) )
			throw OverflowException();
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(uint), (uint)((uLen + 1) * sizeof(CharA)));
		uint uIndex = m_alloc.Allocate(uBytes);
		//length
		uint uSave = (uint)uLen;
		if( !ByteOrderHelper::IsBigEndianHost() ) {
			uSave = ByteOrderHelper::Swap(uSave);
		}
		*((uint*)m_alloc.ToPtr(uIndex)) = uSave;
		//copy
		mem_copy(SharedArrayHelper::GetInternalPointer(str), (uLen + 1) * sizeof(CharA), m_alloc.ToPtr(uIndex + sizeof(uint)));
		return uIndex;
	}

private:
	DataPoolAllocator m_alloc;

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__POOL_H__
////////////////////////////////////////////////////////////////////////////////
