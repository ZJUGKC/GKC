/*
** Copyright (c) 2019, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//unique pointers

// unique_ptr<T>

template <typename T>
class unique_ptr
{
public:
	unique_ptr() throw() : m_p(NULL)
	{
	}
	unique_ptr(unique_ptr<T>&& src) throw()
	{
		m_p = src.m_p;
		src.m_p = NULL;
	}
	~unique_ptr() throw()
	{
		Release();
	}

	unique_ptr<T>& operator=(unique_ptr<T>&& src) throw()
	{
		if( this != &src ) {
			if( m_p == src.m_p ) {
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

	void Release() throw()
	{
		if( m_p != NULL ) {
			m_p->~T();
			crt_free(m_p);
			m_p = NULL;
		}
	}

	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	const T& Deref() const throw()
	{
		assert( !IsNull() );
		return *m_p;
	}
	T& Deref() throw()
	{
		assert( !IsNull() );
		return *m_p;
	}

protected:
	T* m_p;

private:
	unique_ptr(const unique_ptr&) throw();
	unique_ptr& operator=(const unique_ptr&) throw();

private:
	friend class unique_ptr_helper;
};

// unique_ptr_helper

class unique_ptr_helper
{
public:
	template <typename T, typename... Args>
	static unique_ptr<T> MakeUniquePtr(Args&&... args)
	{
		T* p = (T*)crt_alloc(sizeof(T));
		if( p == NULL )
			throw outofmemory_exception();
		try {
			call_constructor(*p, rv_forward<Args>(args)...);
		}
		catch(...) {
			crt_free(p);
			throw;
		}
		unique_ptr<T> ret;
		ret.m_p = p;
		return ret;
	}

	template <typename T>
	static T* GetInternalPointer(const unique_ptr<T>& sp) throw()
	{
		return const_cast<T*>(sp.m_p);
	}

	//type cast (derived -> base)
	template <typename TSrc, typename TDest>
	static ref_ptr<TDest> TypeCast(const unique_ptr<TSrc>& sp) throw()
	{
		TDest* pRet = NULL;
		if( !sp.IsNull() )
			pRet = static_cast<TDest*>(sp.m_p);
		return ref_ptr<TDest>(pRet);
	}
};

// unique_array<T>

template <typename T>
class unique_array
{
public:
	typedef array_position  Position;
	typedef array_iterator<T>  Iterator;

public:
	unique_array() throw() : m_p(NULL), m_uCount(0)
	{
	}
	unique_array(unique_array<T>&& src) throw()
	{
		m_p = src.m_p;
		src.m_p = NULL;
		m_uCount = src.m_uCount;
		src.m_uCount = 0;
	}
	~unique_array() throw()
	{
		Free();
	}

	unique_array<T>& operator=(unique_array<T>&& src) throw()
	{
		if( this != &src ) {
			if( m_p == src.m_p ) {
				assert( m_p == NULL );
			}
			else {
				Free();
				m_p = src.m_p;
				src.m_p = NULL;
				m_uCount = src.m_uCount;
				src.m_uCount = 0;
			}
		}
		return *this;
	}

	const Iterator operator[](uintptr uIndex) const throw()
	{
		return GetAt(uIndex);
	}
	Iterator operator[](uintptr uIndex) throw()
	{
		return GetAt(uIndex);
	}

	uintptr GetCount() const throw()
	{
		return m_p == NULL ? 0 : m_uCount;
	}
	bool IsNull() const throw()
	{
		return m_p == NULL;
	}

	void Free() throw()
	{
		if( m_p != NULL ) {
			call_destructors(m_p, m_uCount);
			crt_free(m_p);
			m_p = NULL;
		}
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
		return Iterator(ref_ptr<T>(m_p + pos.GetIndex()));
	}
	Iterator GetAtPosition(const Position& pos) throw()
	{
		return Iterator(ref_ptr<T>(m_p + pos.GetIndex()));
	}
	Position ToPosition(const Iterator& iter) const throw()
	{
		return Position(ref_ptr_helper::GetInternalPointer(iter.get_Ref()) - m_p);
	}

	//iterator
	const Iterator GetBegin() const throw()
	{
		return Iterator(ref_ptr<T>(m_p));
	}
	Iterator GetBegin() throw()
	{
		return Iterator(ref_ptr<T>(m_p));
	}
	const Iterator GetEnd() const throw()
	{
		return Iterator(ref_ptr<T>(m_p + m_uCount));
	}
	Iterator GetEnd() throw()
	{
		return Iterator(ref_ptr<T>(m_p + m_uCount));
	}

	const Iterator GetAt(uintptr uIndex) const throw()
	{
		assert( uIndex < GetCount() );
		return Iterator(ref_ptr<T>(m_p + uIndex));
	}
	Iterator GetAt(uintptr uIndex) throw()
	{
		assert( uIndex < GetCount() );
		return Iterator(ref_ptr<T>(m_p + uIndex));
	}
	void SetAt(uintptr uIndex, const T& t)
	{
		assert( uIndex < GetCount() );
		m_p[uIndex] = t;  //may throw
	}
	void SetAt(uintptr uIndex, T&& t)
	{
		assert( uIndex < GetCount() );
		m_p[uIndex] = rv_forward(t);  //may throw
	}

protected:
	static void call_destructors(T* p, uintptr uCount) throw()
	{
		T* pt = p;
		for( uintptr i = 0; i < uCount; i ++ ) {
			pt->~T();
			++ pt;
		}
	}

protected:
	T*       m_p;
	uintptr  m_uCount;

private:
	unique_array(const unique_array&) throw();
	unique_array& operator=(const unique_array&) throw();

private:
	friend class unique_array_helper;
};

// unique_array_helper

class unique_array_helper
{
public:
	template <typename T, typename... Args>
	static unique_array<T> MakeUniqueArray(uintptr uCount, Args&&... args)
	{
		assert( uCount > 0 );
		uintptr uBytes = safe_operators::MultiplyThrow(uCount, (uintptr)sizeof(T));
		T* p = (T*)crt_alloc(uBytes);
		if( p == NULL )
			throw outofmemory_exception();
		uintptr i;
		try {
			for( i = 0; i < uCount; i ++ ) {
				call_constructor(*(p + i), rv_forward<Args>(args)...);
			}
		}
		catch(...) {
			while( i > 0 ) {
				i --;
				p[i].~T();
			}
			crt_free(p);
			throw;  //re-throw
		}
		unique_array<T> ret;
		ret.m_p = p;
		ret.m_uCount = uCount;
		return ret;
	}

	template <typename T>
	static T* GetInternalPointer(const unique_array<T>& sp) throw()
	{
		return const_cast<T*>(sp.m_p);
	}
};

////////////////////////////////////////////////////////////////////////////////
