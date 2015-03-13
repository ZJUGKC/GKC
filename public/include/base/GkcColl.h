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
This file contains collection classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_COLL_H__
#define __GKC_COLL_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error GkcColl.h requires GkcBase.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

//------------------------------------------------------------------------------
// TOOLS

// FreeList<TNode>
//  TNode : has a member named m_pNext (TNode*)

template <class TNode>
class FreeList
{
public:
	FreeList(const RefPtr<IMemoryManager>& mgr, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_pool(mgr), m_pFree(NULL), m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements );
	}
	~FreeList() throw()
	{
	}

	//methods
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
	FixedElementMemoryPool  m_pool;
	TNode*  m_pFree;  //free list
	uintptr m_uMinBlockElements, m_uMaxBlockElements;

private:
	FreeList(const FreeList&) throw();
	FreeList& operator=(const FreeList&) throw();
};

// PairHelper<TNode, TPair>

template <typename TNode, typename TPair>
class PairHelper
{
public:
	//tools
	static TNode* ConstructNode(FreeList<TNode>& freelist)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode);  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey>
	static TNode* ConstructNode(FreeList<TNode>& freelist, const TKey& key)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode, key);  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey>
	static TNode* ConstructNode(FreeList<TNode>& freelist, TKey&& key)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(key));  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey, typename TValue>
	static TNode* ConstructNode(FreeList<TNode>& freelist, const TKey& key, const TValue& val)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode, key, val);  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	template <typename TKey, typename TValue>
	static TNode* ConstructNode(FreeList<TNode>& freelist, TKey&& key, TValue&& val)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(key), rv_forward(val));  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	static TNode* ConstructNode(FreeList<TNode>& freelist, const TPair& pair)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode, pair);  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	static TNode* ConstructNode(FreeList<TNode>& freelist, TPair&& pair)  //may throw
	{
		TNode* pNewNode = freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(pair));  //may throw
		freelist.PickFreeNode();
		return pNewNode;
	}
	static void DestructNode(FreeList<TNode>& freelist, TNode* pNode, uintptr& uElements) throw()
	{
		assert( pNode != NULL );
		pNode->~TNode();
		freelist.PutFreeNode(pNode);
		assert( uElements > 0 );
		uElements --;
		if( uElements == 0 )
			freelist.Clear();
	}
};

//------------------------------------------------------------------------------
// SingleList<T, TCompareTrait>

template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
class SingleList
{
private:
	typedef SingleList<T, TCompareTrait>  thisClass;

	//internal node
	struct _Node
	{
		_Node()
		{
		}
		_Node(const T& t) : m_t(t)
		{
		}
		_Node(T&& t) : m_t(rv_forward(t))
		{
		}
		~_Node() throw()
		{
		}

		_Node* m_pNext;
		T m_t;

	private:
		_Node(const _Node&) throw();
	};

public:
	//iterator
	class Iterator
	{
	public:
		Iterator() throw()
		{
		}
		Iterator(const Iterator& src) throw() : m_refNode(src.m_refNode)
		{
		}
		~Iterator() throw()
		{
		}
		Iterator& operator=(const Iterator& src) throw()
		{
			if( &src != this ) {
				m_refNode = src.m_refNode;
			}
			return *this;
		}
		//properties
		const RefPtr<T> get_Ref() const throw()
		{
			return RefPtr<T>(m_refNode.Deref().m_t);
		}
		RefPtr<T> get_Ref() throw()
		{
			return RefPtr<T>(m_refNode.Deref().m_t);
		}
		const T& get_Value() const throw()
		{
			return m_refNode.Deref().m_t;
		}
		T& get_Value() throw()
		{
			return m_refNode.Deref().m_t;
		}
		void set_Value(const T& t)  //may throw
		{
			//may throw
			m_refNode.Deref().m_t = t;
		}
		void set_Value(T&& t)  //may throw
		{
			//may throw
			m_refNode.Deref().m_t = rv_forward(t);
		}
		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_refNode == right.m_refNode;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_refNode != right.m_refNode;
		}
		//next
		void MoveNext() throw()
		{
			m_refNode = m_refNode.Deref().m_pNext;
		}

	private:
		RefPtr<_Node>  m_refNode;

		friend class thisClass;
	};

public:
	SingleList(const RefPtr<IMemoryManager>& mgr, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_pHead(NULL), m_uElements(0),
			m_freelist(mgr, uMinElements, uMaxElements)
	{
	}
	~SingleList() throw()
	{
		RemoveAll();
	}

	uintptr GetCount() const throw()
	{
		return m_uElements;
	}
	bool IsEmpty() const throw()
	{
		return m_uElements == 0;
	}

	//iterator
	const Iterator GetHead() const throw()
	{
		return get_iterator(m_pHead);
	}
	Iterator GetHead() throw()
	{
		return get_iterator(m_pHead);
	}
	const Iterator GetBegin() const throw()
	{
		return GetHead();
	}
	Iterator GetBegin() throw()
	{
		return GetHead();
	}
	const Iterator GetEnd() const throw()
	{
		return get_iterator(NULL);
	}
	Iterator GetEnd() throw()
	{
		return get_iterator(NULL);
	}

	//methods
	void RemoveAll() throw()
	{
		while( m_uElements > 0 ) {
			_Node* pKill = m_pHead;
			assert( pKill != NULL );
			m_pHead = m_pHead->m_pNext;
			free_node(pKill);
		}
		assert( m_uElements == 0 );
		assert( m_pHead == NULL );
		m_freelist.Clear();
	}

	//Add
	Iterator AddHead()  //may throw
	{
		_Node* pNode = new_node(m_pHead);
		m_pHead = pNode;
		return GetHead();
	}
	Iterator AddHead(const T& t)  //may throw
	{
		_Node* pNode = new_node(t, m_pHead);
		m_pHead = pNode;
		return GetHead();
	}
	Iterator AddHead(T&& t)  //may throw
	{
		_Node* pNode = new_node(rv_forward(t), m_pHead);
		m_pHead = pNode;
		return GetHead();
	}
	//Remove
	void RemoveHead() throw()
	{
		assert( m_pHead != NULL );
		_Node* pNode = m_pHead;
		m_pHead = pNode->m_pNext;
		free_node(pNode);
	}
	//Find
	Iterator FindIndex(uintptr uIndex) const throw()
	{
		if( uIndex > m_uElements )
			return get_iterator(NULL);
		if( m_pHead == NULL )
			return get_iterator(NULL);
		_Node* pNode = m_pHead;
		for( uintptr i = 0; i < uIndex; i ++ ) {
			pNode = pNode->m_pNext;
		} //end for
		return get_iterator(pNode);
	}
	Iterator Find(const T& t, const Iterator& iterAfter) const throw()
	{
		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterAfter.m_refNode));
		if( pNode == NULL ) {
			pNode = m_pHead;  //start at head
		}
		else {
			pNode = pNode->m_pNext;  // start after the one specified
		}
		for( ; pNode != NULL; pNode = pNode->m_pNext ) {
			if( TCompareTrait::IsEQ(pNode->m_t, t) )
				return get_iterator(pNode);
		}
		return get_iterator(NULL);
	}

private:
	//tools
	void fill_new_node(_Node* pNewNode, _Node* pNext) throw()
	{
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );
	}

	// nodes
	_Node* new_node(_Node* pNext)  //may throw
	{
		_Node* pNewNode = PairHelper<_Node, T>::ConstructNode(m_freelist);
		fill_new_node(pNewNode, pNext);
		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pNext)  //may throw
	{
		_Node* pNewNode = PairHelper<_Node, T>::ConstructNode(m_freelist, t);
		fill_new_node(pNewNode, pNext);
		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pNext)  //may throw
	{
		_Node* pNewNode = PairHelper<_Node, T>::ConstructNode(m_freelist, rv_forward(t));
		fill_new_node(pNewNode, pNext);
		return pNewNode;
	}
	// free
	void free_node(_Node* pNode) throw()
	{
		PairHelper<_Node, T>::DestructNode(m_freelist, pNode, m_uElements);
	}

	//iterator
	static Iterator get_iterator(_Node* pNode) throw()
	{
		Iterator iter;
		iter.m_refNode = pNode;
		return iter;
	}

private:
	_Node*   m_pHead;  //head node
	uintptr  m_uElements;
	//free list
	FreeList<_Node>  m_freelist;

private:
	//non-copyable
	SingleList(const SingleList&) throw();
	SingleList& operator=(const SingleList&) throw();
};

// List<T, TCompareTrait>

template <typename T, class TCompareTrait = DefaultCompareTrait<T>>
class List
{
private:
	typedef List<T, TCompareTrait>  thisClass;

	//internal node
	struct _Node
	{
		_Node()
		{
		}
		_Node(const T& t) : m_t(t)
		{
		}
		_Node(T&& t) : m_t(rv_forward(t))
		{
		}
		~_Node() throw()
		{
		}

		_Node* m_pNext;
		_Node* m_pPrev;
		T m_t;

