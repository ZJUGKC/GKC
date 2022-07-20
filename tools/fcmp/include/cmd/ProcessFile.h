/*
** Copyright (c) 2022, Xin YUAN, courses of Zhejiang University
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

// process file

inline
int _Process_File(const ConstStringS& strFile1, const ConstStringS& strFile2, bool bCaseIgnore) noexcept
{
	CallResult cr;

	//input1
	UniqueCom upFile1;
	RefPtr<IByteStream> refInput1;
	cr = StreamHelper::CreateFileStream(strFile1, FileOpenTypes::Read, 0, upFile1);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File1 cannot be opened!")));
		return 2;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(IByteStream, upFile1, refInput1, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File1 cannot be opened!")));
		return 2;
	}
	int64 iFileSize1;
	{
		StorageStatus ss;
		cr = refInput1.Deref().GetStatus(ss);
		if ( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File1 has errors!")));
			return 2;
		}
		iFileSize1 = ss.iSize;
	}

	//input2
	UniqueCom upFile2;
	RefPtr<IByteStream> refInput2;
	cr = StreamHelper::CreateFileStream(strFile2, FileOpenTypes::Read, 0, upFile2);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File2 cannot be opened!")));
		return 2;
	}
	_UNIQUECOM_INSTANCE_INTERFACE(IByteStream, upFile2, refInput2, cr);
	if ( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File2 cannot be opened!")));
		return 2;
	}
	int64 iFileSize2;
	{
		StorageStatus ss;
		cr = refInput2.Deref().GetStatus(ss);
		if ( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File2 has errors!")));
			return 2;
		}
		iFileSize2 = ss.iSize;
	}

	//print size
	{
		CharS buffer[1024];
		int ret = value_to_string(buffer, sizeof(buffer) / sizeof(CharS),
					_S("file1: %I64d bytes"), iFileSize1);
		if ( ret >= 0 )
			buffer[ret] = '\0';
		ConsoleHelper::WriteLine(ConstStringS(buffer, ret));
		ret = value_to_string(buffer, sizeof(buffer) / sizeof(CharS),
					_S("file2: %I64d bytes"), iFileSize2);
		if ( ret >= 0 )
			buffer[ret] = '\0';
		ConsoleHelper::WriteLine(ConstStringS(buffer, ret));
	} //end block

	//compare
	int64 iBytes = 0;
	while ( true ) {
		uint uRead;
		//1
		CharA ch1;
		cr = refInput1.Deref().Read((uintptr)(&ch1), 1, uRead);
		if ( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File1 has errors!")));
			return 2;
		}
		if ( uRead != 1 ) {
			if ( iBytes != iFileSize1 ) {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File1 has errors!")));
				return 2;
			}
			break;
		}
		//2
		CharA ch2;
		cr = refInput2.Deref().Read((uintptr)(&ch2), 1, uRead);
		if ( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File2 has errors!")));
			return 2;
		}
		if ( uRead != 1 ) {
			if ( iBytes != iFileSize2 ) {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: File2 has errors!")));
				return 2;
			}
			break;
		}
		//case
		if ( bCaseIgnore ) {
			ch1 = char_upper(ch1);
			ch2 = char_upper(ch2);
		}
		//compare
		if ( ch1 != ch2 ) {
			CharS buffer[1024];
			int ret = value_to_string(buffer, sizeof(buffer) / sizeof(CharS),
						_S("Different position: %I64d [%d] -- [%d]"),
						iBytes, (int)(byte)ch1, (int)(byte)ch2);
			if ( ret >= 0 )
				buffer[ret] = '\0';
			ConsoleHelper::WriteLine(ConstStringS(buffer, ret));
			return 3;
		}
		iBytes ++;
	} //end while

	//size
	if ( iFileSize1 != iFileSize2 ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Result: Different size!")));
		return 3;
	}
	ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Result: Two files are the same!")));
	return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
