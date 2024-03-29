﻿/*
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
This file contains GkcSys functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __SA_GKC_SYS_H__
#define __SA_GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// CRT MemoryManager

SA_FUNCTION GKC::IMemoryManager* _CrtMemoryManager_Get() throw();

SA_FUNCTION GKC::Mutex* _MemoryMutex_Get() throw();

//------------------------------------------------------------------------------
// share_ptr_block pool

SA_FUNCTION share_ptr_block* _SpbPool_Allocate() throw();
SA_FUNCTION void _SpbPool_Free(share_ptr_block* p) throw();

//------------------------------------------------------------------------------
// share_array_block pool

SA_FUNCTION share_array_block* _SabPool_Allocate() throw();
SA_FUNCTION void _SabPool_Free(share_array_block* p) throw();

//------------------------------------------------------------------------------
// share_com_block pool

SA_FUNCTION share_com_block* _ScbPool_Allocate() throw();
SA_FUNCTION void _ScbPool_Free(share_com_block* p) throw();

//------------------------------------------------------------------------------
// classes

#pragma pack(push, 1)

// _SObjHelper

class _SObjHelper
{
public:
	static void AddShareRef(share_block_base* p) throw()
	{
		assert( p->GetShareCount() > 0 );  //must have share object
		p->AddRefCopy();
	}
	static void AddWeakRef(share_block_base* p) throw()
	{
		assert( p->GetWeakCount() > 0 );  //must have weak object
		p->WeakAddRef();
	}
	static void MoveDirect(void*& pd, void*& ps) throw()
	{
		pd = ps;
		ps = NULL;
	}
};

// _SObjRelease<T>

template <class T>
class _SObjRelease
{
};

template <>
class _SObjRelease<share_ptr_block>
{
public:
	static void WeakRelease(share_block_base* p) throw()
	{
		assert( p->GetWeakCount() > 0 );  //must have weak object
		if( p->WeakRelease() <= 0 ) {
			//free block
			::_SpbPool_Free(static_cast<share_ptr_block*>(p));
		}
	}
	static void ShareRelease(share_block_base* p) throw()
	{
		assert( p->GetShareCount() > 0 );  //must have share object
		if( p->Release() <= 0 ) {
			//free object
			(static_cast<share_ptr_block*>(p))->DestroyObject();
			//weak
			WeakRelease(p);
		}
	}
};
template <>
class _SObjRelease<share_com_block>
{
public:
	static void WeakRelease(share_block_base* p) throw()
	{
		assert( p->GetWeakCount() > 0 );  //must have weak object
		if( p->WeakRelease() <= 0 ) {
			//free block
			::_ScbPool_Free(static_cast<share_com_block*>(p));
		}
	}
	static void ShareRelease(share_block_base* p) throw()
	{
		assert( p->GetShareCount() > 0 );  //must have share object
		if( p->Release() <= 0 ) {
			//free com object
			(static_cast<share_com_block*>(p))->DestroyObject();
			//weak
			WeakRelease(p);
		}
	}
};
template <>
class _SObjRelease<share_array_block>
{
public:
	static void WeakRelease(share_block_base* p) throw()
	{
		assert( p->GetWeakCount() > 0 );  //must have weak object
		if( p->WeakRelease() <= 0 ) {
			//free block
			::_SabPool_Free(static_cast<share_array_block*>(p));
		}
	}
	template <typename T>
	static void ShareRelease(share_block_base* p) throw()
	{
		assert( p->GetShareCount() > 0 );  //must have share object
		if( p->Release() <= 0 ) {
			//free array
			(static_cast<share_array_block*>(p))->DestroyArray<T>();
			//weak
			WeakRelease(p);
		}
	}
};

// _ShareObjectBase

class _ShareObjectBase
{
protected:
	_ShareObjectBase() throw() : m_pB(NULL)
	{
	}
	_ShareObjectBase(const _ShareObjectBase& src) throw() : m_pB(src.m_pB)
	{
		AddRef();
	}
	_ShareObjectBase(_ShareObjectBase&& src) throw()
	{
		_SObjHelper::MoveDirect((void*&)(m_pB), (void*&)(src.m_pB));
	}
	~_ShareObjectBase() throw()
	{
	}

protected:
	void AddRef() throw()
	{
		if( m_pB != NULL )
			_SObjHelper::AddShareRef(m_pB);
	}

public:
	/*! \brief Check if the object is NULL.

	Check if the object is NULL.
	\return true for NULL object, false for otherwise.
	*/
	bool IsBlockNull() const throw()
	{
		return m_pB == NULL;
	}
	bool IsNullObject() const throw()
	{
		return IsBlockNull();
	}

protected:
	share_block_base* m_pB;
};

// _WeakObjectBase

class _WeakObjectBase
{
protected:
	_WeakObjectBase() throw() : m_pB(NULL)
	{
	}
	_WeakObjectBase(const _WeakObjectBase& src) throw() : m_pB(src.m_pB)
	{
		AddRef();
	}
	_WeakObjectBase(_WeakObjectBase&& src) throw()
	{
		_SObjHelper::MoveDirect((void*&)(m_pB), (void*&)(src.m_pB));
	}
	~_WeakObjectBase() throw()
	{
	}

protected:
	void AddRef() throw()
	{
		if( m_pB != NULL )
			_SObjHelper::AddWeakRef(m_pB);
	}

public:
	bool IsBlockNull() const throw()
	{
		return m_pB == NULL;
	}
	bool IsNullObject() const throw()
	{
		return IsBlockNull();
	}

protected:
	share_block_base* m_pB;
};

// _SObjT<T, TBase>

