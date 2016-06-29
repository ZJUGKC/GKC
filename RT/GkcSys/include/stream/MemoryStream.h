/*
** Copyright (c) 2016, Xin YUAN, courses of Zhejiang University
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
This file contains component class of memory stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __MEMORY_STREAM_H__
#define __MEMORY_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// MemoryStream

class MemoryStream : public _IByteStream,
					public _IMemoryUtility
{
public:
	MemoryStream() throw() : m_iPos(0)
	{
	}
	~MemoryStream() throw()
	{
	}

// _IByteSequentialStream methods
	virtual GKC::CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) throw()
	{
		assert( is_valid() );

		{
			//read lock
			RWLockShared lock(m_rwlock.Deref());

			int64 iCount = (int64)m_array.GetCount();  //safe
			byte* pArray = _ShareArrayHelper::GetInternalPointer(m_array);

			uRead = uBytes;
			if( m_iPos > iCount )
				uRead = 0;
			else if( (int64)uBytes > iCount - m_iPos )
				uRead = (uint)(iCount - m_iPos);

			//read
			if( uRead != 0 ) {
				mem_copy(pArray + m_iPos, uRead, (void*)pv);
				m_iPos += ((int64)uRead);
			}
		} //end block

		return CallResult();
	}
	virtual GKC::CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) throw()
	{
		assert( is_valid() );

		CallResult cr;
		{
			//write lock
			RWLockExclusive lock(m_rwlock.Deref());

			int64 iCount = (int64)m_array.GetCount();  //safe
			int64 iNewPos = 0;
			cr = SafeOperators::Add(m_iPos, (int64)uBytes, iNewPos);
			if( cr.IsFailed() )
				return cr;
			if( (uint64)iNewPos > (uint64)(Limits<uintptr>::Max) ) {  //check 32 bits
				cr.SetResult(SystemCallResults::Overflow);
				return cr;
			}
			//resize
			if( iNewPos > iCount ) {
				try {
					m_array.SetCount((uintptr)iNewPos, 0);
				}
				catch(Exception& e) {
					cr = e.GetResult();
				}
				catch(...) {
					cr.SetResult(SystemCallResults::Fail);
				}
				if( cr.IsFailed() )
					return cr;
			}

			//write
			byte* pArray = _ShareArrayHelper::GetInternalPointer(m_array);
			mem_copy((void*)pv, uBytes, pArray + m_iPos);
			m_iPos = iNewPos;
			uWritten = uBytes;
		} //end block

		return cr;
	}

// _IByteStream methods
	virtual GKC::CallResult Commit() throw()
	{
		return CallResult();
	}
	virtual GKC::CallResult Seek(const uint& uMethod, const int64& iOffset, int64& iNewPos) throw()
	{
		assert( is_valid() );

		CallResult cr;
		{
			//read lock
			RWLockShared lock(m_rwlock.Deref());

			int64 iNow = m_iPos;
			if( uMethod == IO_SEEK_BEGIN ) {
				if( iOffset < 0 || (uint64)iOffset > (uint64)(Limits<uintptr>::Max) ) {  //check 32 bits
					cr.SetResult(SystemCallResults::Invalid);
					return cr;
				}
				iNow = iOffset;
			}
			else if( uMethod == IO_SEEK_CURRENT ) {
				if( iOffset > 0 ) {
					cr = SafeOperators::Add(m_iPos, iOffset, iNow);
					if( cr.IsFailed() )
						return cr;
					if( (uint64)iNow > (uint64)(Limits<uintptr>::Max) ) {  //check 32 bits
						cr.SetResult(SystemCallResults::Invalid);
						return cr;
					}
				}
				else if( iOffset < 0 ) {
					if( -iOffset > m_iPos ) {
						cr.SetResult(SystemCallResults::Invalid);
						return cr;
					}
					iNow = m_iPos + iOffset;
				}
			}
			else if( uMethod == IO_SEEK_END ) {
				int64 iCount = (int64)m_array.GetCount();  //safe
				if( iOffset > 0 ) {
					cr = SafeOperators::Add(iCount, iOffset, iNow);
					if( cr.IsFailed() )
						return cr;
					if( (uint64)iNow > (uint64)(Limits<uintptr>::Max) ) {  //check 32 bits
						cr.SetResult(SystemCallResults::Invalid);
						return cr;
					}
				}
				else if( iOffset < 0 ) {
					if( -iOffset > iCount ) {
						cr.SetResult(SystemCallResults::Invalid);
						return cr;
					}
					iNow = iCount + iOffset;
				}
				else {
					iNow = iCount;
				}
			}
			else {
				cr.SetResult(SystemCallResults::Invalid);
				return cr;
			}

			m_iPos  = iNow;
			iNewPos = iNow;
		} //end block

		return cr;
	}
	virtual GKC::CallResult SetSize(const int64& iSize) throw()
	{
		assert( is_valid() );

		if( iSize < 0 || (uint64)iSize > (uint64)(Limits<uintptr>::Max) )  //check 32 bits
			return CallResult(SystemCallResults::Invalid);

		CallResult cr;
		{
			//write lock
			RWLockExclusive lock(m_rwlock.Deref());

			//resize
			try {
				m_array.SetCount((uintptr)iSize, 0);
			}
			catch(Exception& e) {
				cr = e.GetResult();
			}
			catch(...) {
				cr.SetResult(SystemCallResults::Fail);
			}
		} //end block

		return cr;
	}
	virtual GKC::CallResult GetStatus(GKC::StorageStatus& status) throw()
	{
		assert( is_valid() );

		{
			//read lock
			RWLockShared lock(m_rwlock.Deref());

			status.iSize = (int64)m_array.GetCount();  //safe
		} //end block

		return CallResult();
	}

// _IMemoryUtility methods
	virtual GKC::CallResult Initialize() throw()
	{
		CallResult cr;
		try {
			if( m_array.IsBlockNull() )
				m_array = _ShareArrayHelper::MakeShareArray<byte>(RefPtr<IMemoryManager>(_CrtMemoryManager_Get()));
			if( m_rwlock.IsBlockNull() ) {
				m_rwlock = _SharePtrHelper::MakeSharePtr<RWLock>(RefPtr<IMemoryManager>(_CrtMemoryManager_Get()));
				m_rwlock.Deref().Init();
			}
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}
		if( cr.IsFailed() ) {
			m_array.Release();
			m_rwlock.Release();
			return cr;
		}
		m_iPos = 0;
		return cr;
	}
	virtual void SetArray(const _ShareArray<byte>& sp) throw()
	{
		assert( is_valid() );
		assert( !sp.IsBlockNull() );
		m_array = sp;
		m_iPos = 0;
	}
	virtual _ShareArray<byte> GetArray() throw()
	{
		assert( is_valid() );
		return m_array;
	}
	virtual GKC::CallResult CloneTo(_ShareCom<_IByteStream>& sp) throw()
	{
		assert( is_valid() );
		CallResult cr;
		_ShareCom<MemoryStream> spC;
		cr = _Create_Component_Instance<MemoryStream>(spC);
		if( cr.IsFailed() )
			return cr;
		_ShareCom<_IByteStream> spI;
		_COMPONENT_INSTANCE_INTERFACE(MemoryStream, _IByteStream, spC, spI, cr);
		if( cr.IsFailed() )
			return cr;
		spC.Deref().m_array  = m_array;
		spC.Deref().m_rwlock = m_rwlock;
		spC.Deref().m_iPos   = 0;
		sp = spI;
		return cr;
	}

private:
	bool is_valid() const throw()
	{
		return !m_array.IsBlockNull() && !m_rwlock.IsBlockNull();
	}

private:
	_ShareArray<byte>  m_array;
	_SharePtr<RWLock>  m_rwlock;  //lock
	int64  m_iPos;

private:
	//noncopyable
	MemoryStream(const MemoryStream&) throw();
	MemoryStream& operator=(const MemoryStream&) throw();
};

DECLARE_COM_TYPECAST(MemoryStream)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
