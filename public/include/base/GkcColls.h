/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
#ifndef __GKC_COLLS_H__
#define __GKC_COLLS_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_DEF_H__
	#error GkcColls.h requires GkcDef.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

//------------------------------------------------------------------------------
// MultiwayTree<T>

template <typename T>
class MultiwayTree
{
public:
	typedef T  EType;

private:
	typedef MultiwayTree<T>  thisClass;

	//internal node
	struct _Node
	{
		_Node()
		{
		}
		explicit _Node(const T& t) : m_t(t)
		{
		}
		explicit _Node(T&& t) : m_t(rv_forward(t))
		{
		}
		~_Node() throw()
		{
		}

		_Node* m_pNext;
		_Node* m_pParent;
		_Node* m_pChild;
		T m_t;

	private:
		_Node(const _Node&) throw();
	};

public:
	//position
	class Position
	{
	public:
		Position() throw()
		{
		}
		Position(const Position& src) throw() : m_refNode(src.m_refNode)
		{
		}
		~Position() throw()
		{
		}
		Position& operator=(const Position& src) throw()
		{
			m_refNode = src.m_refNode;
			return *this;
		}
		bool IsNull() const throw()
		{
			return m_refNode.IsNull();
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
		void set_Value(const T& t)
		{
			m_refNode.Deref().m_t = t;  //may throw
		}
		void set_Value(T&& t)
		{
			m_refNode.Deref().m_t = rv_forward(t);  //may throw
		}
		//compare
		bool operator==(const Position& right) const throw()
		{
			return m_refNode == right.m_refNode;
		}
		bool operator!=(const Position& right) const throw()
		{
			return m_refNode != right.m_refNode;
		}
		//next
		void MoveNext() throw()
		{
			m_refNode = m_refNode.Deref().m_pNext;
		}
		//parent
		void MoveParent() throw()
		{
			m_refNode = m_refNode.Deref().m_pParent;
		}
		//child
		void MoveChild() throw()
		{
			m_refNode = m_refNode.Deref().m_pChild;
		}

	private:
		RefPtr<_Node>  m_refNode;

	private:
		friend thisClass;
	};

	//iterator
	class Iterator
	{
	public:
		Iterator() throw()
		{
		}
		Iterator(const Iterator& src) throw() : m_pos(src.m_pos)
		{
		}
		~Iterator() throw()
		{
		}
		Iterator& operator=(const Iterator& src) throw()
		{
			m_pos = src.m_pos;
			return *this;
		}
		bool IsNull() const throw()
		{
			return m_pos.IsNull();
		}
		const Position GetPosition() const throw()
		{
			return m_pos;
		}
		Position GetPosition() throw()
		{
			return m_pos;
		}
		//properties
		const RefPtr<T> get_Ref() const throw()
		{
			return m_pos.get_Ref();
		}
		RefPtr<T> get_Ref() throw()
		{
			return m_pos.get_Ref();
		}
		const T& get_Value() const throw()
		{
			return m_pos.get_Value();
		}
		T& get_Value() throw()
		{
			return m_pos.get_Value();
		}
		void set_Value(const T& t)
		{
			m_pos.set_Value(t);  //may throw
		}
		void set_Value(T&& t)
		{
			m_pos.set_Value(rv_forward(t));  //may throw
		}
		//compare
		bool operator==(const Iterator& right) const throw()
		{
			return m_pos == right.m_pos;
		}
		bool operator!=(const Iterator& right) const throw()
		{
			return m_pos != right.m_pos;
		}
		//next
		void MoveNext() throw()
		{
			m_pos.MoveNext();
		}
		//parent
		void MoveParent() throw()
		{
			m_pos.MoveParent();
		}
		//child
		void MoveChild() throw()
		{
			m_pos.MoveChild();
		}

	private:
		Position  m_pos;

	private:
		friend thisClass;
	};

public:
	explicit MultiwayTree(const RefPtr<IMemoryManager>& mgr = RefPtr<IMemoryManager>(), uintptr uMinElements = 10, uintptr uMaxElements = 10) throw()
				: m_pRoot(NULL), m_uElements(1),
				m_freelist(RefPtrHelper::GetInternalPointer(mgr), uMinElements, uMaxElements)
	{
	}
	~MultiwayTree() throw()
	{
		RemoveAll();
		if( m_pRoot != NULL )
			free_node(m_pRoot);
	}

