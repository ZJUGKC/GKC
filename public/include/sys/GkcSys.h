/*
** Copyright (c) 2013, Xin YUAN, courses of Zhejiang University
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
This file contains GkcSys helper classes.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __GKC_SYS_H__
#define __GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////

#ifndef __GKC_DEF_H__
	#error GkcSys.h requires GkcDef.h to be included first.
#endif

#ifndef __GKC_SYS_DEF_H__
	#error GkcSys.h requires GkcSysDef.h to be included first.
#endif

////////////////////////////////////////////////////////////////////////////////

#include "_GkcSys.h"

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// classes

// MemoryHelper

class MemoryHelper
{
public:
	static RefPtr<IMemoryManager> GetCrtMemoryManager() throw()
	{
		return RefPtr<IMemoryManager>(::_CrtMemoryManager_Get());
	}
};

// SharePtr<T>
template <typename T>
using SharePtr = _SharePtr<T>;

// WeakPtr<T>
template <typename T>
using WeakPtr = _WeakPtr<T>;

// SharePtrHelper
typedef _SharePtrHelper  SharePtrHelper;

// ShareCom<T>
template <class T>
using ShareCom = _ShareCom<T>;

// WeakCom<T>
template <class T>
using WeakCom = _WeakCom<T>;

// ShareComHelper
typedef _ShareComHelper  ShareComHelper;

// ShareArray<T>
template <typename T>
using ShareArray = _ShareArray<T>;

// WeakArray<T>
template <typename T>
using WeakArray = _WeakArray<T>;

// ShareArrayHelper
typedef _ShareArrayHelper  ShareArrayHelper;

// StringT<Tchar>
template <typename Tchar>
using StringT = _StringT<Tchar>;

// String*
typedef _StringA  StringA;
typedef _StringH  StringH;
typedef _StringL  StringL;
typedef _StringS  StringS;
typedef _StringW  StringW;

// StringCompareTrait<T>
template <class T>
using StringCompareTrait = _StringCompareTrait<T>;

// StringCaseIgnoreCompareTrait<T>
template <class T>
using StringCaseIgnoreCompareTrait = _StringCaseIgnoreCompareTrait<T>;

// StringHashTrait<T>
template <class T>
using StringHashTrait = _StringHashTrait<T>;

// StringCaseIgnoreHashTrait<T>
template <class T>
using StringCaseIgnoreHashTrait = _StringCaseIgnoreHashTrait<T>;

// StringHelper
typedef _StringHelper  StringHelper;

// UniqueArray<T>
template <typename T>
using UniqueArray = _UniqueArray<T>;

// UniqueArrayHelper
typedef _UniqueArrayHelper  UniqueArrayHelper;

// UniqueStringT<Tchar>
template <typename Tchar>
using UniqueStringT = _UniqueStringT<Tchar>;

// UniqueString*
typedef _UniqueStringA  UniqueStringA;
typedef _UniqueStringH  UniqueStringH;
typedef _UniqueStringL  UniqueStringL;
typedef _UniqueStringS  UniqueStringS;
typedef _UniqueStringW  UniqueStringW;

// UniqueStringCompareTrait<T>
template <class T>
using UniqueStringCompareTrait = _UniqueStringCompareTrait<T>;

// UniqueStringCaseIgnoreCompareTrait<T>
template <class T>
using UniqueStringCaseIgnoreCompareTrait = _UniqueStringCaseIgnoreCompareTrait<T>;

// UniqueStringHashTrait<T>
template <class T>
using UniqueStringHashTrait = _UniqueStringHashTrait<T>;

// UniqueStringCaseIgnoreHashTrait<T>
template <class T>
using UniqueStringCaseIgnoreHashTrait = _UniqueStringCaseIgnoreHashTrait<T>;

// UniqueStringHelper
typedef _UniqueStringHelper  UniqueStringHelper;

// StringOpHelper
typedef _StringOpHelper  StringOpHelper;

// ArrayUtilHelper
typedef _ArrayUtilHelper  ArrayUtilHelper;

// StringUtilHelper
typedef _StringUtilHelper  StringUtilHelper;

// VariantString
typedef _VariantString  VariantString;

// IComFactory
typedef _IComFactory  IComFactory;

#define GUID_IComFactory  GUID__IComFactory

// IComSA
typedef _IComSA  IComSA;

#define GUID_IComSA  GUID__IComSA

// ComponentClientHelper

class ComponentClientHelper
{
public:
	static CallResult GetClassObject(const StringS& strAssembly, const guid& cid, ShareCom<IComFactory>& sp) throw()
	{
		CallResult cr;
		::_Com_SA_GetClassObject(strAssembly, cid, sp, cr);
		return cr;
	}
	static void FreeUnusedLibraries() throw()
	{
		::_Com_SA_FreeUnusedLibraries();
	}
	static void FreeClassObject(ShareCom<IComFactory>& sp) throw()
	{
		CallResult cr;
		ShareCom<IComSA> spSA;
		_COMPONENT_INSTANCE_INTERFACE(IComFactory, IComSA, sp, spSA, cr);
		assert( cr.IsSucceeded() );
		IComSA* pSA = ShareComHelper::GetInternalPointer(spSA);
		spSA.Release();
		sp.Release();
		//this must be the last call for destroying the factory object correctly when unloading SA
		pSA->LockServer(false);
	}
	static CallResult CreateInstance(const ShareCom<IComFactory>& spCF, const guid& iid, ShareCom<void>& sp) throw()
	{
		return (const_cast<ShareCom<IComFactory>&>(spCF)).Deref().CreateInstance(iid, sp);
	}
	static CallResult CreateInstance(const StringS& strAssembly, const guid& cid, const guid& iid, ShareCom<void>& sp) throw()
	{
		CallResult cr;
		ShareCom<IComFactory> spCF;
		cr = GetClassObject(strAssembly, cid, spCF);
		if( cr.IsFailed() )
			return cr;
		ShareCom<void> spI;
		cr = CreateInstance(spCF, iid, spI);
		FreeClassObject(spCF);
		if( cr.IsFailed() )
			return cr;
		sp = spI;
		return cr;
	}
};

// IByteSequentialStream
typedef _IByteSequentialStream  IByteSequentialStream;

#define GUID_IByteSequentialStream  GUID__IByteSequentialStream

// IByteStream
typedef _IByteStream  IByteStream;

#define GUID_IByteStream  GUID__IByteStream

// IFileUtility
typedef _IFileUtility  IFileUtility;

#define GUID_IFileUtility  GUID__IFileUtility

// IMemoryUtility
typedef _IMemoryUtility  IMemoryUtility;

#define GUID_IMemoryUtility  GUID__IMemoryUtility

// IBufferUtility
typedef _IBufferUtility  IBufferUtility;

#define GUID_IBufferUtility  GUID__IBufferUtility

// BOMTypes
typedef _BOMTypes  BOMTypes;

// CRLFStyles
typedef _CRLFStyles  CRLFStyles;

// ITextStream
typedef _ITextStream  ITextStream;

#define GUID_ITextStream  GUID__ITextStream

// StreamHelper

class StreamHelper
{
public:
	// file stream
	//   use platform path prefix
	static CallResult CreateFileStream(const ConstStringS& strFile, int iOpenType, int iCreateType, ShareCom<IByteStream>& sp) throw()
	{
		CallResult cr;
		::_FileStream_Create(ConstArrayHelper::GetInternalPointer(strFile), iOpenType, iCreateType, sp, cr);
		return cr;
	}
	// memory stream
	static CallResult CreateMemoryStream(ShareCom<IByteStream>& sp) throw()
	{
		CallResult cr;
		::_MemoryStream_Create(sp, cr);
		return cr;
	}
	// buffer stream
	static CallResult CreateBufferStream(const uintptr pv, uintptr uBytes, ShareCom<IByteStream>& sp) throw()
	{
		CallResult cr;
		::_BufferStream_Create((const void*)pv, uBytes, sp, cr);
		return cr;
	}
	// text stream
	//   this object is not thread-safe
	static CallResult CreateTextStream(ShareCom<ITextStream>& sp) throw()
	{
		CallResult cr;
		::_TextStream_Create(sp, cr);
		return cr;
	}

	//variant string
	static void InitializeVariantString(int iBomType, VariantString& str)
	{
		_Initialize_VariantString(iBomType, str);  //may throw
	}

	//copy
	static CallResult Copy(ShareCom<IByteStream>& spSrc, ShareCom<IByteStream>& spDest) throw()
	{
		CallResult cr;
		const uint c_buffer_size = 4096;
		byte buffer[c_buffer_size];
		while( true ) {
			uint uRead;
			cr = spSrc.Deref().Read((uintptr)buffer, c_buffer_size, uRead);
			if( cr.IsFailed() )
				return cr;
			//file end
			if( uRead == 0 )
				break;
			uint uWritten;
			cr = spDest.Deref().Write((uintptr)buffer, uRead, uWritten);
			if( cr.IsFailed() )
				return cr;
			if( uWritten != uRead ) {
				cr.SetResult(SystemCallResults::DiskFull);
				return cr;
			}
		} //end while
		return cr;
	}
};

// Help Authoring

// HelpLanguageInfo
typedef _HelpLanguageInfo  HelpLanguageInfo;

// HelpAuthoringHelper

class HelpAuthoringHelper
{
public:
	static bool FindCodePageFromCharset(const GKC::ConstStringS& strCharset, GKC::ConstStringS& strCodePage) throw()
	{
		return ::_HelpAuthoring_FindCodePageFromCharset(strCharset, strCodePage);
	}
	static bool FindLCID(uint uLCID, HelpLanguageInfo& info) throw()
	{
		return ::_HelpAuthoring_FindLCID(uLCID, info);
	}
	static bool FindShortString(const GKC::ConstStringS& strShortString, HelpLanguageInfo& info) throw()
	{
		return ::_HelpAuthoring_FindShortString(strShortString, info);
	}
};

// EnvironmentVariableHelper

class EnvironmentVariableHelper
{
public:
	template <typename Tchar>
	static bool Query(const ConstStringT<Tchar>& strName, StringT<Tchar>& strVar)
	{
		return get_environment_variable(ConstArrayHelper::GetInternalPointer(strName), strVar);  //may throw
	}
	template <typename Tchar>
	static bool Set(const ConstStringT<Tchar>& strName, const ConstStringT<Tchar>& strValue) throw()
	{
		return set_environment_variable(ConstArrayHelper::GetInternalPointer(strName), ConstArrayHelper::GetInternalPointer(strValue));
	}
};

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// The macros of environment variables

#define ENVVAR_GKC_SYSTEM_ROOT        _S("GKC_SYSTEM_ROOT")
#define ENVVAR_GKC_LOCAL_WORKSPACE    _S("GKC_LOCAL_WORKSPACE")
#define ENVVAR_GKC_UNIFIED_WORKSPACE  _S("GKC_UNIFIED_WORKSPACE")

//------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
#endif //__GKC_SYS_H__
////////////////////////////////////////////////////////////////////////////////
