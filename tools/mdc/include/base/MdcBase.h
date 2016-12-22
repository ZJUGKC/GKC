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
#ifndef __MDC_BASE_H__
#define __MDC_BASE_H__
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
namespace GKC {
////////////////////////////////////////////////////////////////////////////////

// _Mdc_Process_File
//   remove BOM and process tables

inline CallResult _Mdc_Process_File(const ShareCom<ITextStream>& sp, ShareCom<ITextStream>& spOut)
{
	CallResult cr;
	ShareCom<ITextStream> spStream(sp);
	//BOM
	int iBOMType;
	cr = spStream.Deref().CheckBOM(iBOMType);
	if( cr.IsFailed() )
		return cr;
	if( iBOMType != BOMTypes::UTF8 && iBOMType != BOMTypes::None ) {
		cr.SetResult(SystemCallResults::Corrupt);
		return cr;
	}
	int64 iBack = 0;
	int iState = 0;
	//loop
	while( true ) {
		CharA ch;
		cr = spStream.Deref().GetCharA(ch);
		if( cr.IsFailed() )
			break;
		if( cr.GetResult() == SystemCallResults::S_EOF ) {
			cr.SetResult(SystemCallResults::OK);
			break;
		}
		//table
		switch( iState ) {
		case 0:
			if( ch == ' ' || ch == '\t' ) {
			}
			else if( ch == '|' ) {
				iBack = 2;
			}
			break;
		case 1:
			break;
		default:
			assert( false );
			break;
		} //end switch
	} //end while
	return cr;
}

////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////
#endif
////////////////////////////////////////////////////////////////////////////////
