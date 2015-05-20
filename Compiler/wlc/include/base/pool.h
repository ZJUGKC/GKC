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

inline uint _cpl_process_byte_order(uint v) throw()
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
	~DataPoolAllocator() throw()
	{
	}

	//return : the index of pool array
	uint Allocate(uint uBytes)  //may throw
	{
		assert( uBytes > 0 );
		if( !m_bInit ) {
			m_arr = SharedArrayHelper::MakeSharedArray<byte>(MemoryHelper::GetCrtMemoryManager());  //may throw
			m_bInit = true;
		}
		uintptr uCount = m_arr.GetCount();
		//overflow
		if( uCount > (uintptr)(Limits<uint>::Max) )
			throw OverflowException();  //throw
		//add a room for count number
		if( uCount < (uintptr)sizeof(uint) )
			uCount = (uintptr)sizeof(uint);
		//align
		uBytes = AlignHelper::RoundUpThrow(uBytes, (uint)sizeof(uint));  //may throw
		//new size
		uint uNew = SafeOperators::AddThrow((uint)uCount, uBytes);  //may throw
		m_arr.SetCount((uintptr)uNew, 0);  //may throw
		//fill the count number
		*((uint*)ToPtr(0)) = _cpl_process_byte_order(uNew - ((uint)sizeof(uint)));
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
		return _cpl_process_byte_order(*((uint*)ToPtr(0)));
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
		uint uLen = _cpl_process_byte_order(m_alloc.ToObject<uint>(uIndex));
		return ConstStringA((CharA*)m_alloc.ToPtr(uIndex + sizeof(uint)), uLen);
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
		uint uIndex = m_alloc.Allocate(uBytes);  //may throw
		//length
		m_alloc.ToObject<uint>(uIndex) = _cpl_process_byte_order((uint)uLen);
		//copy
		mem_copy(ConstHelper::GetInternalPointer(str), (uLen + 1) * sizeof(CharA), m_alloc.ToPtr(uIndex + sizeof(uint)));
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
	typedef SymbolPool  thisClass;

	class _Node
	{
	public:
		_Node() throw()
		{
		}
		~_Node() throw()
		{
		}
		//These are not necessary
		//_Node(const _Node& src) throw()
		//_Node& operator=(const _Node& src) throw()
		void Init() throw()
		{
			uHashNext = uLevelNext = 0;
			uStringIndex = uHashCode = 0;
			uType = uLevel = 0;
		}
		//properties
		uint GetHashNext() const throw()
		{
			return _cpl_process_byte_order(uHashNext);
		}
		void SetHashNext(uint uValue) throw()
		{
			uHashNext = _cpl_process_byte_order(uValue);
		}
		uint GetLevelNext() const throw()
		{
			return _cpl_process_byte_order(uLevelNext);
		}
		void SetLevelNext(uint uValue) throw()
		{
			uLevelNext = _cpl_process_byte_order(uValue);
		}
		uint GetStringIndex() const throw()
		{
			return _cpl_process_byte_order(uStringIndex);
		}
		void SetStringIndex(uint uValue) throw()
		{
			uStringIndex = _cpl_process_byte_order(uValue);
		}
		uint GetHashCode() const throw()
		{
			return _cpl_process_byte_order(uHashCode);
		}
		void SetHashCode(uint uValue) throw()
		{
			uHashCode = _cpl_process_byte_order(uValue);
		}
		uint GetType() const throw()
		{
			return _cpl_process_byte_order(uType);
		}
		void SetType(uint uValue) throw()
		{
			uType = _cpl_process_byte_order(uValue);
		}
		uint GetLevel() const throw()
		{
			return _cpl_process_byte_order(uLevel);
		}
		void SetLevel(uint uValue) throw()
		{
			uLevel = _cpl_process_byte_order(uValue);
		}

	private:
		uint uHashNext;     //the next node of hash list
		uint uLevelNext;    //the next node of current level
		uint uStringIndex;  //index for string (key)
		uint uHashCode;     //the original hash code
		uint uType;   //data type
		uint uLevel;  //level (30 bits) and flags
		// Data block is located after the node
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
		explicit Iterator(thisClass* p, uint uNode = 0) throw() : m_pPool(p), m_uNode(uNode)
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

		bool IsNull() const throw()
		{
			return m_uNode == 0;
		}

		//properties
		ConstStringA GetKey() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return m_pPool->GetStringPool().GetString(pNode->GetStringIndex());
		}
		uint GetNodeIndex() const throw()
		{
			return m_uNode;
		}
		uint GetType() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return pNode->GetType();
		}
		void SetType(uint uType) throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			pNode->SetType(uType);
		}
		uint GetLevel() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return (pNode->GetLevel()) & MASK_LEVEL;
		}
		bool IsAnalysis() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			return ((pNode->GetLevel()) & MASK_ANALYSIS) != 0;
		}
		//clear analysis flag
		void ClearAnalysisFlag() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			uint uLevel = pNode->GetLevel();
			uLevel &= (~MASK_ANALYSIS);
			pNode->SetLevel(uLevel);
		}

		//user data
		void* GetDataPtr() const throw()
		{
			assert( !IsNull() );
			//after node
			return m_pPool->ToPtr(m_uNode + sizeof(_Node));
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

		//methods
		void MoveLevelNext() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pPool->ToPtr(m_uNode);
			m_uNode = pNode->GetLevelNext();
		}

	private:
		thisClass* m_pPool;
		uint m_uNode;  //index for node
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
		assert( uIndex != 0 );
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
		if( m_uStart == 0 )
			return Iterator(const_cast<thisClass*>(this), 0);  //null
		uint uHash = _calc_hash(ConstHelper::GetInternalPointer(str));
		uint uBin = uHash % m_uBins;
		uint uNode = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)));
		//first node
		return Iterator(const_cast<thisClass*>(this), get_first_node(uNode, uHash, str));
	}
	Iterator FindNext(const Iterator& iter) const throw()
	{
		assert( !iter.IsNull() );
		assert( !IsNull() );
		ConstStringA str(iter.GetKey());
		uint uNode = iter.GetNodeIndex();
		const _Node& node = m_alloc.ToObject<_Node>(uNode);
		uint uHash = node.GetHashCode();
		uNode = node.GetHashNext();
		//next first node
		return Iterator(const_cast<thisClass*>(this), get_first_node(uNode, uHash, str));
	}

