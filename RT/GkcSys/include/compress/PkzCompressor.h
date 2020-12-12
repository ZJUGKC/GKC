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
This file contains component class of PKZ compressor.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PKZ_COMPRESSOR_H__
#define __PKZ_COMPRESSOR_H__
////////////////////////////////////////////////////////////////////////////////

#include "_PkCompress.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// PkzCompressor

class PkzCompressor : public _IPkCompressor
{
public:
	PkzCompressor() noexcept
	{
	}
	PkzCompressor(const PkzCompressor&) = delete;
	PkzCompressor& operator=(const PkzCompressor&) = delete;
	~PkzCompressor() noexcept
	{
	}

// _IPkCompressor methods
	virtual void Initialize() noexcept
	{
		m_c.Initialize();
	}
	virtual void SetLongestSearch(bool b) noexcept
	{
		m_c.SetLongestSearch(b);
	}
	virtual void GetInputBuffer(_PkInputBuffer& buf) noexcept
	{
		void* p = (void*)&(buf.pBuffer);
		m_c.GetInputBuffer(*(reinterpret_cast<byte**>(p)), buf.uLength);
	}
	virtual void SetInputSize(ushort uSize) noexcept
	{
		m_c.SetInputSize(uSize);
	}
	virtual void GetOutputBuffer(_PkOutputBuffer& buf) noexcept
	{
		void* p = (void*)&(buf.pBuffer);
		m_c.GetOutputBuffer(*(reinterpret_cast<byte**>(p)), buf.uLength);
	}
	virtual void ResetOutputBuffer() noexcept
	{
		m_c.ResetOutputBuffer();
	}
	virtual int Encode() noexcept
	{
		return m_c.Encode();
	}
	virtual void StartFinish() noexcept
	{
		m_c.StartFinish();
	}
	virtual int ProcessFinish() noexcept
	{
		return m_c.ProcessFinish();
	}

private:
	pkzip_compressor  m_c;
};

DECLARE_COM_TYPECAST(PkzCompressor)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
