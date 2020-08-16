/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

// Internal header

////////////////////////////////////////////////////////////////////////////////
#ifndef __INTERNAL_MEM_STREAM_H__
#define __INTERNAL_MEM_STREAM_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _MemStream<Tstring>
//   Tarray: _ShareArray<byte> or _UniqueArray<byte>
//   Thelper: _ShareArrayHelper or _UniqueArrayHelper
template <class Tarray, class Thelper>
class _MemStream
{
public:
	_MemStream() noexcept : m_iPos(0)
	{
	}
	_MemStream(const _MemStream&) = delete;
	_MemStream& operator=(const _MemStream&) = delete;
	~_MemStream() noexcept
	{
	}

	const Tarray& GetArray() const noexcept
	{
		return m_array;
	}
	Tarray& GetArray() noexcept
	{
		return m_array;
	}
	int64& GetPos() noexcept
	{
		return m_iPos;
	}

//methods
	CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) noexcept
	{
		int64 iCount = (int64)m_array.GetCount();  //safe
		byte* pArray = Thelper::GetInternalPointer(m_array);

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

		return CallResult();
	}
	CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) noexcept
	{
		CallResult cr;

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
				m_array.SetCountD((uintptr)iNewPos);
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
		byte* pArray = Thelper::GetInternalPointer(m_array);
		mem_copy((void*)pv, uBytes, pArray + m_iPos);
		m_iPos = iNewPos;
		uWritten = uBytes;

		return cr;
	}

	CallResult Seek(const uint& uMethod, const int64& iOffset, int64& iNewPos) noexcept
	{
		CallResult cr;

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

		return cr;
	}
	CallResult SetSize(const int64& iSize) noexcept
	{
		if( iSize < 0 || (uint64)iSize > (uint64)(Limits<uintptr>::Max) )  //check 32 bits
			return CallResult(SystemCallResults::Invalid);

		CallResult cr;

		//resize
		try {
			m_array.SetCountD((uintptr)iSize);
		}
		catch(Exception& e) {
			cr = e.GetResult();
		}
		catch(...) {
			cr.SetResult(SystemCallResults::Fail);
		}

		return cr;
	}
	CallResult GetStatus(StorageStatus& status) noexcept
	{
		status.iSize = (int64)m_array.GetCount();  //safe
		return CallResult();
	}

private:
	Tarray  m_array;
	int64   m_iPos;
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
