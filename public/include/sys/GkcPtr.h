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
This file contains smart pointer classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_PTR_H__
#define __GKC_PTR_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_BASE_H__
	#error GkcPtr.h requires GkcBase.h to be included first.
#endif

#ifndef __GKC_SYS_H__
	#error GkcPtr.h requires GkcSys.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

//------------------------------------------------------------------------------
// Pointers

// SharedPtr<T>

template <class T>
class SharedPtr
{
public:
	SharedPtr() throw() : m_pB(NULL), m_pT(NULL)
	{
	}
	SharedPtr(const SharedPtr<T>& src) throw() : m_pB(src.m_pB), m_pT(src.m_pT)
	{
		//add ref
		if( m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
			assert( pB->m_shareCount > 0 );  //must have shared object
			pB->AddRefCopy();
		}
	}
	SharedPtr(SharedPtr<T>&& src) throw()
	{
		m_pB = src.m_pB;
		m_pT = src.m_pT;
		src.m_pB = NULL;
		src.m_pT = NULL;
	}
	~SharedPtr() throw()
	{
		Release();
	}

	//methods
	void Release() throw()
	{
		if( m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
			assert( pB->m_shareCount > 0 );  //must have shared object
			if( pB->Release() <= 0 ) {
				//free
				pB->DestroyObject();
				m_pT = NULL;
			}
			assert( pB->m_weakCount > 0 );  //must have weak object
			if( pB->WeakRelease() <= 0 ) {
				//free block
				pB->~SharedPtrBlock();
				SharedPtrBlockHelper::Free(pB);
				m_pB = NULL;
			}
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

	//operators
	SharedPtr<T>& operator=(const SharedPtr<T>& src) throw()
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
					SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
					assert( pB->m_shareCount > 0 );  //must have shared object
					pB->AddRefCopy();
				}
			}
		}
		return *this;
	}
	SharedPtr<T>& operator=(SharedPtr<T>&& src) throw()
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

	bool operator==(const SharedPtr<T>& src) const throw()
	{
		return m_pT == src.m_pT;
	}
	bool operator!=(const SharedPtr<T>& src) const throw()
	{
		return !operator==(src);
	}
	bool IsNull() const throw()
	{
		return m_pT == NULL;
	}

	T& Deref() throw()
	{
		assert( !IsNull() );
		return *m_pT;
	}

private:
	void*  m_pB;   //A pointer to SharedPtrBlock
	T*     m_pT;   //A pointer to object

private:
	friend class SharedPtrHelper;
};

// WeakPtr<T>

template <class T>
class WeakPtr
{
public:
	WeakPtr() throw() : m_pB(NULL), m_pT(NULL)
	{
	}
	WeakPtr(const WeakPtr<T>& src) throw() : m_pB(src.m_pB), m_pT(src.m_pT)
	{
		//add ref
		if( m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
			assert( pB->m_weakCount > 0 );  //must have weak object
			pB->WeakAddRef();
		}
	}
	WeakPtr(WeakPtr<T>&& src) throw()
	{
		m_pB = src.m_pB;
		m_pT = src.m_pT;
		src.m_pB = NULL;
		src.m_pT = NULL;
	}
	~WeakPtr() throw()
	{
		Release();
	}

	//methods
	void Release() throw()
	{
		if( m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
			assert( pB->m_weakCount > 0 );  //must have weak object
			if( pB->WeakRelease() <= 0 ) {
				//free block
				pB->~SharedPtrBlock();
				SharedPtrBlockHelper::Free(pB);
				m_pB = NULL;
			}
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

	//operators
	WeakPtr<T>& operator=(const WeakPtr<T>& src) throw()
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
					SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
					assert( pB->m_weakCount > 0 );  //must have weak object
					pB->WeakAddRef();
				}
			}
		}
		return *this;
	}
	WeakPtr<T>& operator=(WeakPtr<T>&& src) throw()
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

	bool operator==(const WeakPtr<T>& src) const throw()
	{
		return m_pT == src.m_pT;
	}
	bool operator!=(const WeakPtr<T>& src) const throw()
	{
		return m_pT != src.m_pT;
	}
	bool IsNull() const throw()
	{
		return m_pT == NULL;
	}

private:
	void*  m_pB;   //A pointer to SharedPtrBlock
	T*     m_pT;   //A pointer to object

private:
	friend class SharedPtrHelper;
};

// SharedPtrHelper

class SharedPtrHelper
{
public:
	//make shared
	template <typename T, typename... Args>
	static SharedPtr<T> MakeSharedPtr(const RefPtr<IMemoryManager>& mgr, RefPtr<ITypeProcess>& tp, Args&&... args)
	{
		assert( !mgr.IsNull() );
		assert( !tp.IsNull() );

		//allocate
		SharedPtrBlock* pB = SharedPtrBlockHelper::Allocate();
		if( pB == NULL )
			throw( OutOfMemoryException() );

		T* pT = (T*)mgr.Deref().Allocate(sizeof(T));
		if( pT == NULL ) {
			SharedPtrBlockHelper::Free(pB);
			throw( OutOfMemoryException() );
		}

		SharedPtr<T> ret;
		//constructor
		try {
			//may throw
			call_constructor(*pT, rv_forward<Args>(args)...);

			call_constructor(*pB);  //no throw
			pB->SetMemoryManager(mgr);
			pB->SetTypeProcess(tp);
			pB->m_p = pT;
			ret.m_pB = pB;
			ret.m_pT = pT;
		}
		catch(...) {
			mgr.Deref().Free(pT);
			SharedPtrBlockHelper::Free(pB);
			throw;  //re-throw
		}

		return ret;
	}

	//obtain weak ptr
	template <typename T>
	static WeakPtr<T> ToWeakPtr(SharedPtr<T>& sp) throw()
	{
		WeakPtr<T> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)ret.m_pB;
			assert( pB->m_weakCount > 0 );  //must have weak object
			pB->WeakAddRef();
		}
		return ret;
	}
	//To SharedPtr
	template <typename T>
	static SharedPtr<T> ToSharedPtr(WeakPtr<T>& sp) throw()
	{
		SharedPtr<T> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)ret.m_pB;
			if( !pB->AddRefLock() ) {
				ret.m_pT = NULL;  //shared object freed
				ret.m_pB = NULL;
			}
		}
		return ret;
	}

	//type cast
	template <class T, class TBase>
	static SharedPtr<TBase> TypeCast(SharedPtr<T>& sp) throw()
	{
		SharedPtr<TBase> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)ret.m_pB;
			assert( ret.m_pT != NULL );  //must have shared object
			ret.m_pT = static_cast<TBase*>(ret.m_pT);
			pB->AddRefCopy();
		}
		return ret;
	}
	template <class T, class TBase>
	static WeakPtr<TBase> TypeCast(WeakPtr<T>& sp) throw()
	{
		WeakPtr<TBase> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			assert( ret.m_pT != NULL );  //must have weak object
			ret.m_pT = static_cast<TBase*>(ret.m_pT);
			ret.m_pB->WeakAddRef();
		}
		return ret;
	}

	//clone
	template <typename T>
	static SharedPtr<T> Clone(SharedPtr<T>& sp)  //may throw
	{
		return ( !sp.IsNull() )
			? MakeSharedPtr(sp.m_pB->GetMemoryManager(), sp.m_pB->GetTypeProcess(), sp.Deref());
			: SharedPtr<T>();
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PTR_H__
////////////////////////////////////////////////////////////////////////////////
