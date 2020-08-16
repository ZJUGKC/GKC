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

/*
This file contains component class of unique memory stream.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __MEMORY_STREAM_U_H__
#define __MEMORY_STREAM_U_H__
////////////////////////////////////////////////////////////////////////////////

#include "_MemStream.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// MemoryStreamU

class MemoryStreamU : public _IByteStream,
					public _IMemoryUtilityU
{
public:
	MemoryStreamU() noexcept
	{
	}
	MemoryStreamU(const MemoryStreamU&) = delete;
	MemoryStreamU& operator=(const MemoryStreamU&) = delete;
	~MemoryStreamU() noexcept
	{
	}

// _IByteSequentialStream methods
	virtual GKC::CallResult Read(const uintptr& pv, const uint& uBytes, uint& uRead) throw()
	{
		return m_stm.Read(pv, uBytes, uRead);
	}
	virtual GKC::CallResult Write(const uintptr& pv, const uint& uBytes, uint& uWritten) throw()
	{
		return m_stm.Write(pv, uBytes, uWritten);
	}

// _IByteStream methods
	virtual GKC::CallResult Commit() throw()
	{
		return CallResult();
	}
	virtual GKC::CallResult Seek(const uint& uMethod, const int64& iOffset, int64& iNewPos) throw()
	{
		return m_stm.Seek(uMethod, iOffset, iNewPos);
	}
	virtual GKC::CallResult SetSize(const int64& iSize) throw()
	{
		return m_stm.SetSize(iSize);
	}
	virtual GKC::CallResult GetStatus(GKC::StorageStatus& status) throw()
	{
		return m_stm.GetStatus(status);
	}

// _IMemoryUtilityU methods
	virtual void Attach(_UniqueArray<byte>&& sp) noexcept
	{
		m_stm.GetArray() = rv_forward(sp);
		m_stm.GetPos() = 0;
	}
	virtual _UniqueArray<byte> Detach() noexcept
	{
		m_stm.GetPos() = 0;
		return _UniqueArray<byte>(rv_forward(m_stm.GetArray()));
	}

private:
	_MemStream<_UniqueArray<byte>, _UniqueArrayHelper>  m_stm;
};

DECLARE_COM_TYPECAST(MemoryStreamU)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
