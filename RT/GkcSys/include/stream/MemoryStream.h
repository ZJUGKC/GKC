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

#include "_MemStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// MemoryStream

class MemoryStream : public _IByteStream,
					public _IMemoryUtility
{
public:
	MemoryStream() throw()
	{
	}
	~MemoryStream() throw()
	{
	}

// _IByteSequentialStream methods
	virtual GKC::CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) throw()
	{
		assert( is_valid() );

		CallResult cr;
		{
			//mutex lock
			MutexLock mutex_lock(m_mutex);

			//read lock
			RWLockShared lock(m_rwlock.Deref());

			cr = m_stm.Read(pv, uBytes, uRead);
		} //end block

		return cr;
	}
	virtual GKC::CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) throw()
	{
		assert( is_valid() );

		CallResult cr;
		{
			//mutex lock
			MutexLock mutex_lock(m_mutex);

			//write lock
			RWLockExclusive lock(m_rwlock.Deref());

			cr = m_stm.Write(pv, uBytes, uWritten);
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
			//mutex lock
			MutexLock mutex_lock(m_mutex);

			//read lock
			RWLockShared lock(m_rwlock.Deref());

			cr = m_stm.Seek(uMethod, iOffset, iNewPos);
		} //end block

		return cr;
	}
	virtual GKC::CallResult SetSize(const int64& iSize) throw()
	{
		assert( is_valid() );

		CallResult cr;
		{
			//write lock
			RWLockExclusive lock(m_rwlock.Deref());

			cr = m_stm.SetSize(iSize);
		} //end block

		return cr;
	}
	virtual GKC::CallResult GetStatus(GKC::StorageStatus& status) throw()
	{
		assert( is_valid() );

		CallResult cr;
		{
			//read lock
			RWLockShared lock(m_rwlock.Deref());

			cr = m_stm.GetStatus(status);
		} //end block

		return cr;
	}

// _IMemoryUtility methods
	virtual GKC::CallResult Initialize() throw()
	{
		CallResult cr;
		//mutex
		if( !m_mutex.IsValid() ) {
			cr = m_mutex.Init();
			if( cr.IsFailed() )
				return cr;
		}
		//array
		try {
			if( m_stm.GetArray().IsBlockNull() )
				m_stm.GetArray() = _ShareArrayHelper::MakeShareArray<byte>(RefPtr<IMemoryManager>(_CrtMemoryManager_Get()));
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
			m_stm.GetArray().Release();
			m_rwlock.Release();
			return cr;
		}
		//position
		m_stm.GetPos() = 0;
		return cr;
	}
	virtual void SetArray(const _ShareArray<byte>& sp) throw()
	{
		assert( is_valid() );
		assert( !sp.IsBlockNull() );
		m_stm.GetArray() = sp;
		m_stm.GetPos() = 0;
	}
	virtual _ShareArray<byte> GetArray() throw()
	{
		assert( is_valid() );
		return m_stm.GetArray();
	}
	virtual GKC::CallResult CloneTo(_ShareCom<_IByteStream>& sp) throw()
	{
		assert( is_valid() );
		CallResult cr;
		_ShareCom<MemoryStream> spC;
		cr = _Create_Component_Instance<MemoryStream>(spC);
		if( cr.IsFailed() )
			return cr;
		cr = spC.Deref().m_mutex.Init();
		if( cr.IsFailed() )
			return cr;
		spC.Deref().m_stm.GetArray() = m_stm.GetArray();
		spC.Deref().m_rwlock = m_rwlock;
		spC.Deref().m_stm.GetPos()   = 0;
		_COMPONENT_INSTANCE_INTERFACE(MemoryStream, _IByteStream, spC, sp, cr);
		if( cr.IsFailed() )
			return cr;
		return cr;
	}

private:
	bool is_valid() const throw()
	{
		return m_mutex.IsValid() && !m_stm.GetArray().IsBlockNull() && !m_rwlock.IsBlockNull();
	}

private:
	_MemStream<_ShareArray<byte>, _ShareArrayHelper>  m_stm;
	_SharePtr<RWLock>  m_rwlock;  //lock
	Mutex  m_mutex;  // for position changed in this object

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