template <class T, class TBase>
class _SObjT : public TBase
{
private:
	typedef TBase  baseClass;
	typedef _SObjT<T, TBase>  thisClass;

protected:
	_SObjT() throw()
	{
	}
	_SObjT(const _SObjT& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_SObjT(_SObjT&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_SObjT() throw()
	{
	}

	thisClass& operator=(const thisClass& src) throw()
	{
		if( &src != this ) {
			if( src.m_pB != baseClass::m_pB ) {
				//release
				T* pT = static_cast<T*>(this);
				pT->Release();
				//assign
				baseClass::m_pB = src.m_pB;
				pT->do_assign(src);
				//add ref
				baseClass::AddRef();
			}
		}
		return *this;
	}
	thisClass& operator=(thisClass&& src) throw()
	{
		if( &src != this ) {
			//release
			T* pT = static_cast<T*>(this);
			pT->Release();
			//move
			_SObjHelper::MoveDirect((void*&)(baseClass::m_pB), (void*&)(src.m_pB));
			pT->do_move(rv_forward(src));
		}
		return *this;
	}

private:
	//overrideable
	void Release() throw();
	void do_assign(const thisClass& src) throw();
	void do_move(thisClass&& src) throw();
};

// _ShareSoloBase<TBlock>

template <class TBlock>
class _ShareSoloBase : public _SObjT<_ShareSoloBase<TBlock>, _ShareObjectBase>
{
private:
#define _SHARESOLOBASE_BASE_CLASS  _SObjT<_ShareSoloBase<TBlock>, _ShareObjectBase>

	typedef _ShareSoloBase<TBlock>  thisClass;

protected:
	/*! \brief Constructor.

	Constructor.
	*/
	_ShareSoloBase() throw() : m_pT(NULL)
	{
	}
	/*! \brief Copy constructor.

	Copy constructor.
	*/
	_ShareSoloBase(const _ShareSoloBase& src) throw() : _SHARESOLOBASE_BASE_CLASS(static_cast<const _SHARESOLOBASE_BASE_CLASS&>(src)), m_pT(src.m_pT)
	{
	}
	_ShareSoloBase(_ShareSoloBase&& src) throw() : _SHARESOLOBASE_BASE_CLASS(rv_forward(static_cast<_SHARESOLOBASE_BASE_CLASS&>(src)))
	{
		_SObjHelper::MoveDirect((void*&)(m_pT), (void*&)(src.m_pT));
	}
	/*! \brief Destructor.

	Destructor.
	*/
	~_ShareSoloBase() throw()
	{
		Release();
	}

	/*! \brief Assignment operator.

	Assignment operator.
	*/
	_ShareSoloBase<TBlock>& operator=(const _ShareSoloBase<TBlock>& src) throw()
	{
		_SHARESOLOBASE_BASE_CLASS::operator=(static_cast<const _SHARESOLOBASE_BASE_CLASS&>(src));
		return *this;
	}
	_ShareSoloBase<TBlock>& operator=(_ShareSoloBase<TBlock>&& src) throw()
	{
		_SHARESOLOBASE_BASE_CLASS::operator=(rv_forward(static_cast<_SHARESOLOBASE_BASE_CLASS&>(src)));
		return *this;
	}

//overriders

public:
	/*! \brief Free the object.

	Free the object.
	*/
	void Release() throw()
	{
		if( _SHARESOLOBASE_BASE_CLASS::m_pB != NULL ) {
			_SObjRelease<TBlock>::ShareRelease(_SHARESOLOBASE_BASE_CLASS::m_pB);
			_SHARESOLOBASE_BASE_CLASS::m_pB = NULL;
			m_pT = NULL;
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

protected:
	void do_assign(const _SHARESOLOBASE_BASE_CLASS& src) throw()
	{
		m_pT = (static_cast<const thisClass&>(src)).m_pT;
	}
	void do_move(_SHARESOLOBASE_BASE_CLASS&& src) throw()
	{
		_SObjHelper::MoveDirect((void*&)(m_pT), (void*&)((static_cast<thisClass&>(src)).m_pT));
	}

protected:
	void* m_pT;  //!< A pointer to object

private:
	friend class _SHARESOLOBASE_BASE_CLASS;
};

// _WeakSoloBase<TBlock>

template <class TBlock>
class _WeakSoloBase : public _SObjT<_WeakSoloBase<TBlock>, _WeakObjectBase>
{
private:
#define _WEAKSOLOBASE_BASE_CLASS  _SObjT<_WeakSoloBase<TBlock>, _WeakObjectBase>

	typedef _WeakSoloBase<TBlock>  thisClass;

protected:
	_WeakSoloBase() throw() : m_pT(NULL)
	{
	}
	_WeakSoloBase(const _WeakSoloBase& src) throw() : _WEAKSOLOBASE_BASE_CLASS(static_cast<const _WEAKSOLOBASE_BASE_CLASS&>(src)), m_pT(src.m_pT)
	{
	}
	_WeakSoloBase(_WeakSoloBase&& src) throw() : _WEAKSOLOBASE_BASE_CLASS(rv_forward(static_cast<_WEAKSOLOBASE_BASE_CLASS&>(src)))
	{
		_SObjHelper::MoveDirect((void*&)(m_pT), (void*&)(src.m_pT));
	}
	~_WeakSoloBase() throw()
	{
		Release();
	}

	_WeakSoloBase<TBlock>& operator=(const _WeakSoloBase<TBlock>& src) throw()
	{
		_WEAKSOLOBASE_BASE_CLASS::operator=(static_cast<const _WEAKSOLOBASE_BASE_CLASS&>(src));
		return *this;
	}
	_WeakSoloBase<TBlock>& operator=(_WeakSoloBase<TBlock>&& src) throw()
	{
		_WEAKSOLOBASE_BASE_CLASS::operator=(rv_forward(static_cast<_WEAKSOLOBASE_BASE_CLASS&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( _WEAKSOLOBASE_BASE_CLASS::m_pB != NULL ) {
			//weak
			_SObjRelease<TBlock>::WeakRelease(_WEAKSOLOBASE_BASE_CLASS::m_pB);
			_WEAKSOLOBASE_BASE_CLASS::m_pB = NULL;
			m_pT = NULL;
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

protected:
	void do_assign(const _WEAKSOLOBASE_BASE_CLASS& src) throw()
	{
		m_pT = (static_cast<const thisClass&>(src)).m_pT;
	}
	void do_move(_WEAKSOLOBASE_BASE_CLASS&& src) throw()
	{
		_SObjHelper::MoveDirect((void*&)(m_pT), (void*&)((static_cast<thisClass&>(src)).m_pT));
	}

protected:
	void* m_pT;  //A pointer to object

private:
	friend class _WEAKSOLOBASE_BASE_CLASS;
};

// _SharePtr<T>
/*! \brief A template class for shared pointer.

A template class for shared pointer.
\tparam T The object type.
*/
template <typename T>
class _SharePtr : public _ShareSoloBase<share_ptr_block>
{
private:
	typedef _ShareSoloBase<share_ptr_block>  baseClass;
	typedef _SharePtr<T>  thisClass;

public:
	/*! \brief Constructor.

	Constructor.
	*/
	_SharePtr() throw()
	{
	}
	/*! \brief Copy constructor.

	Copy constructor.
	*/
	_SharePtr(const _SharePtr& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	/*! \brief Right-value copy constructor.

	Right-value copy constructor.
	*/
	_SharePtr(_SharePtr&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	/*! \brief Destructor.

	Destructor.
	*/
	~_SharePtr() throw()
	{
	}

	/*! \brief Assignment operator.

	Assignment operator.
	*/
	_SharePtr<T>& operator=(const _SharePtr<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	/*! \brief Right-value assignment operator.

	Right-value assignment operator.
	*/
	_SharePtr<T>& operator=(_SharePtr<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _SharePtr<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const _SharePtr<T>& right) const throw()
	{
		return !operator==(right);
	}

	/*! \brief Get the object.

	Get the object.
	\return A reference to object.
	*/
	const T& Deref() const throw()
	{
		assert( !IsBlockNull() && m_pT != NULL );
		return *((T*)m_pT);
	}
	T& Deref() throw()
	{
		assert( !IsBlockNull() && m_pT != NULL );
		return *((T*)m_pT);
	}

private:
	friend class _SObjSoloHelper;
	friend class _SharePtrHelper;
};

// _WeakPtr<T>

template <typename T>
class _WeakPtr : public _WeakSoloBase<share_ptr_block>
{
private:
	typedef _WeakSoloBase<share_ptr_block>  baseClass;
	typedef _WeakPtr<T>  thisClass;

public:
	_WeakPtr() throw()
	{
	}
	_WeakPtr(const _WeakPtr& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_WeakPtr(_WeakPtr&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_WeakPtr() throw()
	{
	}

	_WeakPtr<T>& operator=(const _WeakPtr<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_WeakPtr<T>& operator=(_WeakPtr<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _WeakPtr<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const _WeakPtr<T>& right) const throw()
	{
		return !operator==(right);
	}

private:
	friend class _SObjSoloHelper;
};

// _SObjSoloHelper

class _SObjSoloHelper
{
public:
	//obtain weak object
	template <class TSObj, class TWObj>
	static TWObj ToWeak(const TSObj& sp) throw()
	{
		TWObj ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			_SObjHelper::AddWeakRef(ret.m_pB);
			ret.m_pT = sp.m_pT;
		}
		return ret;
	}
	//to share object
	template <class TSObj, class TWObj>
	static TSObj ToShare(const TWObj& sp) throw()
	{
		TSObj ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			if( !((ret.m_pB)->AddRefLock()) )
				ret.m_pB = NULL;  //share object freed
			else
				ret.m_pT = sp.m_pT;
		}
		return ret;
	}

	//type cast (derived -> base or base -> derived)
	template <class TDest, class TSObjS, class TSObjD>
	static TSObjD Share_TypeCast(const TSObjS& sp) throw()
	{
		TSObjD ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			assert( sp.m_pT != NULL );  //must have share object
			ret.m_pT = static_cast<TDest*>(sp.m_pT);
			(ret.m_pB)->AddRefCopy();
		}
		return ret;
	}
	template <class TDest, class TWObjS, class TWObjD>
	static TWObjD Weak_TypeCast(const TWObjS& sp) throw()
	{
		TWObjD ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			assert( sp.m_pT != NULL );  //must have weak object
			ret.m_pT = static_cast<TDest*>(sp.m_pT);
			(ret.m_pB)->WeakAddRef();
		}
		return ret;
	}

	//get internal pointer
	template <typename T, class TObj>
	static T* GetInternalPointer(const TObj& sp) throw()
	{
		return (T*)(sp.m_pT);
	}

	//only for test
	template <class TObj>
	static int test_get_share_count(const TObj& sp) throw()
	{
		return sp.IsBlockNull() ? 0 : (sp.m_pB)->GetShareCount();
	}
	template <class TObj>
	static int test_get_weak_count(const TObj& sp) throw()
	{
		return sp.IsBlockNull() ? 0 : (sp.m_pB)->GetWeakCount();
	}

	//for internal
	//  no-inline
	template <typename T>
	static void object_destruction(void* p) throw()
	{
		((T*)p)->~T();
	}
};

// _SObjConnectionImpl<TSObjArray>
//   TSObjArray : SObjArray<TSObj> or SObjArrayWithLock<TSObj>

template <class TSObjArray>
class _SObjConnectionImpl
{
public:
	_SObjConnectionImpl() throw()
	{
	}
	~_SObjConnectionImpl() throw()
	{
	}

	void SetLock(const GKC::RefPtr<GKC::Mutex>& mtx) throw()
	{
		m_arr.SetMutex(mtx);  //may be no-matching
	}
	uintptr Add(const typename TSObjArray::EObj& obj) throw()
	{
		return m_arr.Add(obj);
	}
	void Remove(uintptr uCookie) throw()
	{
		m_arr.Remove(uCookie);
	}
	void GetObject(uintptr index, typename TSObjArray::EObj& obj) const throw()
	{
		m_arr.GetObject(index, obj);
	}
	uintptr GetCount() const throw()
	{
		return m_arr.GetCount();
	}

private:
	TSObjArray m_arr;

private:
	//noncopyable
	_SObjConnectionImpl(const _SObjConnectionImpl&) throw();
	_SObjConnectionImpl& operator=(const _SObjConnectionImpl&) throw();
};

/*
_SharePtr:
User can define a proxy class named as Proxy_<event_class_name> derived from _SObjConnectionImpl<SObjArray<_WeakPtr<event_class_name>>> or _SObjConnectionImpl<SObjArrayWithLock<_WeakPtr<event_class_name>>>.
This proxy class provides the fire functions with Fire_<event_method_name> format, and is used as the base class of main class.
*/

// _SharePtrHelper

class _SharePtrHelper
{
public:
	//make share object
	template <typename T, typename... Args>
	static _SharePtr<T> MakeSharePtr(const GKC::RefPtr<GKC::IMemoryManager>& mgr, Args&&... args)
	{
		assert( !mgr.IsNull() );

		//allocate
		share_ptr_block* pB = ::_SpbPool_Allocate();
		if( pB == NULL )
			throw GKC::OutOfMemoryException();

		T* pT = (T*)((const_cast<GKC::RefPtr<GKC::IMemoryManager>&>(mgr)).Deref().Allocate(sizeof(T)));
		if( pT == NULL ) {
			::_SpbPool_Free(pB);
			throw GKC::OutOfMemoryException();
		}

		//constructor
		try {
			//may throw
			call_constructor(*pT, rv_forward<Args>(args)...);
		}
		catch(...) {
			(const_cast<GKC::RefPtr<GKC::IMemoryManager>&>(mgr)).Deref().Free((uintptr)pT);
			::_SpbPool_Free(pB);
			throw;  //re-throw
		}

		//initialize
		pB->SetMemoryManager(GKC::RefPtrHelper::GetInternalPointer(mgr));
		pB->SetAddress(pT);
		pB->SetDestructionFunc(&(_SObjSoloHelper::object_destruction<T>));

		_SharePtr<T> ret;
		//return value
		ret.m_pB = static_cast<share_block_base*>(pB);
		ret.m_pT = pT;

		return ret;
	}

	//obtain weak ptr
	template <typename T>
	static _WeakPtr<T> ToWeakPtr(const _SharePtr<T>& sp) throw()
	{
		return _SObjSoloHelper::ToWeak<_SharePtr<T>, _WeakPtr<T>>(sp);
	}
	//to share ptr
	template <typename T>
	static _SharePtr<T> ToSharePtr(const _WeakPtr<T>& sp) throw()
	{
		return _SObjSoloHelper::ToShare<_SharePtr<T>, _WeakPtr<T>>(sp);
	}

	//type cast (derived -> base or base -> derived)
	template <class TSrc, class TDest>
	static _SharePtr<TDest> TypeCast(const _SharePtr<TSrc>& sp) throw()
	{
		return _SObjSoloHelper::Share_TypeCast<TDest, _SharePtr<TSrc>, _SharePtr<TDest>>(sp);
	}
	template <class TSrc, class TDest>
	static _WeakPtr<TDest> TypeCast(const _WeakPtr<TSrc>& sp) throw()
	{
		return _SObjSoloHelper::Weak_TypeCast<TDest, _WeakPtr<TSrc>, _WeakPtr<TDest>>(sp);
	}

	//clone
	template <typename T>
	static _SharePtr<T> Clone(const _SharePtr<T>& sp)  //may throw
	{
		return ( !sp.IsBlockNull() )
				? ( assert( sp.m_pT != NULL ), MakeSharePtr(GKC::RefPtr<GKC::IMemoryManager>((sp.m_pB)->GetMemoryManager()), sp.Deref()) )
				: _SharePtr<T>();
	}

	//event
	template <class T, class TProxy>
	static void SetLock(const _SharePtr<T>& sp, const GKC::RefPtr<GKC::Mutex>& mtx) throw()
	{
		assert( sp.m_pB != NULL );
		TProxy* pX = static_cast<TProxy*>(sp.m_pT);
		pX->SetLock(mtx);
	}
	template <class T, class TEvent, class TProxy>
	static uintptr Advise(const _SharePtr<T>& sp, const _WeakPtr<TEvent>& spE) throw()
	{
		assert( sp.m_pB != NULL );
		TProxy* pX = static_cast<TProxy*>(sp.m_pT);
		return pX->Add(spE);
	}
	template <class T, class TProxy>
	static void Unadvise(const _SharePtr<T>& sp, uintptr uCookie) throw()
	{
		assert( sp.m_pB != NULL );
		TProxy* pX = static_cast<TProxy*>(sp.m_pT);
		pX->Remove(uCookie);
	}

	//get internal pointer
	template <typename T>
	static T* GetInternalPointer(const _SharePtr<T>& sp) throw()
	{
		return _SObjSoloHelper::GetInternalPointer<T, _SharePtr<T>>(sp);
	}
	template <typename T>
	static T* GetInternalPointer(const _WeakPtr<T>& sp) throw()
	{
		return _SObjSoloHelper::GetInternalPointer<T, _WeakPtr<T>>(sp);
	}

	//only for test
	template <typename T>
	static int test_get_share_count(const _SharePtr<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_share_count<_SharePtr<T>>(sp);
	}
	template <typename T>
	static int test_get_weak_count(const _SharePtr<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_weak_count<_SharePtr<T>>(sp);
	}
	template <typename T>
	static int test_get_share_count(const _WeakPtr<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_share_count<_WeakPtr<T>>(sp);
	}
	template <typename T>
	static int test_get_weak_count(const _WeakPtr<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_weak_count<_WeakPtr<T>>(sp);
	}
};

// _ShareCom<T>

template <class T>
class _ShareCom : public _ShareSoloBase<share_com_block>
{
public:
	typedef T  EType;

private:
	typedef _ShareSoloBase<share_com_block>  baseClass;
	typedef _ShareCom<T>  thisClass;

public:
	_ShareCom() throw()
	{
	}
	_ShareCom(const _ShareCom& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_ShareCom(_ShareCom&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_ShareCom() throw()
	{
	}

	_ShareCom<T>& operator=(const _ShareCom<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_ShareCom<T>& operator=(_ShareCom<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _ShareCom<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const _ShareCom<T>& right) const throw()
	{
		return !operator==(right);
	}

	const T& Deref() const throw()
	{
		assert( !IsBlockNull() && m_pT != NULL );
		return *((T*)m_pT);
	}
	T& Deref() throw()
	{
		assert( !IsBlockNull() && m_pT != NULL );
		return *((T*)m_pT);
	}

private:
	friend class _SObjSoloHelper;
	friend class _ShareComHelper;
};

// _ShareCom<void>

template <>
class _ShareCom<void> : public _ShareSoloBase<share_com_block>
{
private:
	typedef _ShareSoloBase<share_com_block>  baseClass;
	typedef _ShareCom<void>  thisClass;

public:
	_ShareCom() throw()
	{
	}
	_ShareCom(const _ShareCom& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_ShareCom(_ShareCom&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_ShareCom() throw()
	{
	}

	_ShareCom<void>& operator=(const _ShareCom<void>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_ShareCom<void>& operator=(_ShareCom<void>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _ShareCom<void>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const _ShareCom<void>& right) const throw()
	{
		return !operator==(right);
	}

private:
	friend class _SObjSoloHelper;
	friend class _ShareComHelper;
};

// _WeakCom<T>

template <class T>
class _WeakCom : public _WeakSoloBase<share_com_block>
{
private:
	typedef _WeakSoloBase<share_com_block>  baseClass;
	typedef _WeakCom<T>  thisClass;

public:
	_WeakCom() throw()
	{
	}
	_WeakCom(const _WeakCom& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_WeakCom(_WeakCom&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_WeakCom() throw()
	{
	}

	_WeakCom<T>& operator=(const _WeakCom<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_WeakCom<T>& operator=(_WeakCom<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _WeakCom<T>& right) const throw()
	{
		return m_pT == right.m_pT;
	}
	bool operator!=(const _WeakCom<T>& right) const throw()
	{
		return !operator==(right);
	}

private:
	friend class _SObjSoloHelper;
};

// _IComConnection

class NOVTABLE _IComConnection
{
public:
	virtual uintptr Advise(const _WeakCom<void>& sp) throw() = 0;
	virtual void Unadvise(const uintptr& uCookie) throw() = 0;
};

// --only one component class can be defined in a pair of .h and .cpp files.--

struct _Com_Interface_Offset_Item
{
	const guid*  pid;
	intptr       offset;
};

namespace GKC {
template <class T>
class _Com_TypeCast_Func
{
};
template <class T>
class _Com_Connection_Func
{
public:
	static object_connection_func GetFunc() throw()
	{
		return NULL;
	}
};
}

inline void* _Com_Get_Base_Object(void* p, const guid& iid,
								const _Com_Interface_Offset_Item* pItem, uintptr uCount) throw()
{
	for( uintptr i = 0; i < uCount; i ++ ) {
		if( guid_equal(*(pItem->pid), iid) ) {
			return (void*)((byte*)p + pItem->offset);
		}
		pItem ++;
	}
	return NULL;
}

// --<header file>--

#define DECLARE_COM_TYPECAST(cls)  \
	DECLARE_STATIC_CONST_ARRAY(com_ca_##cls, _Com_Interface_Offset_Item)  \
	class _Com_TC_##cls {  \
	public:  \
		BEGIN_NOINLINE  \
		static void* _Com_TypeCast(void* p, const guid& iid) throw()  \
		END_NOINLINE  \
		{ return _Com_Get_Base_Object(p, iid, com_ca_##cls ::GetAddress(), com_ca_##cls ::GetCount()); }  \
	};  \
	template <> class _Com_TypeCast_Func<cls> { public:  \
		static const object_typecast_func c_func;  \
	};

#define DECLARE_COM_CONNECTION(cls)  \
	DECLARE_STATIC_CONST_ARRAY(com_cc_##cls, _Com_Interface_Offset_Item)  \
	class _Com_CN_##cls {  \
	public:  \
		BEGIN_NOINLINE  \
		static void* _Com_Connection(void* p, const guid& iid) throw()  \
		END_NOINLINE  \
		{ return _Com_Get_Base_Object(p, iid, com_cc_##cls ::GetAddress(), com_cc_##cls ::GetCount()); }  \
	};  \
	template <> class _Com_Connection_Func<cls> { public:  \
		static object_connection_func GetFunc() throw()  \
		{ return &(_Com_CN_##cls ::_Com_Connection); }  \
	};

// --<.h end>--

// --<source file>--

#define BEGIN_COM_TYPECAST(cls)  \
	typedef cls com_x_class;  \
	typedef com_ca_##cls  com_ca_class;  \
	typedef _Com_TC_##cls  com_tc_class;  \
	BEGIN_STATIC_CONST_ARRAY(com_ca_class)

// ifname : interface name, ibname : implemented base class name
#define COM_TYPECAST_ENTRY(ifname, ibname)  \
	BEGIN_STATIC_CONST_ARRAY_GROUP()  \
		STATIC_CONST_ARRAY_ENTRY(&(USE_GUID(GUID_##ifname)))  \
		STATIC_CONST_ARRAY_ENTRY_LAST( (intptr)( (byte*)(static_cast<ifname*>(static_cast<ibname*>((com_x_class*)(0x128)))) - (byte*)(0x128) ) )  \
	END_STATIC_CONST_ARRAY_GROUP()

#define END_COM_TYPECAST()  \
	BEGIN_STATIC_CONST_ARRAY_GROUP()  \
		STATIC_CONST_ARRAY_ENTRY(NULL)  \
		STATIC_CONST_ARRAY_ENTRY_LAST(0)  \
	END_STATIC_CONST_ARRAY_GROUP_LAST()  \
	END_STATIC_CONST_ARRAY(com_ca_class)  \
	const object_typecast_func _Com_TypeCast_Func<com_x_class>::c_func = &(com_tc_class::_Com_TypeCast);

#define BEGIN_COM_CONNECTION(cls)  \
	typedef cls com_y_class;  \
	typedef com_cc_##cls  com_cc_class;  \
	BEGIN_STATIC_CONST_ARRAY(com_cc_class)

// if_name : event interface name
#define COM_CONNECTION_ENTRY(ifname)  \
	BEGIN_STATIC_CONST_ARRAY_GROUP()  \
		STATIC_CONST_ARRAY_ENTRY(&(USE_GUID(GUID_##ifname)))  \
		STATIC_CONST_ARRAY_ENTRY_LAST( (intptr)( (byte*)(static_cast<_IComConnection*>(static_cast< Proxy_##ifname *>((com_y_class*)(0x128)))) - (byte*)(0x128) ) )  \
	END_STATIC_CONST_ARRAY_GROUP()

#define END_COM_CONNECTION()  \
	BEGIN_STATIC_CONST_ARRAY_GROUP()  \
		STATIC_CONST_ARRAY_ENTRY(NULL)  \
		STATIC_CONST_ARRAY_ENTRY_LAST(0)  \
	END_STATIC_CONST_ARRAY_GROUP_LAST()  \
	END_STATIC_CONST_ARRAY(com_cc_class)

// --<.cpp end>--

// _ScopeShareComObject<T>

template <class T>
class _ScopeShareComObject : public share_com_block, public T
{
public:
	_ScopeShareComObject()
	{
		share_com_block::m_p = static_cast<T*>(this);
		share_com_block::SetTypeCastFunc(GKC::_Com_TypeCast_Func<T>::c_func);
		share_com_block::SetConnectionFunc(GKC::_Com_Connection_Func<T>::GetFunc());
	}
	_ScopeShareComObject(const _ScopeShareComObject&) = delete;
	_ScopeShareComObject& operator=(const _ScopeShareComObject&) = delete;
	~_ScopeShareComObject() noexcept
	{
	}

private:
	friend class _ShareComHelper;
};

// _ShareComHelper

class _ShareComHelper
{
public:
	//make share component object
	template <class T, typename... Args>
	static _ShareCom<T> MakeShareCom(const GKC::RefPtr<GKC::IMemoryManager>& mgr, Args&&... args)
	{
		assert( !mgr.IsNull() );

		//allocate
		share_com_block* pB = ::_ScbPool_Allocate();
		if( pB == NULL )
			throw GKC::OutOfMemoryException();

		T* pT = (T*)((const_cast<GKC::RefPtr<GKC::IMemoryManager>&>(mgr)).Deref().Allocate(sizeof(T)));
		if( pT == NULL ) {
			::_ScbPool_Free(pB);
			throw GKC::OutOfMemoryException();
		}

		//constructor
		try {
			//may throw
			call_constructor(*pT, rv_forward<Args>(args)...);
		}
		catch(...) {
			(const_cast<GKC::RefPtr<GKC::IMemoryManager>&>(mgr)).Deref().Free((uintptr)pT);
			::_ScbPool_Free(pB);
			throw;  //re-throw
		}

		//initialize
		pB->SetMemoryManager(GKC::RefPtrHelper::GetInternalPointer(mgr));
		pB->SetAddress(pT);
		pB->SetDestructionFunc(&(_SObjSoloHelper::object_destruction<T>));
		pB->SetTypeCastFunc(GKC::_Com_TypeCast_Func<T>::c_func);
		pB->SetConnectionFunc(GKC::_Com_Connection_Func<T>::GetFunc());

		_ShareCom<T> ret;
		//return value
		ret.m_pB = static_cast<share_block_base*>(pB);
		ret.m_pT = pT;

		return ret;
	}

	//obtain weak com
	template <class T>
	static _WeakCom<T> ToWeakCom(const _ShareCom<T>& sp) throw()
	{
		return _SObjSoloHelper::ToWeak<_ShareCom<T>, _WeakCom<T>>(sp);
	}
	//to share com
	template <class T>
	static _ShareCom<T> ToShareCom(const _WeakCom<T>& sp) throw()
	{
		return _SObjSoloHelper::ToShare<_ShareCom<T>, _WeakCom<T>>(sp);
	}

	//type cast (derived -> base or base -> derived)
	template <class TSrc, class TDest>
	static _ShareCom<TDest> TypeCast(const _ShareCom<TSrc>& sp) throw()
	{
		return _SObjSoloHelper::Share_TypeCast<TDest, _ShareCom<TSrc>, _ShareCom<TDest>>(sp);
	}
	template <class TSrc, class TDest>
	static _WeakCom<TDest> TypeCast(const _WeakCom<TSrc>& sp) throw()
	{
		return _SObjSoloHelper::Weak_TypeCast<TDest, _WeakCom<TSrc>, _WeakCom<TDest>>(sp);
	}

	//query type
	// TSrc : interface, TDest : interface supported by component class (direct base class with first level)
	template <class TSrc>
	static _ShareCom<void> Query(const _ShareCom<TSrc>& sp, const guid& iid) throw()
	{
		_ShareCom<void> ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			assert( !((ret.m_pB)->IsNull()) );
			ret.m_pT = ((static_cast<share_com_block*>(ret.m_pB))->GetTypeCastFunc())((ret.m_pB)->GetAddress(), iid);
			if( ret.m_pT != NULL )
				(ret.m_pB)->AddRefCopy();
			else
				ret.m_pB = NULL;
		}
		return ret;
	}
	template <class TSrc, class TDest>
	static _ShareCom<TDest> Query(const _ShareCom<TSrc>& sp, const guid& iid) throw()
	{
		return TypeCast<void, TDest>(Query<TSrc>(sp, iid));
	}

	//clone
	// T : the component class
	template <class T>
	static _ShareCom<T> Clone(const _ShareCom<T>& sp)  //may throw
	{
		return ( !sp.IsBlockNull() )
				? ( assert( sp.m_pT != NULL ), MakeShareCom(GKC::RefPtr<GKC::IMemoryManager>((sp.m_pB)->GetMemoryManager()), sp.Deref()) )
				: _ShareCom<T>();
	}

	//event
	template <class T, class TEvent>
	static uintptr Advise(const _ShareCom<T>& sp, const guid& iid, const _WeakCom<TEvent>& spE) throw()
	{
		if( sp.IsBlockNull() )
			return 0;
		_IComConnection* pC = (_IComConnection*)(((static_cast<share_com_block*>(sp.m_pB))->GetConnectionFunc())((sp.m_pB)->GetAddress(), iid));
		if( pC == NULL )
			return 0;
		return pC->Advise(TypeCast<TEvent, void>(spE));
	}
	template <class T>
	static void Unadvise(const _ShareCom<T>& sp, const guid& iid, uintptr uCookie) throw()
	{
		if( sp.IsBlockNull() )
			return ;
		_IComConnection* pC = (_IComConnection*)(((static_cast<share_com_block*>(sp.m_pB))->GetConnectionFunc())((sp.m_pB)->GetAddress(), iid));
		if( pC == NULL )
			return ;
		pC->Unadvise(uCookie);
	}

	//scope
	template <class T>
	static _ShareCom<T> ToShareCom(const _ScopeShareComObject<T>& sp) throw()
	{
		_ShareCom<T> ret;
		ret.m_pT = sp.m_p;
		ret.m_pB = const_cast<share_com_block*>(static_cast<const share_com_block*>(&sp));
		ret.m_pB->AddRefCopy();
		return ret;
	}

	//get internal pointer
	template <class T>
	static T* GetInternalPointer(const _ShareCom<T>& sp) throw()
	{
		return _SObjSoloHelper::GetInternalPointer<T, _ShareCom<T>>(sp);
	}
	template <class T>
	static T* GetInternalPointer(const _WeakCom<T>& sp) throw()
	{
		return _SObjSoloHelper::GetInternalPointer<T, _WeakCom<T>>(sp);
	}

	//only for test
	template <class T>
	static int test_get_share_count(const _ShareCom<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_share_count<_ShareCom<T>>(sp);
	}
	template <class T>
	static int test_get_weak_count(const _ShareCom<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_weak_count<_ShareCom<T>>(sp);
	}
	template <class T>
	static int test_get_share_count(const _WeakCom<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_share_count<_WeakCom<T>>(sp);
	}
	template <class T>
	static int test_get_weak_count(const _WeakCom<T>& sp) throw()
	{
		return _SObjSoloHelper::test_get_weak_count<_WeakCom<T>>(sp);
	}
};

#define CALL_COM_TYPECAST(src, src_type, ifname)  _ShareComHelper::Query<src_type, ifname>(src, USE_GUID(GUID_##ifname))

#define CALL_COM_ADVISE(sp, type, ifname, spe)  _ShareComHelper::Advise<type, ifname>(sp, USE_GUID(GUID_##ifname), spe)

#define CALL_COM_UNADVISE(sp, type, ifname, ck)  _ShareComHelper::Unadvise<type>(sp, USE_GUID(GUID_##ifname), ck)

// _ComConnectionImpl<TSObjArray>

template <class TSObjArray>
class NOVTABLE _ComConnectionImpl : public _SObjConnectionImpl<TSObjArray>,
									public _IComConnection
{
private:
	typedef _SObjConnectionImpl<TSObjArray>  baseClass;

public:
	_ComConnectionImpl() throw()
	{
	}
	~_ComConnectionImpl() throw()
	{
	}

// _IComConnection methods
	virtual uintptr Advise(const _WeakCom<void>& sp) throw()
	{
		return baseClass::Add(_ShareComHelper::TypeCast<void, typename TSObjArray::EObj::EType>(sp));
	}
	virtual void Unadvise(const uintptr& uCookie) throw()
	{
		baseClass::Remove(uCookie);
	}

private:
	//noncopyable
	_ComConnectionImpl(const _ComConnectionImpl&) throw();
	_ComConnectionImpl& operator=(const _ComConnectionImpl&) throw();
};

/*
_ShareCom:
User can define a proxy class named as Proxy_<event_interface_name> derived from _ComConnectionImpl<SObjArray<_WeakCom<event_interface_name>>> or _ComConnectionImpl<SObjArrayWithLock<_WeakCom<event_interface_name>>>.
This proxy class provides the fire functions with Fire_<event_method_name> format, and is used as the base class of main class.

The class used as method parameter type of interface should not be upgraded in a SA.
*/

// _ShareArrayBase<T>

template <typename T>
class _ShareArrayBase : public _SObjT<_ShareArrayBase<T>, _ShareObjectBase>
{
private:
#define _SHAREARRAYBASE_BASE_CLASS  _SObjT<_ShareArrayBase<T>, _ShareObjectBase>

	typedef _ShareArrayBase<T>  thisClass;

protected:
	_ShareArrayBase() throw()
	{
	}
	_ShareArrayBase(const _ShareArrayBase& src) throw() : _SHAREARRAYBASE_BASE_CLASS(static_cast<const _SHAREARRAYBASE_BASE_CLASS&>(src))
	{
	}
	_ShareArrayBase(_ShareArrayBase&& src) throw() : _SHAREARRAYBASE_BASE_CLASS(rv_forward(static_cast<_SHAREARRAYBASE_BASE_CLASS&>(src)))
	{
	}
	~_ShareArrayBase() throw()
	{
		Release();
	}

	_ShareArrayBase<T>& operator=(const _ShareArrayBase<T>& src) throw()
	{
		_SHAREARRAYBASE_BASE_CLASS::operator=(static_cast<const _SHAREARRAYBASE_BASE_CLASS&>(src));
		return *this;
	}
	_ShareArrayBase<T>& operator=(_ShareArrayBase<T>&& src) throw()
	{
		_SHAREARRAYBASE_BASE_CLASS::operator=(rv_forward(static_cast<_SHAREARRAYBASE_BASE_CLASS&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( _SHAREARRAYBASE_BASE_CLASS::m_pB != NULL ) {
			_SObjRelease<share_array_block>::ShareRelease<T>(_SHAREARRAYBASE_BASE_CLASS::m_pB);
			_SHAREARRAYBASE_BASE_CLASS::m_pB = NULL;
		} //end if
	}

protected:
	void do_assign(const _SHAREARRAYBASE_BASE_CLASS& src) throw()
	{
	}
	void do_move(_SHAREARRAYBASE_BASE_CLASS&& src) throw()
	{
	}

private:
	friend class _SHAREARRAYBASE_BASE_CLASS;
};

// _WeakArrayBase

class _WeakArrayBase : public _SObjT<_WeakArrayBase, _WeakObjectBase>
{
private:
#define _WEAKARRAYBASE_BASE_CLASS  _SObjT<_WeakArrayBase, _WeakObjectBase>

	typedef _WeakArrayBase  thisClass;

protected:
	_WeakArrayBase() throw()
	{
	}
	_WeakArrayBase(const _WeakArrayBase& src) throw() : _WEAKARRAYBASE_BASE_CLASS(static_cast<const _WEAKARRAYBASE_BASE_CLASS&>(src))
	{
	}
	_WeakArrayBase(_WeakArrayBase&& src) throw() : _WEAKARRAYBASE_BASE_CLASS(rv_forward(static_cast<_WEAKARRAYBASE_BASE_CLASS&>(src)))
	{
	}
	~_WeakArrayBase() throw()
	{
		Release();
	}

	_WeakArrayBase& operator=(const _WeakArrayBase& src) throw()
	{
		_WEAKARRAYBASE_BASE_CLASS::operator=(static_cast<const _WEAKARRAYBASE_BASE_CLASS&>(src));
		return *this;
	}
	_WeakArrayBase& operator=(_WeakArrayBase&& src) throw()
	{
		_WEAKARRAYBASE_BASE_CLASS::operator=(rv_forward(static_cast<_WEAKARRAYBASE_BASE_CLASS&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( _WEAKARRAYBASE_BASE_CLASS::m_pB != NULL ) {
			//weak
			_SObjRelease<share_array_block>::WeakRelease(_WEAKARRAYBASE_BASE_CLASS::m_pB);
			_WEAKARRAYBASE_BASE_CLASS::m_pB = NULL;
		} //end if
	}

protected:
	void do_assign(const _WEAKARRAYBASE_BASE_CLASS& src) throw()
	{
	}
	void do_move(_WEAKARRAYBASE_BASE_CLASS&& src) throw()
	{
	}

private:
	friend class _WEAKARRAYBASE_BASE_CLASS;
};

// _ShareArray<T>
/*! \brief A template class for shared array.

A template class for shared array.
\tparam T The element type.
*/
template <typename T>
class _ShareArray : public _ShareArrayBase<T>
{
private:
	typedef _ShareArrayBase<T>  baseClass;
	typedef _ShareArray<T>  thisClass;

public:
	typedef T  EType;
	typedef GKC::ArrayPosition  Position;
	typedef GKC::ArrayIterator<T>  Iterator;

public:
	/*! \brief Constructor.

	Constructor.
	*/
	_ShareArray() throw()
	{
	}
	/*! \brief Copy constructor.

	Copy constructor.
	*/
	_ShareArray(const _ShareArray& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	/*! \brief Right-value copy constructor.

	Right-value copy constructor.
	*/
	_ShareArray(_ShareArray&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	/*! \brief Destructor.

	Destructor.
	*/
	~_ShareArray() throw()
	{
	}

	/*! \brief Assignment operator.

	Assignment operator.
	*/
	_ShareArray<T>& operator=(const _ShareArray<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	/*! \brief Right-value assignment operator.

	Right-value assignment operator.
	*/
	_ShareArray<T>& operator=(_ShareArray<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _ShareArray<T>& right) const throw()
	{
		return baseClass::m_pB == right.m_pB;
	}
	bool operator!=(const _ShareArray<T>& right) const throw()
	{
		return !operator==(right);
	}

//methods

	uintptr GetCount() const throw()
	{
		return baseClass::IsBlockNull() ? 0 : (static_cast<share_array_block*>(baseClass::m_pB))->GetLength();
	}
	bool IsEmpty() const throw()
	{
		return GetCount() == 0;
	}

	const T& operator[](uintptr index) const throw()
	{
		return GetAt(index);
	}
	T& operator[](uintptr index) throw()
	{
		return GetAt(index);
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
	Position GetPosition(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return Position(index);
	}

	const Iterator ToIterator(const Position& pos) const throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Iterator(GKC::RefPtr<T>(get_array_address() + pos.GetIndex()));
	}
	Iterator ToIterator(const Position& pos) throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Iterator(GKC::RefPtr<T>(get_array_address() + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Position(GKC::RefPtrHelper::GetInternalPointer(iter.get_Ref()) - get_array_address());
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Iterator(GKC::RefPtr<T>(get_array_address()));
	}
	Iterator GetBegin() throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Iterator(GKC::RefPtr<T>(get_array_address()));
	}
	const Iterator GetEnd() const throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Iterator(GKC::RefPtr<T>(get_array_address() + GetCount()));
	}
	Iterator GetEnd() throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return Iterator(GKC::RefPtr<T>(get_array_address() + GetCount()));
	}
	const GKC::ReverseIterator<Iterator> GetReverseBegin() const throw()
	{
		return GKC::ReverseIterator<Iterator>(ToIterator(GetTailPosition()));
	}
	GKC::ReverseIterator<Iterator> GetReverseBegin() throw()
	{
		return GKC::ReverseIterator<Iterator>(ToIterator(GetTailPosition()));
	}
	const GKC::ReverseIterator<Iterator> GetReverseEnd() const throw()
	{
		return GKC::ReverseIterator<Iterator>(Iterator(GKC::RefPtr<T>(get_array_address() - 1)));
	}
	GKC::ReverseIterator<Iterator> GetReverseEnd() throw()
	{
		return GKC::ReverseIterator<Iterator>(Iterator(GKC::RefPtr<T>(get_array_address() - 1)));
	}
	const GKC::ReverseIterator2<Iterator> GetReverseBegin2() const throw()
	{
		return GKC::ReverseIterator2<Iterator>(GetEnd());
	}
	GKC::ReverseIterator2<Iterator> GetReverseBegin2() throw()
	{
		return GKC::ReverseIterator2<Iterator>(GetEnd());
	}
	const GKC::ReverseIterator2<Iterator> GetReverseEnd2() const throw()
	{
		return GKC::ReverseIterator2<Iterator>(GetBegin());
	}
	GKC::ReverseIterator2<Iterator> GetReverseEnd2() throw()
	{
		return GKC::ReverseIterator2<Iterator>(GetBegin());
	}

	const T& GetAt(uintptr index) const throw()
	{
		assert( index < GetCount() );
		assert( !(baseClass::IsBlockNull()) );
		return *(get_array_address() + index);
	}
	T& GetAt(uintptr index) throw()
	{
		assert( index < GetCount() );
		assert( !(baseClass::IsBlockNull()) );
		return *(get_array_address() + index);
	}
	void SetAt(uintptr index, const T& t)  //may throw
	{
		assert( index < GetCount() );
		assert( !(baseClass::IsBlockNull()) );
		get_array_address()[index] = t;
	}
	void SetAt(uintptr index, T&& t)  //may throw
	{
		assert( index < GetCount() );
		assert( !(baseClass::IsBlockNull()) );
		get_array_address()[index] = rv_forward(t);
	}

	//size
	//  uGrowBy: the default value is 0.
	template <typename... Args>
	void SetCount(uintptr uCount, uintptr uGrowBy, Args&&... args)  //may throw
	{
		assert( !(baseClass::IsBlockNull()) );  //must have a block for allocation
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		assert( pB->GetMemoryManager() != NULL );
		uintptr uOldSize = pB->GetLength();
		if( uCount == 0 ) {
			// shrink to nothing
			pB->DestroyArray<T>();
		}
		else if( uCount <= pB->GetAllocLength() ) {
			// it fits
			if( uCount > uOldSize ) {
				// initialize the new elements
				call_array_constructors(get_array_address() + uOldSize, uCount - uOldSize, rv_forward<Args>(args)...);
			}
			else if( uOldSize > uCount ) {
				// destroy the old elements
				call_array_destructors(get_array_address() + uCount, uOldSize - uCount);
			}
			pB->SetLength(uCount);
		}
		else {
			// grow
			grow_buffer(uCount, uGrowBy);  //may throw
			// construct new elements
			assert( uCount > pB->GetLength() );
			call_array_constructors(get_array_address() + uOldSize, uCount - uOldSize, rv_forward<Args>(args)...);
			pB->SetLength(uCount);
		} //end if
	}
	void SetCountD(uintptr uCount)
	{
		SetCount(uCount, 0);  //may throw
	}

	//clear
	void RemoveAll() throw()
	{
		assert( !(baseClass::IsBlockNull()) );  //must have a block for free
		(static_cast<share_array_block*>(baseClass::m_pB))->DestroyArray<T>();
	}

	void FreeExtra() throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		assert( pB->GetMemoryManager() != NULL );
		uintptr uSize = pB->GetLength();
		assert( uSize <= pB->GetAllocLength() );
		if( uSize == pB->GetAllocLength() )
			return ;
		// shrink to desired size
		if( uSize == 0 ) {
			//free
			pB->DestroyArray<T>();
			return ;
		}
		T* pNew = (T*)(pB->GetMemoryManager()->Reallocate((uintptr)get_array_address(), uSize * sizeof(T)));
		if( pNew == NULL )
			return ;
		set_array_address(pNew);
		pB->SetAllocLength(uSize);
	}

	//add
	template <typename... Args>
	Iterator Add(Args&&... args)  //may throw
	{
		assert( !(baseClass::IsBlockNull()) );
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		uintptr uElement = pB->GetLength();
		uintptr uNew = GKC::SafeOperators::AddThrow(uElement, (uintptr)1);
		SetCount(uNew, 0, rv_forward<Args>(args)...);
		return ToIterator(GetPosition(uElement));
	}
	void Append(const _ShareArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		assert( !(baseClass::IsBlockNull()) );
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		uintptr uOldSize = pB->GetLength();
		uintptr uSrcSize = src.GetCount();
		uintptr uNewSize = GKC::SafeOperators::AddThrow(uOldSize, uSrcSize);
		SetCount(uNewSize, 0);
		if( uSrcSize != 0 )
			copy_array_elements(src.get_array_address(), get_array_address() + uOldSize, uSrcSize);
	}

	void Copy(const _ShareArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		uintptr uSize = src.GetCount();
		SetCount(uSize, 0);
		if( uSize != 0 )
			copy_array_elements(src.get_array_address(), get_array_address(), uSize);
	}

	// count: the default value is 1.
	template <typename... Args>
	void InsertAt(uintptr index, uintptr count, Args&&... args)  //may throw
	{
		assert( count > 0 );  // zero size not allowed
		assert( !(baseClass::IsBlockNull()) );
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		uintptr uSize = pB->GetLength();
		if( index >= uSize ) {
			// adding after the end of the array
			uintptr uNewSize;
			if( GKC::SafeOperators::Add(index, count, uNewSize).IsFailed() )
				throw GKC::OverflowException();
			SetCount(uNewSize, 0, rv_forward<Args>(args)...);  //grow so index is valid
		}
		else {
			// inserting in the middle of the array
			uintptr uOldSize = uSize;
			uintptr uNewSize;
			if( GKC::SafeOperators::Add(uSize, count, uNewSize).IsFailed() )
				throw GKC::OverflowException();
			SetCount(uNewSize, 0);  //grow it to new size
			// destroy intial data before copying over it
			call_array_destructors(get_array_address() + uOldSize, count);
			// shift old data up to fill gap
			relocate_array_elements(get_array_address() + index, get_array_address() + (index + count), uOldSize - index);
			try {
				// re-init slots we copied from
				call_array_constructors(get_array_address() + index, count, rv_forward<Args>(args)...);
			}
			catch(...) {
				relocate_array_elements(get_array_address() + (index + count), get_array_address() + index, uOldSize - index);
				pB->SetLength(uOldSize);
				throw ;  //re-throw
			}
		} //end if
		assert( (index + count) <= pB->GetLength() );
	}
	void InsertArrayAt(uintptr index, const _ShareArray<T>& src)  //may throw
	{
		uintptr uSize = src.GetCount();
		if( uSize > 0 ) {
			InsertAt(index, uSize);
			try {
				for( uintptr i = 0; i < uSize; i ++ ) {
					SetAt(index + i, src[i]);
				}
			}
			catch(...) {
				RemoveAt(index, uSize);
				throw ;  //re-throw
			}
		} //end if
	}

	//remove
	void RemoveAt(uintptr index, uintptr count = 1) throw()
	{
		assert( count > 0 );  // zero size not allowed
		assert( !(baseClass::IsBlockNull()) );
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		uintptr uSize = pB->GetLength();
		uintptr uCut = index + count;
		assert( uCut >= index && uCut >= count && uCut <= uSize );  //no overflow
		// just remove a range
		uintptr uMoveCount = uSize - uCut;
		//destructor
		call_array_destructors(get_array_address() + index, count);
		if( uMoveCount > 0 ) {
			relocate_array_elements(get_array_address() + uCut, get_array_address() + index, uMoveCount);
		}
		//size
		pB->SetLength(uSize - count);
	}

protected:
	T* get_array_address() const throw()
	{
		return (T*)((static_cast<share_array_block*>(baseClass::m_pB))->GetAddress());
	}
	void set_array_address(T* p) throw()
	{
		(static_cast<share_array_block*>(baseClass::m_pB))->SetAddress(p);
	}

private:
	//grow
	void grow_buffer(uintptr uNewSize, uintptr uGrowBy)  //may throw
	{
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		if( uNewSize <= pB->GetAllocLength() )
			return ;
		uintptr uAllocSize;
		if( get_array_address() == NULL ) {
			uAllocSize = (uGrowBy > uNewSize) ? uGrowBy : uNewSize;
			uintptr uBytes = 0;
			//overflow
			if( GKC::SafeOperators::Multiply(uAllocSize, (uintptr)(sizeof(T)), uBytes).IsFailed() )
				throw GKC::OverflowException();
			//allocate
			T* pNew = (T*)(pB->GetMemoryManager()->Allocate(uBytes));
			if( pNew == NULL )
				throw GKC::OutOfMemoryException();
			set_array_address(pNew);
		}
		else {
			//grow
			if( uGrowBy == 0 ) {
				//default situation
				uGrowBy = pB->GetLength() / 8;
				uGrowBy = (uGrowBy < 4) ? 4 : ((uGrowBy > 1024) ? 1024 : uGrowBy);
			}
			//overflow
			if( GKC::SafeOperators::Add(pB->GetAllocLength(), uGrowBy, uAllocSize).IsFailed() )
				throw GKC::OverflowException();
			if( uNewSize > uAllocSize )
				uAllocSize = uNewSize;  //no extra
			uintptr uBytes = 0;
			//overflow
			if( GKC::SafeOperators::Multiply(uAllocSize, (uintptr)(sizeof(T)), uBytes).IsFailed() )
				throw GKC::OverflowException();
			//reallocate
			//  because uBytes != 0, m_p is not freed
			T* pNew = (T*)(pB->GetMemoryManager()->Reallocate((uintptr)get_array_address(), uBytes));
			if( pNew == NULL )
				throw GKC::OutOfMemoryException();
			set_array_address(pNew);
		} //end if
		pB->SetAllocLength(uAllocSize);
	}

private:
	friend class _ShareArrayHelper;
};

// _WeakArray<T>

template <typename T>
class _WeakArray : public _WeakArrayBase
{
private:
	typedef _WeakArrayBase  baseClass;
	typedef _WeakArray<T>  thisClass;

public:
	typedef T  EType;

public:
	_WeakArray() throw()
	{
	}
	_WeakArray(const _WeakArray& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_WeakArray(_WeakArray&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_WeakArray() throw()
	{
	}

	_WeakArray<T>& operator=(const _WeakArray<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_WeakArray<T>& operator=(_WeakArray<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

	bool operator==(const _WeakArray<T>& right) const throw()
	{
		return m_pB == right.m_pB;
	}
	bool operator!=(const _WeakArray<T>& right) const throw()
	{
		return !operator==(right);
	}

private:
	friend class _ShareArrayHelper;
};

// _ShareArrayHelper

class _ShareArrayHelper
{
public:
	//make share array
	template <typename T>
	static _ShareArray<T> MakeShareArray(const GKC::RefPtr<GKC::IMemoryManager>& mgr)
	{
		assert( !mgr.IsNull() );

		//allocate
		share_array_block* pB = ::_SabPool_Allocate();
		if( pB == NULL )
			throw GKC::OutOfMemoryException();

		//initialize
		pB->SetMemoryManager(GKC::RefPtrHelper::GetInternalPointer(mgr));

		_ShareArray<T> ret;
		//return value
		ret.m_pB = static_cast<share_block_base*>(pB);

		return ret;
	}

	//obtain weak array
	template <typename T>
	static _WeakArray<T> ToWeakArray(const _ShareArray<T>& sp) throw()
	{
		_WeakArray<T> ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			_SObjHelper::AddWeakRef(ret.m_pB);
		}
		return ret;
	}
	//To share array
	template <typename T>
	static _ShareArray<T> ToShareArray(const _WeakArray<T>& sp) throw()
	{
		_ShareArray<T> ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			if( !((ret.m_pB)->AddRefLock()) ) {
				//share array freed
				ret.m_pB = NULL;
			}
		}
		return ret;
	}

	//clone
	template <typename T>
	static _ShareArray<T> Clone(const _ShareArray<T>& sp)  //may throw
	{
		_ShareArray<T> ret;
		if( !sp.IsBlockNull() ) {
			ret = MakeShareArray<T>(GKC::RefPtr<GKC::IMemoryManager>((sp.m_pB)->GetMemoryManager()));
			ret.Copy(sp);
		}
		return ret;
	}

	//internal pointer
	/*! \brief Get the beginning pointer of array.

	Get the beginning pointer of array.
	\param sp [in] Specify the array object.
	\return A pointer to the beginning of array.
	*/
	template <typename T>
	static T* GetInternalPointer(const _ShareArray<T>& sp) throw()
	{
		return (sp.m_pB == NULL) ? NULL : sp.get_array_address();
	}
	template <typename T>
	static T* GetInternalPointer(const _WeakArray<T>& sp) throw()
	{
		return (sp.m_pB == NULL) ? NULL : (T*)((sp.m_pB)->GetAddress());
	}

	//to const array
	template <typename T>
	static GKC::ConstArray<T> To_ConstArray(const _ShareArray<T>& sp) noexcept
	{
		return GKC::ConstArray<T>(GetInternalPointer(sp), sp.GetCount());
	}

	//only for test
	template <typename T>
	static int test_get_share_count(const _ShareArray<T>& sp) throw()
	{
		return (sp.m_pB == NULL) ? 0 : (sp.m_pB)->GetShareCount();
	}
	template <typename T>
	static int test_get_weak_count(const _ShareArray<T>& sp) throw()
	{
		return (sp.m_pB == NULL) ? 0 : (sp.m_pB)->GetWeakCount();
	}
	template <typename T>
	static int test_get_share_count(const _WeakArray<T>& sp) throw()
	{
		return (sp.m_pB == NULL) ? 0 : (sp.m_pB)->GetShareCount();
	}
	template <typename T>
	static int test_get_weak_count(const _WeakArray<T>& sp) throw()
	{
		return (sp.m_pB == NULL) ? 0 : (sp.m_pB)->GetWeakCount();
	}
};

// _StringT<Tchar>
//   Tchar: CharA CharH CharL, CharS CharW

template <typename Tchar>
class _StringT : public _ShareArray<Tchar>
{
private:
	typedef _ShareArray<Tchar>  baseClass;
	typedef _StringT<Tchar>  thisClass;

public:
	template <typename T2>
	using StringType = _StringT<T2>;

public:
	_StringT() throw()
	{
	}
	_StringT(const thisClass& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_StringT(thisClass&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_StringT() throw()
	{
	}

	//operators
	thisClass& operator=(const thisClass& src) throw()
	{
		return static_cast<thisClass&>(baseClass::operator=(static_cast<const baseClass&>(src)));
	}
	thisClass& operator=(thisClass&& src) throw()
	{
		return static_cast<thisClass&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

	uintptr GetLength() const throw()
	{
		uintptr uCount;
		return (baseClass::IsBlockNull()) ? 0 :
				( (uCount = (static_cast<share_array_block*>(baseClass::m_pB))->GetLength(), uCount == 0) ? 0 : uCount - 1 )
				;
	}
	bool IsEmpty() const throw()
	{
		return GetLength() == 0;
	}

	//position
	typename thisClass::Position GetTailPosition() const throw()
	{
		return typename thisClass::Position(GetLength() - 1);
	}

	//iterators
	const typename thisClass::Iterator GetEnd() const throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	typename thisClass::Iterator GetEnd() throw()
	{
		assert( !(baseClass::IsBlockNull()) );
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	const GKC::ReverseIterator<typename thisClass::Iterator> GetReverseBegin() const throw()
	{
		return GKC::ReverseIterator<typename thisClass::Iterator>(ToIterator(GetTailPosition()));
	}
	GKC::ReverseIterator<typename thisClass::Iterator> GetReverseBegin() throw()
	{
		return GKC::ReverseIterator<typename thisClass::Iterator>(ToIterator(GetTailPosition()));
	}
	const GKC::ReverseIterator2<typename thisClass::Iterator> GetReverseBegin2() const throw()
	{
		return GKC::ReverseIterator2<typename thisClass::Iterator>(GetEnd());
	}
	GKC::ReverseIterator2<typename thisClass::Iterator> GetReverseBegin2() throw()
	{
		return GKC::ReverseIterator2<typename thisClass::Iterator>(GetEnd());
	}

	const Tchar& GetAt(uintptr index) const throw()
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsBlockNull()) );
		return *(baseClass::get_array_address() + index);
	}
	Tchar& GetAt(uintptr index) throw()
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsBlockNull()) );
		return *(baseClass::get_array_address() + index);
	}
	void SetAt(uintptr index, const Tchar& t)  //may throw
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsBlockNull()) );
		baseClass::get_array_address()[index] = t;
	}
	void SetAt(uintptr index, Tchar&& t)  //may throw
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsBlockNull()) );
		baseClass::get_array_address()[index] = rv_forward(t);
	}

	//methods
	void SetLength(uintptr uLength)
	{
		uintptr uSize = GKC::SafeOperators::AddThrow(uLength, (uintptr)1);
		baseClass::SetCount(uSize, 0);
		baseClass::get_array_address()[uLength] = 0;
	}
	void RecalcLength() throw()
	{
		assert( !(baseClass::IsBlockNull()) );  //must have a block for allocation
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		pB->SetLength(calc_string_length(baseClass::get_array_address()) + 1);
	}

	//clear content and free array
	void Clear() throw()
	{
		baseClass::RemoveAll();
	}

private:
	friend class _StringHelper;
};

// _String*
typedef _StringT<GKC::CharA>  _StringA;
typedef _StringT<GKC::CharH>  _StringH;
typedef _StringT<GKC::CharL>  _StringL;
typedef _StringT<GKC::CharS>  _StringS;
typedef _StringT<GKC::CharW>  _StringW;

// _StringCompareTrait<T>

template <class T>
class _StringCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2));
	}
};

// _StringCaseIgnoreCompareTrait<T>

template <class T>
class _StringCaseIgnoreCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_ShareArrayHelper::GetInternalPointer(t1), _ShareArrayHelper::GetInternalPointer(t2));
	}
};

// _StringHashTrait<T>

template <class T>
class _StringHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = _ShareArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)(*pch);
			pch ++;
		}
		return uHash;
	}
};

// _StringCaseIgnoreHashTrait<T>

template <class T>
class _StringCaseIgnoreHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = _ShareArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

// _StringHelper

class _StringHelper
{
public:
	//To C-style string
	template <typename Tchar>
	static GKC::RefPtr<Tchar> To_C_Style(const _StringT<Tchar>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetLength() );
		return GKC::RefPtr<Tchar>(_ShareArrayHelper::GetInternalPointer(str) + uStart);
	}

	//make empty string
	template <typename Tchar>
	static _StringT<Tchar> MakeEmptyString(const GKC::RefPtr<GKC::IMemoryManager>& mgr)
	{
		_StringT<Tchar> ret;
		static_cast<_ShareArray<Tchar>&>(ret) = _ShareArrayHelper::MakeShareArray<Tchar>(mgr);
		return ret;
	}

	//clone
	template <typename Tchar>
	static _StringT<Tchar> Clone(const _StringT<Tchar>& str)
	{
		_StringT<Tchar> ret;
		if( !str.IsBlockNull() ) {
			ret = MakeEmptyString<Tchar>(GKC::RefPtr<GKC::IMemoryManager>((static_cast<share_array_block*>(str.m_pB))->GetMemoryManager()));
			uintptr uCount = str.GetLength();
			ret.SetLength(uCount);
			if( uCount != 0 )
				mem_copy(_ShareArrayHelper::GetInternalPointer(str), uCount * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(ret));
		}
		return ret;
	}

	//find (return value : check null)
	template <typename Tchar>
	static typename _StringT<Tchar>::Iterator Find(const _StringT<Tchar>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsBlockNull() );
		return typename _StringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_char(_ShareArrayHelper::GetInternalPointer(str) + uStart, ch)));
	}
	//find last (return value : check null)
	template <typename Tchar>
	static typename _StringT<Tchar>::Iterator FindLast(const _StringT<Tchar>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsBlockNull() );
		return typename _StringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_last_char(_ShareArrayHelper::GetInternalPointer(str) + uStart, ch)));
	}
};

#pragma pack(pop)

//------------------------------------------------------------------------------
// Unique

#pragma pack(push, 1)

// _UniqueArray<T>

template <typename T>
class _UniqueArray
{
public:
	typedef T  EType;
	typedef GKC::ArrayPosition  Position;
	typedef GKC::ArrayIterator<T>  Iterator;

public:
	_UniqueArray() noexcept : m_p(NULL)
	{
	}
	_UniqueArray(const _UniqueArray&) = delete;
	_UniqueArray& operator=(const _UniqueArray&) = delete;
	_UniqueArray(_UniqueArray&& src) noexcept : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	~_UniqueArray() noexcept
	{
		RemoveAll();
	}

	_UniqueArray& operator=(_UniqueArray&& src) noexcept
	{
		if ( this != &src ) {
			if ( m_p != src.m_p ) {
				RemoveAll();
				m_p = src.m_p;
				src.m_p = NULL;
			}
			else {
				assert( m_p == NULL );  // unique
			}
		}
		return *this;
	}

	bool operator==(const _UniqueArray<T>& right) const noexcept
	{
		return m_p == right.m_p;
	}
	bool operator!=(const _UniqueArray<T>& right) const noexcept
	{
		return m_p != right.m_p;
	}

//methods
	void RemoveAll() noexcept
	{
		if ( m_p != NULL ) {
			unique_array_block* pB = (unique_array_block*)m_p;
			call_array_destructors(get_array_address(), pB->GetLength());
			pB->GetMemoryManager()->Free((uintptr)m_p);
			m_p = NULL;
		}
	}

	uintptr GetCount() const noexcept
	{
		return m_p == NULL ? 0 : ((unique_array_block*)m_p)->GetLength();
	}
	bool IsEmpty() const noexcept
	{
		return GetCount() == 0;
	}
	bool IsNull() const noexcept
	{
		return m_p == NULL;
	}

	const T& operator[](uintptr index) const noexcept
	{
		return GetAt(index);
	}
	T& operator[](uintptr index) noexcept
	{
		return GetAt(index);
	}

	//position
	Position GetStartPosition() const noexcept
	{
		return Position(0);
	}
	Position GetTailPosition() const noexcept
	{
		return Position(GetCount() - 1);
	}
	Position GetPosition(uintptr index) const noexcept
	{
		assert( index < GetCount() );
		return Position(index);
	}

	const Iterator ToIterator(const Position& pos) const noexcept
	{
		assert( m_p != NULL );
		return Iterator(GKC::RefPtr<T>(get_array_address() + pos.GetIndex()));
	}
	Iterator ToIterator(const Position& pos) noexcept
	{
		assert( m_p != NULL );
		return Iterator(GKC::RefPtr<T>(get_array_address() + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const noexcept
	{
		assert( m_p != NULL );
		return Position(GKC::RefPtrHelper::GetInternalPointer(iter.get_Ref()) - get_array_address());
	}

	//iterator
	/*! \brief Get the beginning iterator.

	Get the beginning iterator.
	\return Return the beginning iterator.
	*/
	const Iterator GetBegin() const noexcept
	{
		assert( m_p != NULL );
		return Iterator(GKC::RefPtr<T>(get_array_address()));
	}
	Iterator GetBegin() noexcept
	{
		assert( m_p != NULL );
		return Iterator(GKC::RefPtr<T>(get_array_address()));
	}
	/*! \brief Get the end iterator.

	Get the end iterator.
	\return Return the end iterator.
	*/
	const Iterator GetEnd() const noexcept
	{
		assert( m_p != NULL );
		return Iterator(GKC::RefPtr<T>(get_array_address() + GetCount()));
	}
	Iterator GetEnd() noexcept
	{
		assert( m_p != NULL );
		return Iterator(GKC::RefPtr<T>(get_array_address() + GetCount()));
	}
	const GKC::ReverseIterator<Iterator> GetReverseBegin() const noexcept
	{
		return GKC::ReverseIterator<Iterator>(ToIterator(GetTailPosition()));
	}
	GKC::ReverseIterator<Iterator> GetReverseBegin() noexcept
	{
		return GKC::ReverseIterator<Iterator>(ToIterator(GetTailPosition()));
	}
	const GKC::ReverseIterator<Iterator> GetReverseEnd() const noexcept
	{
		return GKC::ReverseIterator<Iterator>(Iterator(GKC::RefPtr<T>(get_array_address() - 1)));
	}
	GKC::ReverseIterator<Iterator> GetReverseEnd() noexcept
	{
		return GKC::ReverseIterator<Iterator>(Iterator(GKC::RefPtr<T>(get_array_address() - 1)));
	}
	const GKC::ReverseIterator2<Iterator> GetReverseBegin2() const noexcept
	{
		return GKC::ReverseIterator2<Iterator>(GetEnd());
	}
	GKC::ReverseIterator2<Iterator> GetReverseBegin2() noexcept
	{
		return GKC::ReverseIterator2<Iterator>(GetEnd());
	}
	const GKC::ReverseIterator2<Iterator> GetReverseEnd2() const noexcept
	{
		return GKC::ReverseIterator2<Iterator>(GetBegin());
	}
	GKC::ReverseIterator2<Iterator> GetReverseEnd2() noexcept
	{
		return GKC::ReverseIterator2<Iterator>(GetBegin());
	}

	const T& GetAt(uintptr index) const noexcept
	{
		assert( index < GetCount() );
		assert( m_p != NULL );
		return *(get_array_address() + index);
	}
	T& GetAt(uintptr index) noexcept
	{
		assert( index < GetCount() );
		assert( m_p != NULL );
		return *(get_array_address() + index);
	}
	void SetAt(uintptr index, const T& t)
	{
		assert( index < GetCount() );
		assert( m_p != NULL );
		get_array_address()[index] = t;  //may throw
	}
	void SetAt(uintptr index, T&& t)
	{
		assert( index < GetCount() );
		assert( m_p != NULL );
		get_array_address()[index] = rv_forward(t);  //may throw
	}

	template <typename... Args>
	void SetCount(uintptr uCount, Args&&... args)
	{
		//clear
		if ( uCount == 0 ) {
			//shrink to nothing
			RemoveAll();
			return ;
		}
		uintptr uOldSize = GetCount();
		uintptr uAllocLength = (m_p == NULL) ? 0 : ((unique_array_block*)m_p)->GetAllocLength();
		//enough
		if ( uCount <= uAllocLength ) {
			// it fits
			if ( uCount > uOldSize ) {
				// initialize the new elements
				call_array_constructors(get_array_address() + uOldSize, uCount - uOldSize, rv_forward<Args>(args)...);  //may throw
			}
			else if ( uOldSize > uCount ) {
				// destroy the old elements
				call_array_destructors(get_array_address() + uCount, uOldSize - uCount);
			}
			((unique_array_block*)m_p)->SetLength(uCount);
			return ;
		}
		//new size, this avoids heap fragmentation in many situations
		uintptr uGrowBy = uOldSize / 8;
		uGrowBy = (uGrowBy < 4) ? 4 : ((uGrowBy > 1024) ? 1024 : uGrowBy);
		// granularity
		uAllocLength = GKC::SafeOperators::AddThrow(uAllocLength, uGrowBy);  //may throw
		if ( uCount > uAllocLength )
			uAllocLength = uCount;  // no slush
		uintptr uBytes = GKC::SafeOperators::MultiplyThrow(uAllocLength, (uintptr)sizeof(T));  //may throw
		uBytes = GKC::SafeOperators::AddThrow(uBytes, (uintptr)sizeof(unique_array_block));  //may throw
		//grow
		if ( m_p == NULL ) {
			GKC::IMemoryManager* pMgr = _CrtMemoryManager_Get();
			m_p = (void*)(pMgr->Allocate(uBytes));
			if ( m_p == NULL )
				throw GKC::OutOfMemoryException();
			((unique_array_block*)m_p)->SetMemoryManager(pMgr);
			((unique_array_block*)m_p)->SetLength(0);
		}
		else {
			GKC::IMemoryManager* pMgr = ((unique_array_block*)m_p)->GetMemoryManager();
			void* pNew = (void*)(pMgr->Reallocate((uintptr)m_p, uBytes));
			if ( pNew == NULL )
				throw GKC::OutOfMemoryException();
			m_p = pNew;
		}
		((unique_array_block*)m_p)->SetAllocLength(uAllocLength);
		//construct new elements
		call_array_constructors(get_array_address() + uOldSize, uCount - uOldSize, rv_forward<Args>(args)...);  //may throw
		((unique_array_block*)m_p)->SetLength(uCount);
	}
	void SetCountD(uintptr uCount)
	{
		SetCount(uCount);  //may throw
	}

	void FreeExtra() noexcept
	{
		if ( m_p == NULL )
			return ;
		unique_array_block* pB = (unique_array_block*)m_p;
		uintptr uSize = pB->GetLength();
		assert( uSize <= pB->GetAllocLength() );
		if ( uSize == pB->GetAllocLength() )
			return ;
		// shrink to desired size
		if ( uSize == 0 ) {
			//free
			RemoveAll();
			return ;
		}
		void* pNew = (void*)(pB->GetMemoryManager()->Reallocate((uintptr)m_p, uSize * sizeof(T) + sizeof(unique_array_block)));
		if ( pNew == NULL )
			return ;
		m_p = pNew;
		((unique_array_block*)m_p)->SetAllocLength(uSize);
	}

	//add
	template <typename... Args>
	Iterator Add(Args&&... args)
	{
		uintptr uElement = GetCount();
		uintptr uNew = GKC::SafeOperators::AddThrow(uElement, (uintptr)1);  //may throw
		SetCount(uNew, rv_forward<Args>(args)...);  //may throw
		return ToIterator(GetPosition(uElement));
	}
	void Append(const _UniqueArray<T>& src)
	{
		assert( this != &src );  //cannot append to itself
		uintptr uOldSize = GetCount();
		uintptr uSrcSize = src.GetCount();
		uintptr uNewSize = GKC::SafeOperators::AddThrow(uOldSize, uSrcSize);  //may throw
		SetCount(uNewSize);  //may throw
		if ( uSrcSize != 0 )
			copy_array_elements(src.get_array_address(), get_array_address() + uOldSize, uSrcSize);  //may throw
	}

	void Copy(const _UniqueArray<T>& src)
	{
		assert( this != &src );  //cannot append to itself
		uintptr uSize = src.GetCount();
		SetCount(uSize);  //may throw
		if( uSize != 0 )
			copy_array_elements(src.get_array_address(), get_array_address(), uSize);  //may throw
	}

	template <typename... Args>
	void InsertAt(uintptr index, uintptr count, Args&&... args)
	{
		assert( count > 0 );  // zero size not allowed
		uintptr uSize = GetCount();
		if ( index >= uSize ) {
			// adding after the end of the array
			uintptr uNewSize = GKC::SafeOperators::AddThrow(index, count);  //may throw
			SetCount(uNewSize, rv_forward<Args>(args)...);  //may throw
		}
		else {
			// inserting in the middle of the array
			uintptr uOldSize = uSize;
			uintptr uNewSize = GKC::SafeOperators::AddThrow(uSize, count);  //may throw
			SetCount(uNewSize);  //may throw
			// destroy intial data before copying over it
			call_array_destructors(get_array_address() + uOldSize, count);
			// shift old data up to fill gap
			relocate_array_elements(get_array_address() + index, get_array_address() + (index + count), uOldSize - index);
			try {
				// re-init slots we copied from
				call_array_constructors(get_array_address() + index, count, rv_forward<Args>(args)...);  //may throw
			}
			catch(...) {
				relocate_array_elements(get_array_address() + (index + count), get_array_address() + index, uOldSize - index);
				((unique_array_block*)m_p)->SetLength(uOldSize);
				throw ;  //re-throw
			}
		} //end if
		assert( (index + count) <= GetCount() );
	}
	void InsertArrayAt(uintptr index, const _UniqueArray<T>& src)
	{
		uintptr uSize = src.GetCount();
		if ( uSize > 0 ) {
			InsertAt(index, uSize);  //may throw
			try {
				for ( uintptr i = 0; i < uSize; i ++ ) {
					SetAt(index + i, src[i]);  //may throw
				}
			}
			catch(...) {
				RemoveAt(index, uSize);
				throw ;  //re-throw
			}
		} //end if
	}

	//remove
	void RemoveAt(uintptr index, uintptr count = 1) noexcept
	{
		assert( count > 0 );  // zero size not allowed
		uintptr uSize = GetCount();
		uintptr uCut = index + count;
		assert( uCut >= index && uCut >= count && uCut <= uSize );  //no overflow
		// just remove a range
		uintptr uMoveCount = uSize - uCut;
		//destructor
		call_array_destructors(get_array_address() + index, count);
		if ( uMoveCount > 0 ) {
			relocate_array_elements(get_array_address() + uCut, get_array_address() + index, uMoveCount);
		}
		//size
		((unique_array_block*)m_p)->SetLength(uSize - count);
	}

protected:
	T* get_array_address() const noexcept
	{
		return (T*)(void*)((unique_array_block*)m_p + 1);
	}

protected:
	void* m_p;

private:
	friend class _UniqueArrayHelper;
};

#pragma pack(pop)

// _UniqueArrayHelper

class _UniqueArrayHelper
{
public:
	//clone
	template <typename T>
	static _UniqueArray<T> Clone(const _UniqueArray<T>& sp)
	{
		_UniqueArray<T> ret;
		if ( sp.m_p != NULL )
			ret.Copy(sp);  //may throw
		return ret;
	}
	//internal pointer
	template <typename T>
	static T* GetInternalPointer(const _UniqueArray<T>& sp) noexcept
	{
		return (sp.m_p == NULL) ? NULL : sp.get_array_address();
	}

	//to const array
	template <typename T>
	static GKC::ConstArray<T> To_ConstArray(const _UniqueArray<T>& sp) noexcept
	{
		return GKC::ConstArray<T>(GetInternalPointer(sp), sp.GetCount());
	}

	template <typename T>
	static const_prefix_array<T> to_const_prefix_array(const _UniqueArray<T>& sp) noexcept
	{
		return const_prefix_array<T>(GetInternalPointer(sp));
	}
};

#pragma pack(push, 1)

// _UniqueStringT<Tchar>
//   Tchar: CharA CharH CharL, CharS CharW

template <typename Tchar>
class _UniqueStringT : public _UniqueArray<Tchar>
{
private:
	typedef _UniqueArray<Tchar>  baseClass;
	typedef _UniqueStringT<Tchar>  thisClass;

public:
	template <typename T2>
	using StringType = _UniqueStringT<T2>;

public:
	_UniqueStringT() noexcept
	{
	}
	_UniqueStringT(const _UniqueStringT&) = delete;
	_UniqueStringT& operator=(const _UniqueStringT&) = delete;
	_UniqueStringT(thisClass&& src) noexcept : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_UniqueStringT() noexcept
	{
	}

	thisClass& operator=(thisClass&& src) noexcept
	{
		return static_cast<thisClass&>(baseClass::operator=(rv_forward(static_cast<baseClass&>(src))));
	}

//methods
	uintptr GetLength() const noexcept
	{
		uintptr uCount;
		return (baseClass::IsNull()) ? 0 :
				( (uCount = (static_cast<unique_array_block*>(baseClass::m_p))->GetLength(), uCount == 0) ? 0 : uCount - 1 )
				;
	}
	bool IsEmpty() const noexcept
	{
		return GetLength() == 0;
	}

	//position
	typename thisClass::Position GetTailPosition() const noexcept
	{
		return typename thisClass::Position(GetLength() - 1);
	}

	//iterators
	const typename thisClass::Iterator GetEnd() const noexcept
	{
		assert( !(baseClass::IsNull()) );
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	typename thisClass::Iterator GetEnd() noexcept
	{
		assert( !(baseClass::IsNull()) );
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	const GKC::ReverseIterator<typename thisClass::Iterator> GetReverseBegin() const noexcept
	{
		return GKC::ReverseIterator<typename thisClass::Iterator>(ToIterator(GetTailPosition()));
	}
	GKC::ReverseIterator<typename thisClass::Iterator> GetReverseBegin() noexcept
	{
		return GKC::ReverseIterator<typename thisClass::Iterator>(ToIterator(GetTailPosition()));
	}
	const GKC::ReverseIterator2<typename thisClass::Iterator> GetReverseBegin2() const noexcept
	{
		return GKC::ReverseIterator2<typename thisClass::Iterator>(GetEnd());
	}
	GKC::ReverseIterator2<typename thisClass::Iterator> GetReverseBegin2() noexcept
	{
		return GKC::ReverseIterator2<typename thisClass::Iterator>(GetEnd());
	}

	const Tchar& GetAt(uintptr index) const noexcept
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsNull()) );
		return *(baseClass::get_array_address() + index);
	}
	Tchar& GetAt(uintptr index) noexcept
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsNull()) );
		return *(baseClass::get_array_address() + index);
	}
	void SetAt(uintptr index, const Tchar& t)
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsNull()) );
		baseClass::get_array_address()[index] = t;  //may throw
	}
	void SetAt(uintptr index, Tchar&& t)
	{
		assert( index < GetLength() );
		assert( !(baseClass::IsNull()) );
		baseClass::get_array_address()[index] = rv_forward(t);  //may throw
	}

	void SetLength(uintptr uLength)
	{
		uintptr uSize = GKC::SafeOperators::AddThrow(uLength, (uintptr)1);  //may throw
		baseClass::SetCount(uSize);  //may throw
		baseClass::get_array_address()[uLength] = 0;
	}
	void RecalcLength() noexcept
	{
		assert( !(baseClass::IsNull()) );
		((unique_array_block*)(baseClass::m_p))->SetLength(calc_string_length(baseClass::get_array_address()) + 1);
	}

	//clear content and free array
	void Clear() noexcept
	{
		baseClass::RemoveAll();
	}

private:
	friend class _UniqueStringHelper;
};

#pragma pack(pop)

// _UniqueString*
typedef _UniqueStringT<GKC::CharA>  _UniqueStringA;
typedef _UniqueStringT<GKC::CharH>  _UniqueStringH;
typedef _UniqueStringT<GKC::CharL>  _UniqueStringL;
typedef _UniqueStringT<GKC::CharS>  _UniqueStringS;
typedef _UniqueStringT<GKC::CharW>  _UniqueStringW;

// _UniqueStringCompareTrait<T>

template <class T>
class _UniqueStringCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2));
	}
};

// _UniqueStringCaseIgnoreCompareTrait<T>

template <class T>
class _UniqueStringCaseIgnoreCompareTrait
{
public:
	typedef T  EType;

public:
	static bool IsEQ(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) == 0;
	}
	static bool IsNE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) != 0;
	}
	static bool IsGT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) > 0;
	}
	static bool IsLT(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) < 0;
	}
	static bool IsGE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) >= 0;
	}
	static bool IsLE(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2)) <= 0;
	}
	static int Compare(const T& t1, const T& t2) throw()
	{
		return compare_string_case_insensitive(_UniqueArrayHelper::GetInternalPointer(t1), _UniqueArrayHelper::GetInternalPointer(t2));
	}
};