	private:
		_Node(const _Node&) throw();
	};

public:
	//iterator
	class Iterator
	{
	public:
		Iterator() throw()
		{
		}
		Iterator(const Iterator& src) throw() : m_refList(src.m_refList), m_refNode(src.m_refNode)
		{
		}
		~Iterator() throw()
		{
		}
		Iterator& operator=(const Iterator& src) throw()
		{
			if( &src != this ) {
				m_refList = src.m_refList;
				m_refNode = src.m_refNode;
			}
			return *this;
		}
		//properties
		const RefPtr<T> get_Ref() const throw()
		{
			return RefPtr<T>(m_refNode.Deref().m_t);
		}
		RefPtr<T> get_Ref() throw()
		{
			return RefPtr<T>(m_refNode.Deref().m_t);
		}
		const T& get_Value() const throw()
		{
			return m_refNode.Deref().m_t;
		}
		T& get_Value() throw()
		{
			return m_refNode.Deref().m_t;
		}
		void set_Value(const T& t)  //may throw
		{
			//may throw
			m_refNode.Deref().m_t = t;
		}
		void set_Value(T&& t)  //may throw
		{
			//may throw
			m_refNode.Deref().m_t = rv_forward(t);
		}
		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_refList == right.m_refList && m_refNode == right.m_refNode;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_refList != right.m_refList || m_refNode != right.m_refNode;
		}
		//next
		void MoveNext() throw()
		{
			m_refNode = m_refNode.Deref().m_pNext;
		}
		//prev
		void MovePrev() throw()
		{
			//NULL node -> Tail
			m_refNode = ( m_refNode.IsNull() ) ? m_refList.Deref().GetTail().m_refNode
												: m_refNode.Deref().m_pPrev;
		}

	private:
		RefPtr<thisClass>  m_refList;
		RefPtr<_Node>      m_refNode;

		friend class thisClass;
	};

public:
	List(const RefPtr<IMemoryManager>& mgr, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
		: m_pHead(NULL), m_pTail(NULL), m_uElements(0),
		m_freelist(mgr, uMinElements, uMaxElements)
	{
	}
	~List() throw()
	{
		RemoveAll();
	}

	uintptr GetCount() const throw()
	{
		return m_uElements;
	}
	bool IsEmpty() const throw()
	{
		return m_uElements == 0;
	}

	//iterator
	const Iterator GetHead() const throw()
	{
		return get_iterator(m_pHead);
	}
	Iterator GetHead() throw()
	{
		return get_iterator(m_pHead);
	}
	const Iterator GetTail() const throw()
	{
		return get_iterator(m_pTail);
	}
	Iterator GetTail() throw()
	{
		return get_iterator(m_pTail);
	}
	const Iterator GetBegin() const throw()
	{
		return GetHead();
	}
	Iterator GetBegin() throw()
	{
		return GetHead();
	}
	const Iterator GetEnd() const throw()
	{
		return get_iterator(NULL);
	}
	Iterator GetEnd() throw()
	{
		return get_iterator(NULL);
	}
	const ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	ReverseIterator<Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	const ReverseIterator<Iterator> GetReverseEnd() const throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}
	ReverseIterator<Iterator> GetReverseEnd() throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}

	//methods
	void RemoveAll() throw()
	{
		while( m_uElements > 0 ) {
			_Node* pKill = m_pHead;
			assert( pKill != NULL );
			m_pHead = m_pHead->m_pNext;
			free_node(pKill);
		}
		assert( m_uElements == 0 );
		assert( m_pHead == NULL );
		m_pTail = NULL;
		m_freelist.Clear();
	}

	//Add
	Iterator AddHead()  //may throw
	{
		_Node* pNode = new_node(NULL, m_pHead);
		if( m_pHead != NULL )
			m_pHead->m_pPrev = pNode;
		else
			m_pTail = pNode;
		m_pHead = pNode;
		return GetHead();
	}
	Iterator AddHead(const T& t)  //may throw
	{
		_Node* pNode = new_node(t, NULL, m_pHead);
		if( m_pHead != NULL )
			m_pHead->m_pPrev = pNode;
		else
			m_pTail = pNode;
		m_pHead = pNode;
		return GetHead();
	}
	Iterator AddHead(T&& t)  //may throw
	{
		_Node* pNode = new_node(rv_forward(t), NULL, m_pHead);
		if( m_pHead != NULL )
			m_pHead->m_pPrev = pNode;
		else
			m_pTail = pNode;
		m_pHead = pNode;
		return GetHead();
	}
	Iterator AddTail()  //may throw
	{
		_Node* pNode = new_node(m_pTail, NULL);
		if( m_pTail != NULL )
			m_pTail->m_pNext = pNode;
		else
			m_pHead = pNode;
		m_pTail = pNode;
		return GetTail();
	}
	Iterator AddTail(const T& t)  //may throw
	{
		_Node* pNode = new_node(t, m_pTail, NULL);
		if( m_pTail != NULL )
			m_pTail->m_pNext = pNode;
		else
			m_pHead = pNode;
		m_pTail = pNode;
		return GetTail();
	}
	Iterator AddTail(T&& t)  //may throw
	{
		_Node* pNode = new_node(rv_forward(t), m_pTail, NULL);
		if( m_pTail != NULL )
			m_pTail->m_pNext = pNode;
		else
			m_pHead = pNode;
		m_pTail = pNode;
		return GetTail();
	}
	void AddHeadList(const List<T>& listNew)
	{
		ReverseIterator<Iterator> iterB = listNew.GetReverseBegin();
		ReverseIterator<Iterator> iterE = listNew.GetReverseEnd();
		for( ; iterB != iterE; iterB.MoveNext() ) {
			AddHead(iterB.get_Value());
		}
	}
	void AddTailList(const List<T>& listNew)
	{
		Iterator iterB = listNew.GetBegin();
		Iterator iterE = listNew.GetEnd();
		for( ; iterB != iterE; iterB.MoveNext() ) {
			AddTail(iterB.get_Value());
		}
	}

	//Remove
	void RemoveHead() throw()
	{
		assert( m_pHead != NULL );
		_Node* pNode = m_pHead;
		m_pHead = pNode->m_pNext;
		if( m_pHead != NULL )
			m_pHead->m_pPrev = NULL;
		else
			m_pTail = NULL;
		free_node(pNode);
	}
	void RemoveTail() throw()
	{
		assert( m_pTail != NULL );
		_Node* pNode = m_pTail;
		m_pTail = pNode->m_pPrev;
		if( m_pTail != NULL )
			m_pTail->m_pNext = NULL;
		else
			m_pHead = NULL;
		free_node(pNode);
	}

	//insert
	Iterator InsertBefore(const Iterator& iter)
	{
		if( iter == GetEnd() )
			return AddHead(); // insert before nothing -> head of the list

		// Insert it before position
		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pNewNode = new_node(pOldNode->m_pPrev, pOldNode);
		if( pOldNode->m_pPrev != NULL ) {
			pOldNode->m_pPrev->m_pNext = pNewNode;
		}
		else {
			assert( pOldNode == m_pHead );
			m_pHead = pNewNode;
		}
		pOldNode->m_pPrev = pNewNode;

		return get_iterator(pNewNode);
	}
	Iterator InsertBefore(const Iterator& iter, const T& t)
	{
		if( iter == GetEnd() )
			return AddHead(t);

		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pNewNode = new_node(t, pOldNode->m_pPrev, pOldNode);
		if( pOldNode->m_pPrev != NULL ) {
			pOldNode->m_pPrev->m_pNext = pNewNode;
		}
		else {
			assert( pOldNode == m_pHead );
			m_pHead = pNewNode;
		}
		pOldNode->m_pPrev = pNewNode;

		return get_iterator(pNewNode);
	}
	Iterator InsertBefore(const Iterator& iter, T&& t)
	{
		if( iter == GetEnd() )
			return AddHead(rv_forward(t));

		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pNewNode = new_node(rv_forward(t), pOldNode->m_pPrev, pOldNode);
		if( pOldNode->m_pPrev != NULL ) {
			pOldNode->m_pPrev->m_pNext = pNewNode;
		}
		else {
			assert( pOldNode == m_pHead );
			m_pHead = pNewNode;
		}
		pOldNode->m_pPrev = pNewNode;

		return get_iterator(pNewNode);
	}

	Iterator InsertAfter(const Iterator& iter)
	{
		if( iter == GetEnd() )
			return AddTail(); // insert after nothing -> tail of the list

		// Insert it after position
		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pNewNode = new_node(pOldNode, pOldNode->m_pNext);
		if( pOldNode->m_pNext != NULL ) {
			pOldNode->m_pNext->m_pPrev = pNewNode;
		}
		else {
			assert( pOldNode == m_pTail );
			m_pTail = pNewNode;
		}
		pOldNode->m_pNext = pNewNode;

		return get_iterator(pNewNode);
	}
	Iterator InsertAfter(const Iterator& iter, const T& t)
	{
		if( iter == GetEnd() )
			return AddTail(t);

		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pNewNode = new_node(t, pOldNode, pOldNode->m_pNext);
		if( pOldNode->m_pNext != NULL ) {
			pOldNode->m_pNext->m_pPrev = pNewNode;
		}
		else {
			assert( pOldNode == m_pTail );
			m_pTail = pNewNode;
		}
		pOldNode->m_pNext = pNewNode;

		return get_iterator(pNewNode);
	}
	Iterator InsertAfter(const Iterator& iter, T&& t)
	{
		if( iter == GetEnd() )
			return AddTail(rv_forward(t));

		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pNewNode = new_node(rv_forward(t), pOldNode, pOldNode->m_pNext);
		if( pOldNode->m_pNext != NULL ) {
			pOldNode->m_pNext->m_pPrev = pNewNode;
		}
		else {
			assert( pOldNode == m_pTail );
			m_pTail = pNewNode;
		}
		pOldNode->m_pNext = pNewNode;

		return get_iterator(pNewNode);
	}

	//remove
	void RemoveAt(const Iterator& iter) throw()
	{
		assert( iter != GetEnd() );

		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		// remove pOldNode from list
		if( pOldNode == m_pHead )
			m_pHead = pOldNode->m_pNext;
		else
			pOldNode->m_pPrev->m_pNext = pOldNode->m_pNext;
		if( pOldNode == m_pTail )
			m_pTail = pOldNode->m_pPrev;
		else
			pOldNode->m_pNext->m_pPrev = pOldNode->m_pPrev;

		free_node(pOldNode);
	}

	//move
	void MoveToHead(const Iterator& iter) throw()
	{
		assert( iter != GetEnd() );

		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		if( pNode == m_pHead ) {
			// Already at the head
			return ;
		}

		if( pNode->m_pNext == NULL ) {
			assert( pNode == m_pTail );
			m_pTail = pNode->m_pPrev;
		}
		else {
			pNode->m_pNext->m_pPrev = pNode->m_pPrev;
		}

		assert( pNode->m_pPrev != NULL );  // This node can't be the head, since we already checked that case
		pNode->m_pPrev->m_pNext = pNode->m_pNext;

		m_pHead->m_pPrev = pNode;
		pNode->m_pNext = m_pHead;
		pNode->m_pPrev = NULL;
		m_pHead = pNode;
	}
	void MoveToTail(const Iterator& iter) throw()
	{
		assert( iter != GetEnd() );

		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		if( pNode == m_pTail ) {
			// Already at the tail
			return ;
		}

		if( pNode->m_pPrev == NULL ) {
			assert( pNode == m_pHead );
			m_pHead = pNode->m_pNext;
		}
		else {
			pNode->m_pPrev->m_pNext = pNode->m_pNext;
		}

		pNode->m_pNext->m_pPrev = pNode->m_pPrev;

		m_pTail->m_pNext = pNode;
		pNode->m_pPrev = m_pTail;
		pNode->m_pNext = NULL;
		m_pTail = pNode;
	}
	//swap
	void SwapElements(const Iterator& iter1, const Iterator& iter2) throw()
	{
		assert( iter1 != GetEnd() );
		assert( iter2 != GetEnd() );

		if( iter1 == iter2 ) {
			// Nothing to do
			return ;
		}

		_Node* pNode1 = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter1.m_refNode));
		_Node* pNode2 = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter2.m_refNode));
		if( pNode2->m_pNext == pNode1 ) {
			// Swap pNode2 and pNode1 so that the next case works
			_Node* pNodeTemp = pNode1;
			pNode1 = pNode2;
			pNode2 = pNodeTemp;
		}
		if( pNode1->m_pNext == pNode2 ) {
			// Node1 and Node2 are adjacent
			pNode2->m_pPrev = pNode1->m_pPrev;
			if( pNode1->m_pPrev != NULL ) {
				pNode1->m_pPrev->m_pNext = pNode2;
			}
			else {
				assert( m_pHead == pNode1 );
				m_pHead = pNode2;
			}
			pNode1->m_pNext = pNode2->m_pNext;
			if( pNode2->m_pNext != NULL ) {
				pNode2->m_pNext->m_pPrev = pNode1;
			}
			else {
				assert( m_pTail == pNode2 );
				m_pTail = pNode1;
			}
			pNode2->m_pNext = pNode1;
			pNode1->m_pPrev = pNode2;
		}
		else {
			// The two nodes are not adjacent
			_Node* pNodeTemp;

			pNodeTemp = pNode1->m_pPrev;
			pNode1->m_pPrev = pNode2->m_pPrev;
			pNode2->m_pPrev = pNodeTemp;

			pNodeTemp = pNode1->m_pNext;
			pNode1->m_pNext = pNode2->m_pNext;
			pNode2->m_pNext = pNodeTemp;

			if( pNode1->m_pNext != NULL ) {
				pNode1->m_pNext->m_pPrev = pNode1;
			}
			else {
				assert( m_pTail == pNode2 );
				m_pTail = pNode1;
			}
			if( pNode1->m_pPrev != NULL ) {
				pNode1->m_pPrev->m_pNext = pNode1;
			}
			else {
				assert( m_pHead == pNode2 );
				m_pHead = pNode1;
			}
			if( pNode2->m_pNext != NULL ) {
				pNode2->m_pNext->m_pPrev = pNode2;
			}
			else {
				assert( m_pTail == pNode1 );
				m_pTail = pNode2;
			}
			if( pNode2->m_pPrev != NULL ) {
				pNode2->m_pPrev->m_pNext = pNode2;
			}
			else {
				assert( m_pHead == pNode1 );
				m_pHead = pNode2;
			}
		} //end if
	}

	//Find
	Iterator FindIndex(uintptr uIndex) const throw()
	{
		if( uIndex > m_uElements )
			return get_iterator(NULL);
		if( m_pHead == NULL )
			return get_iterator(NULL);
		_Node* pNode = m_pHead;
		for( uintptr i = 0; i < uIndex; i ++ ) {
			pNode = pNode->m_pNext;
		} //end for
		return get_iterator(pNode);
	}
	Iterator Find(const T& t, const Iterator& iterAfter) const throw()
	{
		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterAfter.m_refNode));
		if( pNode == NULL ) {
			pNode = m_pHead;  //start at head
		}
		else {
			pNode = pNode->m_pNext;  // start after the one specified
		}
		for( ; pNode != NULL; pNode = pNode->m_pNext ) {
			if( TCompareTrait::IsEQ(pNode->m_t, t) )
				return get_iterator(pNode);
		}
		return get_iterator(NULL);
	}