	void SetMemoryManager(const RefPtr<IMemoryManager>& mgr) throw()
	{
		m_freelist.SetMemoryManager(RefPtrHelper::GetInternalPointer(mgr));
	}

	uintptr GetCount() const throw()
	{
		return m_uElements - 1;
	}
	bool IsEmpty() const throw()
	{
		return m_uElements <= 1;
	}

	//position
	const Position GetRootPosition() const throw()
	{
		return get_position(m_pRoot);
	}
	Position GetRootPosition() throw()
	{
		return get_position(m_pRoot);
	}
	const Iterator GetAtPosition(const Position& pos) const throw()
	{
		return get_iterator(pos);
	}
	Iterator GetAtPosition(const Position& pos) throw()
	{
		return get_iterator(pos);
	}

	//iterator
	const Iterator GetRoot() const throw()
	{
		return get_iterator(m_pRoot);
	}
	Iterator GetRoot() throw()
	{
		return get_iterator(m_pRoot);
	}

	//methods
	void RemoveAll() throw()
	{
		//root
		if( m_pRoot == NULL )
			return ;
		//children
		remove_node_children(m_pRoot);
		assert( m_uElements == 1 );
	}

	//insert
	Iterator Insert(const Iterator& iterParent, const Iterator& iterAfter)
	{
		_Node* pParent = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterParent.m_pos.m_refNode));
		_Node* pAfter = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterAfter.m_pos.m_refNode));
		_Node* pNewNode = new_node(pParent, pAfter);  //may throw
		return get_iterator(pNewNode);
	}
	Iterator Insert(const Iterator& iterParent, const Iterator& iterAfter, const T& t)
	{
		_Node* pParent = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterParent.m_pos.m_refNode));
		_Node* pAfter = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterAfter.m_pos.m_refNode));
		_Node* pNewNode = new_node(t, pParent, pAfter);  //may throw
		return get_iterator(pNewNode);
	}
	Iterator Insert(const Iterator& iterParent, const Iterator& iterAfter, T&& t)
	{
		_Node* pParent = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterParent.m_pos.m_refNode));
		_Node* pAfter = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterAfter.m_pos.m_refNode));
		_Node* pNewNode = new_node(rv_forward(t), pParent, pAfter);  //may throw
		return get_iterator(pNewNode);
	}

	//remove
	void RemoveAt(const Iterator& iter) throw()
	{
		assert( !iter.IsNull() );
		assert( iter != GetRoot() );

		_Node* pOldNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_pos.m_refNode));
		//break
		break_link(pOldNode);
		//children
		remove_node_children(pOldNode);
		//self
		free_node(pOldNode);
	}

	//reverse
	Iterator ReverseLevelLink(const Iterator& iterHead) throw()
	{
		assert( !iterHead.IsNull() );
		assert( iterHead != GetRoot() );

		_Node* pHead = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterHead.m_pos.m_refNode));
		assert( pHead->m_pParent->m_pChild == pHead );

		//reverse
		_Node* pNode = pHead;
		pHead = NULL;
		while( pNode != NULL ) {
			_Node* pNext = pNode->m_pNext;
			pNode->m_pNext = pHead;
			pHead = pNode;
			pNode = pNext;
		}
		//child
		pHead->m_pParent->m_pChild = pHead;

		return get_iterator(pHead);
	}

	//change
	void Move(const Iterator& iter, const Iterator& iterParent, const Iterator& iterAfter) throw()
	{
		assert( !iter.IsNull() );
		assert( iter != GetRoot() );

		_Node* pNode = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iter.m_pos.m_refNode));
		_Node* pParent = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterParent.m_pos.m_refNode));
		_Node* pAfter = const_cast<_Node*>(RefPtrHelper::GetInternalPointer(iterAfter.m_pos.m_refNode));

		//break
		break_link(pNode);
		//new link
		connect_link(pNode, pParent, pAfter);
	}

