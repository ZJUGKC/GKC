/*
** Xin YUAN, 2023, BSD (2)
*/

////////////////////////////////////////////////////////////////////////////////

#pragma pack(push, 1)

//lite_com_block_base

struct lite_com_block_base
{
public:
//methods
	void SetDestructionFunc(object_destruction_func pFunc) noexcept
	{
		m_pDestruction = pFunc;
	}
	object_destruction_func GetDestructionFunc() const noexcept
	{
		return m_pDestruction;
	}

protected:
	object_destruction_func m_pDestruction;
};

//_ILiteComBase

class NOVTABLE _ILiteComBase
{
public:
	virtual void SetLibraryHandle(sa_handle&& hd) noexcept = 0;
	virtual void Release() noexcept = 0;
};

//_LiteCom<T>
//  T: The major interface of component class

template <class T>
class _LiteCom
{
public:
	_LiteCom() noexcept : m_p(NULL)
	{
	}
	_LiteCom(_LiteCom<T>&& src) noexcept : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	_LiteCom<T>& operator=(_LiteCom<T>&& src) noexcept
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
	_LiteCom(const _LiteCom&) = delete;
	_LiteCom& operator=(const _LiteCom&) = delete;
	~_LiteCom() noexcept
	{
		Release();
	}

	void Release() noexcept
	{
		if ( m_p != NULL ) {
			(static_cast<_ILiteComBase*>(m_p))->Release();
			m_p = NULL;
		}
	}

	bool IsNull() const noexcept
	{
		return m_p == NULL;
	}

	const T* Get() const noexcept
	{
		return m_p;
	}
	T* Get() noexcept
	{
		return m_p;
	}
	const T* operator->() const noexcept
	{
		assert( !IsNull() );
		return Get();
	}
	T* operator->() noexcept
	{
		assert( !IsNull() );
		return Get();
	}
	const T& operator*() const noexcept
	{
		assert( !IsNull() );
		return *Get();
	}
	T& operator*() noexcept
	{
		assert( !IsNull() );
		return *Get();
	}

	const T& Deref() const noexcept
	{
		return operator*();
	}
	T& Deref() noexcept
	{
		return operator*();
	}

protected:
	T* m_p;

private:
	template <class TC>
	friend class LiteComObject;
};

//T: component class
//TI: interface class
//TB: implement class for interface
template <class T, class TI, class TB>
inline GKC::RefPtr<TI> _LiteCom_GetInterface(T* pC) noexcept
{
	return GKC::RefPtr<TI>(static_cast<TI*>(static_cast<TB*>(pC)));
}

//_LiteComObject<T>

template <class T>
class _LiteComObject : public T
{
private:
	typedef _LiteComObject<T>  thisClass;

public:
	_LiteComObject()
	{
		m_block.SetDestructionFunc(NULL);
	}
	_LiteComObject(const _LiteComObject&) = delete;
	_LiteComObject& operator=(const _LiteComObject&) = delete;
	~_LiteComObject() noexcept
	{
	}

//_ILiteComBase methods

	virtual void SetLibraryHandle(sa_handle&& hd) noexcept
	{
		m_hd = std::move(hd);
	}
	virtual void Release() noexcept
	{
		object_destruction_func pFunc = m_block.GetDestructionFunc();
		if ( pFunc != NULL ) {
			thisClass* pv = this;
			pFunc(pv);
			::free(pv);
		}
	}

//methods

	static void object_destruction(void* p) noexcept
	{
		((thisClass*)p)->~thisClass();
	}

	template <class TI>
	static _LiteCom<TI> CreateInstance() noexcept
	{
		_LiteCom<TI> ret;

		thisClass* p = (thisClass*)::malloc(sizeof(thisClass));
		if ( p == NULL )
			return ret;

		try {
			call_constructor(*p);  //may throw
		}
		catch(...) {
			::free(p);
			return ret;
		}

		p->m_block.SetDestructionFunc(&object_destruction);

		ret.m_p = static_cast<TI*>(static_cast<T*>(p));

		return ret;
	}

	template <class TI>
	_LiteCom<TI> ToLiteCom() noexcept
	{
		_LiteCom<TI> ret;
		ret.m_p = static_cast<TI*>(static_cast<T*>(this));
		return ret;
	}

protected:
	sa_handle m_hd;
	lite_com_block_base m_block;
};

#pragma pack(pop)

////////////////////////////////////////////////////////////////////////////////
