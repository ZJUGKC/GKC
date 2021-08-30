/*
** Copyright (c) 2021, Xin YUAN, courses of Zhejiang University
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
This file contains functions.
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __PROCESS_FILE_H__
#define __PROCESS_FILE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// tools

inline bool write_h_file(RefPtr<ITextStreamRoot>& refTS, const ConstStringS& strVar)
{
	StringA strV(CS_S2U(strVar).GetV());  //may throw
	CallResult cr;
	cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, "#ifndef __FTSC_"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(StringUtilHelper::To_ConstString(strV));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "_H__"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, "#define __FTSC_"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(StringUtilHelper::To_ConstString(strV));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "_H__"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, "extern const unsigned char c_"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(StringUtilHelper::To_ConstString(strV));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "[];"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "#endif"));
	if ( cr.IsFailed() )
		return false;
	return true;
}

class byte_to_string
{
private:
	typedef FixedStringT<CharA, 8> _BufferClass;

public:
	explicit byte_to_string(byte v) noexcept
	{
		int ret = value_to_string(FixedArrayHelper::GetInternalPointer(m_str), _BufferClass::c_size, "%d", (int)v);
		if ( ret >= 0 )
			m_str.SetLength(ret);
	}
	ConstStringA GetC() const noexcept
	{
		return StringUtilHelper::To_ConstString(m_str);
	}

private:
	_BufferClass m_str;
};

inline bool write_source_file(RefPtr<IByteStream>& refInput, RefPtr<ITextStreamRoot>& refTS, const ConstStringS& strVar, uintptr uFileSize)
{
	StringA strV(CS_S2U(strVar).GetV());  //may throw
	CallResult cr;
	cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, "#include \""));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(StringUtilHelper::To_ConstString(strV));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, ".h\""));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, "const unsigned char c_"));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutStringA(StringUtilHelper::To_ConstString(strV));
	if ( cr.IsFailed() )
		return false;
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "[]={"));
	if ( cr.IsFailed() )
		return false;
	//length
	{
		byte* p = (byte*)(&uFileSize);
		for ( uintptr i = 0; i < sizeof(uintptr); i ++ ) {
			cr = refTS.Deref().PutStringA(byte_to_string(p[i]).GetC());
			if ( cr.IsFailed() )
				return false;
			cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, ","));
			if ( cr.IsFailed() )
				return false;
		}
		cr = refTS.Deref().PutNewLineA();
		if ( cr.IsFailed() )
			return false;
	} //end block
	//body
	uintptr cur = 0;
	while ( true ) {
		uint uRead;
		byte ch;
		cr = refInput.Deref().Read((uintptr)(&ch), 1, uRead);
		if ( cr.IsFailed() )
			return false;
		if ( uRead != 1 )
			break;
		cr = refTS.Deref().PutStringA(byte_to_string(ch).GetC());
		if ( cr.IsFailed() )
			return false;
		cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, ","));
		if ( cr.IsFailed() )
			return false;
		cur ++;
		if ( (cur % 100) == 0 ) {
			cr = refTS.Deref().PutNewLineA();
			if ( cr.IsFailed() )
				return false;
		}
	}
	if ( cur != uFileSize )
		return false;
	//tail
	cur = (4 - (cur % 4)) % 4;
	for ( uintptr i = 0; i < cur + 3; i ++ ) {
		cr = refTS.Deref().PutStringA(DECLARE_TEMP_CONST_STRING(ConstStringA, "0,"));
		if ( cr.IsFailed() )
			return false;
	}
	cr = refTS.Deref().PutLineA(DECLARE_TEMP_CONST_STRING(ConstStringA, "0};"));
	if ( cr.IsFailed() )
		return false;
	return true;
}

// process file

inline bool _Process_File(const ConstStringS& strSrc, const ConstStringS& strDest, const ConstStringS& strVar, bool bCpp)
{
	CallResult cr;

	UniqueStringS ustrDest;
	UniqueCom upOutput;
	UniqueCom upText;
	RefPtr<IByteStream> refOutput;
	RefPtr<ITextUtilityU> refTU;
	RefPtr<ITextStreamRoot> refTS;

	//input
	UniqueCom upInput;
	RefPtr<IByteStream> refInput;
	cr = StreamHelper::CreateFileStream(strSrc, FileOpenTypes::Read, 0, upInput);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source file cannot be opened!")));
		return false;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(IByteStream, upInput, refInput, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source file cannot be opened!")));
		return false;
	}
	uintptr uFileSize;
	{
		StorageStatus ss;
		cr = refInput.Deref().GetStatus(ss);
		if ( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source file has errors!")));
			return false;
		}
		if ( ss.iSize > (int64)(uint)(Limits<int>::Max) ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source file is too large!")));
			return false;
		}
		uFileSize = (uintptr)(ss.iSize);
	} //end block

	//initialization
	cr = StreamHelper::CreateTextStream(upText);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Initialization failed!")));
		return false;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(ITextUtilityU, upText, refTU, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Initialization failed!")));
		return false;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(ITextStreamRoot, upText, refTS, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Initialization failed!")));
		return false;
	}

	//.h
	StringUtilHelper::MakeString(strDest, ustrDest);  //may throw
	StringUtilHelper::Append(DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".h")), ustrDest);  //may throw
	cr = StreamHelper::CreateFileStream(StringUtilHelper::To_ConstString(ustrDest), FileOpenTypes::Write, FileCreationTypes::Create, upOutput);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The .h file cannot be opened!")));
		return false;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(IByteStream, upOutput, refOutput, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The .h file cannot be opened!")));
		return false;
	}
	refTU.Deref().SetStream(refOutput);
	if ( !write_h_file(refTS, strVar) ) { //may throw
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Cannot write to .h file!")));
		return false;
	}

	//.cpp
	StringUtilHelper::MakeString(strDest, ustrDest);  //may throw
	StringUtilHelper::Append(bCpp ? DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".cpp"))
								: DECLARE_TEMP_CONST_STRING(ConstStringS, _S(".c")),
							ustrDest);  //may throw
	cr = StreamHelper::CreateFileStream(StringUtilHelper::To_ConstString(ustrDest), FileOpenTypes::Write, FileCreationTypes::Create, upOutput);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The c/cpp file cannot be opened!")));
		return false;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(IByteStream, upOutput, refOutput, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The c/cpp file cannot be opened!")));
		return false;
	}
	refTU.Deref().SetStream(refOutput);
	refTS.Deref().Reset();
	if ( !write_source_file(refInput, refTS, strVar, uFileSize) ) { //may throw
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Cannot write to c/cpp file!")));
		return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