// _UniqueStringHashTrait<T>

template <class T>
class _UniqueStringHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = _UniqueArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)(*pch);
			pch ++;
		}
		return uHash;
	}
};

// _UniqueStringCaseIgnoreHashTrait<T>

template <class T>
class _UniqueStringCaseIgnoreHashTrait
{
public:
	static uintptr CalcHash(const T& t) throw()
	{
		uintptr uHash = 0;
		const typename T::EType* pch = _UniqueArrayHelper::GetInternalPointer(t);
		assert( pch != NULL );
		while( *pch != 0 ) {
			uHash = (uHash << 5) + uHash + (uintptr)char_upper(*pch);
			pch ++;
		}
		return uHash;
	}
};

// _UniqueStringHelper

class _UniqueStringHelper
{
public:
	//To C-style string
	template <typename Tchar>
	static GKC::RefPtr<Tchar> To_C_Style(const _UniqueStringT<Tchar>& str, uintptr uStart = 0) noexcept
	{
		assert( uStart <= str.GetLength() );
		return GKC::RefPtr<Tchar>(_UniqueArrayHelper::GetInternalPointer(str) + uStart);
	}

	//clone
	template <typename Tchar>
	static _UniqueStringT<Tchar> Clone(const _UniqueStringT<Tchar>& str)
	{
		_UniqueStringT<Tchar> ret;
		if ( !str.IsNull() ) {
			uintptr uCount = str.GetLength();
			ret.SetLength(uCount);  //may throw
			if ( uCount != 0 )
				mem_copy(_UniqueArrayHelper::GetInternalPointer(str), uCount * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(ret));
		}
		return ret;
	}

