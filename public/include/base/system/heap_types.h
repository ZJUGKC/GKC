/*
** Copyright (c) 2019, Xin YUAN, courses of Zhejiang University
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
// internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//memory

#pragma pack(push, 1)

// i_memory_manager

class NOVTABLE i_memory_manager
{
public:
	virtual uintptr Allocate(const uintptr& uBytes) throw() = 0;
	// p == NULL : the same as Allocate
	// uBytes == 0 : free p, return NULL
	// return NULL, failed, and p unchanged
	virtual uintptr Reallocate(const uintptr& p, const uintptr& uBytes) throw() = 0;
	virtual void    Free(const uintptr& p) throw() = 0;
};

// i_memory_allocator_ref_32

class NOVTABLE i_memory_allocator_ref_32
{
public:
	virtual uint    Allocate(const uint& uBytes) throw() = 0;
	virtual uintptr ToPointer(const uint& p) throw() = 0;
};

// i_memory_allocator_ref_32_full

class NOVTABLE i_memory_allocator_ref_32_full : public i_memory_allocator_ref_32
{
public:
	// p == 0 : the same as Allocate
	// uBytes == 0 : free p, return 0
	// return 0, failed, and p unchanged
	virtual uint Reallocate(const uint& p, const uint& uBytes) throw() = 0;
	virtual void Free(const uint& p) throw() = 0;
};

// i_memory_allocator_ref_64

class NOVTABLE i_memory_allocator_ref_64
{
public:
	virtual int64   Allocate(const int64& uBytes) throw() = 0;
	virtual uintptr ToPointer(const int64& p) throw() = 0;
};

// i_memory_allocator_ref_64_full

class NOVTABLE i_memory_allocator_ref_64_full : public i_memory_allocator_ref_64
{
public:
	// p == 0 : the same as Allocate
	// uBytes == 0 : free p, return 0
	// return 0, failed, and p unchanged
	virtual int64 Reallocate(const int64& p, const int64& uBytes) throw() = 0;
	virtual void  Free(const int64& p) throw() = 0;
};

#pragma pack(pop)

// memory_allocator_ref_to_object<T, TAllocator, TPointer>
//   TAllocator : i_memory_allocator_ref_*
//   TPointer : uint or int64
template <typename T, class TAllocator, typename TPointer>
inline T& memory_allocator_ref_to_object(const ref_ptr<TAllocator>& allocator, const TPointer& p) throw()
{
	return *((T*)(const_cast<TAllocator&>(allocator.Deref()).ToPointer(p)));
}

// fixed_element_memory_pool

class fixed_element_memory_pool
{
public:
	fixed_element_memory_pool() throw() : m_pMgr(NULL), m_pHead(NULL)
	{
	}
	explicit fixed_element_memory_pool(i_memory_manager* pMgr) throw() : m_pMgr(pMgr), m_pHead(NULL)
	{
	}
	~fixed_element_memory_pool() throw()
	{
		FreeDataChain();
	}

//methods

	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_pMgr;
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		assert( pMgr != NULL && m_pMgr == NULL );
		m_pMgr = pMgr;
	}

	// free data links
	void FreeDataChain() throw()
	{
		if( m_pMgr == NULL ) {
			assert( m_pHead == NULL );
			return ;
		}
		void* pPlex = m_pHead;
		while( pPlex != NULL ) {
			void* pNext = get_next_block(pPlex);
			m_pMgr->Free((uintptr)pPlex);
			pPlex = pNext;
		}
		m_pHead = NULL;
	}

	// create block
	void* CreateBlock(uintptr uMinElements, uintptr uMaxElements, uintptr uElementSize, uintptr& uActElements)  //may throw
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements && uElementSize > 0 );
		assert( m_pMgr != NULL );

		void* pPlex = NULL;
		uintptr uBytes = 0;
		uintptr uElements = uMaxElements;
		uintptr uLinkSize = sizeof(void*);

		uActElements = uElements;
		while( uElements >= uMinElements ) {
			//try
			if( safe_operators::Multiply<uintptr>(uElements, uElementSize, uBytes).IsFailed()
				|| safe_operators::Add<uintptr>(uBytes, uLinkSize, uBytes).IsFailed() ) {
				uElements --;
				continue;
			}
			break;
		}
		if( uElements < uMinElements )
			throw overflow_exception();

		while( uElements >= uMinElements ) {
			//no overflow
			uBytes = uElements * uElementSize + sizeof(void*);
			pPlex = (void*)(m_pMgr->Allocate(uBytes));
			if( pPlex == NULL ) {
				uElements --;
				continue;
			}
			break;
		}
		if( pPlex == NULL )
			throw outofmemory_exception();
		uActElements = uElements;

		get_next_block(pPlex) = m_pHead;
		m_pHead = pPlex;

		return get_data(pPlex);
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
	i_memory_manager* m_pMgr;
	void* m_pHead;

private:
	//non-copyable
	fixed_element_memory_pool(const fixed_element_memory_pool& src) throw();
	fixed_element_memory_pool& operator=(const fixed_element_memory_pool& src) throw();
};

// free_list<TNode>
//  TNode : has a member named m_pNext (TNode*)

template <class TNode>
class free_list
{
public:
	explicit free_list(i_memory_manager* pMgr = NULL, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
					: m_pool(pMgr), m_pFree(NULL), m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements );
	}
	~free_list() throw()
	{
	}

//methods

	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_pool.GetMemoryManager();
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		m_pool.SetMemoryManager(pMgr);
	}

	void Clear() throw()
	{
		m_pFree = NULL;
		m_pool.FreeDataChain();
	}
	//get a free node
	TNode* GetFreeNode()  //may throw
	{
		if( m_pFree == NULL ) {
			uintptr uActElements;
			//may throw
			TNode* pNode = (TNode*)m_pool.CreateBlock(m_uMinBlockElements, m_uMaxBlockElements, sizeof(TNode), uActElements);
			pNode += (uActElements - 1);
			for( uintptr uBlock = uActElements; uBlock > 0; uBlock -- ) {
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				pNode --;
			}
		}
		assert( m_pFree != NULL );
		return m_pFree;
	}
	//pick free node
	void PickFreeNode() throw()
	{
		if( m_pFree != NULL )
			m_pFree = m_pFree->m_pNext;
	}
	void PutFreeNode(TNode* pNode) throw()
	{
		pNode->m_pNext = m_pFree;
		m_pFree = pNode;
	}

private:
	//pool
	fixed_element_memory_pool  m_pool;
	TNode*  m_pFree;  //free list
	uintptr m_uMinBlockElements, m_uMaxBlockElements;

private:
	//noncopyable
	free_list(const free_list&) throw();
	free_list& operator=(const free_list&) throw();
};

// node_helper<TNode>

template <typename TNode>
class node_helper
{
public:
	//tools
	template <typename... Args>
	static TNode* ConstructNode(free_list<TNode>& fl, Args&&... args)  //may throw
	{
		TNode* pNewNode = fl.GetFreeNode();  //may throw
		call_constructor(*pNewNode, rv_forward<Args>(args)...);  //may throw
		fl.PickFreeNode();
		return pNewNode;
	}
	static void DestructNode(free_list<TNode>& fl, TNode* pNode, uintptr& uElements) throw()
	{
		assert( pNode != NULL );
		pNode->~TNode();
		fl.PutFreeNode(pNode);
		assert( uElements > 0 );
		uElements --;
		if( uElements == 0 )
			fl.Clear();
	}
};

// fixed_size_memory_pool<t_size>

template <uintptr t_size>
class fixed_size_memory_pool
{
private:
	struct _Node
	{
		_Node* m_pNext;
		char   m_data[t_size];
	};

public:
	explicit fixed_size_memory_pool(i_memory_manager* pMgr = NULL, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
									: m_uElements(0), m_freelist(pMgr, uMinElements, uMaxElements)
	{
	}
	~fixed_size_memory_pool() throw()
	{
		//check memory leakage
		assert( m_uElements == 0 );
	}

//methods

	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_freelist.GetMemoryManager();
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		m_freelist.SetMemoryManager(pMgr);
	}

	void* Allocate() //may throw
	{
		_Node* pNode = m_freelist.GetFreeNode();  //may throw
		m_freelist.PickFreeNode();

		m_uElements ++;
		assert( m_uElements > 0 );

		return (void*)(pNode->m_data);  //data
	}
	void Free(void* p) throw()
	{
		//back to node
		_Node* pNode = (_Node*)((byte*)p - (intptr)((byte*)(((_Node*)(0x128))->m_data) - (byte*)(0x128)));
		m_freelist.PutFreeNode(pNode);

		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 )
			m_freelist.Clear();
	}

	void Clear() throw()
	{
		m_freelist.Clear();
		m_uElements = 0;
	}

private:
	uintptr m_uElements;

	free_list<_Node>  m_freelist;

private:
	//noncopyable
	fixed_size_memory_pool(const fixed_size_memory_pool&) throw();
	fixed_size_memory_pool& operator=(const fixed_size_memory_pool&) throw();
};

#pragma pack(push, 1)

//object callbacks
typedef void  (* object_destruction_func)(void* p);
typedef void* (* object_typecast_func)(void* p, const guid& iid);
typedef void* (* object_connection_func)(void* p, const guid& iid);

//for share object

// share_block_base

class share_block_base
{
public:
	share_block_base() throw() : m_pMgr(NULL), m_p(NULL), m_shareCount(1), m_weakCount(1)
	{
	}
	~share_block_base() throw()
	{
	}

	//methods
	i_memory_manager* GetMemoryManager() const throw()
	{
		return m_pMgr;
	}
	void SetMemoryManager(i_memory_manager* pMgr) throw()
	{
		m_pMgr = pMgr;
	}
	//object pointer
	void* GetAddress() const throw()
	{
		return m_p;
	}
	void SetAddress(void* p) throw()
	{
		m_p = p;
	}
	bool IsNull() const throw()
	{
		return m_p == NULL;
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

protected:
	i_memory_manager* m_pMgr;
	void* m_p;
	volatile int m_shareCount;
	volatile int m_weakCount;

private:
	//noncopyable
	share_block_base(const share_block_base&) throw();
	share_block_base& operator=(const share_block_base&) throw();
};

// share_ptr_block

class share_ptr_block : public share_block_base
{
public:
	share_ptr_block() throw() : m_pDestruction(NULL)
	{
	}
	~share_ptr_block() throw()
	{
	}

	void SetDestructionFunc(object_destruction_func pFunc) throw()
	{
		m_pDestruction = pFunc;
	}

	//destroy
	void DestroyObject() throw()
	{
		assert( m_pDestruction != NULL );
		assert( m_pMgr != NULL );
		if( m_p != NULL ) {
			//destruction
			m_pDestruction(m_p);
			//free
			m_pMgr->Free((uintptr)m_p);
			m_p = NULL;
		}
	}

protected:
	object_destruction_func m_pDestruction;

private:
	//noncopyable
	share_ptr_block(const share_ptr_block&) throw();
	share_ptr_block& operator=(const share_ptr_block&) throw();
};

// share_array_block

class share_array_block : public share_block_base
{
public:
	share_array_block() throw() : m_uLength(0), m_uAllocLength(0)
	{
	}
	~share_array_block() throw()
	{
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

	//destroy
	template <typename T>
	void DestroyArray() throw()
	{
		assert( m_pMgr != NULL );
		if( m_p == NULL ) {
			assert( m_uLength == 0 && m_uAllocLength == 0 );
			return ;
		}
		//destruction
		call_array_destructors((T*)m_p, m_uLength);
		//free
		m_pMgr->Free((uintptr)m_p);
		m_p = NULL;
		m_uLength = 0;
		m_uAllocLength = 0;
	}

protected:
	uintptr m_uLength;
	uintptr m_uAllocLength;

private:
	//noncopyable
	share_array_block(const share_array_block&) throw();
	share_array_block& operator=(const share_array_block&) throw();
};

// share_com_block

class share_com_block : public share_ptr_block
{
public:
	share_com_block() throw() : m_pTypeCast(NULL), m_pConnection(NULL)
	{
	}
	~share_com_block() throw()
	{
	}

	object_typecast_func GetTypeCastFunc() const throw()
	{
		return m_pTypeCast;
	}
	void SetTypeCastFunc(object_typecast_func pFunc) throw()
	{
		m_pTypeCast = pFunc;
	}
	object_connection_func GetConnectionFunc() const throw()
	{
		return m_pConnection;
	}
	void SetConnectionFunc(object_connection_func pFunc) throw()
	{
		m_pConnection = pFunc;
	}

protected:
	object_typecast_func m_pTypeCast;
	object_connection_func m_pConnection;

private:
	share_com_block(const share_com_block&) throw();
	share_com_block& operator=(const share_com_block&) throw();
};

#pragma pack(pop)

//------------------------------------------------------------------------------
//unique pointers

// unique_ptr<T>

template <typename T>
class unique_ptr
{
public:
	unique_ptr() throw() : m_p(NULL)
	{
	}
	unique_ptr(unique_ptr<T>&& src) throw() : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	~unique_ptr() throw()
	{
		Release();
	}

	unique_ptr<T>& operator=(unique_ptr<T>&& src) throw()
	{
		if( this != &src ) {
			if( m_p == src.m_p ) {
				assert( m_p == NULL );
			}
			else {
				Release();
				m_p = src.m_p;
				src.m_p = NULL;
			}
		}
		return *this;
	}

	void Release() throw()
	{
		if( m_p != NULL ) {
			m_p->~T();
			crt_free(m_p);
			m_p = NULL;
		}
	}

	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	void Attach(T* p) throw()
	{
		if( m_p != p )
			Release();
		m_p = p;
	}
	T* Detach() throw()
	{
		T* p = m_p;
		m_p = NULL;
		return p;
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

protected:
	T* m_p;

private:
	unique_ptr(const unique_ptr&) throw();
	unique_ptr& operator=(const unique_ptr&) throw();

private:
	friend class unique_ptr_helper;
};

// unique_ptr_helper

class unique_ptr_helper
{
public:
	//make
	template <typename T, typename... Args>
	static unique_ptr<T> MakeUniquePtr(Args&&... args)
	{
		T* p = (T*)crt_alloc(sizeof(T));
		if( p == NULL )
			throw outofmemory_exception();
		try {
			call_constructor(*p, rv_forward<Args>(args)...);
		}
		catch(...) {
			crt_free(p);
			throw;
		}
		unique_ptr<T> ret;
		ret.m_p = p;
		return ret;
	}

	//clone
	template <typename T>
	static unique_ptr<T> Clone(const unique_ptr<T>& sp)
	{
		return ( !sp.IsNull() )
				? MakeUniquePtr(sp.Deref())
				: unique_ptr<T>();
	}

	template <typename T>
	static T* GetInternalPointer(const unique_ptr<T>& sp) throw()
	{
		return const_cast<T*>(sp.m_p);
	}

	//type cast (derived -> base)
	template <typename TSrc, typename TDest>
	static ref_ptr<TDest> TypeCast(const unique_ptr<TSrc>& sp) throw()
	{
		TDest* pRet = NULL;
		if( !sp.IsNull() )
			pRet = static_cast<TDest*>(sp.m_p);
		return ref_ptr<TDest>(pRet);
	}
};

// unique_fixed_array<T>

template <typename T>
class unique_fixed_array
{
public:
	typedef T  EType;
	typedef array_position  Position;
	typedef array_iterator<T>  Iterator;

public:
	unique_fixed_array() throw() : m_p(NULL)
	{
	}
	unique_fixed_array(unique_fixed_array<T>&& src) throw() : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	~unique_fixed_array() throw()
	{
		Free();
	}

	unique_fixed_array<T>& operator=(unique_fixed_array<T>&& src) throw()
	{
		if( this != &src ) {
			if( m_p == src.m_p ) {
				assert( m_p == NULL );
			}
			else {
				Free();
				m_p = src.m_p;
				src.m_p = NULL;
			}
		}
		return *this;
	}

	const T& operator[](uintptr uIndex) const throw()
	{
		return GetAt(uIndex);
	}
	T& operator[](uintptr uIndex) throw()
	{
		return GetAt(uIndex);
	}

	uintptr GetCount() const throw()
	{
		return m_p == NULL ? 0 : get_count();
	}
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	void Free() throw()
	{
		if( m_p != NULL ) {
			call_array_destructors(m_p, get_count());
			crt_free(get_start_pointer());
			m_p = NULL;
		}
	}

	//position
	Position GetStartPosition() const throw()
	{
		return Position(0);
	}
	Position GetTailPosition() const throw()
	{
		return Position(GetCount() - 1);
	}
	Position GetPosition(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return Position(uIndex);
	}

	const Iterator ToIterator(const Position& pos) const throw()
	{
		return Iterator(ref_ptr<T>(m_p + pos.GetIndex()));
	}
	Iterator ToIterator(const Position& pos) throw()
	{
		return Iterator(ref_ptr<T>(m_p + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		return Position(ref_ptr_helper::GetInternalPointer(iter.get_Ref()) - m_p);
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(ref_ptr<T>(m_p));
	}
	Iterator GetBegin() throw()
	{
		return Iterator(ref_ptr<T>(m_p));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(ref_ptr<T>(m_p + get_count()));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(ref_ptr<T>(m_p + get_count()));
	}
	const reverse_iterator<Iterator> GetReverseBegin() const throw()
	{
		return reverse_iterator<Iterator>(ToIterator(GetTailPosition()));
	}
	reverse_iterator<Iterator> GetReverseBegin() throw()
	{
		return reverse_iterator<Iterator>(ToIterator(GetTailPosition()));
	}
	const reverse_iterator<Iterator> GetReverseEnd() const throw()
	{
		return reverse_iterator<Iterator>(Iterator(ref_ptr<T>(m_p - 1)));
	}
	reverse_iterator<Iterator> GetReverseEnd() throw()
	{
		return reverse_iterator<Iterator>(Iterator(ref_ptr<T>(m_p - 1)));
	}

	const T& GetAt(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return *(m_p + uIndex);
	}
	T& GetAt(uintptr uIndex) throw()
	{
		assert( uIndex < GetCount() );
		return *(m_p + uIndex);
	}
	void SetAt(uintptr uIndex, const T& t)
	{
		assert( uIndex < GetCount() );
		m_p[uIndex] = t;  //may throw
	}
	void SetAt(uintptr uIndex, T&& t)
	{
		assert( uIndex < GetCount() );
		m_p[uIndex] = rv_forward(t);  //may throw
	}

protected:
	const uintptr& get_count() const throw()
	{
		assert( m_p != NULL );
		return *((const uintptr*)(const void*)m_p - 1);
	}
	uintptr& get_count() throw()
	{
		assert( m_p != NULL );
		return *((uintptr*)(void*)m_p - 1);
	}
	void* get_start_pointer() const throw()
	{
		return (void*)(const void*)&(get_count());
	}

protected:
	T*  m_p;

private:
	unique_fixed_array(const unique_fixed_array&) throw();
	unique_fixed_array& operator=(const unique_fixed_array&) throw();

private:
	friend class unique_fixed_array_helper;
};

// unique_fixed_array_helper

class unique_fixed_array_helper
{
public:
	template <typename T, typename... Args>
	static unique_fixed_array<T> MakeUniqueFixedArray(uintptr uCount, Args&&... args)
	{
		assert( uCount > 0 );
		uintptr uBytes = safe_operators::MultiplyThrow(uCount, (uintptr)sizeof(T));
		uBytes = safe_operators::AddThrow(uBytes, (uintptr)sizeof(uintptr));
		void* p = crt_alloc(uBytes);
		if( p == NULL )
			throw outofmemory_exception();
		try {
			call_array_constructors((byte*)p + sizeof(uintptr), uCount, rv_forward<Args>(args)...);
		}
		catch(...) {
			crt_free(p);
			throw;  //re-throw
		}
		unique_fixed_array<T> ret;
		ret.m_p = (T*)((byte*)p + sizeof(uintptr));
		ret.get_count() = uCount;
		return ret;
	}

	template <typename T>
	static T* GetInternalPointer(const unique_fixed_array<T>& sp) throw()
	{
		return const_cast<T*>(sp.m_p);
	}
};

#pragma pack(push, 1)

// unique_block_base

struct unique_block_base
{
public:
	//methods
	i_memory_manager* GetMemoryManager() const noexcept
	{
		return m_pMgr;
	}
	void SetMemoryManager(i_memory_manager* pMgr) noexcept
	{
		m_pMgr = pMgr;
	}

protected:
	i_memory_manager* m_pMgr;
};

// unique_array_block

struct unique_array_block : public unique_block_base
{
public:
//methods
	uintptr GetLength() const noexcept
	{
		return m_uLength;
	}
	void SetLength(uintptr uLength) noexcept
	{
		m_uLength = uLength;
	}
	uintptr GetAllocLength() const noexcept
	{
		return m_uAllocLength;
	}
	void SetAllocLength(uintptr uAllocLength) noexcept
	{
		m_uAllocLength = uAllocLength;
	}

protected:
	uintptr m_uAllocLength;
	uintptr m_uLength;
};

// unique_com_block_base

struct unique_com_block_base : public unique_block_base
{
public:
//methods
	void SetDestructionFunc(object_destruction_func pFunc) noexcept
	{
		m_pDestruction = pFunc;
	}
	object_destruction_func GetDestructionFunc() const noexcept
	{
		return m_pDestruction;
	}

protected:
	object_destruction_func m_pDestruction;
};

// unique_com_block

struct unique_com_block : public unique_com_block_base
{
public:
//methods
	void SetAddress(void* p) noexcept
	{
		m_p = p;
	}
	void* GetAddress() const noexcept
	{
		return m_p;
	}

	//destruct
	void DestructObject() noexcept
	{
		assert( m_pDestruction != NULL );
		if( m_p != NULL ) {
			//destruction
			m_pDestruction(m_p);
			m_p = NULL;
		}
	}

	object_typecast_func GetTypeCastFunc() const noexcept
	{
		return m_pTypeCast;
	}
	void SetTypeCastFunc(object_typecast_func pFunc) noexcept
	{
		m_pTypeCast = pFunc;
	}

protected:
	object_typecast_func m_pTypeCast;
	void* m_p;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
