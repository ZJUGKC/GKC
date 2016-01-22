/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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
This file contains basic classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_BASE_H__
#define __GKC_BASE_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_DEF_H__
	#error GkcBase.h requires GkcDef.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//functions

//Swap
template <typename T>
inline void Swap(T& t1, T& t2)
{
	assert( &t1 != &t2 );
	T tmp = static_cast<T&&>(t1);
	t1 = static_cast<T&&>(t2);
	t2 = static_cast<T&&>(tmp);
}

template <>
inline void Swap<int>(int& t1, int& t2)
{
	assert( &t1 != &t2 );
	t1 ^= t2 ^= t1 ^= t2;
}

// classes

//------------------------------------------------------------------------------
// Memory

// AlignHelper

class AlignHelper
{
public:
	//T : Integer
	//uAlign : it must be a value as 2^N.
	template <typename T>
	static T RoundUp(IN T n, IN uint uAlign) throw()
	{
		assert( uAlign > 0 );
		//overflow is not checked
		return T( (n + (uAlign - 1)) & ~(T(uAlign) - 1) );
	}
	template <typename T>
	static T RoundUpThrow(IN T n, IN uint uAlign)
	{
		assert( uAlign > 0 );
		T v = SafeOperators::AddThrow(n, T(uAlign - 1));
		return T( (v) & ~(T(uAlign) - 1) );
	}
	template <typename T>
	static T RoundDown(IN T n, IN uint uAlign) throw()
	{
		assert( uAlign > 0 );
		return T( n & ~(T(uAlign) - 1) );
	}
};

// FixedElementMemoryPool

class FixedElementMemoryPool
{
public:
	FixedElementMemoryPool() throw() : m_pHead(NULL)
	{
	}
	FixedElementMemoryPool(const RefPtr<IMemoryManager>& mgr) throw() : m_mgr(mgr), m_pHead(NULL)
	{
	}
	~FixedElementMemoryPool() throw()
	{
		FreeDataChain();
	}

//methods

	void SetMemoryManager(const RefPtr<IMemoryManager>& mgr) throw()
	{
		assert( !mgr.IsNull() && m_mgr.IsNull() );
		m_mgr = mgr;
	}

	// free data links
	void FreeDataChain() throw()
	{
		if( m_mgr.IsNull() ) {
			assert( m_pHead == NULL );
			return ;
		}
		void* pPlex = m_pHead;
		while( pPlex != NULL ) {
			void* pNext = get_next_block(pPlex);
			m_mgr.Deref().Free((uintptr)pPlex);
			pPlex = pNext;
		}
		m_pHead = NULL;
	}

	// create block
	uintptr CreateBlock(uintptr uMinElements, uintptr uMaxElements, uintptr uElementSize, uintptr& uActElements)  //may throw
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements && uElementSize > 0 );
		assert( !m_mgr.IsNull() );

		void* pPlex = NULL;
		uintptr uBytes = 0;
		uintptr uElements = uMaxElements;
		uintptr uLinkSize = sizeof(void*);

		uActElements = uElements;
		while( uElements >= uMinElements ) {
			//try
			if( SafeOperators::Multiply<uintptr>(uElements, uElementSize, uBytes).IsFailed()
				|| SafeOperators::Add<uintptr>(uBytes, uLinkSize, uBytes).IsFailed() ) {
				uElements --;
				continue;
			}
			break;
		}
		if( uElements < uMinElements ) {
			throw( OverflowException() );
		}

		while( uElements >= uMinElements ) {
			//no overflow
			uBytes = uElements * uElementSize + sizeof(void*);
			pPlex = (void*)(m_mgr.Deref().Allocate(uBytes));
			if( pPlex == NULL ) {
				uElements --;
				continue;
			}
			break;
		}
		if( pPlex == NULL ) {
			throw( OutOfMemoryException() );
		}
		uActElements = uElements;

		get_next_block(pPlex) = m_pHead;
		m_pHead = pPlex;

		return (uintptr)get_data(pPlex);
	}

private:
	static void*& get_next_block(void* pBlock) throw()
	{
		assert( pBlock != NULL );
		return *((void**)pBlock);
	}
	static void* get_data(void* pBlock) throw()
	{
		return (void*)(((void**)pBlock) + 1);
	}

