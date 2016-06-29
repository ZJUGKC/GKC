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
This file contains component class of buffer stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __BUFFER_STREAM_H__
#define __BUFFER_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// BufferStream

class BufferStream : public _IByteStream,
					public _IBufferUtility
{
public:
	BufferStream() throw() : m_pBuffer(NULL), m_iSize(0), m_iPos(0)
	{
	}
	~BufferStream() throw()
	{
	}

// _IByteSequentialStream methods
	virtual GKC::CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) throw()
	{
		assert( is_valid() );
		uRead = uBytes;
		if( (int64)uBytes > m_iSize - m_iPos )
			uRead = (uint)(m_iSize - m_iPos);
		if( uRead != 0 ) {
			mem_copy((byte*)m_pBuffer + m_iPos, uRead, (void*)pv);
			m_iPos += ((int64)uRead);
		}
		return CallResult();
	}
	virtual GKC::CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) throw()
	{
		return CallResult(SystemCallResults::NotImpl);
	}

// _IByteStream methods
	virtual GKC::CallResult Commit() throw()
	{
		return CallResult(SystemCallResults::NotImpl);
	}
	virtual GKC::CallResult Seek(const uint& uMethod, const int64& iOffset, int64& iNewPos) throw()
	{
		assert( is_valid() );

		CallResult cr;
		int64 iNow = m_iPos;
		if( uMethod == IO_SEEK_BEGIN ) {
			if( iOffset < 0 || iOffset > m_iSize ) {
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
				if( iNow > m_iSize ) {
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
			if( iOffset > 0 || -iOffset > m_iSize ) {
				cr.SetResult(SystemCallResults::Invalid);
				return cr;
			}
			iNow = m_iSize + iOffset;
		}
		else {
			cr.SetResult(SystemCallResults::Invalid);
			return cr;
		}
		m_iPos  = iNow;
		iNewPos = iNow;

		return cr;
	}
	virtual GKC::CallResult SetSize(const int64& iSize) throw()
	{
		return CallResult(SystemCallResults::NotImpl);
	}
	virtual GKC::CallResult GetStatus(GKC::StorageStatus& status) throw()
	{
		assert( is_valid() );
		status.iSize = m_iSize;
		return CallResult();
	}

// _IBufferUtility methods
	virtual GKC::CallResult SetBuffer(const uintptr& p, const uintptr& uBytes) throw()
	{
		assert( p != 0 );
		uint64 uSize = (uint64)uBytes;
		if( uSize > (uint64)(Limits<int64>::Max) )
			return CallResult(SystemCallResults::Invalid);
		m_pBuffer = (void*)p;
		m_iSize   = (int64)uSize;
		m_iPos    = 0;
		return CallResult();
	}

private:
	bool is_valid() const throw()
	{
		return m_pBuffer != NULL;
	}

private:
	void*  m_pBuffer;
	int64  m_iSize;
	int64  m_iPos;  //the current position in buffer

private:
	//noncopyable
	BufferStream(const BufferStream&) throw();
	BufferStream& operator=(const BufferStream&) throw();
};

DECLARE_COM_TYPECAST(BufferStream)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
