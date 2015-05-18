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

// Storage : Big Endian

// functions

inline uint _process_byte_order(uint v) throw()
{
	if( !ByteOrderHelper::IsBigEndianHost() ) {
		v = ByteOrderHelper::Swap(v);
	}
	return v;
}

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
		//align
		uBytes = AlignHelper::RoundUpThrow(uBytes, (uint)sizeof(uint));
		//new size
		uint uNew = SafeOperators::AddThrow((uint)uCount, uBytes);
		m_arr.SetCount((uintptr)uNew, 0);
		//fill the count number
		*((uint*)ToPtr(0)) = _process_byte_order(uNew - ((uint)sizeof(uint)));
		//return a nonzero value
		return (uint)uCount;
	}
	//get the pointer
	void* ToPtr(uint index) const throw()
	{
		assert( index < (uint)m_arr.GetCount() );
		return SharedArrayHelper::GetInternalPointer(m_arr) + index;
	}
	template <typename T>
	const T& ToObject(uint index) const throw()
	{
		return *((T*)ToPtr(index));
	}
	template <typename T>
	T& ToObject(uint index) throw()
	{
		return *((T*)ToPtr(index));
	}
	//get the size
	uint GetSize() const throw()
	{
		uintptr uCount = m_arr.GetCount();
		if( uCount == 0 )
			return 0;
		assert( uCount >= (uintptr)sizeof(uint) );
		return _process_byte_order(*((uint*)ToPtr(0)));
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

	ConstStringA GetString(uint uIndex) const throw()
	{
		uint uLen = _process_byte_order(m_alloc.ToObject<uint>(uIndex));
		return ConstStringA((CharA*)m_alloc.ToPtr(uIndex + sizeof(uint)), uLen);
	}

	//nonempty string
	uint PutString(const StringA& str)
	{
		uintptr uLen = str.GetLength();
		assert( uLen > 0 );
		//overflow
		if( uLen >= (uintptr)(Limits<uint>::Max) )
			throw OverflowException();
		//allocate
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(uint), (uint)((uLen + 1) * sizeof(CharA)));
		uint uIndex = m_alloc.Allocate(uBytes);
		//length
		m_alloc.ToObject<uint>(uIndex) = _process_byte_order((uint)uLen);
		//copy
		mem_copy(SharedArrayHelper::GetInternalPointer(str), (uLen + 1) * sizeof(CharA), m_alloc.ToPtr(uIndex + sizeof(uint)));
		return uIndex;
	}

private:
	DataPoolAllocator m_alloc;

private:
	//noncopyable
};

// SymbolPool
//   All symbols are in a hash table with multi-key. The list header of level 0 must be defined in this class. 

class SymbolPool
{
private:
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
			uHashNext = uLevelNext = 0;
			uStringIndex = uHashCode = 0;
			uData = uLevel = 0;
		}
		//properties
		uint GetHashNext() const throw()
		{
			return _process_byte_order(uHashNext);
		}
		void SetHashNext(uint uValue) throw()
		{
			uHashNext = _process_byte_order(uValue);
		}
		uint GetLevelNext() const throw()
		{
			return _process_byte_order(uLevelNext);
		}
		void SetLevelNext(uint uValue) throw()
		{
			uLevelNext = _process_byte_order(uValue);
		}
		uint GetStringIndex() const throw()
		{
			return _process_byte_order(uStringIndex);
		}
		void SetStringIndex(uint uValue) throw()
		{
			uStringIndex = _process_byte_order(uValue);
		}
		uint GetHashCode() const throw()
		{
			return _process_byte_order(uHashCode);
		}
		void SetHashCode(uint uValue) throw()
		{
			uHashCode = _process_byte_order(uValue);
		}
		uint GetData() const throw()
		{
			return _process_byte_order(uData);
		}
		void SetData(uint uValue) throw()
		{
			uData = _process_byte_order(uValue);
		}
		uint GetLevel() const throw()
		{
			return _process_byte_order(uLevel);
		}
		void SetLevel(uint uValue) throw()
		{
			uLevel = _process_byte_order(uValue);
		}

	private:
		uint uHashNext;     //the next node of hash list
		uint uLevelNext;    //the next node of current level
		uint uStringIndex;  //index for string (key)
		uint uHashCode;     //the original hash code
		uint uData;   //data block
		uint uLevel;  //level (30 bits) and flags
	};

