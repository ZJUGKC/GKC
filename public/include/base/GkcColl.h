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

// SingleList<T>

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
		Iterator(const RefPtr<thisClass>& list, const RefPtr<_Node>& node) throw() : m_refList(list), m_refNode(node)
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

	private:
		RefPtr<thisClass>  m_refList;
		RefPtr<_Node>      m_refNode;

		friend class thisClass;
	};

public:
	SingleList(const RefPtr<IMemoryManager>& mgr, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
			: m_pHead(NULL), m_uElements(0), m_pool(mgr), m_pFree(NULL),
			m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements );
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
	Iterator GetHead() const throw()
	{
		return get_iterator(m_pHead);
	}
	Iterator GetBegin() const throw()
	{
		return GetHead();
	}
	Iterator GetEnd() const throw()
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
		m_pFree = NULL;
		m_pool.FreeDataChain();
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
	void get_free_node()  //may throw
	{
		if( m_pFree == NULL ) {
			uintptr uActElements;
			//may throw
			_Node* pNode = (_Node*)m_pool.CreateBlock(m_uMinBlockElements, m_uMaxBlockElements, sizeof(_Node), uActElements);
			pNode += (uActElements - 1);
			for( uintptr uBlock = uActElements; uBlock > 0; uBlock -- ) {
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				pNode --;
			}
		}
		assert( m_pFree != NULL );
	}
	_Node* new_node(_Node* pNext)  //may throw
	{
		get_free_node();

		_Node* pNewNode = m_pFree;
		_Node* pNextFree = m_pFree->m_pNext;

		call_constructor(*pNewNode);  //may throw

		m_pFree = pNextFree;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );

		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pNext)  //may throw
	{
		get_free_node();

		_Node* pNewNode = m_pFree;
		_Node* pNextFree = m_pFree->m_pNext;

		call_constructor(*pNewNode, t);  //may throw

		m_pFree = pNextFree;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );

		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pNext)  //may throw
	{
		get_free_node();

		_Node* pNewNode = m_pFree;
		_Node* pNextFree = m_pFree->m_pNext;

		call_constructor(*pNewNode, rv_forward(t));  //may throw

		m_pFree = pNextFree;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );

		return pNewNode;
	}

	void free_node(_Node* pNode) throw()
	{
		pNode->~_Node();
		pNode->m_pNext = m_pFree;
		m_pFree = pNode;
		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 ) {
			RemoveAll();
		}
	}

	//iterator
	Iterator get_iterator(_Node* pNode) const throw()
	{
		return Iterator(RefPtr<thisClass>(this), RefPtr<_Node>(pNode));
	}

private:
	_Node*   m_pHead;  //head node
	uintptr  m_uElements;
	//pool
	FixedElementMemoryPool  m_pool;
	_Node*  m_pFree;  //free list
	uintptr m_uMinBlockElements, m_uMaxBlockElements;

private:
	//non-copyable
	SingleList(const SingleList& src) throw();
	SingleList& operator=(const SingleList& src) throw();
};

// List<T>

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
		Iterator(const RefPtr<thisClass>& list, const RefPtr<_Node>& node) throw() : m_refList(list), m_refNode(node)
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
			m_refNode = ( m_refNode.IsNull() ) ? m_refList.GetTail().m_refNode
												: m_refNode.Deref().m_pPrev;
		}

	private:
		RefPtr<thisClass>  m_refList;
		RefPtr<_Node>      m_refNode;

		friend class thisClass;
	};

public:
	List(const RefPtr<IMemoryManager>& mgr, uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
		: m_pHead(NULL), m_pTail(NULL), m_uElements(0), m_pool(mgr), m_pFree(NULL),
		m_uMinBlockElements(uMinElements), m_uMaxBlockElements(uMaxElements)
	{
		assert( uMinElements > 0 && uMaxElements > 0 && uMinElements <= uMaxElements );
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
	Iterator GetHead() const throw()
	{
		return get_iterator(m_pHead);
	}
	Iterator GetTail() const throw()
	{
		return get_iterator(m_pTail);
	}
	Iterator GetBegin() const throw()
	{
		return GetHead();
	}
	Iterator GetEnd() const throw()
	{
		return get_iterator(NULL);
	}
	ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	ReverseIterator<Iterator> GetReverseEnd() const throw()
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
		m_pFree = NULL;
		m_pool.FreeDataChain();
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
	void get_free_node()  //may throw
	{
		if( m_pFree == NULL ) {
			uintptr uActElements;
			//may throw
			_Node* pNode = (_Node*)m_pool.CreateBlock(m_uMinBlockElements, m_uMaxBlockElements, sizeof(_Node), uActElements);
			pNode += (uActElements - 1);
			for( uintptr uBlock = uActElements; uBlock > 0; uBlock -- ) {
				pNode->m_pNext = m_pFree;
				m_pFree = pNode;
				pNode --;
			}
		}
		assert( m_pFree != NULL );
	}
	_Node* new_node(_Node* pPrev, _Node* pNext)  //may throw
	{
		get_free_node();

		_Node* pNewNode = m_pFree;
		_Node* pNextFree = m_pFree->m_pNext;

		call_constructor(*pNewNode);  //may throw

		m_pFree = pNextFree;
		pNewNode->m_pPrev = pPrev;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );

		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pPrev, _Node* pNext)  //may throw
	{
		get_free_node();

		_Node* pNewNode = m_pFree;
		_Node* pNextFree = m_pFree->m_pNext;

		call_constructor(*pNewNode, t);  //may throw

		m_pFree = pNextFree;
		pNewNode->m_pPrev = pPrev;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );

		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pPrev, _Node* pNext)  //may throw
	{
		get_free_node();

		_Node* pNewNode = m_pFree;
		_Node* pNextFree = m_pFree->m_pNext;

		call_constructor(*pNewNode, rv_forward(t));  //may throw

		m_pFree = pNextFree;
		pNewNode->m_pPrev = pPrev;
		pNewNode->m_pNext = pNext;
		m_uElements ++;
		assert( m_uElements > 0 );

		return pNewNode;
	}

	void free_node(_Node* pNode) throw()
	{
		pNode->~_Node();
		pNode->m_pNext = m_pFree;
		m_pFree = pNode;
		assert( m_uElements > 0 );
		m_uElements --;
		if( m_uElements == 0 ) {
			RemoveAll();
		}
	}

	//iterator
	Iterator get_iterator(_Node* pNode) const throw()
	{
		return Iterator(RefPtr<thisClass>(this), RefPtr<_Node>(pNode));
	}

private:
	_Node*   m_pHead;  //head node
	_Node*   m_pTail;  //tail node
	uintptr  m_uElements;
	//pool
	FixedElementMemoryPool  m_pool;
	_Node*  m_pFree;  //free list
	uintptr m_uMinBlockElements, m_uMaxBlockElements;

private:
	//non-copyable
	List(const List& src) throw();
	List& operator=(const List& src) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COLL_H__
////////////////////////////////////////////////////////////////////////////////
