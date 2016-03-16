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

////////////////////////////////////////////////////////////////////////////////

//functions

// file stream

int _FileStream_Create(const GKC::RefPtr<GKC::CharS>& szFile, int iOpenType, int iCreateType, _ShareCom<_IByteStream>& sp) throw()
{
	_ShareCom<_IByteStream> spStream;
	try {
		_ShareCom<GKC::FileStream> spFS(_ShareComHelper::MakeShareCom<GKC::FileStream>(GKC::RefPtr<GKC::IMemoryManager>(_CrtMemoryManager_Get())));
		spStream = CALL_COM_TYPECAST(spFS, GKC::FileStream, _IByteStream);
	}
	catch(GKC::Exception& e) {
		return e.GetResult().GetResult();
	}
	catch(...) {
		return GKC::CallResult(GKC::SystemCallResults::Fail).GetResult();
	}
	if( spStream.IsBlockNull() )
		return GKC::CallResult(GKC::SystemCallResults::Fail).GetResult();
	//file utility
	_ShareCom<_IFileUtility> spFU(CALL_COM_TYPECAST(spStream, _IByteStream, _IFileUtility));
	if( spFU.IsBlockNull() )
		return GKC::CallResult(GKC::SystemCallResults::Fail).GetResult();
	GKC::CallResult cr(spFU.Deref().Open(szFile, iOpenType, iCreateType));
	if( cr.IsFailed() )
		return cr.GetResult();
	//assignment
	sp = spStream;
	return cr.GetResult();
}

////////////////////////////////////////////////////////////////////////////////
