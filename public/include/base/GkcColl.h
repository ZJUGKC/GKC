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

template <typename T>
class SingleList
{
private:
	typedef SingleList<T>  thisClass;

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
	Iterator GetTail() const throw()
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
			return GetTail();
		if( m_pHead == NULL )
			return GetTail();
		_Node* pNode = m_pHead;
		for( uintptr i = 0; i < uIndex; i ++ ) {
			pNode = pNode->m_pNext;
		} //end for
		return get_iterator(pNode);
	}
	Iterator Find(const T& t, const Iterator& iterAfter) const throw()
	{
		_Node* pNode = &(iterAfter.m_refNode.Deref());
		if( pNode == NULL ) {
			pNode = m_pHead;  //start at head
		}
		else {
			pNode = pNode->m_pNext;  // start after the one specified
		}
		for( ; pNode != NULL; pNode = pNode->m_pNext ) {
			if( LogicalOperators::IsEqual(pNode->m_t, t) )
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

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COLL_H__
////////////////////////////////////////////////////////////////////////////////
