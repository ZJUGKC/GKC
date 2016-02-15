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
		_SObjHelper::MoveDirect(m_pB, src.m_pB);
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
		_SObjHelper::MoveDirect(m_pB, src.m_pB);
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

	thusClass& operator=(const thisClass& src) throw()
	{
		if( &src != this ) {
			if( src.m_pB != m_pB ) {
				//release
				T* pT = static_cast<T*>(this);
				pT->Release();
				//assign
				m_pB = src.m_pB;
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
			if( src.m_pB != m_pB ) {
				//release
				T* pT = static_cast<T*>(this);
				pT->Release();
				//move
				_SObjHelper::MoveDirect(m_pB, src.m_pB);
				pT->do_move(src);
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
	typedef _SObjT<_ShareSoloBase<TBlock>, _ShareObjectBase>  baseClass;
	typedef _ShareSoloBase<TBlock>  thisClass;

protected:
	_ShareSoloBase() throw() : m_pT(NULL)
	{
	}
	_ShareSoloBase(const _ShareSoloBase& src) throw() : baseClass(static_cast<const baseClass&>(src)), m_pT(src.m_pT)
	{
	}
	_ShareSoloBase(_ShareSoloBase&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
		_SObjHelper::MoveDirect(m_pT, src.m_pT);
	}
	~_ShareSoloBase() throw()
	{
		Release();
	}

	_ShareSoloBase<TBlock>& operator=(const _ShareSoloBase<TBlock>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_ShareSoloBase<TBlock>& operator=(_ShareSoloBase<TBlock>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( m_pB != NULL ) {
			_SObjRelease<TBlock>::ShareRelease(m_pB);
			m_pB = NULL;
			m_pT = NULL;
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

protected:
	void do_assign(const baseClass& src) throw()
	{
		m_pT = (static_cast<const thisClass&>(src)).m_pT;
	}
	void do_move(baseClass&& src) throw()
	{
		_SObjHelper::MoveDirect(m_pT, (static_cast<thisClass&>(src)).m_pT);
	}

protected:
	void* m_pT;  //A pointer to object

private:
	friend class baseClass;
};

// _WeakSoloBase<TBlock>

template <class TBlock>
class _WeakSoloBase : public _SObjT<_WeakSoloBase<TBlock>, _WeakObjectBase>
{
private:
	typedef _SObjT<_WeakSoloBase<TBlock>, _WeakObjectBase>  baseClass;
	typedef _WeakSoloBase<TBlock>  thisClass;

protected:
	_WeakSoloBase() throw() : m_pT(NULL)
	{
	}
	_WeakSoloBase(const _WeakSoloBase& src) throw() : baseClass(static_cast<const baseClass&>(src)), m_pT(src.m_pT)
	{
	}
	_WeakSoloBase(_WeakSoloBase&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
		_SObjHelper::MoveDirect(m_pT, src.m_pT);
	}
	~_WeakSoloBase() throw()
	{
		Release();
	}

	_WeakSoloBase<TBlock>& operator=(const _WeakSoloBase<TBlock>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_WeakSoloBase<TBlock>& operator=(_WeakSoloBase<TBlock>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( m_pB != NULL ) {
			//weak
			_SObjRelease<TBlock>::WeakRelease(m_pB);
			m_pB = NULL;
			m_pT = NULL;
		}
		else {
			assert( m_pT == NULL );
		} //end if
	}

protected:
	void do_assign(const baseClass& src) throw()
	{
		m_pT = (static_cast<const thisClass&>(src)).m_pT;
	}
	void do_move(baseClass&& src) throw()
	{
		_SObjHelper::MoveDirect(m_pT, (static_cast<thisClass&>(src)).m_pT);
	}

protected:
	void* m_pT;  //A pointer to object

private:
	friend class baseClass;
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
	friend class _SharePtrHelper;
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
};

// _SharePtrHelper

class _SharePtrHelper
{
protected:
	template <typename T>
	BEGIN_NOINLINE
	static void object_destruction(void* p) throw()
	END_NOINLINE
	{
		((T*)p)->~T();
	}

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

		_SharePtr<T> ret;
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
		pB->SetMemoryManager(mgr);
		pB->SetAddress(pT);
		pB->SetDestructionFunc(&(object_destruction<T>));

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
				? ( assert( sp.m_pT != NULL ), MakeSharePtr((sp.m_pB)->GetMemoryManager(), sp.Deref()) )
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
	friend class _ShareComHelper;
};

// --only one component class can be defined in a pair of .h and .cpp files.--

struct _Com_Interface_Offset_Item
{
	guid*   pid;
	intptr  offset;
};

template <class T>
class _Com_TypeCast_Func
{
};

// --<header file>--

#define DECLARE_COM_TYPECAST(cls)  \
	DECLARE_STATIC_CONST_ARRAY(com_ca_##cls, _Com_Interface_Offset_Item)  \
	class com_tc_##cls {  \
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
	typedef com_tc_##cls  com_tc_class;  \
	BEGIN_STATIC_CONST_ARRAY(cls)

// ifname : interface name, ibname : implemented base class name
#define COM_TYPECAST_ENTRY(ifname, ibname)  \
	BEGIN_STATIC_CONST_ARRAY_GROUP()  \
		STATIC_CONST_ARRAY_ENTRY(&(USE_GUID(GUID_##ifname)))  \
		STATIC_CONST_ARRAY_ENTRY_LAST( (intptr)( (byte*)(static_cast<ibname*>((com_x_class*)(0x128))) - (byte*)(0x128) ) )  \
	END_STATIC_CONST_ARRAY_GROUP()

#define END_COM_TYPECAST()  \
	BEGIN_STATIC_CONST_ARRAY_GROUP()  \
		STATIC_CONST_ARRAY_ENTRY(NULL)  \
		STATIC_CONST_ARRAY_ENTRY_LAST(0)  \
	END_STATIC_CONST_ARRAY_GROUP_LAST()  \
	END_STATIC_CONST_ARRAY(com_x_class)  \
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

		_ShareCom<T> ret;
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
		pB->SetMemoryManager(mgr);
		pB->SetAddress(pT);
		pB->SetDestructionFunc(&(object_destruction<T>));
		pB->SetTypeCastFunc(_Com_TypeCast_Func<T>::c_func);

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
	template <class TSrc, class TDest>
	static _ShareCom<TDest> Query(const _ShareCom<TSrc>& sp, const guid& iid) throw()
	{
		_ShareCom<TDest> ret;
		ret.m_pB = sp.m_pB;
		if( ret.m_pB != NULL ) {
			assert( !((ret.m_pB)->IsNull()) );
			ret.m_pT = (TDest*)(((static_cast<share_com_block*>(ret.m_pB))->GetTypeCastFunc())((ret.m_pB)->GetAddress(), iid));
			if( ret.m_pT != NULL )
				(ret.m_pB)->AddRefCopy();
			else
				ret.m_pB = NULL;
		}
		return ret;
	}

	//clone
	// T : the component class
	template <class T>
	static _ShareCom<T> Clone(const _ShareCom<T>& sp)  //may throw
	{
		return ( !sp.IsBlockNull() )
				? ( assert( sp.m_pT != NULL ), MakeShareCom((sp.m_pB)->GetMemoryManager(), sp.Deref()) )
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

#define CALL_COM_TYPECAST(src, ifname)  _ShareComHelper::Query(src, USE_GUID(GUID_##ifname))

// _ShareArrayBase<T>

template <typename T>
class _ShareArrayBase : public _SObjT<_ShareArrayBase<T>, _ShareObjectBase>
{
private:
	typedef _SObjT<_ShareArrayBase<T>, _ShareObjectBase>  baseClass;
	typedef _ShareArrayBase<T>  thisClass;

protected:
	_ShareArrayBase() throw()
	{
	}
	_ShareArrayBase(const _ShareArrayBase& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_ShareArrayBase(_ShareArrayBase&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_ShareArrayBase() throw()
	{
		Release();
	}

	_ShareArrayBase<T>& operator=(const _ShareArrayBase<T>& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_ShareArrayBase<T>& operator=(_ShareArrayBase<T>&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( m_pB != NULL ) {
			_SObjRelease<share_array_block>::ShareRelease<T>(m_pB);
			m_pB = NULL;
		} //end if
	}

protected:
	void do_assign(const baseClass& src) throw()
	{
	}
	void do_move(baseClass&& src) throw()
	{
	}
};

// _WeakArrayBase

class _WeakArrayBase : public _SObjT<_WeakArrayBase, _WeakObjectBase>
{
private:
	typedef _SObjT<_WeakArrayBase, _WeakObjectBase>  baseClass;
	typedef _WeakArrayBase  thisClass;

protected:
	_WeakArrayBase() throw()
	{
	}
	_WeakArrayBase(const _WeakArrayBase& src) throw() : baseClass(static_cast<const baseClass&>(src))
	{
	}
	_WeakArrayBase(_WeakArrayBase&& src) throw() : baseClass(rv_forward(static_cast<baseClass&>(src)))
	{
	}
	~_WeakArrayBase() throw()
	{
		Release();
	}

	_WeakArrayBase& operator=(const _WeakArrayBase& src) throw()
	{
		baseClass::operator=(static_cast<const baseClass&>(src));
		return *this;
	}
	_WeakArrayBase& operator=(_WeakArrayBase&& src) throw()
	{
		baseClass::operator=(rv_forward(static_cast<baseClass&>(src)));
		return *this;
	}

//overriders

public:
	void Release() throw()
	{
		if( m_pB != NULL ) {
			//weak
			_SObjRelease<share_array_block>::WeakRelease(m_pB);
			m_pB = NULL;
		} //end if
	}

protected:
	void do_assign(const baseClass& src) throw()
	{
	}
	void do_move(baseClass&& src) throw()
	{
	}
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
		return m_pB == right.m_pB;
	}
	bool operator!=(const _ShareArray<T>& right) const throw()
	{
		return !operator==(right);
	}

//methods

	uintptr GetCount() const throw()
	{
		return IsBlockNull() ? 0 : (static_cast<share_array_block*>(m_pB))->GetLength();
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
		assert( !IsBlockNull() );  //must have a block for allocation
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
		assert( !(pB->GetMemoryManager().IsNull()) );
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
		assert( !IsBlockNull() );  //must have a block for free
		(static_cast<share_array_block*>(m_pB))->DestroyArray<T>();
	}

	void FreeExtra() throw()
	{
		assert( !IsBlockNull() );
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
		assert( !(pB->GetMemoryManager().IsNull()) );
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
		T* pNew = (T*)(pB->GetMemoryManager().Deref().Reallocate((uintptr)get_array_address(), uBytes));
		if( pNew == NULL )
			return ;
		set_array_address(pNew);
		pB->SetAllocLength(uSize);
	}

	//add
	template <typename... Args>
	Iterator Add(Args&&... args)  //may throw
	{
		assert( !IsBlockNull() );
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
		uintptr uElement = pB->GetLength();
		SetCount(uElement + 1, 0, rv_forward<Args>(args)...);
		return GetAt(uElement);
	}
	void Append(const _ShareArray<T>& src)  //may throw
	{
		assert( this != &src );  // cannot append to itself
		assert( !IsBlockNull() );
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
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
		assert( !IsBlockNull() );
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
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
		assert( !IsBlockNull() );
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
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
		return (T*)((static_cast<share_array_block*>(m_pB))->GetAddress());
	}
	void set_array_address(T* p) throw()
	{
		(static_cast<share_array_block*>(m_pB))->SetAddress(p);
	}

private:
	//grow
	void grow_buffer(uintptr uNewSize, uintptr uGrowBy)  //may throw
	{
		share_array_block* pB = static_cast<share_array_block*>(m_pB);
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
			T* pNew = (T*)(pB->GetMemoryManager().Deref().Allocate(uBytes));
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
			T* pNew = (T*)(pB->GetMemoryManager().Deref().Reallocate((uintptr)get_array_address(), uBytes));
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
	static _SharedArray<T> MakeShareArray(const GKC::RefPtr<GKC::IMemoryManager>& mgr)
	{
		assert( !mgr.IsNull() );

		//allocate
		share_array_block* pB = ::_SabPool_Allocate();
		if( pB == NULL )
			throw GKC::OutOfMemoryException();

		_ShareArray<T> ret;
		pB->SetMemoryManager(mgr);
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
			ret = MakeShareArray<T>((sp.m_pB)->GetMemoryManager());
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
};

#pragma pack(pop)

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
#endif //__SA_GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////