private:
	//tools
	void fill_new_node(_Node* pNewNode, _Node* pPrev, _Node* pNext) throw()
	{
		pNewNode->m_pPrev = pPrev;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );
	}

	//nodes
	_Node* new_node(_Node* pPrev, _Node* pNext)  //may throw
	{
		_Node* pNewNode = PairHelper<_Node, T>::ConstructNode(m_freelist);
		fill_new_node(pNewNode, pPrev, pNext);
		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pPrev, _Node* pNext)  //may throw
	{
		_Node* pNewNode = PairHelper<_Node, T>::ConstructNode(m_freelist, t);
		fill_new_node(pNewNode, pPrev, pNext);
		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pPrev, _Node* pNext)  //may throw
	{
		_Node* pNewNode = PairHelper<_Node, T>::ConstructNode(m_freelist, rv_forward(t));
		fill_new_node(pNewNode, pPrev, pNext);
		return pNewNode;
	}
	//free
	void free_node(_Node* pNode) throw()
	{
		PairHelper<_Node, T>::DestructNode(m_freelist, pNode, m_uElements);
	}

	//iterator
	Iterator get_iterator(_Node* pNode) const throw()
	{
		Iterator iter;
		iter.m_refList = this;
		iter.m_refNode = pNode;
		return iter;
	}

private:
	_Node*   m_pHead;  //head node
	_Node*   m_pTail;  //tail node
	uintptr  m_uElements;
	//free list
	FreeList<_Node>  m_freelist;

private:
	//non-copyable
	List(const List&) throw();
	List& operator=(const List&) throw();
};

//------------------------------------------------------------------------------
// Hash Table

//internal
// _HashTable<TKey, TPair, THashTrait, TCompareTrait>

template <typename TKey, class TPair, class THashTrait = DefaultHashTrait<TKey>, class TCompareTrait = DefaultCompareTrait<TKey>>
class _HashTable
{
private:
	typedef _HashTable<TKey, TPair, THashTrait, TCompareTrait>  thisClass;

	//special usage
	class _Key : public TKey
	{
	};

private:
	//internal node
	struct _Node
	{
		_Node()
		{
		}
		explicit _Node(const _Key& key) : m_t(static_cast<const TKey&>(key))
		{
		}
		explicit _Node(_Key&& key) : m_t(rv_forward(static_cast<TKey&>(key)))
		{
		}
		template <typename V>
		_Node(const TKey& key, const V& v) : m_t(key, v)
		{
		}
		template <typename V>
		_Node(TKey&& key, V&& v) : m_t(rv_forward(key), rv_forward(v))
		{
		}
		_Node(const TPair& t) : m_t(t)
		{
		}
		_Node(TPair&& t) : m_t(rv_forward(t))
		{
		}
		~_Node() throw()
		{
		}

		_Node*  m_pNext;
		TPair   m_t;
		uintptr m_uHashCode;  //hash code for element

	private:
		_Node(const _Node&) throw();
	};

public:
	//iterator
	class Iterator
	{
	public:
		Iterator() throw()
		{
		}
		Iterator(const Iterator& src) throw() : m_refTable(src.m_refTable), m_refNode(src.m_refNode)
		{
		}
		~Iterator() throw()
		{
		}
		Iterator& operator=(const Iterator& src) throw()
		{
			if( &src != this ) {
				m_refTable = src.m_refTable;
				m_refNode  = src.m_refNode;
			}
			return *this;
		}
		//properties
		const RefPtr<TPair> get_Ref() const throw()
		{
			return RefPtr<TPair>(m_refNode.Deref().m_t);
		}
		RefPtr<TPair> get_Ref() throw()
		{
			return RefPtr<TPair>(m_refNode.Deref().m_t);
		}
		const TPair& get_Value() const throw()
		{
			return m_refNode.Deref().m_t;
		}
		TPair& get_Value() throw()
		{
			return m_refNode.Deref().m_t;
		}
		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_refTable == right.m_refTable && m_refNode == right.m_refNode;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_refTable != right.m_refTable || m_refNode != right.m_refNode;
		}
		//next
		void MoveNext() throw()
		{
			m_refNode = m_refTable.Deref().to_next_node(RefPtrHelper::GetInternalPointer(m_refNode));
		}

	private:
		RefPtr<thisClass>  m_refTable;
		RefPtr<_Node>      m_refNode;