	//find (return value : check null)
	template <typename Tchar>
	static typename _UniqueStringT<Tchar>::Iterator Find(const _UniqueStringT<Tchar>& str, const Tchar& ch, uintptr uStart) noexcept
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsNull() );
		return typename _UniqueStringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_char(_UniqueArrayHelper::GetInternalPointer(str) + uStart, ch)));
	}
	//find last (return value : check null)
	template <typename Tchar>
	static typename _UniqueStringT<Tchar>::Iterator FindLast(const _UniqueStringT<Tchar>& str, const Tchar& ch, uintptr uStart) noexcept
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsNull() );
		return typename _UniqueStringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_last_char(_UniqueArrayHelper::GetInternalPointer(str) + uStart, ch)));
	}

	//to const_prefix_string
	template <typename Tchar>
	static const_prefix_string<Tchar> to_const_prefix_string(const _UniqueStringT<Tchar>& str) noexcept
	{
		return const_prefix_string<Tchar>(_UniqueArrayHelper::GetInternalPointer(str));
	}
};

//------------------------------------------------------------------------------
// String tools

// _StringOpHelper

class _StringOpHelper
{
public:
	template <typename Tchar>
	static void Initialize(_StringT<Tchar>& str)
	{
		if ( str.IsBlockNull() )
			str = _StringHelper::MakeEmptyString<Tchar>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get()));  //may throw
	}
	template <typename Tchar>
	static void Initialize(_UniqueStringT<Tchar>& str) throw()
	{
	}
	template <typename Tchar>
	static void Uninitialize(_StringT<Tchar>& str) throw()
	{
		str.Release();
	}
	template <typename Tchar>
	static void Uninitialize(_UniqueStringT<Tchar>& str) throw()
	{
		str.Clear();
	}

	//Tstring : _StringT<Tchar> or _UniqueStringT<Tchar>

	//append character
	template <class Tstring>
	static void Append(const typename Tstring::EType& ch, INOUT Tstring& strDest)
	{
		uintptr uCount = strDest.GetLength();
		uintptr uNew = GKC::SafeOperators::AddThrow(uCount, (uintptr)1);  //may throw
		strDest.SetLength(uNew);  //may throw
		strDest.SetAt(uCount, ch);
	}
	//insert character
	template <class Tstring>
	static void Insert(uintptr uPos, const typename Tstring::EType& ch, Tstring& str)
	{
		uintptr uLength = str.GetLength();
		if( uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, 1, ch);
	}
	//delete
	template <class Tstring>
	static void Delete(uintptr uPos, uintptr uLength, Tstring& str) throw()
	{
		uintptr uCount = str.GetLength();
		uintptr uRet = calc_sub_string_act_length(uCount, uPos, uLength);
		if( uRet == 0 )
			return ;
		str.RemoveAt(uPos, uRet);
	}

	//replace
	/*! \brief Replace characters in a string.

	Replace characters in a string.
	\tparam Tstring A string type.
	\tparam TCompareTrait A compare trait type.
	\param chOld [in] The character will be replaced.
	\param chNew [in] The character used to replace.
	\param str [in, out] Specify the string.
	\return The number of replaced characters.
	*/
	template <class Tstring, class TCompareTrait = GKC::DefaultCompareTrait<typename Tstring::EType>>
	static uintptr Replace(const typename Tstring::EType& chOld, const typename Tstring::EType& chNew, INOUT Tstring& str) throw()
	{
		assert( chOld != 0 && chNew != 0 && TCompareTrait::IsNE(chOld, chNew) );
		if( str.IsEmpty() )
			return 0;
		return coll_replace_elements<typename Tstring::Iterator, TCompareTrait>(chOld, chNew, str.GetBegin(), str.GetEnd());
	}
};

