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
/*! \brief Lite Com Smart Pointer.

Lite Com Smart Pointer.
\tparam T The major interface of component class
*/
template <class T>
class _LiteCom
{
public:
	/*! \brief Constructor.

	Constructor.
	*/
	_LiteCom() noexcept : m_p(NULL)
	{
	}
	/*! \brief Move Constructor.

	Move Constructor.
	*/
	_LiteCom(_LiteCom<T>&& src) noexcept : m_p(src.m_p)
	{
		src.m_p = NULL;
	}
	/*! \brief Assignment operator.

	Assignment operator.
	*/
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
	friend class _LiteComObject;
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
		m_hd = rv_forward(hd);
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

//------------------------------------------------------------------------------

#pragma pack(push, 1)

//_IFileStream

class NOVTABLE _IFileStream : public _ILiteComBase
{
public:
	virtual GKC::RefPtr<_IFileUtility> GetFileUtility() noexcept = 0;
	virtual GKC::RefPtr<_IByteStream> GetByteStream() noexcept = 0;
	virtual GKC::RefPtr<_IStreamLocker> GetStreamLocker() noexcept = 0;
};

//_IMemoryStream

class NOVTABLE _IMemoryStream : public _ILiteComBase
{
public:
	virtual GKC::RefPtr<_IMemoryUtilityU> GetMemoryUtility() noexcept = 0;
	virtual GKC::RefPtr<_IByteStream> GetByteStream() noexcept = 0;
};

//_IBufferStream

class NOVTABLE _IBufferStream : public _ILiteComBase
{
public:
	virtual GKC::RefPtr<_IBufferUtility> GetBufferUtility() noexcept = 0;
	virtual GKC::RefPtr<_IByteStream> GetByteStream() noexcept = 0;
};

//_ITextStream

class NOVTABLE _ITextStream : public _ILiteComBase
{
public:
	virtual GKC::RefPtr<_ITextUtilityU> GetTextUtility() noexcept = 0;
	virtual GKC::RefPtr<_ITextStreamRoot> GetTextStreamRoot() noexcept = 0;
	virtual GKC::RefPtr<_ITextStreamStringU> GetTextStreamString() noexcept = 0;
};

//_IPkZip

class NOVTABLE _IPkZip : public _ILiteComBase
{
public:
	virtual GKC::RefPtr<_IPkCompressor> GetPkCompressor() noexcept = 0;
};

//_IPkUnzip

class NOVTABLE _IPkUnzip : public _ILiteComBase
{
public:
	virtual GKC::RefPtr<_IPkDecompressor> GetPkDecompressor() noexcept = 0;
};

#pragma pack(pop)

SA_FUNCTION _LiteCom<_IFileStream> _FileStream_CreateL() noexcept;
SA_FUNCTION _LiteCom<_IMemoryStream> _MemoryStream_CreateL() noexcept;
SA_FUNCTION _LiteCom<_IBufferStream> _BufferStream_CreateL() noexcept;
SA_FUNCTION _LiteCom<_ITextStream> _TextStream_CreateL() noexcept;

SA_FUNCTION _LiteCom<_IPkZip> _PkZip_CreateL() noexcept;
SA_FUNCTION _LiteCom<_IPkUnzip> _PkUnzip_CreateL() noexcept;

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
