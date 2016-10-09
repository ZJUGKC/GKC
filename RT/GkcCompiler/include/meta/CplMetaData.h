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
This file contains component class of meta data.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __CPL_META_DATA_H__
#define __CPL_META_DATA_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// CplMetaData

class CplMetaData : public _ICplMetaData
{
public:
	CplMetaData() throw() : m_str_pool(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_str_allocator))),
							m_sym_pool(m_str_pool, RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_sym_allocator)), 1031),
							m_tree(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(m_ast_allocator)))
	{
	}
	~CplMetaData() throw()
	{
	}

// _ICplMetaData methods
	virtual uint GetCount() throw()
	{
		return m_sym_pool.GetTotalCount();
	}
	virtual GKC::CallResult Save(const GKC::ShareCom<GKC::IByteStream>& sp) throw()
	{
		CallResult cr;
		ShareCom<IByteStream> spStream(sp);
		uint uWritten;
		//header
		DECLARE_LOCAL_CONST_STRING(CharA, l_szHead, l_uHeadLen, "WOBJ")
		cr = spStream.Deref().Write((uintptr)l_szHead, l_uHeadLen * sizeof(CharA), uWritten);
		if( cr.IsFailed() )
			return cr;
		//sym
		cr = save_pool(spStream, m_sym_allocator);
		if( cr.IsFailed() )
			return cr;
		//str
		cr = save_pool(spStream, m_str_allocator);
		if( cr.IsFailed() )
			return cr;
		//ast
		cr = save_pool(spStream, m_ast_allocator);
		if( cr.IsFailed() )
			return cr;
		//usr
		cr = save_pool(spStream, m_usr_allocator);
		if( cr.IsFailed() )
			return cr;
		return cr;
	}

private:
	static CallResult save_pool(ShareCom<IByteStream>& spStream, const ArrayPoolAllocator& pool) throw()
	{
		uint uWritten;
		//zero
		BeType<uint> uZero;
		uZero.set_Value(0);
		uint uSize = pool.GetSize();
		if( uSize == 0 )
			return spStream.Deref().Write((uintptr)(&uZero), sizeof(uZero), uWritten);
		//pool
		return spStream.Deref().Write(pool.GetAddress(), uSize + sizeof(uint), uWritten);
	}

private:
	ArrayPoolAllocator m_str_allocator;
	ArrayPoolAllocator m_sym_allocator;
	ArrayPoolAllocator m_ast_allocator;
	ArrayPoolAllocator m_usr_allocator;

	StringPool m_str_pool;
	SymbolPool m_sym_pool;
	AstTree    m_tree;

private:
	//noncopyable
	CplMetaData(const CplMetaData&) throw();
	CplMetaData& operator=(const CplMetaData&) throw();
};

DECLARE_COM_TYPECAST(CplMetaData)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
