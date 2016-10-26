﻿/*
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
#ifndef __DATA_COLL_H__
#define __DATA_COLL_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// Storage : Big Endian

// _RefAllocatorHelper

class _RefAllocatorHelper
{
public:
	template <typename T>
	static T& ToObject(const RefPtr<IMemoryAllocatorRef32>& allocator, uint p) throw()
	{
		return memory_allocator_ref_to_object<T, IMemoryAllocatorRef32, uint>(allocator, p);
	}
};

// ArrayPoolAllocator

class ArrayPoolAllocator : public IMemoryAllocatorRef32
{
public:
	ArrayPoolAllocator() throw()
	{
	}
	~ArrayPoolAllocator() throw()
	{
	}

// IMemoryAllocatorRef32 methods

	virtual uint    Allocate(const uint& uBytes) throw()
	{
		assert( uBytes > 0 );
		uint uRet = 0;
		try {
			if( m_arr.IsBlockNull() ) {
				m_arr = ShareArrayHelper::MakeShareArray<byte>(MemoryHelper::GetCrtMemoryManager());  //may throw
			}
			uintptr uCount = m_arr.GetCount();
			//overflow
			if( uCount > (uintptr)(Limits<uint>::Max) )
				throw OverflowException();  //throw
			//add a room for count number
			if( uCount < (uintptr)sizeof(uint) )
				uCount = (uintptr)sizeof(uint);
			//align
			uint uNew = AlignHelper::RoundUpThrow(uBytes, (uint)sizeof(uint));  //may throw
			//new size
			uNew = SafeOperators::AddThrow((uint)uCount, uNew);  //may throw
			m_arr.SetCount((uintptr)uNew, 0);  //may throw
			//fill the count number
			to_object<BeType<uint>>(0).set_Value(uNew - ((uint)sizeof(uint)));
			//return a nonzero value
			uRet = (uint)uCount;
		}
		catch(...) {
		}
		return uRet;
	}
	virtual uintptr ToPointer(const uint& p) throw()
	{
		assert( p < (uint)m_arr.GetCount() );
		return (uintptr)(ShareArrayHelper::GetInternalPointer(m_arr) + p);
	}

//methods

	void Clear() throw()
	{
		if( !m_arr.IsBlockNull() )
			m_arr.RemoveAll();
	}

	//get the size
	uint GetSize() const throw()
	{
		uintptr uCount = m_arr.GetCount();
		if( uCount == 0 )
			return 0;
		assert( uCount >= (uintptr)sizeof(uint) );
		return to_object<BeType<uint>>(0).get_Value();
	}
	//address
	uintptr GetAddress() const throw()
	{
		return (uintptr)ShareArrayHelper::GetInternalPointer(m_arr);
	}

private:
	template <typename T>
	T& to_object(uint p) const throw()
	{
		return _RefAllocatorHelper::ToObject<T>(RefPtrHelper::TypeCast<ArrayPoolAllocator, IMemoryAllocatorRef32>(RefPtr<ArrayPoolAllocator>(this)), p);
	}

private:
	ShareArray<byte>  m_arr;

private:
	//noncopyable
	ArrayPoolAllocator(const ArrayPoolAllocator&) throw();
	ArrayPoolAllocator& operator=(const ArrayPoolAllocator&) throw();
};

// StringPool

class StringPool
{
public:
	explicit StringPool(const RefPtr<IMemoryAllocatorRef32>& allocator) throw() : m_allocator(allocator)
	{
	}
	~StringPool() throw()
	{
	}

	ConstStringA GetString(uint p) const throw()
	{
		uint uLen = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, p).get_Value();
		return ConstStringA((CharA*)(const_cast<IMemoryAllocatorRef32&>(m_allocator.Deref()).ToPointer(p + sizeof(uint))), uLen);
	}

	//nonempty string
	uint PutString(const ConstStringA& str)
	{
		uintptr uLen = str.GetCount();
		assert( uLen > 0 );
		//overflow
		if( uLen >= (uintptr)(Limits<uint>::Max) )
			throw OverflowException();  //throw
		//allocate
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(uint), (uint)((uLen + 1) * sizeof(CharA)));  //may throw
		uint uP = m_allocator.Deref().Allocate(uBytes);
		if( uP == 0 )
			throw OutOfMemoryException();  //throw
		//length
		_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, uP).set_Value((uint)uLen);
		//copy
		mem_copy(ConstArrayHelper::GetInternalPointer(str), (uLen + 1) * sizeof(CharA), (void*)(m_allocator.Deref().ToPointer(uP + sizeof(uint))));
		return uP;
	}

private:
	RefPtr<IMemoryAllocatorRef32>  m_allocator;

private:
	//noncopyable
	StringPool(const StringPool&) throw();
	StringPool& operator=(const StringPool&) throw();
};

// SymbolPool
//   All symbols are in a hash table with multi-key. The list header of level 0 must be defined in this class. 

class SymbolPool
{
private:
	typedef SymbolPool  thisClass;

#pragma pack(push, 1)

	class _Node
	{
	public:
		_Node() throw()
		{
		}
		~_Node() throw()
		{
		}
		void Init() throw()
		{
			uHashNext.set_Value(0);
			uLevelNext.set_Value(0);
			uStringAddr.set_Value(0);
			uHashCode.set_Value(0);
			uType.set_Value(0);
			uLevel.set_Value(0);
		}
		//properties
		uint GetHashNext() const throw()
		{
			return uHashNext.get_Value();
		}
		void SetHashNext(uint uValue) throw()
		{
			uHashNext.set_Value(uValue);
		}
		uint GetLevelNext() const throw()
		{
			return uLevelNext.get_Value();
		}
		void SetLevelNext(uint uValue) throw()
		{
			uLevelNext.set_Value(uValue);
		}
		uint GetStringAddr() const throw()
		{
			return uStringAddr.get_Value();
		}
		void SetStringAddr(uint uValue) throw()
		{
			uStringAddr.set_Value(uValue);
		}
		uint GetHashCode() const throw()
		{
			return uHashCode.get_Value();
		}
		void SetHashCode(uint uValue) throw()
		{
			uHashCode.set_Value(uValue);
		}
		uint GetType() const throw()
		{
			return uType.get_Value();
		}
		void SetType(uint uValue) throw()
		{
			uType.set_Value(uValue);
		}
		uint GetLevel() const throw()
		{
			return uLevel.get_Value();
		}
		void SetLevel(uint uValue) throw()
		{
			uLevel.set_Value(uValue);
		}

	private:
		BeType<uint> uHashNext;     //the next node of hash list
		BeType<uint> uLevelNext;    //the next node of current level
		BeType<uint> uStringAddr;   //address for string (key)
		BeType<uint> uHashCode;     //the original hash code
		BeType<uint> uType;   //data type
		BeType<uint> uLevel;  //level (30 bits) and flags
		// Data block is located after the node

	private:
		_Node(const _Node& src) throw();
		_Node& operator=(const _Node& src) throw();
	};

#pragma pack(pop)

public:
	//constants
	enum { MASK_LEVEL = 0x3FFFFFFF, MAX_LEVEL = MASK_LEVEL,
		MASK_ANALYSIS = 0x40000000, RSHIFT_BITS = 30  //bit 30 in _Node::uLevel, analysis flag, indicating the analysis process or not
	};

	//Position
	class Position
	{
	public:
		explicit Position(uint uNode = 0) throw() : m_uNode(uNode)
		{
		}
		Position(const Position& src) throw() : m_uNode(src.m_uNode)
		{
		}
		~Position() throw()
		{
		}

		Position& operator=(const Position& src) throw()
		{
			if( this != &src ) {
				m_uNode = src.m_uNode;
			}
			return *this;
		}

		bool IsNull() const throw()
		{
			return m_uNode == 0;
		}

		uint GetAddr() const throw()
		{
			return m_uNode;
		}
		void SetAddr(uint uNode) throw()
		{
			m_uNode = uNode;
		}

		bool operator==(const Position& right) const throw()
		{
			return m_uNode == right.m_uNode;
		}
		bool operator!=(const Position& right) const throw()
		{
			return m_uNode != right.m_uNode;
		}

	private:
		uint m_uNode;  //address for node
	};

	//Iterator
	class Iterator
	{
	public:
		Iterator() throw() : m_pPool(NULL), m_pos()
		{
		}
		explicit Iterator(thisClass* p, uint uNode = 0) throw() : m_pPool(p), m_pos(uNode)
		{
		}
		Iterator(const Iterator& src) throw() : m_pPool(src.m_pPool), m_pos(src.m_pos)
		{
		}
		~Iterator() throw()
		{
		}

		Iterator& operator=(const Iterator& src) throw()
		{
			m_pPool = src.m_pPool;
			m_pos = src.m_pos;
			return *this;
		}

		const Position GetPosition() const throw()
		{
			return m_pos;
		}
		Position GetPosition() throw()
		{
			return m_pos;
		}

		bool IsNull() const throw()
		{
			return m_pos.IsNull();
		}

		//properties
		uint GetNodeAddr() const throw()
		{
			return m_pos.GetAddr();
		}
		uint GetKeyAddr() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			return pNode->GetStringAddr();
		}
		ConstStringA GetKey() const throw()
		{
			return m_pPool->GetStringPool().GetString(GetKeyAddr());
		}
		uint GetType() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			return pNode->GetType();
		}
		void SetType(uint uType) throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			pNode->SetType(uType);
		}
		uint GetLevel() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			return (pNode->GetLevel()) & MASK_LEVEL;
		}
		bool IsAnalysis() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			return ((pNode->GetLevel()) & MASK_ANALYSIS) != 0;
		}
		//clear analysis flag
		void ClearAnalysisFlag() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			uint uLevel = pNode->GetLevel();
			uLevel &= (~MASK_ANALYSIS);
			pNode->SetLevel(uLevel);
		}

		//for undetermined symbol
		void SetLevel(uint uLevel) throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			pNode->SetLevel(uLevel | MASK_ANALYSIS);
		}

		//user data
		void* GetDataPointer() const throw()
		{
			assert( !IsNull() );
			//after node
			return m_pPool->ToPointer(GetNodeAddr() + sizeof(_Node));
		}
		template <typename T>
		const T& GetData() const throw()
		{
			return *((T*)GetDataPointer());
		}
		template <typename T>
		T& GetData() throw()
		{
			return *((T*)GetDataPointer());
		}

		//methods
		void MoveLevelNext() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pPool->ToPointer(GetNodeAddr()));
			m_pos.SetAddr(pNode->GetLevelNext());
		}

		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_pPool == right.m_pPool && m_pos == right.m_pos;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_pPool != right.m_pPool || m_pos != right.m_pos;
		}

	private:
		thisClass* m_pPool;
		Position m_pos;
	};

public:
	SymbolPool(StringPool& pool, const RefPtr<IMemoryAllocatorRef32>& allocator, uint uBins = 17) throw() : m_string_pool(pool), m_allocator(allocator), m_uStart(0)
	{
		m_uBins = _pick_bucket_size(uBins);
	}
	~SymbolPool() throw()
	{
	}

	void Reset(uint uStart = 0) throw()
	{
		m_uStart = uStart;
	}

	bool IsNull() const throw()
	{
		return m_uStart == 0;
	}

	//tools
	StringPool& GetStringPool() throw()
	{
		return m_string_pool;
	}

// Data Block or Node
	void* ToPointer(uint uAddr) const throw()
	{
		assert( uAddr != 0 );
		return (void*)(const_cast<IMemoryAllocatorRef32&>(m_allocator.Deref()).ToPointer(uAddr));
	}

// Node
	const Iterator GetAtPosition(const Position& pos) const throw()
	{
		return Iterator(const_cast<thisClass*>(this), pos.GetAddr());
	}
	Iterator GetAtPosition(const Position& pos) throw()
	{
		return Iterator(this, pos.GetAddr());
	}

	Iterator Find(const ConstStringA& str) const throw()
	{
		assert( !str.IsNull() );
		if( m_uStart == 0 )
			return Iterator(const_cast<thisClass*>(this), 0);  //null
		uint uHash = _calc_hash(ConstArrayHelper::GetInternalPointer(str));
		uint uBin = uHash % m_uBins;
		uint uNode = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)).get_Value();
		//first node
		return Iterator(const_cast<thisClass*>(this), get_first_node(uNode, uHash, str));
	}
	Iterator FindNext(const Iterator& iter) const throw()
	{
		assert( !iter.IsNull() );
		assert( !IsNull() );
		ConstStringA str(iter.GetKey());
		uint uNode = iter.GetNodeAddr();
		const _Node& node = *((const _Node*)ToPointer(uNode));
		uint uHash = node.GetHashCode();
		uNode = node.GetHashNext();
		//next first node
		return Iterator(const_cast<thisClass*>(this), get_first_node(uNode, uHash, str));
	}

// create without find
	Iterator CreateNode(const ConstStringA& str, uint uDataSize, uint uType, uint uLevel, bool bLevelLink, uint& uLevelHead)
	{
		assert( !str.IsNull() );
		assert( uDataSize > 0 );
		//maximum level
		if( uLevel > MAX_LEVEL )
			throw OverflowException();  //throw
		//header
		if( IsNull() ) {
			uint uBytes = SafeOperators::MultiplyThrow((uint)sizeof(uint), m_uBins);  //may throw
			uBytes = SafeOperators::AddThrow(uBytes, (uint)IDX_BUCKET_FIRST);  //may throw
			m_uStart = m_allocator.Deref().Allocate(uBytes);
			if( m_uStart == 0 )
				throw OutOfMemoryException();
			assert( m_uStart == ((uint)sizeof(uint)) );
			//count
			_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).set_Value(0);
			//zero level
			_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ZERO_LEVEL_HEAD).set_Value(0);
			//bucket size
			_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_BUCKET_SIZE).set_Value(m_uBins);
			//bins
			for( uint i = 0; i < m_uBins; i ++ ) {
				_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_BUCKET_FIRST + i * sizeof(uint)).set_Value(0);
			}
		}
		//hash
		uint uHash = _calc_hash(ConstArrayHelper::GetInternalPointer(str));
		uint uBin = uHash % m_uBins;
		uint uFirstNode = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)).get_Value();
		//node
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(_Node), uDataSize);  //may throw
		uint uNode = m_allocator.Deref().Allocate(uBytes);
		if( uNode == 0 )
			throw OutOfMemoryException();
		//key
		uint uKeyAddr;
		{
			uint uStringNode = get_first_node(uFirstNode, uHash, str);
			if( uStringNode == 0 )
				uKeyAddr = m_string_pool.PutString(str);  //may throw
			else
				uKeyAddr = ((_Node*)ToPointer(uStringNode))->GetStringAddr();
		} //end block
		//fill
		_Node& node = *((_Node*)ToPointer(uNode));
		node.Init();
		node.SetStringAddr(uKeyAddr);
		node.SetHashCode(uHash);
		node.SetType(uType);
		node.SetLevel(uLevel | MASK_ANALYSIS);
		//fill list
		node.SetHashNext(uFirstNode);
		_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)).set_Value(uNode);
		if( bLevelLink ) {
			node.SetLevelNext(uLevelHead);
			uLevelHead = uNode;
		}
		//count
		uint uCount = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).get_Value();
		uCount = SafeOperators::AddThrow(uCount, (uint)1);  //may throw
		_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).set_Value(uCount);
		//iterator
		return Iterator(this, uNode);
	}

	uint GetTotalCount() const throw()
	{
		return get_count();
	}

// level
	Iterator GetZeroLevelHead() const throw()
	{
		if( IsNull() )
			return Iterator(const_cast<thisClass*>(this), 0);  //null
		uint uHead = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ZERO_LEVEL_HEAD).get_Value();
		return Iterator(const_cast<thisClass*>(this), uHead);
	}
	void SetZeroLevelHead(uint uHead) throw()
	{
		assert( !IsNull() );
		_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ZERO_LEVEL_HEAD).set_Value(uHead);
	}

	void ReverseLevelLink(uint& uLevelHead) throw()
	{
		uint uNode = uLevelHead;
		uLevelHead = 0;
		while( uNode != 0 ) {
			_Node& node = *((_Node*)ToPointer(uNode));
			uint uNext = node.GetLevelNext();
			node.SetLevelNext(uLevelHead);
			uLevelHead = uNode;
			uNode = uNext;
		}
	}
	void ClearLevelLinkAnalysisFlag(uint uLevelHead) throw()
	{
		Iterator iter(this, uLevelHead);
		while( !iter.IsNull() ) {
			iter.ClearAnalysisFlag();
			iter.MoveLevelNext();
		}
	}

private:
	//pool
	StringPool&  m_string_pool;
	//allocator
	RefPtr<IMemoryAllocatorRef32>  m_allocator;
	uint  m_uStart;  //address in data pool, indicating the hash header ( this value is sizeof(uint) )
	/*
	uCount (the number of nodes)
	uZeroLevelHead (list head of 0 level)
	uBucketSize
	uBucketSize * sizeof(uint)
	*/
	enum { IDX_COUNT = 0, IDX_ZERO_LEVEL_HEAD = sizeof(uint), IDX_BUCKET_SIZE = sizeof(uint) + sizeof(uint), IDX_BUCKET_FIRST = sizeof(uint) + sizeof(uint) + sizeof(uint) };
	//bucket
	uint  m_uBins;   //bucket size