// _ArrayUtilHelper

class _ArrayUtilHelper
{
public:
	//find
	//  TArray : GKC::ConstArray<T>, GKC::FixedArray<T, t_size>, _ShareArray<T>, _UniqueArray<T>
	template <class TArray, class TCompareTrait = GKC::DefaultCompareTrait<typename TArray::EType>>
	static GKC::ArrayPosition Find(const TArray& arr, const typename TArray::EType& t) throw()
	{
		if( arr.GetCount() != 0 ) {
			for( auto iter(arr.GetBegin()); iter != arr.GetEnd(); iter.MoveNext() ) {
				if( TCompareTrait::IsEQ(iter.get_Value(), t) )
					return arr.ToPosition(iter);
			}
		}
		return GKC::ArrayPosition();  //invalid : INVALID_ARRAY_INDEX
	}
	template <class TArray, class TCompareTrait = GKC::DefaultCompareTrait<typename TArray::EType>>
	static GKC::ArrayPosition Find(const TArray& arr, uintptr uStart, uintptr uCount, const typename TArray::EType& t) throw()
	{
		if( arr.GetCount() != 0 && uCount != 0 ) {
			auto iterB(arr.ToIterator(arr.GetPosition(uStart)));
			auto iterE(arr.ToIterator(arr.GetPosition(uStart + uCount)));  //no check : overflow
			for( auto iter(iterB); iter != iterE; iter.MoveNext() ) {
				if( TCompareTrait::IsEQ(iter.get_Value(), t) )
					return arr.ToPosition(iter);
			}
		}
		return GKC::ArrayPosition();  //invalid : INVALID_ARRAY_INDEX
	}
};

// _StringUtilHelper

class _StringUtilHelper
{
public:
	//To ConstStringT object (use carefully)
	template <typename Tchar, uintptr t_size>
	static GKC::ConstStringT<Tchar> To_ConstString(const GKC::FixedStringT<Tchar, t_size>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetLength() );
		return GKC::ConstStringT<Tchar>(GKC::FixedArrayHelper::GetInternalPointer(str) + uStart, str.GetLength() - uStart);
	}
	template <typename Tchar>
	static GKC::ConstStringT<Tchar> To_ConstString(const _StringT<Tchar>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetLength() );
		return GKC::ConstStringT<Tchar>(_ShareArrayHelper::GetInternalPointer(str) + uStart, str.GetLength() - uStart);
	}
	template <typename Tchar>
	static GKC::ConstStringT<Tchar> To_ConstString(const _UniqueStringT<Tchar>& str, uintptr uStart = 0) throw()
	{
		assert( uStart <= str.GetLength() );
		return GKC::ConstStringT<Tchar>(_UniqueArrayHelper::GetInternalPointer(str) + uStart, str.GetLength() - uStart);
	}

	//make string
	template <typename Tchar, uintptr t_size>
	static uintptr MakeString(const GKC::ConstStringT<Tchar>& strSrc, GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		assert( !strSrc.IsNull() );
		uintptr uCount = strSrc.GetCount();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uCount;
	}
	template <typename Tchar>
	static void MakeString(const GKC::ConstStringT<Tchar>& strSrc, _StringT<Tchar>& strDest)
	{
		assert( !strSrc.IsNull() );
		uintptr uCount = strSrc.GetCount();
		strDest.SetLength(uCount);
		if( uCount == 0 )
			return ;
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}
	template <typename Tchar>
	static void MakeString(const GKC::ConstStringT<Tchar>& strSrc, _UniqueStringT<Tchar>& strDest)
	{
		assert( !strSrc.IsNull() );
		uintptr uCount = strSrc.GetCount();
		strDest.SetLength(uCount);
		if( uCount == 0 )
			return ;
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	template <typename Tchar, uintptr t_sizeS, uintptr t_sizeD>
	static uintptr MakeString(const GKC::FixedStringT<Tchar, t_sizeS>& strSrc, GKC::FixedStringT<Tchar, t_sizeD>& strDest) throw()
	{
		assert( t_sizeS != t_sizeD );
		uintptr uCount = strSrc.GetLength();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_sizeD - 1 )
			uCount = t_sizeD - 1;
		strDest.SetLength(uCount);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uCount;
	}
	template <typename Tchar, uintptr t_size>
	static void MakeString(const GKC::FixedStringT<Tchar, t_size>& strSrc, _StringT<Tchar>& strDest)
	{
		uintptr uCount = strSrc.GetLength();
		strDest.SetLength(uCount);
		if( uCount == 0 )
			return ;
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}
	template <typename Tchar, uintptr t_size>
	static void MakeString(const GKC::FixedStringT<Tchar, t_size>& strSrc, _UniqueStringT<Tchar>& strDest)
	{
		uintptr uCount = strSrc.GetLength();
		strDest.SetLength(uCount);
		if( uCount == 0 )
			return ;
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	template <typename Tchar, uintptr t_size>
	static uintptr MakeString(const _StringT<Tchar>& strSrc, GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount = strSrc.GetLength();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uCount;
	}
	template <typename Tchar>
	static void MakeString(const _StringT<Tchar>& strSrc, _UniqueStringT<Tchar>& strDest)
	{
		uintptr uCount = strSrc.GetLength();
		strDest.SetLength(uCount);
		if( uCount == 0 )
			return ;
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	template <typename Tchar, uintptr t_size>
	static uintptr MakeString(const _UniqueStringT<Tchar>& strSrc, GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount = strSrc.GetLength();
		if( uCount == 0 ) {
			strDest.SetLength(0);
			return 0;
		}
		if( uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uCount;
	}
	template <typename Tchar>
	static void MakeString(const _UniqueStringT<Tchar>& strSrc, _StringT<Tchar>& strDest)
	{
		uintptr uCount = strSrc.GetLength();
		strDest.SetLength(uCount);
		if( uCount == 0 )
			return ;
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc), uCount * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}

	//append (use carefully)
	//  return: the characters of strSrc are copied
	template <typename Tchar, uintptr t_size>
	static uintptr Append(const GKC::ConstStringT<Tchar>& strSrc, INOUT GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		assert( !strSrc.IsNull() );
		uintptr uCount1 = strSrc.GetCount();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		GKC::CallResult cr = GKC::SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest) + uCount2);
		return uCount1;
	}
	template <typename Tchar>
	static void Append(const GKC::ConstStringT<Tchar>& strSrc, INOUT _StringT<Tchar>& strDest)
	{
		assert( !strSrc.IsNull() );
		uintptr uCount1 = strSrc.GetCount();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest) + uCount2);
	}
	template <typename Tchar>
	static void Append(const GKC::ConstStringT<Tchar>& strSrc, INOUT _UniqueStringT<Tchar>& strDest)
	{
		assert( !strSrc.IsNull() );
		uintptr uCount1 = strSrc.GetCount();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest) + uCount2);
	}

	template <typename Tchar, uintptr t_sizeS, uintptr t_sizeD>
	static uintptr Append(const GKC::FixedStringT<Tchar, t_sizeS>& strSrc, INOUT GKC::FixedStringT<Tchar, t_sizeD>& strDest) throw()
	{
		assert( &strSrc != &strDest );
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		GKC::CallResult cr = GKC::SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_sizeD - 1 )
			uCount = t_sizeD - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest) + uCount2);
		return uCount1;
	}
	template <typename Tchar, uintptr t_size>
	static void Append(const GKC::FixedStringT<Tchar, t_size>& strSrc, INOUT _StringT<Tchar>& strDest)
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest) + uCount2);
	}
	template <typename Tchar, uintptr t_size>
	static void Append(const GKC::FixedStringT<Tchar, t_size>& strSrc, INOUT _UniqueStringT<Tchar>& strDest)
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest) + uCount2);
	}

	template <typename Tchar, uintptr t_size>
	static uintptr Append(const _StringT<Tchar>& strSrc, INOUT GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		GKC::CallResult cr = GKC::SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest) + uCount2);
		return uCount1;
	}
	template <typename Tchar>
	static void Append(const _StringT<Tchar>& strSrc, INOUT _StringT<Tchar>& strDest)
	{
		assert( &strSrc != &strDest && strSrc != strDest );
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest) + uCount2);
	}
	template <typename Tchar>
	static void Append(const _StringT<Tchar>& strSrc, INOUT _UniqueStringT<Tchar>& strDest)
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest) + uCount2);
	}

	template <typename Tchar, uintptr t_size>
	static uintptr Append(const _UniqueStringT<Tchar>& strSrc, INOUT GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount;
		GKC::CallResult cr = GKC::SafeOperators::Add(uCount1, uCount2, uCount);
		if( cr.IsFailed() || uCount > t_size - 1 )
			uCount = t_size - 1;
		strDest.SetLength(uCount);
		uCount1 = uCount - uCount2;
		if( uCount1 == 0 )
			return 0;
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest) + uCount2);
		return uCount1;
	}
	template <typename Tchar>
	static void Append(const _UniqueStringT<Tchar>& strSrc, INOUT _StringT<Tchar>& strDest)
	{
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest) + uCount2);
	}
	template <typename Tchar>
	static void Append(const _UniqueStringT<Tchar>& strSrc, INOUT _UniqueStringT<Tchar>& strDest)
	{
		assert( &strSrc != &strDest && strSrc != strDest );
		uintptr uCount1 = strSrc.GetLength();
		uintptr uCount2 = strDest.GetLength();
		uintptr uCount = GKC::SafeOperators::AddThrow(uCount1, uCount2);
		strDest.SetLength(uCount);
		if( uCount1 == 0 )
			return ;
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc), uCount1 * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest) + uCount2);
	}

	//insert (use carefully)
	template <typename Tchar, uintptr t_size>
	static uintptr Insert(uintptr uPos, const GKC::ConstStringT<Tchar>& strAdd, GKC::FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		assert( t_size > uLength );
		uintptr uAdd = strAdd.GetCount();
		if( uAdd == 0 || uPos > uLength || t_size - 1 - uLength < uAdd )
			return 0;
		mem_move(GKC::FixedArrayHelper::GetInternalPointer(str) + uPos, (uLength - uPos) * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos + uAdd);
		str.SetLength(uLength + uAdd);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos);
		return uAdd;
	}
	template <typename Tchar, uintptr t_size, uintptr t_sizeA>
	static uintptr Insert(uintptr uPos, const GKC::FixedStringT<Tchar, t_sizeA>& strAdd, GKC::FixedStringT<Tchar, t_size>& str) throw()
	{
		assert( &strAdd != &str );
		uintptr uLength = str.GetLength();
		assert( t_size > uLength );
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength || t_size - 1 - uLength < uAdd )
			return 0;
		mem_move(GKC::FixedArrayHelper::GetInternalPointer(str) + uPos, (uLength - uPos) * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos + uAdd);
		str.SetLength(uLength + uAdd);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos);
		return uAdd;
	}
	template <typename Tchar, uintptr t_size>
	static uintptr Insert(uintptr uPos, const _StringT<Tchar>& strAdd, GKC::FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		assert( t_size > uLength );
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength || t_size - 1 - uLength < uAdd )
			return 0;
		mem_move(GKC::FixedArrayHelper::GetInternalPointer(str) + uPos, (uLength - uPos) * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos + uAdd);
		str.SetLength(uLength + uAdd);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos);
		return uAdd;
	}
	template <typename Tchar, uintptr t_size>
	static uintptr Insert(uintptr uPos, const _UniqueStringT<Tchar>& strAdd, GKC::FixedStringT<Tchar, t_size>& str) throw()
	{
		uintptr uLength = str.GetLength();
		assert( t_size > uLength );
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength || t_size - 1 - uLength < uAdd )
			return 0;
		mem_move(GKC::FixedArrayHelper::GetInternalPointer(str) + uPos, (uLength - uPos) * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos + uAdd);
		str.SetLength(uLength + uAdd);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(str) + uPos);
		return uAdd;
	}

	template <typename Tchar>
	static void Insert(uintptr uPos, const GKC::ConstStringT<Tchar>& strAdd, _StringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetCount();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(str) + uPos);
	}
	template <typename Tchar, uintptr t_size>
	static void Insert(uintptr uPos, const GKC::FixedStringT<Tchar, t_size>& strAdd, _StringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(str) + uPos);
	}
	template <typename Tchar>
	static void Insert(uintptr uPos, const _StringT<Tchar>& strAdd, _StringT<Tchar>& str)
	{
		assert( &strAdd != &str && strAdd != str );
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(str) + uPos);
	}
	template <typename Tchar>
	static void Insert(uintptr uPos, const _UniqueStringT<Tchar>& strAdd, _StringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(str) + uPos);
	}

	template <typename Tchar>
	static void Insert(uintptr uPos, const GKC::ConstStringT<Tchar>& strAdd, _UniqueStringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetCount();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(str) + uPos);
	}
	template <typename Tchar, uintptr t_size>
	static void Insert(uintptr uPos, const GKC::FixedStringT<Tchar, t_size>& strAdd, _UniqueStringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(str) + uPos);
	}
	template <typename Tchar>
	static void Insert(uintptr uPos, const _StringT<Tchar>& strAdd, _UniqueStringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(str) + uPos);
	}
	template <typename Tchar>
	static void Insert(uintptr uPos, const _UniqueStringT<Tchar>& strAdd, _UniqueStringT<Tchar>& str)
	{
		assert( &strAdd != &str && strAdd != str );
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetLength();
		if( uAdd == 0 || uPos > uLength )
			return ;
		if( uLength == 0 )
			str.SetLength(0);
		str.InsertAt(uPos, uAdd);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(str) + uPos);
	}

	//sub-string (use carefully)
	template <typename Tchar, uintptr t_size>
	static uintptr Sub(const GKC::ConstStringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetCount(), uStart, uLength);
		if( uRet == 0 )
			return 0;
		if( uRet > t_size - 1 )
			uRet = t_size - 1;
		strDest.SetLength(uRet);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uRet;
	}
	template <typename Tchar>
	static void Sub(const GKC::ConstStringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, _StringT<Tchar>& strDest)
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetCount(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}
	template <typename Tchar>
	static void Sub(const GKC::ConstStringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, _UniqueStringT<Tchar>& strDest)
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetCount(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	template <typename Tchar, uintptr t_sizeS, uintptr t_sizeD>
	static uintptr Sub(const GKC::FixedStringT<Tchar, t_sizeS>& strSrc, uintptr uStart, uintptr uLength, GKC::FixedStringT<Tchar, t_sizeD>& strDest) throw()
	{
		assert( &strSrc != &strDest );
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return 0;
		if( uRet > t_sizeD - 1 )
			uRet = t_sizeD - 1;
		strDest.SetLength(uRet);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uRet;
	}
	template <typename Tchar, uintptr t_size>
	static void Sub(const GKC::FixedStringT<Tchar, t_size>& strSrc, uintptr uStart, uintptr uLength, _StringT<Tchar>& strDest)
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}
	template <typename Tchar, uintptr t_size>
	static void Sub(const GKC::FixedStringT<Tchar, t_size>& strSrc, uintptr uStart, uintptr uLength, _UniqueStringT<Tchar>& strDest)
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(GKC::FixedArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	template <typename Tchar, uintptr t_size>
	static uintptr Sub(const _StringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return 0;
		if( uRet > t_size - 1 )
			uRet = t_size - 1;
		strDest.SetLength(uRet);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uRet;
	}
	template <typename Tchar>
	static void Sub(const _StringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, _StringT<Tchar>& strDest)
	{
		assert( &strSrc != &strDest && strSrc != strDest );
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}
	template <typename Tchar>
	static void Sub(const _StringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, _UniqueStringT<Tchar>& strDest)
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	template <typename Tchar, uintptr t_size>
	static uintptr Sub(const _UniqueStringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, GKC::FixedStringT<Tchar, t_size>& strDest) throw()
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return 0;
		if( uRet > t_size - 1 )
			uRet = t_size - 1;
		strDest.SetLength(uRet);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), GKC::FixedArrayHelper::GetInternalPointer(strDest));
		return uRet;
	}
	template <typename Tchar>
	static void Sub(const _UniqueStringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, _StringT<Tchar>& strDest)
	{
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(strDest));
	}
	template <typename Tchar>
	static void Sub(const _UniqueStringT<Tchar>& strSrc, uintptr uStart, uintptr uLength, _UniqueStringT<Tchar>& strDest)
	{
		assert( &strSrc != &strDest && strSrc != strDest );
		strDest.SetLength(0);
		uintptr uRet = calc_sub_string_act_length(strSrc.GetLength(), uStart, uLength);
		if( uRet == 0 )
			return ;
		strDest.SetLength(uRet);
		mem_copy(_UniqueArrayHelper::GetInternalPointer(strSrc) + uStart, uRet * sizeof(Tchar), _UniqueArrayHelper::GetInternalPointer(strDest));
	}

	//find (return value : check null)
	template <typename Tchar>
	static typename GKC::ConstStringT<Tchar>::Iterator Find(const GKC::ConstStringT<Tchar>& str, const GKC::ConstStringT<Tchar>& strFind, uintptr uStart) throw()
	{
		assert( uStart <= str.GetCount() );
		return typename GKC::ConstStringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_string(GKC::ConstArrayHelper::GetInternalPointer(str) + uStart, GKC::ConstArrayHelper::GetInternalPointer(strFind))));
	}
	template <typename Tchar, uintptr t_size>
	static typename GKC::FixedStringT<Tchar, t_size>::Iterator Find(const GKC::FixedStringT<Tchar, t_size>& str, const GKC::ConstStringT<Tchar>& strFind, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		return typename GKC::FixedStringT<Tchar, t_size>::Iterator(GKC::RefPtr<Tchar>(find_string_string(GKC::FixedArrayHelper::GetInternalPointer(str) + uStart, GKC::ConstArrayHelper::GetInternalPointer(strFind))));
	}
	template <typename Tchar>
	static typename _StringT<Tchar>::Iterator Find(const _StringT<Tchar>& str, const GKC::ConstStringT<Tchar>& strFind, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsBlockNull() );
		return typename _StringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_string(_ShareArrayHelper::GetInternalPointer(str) + uStart, GKC::ConstArrayHelper::GetInternalPointer(strFind))));
	}
	template <typename Tchar>
	static typename _UniqueStringT<Tchar>::Iterator Find(const _UniqueStringT<Tchar>& str, const GKC::ConstStringT<Tchar>& strFind, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsNull() );
		return typename _UniqueStringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_string(_UniqueArrayHelper::GetInternalPointer(str) + uStart, GKC::ConstArrayHelper::GetInternalPointer(strFind))));
	}

	//replace
	template <typename Tchar>
	static uintptr Replace(uintptr uPos, uintptr uOld, const GKC::ConstStringT<Tchar>& strNew, INOUT _StringT<Tchar>& str)
	{
		assert( uPos <= str.GetLength() && uOld <= str.GetLength() - uPos );
		uintptr uNew = strNew.GetCount();
		Tchar* pr = _ShareArrayHelper::GetInternalPointer(str) + uPos;
		if( uOld > uNew ) {
			_StringOpHelper::Delete(uPos + uNew, uOld - uNew, str);
			pr = _ShareArrayHelper::GetInternalPointer(str) + uPos;
		}
		else if( uOld < uNew ) {
			if( str.GetLength() == 0 )
				str.SetLength(0);
			str.InsertAt(uPos + uOld, uNew - uOld);  //may throw
			pr = _ShareArrayHelper::GetInternalPointer(str) + uPos;
		}
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strNew), uNew * sizeof(Tchar), pr);
		return uPos + uNew;
	}
	template <typename Tchar>
	static uintptr Replace(const GKC::ConstStringT<Tchar>& strOld, const GKC::ConstStringT<Tchar>& strNew, INOUT _StringT<Tchar>& str)
	{
		assert( !strOld.IsNull() );
		assert( !str.IsBlockNull() );
		if( str.IsEmpty() )
			return 0;
		uintptr uCount = 0;
		uintptr uOld = strOld.GetCount();
		uintptr uPos = 0;
		while( true ) {
			Tchar* ps = _ShareArrayHelper::GetInternalPointer(str);
			Tchar* pr = find_string_string(ps + uPos, GKC::ConstArrayHelper::GetInternalPointer(strOld));
			if( pr == NULL )
				break;
			uPos = Replace<Tchar>(pr - ps, uOld, strNew, str);  //may throw
			uCount ++;
		} //end while
		return uCount;
	}

	template <typename Tchar>
	static uintptr Replace(uintptr uPos, uintptr uOld, const GKC::ConstStringT<Tchar>& strNew, INOUT _UniqueStringT<Tchar>& str)
	{
		assert( uPos <= str.GetLength() && uOld <= str.GetLength() - uPos );
		uintptr uNew = strNew.GetCount();
		Tchar* pr = _UniqueArrayHelper::GetInternalPointer(str) + uPos;
		if( uOld > uNew ) {
			_StringOpHelper::Delete(uPos + uNew, uOld - uNew, str);
			pr = _UniqueArrayHelper::GetInternalPointer(str) + uPos;
		}
		else if( uOld < uNew ) {
			if( str.GetLength() == 0 )
				str.SetLength(0);
			str.InsertAt(uPos + uOld, uNew - uOld);  //may throw
			pr = _UniqueArrayHelper::GetInternalPointer(str) + uPos;
		}
		mem_copy(GKC::ConstArrayHelper::GetInternalPointer(strNew), uNew * sizeof(Tchar), pr);
		return uPos + uNew;
	}
	template <typename Tchar>
	static uintptr Replace(const GKC::ConstStringT<Tchar>& strOld, const GKC::ConstStringT<Tchar>& strNew, INOUT _UniqueStringT<Tchar>& str)
	{
		assert( !strOld.IsNull() );
		if( str.IsEmpty() )
			return 0;
		uintptr uCount = 0;
		uintptr uOld = strOld.GetCount();
		uintptr uPos = 0;
		while( true ) {
			Tchar* ps = _UniqueArrayHelper::GetInternalPointer(str);
			Tchar* pr = find_string_string(ps + uPos, GKC::ConstArrayHelper::GetInternalPointer(strOld));
			if( pr == NULL )
				break;
			uPos = Replace<Tchar>(pr - ps, uOld, strNew, str);  //may throw
			uCount ++;
		} //end while
		return uCount;
	}
};