		friend class thisClass;
	};

public:
	_HashTable(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
			float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
			uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_mgr(mgr), m_ppBins(NULL), m_uBins(uBins), m_uElements(0),
			m_fOptimalLoad(fOptimalLoad), m_fLowThreshold(fLowThreshold), m_fHighThreshold(fHighThreshold),
			m_uHighRehashThreshold(Limits<uintptr>::Max), m_uLowRehashThreshold(0), m_uLockCount(0), // Start unlocked
			m_freelist(mgr, uMinElements, uMaxElements)
	{
		assert( uBins > 0 );
		assert( fOptimalLoad > 0.0f );
		assert( (fLowThreshold >= 0.0f) && (fLowThreshold < fOptimalLoad) );
		assert( fHighThreshold > fOptimalLoad );

		update_rehash_thresholds();
	}
	~_HashTable() throw()
	{
		RemoveAll();
	}

	uintptr GetCount() const throw()
	{
		return m_uElements;
	}
	bool IsEmpty() const throw()
	{
		return m_uElements == 0;
	}
	uintptr GetBucketSize() const throw()
	{
		return m_uBins;
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		if( IsEmpty() )
			return get_iterator(NULL);
		for( uintptr uBin = 0; uBin < m_uBins; uBin ++ ) {
			if( m_ppBins[uBin] != NULL )
				return get_iterator(m_ppBins[uBin]);
		}
		assert(false);
		return get_iterator(NULL);
	}
	Iterator GetBegin() throw()
	{
		if( IsEmpty() )
			return get_iterator(NULL);
		for( uintptr uBin = 0; uBin < m_uBins; uBin ++ ) {
			if( m_ppBins[uBin] != NULL )
				return get_iterator(m_ppBins[uBin]);
		}
		assert(false);
		return get_iterator(NULL);
	}
	const Iterator GetEnd() const throw()
	{
		return get_iterator(NULL);
	}
	Iterator GetEnd() throw()
	{
		return get_iterator(NULL);
	}

	//methods
	void RemoveAll() throw()
	{
		//lock
		DisableAutoRehash();
		//nodes
		if( m_ppBins != NULL ) {
			for( uintptr uBin = 0; uBin < m_uBins; uBin ++ ) {
				_Node* pNext = m_ppBins[uBin];
				while( pNext != NULL ) {
					_Node* pKill = pNext;
					pNext = pNext->m_pNext;
					free_node(pKill);
				}
			}
		} //end if
		//bucket
		clear_bucket();
		m_uElements = 0;
		//free list
		m_freelist.Clear();
		//unlock
		EnableAutoRehash();
	}

	//rehash
	void DisableAutoRehash() throw()
	{
		m_uLockCount ++;
	}
	void EnableAutoRehash() throw()
	{
		assert( m_uLockCount > 0 );
		m_uLockCount --;
	}
	void CompactBucket()  //may throw
	{
		if( (m_uElements < m_uLowRehashThreshold) && !is_locked() )
			rehash(pick_bucket_size(m_uElements));  //may throw
	}

	//find
	Iterator Find(const TKey& key) const throw()
	{
		uintptr uBin;
		uintptr uHash;
		_Node*  pPrev;
		return get_iterator(find_node(key, uBin, uHash, pPrev));
	}
	//find next key
	Iterator FindNext(const Iterator& iter) const throw()
	{
		assert( iter != GetEnd() );
		_Node*  pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		uintptr uHash = pNode->m_uHashCode;
		const Tkey& key = KeyHelper::GetKey<const TKey>(pNode->m_t);
		pNode = pNode->m_pNext;
		while( pNode != NULL ) {
			if( (pNode->m_uHashCode == uHash) && TCompareTrait::IsEQ(KeyHelper::GetKey(pNode->m_t), key) ) {
				return get_iterator(pNode);
			}
			pNode = pNode->m_pNext;
		}
		return get_iterator(NULL);
	}

	//add
	Iterator InsertWithoutFind(const TKey& key)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(static_cast<const _Key&>(key), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	Iterator InsertWithoutFind(TKey&& key)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(rv_forward(static_cast<_Key&>(key)), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	template <typename TValue>
	Iterator InsertWithoutFind(const TKey& key, const TValue& val)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(key, val, uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	template <typename TValue>
	Iterator InsertWithoutFind(TKey&& key, TValue&& val)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(rv_forward(key), rv_forward(val), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}

	Iterator Insert(const TKey& key)  //may throw
	{
		uintptr uBin;
		uintptr uHash;
		_Node* pPrev;
		_Node* pNode = find_node(key, uBin, uHash, pPrev);
		if( pNode == NULL )
			pNode = create_node(static_cast<const _Key&>(key), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	Iterator Insert(TKey&& key)  //may throw
	{
		uintptr uBin;
		uintptr uHash;
		_Node* pPrev;
		_Node* pNode = find_node(key, uBin, uHash, pPrev);
		if( pNode == NULL )
			pNode = create_node(rv_forward(static_cast<_Key&>(key)), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	template <typename TValue>
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		uintptr uBin;
		uintptr uHash;
		_Node* pPrev;
		_Node* pNode = find_node(key, uBin, uHash, pPrev);
		if( pNode == NULL )
			pNode = create_node(key, val, uBin, uHash);  //may throw
		else
			pNode->m_t.set_Second(val);  //may throw
		return get_iterator(pNode);
	}
	template <typename TValue>
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		uintptr uBin;
		uintptr uHash;
		_Node* pPrev;
		_Node* pNode = find_node(key, uBin, uHash, pPrev);
		if( pNode == NULL )
			pNode = create_node(rv_forward(key), rv_forward(val), uBin, uHash);  //may throw
		else
			pNode->m_t.set_Second(rv_forward(val));  //may throw
		return get_iterator(pNode);
	}

	//remove
	bool Remove(const TKey& key) throw()
	{
		uintptr uBin;
		uintptr uHash;
		_Node* pPrev = NULL;
		_Node* pNode = find_node(key, uBin, uHash, pPrev);
		if( pNode == NULL )
			return false;
		remove_node(pNode, pPrev);
		return true;
	}
	void RemoveAt(const Iterator& iter) throw()
	{
		assert( iter != GetEnd() );
		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		_Node* pPrev = NULL;
		uintptr uBin = pNode->m_uHashCode % m_uBins;
		assert( m_ppBins[uBin] != NULL );
		if( pNode != m_ppBins[uBin] ) {
			pPrev = m_ppBins[uBin];
			while( pPrev->m_pNext != pNode ) {
				pPrev = pPrev->m_pNext;
				assert( pPrev != NULL );
			}
		}
		remove_node(pNode, pPrev);
	}

protected:
	// bucket
	uintptr pick_bucket_size(uintptr uElements) const throw()
	{
		// List of primes such that c_Primes[i] is the smallest prime greater than 2^(5+i/3)
		static const uint c_Primes[] = {
			17, 23, 29, 37, 41, 53, 67, 83, 103, 131, 163, 211, 257, 331, 409, 521, 647, 821,
			1031, 1291, 1627, 2053, 2591, 3251, 4099, 5167, 6521, 8209, 10331,
			13007, 16411, 20663, 26017, 32771, 41299, 52021, 65537, 82571, 104033,
			131101, 165161, 208067, 262147, 330287, 416147, 524309, 660563,
			832291, 1048583, 1321139, 1664543, 2097169, 2642257, 3329023, 4194319,
			5284493, 6658049, 8388617, 10568993, 13316089, Limits<uint>::Max
		};

		uintptr uBins = (uintptr)(uElements / m_fOptimalLoad);
		uintptr uBinsEstimate = uBins;
		if( uBins > (uintptr)(Limits<uint>::Max) )
			uBinsEstimate = (uintptr)(Limits<uint>::Max);

		// Find the smallest prime greater than estimated value
		int iPrime = 0;
		while( uBinsEstimate > (uintptr)(c_Primes[iPrime]) )
			iPrime ++;

		if( c_Primes[iPrime] == Limits<uint>::Max )
			return uBinsEstimate;
		return (uintptr)(c_Primes[iPrime]);
	}
	void clear_bucket() throw()
	{
		assert( !m_mgr.IsNull() );
		if( m_ppBins != NULL ) {
			m_mgr.Deref().Free((uintptr)m_ppBins);
			m_ppBins = NULL;
		}
	}
	void alloc_bucket(_Node**& ppBins, uintptr uBins)  //may throw
	{
		assert( !m_mgr.IsNull() );
		assert( ppBins == NULL );
		//check overflow
		uintptr uBytes = SafeOperators::MultiplyThrow(uBins, sizeof(_Node*));  //may throw
		ppBins = (_Node**)(m_mgr.Deref().Allocate(uBytes));
		if( ppBins == NULL )
			throw OutOfMemoryException();
		mem_zero(ppBins, uBytes);
	}

	// rehash
	void update_rehash_thresholds() throw()
	{
		m_uHighRehashThreshold = (uintptr)(m_fHighThreshold * (float)m_uBins);
		m_uLowRehashThreshold = (uintptr)(m_fLowThreshold * (float)m_uBins);
		if( m_uLowRehashThreshold < 17 )
			m_uLowRehashThreshold = 0;
	}
	bool is_locked() const throw()
	{
		return m_uLockCount != 0;
	}
	void rehash(uintptr uBins)  //may throw
	{
		if( uBins == 0 )
			uBins = pick_bucket_size(m_uElements);
		if( uBins == m_uBins )
			return ;
		if( m_ppBins == NULL ) {
			// only set bucket size
			assert( m_uElements == 0 );  //must be empty
			m_uBins = uBins;
			update_rehash_thresholds();
			return ;
		}
		//transfer the old nodes into the new bins
		_Node** ppBins = NULL;
		alloc_bucket(ppBins, uBins);  //may throw
		// No copy. Rewrite the old nodes into the new bins.
		for( uintptr uSrcBin = 0; uSrcBin < m_uBins; uSrcBin ++ ) {
			_Node* pNode = m_ppBins[uSrcBin];
			while( pNode != NULL ) {
				_Node*  pNext;
				uintptr uDestBin;
				pNext = pNode->m_pNext;  //save the next node
				uDestBin = pNode->m_uHashCode % uBins;
				pNode->m_pNext = ppBins[uDestBin];
				ppBins[uDestBin] = pNode;
				pNode = pNext;
			}
		} //end for
		clear_bucket();
		m_ppBins = ppBins;
		m_uBins  = uBins;
		update_rehash_thresholds();
	}

	//nodes
	// tools
	void fill_new_node(_Node* pNewNode, uintptr uBin, uintptr uHash)  //may throw
	{
		pNewNode->m_uHashCode = uHash;
		m_uElements ++;
		pNewNode->m_pNext = m_ppBins[uBin];
		m_ppBins[uBin] = pNewNode;
		if( (m_uElements > m_uHighRehashThreshold) && !is_locked() )
			rehash(pick_bucket_size(m_uElements));  //may throw
	}
	void bucket_allocate()  //may throw
	{
		if( m_ppBins == NULL ) {
			assert( m_uElements == 0 );
			assert( m_uBins > 0 );
			alloc_bucket(m_ppBins, m_uBins);  //may throw
			update_rehash_thresholds();
		}
	}

	// create nodes
	_Node* create_node(const _Key& key, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode<_Key>(m_freelist, key);
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	_Node* create_node(_Key&& key, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode<_Key>(m_freelist, rv_forward(key));
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	template <typename V>
	_Node* create_node(const TKey& key, const V& v, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, key, v);
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	template <typename V>
	_Node* create_node(TKey&& key, V&& v, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, rv_forward(key), rv_forward(v));
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	_Node* create_node(const TPair& pair, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, pair);
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	_Node* create_node(TPair&& pair, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, rv_forward(pair));
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}

	// free
	void free_node(_Node* pNode) throw()
	{
		PairHelper<_Node, TPair>::DestructNode(m_freelist, pNode, m_uElements);
	}
	void remove_node(_Node* pNode, _Node* pPrev) throw()
	{
		assert( pNode != NULL );
		uintptr uBin = pNode->m_uHashCode % m_uBins;
		if( pPrev == NULL ) {
			assert( m_ppBins[uBin] == pNode );
			m_ppBins[uBin] = pNode->m_pNext;
		}
		else {
			assert( pPrev->m_pNext == pNode );
			pPrev->m_pNext = pNode->m_pNext;
		}
		free_node(pNode);
	}

	//find
	_Node* find_node(const TKey& key, uintptr& uBin, uintptr& uHash, _Node*& pPrev) const throw()
	{
		uHash = THashTrait::CalcHash(key);
		uBin  = uHash % m_uBins;
		if( m_ppBins == NULL )
			return NULL;
		_Node* pFollow = NULL;
		pPrev = NULL;
		for( _Node* pNode = m_ppBins[uBin]; pNode != NULL; pNode = pNode->m_pNext ) {
			if( (pNode->m_uHashCode == uHash) && TCompareTrait::IsEQ(KeyHelper::GetKey(pNode->m_t), key) ) {
				pPrev = pFollow;
				return pNode;
			}
			pFollow = pNode;
		} //end for
		return NULL;
	}
	_Node* to_next_node(_Node* pNode) const throw()
	{
		assert( pNode != NULL );
		_Node* pNext;
		if( pNode->m_pNext != NULL ) {
			pNext = pNode->m_pNext;
		}
		else {
			uintptr uBin = (pNode->m_uHashCode % m_uBins) + 1;
			pNext = NULL;
			while( (pNext == NULL) && (uBin < m_uBins) ) {
				pNext = m_ppBins[uBin];
				uBin ++;
			}
		} //end if
		return pNext;
	}

	//iterator
	Iterator get_iterator(_Node* pNode) const throw()
	{
		Iterator iter;
		iter.m_refTable = const_cast<thisClass*>(this);
		iter.m_refNode  = pNode;
		return iter;
	}

private:
	RefPtr<IMemoryManager>  m_mgr;
	//bucket
	_Node**  m_ppBins;
	uintptr  m_uBins;
	uintptr  m_uElements;
	//optimal
	float    m_fOptimalLoad;
	float    m_fLowThreshold;
	float    m_fHighThreshold;
	uintptr  m_uHighRehashThreshold;
	uintptr  m_uLowRehashThreshold;
	uintptr  m_uLockCount;
	//free list
	FreeList<_Node>  m_freelist;

private:
	//non-copyable
	_HashTable(const _HashTable&) throw();
	_HashTable& operator=(const _HashTable&) throw();
};

// HashList<TKey, THashTrait, TCompareTrait>

template <typename TKey, class THashTrait = DefaultHashTrait<TKey>, class TCompareTrait = DefaultCompareTrait<TKey>>
class HashList : public _HashTable<TKey, const TKey, THashTrait, TCompareTrait>
{
private:
	typedef _HashTable<TKey, const TKey, THashTrait, TCompareTrait>  baseClass;

public:
	HashList(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
			float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
			uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: baseClass(mgr, uBins, fOptimalLoad, fLowThreshold, fHighThreshold, uMinElements, uMaxElements)
	{
	}
	~HashList() throw()
	{
	}

private:
	Iterator FindNext(const Iterator& iter) const throw();

	//non-copyable
	HashList(const HashList&) throw();
	HashList& operator=(const HashList&) throw();
};

// HashMultiList<TKey, THashTrait, TCompareTrait>

template <typename TKey, class THashTrait = DefaultHashTrait<TKey>, class TCompareTrait = DefaultCompareTrait<TKey>>
class HashMultiList : public _HashTable<TKey, const TKey, THashTrait, TCompareTrait>
{
private:
	typedef _HashTable<TKey, const TKey, THashTrait, TCompareTrait>  baseClass;

public:
	HashMultiList(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
				float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
				uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
				: baseClass(mgr, uBins, fOptimalLoad, fLowThreshold, fHighThreshold, uMinElements, uMaxElements)
	{
	}
	~HashMultiList() throw()
	{
	}

	//add
	Iterator Insert(const TKey& key)  //may throw
	{
		return baseClass::InsertWithoutFind(key);
	}
	Iterator Insert(TKey&& key)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key));
	}

private:
	bool Remove(const TKey& key) throw();

	//non-copyable
	HashMultiList(const HashMultiList&) throw();
	HashMultiList& operator=(const HashMultiList&) throw();
};

// HashMap<TKey, TValue, THashTrait, TCompareTrait>

template <typename TKey, typename TValue, class THashTrait = DefaultHashTrait<TKey>, class TCompareTrait = DefaultCompareTrait<TKey>>
class HashMap : public _HashTable<TKey, Pair<const TKey, TValue>, THashTrait, TCompareTrait>
{
private:
	typedef HashMap<TKey, TValue, THashTrait, TCompareTrait>  thisClass;
	typedef Pair<const TKey, TValue>  pairClass;
	typedef _HashTable<TKey, pairClass, THashTrait, TCompareTrait>  baseClass;

public:
	HashMap(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
			float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
			uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: baseClass(mgr, uBins, fOptimalLoad, fLowThreshold, fHighThreshold, uMinElements, uMaxElements)
	{
	}
	~HashMap() throw()
	{
	}

	//add
	Iterator InsertWithoutFind(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::InsertWithoutFind(key, val);
	}
	Iterator InsertWithoutFind(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key), rv_forward(val));
	}
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::Insert(key, val);
	}
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::Insert(rv_forward(key), rv_forward(val));
	}

private:
	Iterator FindNext(const Iterator& iter) const throw();

	//non-copyable
	HashMap(const HashMap&) throw();
	HashMap& operator=(const HashMap&) throw();
};

// HashMultiMap<TKey, TValue, THashTrait, TCompareTrait>

template <typename TKey, typename TValue, class THashTrait = DefaultHashTrait<TKey>, class TCompareTrait = DefaultCompareTrait<TKey>>
class HashMultiMap : public _HashTable<TKey, Pair<const TKey, TValue>, THashTrait, TCompareTrait>
{
private:
	typedef HashMultiMap<TKey, TValue, THashTrait, TCompareTrait>  thisClass;
	typedef Pair<const TKey, TValue>  pairClass;
	typedef _HashTable<TKey, pairClass, THashTrait, TCompareTrait>  baseClass;

public:
	HashMultiMap(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
				float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
				uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
				: baseClass(mgr, uBins, fOptimalLoad, fLowThreshold, fHighThreshold, uMinElements, uMaxElements)
	{
	}
	~HashMultiMap() throw()
	{
	}

	//add
	Iterator InsertWithoutFind(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::InsertWithoutFind(key, val);
	}
	Iterator InsertWithoutFind(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key), rv_forward(val));
	}
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::InsertWithoutFind(key, val);
	}
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key), rv_forward(val));
	}