private:
	//count
	uint get_count() const throw()
	{
		if( IsNull() )
			return 0;
		return _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).get_Value();
	}
	// get first node in a bin from uNode
	uint get_first_node(uint uNode, uint uHash, const ConstStringA& str) const throw()
	{
		while( uNode != 0 ) {
			const _Node& node = *((const _Node*)ToPointer(uNode));
			//case sensitive
			if( node.GetHashCode() == uHash && ConstStringCompareTrait<ConstStringA>::IsEQ(m_string_pool.GetString(node.GetStringAddr()), str) ) {
				break;
			}
			uNode = node.GetHashNext();
		}
		return uNode;
	}

	// hash function -- PJW (P. J. Weinberger)
	static uint _calc_hash(const CharA* sz) throw()
	{
		uint uBitsInUnsignedInt = (uint)(sizeof(uint) * 8);
		uint uThreeQuarters     = (uint)((uBitsInUnsignedInt * 3) / 4);
		uint uOneEighth         = (uint)(uBitsInUnsignedInt / 8);
		uint uHighBits          = (uint)(0xFFFFFFFF) << (uBitsInUnsignedInt - uOneEighth);
		uint uHash              = 0;
		uint uTest              = 0;
		while( *sz != 0 ) {
			uHash = (uHash << uOneEighth) + (uint)(*sz ++);
			if( (uTest = uHash & uHighBits) != 0 ) {
				uHash = ((uHash ^ (uTest >> uThreeQuarters)) & (~uHighBits));
			}
		}
		return (uHash & 0x7FFFFFFF);
	}
	static uint _calc_hash_case_ignore(const CharA* sz) throw()
	{
		uint uBitsInUnsignedInt = (uint)(sizeof(uint) * 8);
		uint uThreeQuarters     = (uint)((uBitsInUnsignedInt * 3) / 4);
		uint uOneEighth         = (uint)(uBitsInUnsignedInt / 8);
		uint uHighBits          = (uint)(0xFFFFFFFF) << (uBitsInUnsignedInt - uOneEighth);
		uint uHash              = 0;
		uint uTest              = 0;
		while( *sz != 0 ) {
			uHash = (uHash << uOneEighth) + (uint)char_upper(*sz ++);
			if( (uTest = uHash & uHighBits) != 0 ) {
				uHash = ((uHash ^ (uTest >> uThreeQuarters)) & (~uHighBits));
			}
		}
		return (uHash & 0x7FFFFFFF);
	}
	//bucket size
	static uint _pick_bucket_size(uint uSize) throw()
	{
		static const uint c_Primes[] = {
			17, 23, 29, 37, 41, 53, 67, 83, 103, 131, 163, 211, 257, 331, 409, 521, 647, 821,
			1031, 1291, 1627, 2053, 2591, 3251, 4099, 5167, 6521, 8209, 10331,
			13007, 16411, 20663, 26017, 32771, 41299, 52021, 65537, 82571, 104033,
			131101, 165161, 208067, 262147, 330287, 416147, 524309, 660563,
			832291, 1048583, 1321139, 1664543, 2097169, 2642257, 3329023, 4194319,
			5284493, 6658049, 8388617, 10568993, 13316089, Limits<uint>::Max
		};
		int iPrime = 0;
		while( uSize > c_Primes[iPrime] )
			iPrime ++;
		return c_Primes[iPrime];
	}