// create
	Iterator CreateNode(const ConstStringA& str, uint uDataSize, uint uType, uint uLevel, uint& uLevelHead)
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
			m_uStart = m_alloc.Allocate(uBytes);  //may throw
			assert( m_uStart == ((uint)sizeof(uint)) );
			//count
			m_alloc.ToObject<uint>(m_uStart + IDX_COUNT) = 0;
			//zero level
			m_alloc.ToObject<uint>(m_uStart + IDX_ZERO_LEVEL_HEAD) = 0;
			//bucket size
			m_alloc.ToObject<uint>(m_uStart + IDX_BUCKET_SIZE) = _cpl_process_byte_order(m_uBins);
			//bins
			for( uint i = 0; i < m_uBins; i ++ ) {
				m_alloc.ToObject<uint>(m_uStart + IDX_BUCKET_FIRST + i * sizeof(uint)) = 0;
			}
		}
		//hash
		uint uHash = _calc_hash(ConstHelper::GetInternalPointer(str));
		uint uBin = uHash % m_uBins;
		uint uFirstNode = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)));
		//node
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(_Node), uDataSize);  //may throw
		uint uNode = m_alloc.Allocate(uBytes);  //may throw
		//key
		uint uIndex = m_string_pool.PutString(str);  //may throw
		//fill
		_Node& node = m_alloc.ToObject<_Node>(uNode);
		node.Init();
		node.SetStringIndex(uIndex);
		node.SetHashCode(uHash);
		node.SetType(uType);
		node.SetLevel(uLevel | MASK_ANALYSIS);
		//fill list
		node.SetHashNext(uFirstNode);
		m_alloc.ToObject<uint>(m_uStart + IDX_BUCKET_FIRST + uBin * sizeof(uint)) = _cpl_process_byte_order(uNode);
		node.SetLevelNext(uLevelHead);
		uLevelHead = uNode;
		//count
		uIndex = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_COUNT));
		m_alloc.ToObject<uint>(m_uStart + IDX_COUNT) = _cpl_process_byte_order(uIndex + 1);
		//iterator
		return Iterator(this, uNode);
	}

