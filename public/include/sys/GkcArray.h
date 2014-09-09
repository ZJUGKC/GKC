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
This file contains array classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_ARRAY_H__
#define __GKC_ARRAY_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error GkcArray.h requires GkcBase.h to be included first.
#endif

#ifndef __GKC_SYS_H__
	#error GkcArray.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// FixedArray<T, t_size>

template <typename T, uintptr t_size>
class FixedArray
{
public:
	static const uintptr c_size = t_size;  //!< The number of elements.

	typedef T EType;   //element type

private:
	typedef FixedArray<T, t_size>  thisClass;

public:
	//iterator
	typedef ArrayIterator<T>  Iterator;

public:
	FixedArray() //may throw
	{
		assert( t_size > 0 );
	}
	FixedArray(const thisClass& src)  //may throw
	{
		assert( t_size > 0 );
		copy_elements(src.m_data, m_data, t_size);
	}
	~FixedArray() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src)  //may throw
	{
		assert( t_size > 0 );
		if( this != &src ) {
			copy_elements(src.m_data, m_data, t_size);
		}
		return *this;
	}

	Iterator operator[](uintptr index) const throw()
	{
		return GetAt(index);
	}

	//iterator
	Iterator GetBegin() const throw()
	{
		return Iterator(RefPtr<T>(m_data));
	}
	Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<T>(m_data + t_size));
	}
	ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	ReverseIterator<Iterator> GetReverseEnd() const throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}

	Iterator GetAt(uintptr index) const throw()
	{
		assert( index < t_size );
		return Iterator(RefPtr<T>(m_data + index));
	}
	void SetAt(uintptr index, const T& t)  //may throw
	{
		assert( index < t_size );
		m_data[index] = t;
	}
	void SetAt(uintptr index, T&& t)  //may throw
	{
		assert( index < t_size );
		m_data[index] = rv_forward(t);
	}

	//logical
	bool operator==(const thisClass& right) const throw()
	{
		for( uintptr i = 0; i < t_size; i ++ ) {
			if( LogicalOperators::IsNE(m_data[i], right.m_data[i]) )
				return false;
		}
		return true;
	}
	bool operator!=(const thisClass& right) const throw()
	{
		for( uintptr i = 0; i < t_size; i ++ ) {
			if( LogicalOperators::IsNE(m_data[i], right.m_data[i]) )
				return true;
		}
		return false;
	}

private:
	static void copy_elements(const T* pSrc, T* pDest, uintptr size)
	{
		for( uintptr i = 0; i < size; i ++ ) {
			pDest[i] = pSrc[i];  //may throw
		}
	}

private:
	T m_data[t_size];  //array
};

// FixedArrayEndian<T, t_size, t_bBigEndian>

template <typename T, uintptr t_size, bool t_bBigEndian>
class FixedArrayEndian : public FixedArray<T, t_size>
{
private:
	typedef FixedArrayEndian<T, t_size, t_bBigEndian>  thisClass;
	typedef FixedArray<T, t_size>  baseClass;

public:
	FixedArrayEndian()
	{
	}
	FixedArrayEndian(const thisClass& src) : baseClass(static_cast<const baseClass&>(src))
	{
	}
	~FixedArrayEndian() throw()
	{
	}

	thisClass& operator=(const thisClass& src)
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
};

//logical special
template <typename T, uintptr t_size, bool t_bBigEndian>
inline bool LogicalOperators::IsLT<FixedArrayEndian<T, t_size, t_bBigEndian>>(const FixedArrayEndian<T, t_size, t_bBigEndian>& t1, const FixedArrayEndian<T, t_size, t_bBigEndian>& t2) throw()
{
	if( t_bBigEndian ) {
		for( uintptr i = 0; i < t_size; i ++ ) {
			if( LogicalOperators::IsGT<T>(t1[i].get_Value(), t2[i].get_Value()) )
				return false;
			if( LogicalOperators::IsLT<T>(t1[i].get_Value(), t2[i].get_Value()) )
				return true;
		}
	}
	else {
		for( uintptr i = t_size; i > 0; i -- ) {
			if( LogicalOperators::IsGT<T>(t1[i - 1].get_Value(), t2[i - 1].get_Value()) )
				return false;
			if( LogicalOperators::IsLT<T>(t1[i - 1].get_Value(), t2[i - 1].get_Value()) )
				return true;
		}
	} //end if
	return false;
}

