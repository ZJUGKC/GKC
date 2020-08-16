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
This file contains stream functions.
*/

////////////////////////////////////////////////////////////////////////////////

#include "PreComp.h"

#include "_GkcSys.h"

#include "base/SysDef.h"

#include "stream/FileStream.h"
#include "stream/MemoryStream.h"
#include "stream/MemoryStreamU.h"
#include "stream/BufferStream.h"
#include "stream/TextStream.h"
#include "stream/TextStreamU.h"

////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//functions

namespace GKC {

// file stream
inline void _Internal_FileStream_Create(const GKC::CharS* szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	_ShareCom<_IByteStream> spStream;
	_CREATE_COMPONENT_INSTANCE(FileStream, _IByteStream, spStream, cr);
	if( cr.IsFailed() )
		return ;
	//file utility
	_ShareCom<_IFileUtility> spFU(CALL_COM_TYPECAST(spStream, _IByteStream, _IFileUtility));
	if( spFU.IsBlockNull() ) {
		cr.SetResult(SystemCallResults::Fail);
		return ;
	}
	cr = spFU.Deref().Open(RefPtr<CharS>(szFile), iOpenType, iCreateType);
	if( cr.IsFailed() )
		return ;
	//assignment
	sp = spStream;
}

// memory stream
inline void _Internal_MemoryStream_Create(_ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	_ShareCom<_IByteStream> spStream;
	_CREATE_COMPONENT_INSTANCE(MemoryStream, _IByteStream, spStream, cr);
	if( cr.IsFailed() )
		return ;
	//memory utility
	_ShareCom<_IMemoryUtility> spMU(CALL_COM_TYPECAST(spStream, _IByteStream, _IMemoryUtility));
	if( spMU.IsBlockNull() ) {
		cr.SetResult(SystemCallResults::Fail);
		return ;
	}
	cr = spMU.Deref().Initialize();
	if( cr.IsFailed() )
		return ;
	//assignment
	sp = spStream;
}

// buffer stream
inline void _Internal_BufferStream_Create(const void* p, uintptr uBytes, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	_ShareCom<_IByteStream> spStream;
	_CREATE_COMPONENT_INSTANCE(BufferStream, _IByteStream, spStream, cr);
	if( cr.IsFailed() )
		return ;
	//buffer utility
	_ShareCom<_IBufferUtility> spBU(CALL_COM_TYPECAST(spStream, _IByteStream, _IBufferUtility));
	if( spBU.IsBlockNull() ) {
		cr.SetResult(SystemCallResults::Fail);
		return ;
	}
	cr = spBU.Deref().SetBuffer((uintptr)p, uBytes);
	if( cr.IsFailed() )
		return ;
	//assignment
	sp = spStream;
}

// text stream
inline void _Internal_TextStream_Create(_ShareCom<_ITextStreamRoot>& sp, GKC::CallResult& cr) throw()
{
	_CREATE_COMPONENT_INSTANCE(TextStream, _ITextStreamRoot, sp, cr);
}

}  //namespace

// file stream

void _FileStream_Create(const GKC::CharS* szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_FileStream_Create(szFile, iOpenType, iCreateType, sp, cr);
}

// memory stream

void _MemoryStream_Create(_ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_MemoryStream_Create(sp, cr);
}

// buffer stream

void _BufferStream_Create(const void* p, uintptr uBytes, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_BufferStream_Create(p, uBytes, sp, cr);
}

// text stream

void _TextStream_Create(_ShareCom<_ITextStreamRoot>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_TextStream_Create(sp, cr);
}

//------------------------------------------------------------------------------
// for unique components

void _FileStream_CreateU(const GKC::CharS* szFile, int iOpenType, int iCreateType, _UniqueCom& sp, GKC::CallResult& cr) noexcept
{
	_UniqueCom spStream;
	_CREATE_UNIQUECOM_INSTANCE(GKC::FileStream, spStream, cr);
	if( cr.IsFailed() )
		return ;
	//file utility
	GKC::RefPtr<_IFileUtility> spFU(CALL_UNIQUECOM_TYPECAST(spStream, _IFileUtility));
	if( spFU.IsNull() ) {
		cr.SetResult(GKC::SystemCallResults::Fail);
		return ;
	}
	cr = spFU.Deref().Open(GKC::RefPtr<GKC::CharS>(szFile), iOpenType, iCreateType);
	if( cr.IsFailed() )
		return ;
	//assignment
	sp = rv_forward(spStream);
}

void _MemoryStreamU_CreateU(_UniqueCom& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_UNIQUECOM_INSTANCE(GKC::MemoryStreamU, sp, cr);
}

void _BufferStream_CreateU(const void* p, uintptr uBytes, _UniqueCom& sp, GKC::CallResult& cr) noexcept
{
	_UniqueCom spStream;
	_CREATE_UNIQUECOM_INSTANCE(GKC::BufferStream, spStream, cr);
	if( cr.IsFailed() )
		return ;
	//buffer utility
	GKC::RefPtr<_IBufferUtility> spBU(CALL_UNIQUECOM_TYPECAST(spStream, _IBufferUtility));
	if( spBU.IsNull() ) {
		cr.SetResult(GKC::SystemCallResults::Fail);
		return ;
	}
	cr = spBU.Deref().SetBuffer((uintptr)p, uBytes);
	if( cr.IsFailed() )
		return ;
	//assignment
	sp = rv_forward(spStream);
}

void _TextStreamU_CreateU(_UniqueCom& sp, GKC::CallResult& cr) noexcept
{
	_CREATE_UNIQUECOM_INSTANCE(GKC::TextStreamU, sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