private:
	bool Remove(const TKey& key) throw();

	//non-copyable
	HashMultiMap(const HashMultiMap&) throw();
	HashMultiMap& operator=(const HashMultiMap&) throw();
};

//------------------------------------------------------------------------------
//RB Tree (Red-Black Tree)

//internal
// _RBTree<TKey, TPair, TCompareTrait>
//
//   The red-black tree code is based on the the descriptions in
//   "Introduction to Algorithms", by Cormen, Leiserson, and Rivest

template <typename TKey, class TPair, class TCompareTrait = DefaultCompareTrait<TKey>>
class _RBTree
{
private:
	typedef _RBTree<TKey, TPair, TCompareTrait>  thisClass;

	//special usage
	class _Key : public TKey
	{
	};

private:
	//internal node
	struct _Node
	{
		_Node()
		{
		}
		explicit _Node(const _Key& key) : m_t(static_cast<const TKey&>(key))
		{
		}
		explicit _Node(_Key&& key) : m_t(rv_forward(static_cast<TKey&>(key)))
		{
		}
		template <typename V>
		_Node(const TKey& key, const V& v) : m_t(key, v)
		{
		}
		template <typename V>
		_Node(TKey&& key, V&& v) : m_t(rv_forward(key), rv_forward(v))
		{
		}
		_Node(const TPair& t) : m_t(t)
		{
		}
		_Node(TPair&& t) : m_t(rv_forward(t))
		{
		}
		~_Node() throw()
		{
		}