// level
	Iterator GetZeroLevelHead() const throw()
	{
		if( IsNull() )
			return Iterator(const_cast<thisClass*>(this), 0);  //null
		uint uHead = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_ZERO_LEVEL_HEAD));
		return Iterator(const_cast<thisClass*>(this), uHead);
	}
	void SetZeroLevelHead(uint uHead) throw()
	{
		assert( !IsNull() );
		m_alloc.ToObject<uint>(m_uStart + IDX_ZERO_LEVEL_HEAD) = _cpl_process_byte_order(uHead);
	}

private:
	//pool
	StringPool&  m_string_pool;
	//allocator
	DataPoolAllocator  m_alloc;
	uint  m_uStart;  //index in data pool, indicating the hash header ( this value is sizeof(uint) )
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
		return _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_COUNT));
	}
	// get first node in a bin from uNode
	uint get_first_node(uint uNode, uint uHash, const ConstStringA& str) const throw()
	{
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

// AstTree

class AstTree
{
private:
	typedef AstTree  thisClass;

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
		//assignment is not necessary
		void Init() throw()
		{
			uParent = uChild = uNext = 0;
			uType = 0;
		}
		//properties
		uint GetParent() const throw()
		{
			return _cpl_process_byte_order(uParent);
		}
		void SetParent(uint uValue) throw()
		{
			uParent = _cpl_process_byte_order(uValue);
		}
		uint GetChild() const throw()
		{
			return _cpl_process_byte_order(uChild);
		}
		void SetChild(uint uValue) throw()
		{
			uChild = _cpl_process_byte_order(uValue);
		}
		uint GetNext() const throw()
		{
			return _cpl_process_byte_order(uNext);
		}
		void SetNext(uint uValue) throw()
		{
			uNext = _cpl_process_byte_order(uValue);
		}
		//type
		uint GetType() const throw()
		{
			return _cpl_process_byte_order(uType);
		}
		void SetType(uint uValue) throw()
		{
			uType = _cpl_process_byte_order(uValue);
		}

	private:
		uint uParent;
		uint uChild;
		uint uNext;
		uint uType;  //AST node type
		//data block
	};

public:
	// Iterator
	class Iterator
	{
	public:
		explicit Iterator(thisClass* p, uint uNode = 0) throw() : m_pTree(p), m_uNode(uNode)
		{
		}
		Iterator(const Iterator& src) throw() : m_pTree(src.m_pTree), m_uNode(src.m_uNode)
		{
		}
		~Iterator() throw()
		{
		}

		Iterator& operator=(const Iterator& src) throw()
		{
			if( this != &src ) {
				m_pTree = src.m_pTree;
				m_uNode = src.m_uNode;
			}
			return *this;
		}

		bool IsNull() const throw()
		{
			return m_uNode == 0;
		}

		uint GetNodeIndex() const throw()
		{
			return m_uNode;
		}

		//properties
		uint GetType() const throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pTree->ToPtr(m_uNode);
			return pNode->GetType();
		}
		void SetType(uint uType) throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pTree->ToPtr(m_uNode);
			pNode->SetType(uType);
		}

		//user data
		void* GetDataPtr() const throw()
		{
			assert( !IsNull() );
			return m_pTree->ToPtr(m_uNode + sizeof(_Node));
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

		//methods
		void MoveParent() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pTree->ToPtr(m_uNode);
			m_uNode = pNode->GetParent();
		}
		void MoveChild() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pTree->ToPtr(m_uNode);
			m_uNode = pNode->GetChild();
		}
		void MoveNext() throw()
		{
			assert( !IsNull() );
			_Node* pNode = (_Node*)m_pTree->ToPtr(m_uNode);
			m_uNode = pNode->GetNext();
		}

	private:
		thisClass* m_pTree;
		uint m_uNode;  //node index
	};

