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
This file contains component class of PKZ decompressor.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PKZ_DECOMPRESSOR_H__
#define __PKZ_DECOMPRESSOR_H__
////////////////////////////////////////////////////////////////////////////////

#include "_PkCompress.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// PkzDecompressor

class PkzDecompressor : public _IPkDecompressor
{
public:
	PkzDecompressor() noexcept
	{
	}
	PkzDecompressor(const PkzDecompressor&) = delete;
	PkzDecompressor& operator=(const PkzDecompressor&) = delete;
	~PkzDecompressor() noexcept
	{
	}

// _IPkDecompressor methods
	virtual void Initialize() noexcept
	{
		m_d.Initialize();
	}
	virtual void GetInputBuffer(_PkInputBuffer& buf) noexcept
	{
		void* p = (void*)&(buf.pBuffer);
		m_d.GetInputBuffer(*(reinterpret_cast<byte**>(p)), buf.uLength);
	}
	virtual void SetInputSize(ushort uSize) noexcept
	{
		m_d.SetInputSize(uSize);
	}
	virtual void GetOutputBuffer(_PkOutputBuffer& buf) noexcept
	{
		void* p = (void*)&(buf.pBuffer);
		m_d.GetOutputBuffer(*(reinterpret_cast<byte**>(p)), buf.uLength);
	}
	virtual void ResetOutputBuffer() noexcept
	{
		m_d.ResetOutputBuffer();
	}
	virtual int Decode() noexcept
	{
		return m_d.Decode();
	}

private:
	pkzip_decompressor  m_d;
};

DECLARE_COM_TYPECAST(PkzDecompressor)

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