private:
	//tools
	static void break_link(_Node* pNode) throw()
	{
		_Node* pPrev = pNode->m_pParent->m_pChild;
		assert( pPrev != NULL );

		//self
		if( pPrev == pNode ) {
			pNode->m_pParent->m_pChild = pNode->m_pNext;
			return ;
		}

		//previous
		_Node* pNext = pPrev->m_pNext;
		assert( pNext != NULL );
		while( pNext != pNode ) {
			pPrev = pNext;
			pNext = pNext->m_pNext;
			assert( pNext != NULL );
		}
		pPrev->m_pNext = pNode->m_pNext;
	}
	void connect_link(_Node* pNode, _Node* pParent, _Node* pAfter) throw()
	{
		if( pParent == NULL )
			pParent = m_pRoot;
		pNode->m_pParent = pParent;
		if( pAfter == NULL ) {
			pNode->m_pNext = pParent->m_pChild;
			pParent->m_pChild = pNode;
			return ;
		}
		assert( pAfter->m_pParent == pParent );
		pNode->m_pNext = pAfter->m_pNext;
		pAfter->m_pNext = pNode;
	}

	void fill_new_node(_Node* pNewNode, _Node* pParent, _Node* pAfter) throw()
	{
		pNewNode->m_pChild = NULL;
		connect_link(pNewNode, pParent, pAfter);
		m_uElements ++;
		assert( m_uElements > 0 );
	}
	void root_allocate()
	{
		if( m_pRoot == NULL ) {
			m_pRoot = node_pair_helper<_Node, T>::ConstructNode(m_freelist);  //may throw
			m_pRoot->m_pNext = m_pRoot->m_pParent = m_pRoot->m_pChild = NULL;
		}
	}

	//nodes
	_Node* new_node(_Node* pParent, _Node* pAfter)
	{
		root_allocate();  //may throw
		_Node* pNewNode = node_pair_helper<_Node, T>::ConstructNode(m_freelist);  //may throw
		fill_new_node(pNewNode, pParent, pAfter);
		return pNewNode;
	}
	_Node* new_node(const T& t, _Node* pParent, _Node* pAfter)
	{
		root_allocate();  //may throw
		_Node* pNewNode = node_pair_helper<_Node, T>::ConstructNode(m_freelist, t);  //may throw
		fill_new_node(pNewNode, pParent, pAfter);
		return pNewNode;
	}
	_Node* new_node(T&& t, _Node* pParent, _Node* pAfter)
	{
		root_allocate();  //may throw
		_Node* pNewNode = node_pair_helper<_Node, T>::ConstructNode(m_freelist, rv_forward(t));  //may throw
		fill_new_node(pNewNode, pParent, pAfter);
		return pNewNode;
	}
	//free
	void free_node(_Node* pNode) throw()
	{
		node_pair_helper<_Node, T>::DestructNode(m_freelist, pNode, m_uElements);
	}

	//remove
	void remove_node_children(_Node* pNode) throw()
	{
		_Node* pKill = pNode->m_pChild;
		if( pKill == NULL )
			return ;
		while( pKill != pNode ) {
			if( pKill->m_pChild != NULL ) {
				pKill = pKill->m_pChild;
				continue;
			}
			while( pKill != pNode ) {
				_Node* pParent = pKill->m_pParent;
				_Node* pNext = pKill->m_pNext;
				free_node(pKill);
				if( pNext != NULL ) {
					pKill = pNext;
					break;
				}
				pKill = pParent;
			}
		} //end while
	}

	//position
	static Position get_position(_Node* pNode) throw()
	{
		Position pos;
		pos.m_refNode = pNode;
		return pos;
	}
	//iterator
	static Iterator get_iterator(const Position& pos) throw()
	{
		Iterator iter;
		iter.m_pos = pos;
		return iter;
	}
	static Iterator get_iterator(_Node* pNode) throw()
	{
		return get_iterator(get_position(pNode));
	}

private:
	_Node*   m_pRoot;
	uintptr  m_uElements;
	//free list
	free_list<_Node>  m_freelist;

private:
	//non-copyable
	MultiwayTree(const MultiwayTree&) throw();
	MultiwayTree& operator=(const MultiwayTree&) throw();
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_COLLS_H__
////////////////////////////////////////////////////////////////////////////////
