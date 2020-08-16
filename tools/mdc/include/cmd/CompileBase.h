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

// delete output file

inline void _Delete_Output_File(const StringS& str) throw()
{
	thread_sleep(10);
	bool bRet = FileManagementHelper::DeleteFile(StringUtilHelper::To_ConstString(str));
	(void)bRet;
	assert( bRet );
}

// compile one file

inline uintptr _Compile_One_File(ShareCom<IWmarkParser>& spParser, const StringS& strSrc, const StringS& strDest) throw()
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
		ShareCom<ITextStreamRoot> spText;
		cr = StreamHelper::CreateTextStream(spText);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened as a text file!")));
			return 1;
		}
		ShareCom<ITextUtility> spTU;
		_COMPONENT_INSTANCE_INTERFACE(ITextStreamRoot, ITextUtility, spText, spTU, cr);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Cannot process the Source File!")));
			return 1;
		}
		spTU.Deref().SetStream(spStream);
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

	//loop

	cr = spParser.Deref().Start();
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The parser cannot start!")));
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

	//generate
	{
		ShareCom<IByteStream> spStream;
		cr = StreamHelper::CreateFileStream(StringUtilHelper::To_ConstString(strDest), FileOpenTypes::Write, FileCreationTypes::Create, spStream);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be opened!")));
			return 1;
		}
		ShareCom<ITextStreamRoot> spText;
		cr = StreamHelper::CreateTextStream(spText);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be opened as a text file!")));
			return 1;
		}
		ShareCom<ITextUtility> spTU;
		_COMPONENT_INSTANCE_INTERFACE(ITextStreamRoot, ITextUtility, spText, spTU, cr);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Cannot process the Destination File!")));
			return 1;
		}
		spTU.Deref().SetStream(spStream);
		cr = _Generate_Html(spMeta, spText);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be written!")));
			return 1;
		}
	} //end block

	return 0;
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