// SharedArray<T>

template <typename T>
class SharedArray
{
private:
	typedef SharedArray<T>  thisClass;

public:
	typedef T  EType;
	typedef ArrayIterator<T>  Iterator;

public:
	SharedArray() throw() : m_pB(NULL), m_pT(NULL)
	{
	}
	SharedArray(const SharedArray<T>& src) throw() : m_pB(src.m_pB), m_pT(src.m_pT)
	{
		//add ref
		if( m_pB != NULL ) {
			SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
			assert( pB->m_shareCount > 0 );  //must have shared array
			pB->AddRefCopy();
		}
	}
	SharedArray(SharedArray<T>&& src) throw()
	{
		m_pB = src.m_pB;
		m_pT = src.m_pT;
		src.m_pB = NULL;
		src.m_pT = NULL;
	}
	~SharedArray() throw()
	{
		Release();
	}

	//methods
	void Release() throw()
	{
		if( m_pB != NULL ) {
			SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
			assert( pB->m_shareCount > 0 );  //must have shared array
			if( pB->Release() <= 0 ) {
				//free
				if( m_pT != NULL ) {
					pB->DestroyArray(m_pT);
					m_pT = NULL;
				}
			}
			assert( pB->m_weakCount > 0 );  //must have weak object
			if( pB->WeakRelease() <= 0 ) {
				//free block
				pB->~SharedArrayBlock();
				SharedArrayBlockHelper::Free(pB);
				m_pB = NULL;
			}
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

	//operators
	SharedArray<T>& operator=(const SharedArray<T>& src) throw()
	{
		if( &src != this ) {
			if( src.m_pT != m_pT ) {
				//release
				Release();
				//assign
				m_pB = src.m_pB;
				m_pT = src.m_pT;
				//add ref
				if( m_pB != NULL ) {
					SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
					assert( pB->m_shareCount > 0 );  //must have shared array
					pB->AddRefCopy();
				}
			}
		}
		return *this;
	}
	SharedArray<T>& operator=(SharedArray<T>&& src) throw()
	{
		if( &src != this ) {
			if( src.m_pT != m_pT ) {
				//release
				Release();
				//assign
				m_pB = src.m_pB;
				m_pT = src.m_pT;
				src.m_pB = NULL;
				src.m_pT = NULL;
			}
		}
		return *this;
	}

	Iterator operator[](uintptr index) const throw()
	{
		return GetAt(index);
	}

	bool operator==(const SharedArray<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const SharedArray<T>& right) const throw()
	{
		return !operator==(right);
	}
	bool IsNull() const throw()
	{
		return m_pT == NULL;
	}

	uintptr GetCount() const throw()
	{
		return (m_pB == NULL) ? 0 : ((SharedArrayBlock*)m_pB)->GetLength();
	}
	bool IsEmpty() const throw()
	{
		return GetCount() == 0;
	}

	//iterator
	Itertaor GetBegin() const throw()
	{
		return Iterator(RefPtr<T>(m_pT));
	}
	Iterator GetEnd() const throw()
	{
		return Iterator(RefPtr<T>(m_pT + GetCount()));
	}
	Iterator GetReverseBegin() const throw()
	{
		return ReverseIterator<Iterator>(GetEnd());
	}
	Iterator GetReverseEnd() const throw()
	{
		return ReverseIterator<Iterator>(GetBegin());
	}
	Iterator GetAt(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return Iterator(RefPtr<T>(m_pT + index));
	}
	void SetAt(uintptr index, const T& t)  //may throw
	{
		assert( index < GetCount() );
		m_pT[index] = t;
	}
	void SetAt(uintptr index, T&& t)  //may throw
	{
		assert( index < GetCount() );
		m_pT[index] = rv_forward(t);
	}

	//size
	template <typename... Args>
	void SetCount(uintptr uCount, uintptr uGrowBy = 0, Args&&... args)  //may throw
	{
		assert( m_pB != NULL );  //must have a block for allocation
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		assert( !pB->GetMemoryManager().IsNull() );
		uintptr uOldSize = pB->GetLength();
		if( uCount == 0 ) {
			// shrink to nothing
			clear_array();
		}
		else if( uCount <= pB->GetAllocLength() ) {
			// it fits
			if( uCount > uOldSize ) {
				// initialize the new elements
				call_array_constructors(m_pT + uOldSize, uCount - uOldSize, rv_forward<Args>(args)...);
			}
			else if( uOldSize > uCount ) {
				// destroy the old elements
				call_array_destructors(m_pT + uCount, uOldSize - uCount);
			}
			pB->SetLength(uCount);
		}
		else {
			// grow
			grow_buffer(uCount, uGrowBy);  //may throw
			// construct new elements
			assert( uCount > pB->GetLength() );
			call_array_constructors(m_pT + uOldSize, uCount - uOldSize, rv_forward<Args>(args)...);
			pB->SetLength(uCount);
		} //end if
	}
	//clear
	void RemoveAll() throw()
	{
		assert( m_pB != NULL );  //must have a block for free
		clear_array();
	}
	void FreeExtra() throw()
	{
		assert( m_pB != NULL );
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		assert( !pB->GetMemoryManager().IsNull() );
		uintptr uSize = pB->GetLength();
		if( uSize == pB->GetAllocLength() )
			return ;
		// shrink to desired size
		if( uSize == 0 ) {
			//free
			pB->GetMemoryManager().Deref().Free(m_pT);
			m_pT = NULL;
			pB->SetAllocLength(0);
			return ;
		}
		uintptr uBytes;
		SafeOperators::Multiply(uSize, sizeof(T), uBytes);  //no check
		T* pNew = (T*)pB->GetMemoryManager().Deref().Reallocate(m_pT, uBytes);
		if( pNew == NULL )
			return ;
		m_pT = pNew;
		pB->SetAllocLength(uSize);
	}

	//add
	template <typename... Args>
	Iterator Add(Args&&... args)  //may throw
	{
		assert( m_pB != NULL );
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		uintptr uElement = pB->GetLength();
		SetCount(uElement + 1, 0, rv_forward<Args>(args)...);
		return GetAt(uElement);
	}
	void Append(const SharedArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		assert( m_pB != NULL );
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		uintptr uOldSize = pB->GetLength();
		SetCount(uOldSize + src.GetCount());
		copy_elements(src.m_pT, m_pT + uOldSize, src.GetCount());
	}
	void Copy(const SharedArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		uintptr uSize = src.GetCount();
		SetCount(uSize);
		copy_elements(src.m_pT, m_pT, uSize);
	}

	template <typename... Args>
	void InsertAt(uintptr index, uintptr count = 1, Args&&... args)  //may throw
	{
		assert( count > 0 );  // zero size not allowed
		assert( m_pB != NULL );
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		uintptr uSize = pB->GetLength();
		if( index >= uSize ) {
			// adding after the end of the array
			SetCount(index + count, 0, rv_forward<Args>(args)...);  //grow so index is valid
		}
		else {
			// inserting in the middle of the array
			uintptr uOldSize = uSize;
			SetCount(uSize + count, 0);  //grow it to new size
			// destroy intial data before copying over it
			call_array_destructors(m_pT + uOldSize, count);
			// shift old data up to fill gap
			relocate_elements(m_pT + index, m_pT + (index + count), uOldSize - index);
			try {
				// re-init slots we copied from
				call_array_constructors(m_pT + index, count, rv_forward<Args>(args)...);
			}
			catch(...) {
				relocate_elements(m_pT + (index + count), m_pT + index, uOldSize - index);
				pB->SetLength(uOldSize);
				throw ;  //re-throw
			}
		} //end if
		assert( (index + count) <= pB->GetLength() );
	}
	void InsertArrayAt(uintptr index, const SharedArray<T>& src)  //may throw
	{
		uintptr uSize = src.GetCount();
		if( uSize > 0 ) {
			InsertAt(index, uSize);
			for( uintptr i = 0; i < uSize; i ++ ) {
				SetAt(index + i, src[i].get_Value());
			}
		} //end if
	}

	//remove
	void RemoveAt(uintptr index, uintptr count = 1) throw()
	{
		assert( count > 0 );  // zero size not allowed
		assert( m_pB != NULL );
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		uintptr uSize = pB->GetLength();
		uintptr uCut = index + count;
		assert( uCut >= index && uCut >= count && uCut <= uSize );  //no overflow
		// just remove a range
		uintptr uMoveCount = uSize - uCut;
		//destructor
		call_array_destructors(m_pT + index, count);
		if( uMoveCount > 0 ) {
			relocate_elements(m_pT + uCut, m_pT + index, uMoveCount);
		}
		//size
		pB->SetLength(uSize - count);
	}

private:
	//clear
	void clear_array() throw()
	{
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		if( m_pT != NULL ) {
			pB->DestroyArray(m_pT);
			m_pT = NULL;
		}
		else {
			pB->SetLength(0);
			pB->SetAllocLength(0);
		} //end if
	}

	//grow
	void grow_buffer(uintptr uNewSize, uintptr uGrowBy)  //may throw
	{
		SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
		if( uNewSize <= pB->GetAllocLength() )
			return ;
		uintptr uAllocSize;
		if( m_pT == NULL ) {
			uAllocSize = (uGrowBy > uNewSize) ? uGrowBy : uNewSize;
			uintptr uBytes = 0;
			//overflow
			if( SafeOperators::Multiply(uAllocSize, sizeof(T), uBytes).IsFailed() )
				throw OverflowException();
			//allocate
			m_pT = static_cast<T*>(pB->GetMemoryManager().Deref().Allocate(uBytes));
			if( m_pT == NULL )
				throw OutOfMemoryException();
		}
		else {
			//grow
			if( uGrowBy == 0 ) {
				//default situation
				uGrowBy = pB->GetLength() / 8;
				uGrowBy = (uGrowBy < 4) ? 4 : ((uGrowBy > 1024) ? 1024 : uGrowBy);
			}
			//overflow
			if( SafeOperators::Add(pB->GetAllocLength(), uGrowBy, uAllocSize).IsFailed() )
				throw OverflowException();
			if( uNewSize > uAllocSize )
				uAllocSize = uNewSize;  //no extra
			uintptr uBytes = 0;
			//overflow
			if( SafeOperators::Multiply(uAllocSize, sizeof(T), uBytes).IsFailed() )
				throw OverflowException();
			//reallocate
			//  because uBytes != 0, m_pT is not freed
			T* pNew = static_cast<T*>(pB->GetMemoryManager().Deref().Reallocate((uintptr)m_pT, uBytes);
			if( pNew == NULL )
				throw OutOfMemoryException();
			m_pT = pNew;
		} //end if
		pB->SetAllocLength(uAllocSize);
	}

	template <typename... Args>
	static void call_array_constructors(T* p, uintptr size, Args&&... args)
	{
		uintptr i;
		try {
			for( i = 0; i < size; i ++ ) {
				call_constructor(p + i, rv_forward<Args>(args)...);
			}
		}
		catch(...) {
			while( i > 0 ) {
				i --;
				p[i].~T();
			}
			throw;  //re-throw
		}
	}
	static void call_array_destructors(T* p, uintptr size) throw()
	{
		for( uintptr i = 0; i < size; i ++ ) {
			p[i].~T();
		}
	}
	//copy
	static void copy_elements(const T* pSrc, T* pDest, uintptr size)
	{
		for( uintptr i = 0; i < size; i ++ ) {
			pDest[i] = pSrc[i];  //may throw
		}
	}
	//relocate
	static void relocate_elements(const T* pSrc, T* pDest, uintptr size) throw()
	{
		mem_move(pSrc, size * sizeof(T), pDest);
	}

private:
	void* m_pB;  //to array block
	T*    m_pT;  //first address of array

private:
	friend class SharedArrayHelper;
};

// WeakArray<T>

template <typename T>
class WeakArray
{
public:
	typedef T  EType;

public:
	WeakArray() throw() : m_pB(NULL), m_pT(NULL)
	{
	}
	WeakArray(const WeakArray<T>& src) throw() : m_pB(src.m_pB), m_pT(src.m_pT)
	{
		//add ref
		if( m_pB != NULL ) {
			SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
			assert( pB->m_weakCount > 0 );  //must have weak object
			pB->WeakAddRef();
		}
	}
	WeakArray(WeakArray<T>&& src) throw()
	{
		m_pB = src.m_pB;
		m_pT = src.m_pT;
		src.m_pB = NULL;
		src.m_pT = NULL;
	}
	~WeakArray() throw()
	{
		Release();
	}

	//methods
	void Release() throw()
	{
		if( m_pB != NULL ) {
			SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
			assert( pB->m_weakCount > 0 );  //must have weak object
			if( pB->WeakRelease() <= 0 ) {
				//free block
				pB->~SharedArrayBlock();
				SharedArrayBlockHelper::Free(pB);
				m_pB = NULL;
			}
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

	//operators
	WeakArray<T>& operator=(const WeakArray<T>& src) throw()
	{
		if( &src != this ) {
			if( src.m_pT != m_pT ) {
				//release
				Release();
				//assign
				m_pB = src.m_pB;
				m_pT = src.m_pT;
				if( m_pB != NULL ) {
					//add ref
					SharedArrayBlock* pB = (SharedArrayBlock*)m_pB;
					assert( pB->m_weakCount > 0 );  //must have weak object
					pB->WeakAddRef();
				}
			}
		}
		return *this;
	}
	WeakArray<T>& operator=(WeakArray<T>&& src) throw()
	{
		if( &src != this ) {
			if( src.m_pT != m_pT ) {
				//release
				Release();
				//assign
				m_pB = src.m_pB;
				m_pT = src.m_pT;
				src.m_pB = NULL;
				src.m_pT = NULL;
			}
		}
		return *this;
	}

	bool operator==(const WeakArray<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const WeakArray<T>& right) const throw()
	{
		return m_pT != right.m_pT;
	}
	bool IsNull() const throw()
	{
		return m_pT == NULL;
	}

private:
	void*  m_pB;   //A pointer to SharedArrayBlock
	T*     m_pT;   //A pointer to array

private:
	friend class SharedArrayHelper;
};

// SharedArrayHelper

class SharedArrayHelper
{
public:
	//make shared
	template <typename T>
	static SharedArray<T> MakeSharedArray(const RefPtr<IMemoryManager>& mgr)
	{
		assert( !mgr.IsNull() );

		//allocate
		SharedArrayBlock* pB = SharedArrayBlockHelper::Allocate();
		if( pB == NULL )
			throw( OutOfMemoryException() );

		SharedArray<T> ret;
		call_constructor(*pB);  //no throw
		pB->SetMemoryManager(mgr);
		ret.m_pB = pB;

		return ret;
	}

	//obtain weak array
	template <typename T>
	static WeakArray<T> ToWeakArray(const SharedArray<T>& sp) throw()
	{
		WeakArray<T> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedArrayBlock* pB = (SharedArrayBlock*)ret.m_pB;
			assert( pB->m_weakCount > 0 );  //must have weak object
			pB->WeakAddRef();
		}
		return ret;
	}
	//To SharedArray
	template <typename T>
	static SharedArray<T> ToSharedArray(const WeakArray<T>& sp) throw()
	{
		SharedArray<T> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedArrayBlock* pB = (SharedArrayBlock*)ret.m_pB;
			if( !pB->AddRefLock() ) {
				ret.m_pT = NULL;  //shared array freed
				ret.m_pB = NULL;
			}
		}
		return ret;
	}

	//clone
	template <typename T>
	static SharedArray<T> Clone(const SharedArray<T>& sp)  //may throw
	{
		SharedArray<T> ret;
		if( !sp.IsNull() ) {
			ret = MakeSharedArray(sp.m_pB->GetMemoryManager());
			ret.Copy(sp);
		}
		return ret;
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_ARRAY_H__
////////////////////////////////////////////////////////////////////////////////
