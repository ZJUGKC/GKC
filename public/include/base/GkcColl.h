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

// SingleList<T, TCompareTrait>

template <typename T, class TCompareTrait = GKC::CompareTrait<T>>
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
		Itertaor& operator=(const Iterator& src) throw()
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
		m_pHead = NULL;
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
	_Node* pick_free_node()  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(const T& t)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, t);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(T&& t)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(t));  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	void fill_new_node(_Node* pNewNode, _Node* pNext) throw()
	{
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );
	}

	// nodes
	_Node* new_node(_Node* pNext)  //may throw
	{
		_Node* pNewNode = pick_free_node();
		fill_new_node(pNewNode, pNext);
		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pNext)  //may throw
	{
		_Node* pNewNode = pick_free_node(t);
		fill_new_node(pNewNode, pNext);
		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pNext)  //may throw
	{
		_Node* pNewNode = pick_free_node(rv_forward(t));
		fill_new_node(pNewNode, pNext);
		return pNewNode;
	}

	void free_node(_Node* pNode) throw()
	{
		pNode->~_Node();
		m_freelist.PutFreeNode(pNode);
		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 ) {
			RemoveAll();
		}
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

template <typename T, class TCompareTrait = GKC::CompareTrait<T>>
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
		Itertaor& operator=(const Iterator& src) throw()
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
		m_pHead = NULL;
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
	_Node* pick_free_node()  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(const T& t)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, t);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(T&& t)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(t));  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
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
		_Node* pNewNode = pick_free_node();
		fill_new_node(pNewNode, pPrev, pNext);
		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pPrev, _Node* pNext)  //may throw
	{
		_Node* pNewNode = pick_free_node(t);
		fill_new_node(pNewNode, pPrev, pNext);
		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pPrev, _Node* pNext)  //may throw
	{
		_Node* pNewNode = pick_free_node(rv_forward(t));
		fill_new_node(pNewNode, pPrev, pNext);
		return pNewNode;
	}

	void free_node(_Node* pNode) throw()
	{
		pNode->~_Node();
		m_freelist.PutFreeNode(pNode);
		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 ) {
			RemoveAll();
		}
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

