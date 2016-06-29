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
	bool IsBlockNull() const throw()
	{
		return m_pB == NULL;
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
			if( src.m_pB != baseClass::m_pB ) {
				//release
				T* pT = static_cast<T*>(this);
				pT->Release();
				//move
				_SObjHelper::MoveDirect((void*&)(baseClass::m_pB), (void*&)(src.m_pB));
				pT->do_move(rv_forward(src));
			}
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
	_ShareSoloBase() throw() : m_pT(NULL)
	{
	}
	_ShareSoloBase(const _ShareSoloBase& src) throw() : _SHARESOLOBASE_BASE_CLASS(static_cast<const _SHARESOLOBASE_BASE_CLASS&>(src)), m_pT(src.m_pT)
	{
	}
	_ShareSoloBase(_ShareSoloBase&& src) throw() : _SHARESOLOBASE_BASE_CLASS(rv_forward(static_cast<_SHARESOLOBASE_BASE_CLASS&>(src)))
	{
		_SObjHelper::MoveDirect((void*&)(m_pT), (void*&)(src.m_pT));
	}
	~_ShareSoloBase() throw()
	{
		Release();
	}

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
	void* m_pT;  //A pointer to object

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

template <typename T>
class _SharePtr : public _ShareSoloBase<share_ptr_block>
{
private:
	typedef _ShareSoloBase<share_ptr_block>  baseClass;
	typedef _SharePtr<T>  thisClass;

public:
	_SharePtr() throw()
	{
	}
	_SharePtr(const _SharePtr& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_SharePtr(_SharePtr&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_SharePtr() throw()
	{
	}

	_SharePtr<T>& operator=(const _SharePtr<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
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
}

// --<header file>--

#define DECLARE_COM_TYPECAST(cls)  \
	DECLARE_STATIC_CONST_ARRAY(com_ca_##cls, _Com_Interface_Offset_Item)  \
	class _Com_TC_##cls {  \
	public:  \
		BEGIN_NOINLINE  \
		static void* _Com_TypeCast(void* p, const guid& iid) throw()  \
		END_NOINLINE  \
		{  const _Com_Interface_Offset_Item* pItem = com_ca_##cls ::GetAddress();  \
		uintptr uCount = com_ca_##cls ::GetCount();  \
		for( uintptr i = 0; i < uCount; i ++ ) {  \
			if( guid_equal(*(pItem->pid), iid) ) {  \
				return (void*)((byte*)p + pItem->offset); }  \
			pItem ++; }  \
		return NULL; }  \
	};  \
	template <> class _Com_TypeCast_Func<cls> { public:  \
		static const share_object_typecast_func c_func;  \
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
	const share_object_typecast_func _Com_TypeCast_Func<com_x_class>::c_func = &(com_tc_class::_Com_TypeCast);

// --<.cpp end>--

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
	_ShareArray() throw()
	{
	}
	_ShareArray(const _ShareArray& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_ShareArray(_ShareArray&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_ShareArray() throw()
	{
	}

	_ShareArray<T>& operator=(const _ShareArray<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
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

	const Iterator operator[](uintptr index) const throw()
	{
		return GetAt(index);
	}
	Iterator operator[](uintptr index) throw()
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
	const Iterator GetAtPosition(const Position& pos) const throw()
	{
		return Iterator(GKC::RefPtr<T>(get_array_address() + pos.GetIndex()));
	}
	Iterator GetAtPosition(const Position& pos) throw()
	{
		return Iterator(GKC::RefPtr<T>(get_array_address() + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		return Position(GKC::RefPtrHelper::GetInternalPointer(iter.get_Ref()) - get_array_address());
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(GKC::RefPtr<T>(get_array_address()));
	}
	Iterator GetBegin() throw()
	{
		return Iterator(GKC::RefPtr<T>(get_array_address()));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(GKC::RefPtr<T>(get_array_address() + GetCount()));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(GKC::RefPtr<T>(get_array_address() + GetCount()));
	}
	const Iterator GetReverseBegin() const throw()
	{
		return GKC::ReverseIterator<Iterator>(GetEnd());
	}
	Iterator GetReverseBegin() throw()
	{
		return GKC::ReverseIterator<Iterator>(GetEnd());
	}
	const Iterator GetReverseEnd() const throw()
	{
		return GKC::ReverseIterator<Iterator>(GetBegin());
	}
	Iterator GetReverseEnd() throw()
	{
		return GKC::ReverseIterator<Iterator>(GetBegin());
	}

	const Iterator GetAt(uintptr index) const throw()
	{
		assert( index < GetCount() );
		return Iterator(GKC::RefPtr<T>(get_array_address() + index));
	}
	Iterator GetAt(uintptr index) throw()
	{
		assert( index < GetCount() );
		return Iterator(GKC::RefPtr<T>(get_array_address() + index));
	}
	void SetAt(uintptr index, const T& t)  //may throw
	{
		assert( index < GetCount() );
		get_array_address()[index] = t;
	}
	void SetAt(uintptr index, T&& t)  //may throw
	{
		assert( index < GetCount() );
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
		if( uSize == pB->GetAllocLength() )
			return ;
		// shrink to desired size
		if( uSize == 0 ) {
			//free
			pB->DestroyArray<T>();
			return ;
		}
		uintptr uBytes;
		GKC::SafeOperators::Multiply(uSize, (uintptr)(sizeof(T)), uBytes);  //no check
		T* pNew = (T*)(pB->GetMemoryManager()->Reallocate((uintptr)get_array_address(), uBytes));
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
		SetCount(uElement + 1, 0, rv_forward<Args>(args)...);
		return GetAt(uElement);
	}
	void Append(const _ShareArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		assert( !(baseClass::IsBlockNull()) );
		share_array_block* pB = static_cast<share_array_block*>(baseClass::m_pB);
		uintptr uOldSize = pB->GetLength();
		SetCount(uOldSize + src.GetCount(), 0);
		copy_elements(src.get_array_address(), get_array_address() + uOldSize, src.GetCount());
	}

	void Copy(const _ShareArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		uintptr uSize = src.GetCount();
		SetCount(uSize, 0);
		copy_elements(src.get_array_address(), get_array_address(), uSize);
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
			relocate_elements(get_array_address() + index, get_array_address() + (index + count), uOldSize - index);
			try {
				// re-init slots we copied from
				call_array_constructors(get_array_address() + index, count, rv_forward<Args>(args)...);
			}
			catch(...) {
				relocate_elements(get_array_address() + (index + count), get_array_address() + index, uOldSize - index);
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
			for( uintptr i = 0; i < uSize; i ++ ) {
				SetAt(index + i, src[i].get_Value());
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
			relocate_elements(get_array_address() + uCut, get_array_address() + index, uMoveCount);
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

	template <typename... Args>
	static void call_array_constructors(T* p, uintptr size, Args&&... args)
	{
		uintptr i;
		try {
			for( i = 0; i < size; i ++ ) {
				call_constructor(*(p + i), rv_forward<Args>(args)...);
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
		T* pT = p;
		for( uintptr i = 0; i < size; i ++ ) {
			pT->~T();
			++ pT;
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
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	typename thisClass::Iterator GetEnd() throw()
	{
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + GetLength()));
	}
	const typename thisClass::Iterator GetReverseBegin() const throw()
	{
		return GKC::ReverseIterator<typename thisClass::Iterator>(GetEnd());
	}
	typename thisClass::Iterator GetReverseBegin() throw()
	{
		return GKC::ReverseIterator<typename thisClass::Iterator>(GetEnd());
	}

	const typename thisClass::Iterator GetAt(uintptr index) const throw()
	{
		assert( index < GetLength() );
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + index));
	}
	typename thisClass::Iterator GetAt(uintptr index) throw()
	{
		assert( index < GetLength() );
		return typename thisClass::Iterator(GKC::RefPtr<Tchar>(baseClass::get_array_address() + index));
	}
	void SetAt(uintptr index, const Tchar& t)  //may throw
	{
		assert( index < GetLength() );
		baseClass::get_array_address()[index] = t;
	}
	void SetAt(uintptr index, Tchar&& t)  //may throw
	{
		assert( index < GetLength() );
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

	//append character
	template <typename Tchar>
	static void Append(const Tchar& ch, INOUT _StringT<Tchar>& strDest)
	{
		uintptr uCount = strDest.GetLength();
		uintptr uNew = GKC::SafeOperators::AddThrow(uCount, (uintptr)1);  //may throw
		strDest.SetLength(uNew);  //may throw
		strDest.SetAt(uCount, ch);
	}
	//insert character
	template <typename Tchar>
	static void Insert(uintptr uPos, const Tchar& ch, _StringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		if( uPos > uLength )
			return ;
		str.InsertAt(uPos, 1, ch);
	}
	//delete
	template <typename Tchar>
	static void Delete(uintptr uPos, uintptr uLength, _StringT<Tchar>& str) throw()
	{
		uintptr uCount = str.GetLength();
		uintptr uRet = calc_sub_string_act_length(uCount, uPos, uLength);
		if( uRet == 0 )
			return ;
		str.RemoveAt(uPos, uRet);
	}

	//replace
	template <typename Tchar, class TCompareTrait = GKC::DefaultCompareTrait<Tchar>>
	static uintptr Replace(const Tchar& chOld, const Tchar& chNew, INOUT _StringT<Tchar>& str) throw()
	{
		assert( chOld != 0 && chNew != 0 && TCompareTrait::IsNE(chOld, chNew) );
		if( str.IsEmpty() )
			return 0;
		return coll_replace_elements<typename _StringT<Tchar>::Iterator, TCompareTrait>(chOld, chNew, str.GetBegin(), str.GetEnd());
	}

	//find (return value : check null)
	template <typename Tchar>
	static typename _StringT<Tchar>::Iterator Find(const _StringT<Tchar>& str, const Tchar& ch, uintptr uStart) throw()
	{
		assert( uStart <= str.GetLength() );
		assert( !str.IsBlockNull() );
		return typename _StringT<Tchar>::Iterator(GKC::RefPtr<Tchar>(find_string_char(_ShareArrayHelper::GetInternalPointer(str) + uStart, ch)));
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

	template <typename Tchar>
	static void Insert(uintptr uPos, const GKC::ConstStringT<Tchar>& strAdd, _StringT<Tchar>& str)
	{
		uintptr uLength = str.GetLength();
		uintptr uAdd = strAdd.GetCount();
		if( uAdd == 0 || uPos > uLength )
			return ;
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
		str.InsertAt(uPos, uAdd);
		mem_copy(_ShareArrayHelper::GetInternalPointer(strAdd), uAdd * sizeof(Tchar), _ShareArrayHelper::GetInternalPointer(str) + uPos);
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
};

#pragma pack(pop)

//------------------------------------------------------------------------------
// Component

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

// create component
//   return : CallResult (no throw)
template <class T>
inline GKC::CallResult _Create_Component_Instance(_ShareCom<T>& sp) throw()
{
	GKC::CallResult cr;
	try {
		_ShareCom<T> spC(_ShareComHelper::MakeShareCom<T>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get())));
		sp = spC;
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

#define _COMPONENT_INSTANCE_INTERFACE(com_type, if_type, spC, sp, cr)  \
	do { _ShareCom<void> __sp_I__;  \
		cr = _Component_Instance_Query<com_type>(spC, USE_GUID(GUID_##if_type), __sp_I__);  \
		if( cr.IsSucceeded() )  \
			sp = _ShareComHelper::TypeCast<void, if_type>(__sp_I__);  \
	} while(0)

#define _CREATE_COMPONENT_INSTANCE(com_type, if_type, sp, cr)  \
	do { _ShareCom<com_type> __sp_C__;  \
	cr = _Create_Component_Instance<com_type>(__sp_C__);  \
	if( cr.IsSucceeded() ) {  \
		_COMPONENT_INSTANCE_INTERFACE(com_type, if_type, __sp_C__, sp, cr);  \
	} } while(0)

typedef GKC::CallResult (* _Com_SA_Create_Factory_Func)(_ShareCom<_IComFactory>& sp);

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
		_ShareCom<void> spI;  \
		cr = _Component_Instance_Query<com_class>(spC, iid, spI);  \
		if( cr.IsSucceeded() ) sp = spI;  \
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
		if( pF->IsBlockNull() ) {
			GKC::CallResult crCreate;
			//lock
			LockMutex();
			//double check
			if( pF->IsBlockNull() )
				crCreate = pFound->pFunc(*pF);
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
	{ &(USE_GUID(GUID_##com_class)), &(USE_COM_SA_FACTORY_CLASS_NAME(com_class)::Create), { 0 } },

#define END_COM_SA_FACTORY_MAP()  \
	{ NULL, NULL, { 0 } } };  \
	_Com_SA_Factory_Item* _Com_SA_Module::get_com_sa_factory_map() throw()  \
	{ return l_com_sa_factory_map; }

#define IMPLEMENT_COM_SA_EXPORT_FUNCTIONS  \
void _SA_Com_GetClassObject(const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw()  \
{ USE_COM_SA_MODULE().SA_Com_GetClassObject(cid, sp, cr); }  \
bool _SA_Com_CanUnloadNow() throw()  \
{ return USE_COM_SA_MODULE().SA_Com_CanUnloadNow(); }

// --<.cpp end>---

// component client functions
SA_FUNCTION void _Com_SA_GetClassObject(const _StringS& strAssembly, const guid& cid, _ShareCom<_IComFactory>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _Com_SA_FreeUnusedLibraries() throw();

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

// _IFileUtility

class NOVTABLE _IFileUtility
{
public:
	// use platform path prefix
	virtual GKC::CallResult Open(const GKC::RefPtr<GKC::CharS>& szFile, const int& iOpenType, const int& iCreateType) throw() = 0;
	virtual void Close() throw() = 0;
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

// _IBufferUtility

class NOVTABLE _IBufferUtility
{
public:
	virtual GKC::CallResult SetBuffer(const uintptr& p, const uintptr& uBytes) throw() = 0;
};

DECLARE_GUID(GUID__IBufferUtility)

// _BOMTypes

BEGIN_ENUM(_BOMTypes)
	ENUM_VALUE_ENTRY(None, 0)
	ENUM_VALUE_ENTRY(Ansi, 1)
	ENUM_VALUE_ENTRY(UTF8, 2)
	ENUM_VALUE_ENTRY(UTF16LE, 3)
	ENUM_VALUE_ENTRY(UTF16BE, 4)
	ENUM_VALUE_ENTRY(UTF32LE, 5)
	ENUM_VALUE_ENTRY(UTF32BE, 6)
END_ENUM()

// _ITextStream

class NOVTABLE _ITextStream
{
public:
	virtual void SetStream(const _ShareCom<_IByteStream>& sp) throw() = 0;
	virtual GKC::CallResult CheckBOM(int& iType) throw() = 0;
	// The return value SystemCallResults::S_EOF means the end of file is reached.
	virtual GKC::CallResult GetCharA(GKC::CharA& ch) throw() = 0;
	virtual GKC::CallResult UngetCharA(const int64& iCharNum) throw() = 0;
	virtual GKC::CallResult GetCharH(GKC::CharH& ch) throw() = 0;
	virtual GKC::CallResult UngetCharH(const int64& iCharNum) throw() = 0;
	virtual GKC::CallResult GetCharL(GKC::CharL& ch) throw() = 0;
	virtual GKC::CallResult UngetCharL(const int64& iCharNum) throw() = 0;
};

DECLARE_GUID(GUID__ITextStream)

#pragma pack(pop)

//functions

// use platform path prefix
SA_FUNCTION void _FileStream_Create(const GKC::CharS* szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _MemoryStream_Create(_ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _BufferStream_Create(const void* p, uintptr uBytes, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw();
SA_FUNCTION void _TextStream_Create(_ShareCom<_ITextStream>& sp, GKC::CallResult& cr) throw();

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////