public:
	//constants
	enum { MASK_LEVEL = 0x3FFFFFFF, MAX_LEVEL = MASK_LEVEL,
		MASK_ANALYSIS = 0x40000000, RSHIFT_BITS = 30  //bit 30 in _Node::uLevel, analysis flag, indicating the analysis process or not
	};

	//Iterator
	class Iterator
	{
	public:
		explicit Iterator(SymbolPool* p, uint uNode = 0) throw() : m_pPool(p), m_uNode(uNode)
		{
		}
		Iterator(const Iterator& src) throw() : m_pPool(src.m_pPool), m_uNode(src.m_uNode)
		{
		}
		~Iterator() throw()
		{
		}

		Iterator& operator=(const Iterator& src) throw()
		{
			if( this != &src ) {
				m_pPool = src.m_pPool;
				m_uNode = src.m_uNode;
			}
			return *this;
		}

		//properties
		ConstStringA GetKey() const throw()
		{
			assert( m_uNode != 0 );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return m_pPool->GetStringPool().GetString(pNode->GetStringIndex());
		}
		void* GetDataPtr() const throw()
		{
			assert( m_uNode != 0 );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return m_pPool->ToPtr(pNode->GetData());
		}
		template <typename T>
		const T& GetData() const throw()
		{
			return *((T*)GetDataPtr());
		}
		template <typename T>
		T& GetData() throw()
		{
			return *((T*)GetDataPtr());
		}
		uint GetLevel() const throw()
		{
			assert( m_uNode != 0 );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return (pNode->GetLevel()) & MASK_LEVEL;
		}
		bool IsAnalysis() const throw()
		{
			assert( m_uNode != 0 );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return ((pNode->GetLevel()) & MASK_ANALYSIS) != 0;
		}
		//clear analysis flag
		void ClearAnalysisFlag() throw()
		{
			assert( m_uNode != 0 );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			uint uLevel = pNode->GetLevel();
			uLevel &= (~MASK_ANALYSIS);
			pNode->SetLevel(uLevel);
		}

		//methods
		void MoveLevelNext() throw()
		{
			assert( m_uNode != 0 );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			m_uNode = pNode->GetLevelNext();
		}

		bool IsNull() const throw()
		{
			return m_uNode == 0;
		}

	private:
		uint m_uNode;  //index for node
		SymbolPool* m_pPool;
	};

public:
	explicit SymbolPool(StringPool& pool, uint uBins = 17) throw() : m_string_pool(pool), m_uStart(0)
	{
		m_uBins = _pick_bucket_size(uBins);
	}
	~SymbolPool() throw()
	{
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
	void* ToPtr(uint uIndex) const throw()
	{
		return m_alloc.ToPtr(uIndex);
	}
	template <typename T>
	const T& ToObject(uint uIndex) const throw()
	{
		return *((T*)ToPtr(uIndex));
	}
	template <typename T>
	T& ToObject(uint uIndex) throw()
	{
		return *((T*)ToPtr(uIndex));
	}

// Node
	Iterator Find(const ConstStringA& str) const throw()
	{
		assert( !str.IsNull() );
		uint uHash = _calc_hash(ConstHelper::GetInternalPointer(str));
		uint uBin = uHash % m_uBins;
		return Iterator(const_cast<SymbolPool*>(this), get_first_node(uBin, uHash, str));
	}
	//FindNext
	//CreateNode
	//uint GetZeroLevelHead
	//Iterator GetLevelHead

private:
	DataPoolAllocator  m_alloc;  //allocator
	uint  m_uStart;  //index in data pool, indicating the hash header ( this value is sizeof(uint) )
	/*
	uCount (the number of nodes)
	uBucketSize
	uBucketSize * sizeof(uint)
	*/
	enum { IDX_COUNT = 0, IDX_BUCKET_SIZE = sizeof(uint), IDX_BUCKET_FIRST = sizeof(uint) + sizeof(uint) };
	//bucket
	uint  m_uBins;   //bucket size
	//pool
	StringPool&  m_string_pool;

private:
	// get first node in a bin
	uint get_first_node(uint uBin, uint uHash, const ConstStringA& str) const throw()
	{
		if( m_uStart == 0 )
			return 0;
		uint uNode = _process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)));
		while( uNode != 0 ) {
			const _Node& node = m_alloc.ToObject<_Node>(uNode);
			//case sensitive
			if( node.GetHashCode() == uHash && ConstStringCompareTrait<ConstStringA>::IsEQ(m_string_pool.GetString(node.GetStringIndex()), str) ) {
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
};

// Abstract Syntax Tree

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__POOL_H__
////////////////////////////////////////////////////////////////////////////////