private:
	//noncopyable
	SymbolPool(const SymbolPool&) throw();
	SymbolPool& operator=(const SymbolPool&) throw();
};

// Abstract Syntax Tree

// AstTree

class AstTree
{
private:
	typedef AstTree  thisClass;

#pragma pack(push, 1)

	//Node
	class _Node
	{
	public:
		_Node() throw()
		{
		}
		~_Node() throw()
		{
		}
		void Init() throw()
		{
			uParent.set_Value(0);
			uChild.set_Value(0);
			uNext.set_Value(0);
			uType.set_Value(0);
		}
		//properties
		uint GetParent() const throw()
		{
			return uParent.get_Value();
		}
		void SetParent(uint uValue) throw()
		{
			uParent.set_Value(uValue);
		}
		uint GetChild() const throw()
		{
			return uChild.get_Value();
		}
		void SetChild(uint uValue) throw()
		{
			uChild.set_Value(uValue);
		}
		uint GetNext() const throw()
		{
			return uNext.get_Value();
		}
		void SetNext(uint uValue) throw()
		{
			uNext.set_Value(uValue);
		}
		//type
		uint GetType() const throw()
		{
			return uType.get_Value();
		}
		void SetType(uint uValue) throw()
		{
			uType.set_Value(uValue);
		}