		//parent
		const _Node*& get_Parent() const throw()
		{
			return m_pNext;
		}
		_Node*& get_Parent() throw()
		{
			return m_pNext;
		}

		enum {
			RB_RED = 0,  RB_BLACK
		};
		int    m_iColor;  //color, RB_*
		_Node* m_pNext;   //used as parent
		_Node* m_pLeft;
		_Node* m_pRight;
		TPair  m_t;

	private:
		_Node(const _Node&) throw();
	};

public:
	//iterator
	class Iterator
	{
	public:
		Iterator() throw()
		{
		}
		Iterator(const Iterator& src) throw() : m_refTree(src.m_refTree), m_refNode(src.m_refNode)
		{
		}
		~Iterator() throw()
		{
		}
		Iterator& operator=(const Iterator& src) throw()
		{
			if( &src != this ) {
				m_refTree = src.m_refTree;
				m_refNode = src.m_refNode;
			}
			return *this;
		}
		//properties
		const RefPtr<TPair> get_Ref() const throw()
		{
			return RefPtr<TPair>(m_refNode.Deref().m_t);
		}
		RefPtr<TPair> get_Ref() throw()
		{
			return RefPtr<TPair>(m_refNode.Deref().m_t);
		}
		const TPair& get_Value() const throw()
		{
			return m_refNode.Deref().m_t;
		}
		TPair& get_Value() throw()
		{
			return m_refNode.Deref().m_t;
		}
		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_refTree == right.m_refTree && m_refNode == right.m_refNode;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_refTree != right.m_refTree || m_refNode != right.m_refNode;
		}
		//next
		void MoveNext() throw()
		{
			m_refNode = m_refTree.Deref().to_successor_node(RefPtrHelper::GetInternalPointer(m_refNode));
		}
		//prev
		void MovePrev() throw()
		{
			//NULL node -> Tail
			m_refNode = ( m_refNode.IsNull() ) ? m_refTree.Deref().GetTail().m_refNode
												: m_refTree.Deref().to_predecessor_node(RefPtrHelper::GetInternalPointer(m_refNode));
		}

	private:
		RefPtr<thisClass>  m_refTree;
		RefPtr<_Node>      m_refNode;

		friend class thisClass;
	};

public:
	_RBTree(const RefPtr<IMemoryManager>& mgr,
			uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_mgr(mgr), m_pRoot(NULL), m_uElements(0), m_pNil(NULL),
			m_freelist(mgr, uMinElements, uMaxElements)
	{
	}
	~_RBTree() throw()
	{
		RemoveAll();
		if( m_pNil != NULL )
			m_mgr.Deref().Free(m_pNil);
	}

	uintptr GetCount() const throw()
	{
		return m_uElements;
	}
	bool IsEmpty() const throw()
	{
		return m_uElements == 0;
	}

	//iterator
	const Iterator GetHead() const throw()
	{
		return to_minimum_node(m_pRoot);
	}
	Iterator GetHead() throw()
	{
		return to_minimum_node(m_pRoot);
	}
	const Iterator GetTail() const throw()
	{
		return to_maximum_node(m_pRoot);
	}
	Iterator GetTail() throw()
	{
		return to_maximum_node(m_pRoot);
	}
	const Iterator GetBegin() const throw()
	{
		return GetHead();
	}
	Iterator GetBegin() throw()
	{
		return GetHead();
	}
	const Iterator GetEnd() const throw()
	{
		return get_iterator(NULL);
	}
	Iterator GetEnd() throw()
	{
		return get_iterator(NULL);
	}
	const ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	ReverseIterator<Iterator> GetReverseBegin() throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	const ReverseIterator<Iterator> GetReverseEnd() const throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}
	ReverseIterator<Iterator> GetReverseEnd() throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}

	//methods
	void RemoveAll() throw()
	{
		if( !is_node_nil(m_pRoot) )
			remove_by_post_order(m_pRoot);
		m_uElements = 0;
		//free list
		m_freelist.Clear();
		//root
		m_pRoot = m_pNil;
	}

	//find
	Iterator Find(const TKey& key) const throw()
	{
		_Node* pKey = find_node(key);
		return get_iterator(pKey);
	}
	Iterator FindFirstKeyAfter(const TKey& key) const throw()
	{
		_Node* pParent;
		_Node* pKey = find_node_from_root(key, pParent);
		if( pKey != NULL ) {
			// We found a node with the exact key, so find the first node after
			// this one with a different key
			while( 1 ) {
				_Node* pNext = to_successor_node(pKey);
				pKey = pNext;
				if( (pNext != NULL) && TCompareTrait::IsEQ(key, KeyHelper::GetKey(pNext->m_t)) ) {
					;  //nothing
				}
				else {
					break;
				}
			}
		}
		else if( pParent != NULL ) {
			// No node matched the key exactly, so pick the first node with
			// a key greater than the given key
			if( TCompareTrait::IsLT(key, KeyHelper::GetKey(pParent->m_t)) )
				pKey = pParent;
			else //key must not be the same as the key of pParent->m_t
				pKey = to_successor_node(pParent);
		}
		return get_iterator(pKey);
	}
	//find next key
	Iterator FindNext(const Iterator& iter) const throw()
	{
		assert( iter != GetEnd() );
		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		const TKey& key = KeyHelper::GetKey<const TKey>(pNode->m_t);
		pNode = to_successor_node(pNode);
		if( pNode == NULL || !TCompareTrait::IsEQ(key, KeyHelper::GetKey(pNode->m_t)) )
			return get_iterator(NULL);
		return get_iterator(pNode);
	}

	//add
	Iterator InsertWithoutFind(const TKey& key)  //may throw
	{
		_Node* pNewNode = create_node(static_cast<const _Key&>(key));
		return get_iterator(pNewNode);
	}
	Iterator InsertWithoutFind(TKey&& key)  //may throw
	{
		_Node* pNewNode = create_node(rv_forward(static_cast<_Key&>(key)));
		return get_iterator(pNewNode);
	}
	template <typename TValue>
	Iterator InsertWithoutFind(const TKey& key, const TValue& val)  //may throw
	{
		_Node* pNewNode = create_node(key, val);
		return get_iterator(pNewNode);
	}
	template <typename TValue>
	Iterator InsertWithoutFind(TKey&& key, TValue&& val)  //may throw
	{
		_Node* pNewNode = create_node(rv_forward(key), rv_forward(val));
		return get_iterator(pNewNode);
	}

	Iterator Insert(const TKey& key)  //may throw
	{
		_Node* pNode = find_node(key);
		if( pNode == NULL )
			pNode = create_node(static_cast<const _Key&>(key));
		return get_iterator(pNode);
	}
	Iterator Insert(TKey&& key)  //may throw
	{
		_Node* pNode = find_node(key);
		if( pNode == NULL )
			pNode = create_node(rv_forward(static_cast<_Key&>(key)));
		return get_iterator(pNode);
	}
	template <typename TValue>
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		_Node* pNode = find_node(key);
		if( pNode == NULL )
			pNode = create_node(key, val);
		else
			pNode->m_t.set_Second(val);
		return get_iterator(pNode);
	}
	template <typename TValue>
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		_Node* pNode = find_node(key);
		if( pNode == NULL )
			pNode = create_node(rv_forward(key), rv_forward(val));
		else
			pNode->m_t.set_Second(rv_forward(val));
		return get_iterator(pNode);
	}

	//remove
	bool Remove(const TKey& key) throw()
	{
		_Node* pNode = find_node(key);
		if( pNode == NULL )
			return false;
		delete_node(pNode);
		return true;
	}
	void RemoveAt(const Iterator& iter) throw()
	{
		assert( iter != GetEnd() );
		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_refNode));
		delete_node(pNode);
	}

