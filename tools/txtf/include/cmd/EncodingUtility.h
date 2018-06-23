/*
** Copyright (c) 2018, Xin YUAN, courses of Zhejiang University
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
#ifndef __ENCODING_UTILITY_H__
#define __ENCODING_UTILITY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// Encoding to Type

inline int _Encoding_To_Type(const ConstStringS& strEncoding, ConstStringS& strCP) throw()
{
	int iType = BOMTypes::None;
	if( ConstStringCompareTrait<ConstStringS>::IsEQ(strEncoding, DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF-8"))) ) {
		iType = BOMTypes::UTF8;
	}
	else if( ConstStringCompareTrait<ConstStringS>::IsEQ(strEncoding, DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF16LE"))) ) {
		iType = BOMTypes::UTF16LE;
	}
	else if( ConstStringCompareTrait<ConstStringS>::IsEQ(strEncoding, DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF16BE"))) ) {
		iType = BOMTypes::UTF16BE;
	}
	else if( ConstStringCompareTrait<ConstStringS>::IsEQ(strEncoding, DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF32LE"))) ) {
		iType = BOMTypes::UTF32LE;
	}
	else if( ConstStringCompareTrait<ConstStringS>::IsEQ(strEncoding, DECLARE_TEMP_CONST_STRING(ConstStringS, _S("UTF32BE"))) ) {
		iType = BOMTypes::UTF32BE;
	}
	else {
		strCP = _Find_CodePage_From_Charset(strEncoding);
		if( strCP.GetCount() != 0 )
			iType = BOMTypes::Ansi;
	} //end if
	return iType;
}

// Check the same encoding

inline bool _Check_Same_Encoding(int iSrcType, const ConstStringS& strSrcEncoding,
								int iDestType, const ConstStringS& strDestEncoding) throw()
{
	if( iSrcType != iDestType )
		return false;
	if( iSrcType == BOMTypes::Ansi ) {
		if( ConstStringCompareTrait<ConstStringS>::IsNE(strSrcEncoding, strDestEncoding) )
			return false;
	}
	return true;
}

// _Convert_String_Encoding

inline void _Convert_String_Encoding(int iSrcType, const ConstStringS& strSrcCP, VariantString& strContent,
									int iDestType, const ConstStringS& strDestCP, VariantString& strConv)
{
	if( iSrcType == BOMTypes::UTF8 ) {
		if( iDestType == BOMTypes::UTF16LE || iDestType == BOMTypes::UTF16BE ) {
			strConv.GetString<StringH>() = CS_U2H(StringUtilHelper::To_ConstString(strContent.GetString<StringA>())).GetV();  //may throw
		}
		else if( iDestType == BOMTypes::UTF32LE || iDestType == BOMTypes::UTF32BE ) {
			strConv.GetString<StringL>() = CS_U2L(StringUtilHelper::To_ConstString(strContent.GetString<StringA>())).GetV();  //may throw
		}
		else {
			//Ansi
			strConv.GetString<StringA>() = CS_U2A(StringUtilHelper::To_ConstString(strContent.GetString<StringA>()), strDestCP).GetV();  //may throw
		}
	}
	else if( iSrcType == BOMTypes::UTF16LE || iSrcType == BOMTypes::UTF16BE ) {
		if( iDestType == BOMTypes::UTF8 ) {
			strConv.GetString<StringA>() = CS_H2U(StringUtilHelper::To_ConstString(strContent.GetString<StringH>())).GetV();  //may throw
		}
		else if( iDestType == BOMTypes::UTF16LE || iDestType == BOMTypes::UTF16BE ) {
			strConv = strContent;
		}
		else if( iDestType == BOMTypes::UTF32LE || iDestType == BOMTypes::UTF32BE ) {
			strConv.GetString<StringL>() = CS_H2L(StringUtilHelper::To_ConstString(strContent.GetString<StringH>())).GetV();  //may throw
		}
		else {
			//Ansi
			strConv.GetString<StringA>() = CS_H2A(StringUtilHelper::To_ConstString(strContent.GetString<StringH>()), strDestCP).GetV();  //may throw
		}
	}
	else if( iSrcType == BOMTypes::UTF32LE || iSrcType == BOMTypes::UTF32BE ) {
		if( iDestType == BOMTypes::UTF8 ) {
			strConv.GetString<StringA>() = CS_L2U(StringUtilHelper::To_ConstString(strContent.GetString<StringL>())).GetV();  //may throw
		}
		else if( iDestType == BOMTypes::UTF16LE || iDestType == BOMTypes::UTF16BE ) {
			strConv.GetString<StringH>() = CS_L2H(StringUtilHelper::To_ConstString(strContent.GetString<StringL>())).GetV();  //may throw
		}
		else if( iDestType == BOMTypes::UTF32LE || iDestType == BOMTypes::UTF32BE ) {
			strConv = strContent;
		}
		else {
			//Ansi
			strConv.GetString<StringA>() = CS_L2A(StringUtilHelper::To_ConstString(strContent.GetString<StringL>()), strDestCP).GetV();  //may throw
		}
	}
	else {
		//Ansi
		if( iDestType == BOMTypes::UTF8 ) {
			strConv.GetString<StringA>() = CS_A2U(StringUtilHelper::To_ConstString(strContent.GetString<StringA>()), strSrcCP).GetV();  //may throw
		}
		else if( iDestType == BOMTypes::UTF16LE || iDestType == BOMTypes::UTF16BE ) {
			strConv.GetString<StringH>() = CS_A2H(StringUtilHelper::To_ConstString(strContent.GetString<StringA>()), strSrcCP).GetV();  //may throw
		}
		else if( iDestType == BOMTypes::UTF32LE || iDestType == BOMTypes::UTF32BE ) {
			strConv.GetString<StringL>() = CS_A2L(StringUtilHelper::To_ConstString(strContent.GetString<StringA>()), strSrcCP).GetV();  //may throw
		}
		else {
			//Ansi
			strConv.GetString<StringA>() = CS_A2A(StringUtilHelper::To_ConstString(strContent.GetString<StringA>()), strSrcCP, strDestCP).GetV();  //may throw
		}
	} //end if
}

// _Cmd_Convert_Encoding

inline int _Cmd_Convert_Encoding(const ConstStringS& strSrcEncoding, const ConstStringS& strDestEncoding,
								const ConstStringS& strSrc, const ConstStringS& strDest)
{
	CallResult cr;
	ConstStringS strSrcCP, strDestCP;
	int iSrcType, iDestType;

	//src type
	iSrcType = _Encoding_To_Type(strSrcEncoding, strSrcCP);
	if( iSrcType == BOMTypes::None ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Invalid source encoding!")));
		return 1;
	}
	//dest type
	iDestType = _Encoding_To_Type(strDestEncoding, strDestCP);
	if( iDestType == BOMTypes::None ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Invalid destination encoding!")));
		return 1;
	}

	//same
	if( _Check_Same_Encoding(iSrcType, strSrcEncoding, iDestType, strDestEncoding) ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source and destination encodings can not be the same!")));
		return 1;
	}

	ShareCom<IByteStream> spStream;
	ShareCom<ITextStream> spText;
	VariantString strContent;

	//src
	cr = StreamHelper::CreateFileStream(strSrc, FileOpenTypes::Read, 0, spStream);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened!")));
		return 1;
	}
	cr = StreamHelper::CreateTextStream(spText);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be opened as a text file!")));
		return 1;
	}
	spText.Deref().SetStream(spStream);
	{
		// BOM
		int iType;
		cr = spText.Deref().CheckBOM(iType);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be read!")));
			return 1;
		}
		if( iType != BOMTypes::None ) {
			assert( iType != BOMTypes::Ansi );
			iSrcType = iType;
			//same
			if( _Check_Same_Encoding(iSrcType, strSrcEncoding, iDestType, strDestEncoding) ) {
				ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The source and destination encodings can not be the same!")));
				return 1;
			}
		}
		else {
			spText.Deref().SetBOM(iSrcType);
		}
		//get string
		StreamHelper::InitializeVariantString(iSrcType, strContent);  //may throw
		cr = spText.Deref().GetAllString(strContent);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be read!")));
			return 1;
		}
	} //end block

	//string conversion
	VariantString strConv;
	StreamHelper::InitializeVariantString(iDestType, strConv);  //may throw

	_Convert_String_Encoding(iSrcType, strSrcCP, strContent,
							iDestType, strDestCP, strConv);  //may throw

	//dest
	ShareCom<IByteStream> spDest;
	cr = StreamHelper::CreateFileStream(strDest, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be opened!")));
		return 1;
	}
	spText.Deref().SetStream(spDest);
	spText.Deref().SetBOM(iDestType);
	cr = spText.Deref().PutString(strConv);
	if( cr.IsFailed() ) {
		_Delete_Output_File(strDest, spDest, spText);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Writing failed!")));
		return 1;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
