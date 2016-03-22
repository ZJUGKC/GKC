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
#include "stream/BufferStream.h"
#include "stream/TextStream.h"

////////////////////////////////////////////////////////////////////////////////

//functions

namespace GKC {

// file stream
inline void _Internal_FileStream_Create(const GKC::CharS* szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	_ShareCom<_IByteStream> spStream;
	_CREATE_COMPONENT_INSTANCE(FileStream, _IByteStream, spStream, cr)
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

// buffer stream
inline void _Internal_BufferStream_Create(const void* p, uintptr uBytes, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	_ShareCom<_IByteStream> spStream;
	_CREATE_COMPONENT_INSTANCE(BufferStream, _IByteStream, spStream, cr)
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
inline void _Internal_TextStream_Create(_ShareCom<_ITextStream>& sp, GKC::CallResult& cr) throw()
{
	_ShareCom<_ITextStream> spText;
	_CREATE_COMPONENT_INSTANCE(TextStream, _ITextStream, spText, cr)
	if( cr.IsFailed() )
		return ;
	sp = spText;
}

}  //namespace

// file stream

void _FileStream_Create(const GKC::CharS* szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_FileStream_Create(szFile, iOpenType, iCreateType, sp, cr);
}

// buffer stream

void _BufferStream_Create(const void* p, uintptr uBytes, _ShareCom<_IByteStream>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_BufferStream_Create(p, uBytes, sp, cr);
}

// text stream

void _TextStream_Create(_ShareCom<_ITextStream>& sp, GKC::CallResult& cr) throw()
{
	GKC::_Internal_TextStream_Create(sp, cr);
}

////////////////////////////////////////////////////////////////////////////////