protected:
	//sentinel node
	bool is_node_nil(_Node* p) const throw()
	{
		return p == m_pNil;
	}
	void set_node_nil(_Node*& p) const throw()
	{
		p = m_pNil;
	}

	//tools
	void fill_new_node(_Node* pNewNode) throw()
	{
		pNewNode->m_iColor = _Node::RB_RED;
		set_node_nil(pNewNode->m_pLeft);
		set_node_nil(pNewNode->m_pRight);
		set_node_nil(pNewNode->get_Parent());
		m_uElements ++;
		assert( m_uElements > 0 );
	}
	void nil_node_allocate()  //may throw
	{
		assert( !m_mgr.IsNull() );
		if( m_pNil == NULL ) {
			m_pNil = (_Node*)(m_mgr.Deref().Allocate(sizeof(_Node)));
			if( m_pNil == NULL )
				throw OutOfMemoryException();
			mem_zero(m_pNil, sizeof(_Node));
			m_pNil->m_iColor = _Node::RB_BLACK;
			m_pNil->get_Parent() = m_pNil->m_pLeft = m_pNil->m_pRight = m_pNil;
			m_pRoot = m_pNil;
		}
	}
	void insert_impl(_Node* pNewNode) throw()
	{
		const TKey& key = KeyHelper::GetKey(pNewNode->m_t);
		_Node* pY = NULL;
		_Node* pX = m_pRoot;
		while( !is_node_nil(pX) ) {
			pY = pX;
			if( TCompareTrait::IsLE(key, KeyHelper::GetKey(pX->m_t)) )
				pX = pX->m_pLeft;
			else
				pX = pX->m_pRight;
		}
		pNewNode->get_Parent() = pY;
		if( pY == NULL )
			m_pRoot = pNewNode;
		else if( TCompareTrait::IsLE(key, KeyHelper::GetKey(pY->m_t)) )
			pY->m_pLeft = pNewNode;
		else
			pY->m_pRight = pNewNode;
	}
	void insert_impl2(_Node* pNewNode) throw()
	{
		_Node* pX = pNewNode;
		pX->m_iColor = _Node::RB_RED;
		_Node* pY = NULL;
		while( pX != m_pRoot && pX->get_Parent()->m_iColor == _Node::RB_RED ) {
			if( pX->get_Parent() == pX->get_Parent()->get_Parent()->m_pLeft ) {
				pY = pX->get_Parent()->get_Parent()->m_pRight;
				if( pY != NULL && pY->m_iColor == _Node::RB_RED ) {
					pX->get_Parent()->m_iColor = _Node::RB_BLACK;
					pY->m_iColor = _Node::RB_BLACK;
					pX->get_Parent()->get_Parent()->m_iColor = _Node::RB_RED;
					pX = pX->get_Parent()->get_Parent();
				}
				else {
					if( pX == pX->get_Parent()->m_pRight ) {
						pX = pX->get_Parent();
						left_rotate(pX);
					}
					pX->get_Parent()->m_iColor = _Node::RB_BLACK;
					pX->get_Parent()->get_Parent()->m_iColor = _Node::RB_RED;
					right_rotate(pX->get_Parent()->get_Parent());
				}
			}
			else {
				pY = pX->get_Parent()->get_Parent()->m_pLeft;
				if( pY != NULL && pY->m_iColor == CNode::RB_RED ) {
					pX->get_Parent()->m_iColor = _Node::RB_BLACK;
					pY->m_iColor = _Node::RB_BLACK;
					pX->get_Parent()->get_Parent()->m_iColor = _Node::RB_RED;
					pX = pX->get_Parent()->get_Parent();
				}
				else {
					if( pX == pX->get_Parent()->m_pLeft ) {
						pX = pX->get_Parent();
						right_rotate(pX);
					}
					pX->get_Parent()->m_iColor = _Node::RB_BLACK;
					pX->get_Parent()->get_Parent()->m_iColor = _Node::RB_RED;
					left_rotate(pX->get_Parent()->get_Parent());
				}
			}
		} //end while
		m_pRoot->m_iColor = _Node::RB_BLACK;
		set_node_nil(m_pRoot->get_Parent());
	}

	//nodes
	_Node* create_node(const _Key& key)  //may throw
	{
		nil_node_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode<_Key>(m_freelist, key);
		fill_new_node(pNewNode);
		insert_impl(pNewNode);
		insert_impl2(pNewNode);
		return pNewNode;
	}
	_Node* create_node(_Key&& key)  //may throw
	{
		nil_node_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode<_Key>(m_freelist, rv_forward(key));
		fill_new_node(pNewNode);
		insert_impl(pNewNode);
		insert_impl2(pNewNode);
		return pNewNode;
	}
	template <typename V>
	_Node* create_node(const TKey& key, const V& v)
	{
		nil_node_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, key, v);
		fill_new_node(pNewNode);
		insert_impl(pNewNode);
		insert_impl2(pNewNode);
		return pNewNode;
	}
	template <typename V>
	_Node* create_node(TKey&& key, V&& v)
	{
		nil_node_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, rv_forward(key), rv_forward(v));
		fill_new_node(pNewNode);
		insert_impl(pNewNode);
		insert_impl2(pNewNode);
		return pNewNode;
	}
	_Node* create_node(const TPair& pair)
	{
		nil_node_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, pair);
		fill_new_node(pNewNode);
		insert_impl(pNewNode);
		insert_impl2(pNewNode);
		return pNewNode;
	}
	_Node* create_node(TPair&& pair)
	{
		nil_node_allocate();
		_Node* pNewNode = PairHelper<_Node, TPair>::ConstructNode(m_freelist, rv_forward(pair));
		fill_new_node(pNewNode);
		insert_impl(pNewNode);
		insert_impl2(pNewNode);
		return pNewNode;
	}

	//free
	void free_node(_Node* pNode) throw()
	{
		PairHelper<_Node, TPair>::DestructNode(m_freelist, pNode, m_uElements);
	}
	void remove_by_post_order(_Node* pNode) throw()
	{
		if( is_node_nil(pNode) )
			return ;
		//recursion
		remove_by_post_order(pNode->m_pLeft);
		remove_by_post_order(pNode->m_pRight);
		free_node(pNode);
	}
	void delete_node_with_fixup(_Node* pNode) throw()
	{
		assert( pNode != NULL );
		_Node* pX = pNode;
		_Node* pW = NULL;
		while( (pX != m_pRoot) && (pX->m_iColor == _Node::RB_BLACK) ) {
			if( pX == pX->get_Parent()->m_pLeft ) {
				pW = pX->get_Parent()->m_pRight;
				if( pW->m_iColor == _Node::RB_RED ) {
					pW->m_iColor = _Node::RB_BLACK;
					pW->get_Parent()->m_iColor = _Node::RB_RED;
					left_rotate(pX->get_Parent());
					pW = pX->get_Parent()->m_pRight;
				}
				if( pW->m_pLeft->m_iColor == _Node::RB_BLACK && pW->m_pRight->m_iColor == _Node::RB_BLACK ) {
					pW->m_iColor = _Node::RB_RED;
					pX = pX->get_Parent();
				}
				else {
					if( pW->m_pRight->m_iColor == _Node::RB_BLACK) {
						pW->m_pLeft->m_iColor = _Node::RB_BLACK;
						pW->m_iColor = _Node::RB_RED;
						right_rotate(pW);
						pW = pX->get_Parent()->m_pRight;
					}
					pW->m_iColor = pX->get_Parent()->m_iColor;
					pX->get_Parent()->m_iColor = _Node::RB_BLACK;
					pW->m_pRight->m_iColor = _Node::RB_BLACK;
					left_rotate(pX->get_Parent());
					pX = m_pRoot;
				}
			}
			else {
				pW = pX->get_Parent()->m_pLeft;
				if( pW->m_iColor == _Node::RB_RED ) {
					pW->m_iColor = _Node::RB_BLACK;
					pW->get_Parent()->m_iColor = _Node::RB_RED;
					right_rotate(pX->get_Parent());
					pW = pX->get_Parent()->m_pLeft;
				}
				if( pW->m_pRight->m_iColor == _Node::RB_BLACK && pW->m_pLeft->m_iColor == _Node::RB_BLACK ) {
					pW->m_iColor = _Node::RB_RED;
					pX = pX->get_Parent();
				}
				else {
					if( pW->m_pLeft->m_iColor == _Node::RB_BLACK ) {
						pW->m_pRight->m_iColor = _Node::RB_BLACK;
						pW->m_iColor = _Node::RB_RED;
						left_rotate(pW);
						pW = pX->get_Parent()->m_pLeft;
					}
					pW->m_iColor = pX->get_Parent()->m_iColor;
					pX->get_Parent()->m_iColor = _Node::RB_BLACK;
					pW->m_pLeft->m_iColor = _Node::RB_BLACK;
					right_rotate(pX->get_Parent());
					pX = m_pRoot;
				}
			}
		} //end while
		pX->m_iColor = _Node::RB_BLACK;
	}
	bool delete_node(_Node* pNode) throw()
	{
		if( pNode == NULL )
			return false;
		_Node* pY = NULL;
		_Node* pX = NULL;
		if( is_node_nil(pNode->m_pLeft) || is_node_nil(pNode->m_pRight) )
			pY = pNode;
		else
			pY = to_successor_node(pNode);
		if( !is_node_nil(pY->m_pLeft) )
			pX = pY->m_pLeft;
		else
			pX = pY->m_pRight;
		pX->get_Parent() = pY->get_Parent();
		if( is_node_nil(pY->get_Parent()) )
			m_pRoot = pX;
		else if( pY == pY->get_Parent()->m_pLeft )
			pY->get_Parent()->m_pLeft = pX;
		else
			pY->get_Parent()->m_pRight = pX;
		if( pY->m_iColor == _Node::RB_BLACK )
			delete_node_with_fixup(pX);
		if( pY != pNode )
			replace_node(pY, pNode);
		if( m_pRoot != NULL )
			set_node_nil(m_pRoot->get_Parent());
		free_node(pNode);
		return true;
	}

	//replace
	void replace_node(_Node* pDest, _Node* pSrc) throw()
	{
		assert( pDest != NULL );
		assert( pSrc != NULL );
		pDest->get_Parent() = pSrc->get_Parent();
		if( pSrc->get_Parent()->m_pLeft == pSrc )
			pSrc->get_Parent()->m_pLeft = pDest;
		else
			pSrc->get_Parent()->m_pRight = pDest;
		pDest->m_pRight = pSrc->m_pRight;
		pDest->m_pLeft = pSrc->m_pLeft;
		pDest->m_iColor = pSrc->m_iColor;
		pDest->m_pRight->get_Parent() = pDest;
		pDest->m_pLeft->get_Parent() = pDest;
		if( m_pRoot == pSrc )
			m_pRoot = pDest;
	}
	//rotate
	_Node* left_rotate(_Node* pNode) throw()
	{
		assert( pNode != NULL );
		_Node* pRight = pNode->m_pRight;
		pNode->m_pRight = pRight->m_pLeft;
		if( !is_node_nil(pRight->m_pLeft) )
			pRight->m_pLeft->get_Parent() = pNode;
		pRight->get_Parent() = pNode->get_Parent();
		if( is_node_nil(pNode->get_Parent()) )
			m_pRoot = pRight;
		else if( pNode == pNode->get_Parent()->m_pLeft )
			pNode->get_Parent()->m_pLeft = pRight;
		else
			pNode->get_Parent()->m_pRight = pRight;
		pRight->m_pLeft = pNode;
		pNode->get_Parent() = pRight;
		return pNode;
	}
	_Node* right_rotate(_Node* pNode) throw()
	{
		assert( pNode != NULL );
		_Node* pLeft = pNode->m_pLeft;
		pNode->m_pLeft = pLeft->m_pRight;
		if( !is_node_nil(pLeft->m_pRight) )
			pLeft->m_pRight->get_Parent() = pNode;
		pLeft->get_Parent() = pNode->get_Parent();
		if( is_node_nil(pNode->get_Parent()) )
			m_pRoot = pLeft;
		else if( pNode == pNode->get_Parent()->m_pRight )
			pNode->get_Parent()->m_pRight = pLeft;
		else
			pNode->get_Parent()->m_pLeft = pLeft;
		pLeft->m_pRight = pNode;
		pNode->get_Parent() = pLeft;
		return pNode;
	}

	//find
	_Node* find_node_from_root(const TKey& key, _Node*& pParent) const throw()
	{
		//attempt to find a node that matches the key exactly
		pParent = NULL;
		_Node* pKey = NULL;
		_Node* pNode = m_pRoot;
		while( !is_node_nil(pNode) && (pKey == NULL) ) {
			pParent = pNode;
			int res = TCompareTrait::Compare(key, KeyHelper::GetKey(pNode->m_t));
			if( res == 0 )
				pKey = pNode;
			else if( res < 0 )
				pNode = pNode->m_pLeft;
			else
				pNode = pNode->m_pRight;
		} //end while
		return pKey;
	}
	_Node* to_minimum_node(_Node* pNode) const throw()
	{
		if( pNode == NULL || is_node_nil(pNode) )
			return NULL;
		_Node* pMin = pNode;
		while( !is_node_nil(pMin->m_pLeft) )
			pMin = pMin->m_pLeft;
		return pMin;
	}
	_Node* to_maximum_node(_Node* pNode) const throw()
	{
		if( pNode == NULL || is_node_nil(pNode) )
			return NULL;
		_Node* pMax = pNode;
		while( !is_node_nil(pMax->m_pRight) )
			pMax = pMax->m_pRight;
		return pMax;
	}
	_Node* to_predecessor_node(_Node* pNode) const throw()
	{
		if( pNode == NULL )
			return NULL;
		if( !is_node_nil(pNode->m_pLeft) )
			return to_maximum_node(pNode->m_pLeft);
		_Node* pParent = pNode->get_Parent();
		_Node* pLeft   = pNode;
		while( !is_node_nil(pParent) && (pLeft == pParent->m_pLeft) ) {
			pLeft   = pParent;
			pParent = pParent->get_Parent();
		}
		if( is_node_nil(pParent) )
			pParent = NULL;
		return pParent;
	}
	_Node* to_successor_node(_Node* pNode) const throw()
	{
		if( pNode == NULL )
			return NULL;
		if( !is_node_nil(pNode->m_pRight) )
			return to_minimum_node(pNode->m_pRight);
		_Node* pParent = pNode->get_Parent();
		_Node* pRight  = pNode;
		while( !is_node_nil(pParent) && (pRight == pParent->m_pRight) ) {
			pRight  = pParent;
			pParent = pParent->get_Parent();
		}
		if( is_node_nil(pParent) )
			pParent = NULL;
		return pParent;
	}
	_Node* find_node(const TKey& key) const throw()
	{
		_Node* pParent;
		_Node* pKey = find_node_from_root(key, pParent);
		if( pKey == NULL )
			return NULL;
		//find first
		while( 1 ) {
			_Node* pPrev = to_predecessor_node(pKey);
			if( (pPrev != NULL) && TCompareTrait::IsEQ(key, KeyHelper::GetKey(pPrev->m_t)) )
				pKey = pPrev;
			else
				break;
		} //end while
		return pKey;
	}

	//iterator
	Iterator get_iterator(_Node* pNode) const throw()
	{
		Iterator iter;
		iter.m_refTree = this;
		iter.m_refNode = pNode;
		return iter;
	}