	private:
		BeType<uint> uParent;
		BeType<uint> uChild;
		BeType<uint> uNext;
		BeType<uint> uType;  //AST node type
		//data block

	private:
		_Node(const _Node&) throw();
		_Node& operator=(const _Node&) throw();
	};

#pragma pack(pop)

public:
	// Position
	class Position
	{
	public:
		explicit Position(uint uNode = 0) throw() : m_uNode(uNode)
		{
		}
		Position(const Position& src) throw() : m_uNode(src.m_uNode)
		{
		}
		~Position() throw()
		{
		}
		Position& operator=(const Position& src) throw()
		{
			if( this != &src ) {
				m_uNode = src.m_uNode;
			}
			return *this;
		}

		bool IsNull() const throw()
		{
			return m_uNode == 0;
		}

		uint GetAddr() const throw()
		{
			return m_uNode;
		}
		void SetAddr(uint uNode) throw()
		{
			m_uNode = uNode;
		}

		bool operator==(const Position& right) const throw()
		{
			return m_uNode == right.m_uNode;
		}
		bool operator!=(const Position& right) const throw()
		{
			return m_uNode != right.m_uNode;
		}

	private:
		uint m_uNode;  //node address
	};

	// Iterator
	class Iterator
	{
	public:
		Iterator() throw() : m_pTree(NULL), m_pos()
		{
		}
		explicit Iterator(thisClass* p, uint uNode = 0) throw() : m_pTree(p), m_pos(uNode)
		{
		}
		Iterator(const Iterator& src) throw() : m_pTree(src.m_pTree), m_pos(src.m_pos)
		{
		}
		~Iterator() throw()
		{
		}

