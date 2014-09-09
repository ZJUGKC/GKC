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
This file contains classes for COM programming.

Author: Lijuan Mei
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////
#ifndef __COM_BASE_H__
#define __COM_BASE_H__
////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

//assign

inline IUnknown* _ComPtrAssign(INOUT IUnknown** pp, IN IUnknown* lp) throw()
{
	if( pp == NULL )
		return NULL;
	if( lp != NULL )
		lp->AddRef();
	if( *pp )
		(*pp)->Release();
	*pp = lp;
	return lp;
}

inline IUnknown* _ComQIPtrAssign(INOUT IUnknown** pp, IN IUnknown* lp, IN REFIID riid) throw()
{
	if( pp == NULL )
		return NULL;
	IUnknown* pTemp = *pp;
	*pp = NULL;
	if( lp != NULL )
		lp->QueryInterface(riid, (void**)pp);
	if( pTemp )
		pTemp->Release();
	return *pp;
}

// com_ptr<T>

template <class T>
class _NoAddRefReleaseOnComPtr : public T
{
private:
	STDMETHOD_(ULONG, AddRef)() = 0;
	STDMETHOD_(ULONG, Release)() = 0;
};

template <class T>
class com_ptr
{
public:
	com_ptr() throw() : m_p(NULL)
	{
	}
	explicit com_ptr(INOUT T* lp) throw() : m_p(lp)
	{
		if( m_p != NULL )
			m_p->AddRef();
	}
	~com_ptr() throw()
	{
		if( m_p != NULL )
			m_p->Release();
	}

	com_ptr(INOUT const com_ptr<T>& sp) throw() : m_p(sp.m_p)
	{
		if( m_p != NULL )
			m_p->AddRef();
	}
	com_ptr(INOUT com_ptr<T>&& lp) throw() : m_p(lp.p)
	{
		lp.p = NULL;
	}

	T* operator=(INOUT T* lp) throw()
	{
		if( *this != lp ) {
			return static_cast<T*>(_ComPtrAssign((IUnknown**)&m_p, lp));
		}
		return *this;
	}
	template <typename Q>
	T* operator=(INOUT const com_ptr<Q>& sp) throw()
	{
		if( !IsEqualObject(sp) ) {
			return static_cast<T*>(_ComQIPtrAssign((IUnknown**)&m_p, sp, __uuidof(T)));
		}
		return *this;
	}
	T* operator=(INOUT const com_ptr<T>& sp) throw()
	{
		if( *this != sp ) {
			return static_cast<T*>(_ComPtrAssign((IUnknown**)&m_p, sp));
		}
		return *this;
	}
	T* operator=(INOUT com_ptr<T>&& lp) throw()
	{
		if( *this != lp ) {
			if( m_p != NULL )
				m_p->Release();
			m_p = lp.p;
			lp.p = NULL;
		}
		return *this;
	}

	//operators
	operator T*() const throw()
	{
		return m_p;
	}
	T& operator*() const throw()
	{
		assert( m_p != NULL );
		return *m_p;
	}
	T** operator&() throw()
	{
		assert( m_p == NULL );
		return &m_p;
	}
	_NoAddRefReleaseOnComPtr<T>* operator->() const throw()
	{
		assert( m_p != NULL );
		return (_NoAddRefReleaseOnComPtr<T>*)m_p;
	}
	bool operator!() const throw()
	{	
		return (m_p == NULL);
	}
	bool operator<(IN T* pT) const throw()
	{
		return m_p < pT;
	}
	bool operator!=(IN T* pT) const throw()
	{
		return !operator==(pT);
	}
	bool operator==(IN T* pT) const throw()
	{
		return m_p == pT;
	}

	//methods
	bool IsNull() const throw()
	{
		return operator!();
	}
	T* Get() const throw()
	{
		return m_p;
	}
	// Release the interface and set to NULL
	void Release() throw()
	{
		T* pTemp = m_p;
		if( pTemp != NULL ) {
			m_p = NULL;
			pTemp->Release();
		}
	}
	// Compare two objects
	bool IsEqualObject(IN IUnknown* pOther) const throw()
	{
		if( m_p == NULL && pOther == NULL )
			return true;   // They are both NULL objects
		if( m_p == NULL || pOther == NULL )
			return false;  // One is NULL the other is not
		com_ptr<IUnknown> punk1;
		com_ptr<IUnknown> punk2;
		m_p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
		pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
		return punk1 == punk2;
	}
	// Attach to an existing interface (does not AddRef)
	void Attach(IN T* p2) throw()
	{
		if( m_p != NULL && m_p != p2 )
			m_p->Release();
		m_p = p2;
	}
	// Detach the interface (does not Release)
	T* Detach() throw()
	{
		T* pt = m_p;
		m_p = NULL;
		return pt;
	}
	HRESULT CopyTo(OUT T** ppT) throw()
	{
		assert( ppT != NULL );
		if( ppT == NULL )
			return E_POINTER;
		*ppT = m_p;
		if( m_p != NULL )
			m_p->AddRef();
		return S_OK;
	}
	HRESULT CoCreateInstance(IN REFCLSID rclsid, INOUT LPUNKNOWN pUnkOuter = NULL, IN DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		assert( m_p == NULL );
		return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&m_p);
	}
	HRESULT CoCreateInstance(IN LPCOLESTR szProgID, INOUT LPUNKNOWN pUnkOuter = NULL, IN DWORD dwClsContext = CLSCTX_ALL) throw()
	{
		CLSID clsid;
		HRESULT hr = ::CLSIDFromProgID(szProgID, &clsid);
		assert( m_p == NULL );
		if( SUCCEEDED(hr) )
			hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&m_p);
		return hr;
	}
	template <class Q>
	HRESULT QueryInterface(OUT Q** pp) const throw()
	{
		assert( pp != NULL );
		assert( m_p != NULL );
		return m_p->QueryInterface(__uuidof(Q), (void**)pp);
	}

private:
	T* m_p;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif //__COM_BASE_H__
////////////////////////////////////////////////////////////////////////////////