template <typename TKey, class TPair, class THashTrait = GKC::HashTrait<TKey>, class TCompareTrait = GKC::CompareTrait<TKey>>
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
		Itertaor& operator=(const Iterator& src) throw()
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
	//add
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
			m_mgr.Deref().Free(m_ppBins);
			m_ppBins = NULL;
		}
	}
	static void alloc_bucket(_Node**& ppBins, uintptr uBins)  //may throw
	{
		assert( !m_mgr.IsNull() );
		assert( ppBins == NULL );
		//check overflow
		uintptr uBytes = SafeOperators::Multiply(uBins, sizeof(_Node*));  //may throw
		ppBins = m_mgr.Deref().Allocate(uBytes);
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
				ppBins[iDestBin] = pNode;
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
	_Node* pick_free_node(const _Key& key)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, key);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(_Key&& key)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(key));  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	template <typename V>
	_Node* pick_free_node(const TKey& key, const V& v)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, key, v);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	template <typename V>
	_Node* pick_free_node(TKey&& key, V&& v)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(key), rv_forward(v));  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(const TPair& pair)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, pair);  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
	_Node* pick_free_node(TPair&& pair)  //may throw
	{
		_Node* pNewNode = m_freelist.GetFreeNode();
		call_constructor(*pNewNode, rv_forward(pair));  //may throw
		m_freelist.PickFreeNode();
		return pNewNode;
	}
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
		_Node* pNewNode = pick_free_node(key);
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	_Node* create_node(_Key&& key, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = pick_free_node(rv_forward(key));
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	template <typename V>
	_Node* create_node(const TKey& key, const V& v, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = pick_free_node(key, v);
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	template <typename V>
	_Node* create_node(TKey&& key, V&& v, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = pick_free_node(rv_forward(key), rv_forward(v));
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	_Node* create_node(const TPair& pair, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = pick_free_node(pair);
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}
	_Node* create_node(TPair&& pair, uintptr uBin, uintptr uHash)
	{
		bucket_allocate();
		_Node* pNewNode = pick_free_node(rv_forward(pair));
		fill_new_node(pNewNode, uBin, uHash);
		return pNewNode;
	}

	// free
	void free_node(_Node* pNode) throw()
	{
		assert( pNode != NULL );
		pNode->~_Node();
		m_freelist.PutFreeNode(pNode);

		assert( m_uElements > 0 );
		m_uElements --;

		if( m_uElements == 0 )
			m_freelist.Clear();
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
		uHash = THashTraits::CalcHash(key);
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
		iter.m_refTable = this;
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

// _HashMultiTable<TKey, TPair, THashTrait, TCompareTrait>

template <typename TKey, class TPair, class THashTrait = GKC::HashTrait<TKey>, class TCompareTrait = GKC::CompareTrait<TKey>>
class _HashMultiTable : public _HashTable<TKey, TPair, THashTrait, TCompareTrait>
{
private:
	typedef _HashMultiTable<TKey, TPair, THashTrait, TCompareTrait>  thisClass;
	typedef _HashTable<TKey, TPair, THashTrait, TCompareTrait>  baseClass;

public:
	_HashMultiTable(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
					float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
					uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
					: baseClass(mgr, uBins, fOptimalLoad, fLowThreshold, fHighThreshold, uMinElements, uMaxElements)
	{
	}
	~_HashMultiTable() throw()
	{
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
	Iterator Insert(const TKey& key)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(static_cast<const _Key&>(key), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	Iterator Insert(TKey&& key)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(rv_forward(static_cast<_Key&>(key)), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}

private:
	bool Remove(const TKey& key) throw();

	//non-copyable
	_HashMultiTable(const _HashMultiTable&) throw();
	_HashMultiTable& operator=(const _HashMultiTable&) throw();
};

// HashList<TKey, THashTrait, TCompareTrait>

template <typename TKey, class THashTrait = GKC::HashTrait<T>, class TCompareTrait = GKC::CompareTrait<T>>
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
	~_HashList() throw()
	{
	}

private:
	//non-copyable
	HashList(const HashList&) throw();
	HashList& operator=(const HashList&) throw();
};

// HashMultiList<TKey, THashTrait, TCompareTrait>

template <typename TKey, class THashTrait = GKC::HashTrait<T>, class TCompareTrait = GKC::CompareTrait<T>>
class HashMultiList : public _HashMultiTable<TKey, const TKey, THashTrait, TCompareTrait>
{
private:
	typedef _HashMultiTable<TKey, const TKey, THashTrait, TCompareTrait>  baseClass;

public:
	HashMultiList(const RefPtr<IMemoryManager>& mgr, uintptr uBins = 17,
				float fOptimalLoad = 0.75f, float fLowThreshold = 0.25f, float fHighThreshold = 2.25f,
				uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
				: baseClass(mgr, uBins, fOptimalLoad, fLowThreshold, fHighThreshold, uMinElements, uMaxElements)
	{
	}
	~_HashMultiList() throw()
	{
	}

private:
	//non-copyable
	HashMultiList(const HashMultiList&) throw();
	HashMultiList& operator=(const HashMultiList&) throw();
};

// HashMap<TKey, TValue, THashTrait, TCompareTrait>

template <typename TKey, typename TValue, class THashTrait = GKC::HashTrait<T>, class TCompareTrait = GKC::CompareTrait<T>>
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

private:
	//non-copyable
	HashMap(const HashMap&) throw();
	HashMap& operator=(const HashMap&) throw();
};

// HashMultiMap<TKey, TValue, THashTrait, TCompareTrait>

template <typename TKey, typename TValue, class THashTrait = GKC::HashTrait<T>, class TCompareTrait = GKC::CompareTrait<T>>
class HashMultiMap : public _HashMultiTable<TKey, Pair<const TKey, TValue>, THashTrait, TCompareTrait>
{
private:
	typedef HashMultiMap<TKey, TValue, THashTrait, TCompareTrait>  thisClass;
	typedef Pair<const TKey, TValue>  pairClass;
	typedef _HashMultiTable<TKey, pairClass, THashTrait, TCompareTrait>  baseClass;

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
	Iterator Insert(const TKey& key, const TValue& val)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(key, val, uBin, uHash);  //may throw
		return get_iterator(pNode);
	}
	Iterator Insert(TKey&& key, TValue&& val)  //may throw
	{
		uintptr uHash = THashTraits::CalcHash(key);
		uintptr uBin  = uHash % m_uBins;
		_Node* pNode = create_node(rv_forward(key), rv_forward(val), uBin, uHash);  //may throw
		return get_iterator(pNode);
	}

private:
	//non-copyable
	HashMultiMap(const HashMultiMap&) throw();
	HashMultiMap& operator=(const HashMultiMap&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COLL_H__
////////////////////////////////////////////////////////////////////////////////