#pragma pack(push, 1)

// _VariantString

class _VariantString
{
public:
	//types
	enum {
		None = 0,
		Char8 = sizeof(GKC::CharA), Char16 = sizeof(GKC::CharH), Char32 = sizeof(GKC::CharL),
		MaxType
	};

public:
	_VariantString() throw() : m_iType(None)
	{
		assert( sizeof(_StringA) == sizeof(_StringH)
			&& sizeof(_StringA) == sizeof(_StringL) );
	}
	_VariantString(const _VariantString& src) throw() : m_iType(None)
	{
		operator=(src);
	}
	_VariantString(_VariantString&& src) throw() : m_iType(None)
	{
		operator=(rv_forward(src));
	}
	~_VariantString() throw()
	{
		destroy();
	}

	//assignment
	_VariantString& operator=(const _VariantString& src) throw()
	{
		SetType(src.m_iType);
		if( m_iType == Char8 )
			get_string<_StringA>() = src.get_string<_StringA>();
		else if( m_iType == Char16 )
			get_string<_StringH>() = src.get_string<_StringH>();
		else if( m_iType == Char32 )
			get_string<_StringL>() = src.get_string<_StringL>();
		return *this;
	}
	_VariantString& operator=(_VariantString&& src) throw()
	{
		SetType(src.m_iType);
		if( m_iType == Char8 )
			get_string<_StringA>() = rv_forward(src.get_string<_StringA>());
		else if( m_iType == Char16 )
			get_string<_StringH>() = rv_forward(src.get_string<_StringH>());
		else if( m_iType == Char32 )
			get_string<_StringL>() = rv_forward(src.get_string<_StringL>());
		return *this;
	}

	//methods

	// iType : it can be sizeof(CharX)
	void SetType(int iType) throw()
	{
		if( m_iType != iType ) {
			destroy();
			construct(iType);
			m_iType = iType;
		}
	}
	int GetType() const throw()
	{
		return m_iType;
	}

	template <class TString>
	TString& GetString() throw()
	{
		assert( m_iType == sizeof(typename TString::EType) );
		return get_string<TString>();
	}
	template <class TString>
	const TString& GetString() const throw()
	{
		assert( m_iType == sizeof(typename TString::EType) );
		return get_string<TString>();
	}

	void Reset()
	{
		if( m_iType == Char8 ) {
			_StringA& str = get_string<_StringA>();
			if( str.IsBlockNull() )
				str = _StringHelper::MakeEmptyString<GKC::CharA>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get()));  //may throw
		}
		else if( m_iType == Char16 ) {
			_StringH& str = get_string<_StringH>();
			if( str.IsBlockNull() )
				str = _StringHelper::MakeEmptyString<GKC::CharH>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get()));  //may throw
		}
		else if( m_iType == Char32 ) {
			_StringL& str = get_string<_StringL>();
			if( str.IsBlockNull() )
				str = _StringHelper::MakeEmptyString<GKC::CharL>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get()));  //may throw
		}
		SetLength(0);  //may throw
	}

	uintptr GetLength() const throw()
	{
		uintptr uLength = 0;
		if( m_iType == Char8 )
			uLength = get_string<_StringA>().GetLength();
		else if( m_iType == Char16 )
			uLength = get_string<_StringH>().GetLength();
		else if( m_iType == Char32 )
			uLength = get_string<_StringL>().GetLength();
		return uLength;
	}
	void SetLength(uintptr uLength)
	{
		if( m_iType == Char8 )
			get_string<_StringA>().SetLength(uLength);  //may throw
		else if( m_iType == Char16 )
			get_string<_StringH>().SetLength(uLength);  //may throw
		else if( m_iType == Char32 )
			get_string<_StringL>().SetLength(uLength);  //may throw
	}
	void GetAt(uintptr uIndex, GKC::CharF& ch) const throw()
	{
		ch = 0;
		if( m_iType == Char8 )
			ch = (GKC::CharF)(get_string<_StringA>().GetAt(uIndex));
		else if( m_iType == Char16 )
			ch = (GKC::CharF)(get_string<_StringH>().GetAt(uIndex));
		else if( m_iType == Char32 )
			ch = (GKC::CharF)(get_string<_StringL>().GetAt(uIndex));
		else
			assert( false );
	}
	void SetAt(uintptr uIndex, const GKC::CharF& ch) throw()
	{
		if( m_iType == Char8 )
			get_string<_StringA>().GetAt(uIndex) = (GKC::CharA)ch;
		else if( m_iType == Char16 )
			get_string<_StringH>().GetAt(uIndex) = (GKC::CharH)ch;
		else if( m_iType == Char32 )
			get_string<_StringL>().GetAt(uIndex) = (GKC::CharL)ch;
		else
			assert( false );
	}
	uintptr GetAddress() const throw()
	{
		uintptr uAddress = 0;
		if( m_iType == Char8 )
			uAddress = (uintptr)_ShareArrayHelper::GetInternalPointer(get_string<_StringA>());
		else if( m_iType == Char16 )
			uAddress = (uintptr)_ShareArrayHelper::GetInternalPointer(get_string<_StringH>());
		else if( m_iType == Char32 )
			uAddress = (uintptr)_ShareArrayHelper::GetInternalPointer(get_string<_StringL>());
		return uAddress;
	}

	void Delete(uintptr uStart, uintptr uLength) throw()
	{
		if( m_iType == Char8 )
			_StringOpHelper::Delete(uStart, uLength, get_string<_StringA>());
		else if( m_iType == Char16 )
			_StringOpHelper::Delete(uStart, uLength, get_string<_StringH>());
		else if( m_iType == Char32 )
			_StringOpHelper::Delete(uStart, uLength, get_string<_StringL>());
		else
			assert( false );
	}
	void Append(const GKC::CharF& uChar)
	{
		if( m_iType == Char8 )
			_StringOpHelper::Append((GKC::CharA)uChar, get_string<_StringA>());  //may throw
		else if( m_iType == Char16 )
			_StringOpHelper::Append((GKC::CharH)uChar, get_string<_StringH>());  //may throw
		else if( m_iType == Char32 )
			_StringOpHelper::Append((GKC::CharL)uChar, get_string<_StringL>());  //may throw
	}
	void Insert(uintptr uStart, const _VariantString& strAdd)
	{
		if( m_iType == Char8 )
			_StringUtilHelper::Insert(uStart, strAdd.get_string<_StringA>(), get_string<_StringA>());  //may throw
		else if( m_iType == Char16 )
			_StringUtilHelper::Insert(uStart, strAdd.get_string<_StringH>(), get_string<_StringH>());  //may throw
		else if( m_iType == Char32 )
			_StringUtilHelper::Insert(uStart, strAdd.get_string<_StringL>(), get_string<_StringL>());  //may throw
		else
			assert( false );
	}

	void CloneTo(_VariantString& str) const
	{
		str.SetType(GetType());
		if( m_iType == Char8 )
			str.get_string<_StringA>() = _StringHelper::Clone(get_string<_StringA>());  //may throw
		else if( m_iType == Char16 )
			str.get_string<_StringH>() = _StringHelper::Clone(get_string<_StringH>());  //may throw
		else if( m_iType == Char32 )
			str.get_string<_StringL>() = _StringHelper::Clone(get_string<_StringL>());  //may throw
	}
	void ToSubString(uintptr uStart, uintptr uLength, _VariantString& str) const
	{
		str.SetType(GetType());
		str.Reset();  //may throw
		if( m_iType == Char8 )
			_StringUtilHelper::Sub(get_string<_StringA>(), uStart, uLength, str.get_string<_StringA>());  //may throw
		else if( m_iType == Char16 )
			_StringUtilHelper::Sub(get_string<_StringH>(), uStart, uLength, str.get_string<_StringH>());  //may throw
		else if( m_iType == Char32 )
			_StringUtilHelper::Sub(get_string<_StringL>(), uStart, uLength, str.get_string<_StringL>());  //may throw
		else
			assert( false );
	}

private:
	void destroy() throw()
	{
		if( m_iType == Char8 )
			_SObjSoloHelper::object_destruction<_StringA>(&get_string<_StringA>());
		else if( m_iType == Char16 )
			_SObjSoloHelper::object_destruction<_StringH>(&get_string<_StringH>());
		else if( m_iType == Char32 )
			_SObjSoloHelper::object_destruction<_StringL>(&get_string<_StringL>());
		m_iType = None;
	}
	void construct(int iType) throw()
	{
		assert( iType >= None && iType < MaxType );
		if( iType == Char8 )
			call_constructor(get_string<_StringA>());  //no throw
		else if( iType == Char16 )
			call_constructor(get_string<_StringH>());  //no throw
		else if( iType == Char32 )
			call_constructor(get_string<_StringL>());  //no throw
	}

	template <class TString>
	TString& get_string() throw()
	{
		TString* p = (TString*)m_bt;
		return *p;
	}
	template <class TString>
	const TString& get_string() const throw()
	{
		const TString* p = (const TString*)m_bt;
		return *p;
	}

private:
	byte m_bt[sizeof(_StringA)];

	int m_iType;  //character type
};

#pragma pack(pop)

//------------------------------------------------------------------------------
// unique component

#pragma pack(push, 1)

struct _unique_com_header
{
	sa_handle hd;
	unique_com_block block;
};

// _UniqueCom

class _UniqueCom
{
public:
	_UniqueCom() noexcept : m_p(NULL)
	{
	}
	_UniqueCom(const _UniqueCom&) = delete;
	_UniqueCom(_UniqueCom&& src) noexcept : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	~_UniqueCom() noexcept
	{
		Release();
	}

