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

template <typename T>
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
			assert( pB->GetShareCount() > 0 );  //must have shared object
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
			assert( pB->GetShareCount() > 0 );  //must have shared object
			if( pB->Release() <= 0 ) {
				//free object
				pB->DestroyObject();
				//weak
				assert( pB->GetWeakCount() > 0 );  //must have weak object
				if( pB->WeakRelease() <= 0 ) {
					//free block
					pB->~SharedPtrBlock();
					SharedPtrBlockHelper::Free(pB);
				}
			}
			m_pB = NULL;
			m_pT = NULL;
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

	//operators
	SharedPtr<T>& operator=(const SharedPtr<T>& src) throw()
	{
		if( &src != this ) {
			if( src.m_pB != m_pB ) {
				//release
				Release();
				//assign
				m_pB = src.m_pB;
				m_pT = src.m_pT;
				//add ref
				if( m_pB != NULL ) {
					SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
					assert( pB->GetShareCount() > 0 );  //must have shared object
					pB->AddRefCopy();
				}
			}
		}
		return *this;
	}
	SharedPtr<T>& operator=(SharedPtr<T>&& src) throw()
	{
		if( &src != this ) {
			if( src.m_pB != m_pB ) {
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

	bool operator==(const SharedPtr<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const SharedPtr<T>& right) const throw()
	{
		return !operator==(right);
	}
	bool IsNull() const throw()
	{
		return m_pT == NULL;
	}

	const T& Deref() const throw()
	{
		assert( !IsNull() );
		return *m_pT;
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

template <typename T>
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
			assert( pB->GetWeakCount() > 0 );  //must have weak object
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
			assert( pB->GetWeakCount() > 0 );  //must have weak object
			if( pB->WeakRelease() <= 0 ) {
				//free block
				pB->~SharedPtrBlock();
				SharedPtrBlockHelper::Free(pB);
			}
			m_pB = NULL;
			m_pT = NULL;
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

	//operators
	WeakPtr<T>& operator=(const WeakPtr<T>& src) throw()
	{
		if( &src != this ) {
			if( src.m_pB != m_pB ) {
				//release
				Release();
				//assign
				m_pB = src.m_pB;
				m_pT = src.m_pT;
				if( m_pB != NULL ) {
					//add ref
					SharedPtrBlock* pB = (SharedPtrBlock*)m_pB;
					assert( pB->GetWeakCount() > 0 );  //must have weak object
					pB->WeakAddRef();
				}
			}
		}
		return *this;
	}
	WeakPtr<T>& operator=(WeakPtr<T>&& src) throw()
	{
		if( &src != this ) {
			if( src.m_pB != m_pB ) {
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

	bool operator==(const WeakPtr<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const WeakPtr<T>& right) const throw()
	{
		return m_pT != right.m_pT;
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
	static SharedPtr<T> MakeSharedPtr(const RefPtr<IMemoryManager>& mgr, const RefPtr<ITypeProcess>& tp, Args&&... args)
	{
		assert( !mgr.IsNull() );
		assert( !tp.IsNull() );

		//allocate
		SharedPtrBlock* pB = SharedPtrBlockHelper::Allocate();
		if( pB == NULL )
			throw( OutOfMemoryException() );

		T* pT = (T*)((const_cast<RefPtr<IMemoryManager>&>(mgr)).Deref().Allocate(sizeof(T)));
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
			pB->SetObject(pT);
			ret.m_pB = pB;
			ret.m_pT = pT;
		}
		catch(...) {
			(const_cast<RefPtr<IMemoryManager>&>(mgr)).Deref().Free((uintptr)pT);
			SharedPtrBlockHelper::Free(pB);
			throw;  //re-throw
		}

		return ret;
	}

	//obtain weak ptr
	template <typename T>
	static WeakPtr<T> ToWeakPtr(const SharedPtr<T>& sp) throw()
	{
		WeakPtr<T> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)ret.m_pB;
			assert( pB->GetWeakCount() > 0 );  //must have weak object
			pB->WeakAddRef();
		}
		return ret;
	}
	//To SharedPtr
	template <typename T>
	static SharedPtr<T> ToSharedPtr(const WeakPtr<T>& sp) throw()
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

	//type cast (derived -> base or base -> derived)
	template <class TSrc, class TDest>
	static SharedPtr<TDest> TypeCast(const SharedPtr<TSrc>& sp) throw()
	{
		SharedPtr<TDest> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			SharedPtrBlock* pB = (SharedPtrBlock*)ret.m_pB;
			assert( ret.m_pT != NULL );  //must have shared object
			ret.m_pT = static_cast<TDest*>(ret.m_pT);
			pB->AddRefCopy();
		}
		return ret;
	}
	template <class TSrc, class TDest>
	static WeakPtr<TDest> TypeCast(const WeakPtr<TSrc>& sp) throw()
	{
		WeakPtr<TDest> ret;
		ret.m_pT = sp.m_pT;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			assert( ret.m_pT != NULL );  //must have weak object
			ret.m_pT = static_cast<TDest*>(ret.m_pT);
			ret.m_pB->WeakAddRef();
		}
		return ret;
	}

	//clone
	template <typename T>
	static SharedPtr<T> Clone(const SharedPtr<T>& sp)  //may throw
	{
		return ( !sp.IsNull() )
				? MakeSharedPtr(sp.m_pB->GetMemoryManager(), sp.m_pB->GetTypeProcess(), sp.Deref())
				: SharedPtr<T>();
	}

	//get internal pointer
	template <typename T>
	static T* GetInternalPointer(const SharedPtr<T>& sp) throw()
	{
		return sp.m_pT;
	}
	template <typename T>
	static T* GetInternalPointer(const WeakPtr<T>& sp) throw()
	{
		return sp.m_pT;
	}
	template <typename T>
	static SharedPtrBlock* GetBlockPointer(const SharedPtr<T>& sp) throw()
	{
		return (SharedPtrBlock*)(sp.m_pB);
	}
	template <typename T>
	static SharedPtrBlock* GetBlockPointer(const WeakPtr<T>& sp) throw()
	{
		return (SharedPtrBlock*)(sp.m_pB);
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_PTR_H__
////////////////////////////////////////////////////////////////////////////////