private:
	RefPtr<IMemoryManager> m_mgr;
	void* m_pHead;

private:
	//non-copyable
	FixedElementMemoryPool(const FixedElementMemoryPool& src) throw();
	FixedElementMemoryPool& operator=(const FixedElementMemoryPool& src) throw();
};

// PoolMemoryManager<t_size>

template <uintptr t_size>
class PoolMemoryManager : public IMemoryManager
{
public:
	PoolMemoryManager(uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_uElements(0), m_pFree(NULL),
			m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
	}
	PoolMemoryManager(const RefPtr<IMemoryManager>& mgr, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_uElements(0), m_pool(mgr), m_pFree(NULL),
			m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
	}
	~PoolMemoryManager() throw()
	{
	}

	void SetMemoryManager(const RefPtr<IMemoryManager>& mgr) throw()
	{
		m_pool.SetMemoryManager(mgr);
	}

	// IMemoryManager methods
	virtual uintptr Allocate(const uintptr& uBytes) throw()
	{
		void* p = NULL;

		try {
			get_free_node();

			void* pNewNode = m_pFree;
			void* pNextFree = *((void**)m_pFree);  //next

			m_pFree = pNextFree;
			m_uElements ++;
			assert( m_uElements > 0 );

			p = (void*)((void**)pNewNode + 1);  //data
		}
		catch(...) {
			return 0;
		}

		return (uintptr)p;
	}
	virtual uintptr Reallocate(const uintptr& p, const uintptr& uBytes) throw()
	{
		return 0;
	}
	virtual void    Free(const uintptr& p) throw()
	{
		void* pNode = (void*)((void**)p - 1);  //back to first

		*((void**)pNode) = m_pFree;  //next
		m_pFree = pNode;

		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 ) {
			m_pFree = NULL;
			m_pool.FreeDataChain();
		}
	}

private:
	//tools
	void get_free_node()  //may throw
	{
		uintptr node_size = SafeOperators::AddThrow<uintptr>(t_size, sizeof(void*));  //may throw
		if( m_pFree == NULL ) {
			uintptr uActElements;
			//may throw
			void* pNode = (void*)m_pool.CreateBlock(m_uMinBlockElements, m_uMaxBlockElements, node_size, uActElements);
			pNode = (byte*)pNode + (uActElements - 1) * node_size;
			for( uintptr uBlock = uActElements; uBlock > 0; uBlock -- ) {
				*((void**)pNode) = m_pFree;  //next
				m_pFree = pNode;
				pNode = (byte*)pNode - node_size;
			}
		}
		assert( m_pFree != NULL );
	}

private:
	uintptr m_uElements;

	FixedElementMemoryPool  m_pool;
	void*  m_pFree;  //free list
	uintptr m_uMinBlockElements, m_uMaxBlockElements;
};

// ITypeProcess

class NOVTABLE ITypeProcess
{
public:
	virtual uintptr TypeCast(const guid& clsid) throw() = 0;
	virtual void Destruction(const uintptr& p) throw() = 0;
};

// SharedPtrBlock

#pragma pack(push, 1)

struct SharedPtrBlock
{
public:
	SharedPtrBlock() throw() : m_p(NULL), m_shareCount(1), m_weakCount(1)
	{
	}
	~SharedPtrBlock() throw()
	{
	}

	//methods
	RefPtr<IMemoryManager>& GetMemoryManager() throw()
	{
		return m_mgr;
	}
	void SetMemoryManager(const RefPtr<IMemoryManager>& mgr) throw()
	{
		m_mgr = mgr;
	}
	RefPtr<ITypeProcess>& GetTypeProcess() throw()
	{
		return m_type;
	}
	void SetTypeProcess(const RefPtr<ITypeProcess>& type) throw()
	{
		m_type = type;
	}
	int AddRefCopy() throw()
	{
		return atomic_increment((int&)m_shareCount);
	}
	bool AddRefLock() throw()
	{
		for( ; ; ) {
			int tmp = m_shareCount;
			if( tmp <= 0 )
				return false;
			if( atomic_compare_exchange((int&)m_shareCount, tmp, tmp + 1) == tmp )
				return true;
		}
		return false;
	}
	int Release() throw()
	{
		return atomic_decrement((int&)m_shareCount);
	}
	int WeakAddRef() throw()
	{
		return atomic_increment((int&)m_weakCount);
	}
	int WeakRelease() throw()
	{
		return atomic_decrement((int&)m_weakCount);
	}

