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

// UniqueCom
typedef _UniqueCom  UniqueCom;

// UniqueComHelper
typedef _UniqueComHelper  UniqueComHelper;

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

	static CallResult GetClassObject(const ConstStringS& strAssembly, const guid& cid, UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_UniqueCom_SA_GetClassObject(strAssembly, cid, sp, cr);
		return cr;
	}
};

// IByteSequentialStream
typedef _IByteSequentialStream  IByteSequentialStream;

#define GUID_IByteSequentialStream  GUID__IByteSequentialStream

// IByteStream
typedef _IByteStream  IByteStream;

#define GUID_IByteStream  GUID__IByteStream

// IStreamLocker
typedef _IStreamLocker  IStreamLocker;

#define GUID_IStreamLocker  GUID__IStreamLocker

// IFileUtility
typedef _IFileUtility  IFileUtility;

#define GUID_IFileUtility  GUID__IFileUtility

// IMemoryUtility
typedef _IMemoryUtility  IMemoryUtility;

#define GUID_IMemoryUtility  GUID__IMemoryUtility

// IMemoryUtilityU
typedef _IMemoryUtilityU  IMemoryUtilityU;

#define GUID_IMemoryUtilityU  GUID__IMemoryUtilityU

// IBufferUtility
typedef _IBufferUtility  IBufferUtility;

#define GUID_IBufferUtility  GUID__IBufferUtility

// BOMTypes
typedef _BOMTypes  BOMTypes;

// CRLFStyles
typedef _CRLFStyles  CRLFStyles;

// ITextStreamRoot
typedef _ITextStreamRoot  ITextStreamRoot;

#define GUID_ITextStreamRoot  GUID__ITextStreamRoot

// ITextStreamString
typedef _ITextStreamString  ITextStreamString;

#define GUID_ITextStreamString  GUID__ITextStreamString

// ITextUtility
typedef _ITextUtility  ITextUtility;

#define GUID_ITextUtility  GUID__ITextUtility

// ITextStreamStringU
typedef _ITextStreamStringU  ITextStreamStringU;

#define GUID_ITextStreamStringU  GUID__ITextStreamStringU

// ITextUtilityU
typedef _ITextUtilityU  ITextUtilityU;

#define GUID_ITextUtilityU  GUID__ITextUtilityU

// StreamHelper

class StreamHelper
{
public:
	// file stream
	/*! \brief Create file stream.

	Create a file stream interface by file name.
	\param strFile [in] The file name. Use platform path prefix.
	\param iOpenType [in] Open type. It can be a value of FileOpenTypes.
	\param iCreateType [in] Creation type. It can be a combination of enum FileCreationTypes.
	\param sp [out] Receive the stream pointer.
	\return A CallResult value.
	*/
	static CallResult CreateFileStream(const ConstStringS& strFile, int iOpenType, int iCreateType, ShareCom<IByteStream>& sp) throw()
	{
		CallResult cr;
		::_FileStream_Create(ConstArrayHelper::GetInternalPointer(strFile), iOpenType, iCreateType, sp, cr);
		return cr;
	}
	// memory stream
	/*! \brief Create a memory stream object.

	Create a memory stream object.
	\param sp [out] Receive the interface pointer of stream.
	\return A CallResult value.
	*/
	static CallResult CreateMemoryStream(ShareCom<IByteStream>& sp) throw()
	{
		CallResult cr;
		::_MemoryStream_Create(sp, cr);
		return cr;
	}
	// buffer stream
	/*! \brief Create a buffer stream object.

	Create a buffer stream object.
	\param pv [in] A pointer to data buffer.
	\param uBytes [in] The bytes of buffer. It can not be 0.
	\param sp [out] Receive the pointer to IByteStream.
	\return A CallResult value.
	*/
	static CallResult CreateBufferStream(const uintptr pv, uintptr uBytes, ShareCom<IByteStream>& sp) throw()
	{
		CallResult cr;
		::_BufferStream_Create((const void*)pv, uBytes, sp, cr);
		return cr;
	}
	// text stream
	/*! \brief Create a text stream.

	Create a text stream.
	\param sp [out] Receive the interface pointer of text stream object.
	\return A CallResult value.
	\note This object is not thread-safe.
	*/
	static CallResult CreateTextStream(ShareCom<ITextStreamRoot>& sp) throw()
	{
		CallResult cr;
		::_TextStream_Create(sp, cr);
		return cr;
	}