	_UniqueCom& operator=(const _UniqueCom&) = delete;
	_UniqueCom& operator=(_UniqueCom&& src) noexcept
	{
		if ( this != &src ) {
			if ( m_p == src.m_p ) {
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

	bool operator==(const _UniqueCom& right) const noexcept
	{
		return m_p == right.m_p;
	}
	bool operator!=(const _UniqueCom& right) const noexcept
	{
		return m_p != right.m_p;
	}

	void Release() noexcept
	{
		if ( m_p != NULL ) {
			GKC::IMemoryManager* pMgr = get_block()->GetMemoryManager();
			if ( pMgr != NULL ) {
				get_block()->DestructObject();
				//may unload a SA
				get_sa_handle()->~sa_handle();
				pMgr->Free((uintptr)m_p);
			}
			m_p = NULL;
		}
	}

	bool IsNull() const noexcept
	{
		return m_p == NULL;
	}

protected:
	_unique_com_header* get_header() const noexcept
	{
		return (_unique_com_header*)m_p;
	}

	sa_handle* get_sa_handle() const noexcept
	{
		return &(get_header()->hd);
	}
	unique_com_block* get_block() const noexcept
	{
		return &(get_header()->block);
	}
	void* get_object() const noexcept
	{
		return get_block()->GetAddress();
	}

protected:
	void* m_p;

private:
	friend class _UniqueComHelper;
};

#pragma pack(pop)

/*
DECLARE_COM_TYPECAST(cls)

BEGIN_COM_TYPECAST(cls)
COM_TYPECAST_ENTRY(ifname, ibname)
END_COM_TYPECAST()
*/

#pragma pack(push, 1)

// _ScopeUniqueComObject<T>

template <class T>
class _ScopeUniqueComObject
{
public:
	_ScopeUniqueComObject()
	{
		m_header.block.SetMemoryManager(NULL);
		m_header.block.SetDestructionFunc(NULL);
		m_header.block.SetTypeCastFunc(GKC::_Com_TypeCast_Func<T>::c_func);
		m_header.block.SetAddress(&m_t);
	}
	_ScopeUniqueComObject(const _ScopeUniqueComObject&) = delete;
	_ScopeUniqueComObject& operator=(const _ScopeUniqueComObject&) = delete;
	~_ScopeUniqueComObject() noexcept
	{
	}

private:
	_unique_com_header m_header;
	T m_t;

private:
	friend class _UniqueComHelper;
};

#pragma pack(pop)

// _UniqueComHelper

class _UniqueComHelper
{
public:
	//make unique component object
	template <class T, typename... Args>
	static _UniqueCom MakeUniqueCom(const GKC::RefPtr<GKC::IMemoryManager>& mgr, Args&&... args)
	{
		assert( !mgr.IsNull() );

		//allocate
		uintptr uBytes = GKC::SafeOperators::AddThrow((uintptr)sizeof(_unique_com_header), (uintptr)sizeof(T));
		void* p = (void*)((const_cast<GKC::RefPtr<GKC::IMemoryManager>&>(mgr)).Deref().Allocate(uBytes));
		if ( p == NULL )
			throw GKC::OutOfMemoryException();

		_unique_com_header* ph = (_unique_com_header*)p;
		T* pT = (T*)(void*)(ph + 1);
		//constructor
		try {
			//may throw
			call_constructor(*pT, rv_forward<Args>(args)...);
		}
		catch(...) {
			(const_cast<GKC::RefPtr<GKC::IMemoryManager>&>(mgr)).Deref().Free((uintptr)p);
			throw;  //re-throw
		}

		call_constructor(ph->hd);  //no throw

		//initialize
		ph->block.SetMemoryManager(GKC::RefPtrHelper::GetInternalPointer(mgr));
		ph->block.SetDestructionFunc(&(_SObjSoloHelper::object_destruction<T>));
		ph->block.SetTypeCastFunc(GKC::_Com_TypeCast_Func<T>::c_func);
		ph->block.SetAddress(pT);

		_UniqueCom ret;
		//return value
		ret.m_p = p;

		return ret;
	}

	//query type
	// T : interface
	template <class T>
	static GKC::RefPtr<T> Query(const _UniqueCom& sp, const guid& iid) noexcept
	{
		GKC::RefPtr<T> ret;
		if( !sp.IsNull() )
			ret = (T*)((sp.get_block()->GetTypeCastFunc())(sp.get_object(), iid));
		return ret;
	}

	//clone
	// T : the component class
	template <class T>
	static _UniqueCom Clone(const _UniqueCom& sp)
	{
		return ( !sp.IsNull() )
				? MakeUniqueCom(GKC::RefPtr<GKC::IMemoryManager>(sp.get_block()->GetMemoryManager()), *((T*)(sp.get_object())))
				: _UniqueCom();
	}

	//scope
	template <class T>
	static _UniqueCom ToUniqueCom(const _ScopeUniqueComObject<T>& sp) noexcept
	{
		_UniqueCom ret;
		ret.m_p = (void*)&(sp.m_header);
		return ret;
	}

	//for test
	template <class T>
	static T& to_object(const _UniqueCom& sp) noexcept
	{
		return *((T*)(sp.get_object()));
	}

	//only for creation from SA
	static void set_sa_handle(sa_handle&& hd, _UniqueCom& sp) noexcept
	{
		assert( !sp.IsNull() );
		*(sp.get_sa_handle()) = rv_forward(hd);
	}
};

#define CALL_UNIQUECOM_TYPECAST(src, ifname)  _UniqueComHelper::Query<ifname>(src, USE_GUID(GUID_##ifname))

//------------------------------------------------------------------------------
// Component

//functions for creating components directly

// share com

template <class T>
inline GKC::CallResult _Create_Component_Instance(_ShareCom<T>& sp) throw()
{
	GKC::CallResult cr;
	try {
		sp = _ShareComHelper::MakeShareCom<T>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get()));
	}
	catch(GKC::Exception& e) {
		cr = e.GetResult();
	}
	catch(...) {
		cr.SetResult(GKC::SystemCallResults::Fail);
	}
	return cr;
}

template <class T>
inline GKC::CallResult _Component_Instance_Query(const _ShareCom<T>& spC, const guid& iid, _ShareCom<void>& sp) throw()
{
	_ShareCom<void> spI(_ShareComHelper::Query<T>(spC, iid));
	if( spI.IsBlockNull() )
		return GKC::CallResult(GKC::SystemCallResults::Fail);
	sp = spI;
	return GKC::CallResult();
}
template <class T, class TInterface>
inline GKC::CallResult _Component_Instance_Query(const _ShareCom<T>& spC, const guid& iid, _ShareCom<TInterface>& sp) throw()
{
	_ShareCom<TInterface> spI(_ShareComHelper::Query<T, TInterface>(spC, iid));
	if( spI.IsBlockNull() )
		return GKC::CallResult(GKC::SystemCallResults::Fail);
	sp = spI;
	return GKC::CallResult();
}

#define _COMPONENT_INSTANCE_INTERFACE(com_type, if_type, spC, sp, cr)  \
	cr = _Component_Instance_Query<com_type, if_type>(spC, USE_GUID(GUID_##if_type), sp)

#define _CREATE_COMPONENT_INSTANCE(com_type, if_type, sp, cr)  \
	do { _ShareCom<com_type> __sp_C__;  \
	cr = _Create_Component_Instance<com_type>(__sp_C__);  \
	if( cr.IsSucceeded() ) {  \
		_COMPONENT_INSTANCE_INTERFACE(com_type, if_type, __sp_C__, sp, cr);  \
	} } while(0)

#define _CREATE_SCOPE_COMPONENT_INSTANCE(com_type, if_type, spS, sp, cr)  \
	do { _ShareCom<com_type> __sp_C__(_ShareComHelper::ToShareCom(spS));  \
	if( !__sp_C__.IsBlockNull() ) _COMPONENT_INSTANCE_INTERFACE(com_type, if_type, __sp_C__, sp, cr);  \
	else cr.SetResult(GKC::SystemCallResults::Fail);  \
	} while(0)

// unique com

template <class T>
inline GKC::CallResult _Create_UniqueCom_Instance(_UniqueCom& sp) throw()
{
	GKC::CallResult cr;
	try {
		sp = _UniqueComHelper::MakeUniqueCom<T>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get()));
	}
	catch(GKC::Exception& e) {
		cr = e.GetResult();
	}
	catch(...) {
		cr.SetResult(GKC::SystemCallResults::Fail);
	}
	return cr;
}

template <class T>
inline GKC::CallResult _UniqueCom_Instance_Query(const _UniqueCom& spC, const guid& iid, GKC::RefPtr<T>& sp) throw()
{
	GKC::RefPtr<T> spI(_UniqueComHelper::Query<T>(spC, iid));
	if( spI.IsNull() )
		return GKC::CallResult(GKC::SystemCallResults::Fail);
	sp = spI;
	return GKC::CallResult();
}

#define _UNIQUECOM_INSTANCE_INTERFACE(if_type, spC, sp, cr)  \
	cr = _UniqueCom_Instance_Query<if_type>(spC, USE_GUID(GUID_##if_type), sp)

#define _CREATE_UNIQUECOM_INSTANCE(com_type, sp, cr)  \
	cr = _Create_UniqueCom_Instance<com_type>(sp);

#define _CREATE_SCOPE_UNIQUECOM_INSTANCE(com_type, spS, sp, cr)  \
	do { _UniqueCom __sp_C__(_UniqueComHelper::ToUniqueCom<com_type>(spS));  \
	cr.SetResult(GKC::SystemCallResults::OK);  \
	if( !__sp_C__.IsNull() ) sp = rv_forward(__sp_C__);  \
	else cr.SetResult(GKC::SystemCallResults::Fail);  \
	} while(0)

//functions for creating components from SA

// share com

#pragma pack(push, 1)

// _IComFactory

class NOVTABLE _IComFactory
{
public:
	virtual GKC::CallResult CreateInstance(const guid& iid, _ShareCom<void>& sp) throw() = 0;
};

DECLARE_GUID(GUID__IComFactory)

// _IComSA

class NOVTABLE _IComSA
{
public:
	virtual void LockServer(const bool& bLock) throw() = 0;
};

DECLARE_GUID(GUID__IComSA)

#pragma pack(pop)

typedef GKC::CallResult (* _Com_SA_Create_Factory_Func)(_ShareCom<_IComFactory>& sp);

// --only one component factory class can be defined in a pair of .h and .cpp files.--

// --<header file>--

#define DECLARE_COM_FACTORY_CLASS(com_class)  \
	class _ComFactory_##com_class : public _IComFactory  \
	{ public:  \
	_ComFactory_##com_class() throw() {}  \
	~_ComFactory_##com_class() throw() {}  \
	/* _IComFactory methods */  \
	virtual GKC::CallResult CreateInstance(const guid& iid, _ShareCom<void>& sp) throw()  \
	{ CallResult cr;  \
	_ShareCom<com_class> spC;  \
	cr = _Create_Component_Instance<com_class>(spC);  \
	if( cr.IsSucceeded() ) {  \
		cr = _Component_Instance_Query<com_class>(spC, iid, sp);  \
	} return cr; }  \
	public:  \
	static CallResult Create(_ShareCom<_IComFactory>& sp) throw()  \
	{ CallResult cr;  \
	_CREATE_COMPONENT_INSTANCE(_ComFactory_##com_class, _IComFactory, sp, cr);  \
	return cr; }  \
	private: /* noncopyable */  \
	_ComFactory_##com_class(const _ComFactory_##com_class&) throw();  \
	_ComFactory_##com_class& operator=(const _ComFactory_##com_class&) throw();  \
	};  \
	DECLARE_COM_TYPECAST(_ComFactory_##com_class)

#define DECLARE_COM_SA_FACTORY_CLASS(com_class)  \
	class _ComSAFactory_##com_class : public _ComFactory_##com_class, public _IComSA  \
	{ public:  \
	_ComSAFactory_##com_class() throw() {}  \
	~_ComSAFactory_##com_class() throw() {}  \
	/* _IComSA methods */  \
	virtual void LockServer(const bool& bLock) throw()  \
	{ bLock ? USE_COM_SA_MODULE().LockCount() : USE_COM_SA_MODULE().UnlockCount(); }  \
	public:  \
	static CallResult Create(_ShareCom<_IComFactory>& sp) throw()  \
	{ CallResult cr;  \
	_CREATE_COMPONENT_INSTANCE(_ComSAFactory_##com_class, _IComFactory, sp, cr);  \
	return cr; }  \
	private: /* noncopyable */  \
	_ComSAFactory_##com_class(const _ComSAFactory_##com_class&) throw();  \
	_ComSAFactory_##com_class& operator=(const _ComSAFactory_##com_class&) throw();  \
	};  \
	DECLARE_COM_TYPECAST(_ComSAFactory_##com_class)

// --<.h end>

// --<source file>--

#define IMPLEMENT_COM_FACTORY_CLASS(com_class)  \
	BEGIN_COM_TYPECAST(_ComFactory_##com_class)  \
		COM_TYPECAST_ENTRY(_IComFactory, _IComFactory)  \
	END_COM_TYPECAST()

#define IMPLEMENT_COM_SA_FACTORY_CLASS(com_class)  \
	BEGIN_COM_TYPECAST(_ComSAFactory_##com_class)  \
		COM_TYPECAST_ENTRY(_IComFactory, _ComFactory_##com_class)  \
		COM_TYPECAST_ENTRY(_IComSA, _IComSA)  \
	END_COM_TYPECAST()

// --<.cpp end>--

#define USE_COM_FACTORY_CLASS_NAME(com_class)  _ComFactory_##com_class

#define USE_COM_SA_FACTORY_CLASS_NAME(com_class)  _ComSAFactory_##com_class

#define _SHARE_COM_FACTORY_SIZE  (sizeof(_ShareCom<_IComFactory>))

// _Com_SA_Factory_Item

struct _Com_SA_Factory_Item
{
	const guid* cid;
	_Com_SA_Create_Factory_Func pFunc;
	byte btFactory[_SHARE_COM_FACTORY_SIZE];
	uint flag;
};

// _Com_SA_Module
//   for global variable in SA of components
class _Com_SA_Module
{
public:
	_Com_SA_Module() throw() : m_iLockCount(0)
	{
		//map
		_Com_SA_Factory_Item* pItem = get_com_sa_factory_map();
		if( pItem != NULL ) {
			while( pItem->cid != NULL ) {
				_ShareCom<_IComFactory>* pF = (_ShareCom<_IComFactory>*)(pItem->btFactory);
				call_constructor(*pF);  //no throw
				pItem ++;
			}
		}
	}
	~_Com_SA_Module() throw()
	{
		//map
		_Com_SA_Factory_Item* pItem = get_com_sa_factory_map();
		if( pItem != NULL ) {
			while( pItem->cid != NULL ) {
				_ShareCom<_IComFactory>* pF = (_ShareCom<_IComFactory>*)(pItem->btFactory);
				pF->~_ShareCom<_IComFactory>();
				pItem ++;
			}
		}
	}

	//lock count
	int LockCount() throw()
	{
		return atomic_increment((int&)m_iLockCount);
	}
	int UnlockCount() throw()
	{
		return atomic_decrement((int&)m_iLockCount);
	}
	int GetLockCount() const throw()
	{
		return m_iLockCount;
	}

	//mutex
	GKC::CallResult InitMutex() throw()
	{
		return m_mtx.Init();
	}
	void TermMutex() throw()
	{
		m_mtx.Term();
	}
	void LockMutex() throw()
	{
		m_mtx.Lock();
	}
	void UnlockMutex() throw()
	{
		m_mtx.Unlock();
	}

	//for SA export functions
	void SA_Com_GetClassObject(const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw()
	{
		cr.SetResult(GKC::SystemCallResults::OK);
		_Com_SA_Factory_Item* pItem = get_com_sa_factory_map();
		if( pItem == NULL ) {
			cr.SetResult(GKC::SystemCallResults::NotImpl);
			return ;
		}
		_Com_SA_Factory_Item* pFound = NULL;
		while( pItem->cid != NULL ) {
			if( guid_equal(*(pItem->cid), cid) ) {
				pFound = pItem;
				break;
			}
			pItem ++;
		}
		if( pFound == NULL ) {
			cr.SetResult(GKC::SystemCallResults::NotImpl);
			return ;
		}
		_ShareCom<_IComFactory>* pF = (_ShareCom<_IComFactory>*)(pFound->btFactory);
		if( pFound->flag == 0 ) {
			GKC::CallResult crCreate;
			//lock
			LockMutex();
			//double check
			if( pFound->flag == 0 ) {
				crCreate = pFound->pFunc(*pF);
				// If double checking pF->IsBlockNull(),
				// since the memory allocation and object initialization of *pF is not atomic operation,
				// other thread may obtain the uninitialized object when pF->IsBlockNull() is not true.
				if( crCreate.IsSucceeded() )
					pFound->flag = 1;
			}
			UnlockMutex();
			if( crCreate.IsFailed() ) {
				cr = crCreate;
				return ;
			}
		}
		sp = *pF;
	}
	bool SA_Com_CanUnloadNow() const throw()
	{
		return GetLockCount() == 0;
	}

private:
	volatile int m_iLockCount;
	GKC::Mutex m_mtx;

	static _Com_SA_Factory_Item* get_com_sa_factory_map() throw();

private:
	//noncopyable
	_Com_SA_Module(const _Com_SA_Module&) throw();
	_Com_SA_Module& operator=(const _Com_SA_Module&) throw();
};

// --<header file>--

#define DECLARE_COM_SA_MODULE  DECLARE_SA_GLOBAL_VARIABLE(_Com_SA_Module, _g_com_sa_module)

#define USE_COM_SA_MODULE()  GET_SA_GLOBAL_VARIABLE(_g_com_sa_module)

#define DECLARE_COM_SA_EXPORT_FUNCTIONS  \
SA_FUNCTION void _SA_Com_GetClassObject(const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw();  \
SA_FUNCTION bool _SA_Com_CanUnloadNow() throw();

// use _ComSABase as a base class of component class in SA
#define DECLARE_COM_SA_BASE_CLASS  \
class _ComSABase { public:  \
_ComSABase() throw() { USE_COM_SA_MODULE().LockCount(); }  \
~_ComSABase() throw() { USE_COM_SA_MODULE().UnlockCount(); }  \
private:  /*noncopyable*/  \
_ComSABase(const _ComSABase&) throw();  \
_ComSABase& operator=(const _ComSABase&) throw();  \
};

// --<.h end>---

// --<source file>--

#define IMPLEMENT_COM_SA_MODULE  \
	BEGIN_SA_GLOBAL_VARIABLE(_Com_SA_Module, _g_com_sa_module)  \
	END_SA_GLOBAL_VARIABLE(_g_com_sa_module)

#define BEGIN_COM_SA_FACTORY_MAP()  \
	_Com_SA_Factory_Item l_com_sa_factory_map[] = {

#define COM_SA_FACTORY_ENTRY(com_class)  \
	{ &(USE_GUID(GUID_##com_class)), &(USE_COM_SA_FACTORY_CLASS_NAME(com_class)::Create), { 0 }, 0 },

#define END_COM_SA_FACTORY_MAP()  \
	{ NULL, NULL, { 0 }, 0 } };  \
	_Com_SA_Factory_Item* _Com_SA_Module::get_com_sa_factory_map() throw()  \
	{ return l_com_sa_factory_map; }

#define IMPLEMENT_COM_SA_EXPORT_FUNCTIONS  \
void _SA_Com_GetClassObject(const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw()  \
{ USE_COM_SA_MODULE().SA_Com_GetClassObject(cid, sp, cr); }  \
bool _SA_Com_CanUnloadNow() throw()  \
{ return USE_COM_SA_MODULE().SA_Com_CanUnloadNow(); }

// --<.cpp end>---

/*
USE_COM_SA_MODULE().InitMutex should be called in ProgramEntryPoint::SAMain function.
*/

// component client functions
SA_FUNCTION void _Com_SA_GetClassObject(const _StringS& strAssembly, const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _Com_SA_FreeUnusedLibraries() throw();

// unique com

typedef GKC::CallResult (* _UniqueCom_SA_Create_Func)(_UniqueCom& sp);

// _UniqueCom_SA_Item

struct _UniqueCom_SA_Item
{
	const guid* cid;
	_UniqueCom_SA_Create_Func pFunc;
};

// _UniqueCom_Factory<T>

template <class T>
class _UniqueCom_Factory
{
public:
	static GKC::CallResult Create(_UniqueCom& sp) noexcept
	{
		GKC::CallResult cr;
		_CREATE_UNIQUECOM_INSTANCE(T, sp, cr);
		return cr;
	}
};

// _UniqueCom_SA_Module
//   for global variable in SA of components
class _UniqueCom_SA_Module
{
public:
	_UniqueCom_SA_Module() noexcept
	{
	}
	_UniqueCom_SA_Module(const _UniqueCom_SA_Module&) = delete;
	_UniqueCom_SA_Module& operator=(const _UniqueCom_SA_Module&) = delete;
	~_UniqueCom_SA_Module() noexcept
	{
	}

	void GetClassObject(const guid& cid, _UniqueCom& sp, GKC::CallResult& cr) noexcept
	{
		cr.SetResult(GKC::SystemCallResults::OK);
		_UniqueCom_SA_Item* pItem = get_unique_com_sa_map();
		if ( pItem == NULL ) {
			cr.SetResult(GKC::SystemCallResults::NotImpl);
			return ;
		}
		_UniqueCom_SA_Item* pFound = NULL;
		while ( pItem->cid != NULL ) {
			if ( guid_equal(*(pItem->cid), cid) ) {
				pFound = pItem;
				break;
			}
			pItem ++;
		}
		if ( pFound == NULL ) {
			cr.SetResult(GKC::SystemCallResults::NotImpl);
			return ;
		}
		cr = pFound->pFunc(sp);
	}

private:
	static _UniqueCom_SA_Item* get_unique_com_sa_map() noexcept;
};

// --<header file>--

#define DECLARE_UNIQUECOM_SA_MODULE  extern _UniqueCom_SA_Module _g_unique_com_sa_module;
#define USE_UNIQUECOM_SA_MODULE()    (_g_unique_com_sa_module)

#define DECLARE_UNIQUECOM_SA_EXPORT_FUNCTIONS  \
SA_FUNCTION void _SA_UniqueCom_GetClassObject(const guid& cid, _UniqueCom& sp, GKC::CallResult& cr) noexcept;

// --<.h end>---

// --<source file>--

#define IMPLEMENT_UNIQUECOM_SA_MODULE  \
	_UniqueCom_SA_Module _g_unique_com_sa_module;

#define BEGIN_UNIQUECOM_SA_MAP()  \
	_UniqueCom_SA_Item l_unique_com_sa_map[] = {

#define UNIQUECOM_SA_ENTRY(com_class)  \
	{ &(USE_GUID(GUID_##com_class)), &(_UniqueCom_Factory<com_class>::Create) },

#define END_UNIQUECOM_SA_MAP()  \
	{ NULL, NULL };  \
	_UniqueCom_SA_Item* _UniqueCom_SA_Module::get_unique_com_sa_map() noexcept  \
	{ return l_unique_com_sa_map; }

#define IMPLEMENT_UNIQUECOM_SA_EXPORT_FUNCTIONS  \
void _SA_UniqueCom_GetClassObject(const guid& cid, _UniqueCom& sp, GKC::CallResult& cr) noexcept  \
{ USE_UNIQUECOM_SA_MODULE().GetClassObject(cid, sp, cr); }

// --<.cpp end>---

// client functions
SA_FUNCTION void _UniqueCom_SA_GetClassObject(const GKC::ConstStringS& strAssembly, const guid& cid, _UniqueCom& sp, GKC::CallResult& cr) noexcept;

//------------------------------------------------------------------------------
// Stream

#pragma pack(push, 1)

// _IByteSequentialStream

class NOVTABLE _IByteSequentialStream
{
public:
	virtual GKC::CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) throw() = 0;
	virtual GKC::CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) throw() = 0;
};

DECLARE_GUID(GUID__IByteSequentialStream)

// _IByteStream

class NOVTABLE _IByteStream : public _IByteSequentialStream
{
public:
	virtual GKC::CallResult Commit() throw() = 0;
	virtual GKC::CallResult Seek(const uint& uMethod, const int64& iOffset, int64& iNewPos) throw() = 0;
	virtual GKC::CallResult SetSize(const int64& iSize) throw() = 0;
	virtual GKC::CallResult GetStatus(GKC::StorageStatus& status) throw() = 0;
};

DECLARE_GUID(GUID__IByteStream)

// _IStreamLocker

class NOVTABLE _IStreamLocker
{
public:
	virtual GKC::CallResult Lock(int64 iOffset, int64 iLen, bool bShare) throw() = 0;
	virtual void Unlock(int64 iOffset, int64 iLen) throw() = 0;
};

DECLARE_GUID(GUID__IStreamLocker)

// _IFileUtility
/*! \brief Interface of utility for File Stream.

Interface of utility for File Stream.
*/
class NOVTABLE _IFileUtility
{
public:
	/*! \brief Open a file.

	Open a file.
	\param szFile [in] File name. Use platform path prefix.
	\param iOpenType [in] Open type. It can be a value of
	                      enumeration GKC::FileOpenTypes.
	\param iCreateType [in] Create type. It can be a combination of
	                        enumeration GKC::FileCreationTypes values.
	\return A CallResult value.
	\sa enum GKC::FileOpenTypes, enum GKC::FileCreationTypes.
	*/
	virtual GKC::CallResult Open(const GKC::RefPtr<GKC::CharS>& szFile, const int& iOpenType, const int& iCreateType) throw() = 0;
	/*! \brief Close file.

	Close file.
	*/
	virtual void Close() throw() = 0;
	/*! \brief Check if the file is opened.

	Check if the file is opened.
	\return A boolean value.
	*/
	virtual bool IsOpened() throw() = 0;
};

DECLARE_GUID(GUID__IFileUtility)

// _IMemoryUtility

class NOVTABLE _IMemoryUtility
{
public:
	virtual GKC::CallResult Initialize() throw() = 0;
	virtual void SetArray(const _ShareArray<byte>& sp) throw() = 0;
	virtual _ShareArray<byte> GetArray() throw() = 0;
	virtual GKC::CallResult CloneTo(_ShareCom<_IByteStream>& sp) throw() = 0;
};

DECLARE_GUID(GUID__IMemoryUtility)

// _IMemoryUtilityU
/*! \brief Interface of utility for Memory Stream.

Interface of utility for Memory Stream.
\note The synchronization of these objects must be implemented by user.
*/
class NOVTABLE _IMemoryUtilityU
{
public:
	/*! \brief Attach a data buffer to memory stream.

	Attach a data buffer to memory stream. The length of stream
	will be set to the size of array, the seek pointer will be set to zero.
	\param sp [in, out] Specify a data buffer.
	*/
	virtual void Attach(_UniqueArray<byte>&& sp) noexcept = 0;
	/*! \brief Detach a data buffer from memory stream.

	Detach a data buffer from memory stream.
	\return Data buffer.
	*/
	virtual _UniqueArray<byte> Detach() noexcept = 0;
};

DECLARE_GUID(GUID__IMemoryUtilityU)

// _IBufferUtility
/*! \brief Interface of utility for Buffer Stream.

Interface of utility for Buffer Stream.
\note The synchronization of these objects must be implemented by user.
*/
class NOVTABLE _IBufferUtility
{
public:
	/*! \brief Attach a data buffer.

	Attach a data buffer to stream.
	\param p [in] A pointer to data buffer.
	\param uBytes [in] The bytes of buffer.
	\return A CallResult value.
	*/
	virtual GKC::CallResult SetBuffer(const uintptr& p, const uintptr& uBytes) throw() = 0;
};

DECLARE_GUID(GUID__IBufferUtility)

//for text stream

// _BOMTypes
/*! \brief An enumerator for text stream encoding BOM types.

An enumerator for text stream encoding BOM types.
*/
BEGIN_ENUM(_BOMTypes)
	ENUM_VALUE_ENTRY(None, 0)     //!< Invalid type
	ENUM_VALUE_ENTRY(Ansi, 1)     //!< ANSI text
	ENUM_VALUE_ENTRY(UTF8, 2)     //!< UTF8 text (default)
	ENUM_VALUE_ENTRY(UTF16LE, 3)  //!< UTF16 little endian text
	ENUM_VALUE_ENTRY(UTF16BE, 4)  //!< UTF16 big endian text
	ENUM_VALUE_ENTRY(UTF32LE, 5)  //!< UTF32 little endian text
	ENUM_VALUE_ENTRY(UTF32BE, 6)  //!< UTF32 big endian text
END_ENUM()

// _CRLFStyles
/*! \brief An enumerator for text stream styles of CRLF.

An enumerator for text stream styles of CRLF.
*/
BEGIN_ENUM(_CRLFStyles)
	ENUM_VALUE_ENTRY(Windows, 0)  //!< windows
	ENUM_VALUE_ENTRY(Unix, 1)     //!< unix and linux
	ENUM_VALUE_ENTRY(Mac, 2)      //!< mac
	ENUM_VALUE_ENTRY(Max, 3)
END_ENUM()

// _ITextStreamRoot

class NOVTABLE _ITextStreamRoot
{
public:
	virtual void Reset() throw() = 0;
	/*! \brief Read the BOM and decide the text encoding type.

	Read the BOM (Byte Order Mark) and decide the text encoding type.
	\param iType [out] Receive the text encoding type. See enum GKC::BOMTypes.
	\return A CallResult value.
	\note The text type may be GKC::BOMTypes::None.
	*/
	virtual GKC::CallResult CheckBOM(int& iType) throw() = 0;
	/*! \brief Set the text encoding BOM type.

	Set the text encoding BOM type.
	\param iType [in] Specify the text encoding BOM type. See enum GKC::BOMTypes.
	*/
	virtual void SetBOM(const int& iType) throw() = 0;
	/*! \brief Get the text encoding BOM type.

	Get the text encoding BOM type.
	\return The text encoding BOM type. See enum GKC::BOMTypes.
	*/
	virtual int GetBOM() throw() = 0;
	/*! \brief Set the style of CRLF.

	Set the style of CRLF.
	\param iStyle [in] Specify the style. See enum GKC::CRLFStyles.
	*/
	virtual void SetCRLFStyle(const int& iStyle) throw() = 0;
	/*! \brief Get the style of CRLF.

	Get the style of CRLF.
	\return The style value. See enum GKC::CRLFStyles.
	*/
	virtual int GetCRLFStyle() throw() = 0;
	/*! \brief Get a character.

	Get a character.
	\param ch [out] Receive the character.
	\return A CallResult value. The return value SystemCallResults::S_EOF means the end of file is reached.
	*/
	virtual GKC::CallResult GetCharA(GKC::CharA& ch) throw() = 0;
	/*! \brief Unget characters.

	Unget characters.
	\param iCharNum [in] The number of characters.
	\return A CallResult value.
	\note This method must be called by correct text stream type.
	*/
	virtual GKC::CallResult UngetCharA(const int64& iCharNum) throw() = 0;
	virtual GKC::CallResult GetCharH(GKC::CharH& ch) throw() = 0;
	virtual GKC::CallResult UngetCharH(const int64& iCharNum) throw() = 0;
	virtual GKC::CallResult GetCharL(GKC::CharL& ch) throw() = 0;
	virtual GKC::CallResult UngetCharL(const int64& iCharNum) throw() = 0;
	virtual GKC::CallResult GetChar(GKC::CharF& ch) throw() = 0;
	virtual GKC::CallResult UngetChar(const int64& iCharNum) throw() = 0;
	/*! \brief Write the BOM.

	Write the BOM.
	\return A CallResult value.
	*/
	virtual GKC::CallResult WriteBOM() throw() = 0;
	virtual GKC::CallResult MoveToEnd() throw() = 0;
	/*! \brief Put a character.

	Put a character.
	\param ch [in] The character.
	\return A CallResult value.
	*/
	virtual GKC::CallResult PutCharA(const GKC::CharA& ch) throw() = 0;
	virtual GKC::CallResult PutCharH(const GKC::CharH& ch) throw() = 0;
	virtual GKC::CallResult PutCharL(const GKC::CharL& ch) throw() = 0;
	virtual GKC::CallResult PutChar(const GKC::CharF& ch) throw() = 0;
	/*! \brief Write a string.

	Write a string.
	\param str [in] The string to be written.
	\return A CallResult value.
	*/
	virtual GKC::CallResult PutStringA(const GKC::ConstStringA& str) throw() = 0;
	virtual GKC::CallResult PutStringH(const GKC::ConstStringH& str) throw() = 0;
	virtual GKC::CallResult PutStringL(const GKC::ConstStringL& str) throw() = 0;
	virtual GKC::CallResult PutNewLineA() throw() = 0;
	virtual GKC::CallResult PutNewLineH() throw() = 0;
	virtual GKC::CallResult PutNewLineL() throw() = 0;
	virtual GKC::CallResult PutNewLine() throw() = 0;
	/*! \brief Write a line string.

	Write a line string.
	\param str [in] The string to be written.
	\return A CallResult value.
	*/
	virtual GKC::CallResult PutLineA(const GKC::ConstStringA& str) throw() = 0;
	virtual GKC::CallResult PutLineH(const GKC::ConstStringH& str) throw() = 0;
	virtual GKC::CallResult PutLineL(const GKC::ConstStringL& str) throw() = 0;
};

DECLARE_GUID(GUID__ITextStreamRoot)

// _ITextStreamString

class NOVTABLE _ITextStreamString
{
public:
	virtual GKC::CallResult GetAllStringA(_StringA& str) throw() = 0;
	virtual GKC::CallResult GetAllStringH(_StringH& str) throw() = 0;
	virtual GKC::CallResult GetAllStringL(_StringL& str) throw() = 0;
	virtual GKC::CallResult GetAllString(_VariantString& str) throw() = 0;
	// The return value SystemCallResults::S_EOF means the end of file is reached.
	virtual GKC::CallResult GetStringA(_StringA& str) throw() = 0;
	virtual GKC::CallResult GetStringH(_StringH& str) throw() = 0;
	virtual GKC::CallResult GetStringL(_StringL& str) throw() = 0;
	virtual GKC::CallResult GetString(_VariantString& str) throw() = 0;
	virtual GKC::CallResult GetLineA(_StringA& str) throw() = 0;
	virtual GKC::CallResult GetLineH(_StringH& str) throw() = 0;
	virtual GKC::CallResult GetLineL(_StringL& str) throw() = 0;
	virtual GKC::CallResult GetLine(_VariantString& str) throw() = 0;
	//put string
	virtual GKC::CallResult PutString(const _VariantString& str) throw() = 0;
	virtual GKC::CallResult PutLine(const _VariantString& str) throw() = 0;
};

DECLARE_GUID(GUID__ITextStreamString)

// _ITextUtility

class NOVTABLE _ITextUtility
{
public:
	/*! \brief Set stream object.

	Set stream object.
	\param rp [in] The shared interface pointer to stream. It can not be a NULL object.
	*/
	virtual void SetStream(const _ShareCom<_IByteStream>& sp) throw() = 0;
	/*! \brief Set the separate character set string.

	Set the separate character set string.
	\param arr [in] The string for separate character set. If this value
	                is NULL string, the default separate string will be set.
	\note Do not include '\r' and '\n' in this set.
	*/
	virtual void SetSeparatorSetA(const _ShareArray<GKC::CharA>& arr) throw() = 0;
	virtual void SetSeparatorSetH(const _ShareArray<GKC::CharH>& arr) throw() = 0;
	virtual void SetSeparatorSetL(const _ShareArray<GKC::CharL>& arr) throw() = 0;
};

DECLARE_GUID(GUID__ITextUtility)

// _ITextStreamStringU

class NOVTABLE _ITextStreamStringU
{
public:
	/*! \brief Get all string.

	Get all string.
	\param str [out] Receive the string.
	\return A CallResult value.
	*/
	virtual GKC::CallResult GetAllStringA(_UniqueStringA& str) noexcept = 0;
	virtual GKC::CallResult GetAllStringH(_UniqueStringH& str) noexcept = 0;
	virtual GKC::CallResult GetAllStringL(_UniqueStringL& str) noexcept = 0;
	/*! \brief Get a string.

	Get a string.
	\param str [out] Receive the string.
	\return A CallResult value. The return value SystemCallResults::S_EOF means the end of file is reached.
	*/
	virtual GKC::CallResult GetStringA(_UniqueStringA& str) noexcept = 0;
	virtual GKC::CallResult GetStringH(_UniqueStringH& str) noexcept = 0;
	virtual GKC::CallResult GetStringL(_UniqueStringL& str) noexcept = 0;
	/*! \brief Get a line string.

	Get a line string.
	\param str [out] Receive the string.
	\return A CallResult value. The return value SystemCallResults::S_EOF means the end of file is reached.
	*/
	virtual GKC::CallResult GetLineA(_UniqueStringA& str) noexcept = 0;
	virtual GKC::CallResult GetLineH(_UniqueStringH& str) noexcept = 0;
	virtual GKC::CallResult GetLineL(_UniqueStringL& str) noexcept = 0;
};

DECLARE_GUID(GUID__ITextStreamStringU)

// _ITextUtilityU

class NOVTABLE _ITextUtilityU
{
public:
	/*! \brief Attach stream object.

	Attach stream object.
	\param rp [in] The interface pointer to stream. It can not be a NULL object.
	*/
	virtual void AttachStream(const GKC::RefPtr<_IByteStream>& rp) noexcept = 0;
	/*! \brief Detach stream object.

	Detach stream object.
	\return The interface pointer to stream.
	*/
	virtual GKC::RefPtr<_IByteStream> DetachStream() noexcept = 0;
	/*! \brief Set the separate character set string.

	Set the separate character set string.
	\param arr [in, out] The string for separate character set. If this value
	                     is NULL string, the default separate string will be set.
	\note Do not include '\r' and '\n' in this set.
	*/
	virtual void SetSeparatorSetA(_UniqueArray<GKC::CharA>&& arr) noexcept = 0;
	virtual void SetSeparatorSetH(_UniqueArray<GKC::CharH>&& arr) noexcept = 0;
	virtual void SetSeparatorSetL(_UniqueArray<GKC::CharL>&& arr) noexcept = 0;
};

DECLARE_GUID(GUID__ITextUtilityU)

#pragma pack(pop)

// _Initialize_VariantString

inline void _Initialize_VariantString(int iBomType, _VariantString& str)
{
	if( iBomType == _BOMTypes::UTF16LE || iBomType == _BOMTypes::UTF16BE )
		str.SetType(_VariantString::Char16);
	else if( iBomType == _BOMTypes::UTF32LE || iBomType == _BOMTypes::UTF32BE )
		str.SetType(_VariantString::Char32);
	else if( iBomType == _BOMTypes::UTF8 || iBomType == _BOMTypes::Ansi )
		str.SetType(_VariantString::Char8);
	else
		str.SetType(_VariantString::None);
	str.Reset();  //may throw
}

//functions

// use platform path prefix
SA_FUNCTION void _FileStream_Create(const GKC::CharS* szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _MemoryStream_Create(_ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _BufferStream_Create(const void* p, uintptr uBytes, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _TextStream_Create(_ShareCom<_ITextStreamRoot>& sp, GKC::CallResult& cr) throw();

SA_FUNCTION void _FileStream_CreateU(const GKC::CharS* szFile, int iOpenType, int iCreateType, _UniqueCom& sp, GKC::CallResult& cr) noexcept;
SA_FUNCTION void _MemoryStreamU_CreateU(_UniqueCom& sp, GKC::CallResult& cr) noexcept;
SA_FUNCTION void _BufferStream_CreateU(const void* p, uintptr uBytes, _UniqueCom& sp, GKC::CallResult& cr) noexcept;
SA_FUNCTION void _TextStreamU_CreateU(_UniqueCom& sp, GKC::CallResult& cr) noexcept;

//------------------------------------------------------------------------------
// Help Authoring

#pragma pack(push, 1)

struct _HelpLanguageInfo
{
	GKC::ConstStringS strShortString;
	GKC::ConstStringS strCharset;
	GKC::ConstStringS strCodePage;
	uint uLCID;
};

#pragma pack(pop)

SA_FUNCTION bool _HelpAuthoring_FindCodePageFromCharset(const GKC::ConstStringS& strCharset, GKC::ConstStringS& strCodePage) throw();
SA_FUNCTION bool _HelpAuthoring_FindLCID(uint uLCID, _HelpLanguageInfo& info) throw();
SA_FUNCTION bool _HelpAuthoring_FindShortString(const GKC::ConstStringS& strShortString, _HelpLanguageInfo& info) throw();

//------------------------------------------------------------------------------
// compression

#pragma pack(push, 1)

// _PkInputBuffer

struct _PkInputBuffer
{
	uintptr pBuffer;
	ushort  uLength;
};

// _PkOutputBuffer

struct _PkOutputBuffer
{
	uintptr pBuffer;
	ushort  uLength;
};

// _IPkCompressor

class NOVTABLE _IPkCompressor
{
public:
	virtual void Initialize() noexcept = 0;
	virtual void SetLongestSearch(bool b) noexcept = 0;
	virtual void GetInputBuffer(_PkInputBuffer& buf) noexcept = 0;
	virtual void SetInputSize(ushort uSize) noexcept = 0;
	virtual void GetOutputBuffer(_PkOutputBuffer& buf) noexcept = 0;
	virtual void ResetOutputBuffer() noexcept = 0;
	//return : 1 -- need input, 2 -- output is full
	virtual int Encode() noexcept = 0;
	virtual void StartFinish() noexcept = 0;
	//return : 0 -- end, 2 -- output is full
	virtual int ProcessFinish() noexcept = 0;
};

DECLARE_GUID(GUID__IPkCompressor)

// _IPkDecompressor

class NOVTABLE _IPkDecompressor
{
public:
	virtual void Initialize() noexcept = 0;
	virtual void GetInputBuffer(_PkInputBuffer& buf) noexcept = 0;
	virtual void SetInputSize(ushort uSize) noexcept = 0;
	virtual void GetOutputBuffer(_PkOutputBuffer& buf) noexcept = 0;
	virtual void ResetOutputBuffer() noexcept = 0;
	// return 0 -- end  1 -- need input  2 -- output is full
	virtual int Decode() noexcept = 0;
};

DECLARE_GUID(GUID__IPkDecompressor)

#pragma pack(pop)

//functions

SA_FUNCTION void _PkzCompressor_Create(_UniqueCom& sp, GKC::CallResult& cr) noexcept;
SA_FUNCTION void _PkzDecompressor_Create(_UniqueCom& sp, GKC::CallResult& cr) noexcept;

//------------------------------------------------------------------------------

#include "_LiteCom.h"

//------------------------------------------------------------------------------
// logger

typedef GKC::RefPtr<_IByteStream> (*_LoggerCallback)();  //noexcept

SA_FUNCTION void _Logger_SetCallback(_LoggerCallback p) noexcept;
SA_FUNCTION void _Logger_Output(const GKC::CharA* szFile, int iLine, const GKC::CharA* szMessage) noexcept;

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////
