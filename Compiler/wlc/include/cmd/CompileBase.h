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
Internal Header
*/

////////////////////////////////////////////////////////////////////////////////
#ifndef __COMPILE_BASE_H__
#define __COMPILE_BASE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// compile one file

inline uintptr _Compile_One_File(ShareCom<IWlangParser>& spParser, const StringS& strSrc, const StringS& strDest) throw()
{
	CallResult cr;

	//file name
	ConsoleHelper::WriteLine(strSrc);

	//output
	ShareCom<ICplMetaData> spMeta;
	cr = CplMetaDataHelper::CreateCplMetaData(spMeta);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Initialization Failed!")));
		return 1;
	}

	//source file
	{
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateFileStream(StringUtilHelper::To_ConstString(strSrc), FileOpenTypes::Read, 0, spStream);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened!")));
			return 1;
		}
		ShareCom<ITextStream> spText;
		cr = StreamHelper::CreateTextStream(spText);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened as a text file!")));
			return 1;
		}
		spText.Deref().SetStream(spStream);
		// BOM
		int iBOMType;
		cr = spText.Deref().CheckBOM(iBOMType);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File must have a BOM with UTF8!")));
			return 1;
		}
		if( iBOMType != BOMTypes::UTF8 ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File must have a BOM with UTF8!")));
			return 1;
		}

		//parser
		spParser.Deref().SetInput(spText);
		spParser.Deref().SetOutput(spMeta);
	} //end block

	cr = spParser.Deref().Start();
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Parser cannot start!")));
		return 1;
	}

	while( true ) {
		cr = spParser.Deref().Parse();
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Parsing Failed!")));
			break;
		}
		if( cr.GetResult() == SystemCallResults::S_False )
			break;
		thread_sleep(1);
	}

	//errors
	ShareArray<StringS> arrError(spParser.Deref().get_ErrorArray());
	uintptr uCount = arrError.GetCount();
	if( uCount != 0 ) {
		ConsoleHelper::SetTextAttribute(STDOUT_ATTR_FORE_RED | STDOUT_ATTR_BACK_KEEP);
		auto iter(arrError.GetBegin());
		for( ; iter != arrError.GetEnd(); iter.MoveNext() ) {
			ConsoleHelper::WriteLine(iter.get_Value());
		}
		ConsoleHelper::RestoreTextAttribute();
		return uCount;
	}
	if( cr.IsFailed() )
		return 1;

	//save
	{
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateFileStream(StringUtilHelper::To_ConstString(strDest), FileOpenTypes::Write, FileCreationTypes::Create, spStream);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be opened!")));
			return 1;
		}
		cr = spMeta.Deref().Save(spStream);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Cannot save to the Destination File!")));
			return 1;
		}
	} //end block

	return 0;
}

// add compiled error count

inline uintptr _Add_Compiled_Error_Count(uintptr u1, uintptr u2) throw()
{
	uintptr uRes;
	const uintptr c_Max = (uintptr)(Limits<uint>::Max);
	CallResult cr;
	cr = SafeOperators::Add(u1, u2, uRes);
	if( cr.IsFailed() )
		return c_Max;
	if( uRes > c_Max )
		return c_Max;
	return uRes;
}

// print compiled result

inline void _Print_Compiled_Result(uintptr uCount) throw()
{
	_CplErrorBuffer tmp;
	int ret = value_to_string(FixedArrayHelper::GetInternalPointer(tmp), _CplErrorBuffer::c_size,
							_S("=====  %u errors.  ====="), (uint)uCount);
	if( ret >= 0 )
		tmp.SetLength(ret);
	ConsoleHelper::WriteLine(tmp);
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