		Iterator& operator=(const Iterator& src) throw()
		{
			m_pTree = src.m_pTree;
			m_pos   = src.m_pos;
			return *this;
		}

		const Position GetPosition() const throw()
		{
			return m_pos;
		}
		Position GetPosition() throw()
		{
			return m_pos;
		}

		bool IsNull() const throw()
		{
			return m_pos.IsNull();
		}

		uint GetNodeAddr() const throw()
		{
			return m_pos.GetAddr();
		}

		//properties
		uint GetType() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pTree->ToPointer(GetNodeAddr()));
			return pNode->GetType();
		}
		void SetType(uint uType) throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pTree->ToPointer(GetNodeAddr()));
			pNode->SetType(uType);
		}

		//user data
		void* GetDataPointer() const throw()
		{
			assert( !IsNull() );
			return m_pTree->ToPointer(GetNodeAddr() + sizeof(_Node));
		}
		template <typename T>
		const T& GetData() const throw()
		{
			return *((T*)GetDataPointer());
		}
		template <typename T>
		T& GetData() throw()
		{
			return *((T*)GetDataPointer());
		}

		//methods
		void MoveParent() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pTree->ToPointer(GetNodeAddr()));
			m_pos.SetAddr(pNode->GetParent());
		}
		void MoveChild() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pTree->ToPointer(GetNodeAddr()));
			m_pos.SetAddr(pNode->GetChild());
		}
		void MoveNext() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)(m_pTree->ToPointer(GetNodeAddr()));
			m_pos.SetAddr(pNode->GetNext());
		}

		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_pTree == right.m_pTree && m_pos == right.m_pos;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_pTree != right.m_pTree || m_pos != right.m_pos;
		}

	private:
		thisClass* m_pTree;
		Position   m_pos;
	};