public:
	explicit AstTree(DataPoolAllocator& pool, uint uStart = 0) throw() : m_alloc(pool), m_uStart(uStart)
	{
	}
	~AstTree() throw()
	{
	}

	bool IsNull() const throw()
	{
		return m_uStart == 0;
	}

//data block or node
	void* ToPtr(uint uIndex) const throw()
	{
		assert( uIndex != 0 );
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

//header
	uint GetStart() const throw()
	{
		return m_uStart;
	}

//nodes
	Iterator GetNullIterator() const throw()
	{
		return Iterator(const_cast<thisClass*>(this), 0);  //null
	}
	Iterator GetRoot() const throw()
	{
		if( IsNull() )
			return GetNullIterator();
		uint uRoot = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_ROOT));
		return Iterator(const_cast<thisClass*>(this), uRoot);
	}
	Iterator Insert(const Iterator& iterParent, const Iterator& iterAfter, uint uDataSize, uint uType)
	{
		if( IsNull() ) {
			//header
			m_uStart = m_alloc.Allocate(SIZE_HEADER);  //may throw
			m_alloc.ToObject<uint>(m_uStart + IDX_COUNT) = 0;
			m_alloc.ToObject<uint>(m_uStart + IDX_ROOT) = 0;
		}
		//root
		uint uRoot = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_ROOT));
		if( uRoot == 0 ) {
			//root has not any data
			uRoot = m_alloc.Allocate(sizeof(_Node));  //may throw
			m_alloc.ToObject<uint>(m_uStart + IDX_ROOT) = _cpl_process_byte_order(uRoot);
		}
		//allocate
		uint uBytes = SafeOperators::AddThrow((uint)sizeof(_Node), uDataSize);  //may throw
		uint uNode = m_alloc.Allocate(uBytes);  //may throw
		//node
		_Node& node = m_alloc.ToObject<_Node>(uNode);
		node.Init();
		node.SetType(uType);
		//list
		uint uParent = iterParent.IsNull() ? uRoot : iterParent.GetNodeIndex();
		_Node& nodeParent = m_alloc.ToObject<_Node>(uParent);
		node.SetParent(uParent);
		if( iterAfter.IsNull() ) {
			//first child
			uint uChild = nodeParent.GetChild();
			if( uChild != 0 ) {
				node.SetNext(uChild);
			}
			nodeParent.SetChild(uNode);
		}
		else {
			uint uAfter = iterAfter.GetNodeIndex();
			_Node& nodeAfter = m_alloc.ToObject<_Node>(uAfter);
			assert( nodeAfter.GetParent() == uParent );
			node.SetNext(nodeAfter.GetNext());
			nodeAfter.SetNext(uNode);
		} //end if
		//count
		uint uIndex = _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_COUNT));
		m_alloc.ToObject<uint>(m_uStart + IDX_COUNT) = _cpl_process_byte_order(uIndex + 1);
		//iterator
		return Iterator(this, uNode);
	}

private:
	DataPoolAllocator& m_alloc;  //a pool for all AST trees
	uint m_uStart;  //header block
	/*
	count
	root
	*/
	enum { IDX_COUNT = 0, IDX_ROOT = sizeof(uint), SIZE_HEADER = sizeof(uint) + sizeof(uint) };

private:
	//count
	uint get_count() const throw()
	{
		if( IsNull() )
			return 0;
		return _cpl_process_byte_order(m_alloc.ToObject<uint>(m_uStart + IDX_COUNT));
	}

private:
	//noncopyable
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__POOL_H__
////////////////////////////////////////////////////////////////////////////////