	//unique
	static CallResult CreateFileStream(const ConstStringS& strFile, int iOpenType, int iCreateType, UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_FileStream_CreateU(ConstArrayHelper::GetInternalPointer(strFile), iOpenType, iCreateType, sp, cr);
		return cr;
	}
	static CallResult CreateMemoryStream(UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_MemoryStreamU_CreateU(sp, cr);
		return cr;
	}
	static CallResult CreateBufferStream(const uintptr pv, uintptr uBytes, UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_BufferStream_CreateU((const void*)pv, uBytes, sp, cr);
		return cr;
	}
	static CallResult CreateTextStream(UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_TextStreamU_CreateU(sp, cr);
		return cr;
	}

	//variant string
	static void InitializeVariantString(int iBomType, VariantString& str)
	{
		_Initialize_VariantString(iBomType, str);  //may throw
	}

	//copy
	/*! \brief Copy stream.

	Copy stream.
	\param spSrc [in] Source stream interface pointer.
	\param spDest [in] A pointer to the destination stream.
	\return A CallResult value.
	*/
	static CallResult Copy(RefPtr<IByteStream>&& spSrc, RefPtr<IByteStream>&& spDest) throw()
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
	static CallResult Copy(ShareCom<IByteStream>& spSrc, ShareCom<IByteStream>& spDest) throw()
	{
		return Copy(RefPtr<IByteStream>(ShareComHelper::GetInternalPointer(spSrc)), RefPtr<IByteStream>(ShareComHelper::GetInternalPointer(spDest)));
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

// Compression

// PkInputBuffer
typedef _PkInputBuffer  PkInputBuffer;

// PkOutputBuffer
typedef _PkOutputBuffer  PkOutputBuffer;

// IPkCompressor
typedef _IPkCompressor  IPkCompressor;

#define GUID_IPkCompressor  GUID__IPkCompressor

// IPkDecompressor
typedef _IPkDecompressor  IPkDecompressor;

#define GUID_IPkDecompressor  GUID__IPkDecompressor

// CompressionHelper

class CompressionHelper
{
public:
	static CallResult CreatePkzCompressor(UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_PkzCompressor_Create(sp, cr);
		return cr;
	}
	static CallResult CreatePkzDecompressor(UniqueCom& sp) noexcept
	{
		CallResult cr;
		::_PkzDecompressor_Create(sp, cr);
		return cr;
	}
};

// *** for Lite COM ***

// LiteCom<T>
template <class T>
using LiteCom = _LiteCom<T>;

// ILiteComBase
typedef _ILiteComBase  ILiteComBase;

// IFileStream
typedef _IFileStream  IFileStream;

// IMemoryStream
typedef _IMemoryStream  IMemoryStream;

// IBufferStream
typedef _IBufferStream  IBufferStream;

// ITextStream
typedef _ITextStream  ITextStream;

// IPkZip
typedef _IPkZip  IPkZip;

// IPkUnzip
typedef _IPkUnzip  IPkUnzip;

// LiteStreamHelper

class LiteStreamHelper
{
public:
	static LiteCom<IFileStream> CreateFileStream() noexcept
	{
		return _FileStream_CreateL();
	}
	static LiteCom<IMemoryStream> CreateMemoryStream() noexcept
	{
		return _MemoryStream_CreateL();
	}
	static LiteCom<IBufferStream> CreateBufferStream() noexcept
	{
		return _BufferStream_CreateL();
	}
	static LiteCom<ITextStream> CreateTextStream() noexcept
	{
		return _TextStream_CreateL();
	}
};

// LiteCompressionHelper

class LiteCompressionHelper
{
public:
	static LiteCom<IPkZip> CreatePkZip() noexcept
	{
		return _PkZip_CreateL();
	}
	static LiteCom<IPkUnzip> CreatePkUnzip() noexcept
	{
		return _PkUnzip_CreateL();
	}
};

// EnvironmentVariableHelper

class EnvironmentVariableHelper
{
public:
	/*! \brief Get the environment variable.

	Retrieves the contents of the specified variable from the environment block of the calling process.
	\tparam Tstring StringT<Tchar> or UniqueStringT<Tchar>.
	\param strName [in] The name of the environment variable.
	\param strVar [out] Receives the contents of the specified environment variable.
	\return true for succeeded, false for otherwise.
	*/
	template <class Tstring>
	static bool Query(const ConstStringT<typename Tstring::EType>& strName, Tstring& strVar)
	{
		return get_environment_variable(ConstArrayHelper::GetInternalPointer(strName), strVar);  //may throw
	}
	template <typename Tchar>
	static bool Set(const ConstStringT<Tchar>& strName, const ConstStringT<Tchar>& strValue) throw()
	{
		return set_environment_variable(ConstArrayHelper::GetInternalPointer(strName), ConstArrayHelper::GetInternalPointer(strValue));
	}
};

// logger

#define LOGGER_INITIALIZE(cb)  _Logger_SetCallback(cb)
#define LOGGER_OUTPUT(s)  _Logger_Output(__FILE__, __LINE__, (s))

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