	int GetShareCount() const throw()
	{
		return m_shareCount;
	}
	int GetWeakCount() const throw()
	{
		return m_weakCount;
	}

	//object pointer
	void SetObject(void* p) throw()
	{
		m_p = p;
	}
	//destroy
	void DestroyObject() throw()
	{
		assert( !m_type.IsNull() );
		assert( !m_mgr.IsNull() );
		//destruction
		m_type.Deref().Destruction((uintptr)m_p);
		//free
		m_mgr.Deref().Free((uintptr)m_p);
		m_p = NULL;
	}

private:
	RefPtr<IMemoryManager> m_mgr;
	RefPtr<ITypeProcess>   m_type;
	void* m_p;
	volatile int   m_shareCount;
	volatile int   m_weakCount;

private:
	//noncopyable
	SharedPtrBlock(const SharedPtrBlock& src) throw();
	SharedPtrBlock& operator=(const SharedPtrBlock& src) throw();
};

// SharedArrayBlock

struct SharedArrayBlock
{
public:
	SharedArrayBlock() throw() : m_p(NULL), m_uLength(0), m_uAllocLength(0), m_shareCount(1), m_weakCount(1)
	{
	}
	~SharedArrayBlock() throw()
	{
	}

	//methods
	RefPtr<IMemoryManager>& GetMemoryManager() throw()
	{
		return m_mgr;
	}
	void SetMemoryManager(const RefPtr<IMemoryManager>& mgr) throw()
	{
		m_mgr = mgr;
	}
	void* GetAddress() const throw()
	{
		return m_p;
	}
	void SetAddress(void* p) throw()
	{
		m_p = p;
	}
	uintptr GetLength() const throw()
	{
		return m_uLength;
	}
	void SetLength(uintptr uLength) throw()
	{
		m_uLength = uLength;
	}
	uintptr GetAllocLength() const throw()
	{
		return m_uAllocLength;
	}
	void SetAllocLength(uintptr uAllocLength) throw()
	{
		m_uAllocLength = uAllocLength;
	}

	int AddRefCopy() throw()
	{
		return atomic_increment((int&)m_shareCount);
	}
	bool AddRefLock() throw()
	{
		for( ; ; ) {
			int tmp = m_shareCount;
			if( tmp <= 0 )
				return false;
			if( atomic_compare_exchange((int&)m_shareCount, tmp, tmp + 1) == tmp )
				return true;
		}
		return false;
	}
	int Release() throw()
	{
		return atomic_decrement((int&)m_shareCount);
	}
	int WeakAddRef() throw()
	{
		return atomic_increment((int&)m_weakCount);
	}
	int WeakRelease() throw()
	{
		return atomic_decrement((int&)m_weakCount);
	}

	int GetShareCount() const throw()
	{
		return m_shareCount;
	}
	int GetWeakCount() const throw()
	{
		return m_weakCount;
	}

	//destroy
	template <typename T>
	void DestroyArray() throw()
	{
		assert( !m_mgr.IsNull() );
		if( m_p == NULL ) {
			assert( m_uLength == 0 && m_uAllocLength == 0 );
			return ;
		}
		//destruction
		T* pt = (T*)m_p;
		for( uintptr i = 0; i < m_uLength; i ++ ) {
			pt->~T();
			++ pt;
		}
		//free
		m_mgr.Deref().Free((uintptr)m_p);
		m_p = NULL;
		m_uLength = 0;
		m_uAllocLength = 0;
	}

private:
	RefPtr<IMemoryManager> m_mgr;
	void*    m_p;  //first address of array
	uintptr  m_uLength;
	uintptr  m_uAllocLength;
	volatile int   m_shareCount;
	volatile int   m_weakCount;

private:
	//noncopyable
	SharedArrayBlock(const SharedArrayBlock& src) throw();
	SharedArrayBlock& operator=(const SharedArrayBlock& src) throw();
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_BASE_H__
////////////////////////////////////////////////////////////////////////////////
