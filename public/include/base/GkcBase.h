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
// Pointers

// RefPtr<T>

template <class T>
class RefPtr
{
public:
	RefPtr() throw() : m_p(NULL)
	{
	}
	explicit RefPtr(const T& t) throw() : m_p(&(const_cast<T&>(t)))
	{
	}
	explicit RefPtr(T& t) throw() : m_p(&t)
	{
	}
	explicit RefPtr(const T* p) throw() : m_p(const_cast<T*>(p))
	{
	}
	explicit RefPtr(T* p) throw() : m_p(p)
	{
	}
	RefPtr(const RefPtr<T>& src) throw() : m_p(src.m_p)
	{
	}
	~RefPtr() throw()
	{
	}

	void Release() throw()
	{
		m_p = NULL;
	}

	//operators
	RefPtr<T>& operator=(const RefPtr<T>& src) throw()
	{
		if( this != &src ) {
			m_p = src.m_p;
		}
		return *this;
	}
	RefPtr<T>& operator=(T* p) throw()
	{
		m_p = p;
		return *this;
	}

	//logical
	bool operator==(const RefPtr<T>& right) const throw()
	{
		return m_p == right.m_p;
	}
	bool operator!=(const RefPtr<T>& right) const throw()
	{
		return !(*this == right);
	}

	//methods
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	const T& Deref() const throw()
	{
		assert( !IsNull() );
		return *m_p;
	}
	T& Deref() throw()
	{
		assert( !IsNull() );
		return *m_p;
	}

private:
	T* m_p;

private:
	friend class RefPtrHelper;
};

// IMemoryManager

class NOVTABLE IMemoryManager
{
public:
	virtual uintptr Allocate(const uintptr& uBytes) throw() = 0;
	// p == NULL : the same as Allocate
	// uBytes == 0 : free p, return NULL
	// return NULL, failed, and p unchanged
	virtual uintptr Reallocate(const uintptr& p, const uintptr& uBytes) throw() = 0;
	virtual void    Free(const uintptr& p) throw() = 0;
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

	//may throw
	uintptr CreateBlock(uintptr uMinElements, uintptr uMaxElements, uintptr uElementSize, uintptr& uActElements)
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

// RefPtrHelper

class RefPtrHelper
{
public:
	template <class T>
	static RefPtr<T> ToRefPtr(const T& t) throw()
	{
		return RefPtr<T>(t);
	}
	//cast
	template <class T, class TBase>
	static RefPtr<TBase> TypeCast(const RefPtr<T>& t) throw()
	{
		assert( !t.IsNull() );
		return RefPtr<TBase>(static_cast<const TBase&>(t.Deref()));
	}
	//clone
	template <class T>
	static void Clone(const RefPtr<T>& tSrc, RefPtr<T>& tDest) //may throw
	{
		if( tSrc.m_p != tDest.m_p && !tSrc.IsNull() && !tDest.IsNull() ) {
			tDest.Deref() = tSrc.Deref();
		}
	}
	//get internal pointer
	template <class T>
	static const T* GetInternalPointer(const RefPtr<T>& t) throw()
	{
		return t.m_p;
	}
	template <class T>
	static T* GetInternalPointer(RefPtr<T>& t) throw()
	{
		return t.m_p;
	}
};

//------------------------------------------------------------------------------
// Memory

// CrtMemoryManager

class CrtMemoryManager : public IMemoryManager
{
public:
	// IMemoryManager methods
	virtual uintptr Allocate(const uintptr& uBytes) throw()
	{
		return crt_alloc(uBytes);
	}
	virtual uintptr Reallocate(const uintptr& p, const uintptr& uBytes) throw()
	{
		return crt_realloc(p, uBytes);
	}
	virtual void    Free(const uintptr& p) throw()
	{
		crt_free(p);
	}
};

// PoolMemoryManager

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

// MemoryHelper

class MemoryHelper
{
public:
	static RefPtr<IMemoryManager> GetCrtMemoryManager() throw()
	{
		return RefPtrHelper::TypeCast<CrtMemoryManager, IMemoryManager>(RefPtr<CrtMemoryManager>(g_crtMemMgr));
	}

private:
	static CrtMemoryManager g_crtMemMgr;
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
	SharedArrayBlock() throw() : m_uLength(0), m_uAllocLength(0), m_shareCount(1), m_weakCount(1)
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
	void DestroyArray(T* p) throw()
	{
		assert( !m_mgr.IsNull() );
		//destruction
		T* pt = p;
		for( uintptr i = 0; i < m_uLength; i ++ ) {
			pt->~T();
			++ pt;
		}
		//free
		m_mgr.Deref().Free((uintptr)p);
		m_uLength = 0;
		m_uAllocLength = 0;
	}

private:
	RefPtr<IMemoryManager> m_mgr;
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

//------------------------------------------------------------------------------
//Iterators

// ArrayIterator<T>

template <typename T>
class ArrayIterator
{
public:
	ArrayIterator() throw()
	{
	}
	explicit ArrayIterator(const RefPtr<T>& element) throw() : m_element(element)
	{
	}
	ArrayIterator(const ArrayIterator<T>& src) throw() : m_element(src.m_element)
	{
	}
	~ArrayIterator() throw()
	{
	}

	ArrayIterator<T>& operator=(const ArrayIterator<T>& src) throw()
	{
		if( this != &src ) {
			m_element = src.m_element;
		}
		return *this;
	}

	const T& get_Value() const throw()
	{
		return m_element.Deref();
	}
	T& get_Value() throw()
	{
		return m_element.Deref();
	}
	void set_Value(const T& t)
	{
		m_element.Deref() = t;
	}
	void set_Value(T&& t)
	{
		m_element.Deref() = rv_forward(t);
	}

	//logical
	bool operator==(const ArrayIterator<T>& right) const throw()
	{
		return m_element == right.m_element;
	}
	bool operator!=(const ArrayIterator<T>& right) const throw()
	{
		return m_element != right.m_element;
	}

	//methods
	void MoveNext() throw()
	{
		m_element = &(m_element.Deref()) + 1;
	}
	void MovePrev() throw()
	{
		m_element = &(m_element.Deref()) - 1;
	}

private:
	RefPtr<T> m_element;
};

//------------------------------------------------------------------------------
// Synchronization

// Mutex
typedef inp_mutex  Mutex;

// SyncLock<T>

template <class T>
class SyncLock
{
public:
	SyncLock(T& t, bool bInitialLock = true) : m_t(t), m_bLocked(false)
	{
		if( bInitialLock )
			Lock();  //may throw
	}
	~SyncLock() throw()
	{
		if( m_bLocked )
			Unlock();
	}

	void Lock()
	{
		assert( !m_bLocked );
		m_t.Deref().Lock();  //may throw
		m_bLocked = true;
	}
	void Unlock() throw()
	{
		assert( m_bLocked );
		m_t.Deref().Unlock();
		m_bLocked = false;
	}

private:
	RefPtr<T>  m_t;
	bool       m_bLocked;

private:
	//noncopyable
	SyncLock(const SyncLock<T>&) throw();
	SyncLock<T>& operator=(const SyncLock<T>&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_BASE_H__
////////////////////////////////////////////////////////////////////////////////