public:
	explicit AstTree(const RefPtr<IMemoryAllocatorRef32>& allocator, uint uStart = 0) throw() : m_allocator(allocator), m_uStart(uStart)
	{
	}
	~AstTree() throw()
	{
	}

	void Reset() throw()
	{
		m_uStart = 0;
	}

	bool IsNull() const throw()
	{
		return m_uStart == 0;
	}

//data block or node
	void* ToPointer(uint uAddr) const throw()
	{
		assert( uAddr != 0 );
		return (void*)(const_cast<IMemoryAllocatorRef32&>(m_allocator.Deref()).ToPointer(uAddr));
	}

//header
	uint GetStart() const throw()
	{
		return m_uStart;
	}
	uint GetCount() const throw()
	{
		return get_count();
	}

//nodes
	Iterator GetNullIterator() const throw()
	{
		return Iterator(const_cast<thisClass*>(this), 0);  //null
	}
	Iterator GetRoot() const throw()
	{
		return Iterator(const_cast<thisClass*>(this), get_root());
	}

	const Iterator GetAtPosition(const Position& pos) const throw()
	{
		return Iterator(const_cast<thisClass*>(this), pos.GetAddr());
	}
	Iterator GetAtPosition(const Position& pos) throw()
	{
		return Iterator(this, pos.GetAddr());
	}

	//create node
	Iterator CreateNode(uint uDataSize, uint uType)
	{
		uint uRoot;
		uint uNode = create_node(uDataSize, uType, uRoot);  //may throw
		//iterator
		return Iterator(this, uNode);
	}

	//set node
	void SetParent(const Iterator& iter, const Iterator& iterParent) throw()
	{
		uint uNode = iter.GetNodeAddr();
		assert( uNode != 0 );
		uint uParent = iterParent.GetNodeAddr();
		set_parent(uNode, uParent, get_root());
	}
	void SetChild(const Iterator& iter, const Iterator& iterChild) throw()
	{
		uint uNode = iter.GetNodeAddr();
		assert( uNode != 0 );
		set_child(uNode, iterChild.GetNodeAddr());
	}
	void SetNext(const Iterator& iter, const Iterator& iterNext) throw()
	{
		uint uNode = iter.GetNodeAddr();
		assert( uNode != 0 );
		set_next(uNode, iterNext.GetNodeAddr());
	}

	//insert
	Iterator Insert(const Iterator& iterParent, const Iterator& iterAfter, uint uDataSize, uint uType)
	{
		//create
		uint uRoot;
		uint uNode = create_node(uDataSize, uType, uRoot);  //may throw
		//list
		uint uParent = iterParent.GetNodeAddr();
		set_parent(uNode, uParent, uRoot);
		uint uAfter = iterAfter.GetNodeAddr();
		if( uAfter == 0 ) {
			uint uChild = get_child(uParent);
			if( uChild != 0 )
				set_next(uNode, uChild);
			set_child(uParent, uNode);
		}
		else {
			assert( get_parent(uAfter) == uParent );
			set_next(uNode, get_next(uAfter));
			set_next(uAfter, uNode);
		} //end if
		//iterator
		return Iterator(this, uNode);
	}

