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
#ifndef __BOM_UTILITY_H__
#define __BOM_UTILITY_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Cmd_Bom_Process

inline int _Cmd_Bom_Process(int iBomType, const ConstStringS& strSrc, const ConstStringS& strDest) throw()
{
	CallResult cr;

	//src
	ShareCom<IByteStream> spStream;
	cr = StreamHelper::CreateFileStream(strSrc, FileOpenTypes::Read, 0, spStream);
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
	{
		// BOM
		int iType;
		cr = spText.Deref().CheckBOM(iType);
		if( cr.IsFailed() ) {
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Source File cannot be read!")));
			return 1;
		}
		if( iBomType != BOMTypes::None ) {
			//BOM Add
			if( iType != BOMTypes::None && iType != BOMTypes::Ansi )
				iBomType = iType;
		}
	} //end block

	//dest
	ShareCom<IByteStream> spDest;
	cr = StreamHelper::CreateFileStream(strDest, FileOpenTypes::Write, FileCreationTypes::Create, spDest);
	if( cr.IsFailed() ) {
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: The Destination File cannot be opened!")));
		return 1;
	}
	if( iBomType != BOMTypes::None ) {
		//BOM Add
		spText.Deref().SetStream(spDest);
		spText.Deref().Reset();
		spText.Deref().SetBOM(iBomType);
		cr = spText.Deref().WriteBOM();
		if( cr.IsFailed() ) {
			_Delete_Output_File(strDest, spDest, spText);
			ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: BOM writing failed!")));
			return 1;
		}
	}

	//copy
	cr = StreamHelper::Copy(spStream, spDest);
	if( cr.IsFailed() ) {
		_Delete_Output_File(strDest, spDest, spText);
		ConsoleHelper::WriteLine(DECLARE_TEMP_CONST_STRING(ConstStringS, _S("Error: Copy failed!")));
		return 1;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
