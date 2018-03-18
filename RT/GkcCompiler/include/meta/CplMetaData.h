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
	//symbols
	virtual _CplMetaDataPosition Find(const ConstStringA& str) throw()
	{
		auto iter(m_sym_pool.Find(str));
		return _CplMetaDataPosition(iter.GetPosition().GetAddr());
	}
	virtual _CplMetaDataPosition FindNext(const _CplMetaDataPosition& pos) throw()
	{
		auto iter(m_sym_pool.FindNext(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr()))));
		return _CplMetaDataPosition(iter.GetPosition().GetAddr());
	}
	virtual _CplMetaDataPosition GetZeroLevelHead() throw()
	{
		return _CplMetaDataPosition(m_sym_pool.GetZeroLevelHead().GetPosition().GetAddr());
	}
	virtual _CplMetaDataPosition GetLevelNext(const _CplMetaDataPosition& pos) throw()
	{
		auto iter(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr())));
		iter.MoveLevelNext();
		return _CplMetaDataPosition(iter.GetPosition().GetAddr());
	}
	virtual void GetInfo(const _CplMetaDataPosition& pos, _CplMetaDataInfo& info, bool& bAnalysis) throw()
	{
		auto iter(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr())));
		info.strKey = iter.GetKey();
		info.uType = iter.GetType();
		info.uLevel = iter.GetLevel();
		info.posData.SetAddr(iter.GetData<_MetaDataAddr>().GetAddr());
		bAnalysis = iter.IsAnalysis();
	}
	virtual void SetType(const _CplMetaDataPosition& pos, const uint& uType) throw()
	{
		auto iter(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr())));
		iter.SetType(uType);
	}
	virtual void SetLevel(const _CplMetaDataPosition& pos, const uint& uLevel) throw()
	{
		auto iter(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr())));
		iter.SetLevel(uLevel);
	}
	virtual void SetData(const _CplMetaDataPosition& pos, const _CplMetaDataPosition& posData) throw()
	{
		auto iter(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr())));
		iter.GetData<_MetaDataAddr>().SetAddr(posData.GetAddr());
	}
	virtual void ClearAnalysisFlag(const _CplMetaDataPosition& pos) throw()
	{
		auto iter(m_sym_pool.GetAtPosition(SymbolPool::Position(pos.GetAddr())));
		iter.ClearAnalysisFlag();
	}
	virtual GKC::CallResult InsertSymbol(const ConstStringA& str, const uint& uType, const bool& bLevelLink, _CplMetaDataPosition& pos) throw()
	{
		CallResult cr;
		try {
			uint uNode;
			insert_symbol(str, uType, bLevelLink, uNode);  //may throw
			pos.SetAddr(uNode);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual GKC::CallResult EnterLevel() throw()
	{
		CallResult cr;
		try {
			enter_level();  //may throw
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual _CplMetaDataPosition LeaveLevel(const bool& bReverseLevelLink) throw()
	{
		_CplMetaDataPosition pos;
		if( m_stack.IsEmpty() )
			return pos;  //zero
		uint uAddr = m_stack.GetTop();
		m_stack.Pop();
		if( bReverseLevelLink )
			m_sym_pool.ReverseLevelLink(uAddr);
		m_sym_pool.ClearLevelLinkAnalysisFlag(uAddr);
		pos.SetAddr(uAddr);
		return pos;
	}
	virtual uint GetCurrentLevel() throw()
	{
		return m_stack.IsEmpty() ? 0 : m_stack.GetTopLevel();
	}
	virtual void FinishZeroLevel(const bool& bReverseLevelLink) throw()
	{
		uint uAddr = LeaveLevel(bReverseLevelLink).GetAddr();
		assert( m_stack.IsEmpty() );
		m_sym_pool.SetZeroLevelHead(uAddr);
	}
	virtual _CplMetaDataPosition ReverseLevelLink(const _CplMetaDataPosition& posHead) throw()
	{
		uint uAddr = posHead.GetAddr();
		m_sym_pool.ReverseLevelLink(uAddr);
		return _CplMetaDataPosition(uAddr);
	}
	//data
	virtual GKC::CallResult InsertData(const uint& uSize, _CplMetaDataPosition& pos) throw()
	{
		CallResult cr;
		uint uAddr = m_usr_allocator.Allocate(uSize);
		if( uAddr == 0 ) {
			cr.SetResult(SystemCallResults::OutOfMemory);
			return cr;
		}
		pos.SetAddr(uAddr);
		return cr;
	}
	virtual uintptr GetData(const _CplMetaDataPosition& posData) throw()
	{
		if( posData.IsNull() )
			return 0;
		return m_usr_allocator.ToPointer(posData.GetAddr());
	}
	//ast
	virtual GKC::CallResult InsertAstNode(const uint& uType, _CplMetaDataPosition& pos) throw()
	{
		CallResult cr;
		try {
			auto iter(m_tree.CreateNode((uint)sizeof(_MetaDataAddr), uType));  //may throw
			pos.SetAddr(iter.GetPosition().GetAddr());
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		return cr;
	}
	virtual void SetAstParent(const _CplMetaDataPosition& pos, const _CplMetaDataPosition& posParent) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(pos.GetAddr())));
		auto iterP(m_tree.GetAtPosition(AstTree::Position(posParent.GetAddr())));
		m_tree.SetParent(iter, iterP);
	}
	virtual void SetAstChild(const _CplMetaDataPosition& pos, const _CplMetaDataPosition& posChild) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(pos.GetAddr())));
		auto iterC(m_tree.GetAtPosition(AstTree::Position(posChild.GetAddr())));
		m_tree.SetChild(iter, iterC);
	}
	virtual void SetAstNext(const _CplMetaDataPosition& pos, const _CplMetaDataPosition& posNext) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(pos.GetAddr())));
		auto iterN(m_tree.GetAtPosition(AstTree::Position(posNext.GetAddr())));
		m_tree.SetNext(iter, iterN);
	}
	virtual void SetAstData(const _CplMetaDataPosition& pos, const _CplMetaDataPosition& posData) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(pos.GetAddr())));
		iter.GetData<_MetaDataAddr>().SetAddr(posData.GetAddr());
	}
	virtual void GetAstNodeInfo(const _CplMetaDataPosition& pos, _CplAstNodeInfo& info) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(pos.GetAddr())));
		auto iterTemp(iter);
		info.uType = iter.GetType();
		iterTemp.MoveParent();
		info.posParent.SetAddr(iterTemp.GetNodeAddr());
		iterTemp = iter;
		iterTemp.MoveChild();
		info.posChild.SetAddr(iterTemp.GetNodeAddr());
		iterTemp = iter;
		iterTemp.MoveNext();
		info.posNext.SetAddr(iterTemp.GetNodeAddr());
		info.posData.SetAddr(iter.GetData<_MetaDataAddr>().GetAddr());
	}
	virtual void SetAstLinkParent(const _CplMetaDataPosition& posHead, const _CplMetaDataPosition& posParent) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(posHead.GetAddr())));
		auto iterP(m_tree.GetAtPosition(AstTree::Position(posParent.GetAddr())));
		m_tree.SetLinkParent(iter, iterP);
	}
	virtual _CplMetaDataPosition ReverseAstLink(const _CplMetaDataPosition& posHead) throw()
	{
		auto iter(m_tree.GetAtPosition(AstTree::Position(posHead.GetAddr())));
		return _CplMetaDataPosition(m_tree.ReverseLink(iter).GetPosition().GetAddr());
	}
	virtual _CplMetaDataPosition ResetAst() throw()
	{
		_CplMetaDataPosition pos(m_tree.GetStart());
		m_tree.Reset();
		return pos;
	}
	virtual _CplMetaDataPosition GetAstRoot(const _CplMetaDataPosition& posStart) throw()
	{
		m_tree.SetStart(posStart.GetAddr());
		return _CplMetaDataPosition(m_tree.GetRoot().GetPosition().GetAddr());
	}
	//storage
	virtual GKC::CallResult Load(const GKC::ShareCom<GKC::IByteStream>& sp) throw()
	{
		clear_all();
		CallResult cr;
		ShareCom<IByteStream> spStream(sp);
		cr = load_from_stream(spStream);
		if( cr.IsFailed() ) {
			clear_all();
			return cr;
		}
		return cr;
	}
	virtual GKC::CallResult Save(const GKC::ShareCom<GKC::IByteStream>& sp) throw()
	{
		CallResult cr;
		ShareCom<IByteStream> spStream(sp);
		uint uWritten;
		//header
		uint uBytes = (uint)(c_header.m_size) * sizeof(CharA);
		cr = spStream.Deref().Write((uintptr)(c_header.m_first), uBytes, uWritten);
		if( cr.IsFailed() )
			return cr;
		if( uWritten != uBytes ) {
			cr.SetResult(SystemCallResults::DiskFull);
			return cr;
		}
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
	static CallResult load_pool(ShareCom<IByteStream>& spStream, ArrayPoolAllocator& pool, uint& uAddr) throw()
	{
		uAddr = 0;
		//variables
		CallResult cr;
		uint uRead;
		//size
		BeType<uint> uSize;
		cr = spStream.Deref().Read((uintptr)(&uSize), (uint)sizeof(uSize), uRead);
		if( cr.IsFailed() )
			return cr;
		if( uRead != (uint)sizeof(uSize) ) {
			cr.SetResult(SystemCallResults::Corrupt);
			return cr;
		}
		uint uBytes = uSize.get_Value();
		if( uBytes == 0 )
			return cr;
		uAddr = pool.Allocate(uBytes);
		if( uAddr == 0 ) {
			cr.SetResult(SystemCallResults::OutOfMemory);
			return cr;
		}
		//content
		cr = spStream.Deref().Read(pool.ToPointer(uAddr), uBytes, uRead);
		if( cr.IsFailed() )
			return cr;
		if( uRead != uBytes ) {
			cr.SetResult(SystemCallResults::Corrupt);
			return cr;
		}
		return cr;
	}
	static CallResult save_pool(ShareCom<IByteStream>& spStream, const ArrayPoolAllocator& pool) throw()
	{
		CallResult cr;
		uint uWritten;
		//zero
		BeType<uint> uZero;
		uZero.set_Value(0);
		uint uSize = pool.GetSize();
		if( uSize == 0 ) {
			cr = spStream.Deref().Write((uintptr)(&uZero), (uint)sizeof(uZero), uWritten);
			if( cr.IsFailed() )
				return cr;
			if( uWritten != (uint)sizeof(uZero) ) {
				cr.SetResult(SystemCallResults::DiskFull);
				return cr;
			}
			return cr;
		}
		//pool
		uint uBytes = uSize + sizeof(uint);
		cr = spStream.Deref().Write(pool.GetAddress(), uBytes, uWritten);
		if( cr.IsFailed() )
			return cr;
		if( uWritten != uBytes ) {
			cr.SetResult(SystemCallResults::DiskFull);
			return cr;
		}
		return cr;
	}

	CallResult load_from_stream(ShareCom<IByteStream>& spStream) throw()
	{
		CallResult cr;
		uint uRead;
		//header
		CharA szBuffer[6];
		mem_zero(szBuffer, sizeof(szBuffer));
		uint uBytes = (uint)(c_header.m_size) * sizeof(CharA);
		cr = spStream.Deref().Read((uintptr)szBuffer, uBytes, uRead);
		if( cr.IsFailed() )
			return cr;
		if( uRead != uBytes ) {
			cr.SetResult(SystemCallResults::Corrupt);
			return cr;
		}
		if( compare_string(szBuffer, c_header.m_first) != 0 ) {
			cr.SetResult(SystemCallResults::Corrupt);
			return cr;
		}
		uint uAddr;
		//sym
		cr = load_pool(spStream, m_sym_allocator, uAddr);
		if( cr.IsFailed() )
			return cr;
		m_sym_pool.Reset(uAddr);
		//str
		cr = load_pool(spStream, m_str_allocator, uAddr);
		if( cr.IsFailed() )
			return cr;
		//ast
		cr = load_pool(spStream, m_ast_allocator, uAddr);
		if( cr.IsFailed() )
			return cr;
		//usr
		cr = load_pool(spStream, m_usr_allocator, uAddr);
		if( cr.IsFailed() )
			return cr;
		return cr;
	}

	void clear_all() throw()
	{
		m_str_allocator.Clear();
		m_sym_allocator.Clear();
		m_ast_allocator.Clear();
		m_usr_allocator.Clear();
		m_sym_pool.Reset();
		m_tree.Reset();
		m_stack.Clear();
	}

	//const string
	DECLARE_STATIC_CONST_STRING_MEMBER(c_header, CharA);

	//insert
	void insert_symbol(const ConstStringA& str, uint uType, bool bLevelLink, uint& uNode)
	{
		//zero level
		if( m_stack.IsEmpty() )
			m_stack.Add();  //may throw
		auto iter(m_sym_pool.CreateNode(str, sizeof(_MetaDataAddr), uType, m_stack.GetTopLevel(), bLevelLink, m_stack.GetTop()));  //may throw
		uNode = iter.GetPosition().GetAddr();
	}
	//level
	void enter_level()
	{
		//zero level
		if( m_stack.IsEmpty() )
			m_stack.Add();  //may throw
		//new level
		m_stack.Add();  //may throw
	}

private:
	ArrayPoolAllocator m_str_allocator;
	ArrayPoolAllocator m_sym_allocator;
	ArrayPoolAllocator m_ast_allocator;
	ArrayPoolAllocator m_usr_allocator;

	StringPool m_str_pool;
	SymbolPool m_sym_pool;
	AstTree    m_tree;

	//stack
	CplLevelStack m_stack;

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