private:
	//node
	uint create_node(uint uDataSize, uint uType, uint& uRoot)
	{
		if( IsNull() ) {
			//header
			m_uStart = m_allocator.Deref().Allocate(SIZE_HEADER);
			if( m_uStart == 0 )
				throw OutOfMemoryException();
			_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).set_Value(0);
			_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ROOT).set_Value(0);
		}
		//root
		uRoot = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ROOT).get_Value();
		if( uRoot == 0 ) {
			//root has not any data
			uRoot = m_allocator.Deref().Allocate(sizeof(_Node));
			if( uRoot == 0 )
				throw OutOfMemoryException();
			_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ROOT).set_Value(uRoot);
		}
		//allocate
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(_Node), uDataSize);  //may throw
		uint uNode = m_allocator.Deref().Allocate(uBytes);
		if( uNode == 0 )
			throw OutOfMemoryException();
		//node
		_Node& node = *((_Node*)ToPointer(uNode));
		node.Init();
		node.SetType(uType);
		//count
		uint uCount = _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).get_Value();
		uCount = SafeOperators::AddThrow(uCount, (uint)1);  //may throw
		_RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).set_Value(uCount);
		return uNode;
	}
	void set_parent(uint uNode, uint& uParent, uint uRoot) throw()
	{
		assert( uRoot != 0 );
		if( uParent == 0 )
			uParent = uRoot;
		_Node& node = *((_Node*)ToPointer(uNode));
		node.SetParent(uParent);
	}
	void set_child(uint uNode, uint uChild) throw()
	{
		_Node& node = *((_Node*)ToPointer(uNode));
		node.SetChild(uChild);
	}
	void set_next(uint uNode, uint uNext) throw()
	{
		_Node& node = *((_Node*)ToPointer(uNode));
		node.SetNext(uNext);
	}
	uint get_parent(uint uNode) const throw()
	{
		_Node& node = *((_Node*)ToPointer(uNode));
		return node.GetParent();
	}
	uint get_child(uint uNode) const throw()
	{
		_Node& node = *((_Node*)ToPointer(uNode));
		return node.GetChild();
	}
	uint get_next(uint uNode) const throw()
	{
		_Node& node = *((_Node*)ToPointer(uNode));
		return node.GetNext();
	}

	//count
	uint get_count() const throw()
	{
		if( IsNull() )
			return 0;
		return _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_COUNT).get_Value();
	}
	//root
	uint get_root() const throw()
	{
		if( IsNull() )
			return 0;
		return _RefAllocatorHelper::ToObject<BeType<uint>>(m_allocator, m_uStart + IDX_ROOT).get_Value();
	}

private:
	RefPtr<IMemoryAllocatorRef32> m_allocator;  //allocator for all AST trees
	uint m_uStart;  //header block
	/*
	count
	root
	*/
	enum { IDX_COUNT = 0, IDX_ROOT = sizeof(uint), SIZE_HEADER = sizeof(uint) + sizeof(uint) };

private:
	//noncopyable
	AstTree(const AstTree&) throw();
	AstTree& operator=(const AstTree&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__DATA_COLL_H__
////////////////////////////////////////////////////////////////////////////////