private:
	RefPtr<IMemoryManager>  m_mgr;
	//tree
	_Node*   m_pRoot;
	uintptr  m_uElements;
	//sentinel node
	_Node*   m_pNil;
	//free list
	FreeList<_Node>  m_freelist;

private:
	//non-copyable
	_RBTree(const _RBTree&) throw();
	_RBTree& operator=(const _RBTree&) throw();
};

// RBList<TKey, TCompareTrait>

template <typename TKey, class TCompareTrait = DefaultCompareTrait<TKey>>
class RBList : public _RBTree<TKey, const TKey, TCompareTrait>
{
private:
	typedef _RBTree<TKey, const TKey, TCompareTrait>  baseClass;

public:
	RBList(const RefPtr<IMemoryManager>& mgr,
			uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: baseClass(mgr, uMinElements, uMaxElements)
	{
	}
	~RBList() throw()
	{
	}

private:
	Iterator FindNext(const Iterator& iter) const throw();

	//non-copyable
	RBList(const RBList&) throw();
	RBList& operator=(const RBList&) throw();
};

// RBMultiList<TKey, TCompareTrait>

template <typename TKey, class TCompareTrait = DefaultCompareTrait<TKey>>
class RBMultiList : public _RBTree<TKey, const TKey, TCompareTrait>
{
private:
	typedef _RBTree<TKey, const TKey, TCompareTrait>  baseClass;

public:
	RBMultiList(const RefPtr<IMemoryManager>& mgr,
				uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
				: baseClass(mgr, uMinElements, uMaxElements)
	{
	}
	~RBMultiList() throw()
	{
	}

	//add
	Iterator Insert(const TKey& key)  //may throw
	{
		return baseClass::InsertWithoutFind(key);
	}
	Iterator Insert(TKey&& key)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key));
	}

private:
	bool Remove(const TKey& key) throw();

	//non-copyable
	RBMultiList(const RBMultiList&) throw();
	RBMultiList& operator=(const RBMultiList&) throw();
};

// RBMap<TKey, TValue, TCompareTrait>

template <typename TKey, typename TValue, class TCompareTrait = DefaultCompareTrait<TKey>>
class RBMap : public _RBTree<TKey, Pair<const TKey, TValue>, TCompareTrait>
{
private:
	typedef RBMap<TKey, TValue, TCompareTrait>  thisClass;
	typedef Pair<const TKey, TValue>  pairClass;
	typedef _RBTree<TKey, pairClass, TCompareTrait>  baseClass;

public:
	RBMap(const RefPtr<IMemoryManager>& mgr,
		uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
		: baseClass(mgr, uMinElements, uMaxElements)
	{
	}
	~RBMap() throw()
	{
	}

	//add
	Iterator InsertWithoutFind(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::InsertWithoutFind(key, val);
	}
	Iterator InsertWithoutFind(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key), rv_forward(val));
	}
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::Insert(key, val);
	}
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::Insert(rv_forward(key), rv_forward(val));
	}

private:
	Iterator FindNext(const Iterator& iter) const throw();

	//non-copyable
	RBMap(const RBMap&) throw();
	RBMap& operator=(const RBMap&) throw();
};

// RBMultiMap<TKey, TValue, TCompareTrait>

template <typename TKey, typename TValue, class TCompareTrait = DefaultCompareTrait<TKey>>
class RBMultiMap : public _RBTree<TKey, Pair<const TKey, TValue>, TCompareTrait>
{
private:
	typedef RBMultiMap<TKey, TValue, TCompareTrait>  thisClass;
	typedef Pair<const TKey, TValue>  pairClass;
	typedef _RBTree<TKey, pairClass, TCompareTrait>  baseClass;

public:
	RBMultiMap(const RefPtr<IMemoryManager>& mgr,
			uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: baseClass(mgr, uMinElements, uMaxElements)
	{
	}
	~RBMultiMap() throw()
	{
	}

	//add
	Iterator InsertWithoutFind(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::InsertWithoutFind(key, val);
	}
	Iterator InsertWithoutFind(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key), rv_forward(val));
	}
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		return baseClass::InsertWithoutFind(key, val);
	}
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		return baseClass::InsertWithoutFind(rv_forward(key), rv_forward(val));
	}

private:
	bool Remove(const TKey& key) throw();

	//non-copyable
	RBMultiMap(const RBMultiMap&) throw();
	RBMultiMap& operator=(const RBMultiMap&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COLL_H__
////////////////////////////////////////////////////////////////////////////////
